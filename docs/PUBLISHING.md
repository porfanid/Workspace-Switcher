# Publishing to Ubuntu PPA

This document describes the process for publishing Workspace Switcher to the Ubuntu PPA (Personal Package Archive).

## Prerequisites

Before you can publish to the PPA, you need to set up the following secrets in your GitHub repository:

### Required GitHub Secrets

1. **GPG_PRIVATE_KEY**: Your GPG private key used for signing packages
   - Export your private key: `gpg --armor --export-secret-keys YOUR_EMAIL > private.key`
   - Copy the entire contents including the header and footer
   
2. **GPG_KEY_ID**: Your GPG key ID (8-character or 16-character hex)
   - Find it with: `gpg --list-secret-keys --keyid-format=long`
   - Use the ID after "sec" (e.g., `rsa4096/1234567890ABCDEF`)

3. **GPG_PASSPHRASE**: The passphrase for your GPG key
   - This is the password you use to unlock your GPG key

### Setting up GPG Keys

If you don't have a GPG key yet:

```bash
# Generate a new GPG key
gpg --full-generate-key

# Choose RSA and RSA
# Key size: 4096
# Expiration: your choice (recommend 2-5 years)
# Provide your name and email (must match your Launchpad account)

# List your keys to find the key ID
gpg --list-secret-keys --keyid-format=long

# Export your public key to upload to Launchpad
gpg --armor --export YOUR_EMAIL > public.key
```

### Launchpad Setup

The PPA is already configured at **ppa:porfanid/workspaces**.

If you need to set up Launchpad access:

1. Create a Launchpad account at https://launchpad.net
2. Upload your GPG public key to Launchpad:
   - Go to https://launchpad.net/~porfanid/+editpgpkeys
   - Paste the contents of your public key
   - Confirm via email
3. The PPA already exists at: https://launchpad.net/~porfanid/+archive/ubuntu/workspaces

## Automated Publishing (Recommended)

### Publishing via Git Tags

The easiest way to publish a new version is to create and push a git tag:

```bash
# Create a new version tag
git tag -a v1.5.0 -m "Release version 1.5.0 with Wayland support"

# Push the tag to GitHub
git push origin v1.5.0
```

This will automatically trigger the `publish-ppa.yml` workflow, which will:
1. Build source packages for Ubuntu Focal (20.04), Jammy (22.04), and Noble (24.04)
2. Sign the packages with your GPG key
3. Upload them to your PPA

### Manual Publishing via Workflow Dispatch

You can also manually trigger the publishing workflow from GitHub:

1. Go to the "Actions" tab in your repository
2. Select "Publish to Ubuntu PPA" workflow
3. Click "Run workflow"
4. Enter the version number (e.g., `1.5.0`)
5. Click "Run workflow"

## Manual Publishing (Alternative)

If you prefer to publish manually from your local machine:

### Quick Method: Using the Helper Script

We provide a helper script to simplify the publishing process:

```bash
# Make the script executable (first time only)
chmod +x scripts/publish-to-ppa.sh

# Publish for a specific Ubuntu release
./scripts/publish-to-ppa.sh 1.5.0 jammy

# The script will guide you through the process
```

### Manual Method: Step by Step

Install Required Tools

```bash
sudo apt-get install -y \
  debhelper \
  devscripts \
  dput \
  gnupg \
  software-properties-common \
  libx11-dev \
  libxrandr-dev
```

Build and Upload for Each Ubuntu Release

```bash
# Set your version
VERSION="1.5.0"

# For each Ubuntu release (focal, jammy, noble):
UBUNTU_RELEASE="jammy"  # or focal, noble

# Set maintainer info
export DEBFULLNAME="Pavlos Orfanidis"
export DEBEMAIL="pavlos@orfanidis.net.gr"

# Update changelog
dch -v "${VERSION}-ubuntu1~${UBUNTU_RELEASE}" \
    -D "${UBUNTU_RELEASE}" \
    -u low \
    "New release: Add Wayland support"

# Build source package
debuild -S -sa

# Upload to PPA (from parent directory)
cd ..
dput ppa:porfanid/workspaces workspace-switcher_${VERSION}-ubuntu1~${UBUNTU_RELEASE}_source.changes
cd -
```

## Supported Ubuntu Releases

The workflow builds packages for the following Ubuntu releases:

- **Ubuntu 20.04 LTS (Focal Fossa)** - `focal`
- **Ubuntu 22.04 LTS (Jammy Jellyfish)** - `jammy`
- **Ubuntu 24.04 LTS (Noble Numbat)** - `noble`

You can add more releases by modifying the `ubuntu_release` matrix in `.github/workflows/publish-ppa.yml`.

## Monitoring Builds

After uploading to the PPA:

1. Go to your PPA page: https://launchpad.net/~porfanid/+archive/ubuntu/workspaces
2. Check the build status for each architecture (amd64, i386, arm64, etc.)
3. Builds typically take 5-30 minutes depending on queue length
4. Once built, packages will be available to users within a few hours

## Troubleshooting

### Build Failures

If a build fails on Launchpad:

1. Click on the failed build to see the build log
2. Common issues:
   - Missing dependencies in `debian/control`
   - Build-time errors (fix in code)
   - Version conflicts (check debian/changelog)

### Upload Rejections

If uploads are rejected:

1. **Wrong signature**: Verify your GPG key is correctly uploaded to Launchpad
2. **Version exists**: You can't upload the same version twice. Increment the debian revision (e.g., `-ubuntu2`)
3. **Invalid changes file**: Ensure you're uploading a source package (`debuild -S`)

### GPG Issues

If you get GPG errors:

```bash
# Verify your key is available
gpg --list-secret-keys

# Test signing
echo "test" | gpg --clearsign

# If passphrase issues, try:
export GPG_TTY=$(tty)
```

## Version Numbering

Follow this versioning scheme:

- **Upstream version**: `1.5.0` (your software version)
- **Debian revision**: `-ubuntu1` (first upload for this version)
- **Ubuntu release**: `~jammy` (target Ubuntu release)
- **Full version**: `1.5.0-ubuntu1~jammy`

The tilde (`~`) ensures that when a newer Ubuntu release comes out, its package will be considered newer.

## Security Notes

- **Never commit GPG keys to the repository**
- Store secrets only in GitHub Secrets (Settings > Secrets and variables > Actions)
- Rotate GPG keys periodically (every 2-5 years)
- Use a strong passphrase for your GPG key
- Keep your private key secure and backed up

## Installation for Users

After publishing, users can install with:

```bash
sudo add-apt-repository ppa:porfanid/workspaces
sudo apt-get update
sudo apt-get install workspace-switcher
```

## References

- [Launchpad PPA Documentation](https://help.launchpad.net/Packaging/PPA)
- [Debian Packaging Guide](https://www.debian.org/doc/manuals/maint-guide/)
- [Ubuntu Packaging Guide](https://packaging.ubuntu.com/html/)
