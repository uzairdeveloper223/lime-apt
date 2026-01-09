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
    // ==================== GAMING ====================
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
    {
        "heroic", "Heroic Games Launcher",
        PKG_SOURCE_DEB_URL,
        "https://github.com/Heroic-Games-Launcher/HeroicGamesLauncher/releases/latest/download/heroic_amd64.deb",
        NULL, NULL
    },
    {
        "gamemode", "Feral GameMode",
        PKG_SOURCE_PPA,
        "ppa:samoilov-lex/gamemode",
        NULL, NULL
    },
    {
        "mangohud", "MangoHud",
        PKG_SOURCE_PPA,
        "ppa:flexiondotorg/mangohud",
        NULL, NULL
    },
    
    // ==================== BROWSERS ====================
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
    {
        "vivaldi-stable", "Vivaldi",
        PKG_SOURCE_REPO,
        "https://repo.vivaldi.com/archive/deb/",
        "https://repo.vivaldi.com/archive/linux_signing_key.pub",
        "deb [arch=amd64] https://repo.vivaldi.com/archive/deb/ stable main"
    },
    {
        "opera-stable", "Opera",
        PKG_SOURCE_REPO,
        "https://deb.opera.com/opera-stable/",
        "https://deb.opera.com/archive.key",
        "deb [arch=amd64] https://deb.opera.com/opera-stable/ stable non-free"
    },
    {
        "microsoft-edge-stable", "Microsoft Edge",
        PKG_SOURCE_REPO,
        "https://packages.microsoft.com/repos/edge",
        "https://packages.microsoft.com/keys/microsoft.asc",
        "deb [arch=amd64] https://packages.microsoft.com/repos/edge stable main"
    },
    {
        "chromium", "Chromium",
        PKG_SOURCE_PPA,
        "ppa:xtradeb/apps",
        NULL, NULL
    },
    
    // ==================== DEVELOPMENT ====================
    {
        "code", "Visual Studio Code",
        PKG_SOURCE_REPO,
        "https://packages.microsoft.com/repos/code",
        "https://packages.microsoft.com/keys/microsoft.asc",
        "deb [arch=amd64] https://packages.microsoft.com/repos/code stable main"
    },
    {
        "code-insiders", "VS Code Insiders",
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
        "sublime-merge", "Sublime Merge",
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
        "deb [arch=amd64] https://download.docker.com/linux/ubuntu noble stable"
    },
    {
        "github-desktop", "GitHub Desktop",
        PKG_SOURCE_DEB_URL,
        "https://github.com/shiftkey/desktop/releases/download/release-3.3.8-linux1/GitHubDesktop-linux-amd64-3.3.8-linux1.deb",
        NULL, NULL
    },
    {
        "gitkraken", "GitKraken",
        PKG_SOURCE_DEB_URL,
        "https://release.gitkraken.com/linux/gitkraken-amd64.deb",
        NULL, NULL
    },
    {
        "postman", "Postman",
        PKG_SOURCE_DEB_URL,
        "https://dl.pstmn.io/download/latest/linux_64",
        NULL, NULL
    },
    {
        "insomnia", "Insomnia",
        PKG_SOURCE_DEB_URL,
        "https://updates.insomnia.rest/downloads/ubuntu/latest?app=com.insomnia.app",
        NULL, NULL
    },
    {
        "dbeaver-ce", "DBeaver CE",
        PKG_SOURCE_DEB_URL,
        "https://dbeaver.io/files/dbeaver-ce_latest_amd64.deb",
        NULL, NULL
    },
    {
        "mongodb-compass", "MongoDB Compass",
        PKG_SOURCE_DEB_URL,
        "https://downloads.mongodb.com/compass/mongodb-compass_1.42.0_amd64.deb",
        NULL, NULL
    },
    {
        "mysql-workbench-community", "MySQL Workbench",
        PKG_SOURCE_DEB_URL,
        "https://dev.mysql.com/get/Downloads/MySQLGUITools/mysql-workbench-community_8.0.36-1ubuntu24.04_amd64.deb",
        NULL, NULL
    },
    {
        "nodejs", "Node.js LTS",
        PKG_SOURCE_REPO,
        "https://deb.nodesource.com/node_20.x",
        "https://deb.nodesource.com/gpgkey/nodesource-repo.gpg.key",
        "deb [arch=amd64] https://deb.nodesource.com/node_20.x nodistro main"
    },
    {
        "yarn", "Yarn Package Manager",
        PKG_SOURCE_REPO,
        "https://dl.yarnpkg.com/debian/",
        "https://dl.yarnpkg.com/debian/pubkey.gpg",
        "deb https://dl.yarnpkg.com/debian/ stable main"
    },
    {
        "gh", "GitHub CLI",
        PKG_SOURCE_REPO,
        "https://cli.github.com/packages",
        "https://cli.github.com/packages/githubcli-archive-keyring.gpg",
        "deb [arch=amd64] https://cli.github.com/packages stable main"
    },
    {
        "android-studio", "Android Studio",
        PKG_SOURCE_PPA,
        "ppa:maarten-fonville/android-studio",
        NULL, NULL
    },
    
    // ==================== COMMUNICATION ====================
    {
        "slack-desktop", "Slack",
        PKG_SOURCE_DEB_URL,
        "https://downloads.slack-edge.com/releases/linux/4.36.140/prod/x64/slack-desktop-4.36.140-amd64.deb",
        NULL, NULL
    },
    {
        "zoom", "Zoom",
        PKG_SOURCE_DEB_URL,
        "https://zoom.us/client/latest/zoom_amd64.deb",
        NULL, NULL
    },
    {
        "teams-for-linux", "Microsoft Teams",
        PKG_SOURCE_DEB_URL,
        "https://github.com/AryToNeX/AnySnap/releases/download/latest/teams-for-linux_amd64.deb",
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
    {
        "element-desktop", "Element (Matrix)",
        PKG_SOURCE_REPO,
        "https://packages.element.io/debian/",
        "https://packages.element.io/debian/element-io-archive-keyring.gpg",
        "deb [arch=amd64] https://packages.element.io/debian/ default main"
    },
    {
        "skypeforlinux", "Skype",
        PKG_SOURCE_DEB_URL,
        "https://go.skype.com/skypeforlinux-64.deb",
        NULL, NULL
    },
    {
        "viber", "Viber",
        PKG_SOURCE_DEB_URL,
        "https://download.cdn.viber.com/cdn/desktop/Linux/viber.deb",
        NULL, NULL
    },
    {
        "wire-desktop", "Wire",
        PKG_SOURCE_DEB_URL,
        "https://wire-app.wire.com/linux/debian/wire-desktop-latest.deb",
        NULL, NULL
    },
    
    // ==================== MEDIA & CREATIVITY ====================
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
    {
        "vlc", "VLC Media Player",
        PKG_SOURCE_PPA,
        "ppa:videolan/stable-daily",
        NULL, NULL
    },
    {
        "kdenlive", "Kdenlive",
        PKG_SOURCE_PPA,
        "ppa:kdenlive/kdenlive-stable",
        NULL, NULL
    },
    {
        "shotcut", "Shotcut",
        PKG_SOURCE_PPA,
        "ppa:haraldhv/shotcut",
        NULL, NULL
    },
    {
        "audacity", "Audacity",
        PKG_SOURCE_PPA,
        "ppa:ubuntuhandbook1/audacity",
        NULL, NULL
    },
    {
        "gimp", "GIMP",
        PKG_SOURCE_PPA,
        "ppa:ubuntuhandbook1/gimp",
        NULL, NULL
    },
    {
        "inkscape", "Inkscape",
        PKG_SOURCE_PPA,
        "ppa:inkscape.dev/stable",
        NULL, NULL
    },
    {
        "blender", "Blender",
        PKG_SOURCE_PPA,
        "ppa:savoury1/blender",
        NULL, NULL
    },
    {
        "krita", "Krita",
        PKG_SOURCE_PPA,
        "ppa:kritalime/ppa",
        NULL, NULL
    },
    {
        "darktable", "Darktable",
        PKG_SOURCE_PPA,
        "ppa:ubuntuhandbook1/darktable",
        NULL, NULL
    },
    {
        "rawtherapee", "RawTherapee",
        PKG_SOURCE_PPA,
        "ppa:dhor/myway",
        NULL, NULL
    },
    {
        "handbrake", "HandBrake",
        PKG_SOURCE_PPA,
        "ppa:stebbins/handbrake-releases",
        NULL, NULL
    },
    
    // ==================== UTILITIES ====================
    {
        "1password", "1Password",
        PKG_SOURCE_REPO,
        "https://downloads.1password.com/linux/debian/amd64",
        "https://downloads.1password.com/linux/keys/1password.asc",
        "deb [arch=amd64] https://downloads.1password.com/linux/debian/amd64 stable main"
    },
    {
        "bitwarden", "Bitwarden",
        PKG_SOURCE_DEB_URL,
        "https://vault.bitwarden.com/download/?app=desktop&platform=linux&variant=deb",
        NULL, NULL
    },
    {
        "keepassxc", "KeePassXC",
        PKG_SOURCE_PPA,
        "ppa:phoerious/keepassxc",
        NULL, NULL
    },
    {
        "nordvpn", "NordVPN",
        PKG_SOURCE_REPO,
        "https://repo.nordvpn.com/deb/nordvpn/debian",
        "https://repo.nordvpn.com/gpg/nordvpn_public.asc",
        "deb https://repo.nordvpn.com/deb/nordvpn/debian stable main"
    },
    {
        "expressvpn", "ExpressVPN",
        PKG_SOURCE_DEB_URL,
        "https://www.expressvpn.works/clients/linux/expressvpn_3.62.0.4-1_amd64.deb",
        NULL, NULL
    },
    {
        "protonvpn", "ProtonVPN",
        PKG_SOURCE_DEB_URL,
        "https://repo.protonvpn.com/debian/dists/stable/main/binary-all/protonvpn-stable-release_1.0.3-3_all.deb",
        NULL, NULL
    },
    {
        "anydesk", "AnyDesk",
        PKG_SOURCE_DEB_URL,
        "https://download.anydesk.com/linux/anydesk_6.3.0-1_amd64.deb",
        NULL, NULL
    },
    {
        "teamviewer", "TeamViewer",
        PKG_SOURCE_DEB_URL,
        "https://download.teamviewer.com/download/linux/teamviewer_amd64.deb",
        NULL, NULL
    },
    {
        "remmina", "Remmina",
        PKG_SOURCE_PPA,
        "ppa:remmina-ppa-team/remmina-next",
        NULL, NULL
    },
    {
        "dropbox", "Dropbox",
        PKG_SOURCE_DEB_URL,
        "https://www.dropbox.com/download?dl=packages/ubuntu/dropbox_2024.04.17_amd64.deb",
        NULL, NULL
    },
    {
        "insync", "Insync",
        PKG_SOURCE_REPO,
        "http://apt.insync.io/ubuntu",
        "https://d2t3ff60b2tber.cloudfront.net/repomd.xml.key",
        "deb http://apt.insync.io/ubuntu noble non-free contrib"
    },
    {
        "syncthing", "Syncthing",
        PKG_SOURCE_REPO,
        "https://apt.syncthing.net/",
        "https://syncthing.net/release-key.gpg",
        "deb [arch=amd64] https://apt.syncthing.net/ syncthing stable"
    },
    {
        "restic", "Restic Backup",
        PKG_SOURCE_PPA,
        "ppa:restic/stable",
        NULL, NULL
    },
    {
        "timeshift", "Timeshift",
        PKG_SOURCE_PPA,
        "ppa:teejee2008/timeshift",
        NULL, NULL
    },
    {
        "ulauncher", "Ulauncher",
        PKG_SOURCE_PPA,
        "ppa:agornostal/ulauncher",
        NULL, NULL
    },
    {
        "albert", "Albert Launcher",
        PKG_SOURCE_PPA,
        "ppa:nilarimogard/webupd8",
        NULL, NULL
    },
    {
        "flameshot", "Flameshot",
        PKG_SOURCE_PPA,
        "ppa:savoury1/utilities",
        NULL, NULL
    },
    {
        "shutter", "Shutter",
        PKG_SOURCE_PPA,
        "ppa:shutter/ppa",
        NULL, NULL
    },
    {
        "stacer", "Stacer",
        PKG_SOURCE_PPA,
        "ppa:oguzhaninan/stacer",
        NULL, NULL
    },
    {
        "bleachbit", "BleachBit",
        PKG_SOURCE_PPA,
        "ppa:n-muench/programs-ppa",
        NULL, NULL
    },
    
    // ==================== OFFICE & PRODUCTIVITY ====================
    {
        "onlyoffice-desktopeditors", "OnlyOffice",
        PKG_SOURCE_DEB_URL,
        "https://github.com/AryToNeX/AnySnap/releases/download/latest/onlyoffice-desktopeditors_amd64.deb",
        NULL, NULL
    },
    {
        "wps-office", "WPS Office",
        PKG_SOURCE_DEB_URL,
        "https://wdl1.pcfg.cache.wpscdn.com/wpsdl/wpsoffice/download/linux/11698/wps-office_11.1.0.11698.XA_amd64.deb",
        NULL, NULL
    },
    {
        "freeoffice", "FreeOffice",
        PKG_SOURCE_DEB_URL,
        "https://www.softmaker.net/down/softmaker-freeoffice-2021_1060-01_amd64.deb",
        NULL, NULL
    },
    {
        "typora", "Typora",
        PKG_SOURCE_REPO,
        "https://typora.io/linux",
        "https://typora.io/linux/public-key.asc",
        "deb https://typora.io/linux ./"
    },
    {
        "notion-app-enhanced", "Notion",
        PKG_SOURCE_DEB_URL,
        "https://github.com/AryToNeX/AnySnap/releases/download/latest/notion-app-enhanced_amd64.deb",
        NULL, NULL
    },
    {
        "obsidian", "Obsidian",
        PKG_SOURCE_DEB_URL,
        "https://github.com/obsidianmd/obsidian-releases/releases/download/v1.5.3/obsidian_1.5.3_amd64.deb",
        NULL, NULL
    },
    {
        "logseq", "Logseq",
        PKG_SOURCE_DEB_URL,
        "https://github.com/logseq/logseq/releases/latest/download/Logseq-linux-x64.deb",
        NULL, NULL
    },
    {
        "joplin", "Joplin",
        PKG_SOURCE_DEB_URL,
        "https://github.com/AryToNeX/AnySnap/releases/download/latest/joplin_amd64.deb",
        NULL, NULL
    },
    {
        "standardnotes", "Standard Notes",
        PKG_SOURCE_DEB_URL,
        "https://github.com/AryToNeX/AnySnap/releases/download/latest/standard-notes_amd64.deb",
        NULL, NULL
    },
    
    // ==================== SYSTEM & CUSTOMIZATION ====================
    {
        "variety", "Variety Wallpaper",
        PKG_SOURCE_PPA,
        "ppa:variety/stable",
        NULL, NULL
    },
    {
        "plank", "Plank Dock",
        PKG_SOURCE_PPA,
        "ppa:ricotz/docky",
        NULL, NULL
    },
    {
        "conky-all", "Conky",
        PKG_SOURCE_PPA,
        "ppa:teejee2008/foss",
        NULL, NULL
    },
    {
        "neofetch", "Neofetch",
        PKG_SOURCE_PPA,
        "ppa:dawidd0811/neofetch",
        NULL, NULL
    },
    {
        "btop", "Btop++",
        PKG_SOURCE_PPA,
        "ppa:aristocratos/btop",
        NULL, NULL
    },
    {
        "kitty", "Kitty Terminal",
        PKG_SOURCE_PPA,
        "ppa:nilarimogard/webupd8",
        NULL, NULL
    },
    {
        "alacritty", "Alacritty",
        PKG_SOURCE_PPA,
        "ppa:aslatter/ppa",
        NULL, NULL
    },
    {
        "wezterm", "WezTerm",
        PKG_SOURCE_DEB_URL,
        "https://github.com/wez/wezterm/releases/download/nightly/wezterm-nightly.Ubuntu24.04.deb",
        NULL, NULL
    },
    
    // ==================== CLOUD & CONTAINERS ====================
    {
        "kubectl", "Kubernetes kubectl",
        PKG_SOURCE_REPO,
        "https://pkgs.k8s.io/core:/stable:/v1.29/deb/",
        "https://pkgs.k8s.io/core:/stable:/v1.29/deb/Release.key",
        "deb [arch=amd64] https://pkgs.k8s.io/core:/stable:/v1.29/deb/ /"
    },
    {
        "helm", "Helm",
        PKG_SOURCE_REPO,
        "https://baltocdn.com/helm/stable/debian/",
        "https://baltocdn.com/helm/signing.asc",
        "deb [arch=amd64] https://baltocdn.com/helm/stable/debian/ all main"
    },
    {
        "terraform", "Terraform",
        PKG_SOURCE_REPO,
        "https://apt.releases.hashicorp.com",
        "https://apt.releases.hashicorp.com/gpg",
        "deb [arch=amd64] https://apt.releases.hashicorp.com noble main"
    },
    {
        "vagrant", "Vagrant",
        PKG_SOURCE_REPO,
        "https://apt.releases.hashicorp.com",
        "https://apt.releases.hashicorp.com/gpg",
        "deb [arch=amd64] https://apt.releases.hashicorp.com noble main"
    },
    {
        "packer", "Packer",
        PKG_SOURCE_REPO,
        "https://apt.releases.hashicorp.com",
        "https://apt.releases.hashicorp.com/gpg",
        "deb [arch=amd64] https://apt.releases.hashicorp.com noble main"
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
