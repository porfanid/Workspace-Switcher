# Workspace Switcher

## Overview

**Workspace Switcher** is a C++ program designed to switch between workspaces on both X11 and Wayland display servers. It provides a unified interface for workspace navigation across different desktop environments.

## Features

- Switch to the next workspace.
- Switch to the previous workspace.
- Support for both X11 and Wayland.
- Automatic detection of display server type.
- Support for multiple Wayland compositors (Sway, Hyprland, GNOME, KDE Plasma).

## Requirements

### For X11 Support:
- **X11 Development Libraries**: Required for X11 support (optional if only using Wayland).
- **C++17 or later**: Recommended for compatibility and modern features.

### For Wayland Support:
- **Wayland compositor**: One of the following:
  - Sway (requires `swaymsg`)
  - Hyprland (requires `hyprctl`)
  - GNOME Shell (requires `gdbus`)
  - KDE Plasma (requires `qdbus`)

The program automatically detects whether you're running X11 or Wayland and uses the appropriate method.

## Installation

You can install **Workspace Switcher** using one of the following methods:

### 1. Install from PPA

For Ubuntu users, installing from the PPA is the easiest method. First, add the PPA to your system and then install the package:

```bash
sudo add-apt-repository ppa:porfanid/workspaces
sudo apt-get update
sudo apt-get install workspace-switcher
```

### 2. Build from Source

If you prefer to build the application from source, follow these steps:

#### Install Dependencies

**For X11 and Wayland support:**

Install the necessary development libraries:

```bash
sudo apt-get install libx11-dev libxrandr-dev
```

**For Wayland-only support:**

No additional libraries are required. The program can be built without X11 dependencies and will work with supported Wayland compositors.

#### Clone the Repository

Clone the repository to your local machine:

```bash
git clone https://github.com/porfanid/workspace-switcher.git
cd workspace-switcher
```

#### Build and Install the Program Using CMake

**Local Installation**: Build the program in the local directory:

```bash
mkdir build
cd build
cmake ..
make
```

This will generate the executable named `workspaces` in the local directory.

**Global Installation**: To install the program globally:

```bash
mkdir build
cd build
cmake ..
make install
```

This will install the executable named `workspaces` to your global system directories.

## Usage

Run the program with either `next` or `prev` as an argument to switch to the next or previous workspace, respectively. The program automatically detects your display server (X11 or Wayland) and uses the appropriate method.

### For X11:

```bash
./workspaces next
./workspaces prev
```

### For Wayland:

The same commands work on Wayland:

```bash
./workspaces next
./workspaces prev
```

**Supported Wayland Compositors:**
- **Sway**: Uses `swaymsg` to switch workspaces
- **Hyprland**: Uses `hyprctl` to switch workspaces
- **GNOME Shell**: Uses `gdbus` to switch workspaces
- **KDE Plasma**: Uses `qdbus` to switch workspaces

#### Demo

Here is a demonstration of the Workspace Switcher in action:



https://github.com/user-attachments/assets/cbda1d2b-cc11-4164-9144-d610973773fe



https://github.com/user-attachments/assets/13ab7ee9-1896-4bfc-9b22-c990d8027298


## How It Works

The program automatically detects your display server and uses the appropriate method for workspace switching:

### X11 Mode

When running on X11, the program performs the following steps:

1. **Open Display**: Connects to the X server.
2. **Get Root Window**: Retrieves the root window for the current display.
3. **Get Number of Workspaces**: Queries the number of available workspaces.
4. **Get Current Workspace**: Retrieves the ID of the current workspace.
5. **Calculate Target Workspace**: Determines which workspace to switch to based on user input (`next` or `prev`).
6. **Switch Workspace**: Sends an X11 `ClientMessage` event to change the workspace.

### Wayland Mode

When running on Wayland, the program detects the compositor and uses the appropriate command:

- **Sway**: Executes `swaymsg workspace next/prev`
- **Hyprland**: Executes `hyprctl dispatch workspace +1/-1`
- **GNOME**: Uses D-Bus to call GNOME Shell's workspace switching methods
- **KDE Plasma**: Uses `qdbus` to call KWin's nextDesktop/previousDesktop methods

## Contributing

If you'd like to contribute to the project, please follow these steps:

1. **Fork the Repository**: Create a personal copy of the repository on GitHub.
2. **Create a New Branch**: Develop your feature or bug fix in a new branch.
3. **Submit a Pull Request**: Open a pull request with a clear description of your changes.

## Publishing

For maintainers: See [docs/PUBLISHING.md](docs/PUBLISHING.md) for instructions on publishing new releases to the Ubuntu PPA.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contact

For any questions or feedback, please contact:

- **Email**: pavlos[at]orfanidis.net.gr
- **GitHub**: [porfanid](https://github.com/porfanid)

---
