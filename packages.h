/**
 * lime-apt Package Database
 * 
 * Contains known external packages with their installation methods
 * (PPAs, direct .deb downloads, custom repos)
 */

#ifndef PACKAGES_H
#define PACKAGES_H

// Package source types
typedef enum {
    PKG_SOURCE_PPA,      // Ubuntu PPA
    PKG_SOURCE_DEB_URL,  // Direct .deb download
    PKG_SOURCE_REPO,     // Custom apt repository
} PackageSourceType;

// External package definition
typedef struct {
    const char *name;           // Package name to search for
    const char *display_name;   // Human-readable name
    PackageSourceType type;     // How to install
    const char *source;         // PPA name, URL, or repo info
    const char *key_url;        // GPG key URL (for repos)
    const char *repo_line;      // Repo line for sources.list
} ExternalPackage;

// Popular packages not in default Ubuntu repos
static const ExternalPackage KNOWN_PACKAGES[] = {
    // Gaming
    {
        "steam", "Steam",
        PKG_SOURCE_DEB_URL,
        "https://cdn.cloudflare.steamstatic.com/client/installer/steam.deb",
        NULL, NULL
    },
    {
        "discord", "Discord",
        PKG_SOURCE_DEB_URL,
        "https://discord.com/api/download?platform=linux&format=deb",
        NULL, NULL
    },
    {
        "lutris", "Lutris",
        PKG_SOURCE_PPA,
        "ppa:lutris-team/lutris",
        NULL, NULL
    },
    
    // Browsers
    {
        "google-chrome-stable", "Google Chrome",
        PKG_SOURCE_REPO,
        "https://dl.google.com/linux/chrome/deb/",
        "https://dl.google.com/linux/linux_signing_key.pub",
        "deb [arch=amd64] https://dl.google.com/linux/chrome/deb/ stable main"
    },
    {
        "brave-browser", "Brave Browser",
        PKG_SOURCE_REPO,
        "https://brave-browser-apt-release.s3.brave.com/",
        "https://brave-browser-apt-release.s3.brave.com/brave-browser-archive-keyring.gpg",
        "deb [arch=amd64] https://brave-browser-apt-release.s3.brave.com/ stable main"
    },
    
    // Development
    {
        "code", "Visual Studio Code",
        PKG_SOURCE_REPO,
        "https://packages.microsoft.com/repos/code",
        "https://packages.microsoft.com/keys/microsoft.asc",
        "deb [arch=amd64] https://packages.microsoft.com/repos/code stable main"
    },
    {
        "sublime-text", "Sublime Text",
        PKG_SOURCE_REPO,
        "https://download.sublimetext.com/",
        "https://download.sublimetext.com/sublimehq-pub.gpg",
        "deb https://download.sublimetext.com/ apt/stable/"
    },
    {
        "docker-ce", "Docker CE",
        PKG_SOURCE_REPO,
        "https://download.docker.com/linux/ubuntu",
        "https://download.docker.com/linux/ubuntu/gpg",
        "deb [arch=amd64] https://download.docker.com/linux/ubuntu jammy stable"
    },
    
    // Communication
    {
        "slack-desktop", "Slack",
        PKG_SOURCE_DEB_URL,
        "https://downloads.slack-edge.com/releases/linux/4.35.126/prod/x64/slack-desktop-4.35.126-amd64.deb",
        NULL, NULL
    },
    {
        "zoom", "Zoom",
        PKG_SOURCE_DEB_URL,
        "https://zoom.us/client/latest/zoom_amd64.deb",
        NULL, NULL
    },
    {
        "teams", "Microsoft Teams",
        PKG_SOURCE_DEB_URL,
        "https://packages.microsoft.com/repos/ms-teams/pool/main/t/teams/teams_1.5.00.23861_amd64.deb",
        NULL, NULL
    },
    {
        "telegram-desktop", "Telegram",
        PKG_SOURCE_PPA,
        "ppa:atareao/telegram",
        NULL, NULL
    },
    {
        "signal-desktop", "Signal",
        PKG_SOURCE_REPO,
        "https://updates.signal.org/desktop/apt",
        "https://updates.signal.org/desktop/apt/keys.asc",
        "deb [arch=amd64] https://updates.signal.org/desktop/apt xenial main"
    },
    
    // Media
    {
        "spotify-client", "Spotify",
        PKG_SOURCE_REPO,
        "http://repository.spotify.com",
        "https://download.spotify.com/debian/pubkey_6224F9941A8AA6D1.gpg",
        "deb http://repository.spotify.com stable non-free"
    },
    {
        "obs-studio", "OBS Studio",
        PKG_SOURCE_PPA,
        "ppa:obsproject/obs-studio",
        NULL, NULL
    },
    
    // Utilities
    {
        "1password", "1Password",
        PKG_SOURCE_REPO,
        "https://downloads.1password.com/linux/debian/amd64",
        "https://downloads.1password.com/linux/keys/1password.asc",
        "deb [arch=amd64] https://downloads.1password.com/linux/debian/amd64 stable main"
    },
    {
        "nordvpn", "NordVPN",
        PKG_SOURCE_REPO,
        "https://repo.nordvpn.com/deb/nordvpn/debian",
        "https://repo.nordvpn.com/gpg/nordvpn_public.asc",
        "deb https://repo.nordvpn.com/deb/nordvpn/debian stable main"
    },
};

#define KNOWN_PACKAGES_COUNT (sizeof(KNOWN_PACKAGES) / sizeof(KNOWN_PACKAGES[0]))

// Find a package in the known packages database
static const ExternalPackage* find_external_package(const char *name)
{
    for (size_t i = 0; i < KNOWN_PACKAGES_COUNT; i++) {
        if (strcmp(KNOWN_PACKAGES[i].name, name) == 0) {
            return &KNOWN_PACKAGES[i];
        }
    }
    return NULL;
}

#endif // PACKAGES_H
