# Workspace Switcher

## Overview

The **Workspace Switcher** is a C++ program designed to switch between workspaces on X11 display servers. It utilizes the Xlib library.

## Features

- Switch to the next or previous workspace.

## Requirements

- **X11 Development Libraries** (for X11 support)
- **C++17 or later** (recommended)

## Installation

1. **Install Dependencies**

   Only for X11 support:
   ```bash
   sudo apt-get install libx11-dev libxrandr-dev
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

## How It Works
The program queries the number of workspaces and the current workspace using Xlib functions and switches workspaces by sending a `ClientMessage` event.

## Contributing

If you want to contribute to the project, please follow these guidelines:

1. Fork the repository.
2. Create a new branch for your feature or bug fix.
3. Submit a pull request with a clear description of the changes.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contact

For any questions or feedback, please contact:

- **Email**: pavlos[at]orfanidis.net.gr
- **GitHub**: [porfanid](https://github.com/porfanid)