#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <string>

#ifdef HAVE_X11
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#endif

// Function to print usage information
void usage(const char *progname) {
    fprintf(stderr, "Usage: %s [next|prev]\n", progname);
    exit(1);
}

// Enum for display server type
enum DisplayServer {
    X11_SERVER,
    WAYLAND_SERVER,
    UNKNOWN_SERVER
};

// Enum for Wayland compositor
enum WaylandCompositor {
    SWAY,
    HYPRLAND,
    GNOME,
    KDE,
    UNKNOWN_COMPOSITOR
};

// Detect the display server type
DisplayServer detectDisplayServer() {
    const char* wayland_display = getenv("WAYLAND_DISPLAY");
    const char* xdg_session_type = getenv("XDG_SESSION_TYPE");
    
    if (wayland_display != NULL || (xdg_session_type != NULL && strcmp(xdg_session_type, "wayland") == 0)) {
        return WAYLAND_SERVER;
    }
    
    const char* display = getenv("DISPLAY");
    if (display != NULL) {
        return X11_SERVER;
    }
    
    return UNKNOWN_SERVER;
}

// Detect Wayland compositor
WaylandCompositor detectWaylandCompositor() {
    const char* xdg_current_desktop = getenv("XDG_CURRENT_DESKTOP");
    const char* desktop_session = getenv("DESKTOP_SESSION");
    
    // Check for Sway
    if (system("command -v swaymsg >/dev/null 2>&1") == 0) {
        return SWAY;
    }
    
    // Check for Hyprland
    if (getenv("HYPRLAND_INSTANCE_SIGNATURE") != NULL || 
        system("command -v hyprctl >/dev/null 2>&1") == 0) {
        return HYPRLAND;
    }
    
    // Check for GNOME
    if ((xdg_current_desktop != NULL && strstr(xdg_current_desktop, "GNOME") != NULL) ||
        (desktop_session != NULL && strstr(desktop_session, "gnome") != NULL)) {
        return GNOME;
    }
    
    // Check for KDE
    if ((xdg_current_desktop != NULL && strstr(xdg_current_desktop, "KDE") != NULL) ||
        (desktop_session != NULL && strstr(desktop_session, "plasma") != NULL)) {
        return KDE;
    }
    
    return UNKNOWN_COMPOSITOR;
}

// Switch workspace on Wayland
int switchWorkspaceWayland(const char* direction) {
    WaylandCompositor compositor = detectWaylandCompositor();
    
    switch (compositor) {
        case SWAY: {
            // Use swaymsg to switch workspace
            std::string cmd = "swaymsg workspace ";
            if (strcmp(direction, "next") == 0) {
                cmd += "next";
            } else {
                cmd += "prev";
            }
            int result = system(cmd.c_str());
            return (result == 0) ? 0 : 1;
        }
        
        case HYPRLAND: {
            // Use hyprctl to switch workspace
            std::string cmd = "hyprctl dispatch workspace ";
            if (strcmp(direction, "next") == 0) {
                cmd += "+1";
            } else {
                cmd += "-1";
            }
            int result = system(cmd.c_str());
            return (result == 0) ? 0 : 1;
        }
        
        case GNOME: {
            // Use gdbus to switch workspace in GNOME
            std::string cmd = "gdbus call --session --dest org.gnome.Shell "
                            "--object-path /org/gnome/Shell "
                            "--method org.gnome.Shell.Eval '";
            if (strcmp(direction, "next") == 0) {
                cmd += "global.workspace_manager.get_active_workspace().get_neighbor(Meta.MotionDirection.RIGHT).activate(global.get_current_time())";
            } else {
                cmd += "global.workspace_manager.get_active_workspace().get_neighbor(Meta.MotionDirection.LEFT).activate(global.get_current_time())";
            }
            cmd += "' >/dev/null 2>&1";
            int result = system(cmd.c_str());
            return (result == 0) ? 0 : 1;
        }
        
        case KDE: {
            // Use qdbus to switch workspace in KDE Plasma
            std::string cmd = "qdbus org.kde.KWin /KWin org.kde.KWin.";
            if (strcmp(direction, "next") == 0) {
                cmd += "nextDesktop";
            } else {
                cmd += "previousDesktop";
            }
            cmd += " >/dev/null 2>&1";
            int result = system(cmd.c_str());
            return (result == 0) ? 0 : 1;
        }
        
        default:
            fprintf(stderr, "Unsupported Wayland compositor. Supported compositors: Sway, Hyprland, GNOME, KDE Plasma\n");
            return 1;
    }
}

