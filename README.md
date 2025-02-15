# 5G Layer 2 Software Implementation 


---

# Install `make` on Windows Using PowerShell

This guide provides step-by-step instructions to install the **`make`** tool on a Windows machine using **PowerShell**. It includes how to check if **Chocolatey** (`choco`) is installed, and instructions for installing it if it is not present.

## Prerequisites

- **Windows Operating System**
- **PowerShell** (comes pre-installed with Windows)
- **Administrator privileges** (required to install Chocolatey and `make`)

---

## Steps to Install Chocolatey (if `choco` is not installed)

### 1. **Check if Chocolatey is Installed**
Before installing `make`, you should first check if **Chocolatey** is already installed on your system.

1. Open **PowerShell** as Administrator:
   - **Right-click** on PowerShell in the **Start Menu**.
   - Select **Run as Administrator**.

2. Run the following command to check if `choco` is recognized:
   ```powershell
   choco --version
   ```

   - If Chocolatey is installed, you should see the version number of Chocolatey.
   - If you see an error like "`choco: command not found`", proceed to the next step to install Chocolatey.

### 2. **Install Chocolatey (if not installed)**

If Chocolatey is not installed, follow these steps:

1. Open **PowerShell as Administrator** (if not already done).

2. Run the following command to install **Chocolatey**:
   ```powershell
   Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.SecurityProtocolType]::Tls12; iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))
   ```

3. Once the installation completes, verify that Chocolatey was installed successfully:
   ```powershell
   choco --version
   ```

   This should return the version number of Chocolatey.

---

## Steps to Install `make`

### 1. **Install `make` Using Chocolatey**

Now that you have Chocolatey installed, you can use it to install the `make` command:

1. Open **PowerShell** (as Administrator if not already running).
2. Run the following command to install `make`:
   ```powershell
   choco install make
   ```

3. Wait for the installation process to complete.

### 2. **Verify `make` Installation**

After the installation is finished, verify that `make` was installed successfully:

1. Run the following command to check the version of `make`:
   ```powershell
   make --version
   ```

   If the installation was successful, you should see the version information for `make`.

---

## Troubleshooting

- **Chocolatey Command Not Found**:
  - Ensure you are running **PowerShell as Administrator**.
  - If `choco` is still not recognized, check if the `PATH` environment variable includes `C:\ProgramData\Chocolatey\bin`. If not, manually add it:
    - Open **Control Panel** > **System** > **Advanced system settings** > **Environment Variables**.
    - Under **System variables**, find **Path** and click **Edit**.
    - Add `C:\ProgramData\Chocolatey\bin` to the list and click **OK**.

- **Permission Errors During Installation**:
  - Ensure that you have **administrator privileges** to install both Chocolatey and `make`.
  - Temporarily disable any **antivirus or security software** that might block the installation process.
  - Check folder permissions for `C:\ProgramData\Chocolatey`.

- **Unable to Remove Temporary Files**:
  - If you encounter an error related to deleting temporary files during the installation, clear the **Temp** folder by going to `C:\Users\<YourUserName>\AppData\Local\Temp` and deleting its contents. Ensure that you have **full control** permissions on the folder.

---

