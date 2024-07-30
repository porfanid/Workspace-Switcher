# Workspace Switcher

## Overview

The **Workspace Switcher** is a C++ program designed to switch between workspaces on both X11 and Wayland display servers. The program detects whether it's running under X11 or Wayland and uses the appropriate method to switch workspaces. For X11, it utilizes the Xlib library, and for Wayland, it uses the `swaymsg` command for the Sway window manager.

## Features

- Switch to the next or previous workspace.
- Automatically detects and adapts to X11 or Wayland display servers.

## Requirements

- **X11 Development Libraries** (for X11 support)
- **Sway** (for Wayland support)
- **C++17 or later** (recommended)

## Installation

1. **Install Dependencies**

   For X11 support:
   ```bash
   sudo apt-get install libx11-dev
   ```

For Wayland support:
   ```bash
   sudo apt-get install sway
   ```

2. **Clone the Repository**

   ```bash
   git clone https://github.com/yourusername/workspace-switcher.git
   cd workspace-switcher
   ```

3. **Build the Program Using CMake**

   Create a build directory, configure the project with CMake, and compile the source code:

   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

   This will generate the executable named `workspaces`.

## Usage

Run the program with either `next` or `prev` as an argument to switch to the next or previous workspace, respectively.

### For X11:

```bash
./workspaces next
./workspaces prev
```

### For Wayland:

Ensure you are running a Wayland session and have Sway installed:

```bash
./workspaces next
./workspaces prev
```

## How It Works

- **X11**: The program queries the number of workspaces and the current workspace using Xlib functions and switches workspaces by sending a `ClientMessage` event.
- **Wayland**: The program uses `swaymsg` to send commands to the Sway window manager to switch workspaces.

## Contributing

If you want to contribute to the project, please follow these guidelines:

1. Fork the repository.
2. Create a new branch for your feature or bug fix.
3. Submit a pull request with a clear description of the changes.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contact

For any questions or feedback, please contact:

- **Email**: your.email@example.com
- **GitHub**: [yourusername](https://github.com/yourusername)