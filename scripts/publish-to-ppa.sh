#!/bin/bash
# Script to manually publish to Ubuntu PPA
# Usage: ./publish-to-ppa.sh <version> <ubuntu-release>
# Example: ./publish-to-ppa.sh 1.5.0 jammy

set -e

# Check arguments
if [ $# -ne 2 ]; then
    echo "Usage: $0 <version> <ubuntu-release>"
    echo "Example: $0 1.5.0 jammy"
    echo ""
    echo "Supported Ubuntu releases: focal, jammy, noble"
    exit 1
fi

VERSION=$1
UBUNTU_RELEASE=$2

# Validate Ubuntu release
case "$UBUNTU_RELEASE" in
    focal|jammy|noble)
        ;;
    *)
        echo "Error: Unsupported Ubuntu release '$UBUNTU_RELEASE'"
        echo "Supported releases: focal, jammy, noble"
        exit 1
        ;;
esac

# Set maintainer info
export DEBFULLNAME="Pavlos Orfanidis"
export DEBEMAIL="pavlos@orfanidis.net.gr"

echo "Publishing workspace-switcher ${VERSION} for Ubuntu ${UBUNTU_RELEASE}"
echo ""

# Update changelog
echo "Updating debian/changelog..."
dch -v "${VERSION}-ubuntu1~${UBUNTU_RELEASE}" \
    -D "${UBUNTU_RELEASE}" \
    -u low \
    "New release: Add Wayland support for Sway, Hyprland, GNOME, and KDE Plasma"

# Finalize changelog
dch -r ""

echo "Building source package..."
# Build source package
debuild -S -sa

echo ""
echo "Source package built successfully!"
echo ""
echo "To upload to PPA, run:"
echo "  cd .."
echo "  dput ppa:porfanid/workspaces workspace-switcher_${VERSION}-ubuntu1~${UBUNTU_RELEASE}_source.changes"
echo ""
echo "Or to upload directly now, run:"
read -p "Upload to PPA now? (y/N) " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    cd ..
    dput ppa:porfanid/workspaces workspace-switcher_${VERSION}-ubuntu1~${UBUNTU_RELEASE}_source.changes
    echo "Uploaded successfully!"
else
    echo "Skipped upload. You can upload manually later."
fi
