# TizenClaw WebView Application

An EFL WebView-based native application designed for the Tizen platform, serving as an extension for the TizenClaw framework. This application receives URIs via Tizen's AppControl mechanism and renders web content using the embedded WebKit engine (`ewk`).

## Prerequisites

To build and run this application, you need the following environment set up:

1. **Tizen Studio & Platform Tools**
   - Install the [Tizen Studio](https://developer.tizen.org/).
   - Ensure you have the Tizen 10.0+ platform headers and libraries.
2. **Gerrit Build System (GBS)**
   - Used for building the RPM package natively. Learn more at the [Tizen GBS Guide](https://source.tizen.org/documentation/developer-guide/getting-started-guide/installing-development-tools/).
3. **Smart Development Bridge (sdb)**
   - Required to communicate with the Tizen device or emulator. Usually included in Tizen Studio platform tools.
4. **Target Device**
   - A physical Tizen device or the Tizen Emulator running with root privileges enabled (`sdb root on`).

## Building and Deploying

We provide a convenient deployment script (`deploy.sh`) to automate the entire process of building the GBS project, transferring the RPM to the device, installing it, and registering the app with the Tizen Application Framework.

### Automated Deployment

Run the script from the project root:

```bash
# Full build and deployment
./deploy.sh

# Skip the initial GBS build environment setup for a faster rebuild
./deploy.sh -n

# Skip the build entirely and only deploy the existing RPM (if already built)
./deploy.sh -s
```

If you have multiple devices connected via `sdb`, specify the target device using the `-d` flag:

```bash
./deploy.sh -d emulator-26101
```

### Manual Deployment (Alternative)

If you prefer to perform the steps manually:

1. **Build using GBS**:
   ```bash
   gbs build -A x86_64 --include-all
   ```
2. **Push to Device**:
   ```bash
   sdb root on
   sdb shell mount -o remount,rw /
   sdb push ~/GBS-ROOT/local/repos/tizen/x86_64/RPMS/tizenclaw-webview-1.0.0-1.x86_64.rpm /tmp/
   ```
3. **Install RPM**:
   ```bash
   sdb shell rpm -Uvh --force /tmp/tizenclaw-webview-1.0.0-1.x86_64.rpm
   ```
4. **Register with the App Framework**:
   ```bash
   sdb shell tpk-backend --preload -y org.tizen.tizenclaw-webview
   ```

## Running the Application

### Launching via App Launcher
You can launch the application directly from the sdb shell using the standard tizen `app_launcher`:

```bash
sdb shell app_launcher -s org.tizen.tizenclaw-webview
```

### Launching via AppControl (with a specific UI)
To launch the WebView and immediately navigate to a specific website, you can use the `app_com_tool` to send an AppControl request containing the desired URI:

```bash
# Example: Launch and navigate to Google
sdb shell app_com_tool -a org.tizen.tizenclaw-webview -u "https://google.com"
```

## Application Architecture

- **Main UI Core (`src/tizenclaw-webview.cc`)**: Uses the EFL library (`Evas`, `Ecore`, `Elm`) to set up the graphical window and the `ewk_view_add` context to host the web engine.
- **AppControl Handler**: Listens for incoming `app_control` events natively. If a URI string is successfully found in the payload via `app_control_get_uri`, it's passed immediately to `ewk_view_url_set`.

## License

This project is licensed under the Apache License, Version 2.0. See the [LICENSE](LICENSE) file for more details.