// Switch workspace on X11
int switchWorkspaceX11(const char* direction) {
#ifdef HAVE_X11
    // Open display
    Display *display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "Cannot open display\n");
        return 1;
    }

    // Get root window
    Window root = DefaultRootWindow(display);

    // Get number of workspaces
    Atom net_desktops = XInternAtom(display, "_NET_NUMBER_OF_DESKTOPS", True);
    Atom actual_type;
    int actual_format;
    unsigned long nitems, bytes_after;
    unsigned char *data = NULL;

    if (XGetWindowProperty(display, root, net_desktops, 0, 1, False, XA_CARDINAL,
                           &actual_type, &actual_format, &nitems, &bytes_after, &data) == Success && data) {
        unsigned long num_workspaces = *(unsigned long *)data;
        XFree(data);

        // Get current workspace
        Atom net_current_desktop = XInternAtom(display, "_NET_CURRENT_DESKTOP", True);
        if (XGetWindowProperty(display, root, net_current_desktop, 0, 1, False, XA_CARDINAL,
                               &actual_type, &actual_format, &nitems, &bytes_after, &data) == Success && data) {
            unsigned long current_workspace = *(unsigned long *)data;
            XFree(data);

            // Calculate target workspace
            int target_workspace;
            if (strcmp(direction, "next") == 0) {
                target_workspace = (current_workspace + 1) % num_workspaces;
            } else if (strcmp(direction, "prev") == 0) {
                target_workspace = (current_workspace - 1 + num_workspaces) % num_workspaces;
            } else {
                usage("workspaces");
            }

            // Switch to target workspace
            XEvent event;
            memset(&event, 0, sizeof(event));
            event.type = ClientMessage;
            event.xclient.window = root;
            event.xclient.message_type = net_current_desktop;
            event.xclient.format = 32;
            event.xclient.data.l[0] = target_workspace;
            event.xclient.data.l[1] = CurrentTime;

            if (XSendEvent(display, root, False, SubstructureNotifyMask | SubstructureRedirectMask, &event)) {
                XFlush(display);
            } else {
                fprintf(stderr, "Cannot switch workspace\n");
                XCloseDisplay(display);
                return 1;
            }
        } else {
            fprintf(stderr, "Cannot get current workspace\n");
            XCloseDisplay(display);
            return 1;
        }
    } else {
        fprintf(stderr, "Cannot get number of workspaces\n");
        XCloseDisplay(display);
        return 1;
    }

    // Close display
    XCloseDisplay(display);
    return 0;
#else
    fprintf(stderr, "X11 support not available. This build only supports Wayland.\n");
    return 1;
#endif
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        usage(argv[0]);
    }

    // Validate direction argument
    if (strcmp(argv[1], "next") != 0 && strcmp(argv[1], "prev") != 0) {
        usage(argv[0]);
    }

    // Detect display server
    DisplayServer server = detectDisplayServer();

    switch (server) {
        case X11_SERVER:
            return switchWorkspaceX11(argv[1]);
        
        case WAYLAND_SERVER:
            return switchWorkspaceWayland(argv[1]);
        
        default:
            fprintf(stderr, "Unable to detect display server (X11 or Wayland)\n");
            return 1;
    }
}
