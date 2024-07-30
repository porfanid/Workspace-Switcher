#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

// Function to print usage information
void usage(const char *progname) {
    fprintf(stderr, "Usage: %s [next|prev]\n", progname);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        usage(argv[0]);
    }

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
            if (strcmp(argv[1], "next") == 0) {
                target_workspace = (current_workspace + 1) % num_workspaces;
            } else if (strcmp(argv[1], "prev") == 0) {
                target_workspace = (current_workspace - 1 + num_workspaces) % num_workspaces;
            } else {
                usage(argv[0]);
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
}
