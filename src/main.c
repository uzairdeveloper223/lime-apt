/**
 * lime-apt - LimeOS Package Manager Wrapper
 * 
 * A stylish wrapper around apt that provides enhanced visuals
 * with LimeOS branding, filtered output, and cleaner display.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <pthread.h>

#include "packages.h"

// ANSI color codes
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define DIM         "\033[2m"
#define WHITE       "\033[97m"
#define GRAY        "\033[90m"
#define GREEN       "\033[92m"
#define RED         "\033[91m"
#define YELLOW      "\033[93m"
#define CYAN        "\033[96m"
#define CLEAR_LINE  "\033[2K\r"

// Box drawing characters
#define BOX_TL      "┌"
#define BOX_TR      "┐"
#define BOX_BL      "└"
#define BOX_BR      "┘"
#define BOX_H       "─"
#define BOX_V       "│"

// Status icons
#define CHECK       "✓"
#define CROSS       "✗"
#define ARROW       "→"
#define BULLET      "•"
#define PKG_ICON    "📦"

static int terminal_width = 80;

static int get_terminal_width(void)
{
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        return w.ws_col > 40 ? w.ws_col : 80;
    }
    return 80;
}

static void print_header(void)
{
    int box_width = terminal_width < 50 ? terminal_width - 2 : 48;
    
    printf("\n");
    printf(WHITE BOX_TL);
    for (int i = 0; i < box_width; i++) printf(BOX_H);
    printf(BOX_TR RESET "\n");
    
    const char *title = " 🍋 LimeOS Package Manager";
    int padding = box_width - 26;
    printf(WHITE BOX_V BOLD "%s" RESET WHITE, title);
    for (int i = 0; i < padding; i++) printf(" ");
    printf(BOX_V RESET "\n");
    
    printf(WHITE BOX_BL);
    for (int i = 0; i < box_width; i++) printf(BOX_H);
    printf(BOX_BR RESET "\n\n");
}

static void print_status(const char *message)
{
    printf(CLEAR_LINE DIM "  %s %s..." RESET, BULLET, message);
    fflush(stdout);
}

static void print_status_done(const char *message)
{
    printf(CLEAR_LINE GREEN "  %s" RESET " %s\n", CHECK, message);
}

static void print_success(const char *message)
{
    printf("\n" GREEN CHECK RESET " %s\n", message);
}

static void print_error(const char *message)
{
    printf("\n" RED CROSS RESET " %s\n", message);
}

static void print_info(const char *message)
{
    printf(DIM "  %s" RESET "\n", message);
}

// Parse and display search results beautifully
static void format_search_output(FILE *pipe)
{
    char line[2048];
    char pkg_name[256] = "";
    char pkg_version[128] = "";
    char pkg_arch[64] = "";
    char pkg_desc[1024] = "";
    int is_installed = 0;
    int pkg_count = 0;
    int in_package = 0;
    
    // Skip "Sorting..." and "Full Text Search..." lines
    while (fgets(line, sizeof(line), pipe)) {
        // Remove newline
        line[strcspn(line, "\n")] = 0;
        
        // Skip apt status messages
        if (strstr(line, "Sorting...") || strstr(line, "Full Text Search...")) {
            continue;
        }
        
        // Check if this is a package line (contains /)
        if (strchr(line, '/') && !isspace(line[0])) {
            // Print previous package if exists
            if (in_package && strlen(pkg_name) > 0) {
                printf(BOLD WHITE "  %s" RESET, pkg_name);
                printf(DIM " %s" RESET, pkg_version);
                if (is_installed) {
                    printf(GREEN " [installed]" RESET);
                }
                printf("\n");
                if (strlen(pkg_desc) > 0) {
                    printf(DIM "    %s" RESET "\n", pkg_desc);
                }
                printf("\n");
                pkg_count++;
            }
            
            // Parse new package: name/repo,repo version arch [status]
            char *slash = strchr(line, '/');
            if (slash) {
                *slash = '\0';
                strncpy(pkg_name, line, sizeof(pkg_name) - 1);
                
                // Find version (after space)
                char *rest = slash + 1;
                char *space = strchr(rest, ' ');
                if (space) {
                    *space = '\0';
                    space++;
                    // Skip to version
                    while (*space && !isdigit(*space) && *space != '+') space++;
                    char *ver_end = strchr(space, ' ');
                    if (ver_end) {
                        *ver_end = '\0';
                        strncpy(pkg_version, space, sizeof(pkg_version) - 1);
                    }
                }
                
                is_installed = (strstr(line, "[installed") != NULL) || 
                               (slash && strstr(slash, "[installed") != NULL) ||
                               (strstr(rest, "[installed") != NULL);
            }
            
            pkg_desc[0] = '\0';
            in_package = 1;
        }
        else if (in_package && isspace(line[0])) {
            // This is a description line
            char *desc = line;
            while (*desc && isspace(*desc)) desc++;
            strncpy(pkg_desc, desc, sizeof(pkg_desc) - 1);
        }
    }
    
    // Print last package
    if (in_package && strlen(pkg_name) > 0) {
        printf(BOLD WHITE "  %s" RESET, pkg_name);
        printf(DIM " %s" RESET, pkg_version);
        if (is_installed) {
            printf(GREEN " [installed]" RESET);
        }
        printf("\n");
        if (strlen(pkg_desc) > 0) {
            printf(DIM "    %s" RESET "\n", pkg_desc);
        }
        printf("\n");
        pkg_count++;
    }
    
    if (pkg_count > 0) {
        printf(DIM "  Found %d package(s)" RESET "\n", pkg_count);
    } else {
        printf(YELLOW "  No packages found" RESET "\n");
    }
}

// Parse and display update output minimally
static void format_update_output(FILE *pipe)
{
    char line[2048];
    int hit_count = 0;
    int get_count = 0;
    int ppa_count = 0;
    int upgradable = 0;
    
    print_status("Connecting to repositories");
    
    while (fgets(line, sizeof(line), pipe)) {
        line[strcspn(line, "\n")] = 0;
        
        if (strncmp(line, "Hit:", 4) == 0) {
            hit_count++;
            if (strstr(line, "ppa.launchpad")) ppa_count++;
            print_status("Checking repositories");
        }
        else if (strncmp(line, "Get:", 4) == 0) {
            get_count++;
            print_status("Fetching package lists");
        }
        else if (strstr(line, "Reading package lists")) {
            print_status("Reading package lists");
        }
        else if (strstr(line, "Building dependency tree")) {
            print_status("Building dependency tree");
        }
        else if (strstr(line, "Reading state information")) {
            print_status("Reading state information");
        }
        else if (strstr(line, "packages can be upgraded")) {
            // Extract number
            sscanf(line, "%d", &upgradable);
        }
        else if (strncmp(line, "Fetched", 7) == 0) {
            print_status("Download complete");
        }
    }
    
    printf(CLEAR_LINE); // Clear status line
    
    // Print summary
    print_status_done("Repositories synchronized");
    
    char summary[256];
    snprintf(summary, sizeof(summary), "%d repositories checked, %d updated", 
             hit_count + get_count, get_count);
    print_info(summary);
    
    if (ppa_count > 0) {
        snprintf(summary, sizeof(summary), "%d PPA(s) included", ppa_count);
        print_info(summary);
    }
    
    if (upgradable > 0) {
        printf("\n" YELLOW "  %s %d package(s) can be upgraded" RESET "\n", ARROW, upgradable);
        printf(DIM "    Run 'lime-apt upgrade' to update them" RESET "\n");
    }
}

// Parse and display install/upgrade/remove output cleanly
static void format_install_output(FILE *pipe, const char *action_type)
{
    char line[2048];
    int installed = 0;
    int upgraded = 0;
    int removed = 0;
    int configured = 0;
    int already_newest = 0;
    char newest_pkg[256] = "";
    int autoremove_available = 0;
    
    print_status("Reading package information");
    
    while (fgets(line, sizeof(line), pipe)) {
        line[strcspn(line, "\n")] = 0;
        
        // Handle "already the newest version" message
        if (strstr(line, "is already the newest version") || 
            strstr(line, "already the newest version")) {
            already_newest++;
            // Extract package name
            char *space = strchr(line, ' ');
            if (space) {
                size_t len = space - line;
                if (len < sizeof(newest_pkg) - 1) {
                    strncpy(newest_pkg, line, len);
                    newest_pkg[len] = '\0';
                }
            }
        }
        else if (strstr(line, "Selecting previously unselected") || 
                 strstr(line, "Preparing to unpack")) {
            print_status("Preparing packages");
        }
        else if (strstr(line, "Unpacking")) {
            installed++;
            char msg[256];
            snprintf(msg, sizeof(msg), "Unpacking (%d)", installed);
            print_status(msg);
        }
        else if (strstr(line, "Setting up")) {
            configured++;
            char msg[256];
            snprintf(msg, sizeof(msg), "Configuring (%d)", configured);
            print_status(msg);
        }
        else if (strstr(line, "Removing")) {
            removed++;
            char msg[256];
            snprintf(msg, sizeof(msg), "Removing (%d)", removed);
            print_status(msg);
        }
        else if (strstr(line, "upgraded") && strstr(line, "newly installed")) {
            // Parse the summary line: "X upgraded, Y newly installed, Z to remove"
            int up = 0, inst = 0, rem = 0;
            sscanf(line, "%d upgraded, %d newly installed, %d to remove", &up, &inst, &rem);
            upgraded = up;
            installed = inst;
            removed = rem;
        }
        else if (strstr(line, "Processing triggers")) {
            print_status("Processing triggers");
        }
        else if (strstr(line, "packages were automatically installed")) {
            autoremove_available = 1;
        }
    }
    
    printf(CLEAR_LINE);
    
    // Print summary based on what happened
    if (already_newest > 0 && installed == 0 && upgraded == 0) {
        if (strlen(newest_pkg) > 0) {
            print_status_done("Already up to date");
            printf(DIM "  %s is the newest version" RESET "\n", newest_pkg);
        } else {
            print_status_done("Already up to date");
        }
    } else {
        // Show what was done
        if (installed > 0 || configured > 0) {
            char msg[256];
            snprintf(msg, sizeof(msg), "%d package(s) installed", configured > 0 ? configured : installed);
            print_status_done(msg);
        }
        if (upgraded > 0) {
            char msg[256];
            snprintf(msg, sizeof(msg), "%d package(s) upgraded", upgraded);
            print_status_done(msg);
        }
        if (removed > 0) {
            char msg[256];
            snprintf(msg, sizeof(msg), "%d package(s) removed", removed);
            print_status_done(msg);
        }
        if (installed == 0 && upgraded == 0 && removed == 0 && configured == 0 && already_newest == 0) {
            print_status_done("Nothing to do");
        }
    }
    
    if (autoremove_available) {
        printf(DIM "  Run 'lime-apt autoremove' to clean up unused packages" RESET "\n");
    }
    
    (void)action_type; // Suppress unused warning
}


static const char *get_action_name(const char *cmd)
{
    if (strcmp(cmd, "install") == 0) return "Installing";
    if (strcmp(cmd, "remove") == 0) return "Removing";
    if (strcmp(cmd, "purge") == 0) return "Purging";
    if (strcmp(cmd, "update") == 0) return "Updating";
    if (strcmp(cmd, "upgrade") == 0) return "Upgrading";
    if (strcmp(cmd, "full-upgrade") == 0) return "Full Upgrade";
    if (strcmp(cmd, "dist-upgrade") == 0) return "Distribution Upgrade";
    if (strcmp(cmd, "autoremove") == 0) return "Auto Removing";
    if (strcmp(cmd, "search") == 0) return "Searching";
    if (strcmp(cmd, "show") == 0) return "Package Info";
    if (strcmp(cmd, "list") == 0) return "Listing";
    if (strcmp(cmd, "clean") == 0) return "Cleaning";
    if (strcmp(cmd, "autoclean") == 0) return "Auto Cleaning";
    return NULL;
}

static int needs_filtering(const char *cmd)
{
    return (strcmp(cmd, "update") == 0 || 
            strcmp(cmd, "search") == 0 ||
            strcmp(cmd, "install") == 0 ||
            strcmp(cmd, "upgrade") == 0 ||
            strcmp(cmd, "remove") == 0 ||
            strcmp(cmd, "autoremove") == 0);
}

// Check if file has .deb extension
static int is_deb_file(const char *path)
{
    size_t len = strlen(path);
    return len > 4 && strcmp(path + len - 4, ".deb") == 0;
}

// Install a .deb file using dpkg
static int install_deb_file(const char *deb_path)
{
    char cmd[4096];
    char line[2048];
    char pkg_name[256] = "";
    
    // Check if file exists
    if (access(deb_path, F_OK) != 0) {
        print_error("File not found");
        printf(DIM "  %s" RESET "\n", deb_path);
        return 1;
    }
    
    // Get package info from deb file
    snprintf(cmd, sizeof(cmd), "dpkg-deb -f '%s' Package 2>/dev/null", deb_path);
    FILE *info_pipe = popen(cmd, "r");
    if (info_pipe) {
        if (fgets(pkg_name, sizeof(pkg_name), info_pipe)) {
            pkg_name[strcspn(pkg_name, "\n")] = 0;
        }
        pclose(info_pipe);
    }
    
    // Display what we're installing
    if (strlen(pkg_name) > 0) {
        printf(GRAY " " ARROW RESET " " BOLD WHITE "Installing" RESET ": %s\n", pkg_name);
    } else {
        printf(GRAY " " ARROW RESET " " BOLD WHITE "Installing" RESET ": %s\n", deb_path);
    }
    printf(DIM "  From: %s" RESET "\n\n", deb_path);
    
    // Install with dpkg
    print_status("Extracting package");
    
    snprintf(cmd, sizeof(cmd), "dpkg -i '%s' 2>&1", deb_path);
    FILE *pipe = popen(cmd, "r");
    if (!pipe) {
        print_error("Failed to execute dpkg");
        return 1;
    }
    
    int has_errors = 0;
    while (fgets(line, sizeof(line), pipe)) {
        line[strcspn(line, "\n")] = 0;
        
        if (strstr(line, "Selecting previously")) {
            print_status("Preparing package");
        }
        else if (strstr(line, "Unpacking")) {
            print_status("Unpacking package");
        }
        else if (strstr(line, "Setting up")) {
            print_status("Configuring package");
        }
        else if (strstr(line, "Processing triggers")) {
            print_status("Processing triggers");
        }
        else if (strstr(line, "error") || strstr(line, "Error") || 
                 strstr(line, "dpkg: dependency")) {
            has_errors = 1;
        }
    }
    
    int status = pclose(pipe);
    int exit_code = WEXITSTATUS(status);
    
    printf(CLEAR_LINE);
    
    // If there were dependency errors, try to fix them
    if (has_errors || exit_code != 0) {
        printf(YELLOW "  %s Fixing dependencies..." RESET "\n", ARROW);
        
        snprintf(cmd, sizeof(cmd), "apt-get install -f -y 2>&1");
        FILE *fix_pipe = popen(cmd, "r");
        if (fix_pipe) {
            while (fgets(line, sizeof(line), fix_pipe)) {
                line[strcspn(line, "\n")] = 0;
                if (strstr(line, "Setting up")) {
                    print_status("Installing dependencies");
                }
            }
            int fix_status = pclose(fix_pipe);
            printf(CLEAR_LINE);
            
            if (WEXITSTATUS(fix_status) == 0) {
                print_status_done("Package installed with dependencies");
                return 0;
            }
        }
        
        print_error("Installation failed");
        printf(DIM "  Some dependencies could not be resolved" RESET "\n");
        return 1;
    }
    
    print_status_done("Package installed successfully");
    return 0;
}

// Download a file using curl/wget
static int download_file(const char *url, const char *output_path)
{
    char cmd[4096];
    
    // Try curl first, then wget
    snprintf(cmd, sizeof(cmd), 
        "curl -fsSL -o '%s' '%s' 2>/dev/null || wget -q -O '%s' '%s' 2>/dev/null",
        output_path, url, output_path, url);
    
    return system(cmd);
}

// Add a PPA repository
static int add_ppa(const char *ppa)
{
    char cmd[1024];
    print_status("Adding PPA repository");
    
    snprintf(cmd, sizeof(cmd), "add-apt-repository -y '%s' >/dev/null 2>&1", ppa);
    int result = system(cmd);
    
    if (result == 0) {
        print_status_done("PPA added");
        return 0;
    }
    return 1;
}

// Add a custom repository with GPG key
static int add_custom_repo(const char *key_url, const char *repo_line, const char *name)
{
    char cmd[2048];
    char keyring_path[256];
    char list_path[256];
    
    snprintf(keyring_path, sizeof(keyring_path), "/usr/share/keyrings/%s.gpg", name);
    snprintf(list_path, sizeof(list_path), "/etc/apt/sources.list.d/%s.list", name);
    
    // Download and add GPG key
    print_status("Adding repository key");
    snprintf(cmd, sizeof(cmd), 
        "curl -fsSL '%s' | gpg --dearmor -o '%s' 2>/dev/null",
        key_url, keyring_path);
    
    if (system(cmd) != 0) {
        // Try alternative method
        snprintf(cmd, sizeof(cmd),
            "wget -qO- '%s' | gpg --dearmor -o '%s' 2>/dev/null",
            key_url, keyring_path);
        if (system(cmd) != 0) {
            return 1;
        }
    }
    
    // Add repository
    print_status("Adding repository");
    FILE *f = fopen(list_path, "w");
    if (!f) return 1;
    
    // Modify repo line to include signed-by
    if (strstr(repo_line, "[") != NULL) {
        // Already has options, insert signed-by
        char modified_line[1024];
        char *bracket = strstr(repo_line, "]");
        if (bracket) {
            size_t prefix_len = bracket - repo_line;
            strncpy(modified_line, repo_line, prefix_len);
            modified_line[prefix_len] = '\0';
            snprintf(modified_line + prefix_len, sizeof(modified_line) - prefix_len,
                " signed-by=%s%s", keyring_path, bracket);
            fprintf(f, "%s\n", modified_line);
        } else {
            fprintf(f, "%s\n", repo_line);
        }
    } else {
        // Add signed-by option
        fprintf(f, "deb [signed-by=%s] %s\n", keyring_path, repo_line + 4);
    }
    fclose(f);
    
    print_status_done("Repository added");
    return 0;
}

// Download and install a .deb from URL
static int install_deb_from_url(const char *url, const char *pkg_name)
{
    char tmp_path[256];
    snprintf(tmp_path, sizeof(tmp_path), "/tmp/%s.deb", pkg_name);
    
    printf(GRAY " " ARROW RESET " " BOLD WHITE "Downloading" RESET ": %s\n", pkg_name);
    print_status("Downloading package");
    
    if (download_file(url, tmp_path) != 0) {
        print_error("Download failed");
        return 1;
    }
    
    print_status_done("Downloaded");
    
    // Install the downloaded .deb
    return install_deb_file(tmp_path);
}

// Check if a package exists in apt
static int package_exists_in_apt(const char *name)
{
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "apt-cache show '%s' >/dev/null 2>&1", name);
    return system(cmd) == 0;
}

// Install an external package from our database
static int install_external_package(const ExternalPackage *pkg)
{
    printf(CYAN "  📦 Found in LimeOS package database" RESET "\n");
    printf(DIM "     %s" RESET "\n\n", pkg->display_name);
    
    int result = 0;
    
    switch (pkg->type) {
        case PKG_SOURCE_PPA:
            result = add_ppa(pkg->source);
            if (result == 0) {
                print_status("Updating package lists");
                system("apt-get update >/dev/null 2>&1");
                printf(CLEAR_LINE);
                
                // Now install via apt
                char cmd[512];
                snprintf(cmd, sizeof(cmd), "apt-get install -y '%s'", pkg->name);
                print_status("Installing package");
                result = system(cmd);
                printf(CLEAR_LINE);
            }
            break;
            
        case PKG_SOURCE_DEB_URL:
            result = install_deb_from_url(pkg->source, pkg->name);
            break;
            
        case PKG_SOURCE_REPO:
            result = add_custom_repo(pkg->key_url, pkg->repo_line, pkg->name);
            if (result == 0) {
                print_status("Updating package lists");
                system("apt-get update >/dev/null 2>&1");
                printf(CLEAR_LINE);
                
                // Now install via apt
                char cmd[512];
                snprintf(cmd, sizeof(cmd), "apt-get install -y '%s'", pkg->name);
                print_status("Installing package");
                result = system(cmd);
                printf(CLEAR_LINE);
            }
            break;
    }
    
    return result;
}

// Thread data for parallel downloads
typedef struct {
    const char *package_name;
    int result;
    int is_external;
    const ExternalPackage *ext_pkg;
} DownloadTask;

static void print_usage(void)
{
    print_header();
    printf(BOLD "Usage:" RESET " lime-apt <command> [options] [packages]\n\n");
    printf(BOLD "Commands:" RESET "\n");
    printf("  install      Install packages from repositories\n");
    printf("  install-deb  Install a local .deb file\n");
    printf("  remove       Remove packages\n");
    printf("  update       Update package lists\n");
    printf("  upgrade      Upgrade installed packages\n");
    printf("  search       Search for packages\n");
    printf("  show         Show package details\n");
    printf("  autoremove   Remove unused dependencies\n");
    printf("  clean        Clear package cache\n");
    printf("\n" BOLD "Examples:" RESET "\n");
    printf("  lime-apt install firefox\n");
    printf("  lime-apt install-deb ~/Downloads/package.deb\n");
    printf("  lime-apt search vim\n");
    printf("\n" DIM "All apt commands are supported." RESET "\n\n");
}

int main(int argc, char *argv[])
{
    terminal_width = get_terminal_width();
    
    if (argc < 2) {
        print_usage();
        return 0;
    }
    
    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        print_usage();
        return 0;
    }
    
    // Check root for privileged commands
    const char *root_cmds[] = {"install", "install-deb", "remove", "purge", "update", "upgrade", 
                                "full-upgrade", "dist-upgrade", "autoremove", NULL};
    int needs_root = 0;
    for (int i = 0; root_cmds[i] != NULL; i++) {
        if (strcmp(argv[1], root_cmds[i]) == 0) {
            needs_root = 1;
            break;
        }
    }
    
    if (needs_root && geteuid() != 0) {
        print_header();
        print_error("This command requires root privileges");
        printf(DIM "  Try: sudo lime-apt %s ..." RESET "\n\n", argv[1]);
        return 1;
    }
    
    print_header();
    
    // Handle install-deb command specially
    if (strcmp(argv[1], "install-deb") == 0) {
        if (argc < 3) {
            print_error("No .deb file specified");
            printf(DIM "  Usage: lime-apt install-deb <file.deb>" RESET "\n\n");
            return 1;
        }
        
        int result = install_deb_file(argv[2]);
        if (result == 0) {
            print_success("Done");
        }
        printf("\n");
        return result;
    }
    
    // Also support: lime-apt install ./package.deb (auto-detect)
    if (strcmp(argv[1], "install") == 0 && argc > 2 && is_deb_file(argv[2])) {
        int result = install_deb_file(argv[2]);
        if (result == 0) {
            print_success("Done");
        }
        printf("\n");
        return result;
    }
    
    // Smart install: check if packages exist in apt, fallback to external database
    if (strcmp(argv[1], "install") == 0 && argc > 2) {
        int all_found = 1;
        int external_count = 0;
        
        // First pass: check which packages need external sources
        for (int i = 2; i < argc; i++) {
            if (argv[i][0] == '-') continue; // Skip flags
            
            if (!package_exists_in_apt(argv[i])) {
                const ExternalPackage *ext_pkg = find_external_package(argv[i]);
                if (ext_pkg) {
                    external_count++;
                    printf(GRAY " " ARROW RESET " " BOLD WHITE "Installing" RESET ": %s\n", argv[i]);
                    
                    int result = install_external_package(ext_pkg);
                    if (result == 0) {
                        print_status_done("Installed");
                    } else {
                        print_error("Failed to install");
                        all_found = 0;
                    }
                    printf("\n");
                } else {
                    printf(YELLOW "  %s Package '%s' not found in repositories" RESET "\n", CROSS, argv[i]);
                    printf(DIM "    Not in apt or LimeOS package database" RESET "\n\n");
                    all_found = 0;
                }
            }
        }
        
        // If we handled all packages externally, we're done
        if (external_count > 0) {
            // Build list of remaining packages (those in apt)
            int has_apt_packages = 0;
            for (int i = 2; i < argc; i++) {
                if (argv[i][0] == '-') continue;
                if (package_exists_in_apt(argv[i])) {
                    has_apt_packages = 1;
                    break;
                }
            }
            
            if (!has_apt_packages) {
                if (all_found) {
                    print_success("All packages installed");
                }
                printf("\n");
                return all_found ? 0 : 1;
            }
            // Continue to install apt packages below
        }
    }
    
    const char *action = get_action_name(argv[1]);
    
    // Build command string
    char cmd[4096] = "apt ";
    
    // Add -y flag for commands that might ask for confirmation (when filtering output)
    if (needs_filtering(argv[1]) && 
        (strcmp(argv[1], "install") == 0 ||
         strcmp(argv[1], "upgrade") == 0 ||
         strcmp(argv[1], "remove") == 0 ||
         strcmp(argv[1], "autoremove") == 0)) {
        strcat(cmd, "-y ");
    }
    
    for (int i = 1; i < argc; i++) {
        strcat(cmd, argv[i]);
        strcat(cmd, " ");
    }
    strcat(cmd, "2>&1");
    
    // For commands that need filtering, capture output
    if (needs_filtering(argv[1])) {
        if (strcmp(argv[1], "search") == 0) {
            printf(GRAY "  Searching packages...\n\n" RESET);
        }
        
        FILE *pipe = popen(cmd, "r");
        if (!pipe) {
            print_error("Failed to execute apt");
            return 1;
        }
        
        if (strcmp(argv[1], "update") == 0) {
            format_update_output(pipe);
        } else if (strcmp(argv[1], "search") == 0) {
            format_search_output(pipe);
        } else if (strcmp(argv[1], "install") == 0 ||
                   strcmp(argv[1], "upgrade") == 0 ||
                   strcmp(argv[1], "remove") == 0 ||
                   strcmp(argv[1], "autoremove") == 0) {
            // Show what we're doing first
            if (argc > 2 && strcmp(argv[1], "install") == 0) {
                char packages[1024] = "";
                for (int i = 2; i < argc && strlen(packages) < 900; i++) {
                    if (argv[i][0] != '-') {
                        if (strlen(packages) > 0) strcat(packages, ", ");
                        strcat(packages, argv[i]);
                    }
                }
                if (strlen(packages) > 0) {
                    printf(GRAY " " ARROW RESET " " BOLD WHITE "%s" RESET ": %s\n\n", action, packages);
                }
            }
            format_install_output(pipe, argv[1]);
        }
        
        int status = pclose(pipe);
        int exit_code = WEXITSTATUS(status);
        
        if (exit_code == 0 && action) {
            print_success("Done");
        } else if (exit_code != 0) {
            print_error("Operation failed");
        }
        printf("\n");
        return exit_code;
    }
    
    // For other commands, run normally with apt colors
    char **apt_args = malloc(sizeof(char*) * (argc + 3));
    apt_args[0] = "apt";
    int apt_argc = 1;
    apt_args[apt_argc++] = "-o";
    apt_args[apt_argc++] = "APT::Color=1";
    for (int i = 1; i < argc; i++) {
        apt_args[apt_argc++] = argv[i];
    }
    apt_args[apt_argc] = NULL;
    
    // Show action
    if (action != NULL && argc > 2) {
        char packages[1024] = "";
        for (int i = 2; i < argc && strlen(packages) < 900; i++) {
            if (argv[i][0] != '-') {
                if (strlen(packages) > 0) strcat(packages, ", ");
                strcat(packages, argv[i]);
            }
        }
        if (strlen(packages) > 0) {
            printf(GRAY " " ARROW RESET " " BOLD WHITE "%s" RESET ": %s\n\n", action, packages);
        }
    }
    
    pid_t pid = fork();
    if (pid == 0) {
        execvp("apt", apt_args);
        perror("Failed to execute apt");
        exit(1);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            int exit_code = WEXITSTATUS(status);
            if (exit_code == 0 && action) {
                print_success("Done");
            } else if (exit_code != 0) {
                print_error("Operation failed");
            }
            printf("\n");
            free(apt_args);
            return exit_code;
        }
    }
    
    free(apt_args);
    return 0;
}
