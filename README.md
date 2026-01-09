<picture>
  <source media="(prefers-color-scheme: dark)" srcset=".assets/repository_banner/dark_mode.png">
  <source media="(prefers-color-scheme: light)" srcset=".assets/repository_banner/light_mode.png">
  <img alt="LimeOS Banner" src=".assets/repository_banner/light_mode.png">
</picture>

######

# lime-apt

A stylish wrapper around `apt` that provides enhanced visuals with LimeOS branding.

## Features

- 🍋 **LimeOS branding** - Clean header with distro identity
- ✨ **Action labels** - Clear indication of what's happening
- ✓ **Status indicators** - Success/error feedback with icons
- 🎨 **Monochrome theme** - Matches LimeOS aesthetic

## Building

```bash
make
```

## Usage

```bash
# Install packages
sudo lime-apt install firefox

# Update package lists
sudo lime-apt update

# Search for packages
lime-apt search vim

# All apt commands work the same
lime-apt show neofetch
```

## License

GPL-3.0 License - See `LICENSE` for details.
