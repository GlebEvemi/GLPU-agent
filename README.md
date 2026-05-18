# GLPU-Agent

A Windows system information collector service that periodically gathers hardware and OS metrics and sends them to a remote server via HTTP API.

## Overview

GLPU-Agent is a lightweight Windows service application written in C that collects detailed computer system information and transmits it to a configured remote server. The service runs continuously in the background and automatically performs data collection at regular intervals.

## Features

- **System Information Collection**: Gathers comprehensive PC hardware and operating system details
- **Windows Service Integration**: Runs as a native Windows service with automatic startup
- **Remote Data Transmission**: Sends collected data to a remote server using HTTP POST requests
- **Configuration Management**: Loads server credentials from a JSON configuration file
- **Logging**: Maintains service operation logs for debugging and monitoring
- **Debug Mode**: Supports debug mode for testing without service registration

## Collected System Information

The service collects and sends the following data in JSON format:

- **Hostname**: Computer name
- **OS Information**: OS name and version
- **BIOS Details**: Manufacturer and status
- **Domain**: Computer's domain membership
- **Processor**: CPU name and specifications
- **Memory**: Total RAM in GB
- **Storage**: C: drive total size and free space in GB

## Requirements

### Dependencies

- **libcurl**: For HTTP communication
- **cJSON**: JSON parsing library (included)
- **Windows API**: Standard Windows libraries (advapi32, winmm, crypt32)
- **PowerShell**: For system information collection

### System Requirements

- Windows OS (Windows 7 or later recommended)
- Administrator privileges for service installation
- PowerShell installed and available on the system
- Network connectivity to the remote server

## Building

### Prerequisites

1. MinGW GCC compiler
2. libcurl development libraries
3. Windows development headers

### Compilation

Modify the `makefile` to set the correct paths for libcurl:

```bash
# Update INCLUDES and LIBS paths in makefile to match your system
INCLUDES = -Iinc -I<path-to-curl>/include
LIBS = -L<path-to-curl>/lib
```

Then compile:

```bash
make
```

This produces `GLPU-Agent.exe`

### Clean Build

```bash
make clean
```

## Configuration

Before running the service, create a configuration file at:

```
C:\ProgramData\GLPU-Agent\config.json
```

Example configuration:

```json
{
  "server_url": "https://glpu-server.ivkh.ee",
  "username": "service_user",
  "password": "service_password"
}
```

**Note**: The service expects the PowerShell script (`getInfoScript.ps1`) to be in the working directory.

## Installation & Usage

### Install Service

```bash
GLPU-Agent.exe install
```

This registers the service with Windows and sets it to auto-start.

### Start Service

```bash
GLPU-Agent.exe start
```

Or using the Windows service manager:

```bash
net start GLPU-Agent
```

### Stop Service

```bash
GLPU-Agent.exe stop
```

Or using the Windows service manager:

```bash
net stop GLPU-Agent
```

### Remove Service

```bash
GLPU-Agent.exe remove
```

This unregisters the service from Windows.

### Debug Mode

For testing and debugging without installing as a service:

```bash
GLPU-Agent.exe debug
```

This runs the service logic once in the console (requires `getInfoScript.ps1` in the current directory).

## Service Behavior

Once running, the GLPU-Agent service:

1. **Initializes**: Registers with the Windows Service Control Manager
2. **Loads Configuration**: Reads credentials from `config.json`
3. **Main Loop**: Executes every hour:
   - Executes `getInfoScript.ps1` via PowerShell to gather system info
   - Formats data as JSON
   - Sends data to `<server_url>/api/pc` via HTTP POST
   - Logs operation status
4. **Cleanup**: Properly terminates on stop signal

### Service Intervals

- **Data Collection**: Every hour
- **Configuration Reload**: Each cycle (every hour)

## Logging

Service logs are written to:

```
C:\service_log.txt
```

This file contains timestamps and operation status messages for troubleshooting.

## API Endpoint

The service sends data to:

```
POST <server_url>/api/pc
```

**Authentication**: HTTP Basic Auth (username/password from config)

**Content-Type**: `application/json`

**Request Body**: System information JSON

## Project Structure

```
GLPU-Agent/
├── src/
│   ├── service.c          # Windows service implementation & main entry point
│   ├── api.c              # HTTP communication and data transmission
│   ├── config.c           # Configuration file parsing
│   ├── cJSON.c            # JSON parsing library
│   └── getInfoScript.ps1  # PowerShell script for system info collection
├── inc/
│   ├── service.h          # Service declarations
│   ├── api.h              # API function declarations
│   ├── config.h           # Config structure definitions
│   └── cJSON.h            # JSON library header
├── makefile               # Build configuration
└── README.md              # This file
```

## Technical Details

### Windows Service Features

- **Service Type**: Win32 Own Process
- **Startup Type**: Auto-start
- **Control Handlers**: Responds to SERVICE_CONTROL_STOP signals
- **Status Updates**: Reports status to Service Control Manager

### Communication

- Uses libcurl for HTTP POST requests
- SSL/TLS verification enabled
- Implements HTTP Basic Authentication
- JSON format for data serialization

### Error Handling

The service includes error handling for:

- Configuration file loading failures
- PowerShell script execution errors
- Network communication failures
- JSON parsing errors

All errors are logged to `C:\service_log.txt`

## Troubleshooting

### Service Won't Start

1. Check that `C:\ProgramData\GLPU-Agent\config.json` exists and is valid JSON
2. Verify `getInfoScript.ps1` is in the working directory
3. Check `C:\service_log.txt` for error messages
4. Ensure the executable has appropriate permissions

### Configuration Not Found

Verify the config file path:

```
C:\ProgramData\GLPU-Agent\config.json
```

And that it contains valid JSON with all required fields.

### Network Errors

Check:

1. Server URL is correct and accessible
2. Credentials (username/password) are correct
3. Network connectivity to the server
4. Firewall rules allow outbound HTTPS traffic

### PowerShell Script Errors

Ensure:

1. PowerShell is installed and in PATH
2. `getInfoScript.ps1` is in the application's working directory
3. PowerShell execution policies allow script execution

## Author

GlebEvemi

## Notes

- The service runs with elevated privileges (when installed as a service)
- SSL certificate verification is enabled
- All configuration values are stored in plain text - keep the config file secure
- The service requires network connectivity to the configured server
