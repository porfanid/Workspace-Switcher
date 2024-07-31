# Workspace Switcher

## Overview

**Workspace Switcher** is a C++ program designed to switch between workspaces on X11 display servers. Utilizing the Xlib library, it allows users to navigate between workspaces with ease.

## Features

- Switch to the next workspace.
- Switch to the previous workspace.

## Requirements

- **X11 Development Libraries**: Required for X11 support.
- **C++17 or later**: Recommended for compatibility and modern features.

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

Install the necessary development libraries:

```bash
sudo apt-get install libx11-dev libxrandr-dev
```

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

Run the program with either `next` or `prev` as an argument to switch to the next or previous workspace, respectively.

### For X11:

```bash
./workspaces next
./workspaces prev
```

#### Demo

Here is a demonstration of the Workspace Switcher in action:

<video src="./demo/workspaces_next.mp4"></video>

<video src="./demo/workspaces_prev.mp4"></video>

## How It Works

The program performs the following steps to switch workspaces:

1. **Open Display**: Connects to the X server.
2. **Get Root Window**: Retrieves the root window for the current display.
3. **Get Number of Workspaces**: Queries the number of available workspaces.
4. **Get Current Workspace**: Retrieves the ID of the current workspace.
5. **Calculate Target Workspace**: Determines which workspace to switch to based on user input (`next` or `prev`).
6. **Switch Workspace**: Sends an X11 `ClientMessage` event to change the workspace.

## Contributing

If you'd like to contribute to the project, please follow these steps:

1. **Fork the Repository**: Create a personal copy of the repository on GitHub.
2. **Create a New Branch**: Develop your feature or bug fix in a new branch.
3. **Submit a Pull Request**: Open a pull request with a clear description of your changes.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contact

For any questions or feedback, please contact:

- **Email**: pavlos[at]orfanidis.net.gr
- **GitHub**: [porfanid](https://github.com/porfanid)

---
