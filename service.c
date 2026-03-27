#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <string.h>

#define SERVICE_NAME _T("MyService")

SERVICE_STATUS g_ServiceStatus;
SERVICE_STATUS_HANDLE g_StatusHandle;
HANDLE g_StopEvent = NULL;

// ---------------- LOG ----------------
void Log(const char* msg)
{
    FILE *f = fopen("C:\\service_log.txt", "a");
    if (f)
    {
        fprintf(f, "%s\n", msg);
        fclose(f);
    }
}

// ---------------- STATUS ----------------
void UpdateStatus(DWORD state)
{
    g_ServiceStatus.dwCurrentState = state;
    SetServiceStatus(g_StatusHandle, &g_ServiceStatus);
}

// ---------------- CONTROL HANDLER ----------------
VOID WINAPI ServiceCtrlHandler(DWORD ctrl)
{
    switch (ctrl)
    {
    case SERVICE_CONTROL_STOP:
        Log("STOP received");
        UpdateStatus(SERVICE_STOP_PENDING);
        SetEvent(g_StopEvent);
        break;
    default:
        break;
    }
}

// ---------------- SERVICE MAIN ----------------
VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv)
{
    g_StatusHandle = RegisterServiceCtrlHandler(SERVICE_NAME, ServiceCtrlHandler);

    if (!g_StatusHandle)
        return;

    ZeroMemory(&g_ServiceStatus, sizeof(g_ServiceStatus));
    g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;

    UpdateStatus(SERVICE_START_PENDING);

    g_StopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    UpdateStatus(SERVICE_RUNNING);
    Log("Service started");

    // MAIN LOOP
    while (WaitForSingleObject(g_StopEvent, 5000) != WAIT_OBJECT_0)
    {
        Log("Service working...");
    }

    Log("Service stopped");
    UpdateStatus(SERVICE_STOPPED);
}

// ---------------- INSTALL ----------------
int InstallService()
{
    SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
    if (!hSCM)
    {
        printf("OpenSCManager failed\n");
        return 1;
    }

    TCHAR path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);

    SC_HANDLE hService = CreateService(
        hSCM,
        SERVICE_NAME,
        SERVICE_NAME,
        SERVICE_ALL_ACCESS,
        SERVICE_WIN32_OWN_PROCESS,
        SERVICE_DEMAND_START,
        SERVICE_ERROR_NORMAL,
        path,
        NULL, NULL, NULL, NULL, NULL
    );

    if (!hService)
    {
        printf("CreateService failed\n");
        CloseServiceHandle(hSCM);
        return 1;
    }

    printf("Service installed\n");

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCM);

    return 0;
}

// ---------------- REMOVE ----------------
int RemoveService()
{
    SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!hSCM) return 1;

    SC_HANDLE hService = OpenService(hSCM, SERVICE_NAME, DELETE);
    if (!hService) return 1;

    DeleteService(hService);

    printf("Service removed\n");

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCM);

    return 0;
}

// ---------------- START ----------------
int StartMyService()
{
    SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!hSCM) return 1;

    SC_HANDLE hService = OpenService(hSCM, SERVICE_NAME, SERVICE_START);
    if (!hService) return 1;

    StartService(hService, 0, NULL);

    printf("Service started\n");

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCM);

    return 0;
}

// ---------------- STOP ----------------
int StopMyService()
{
    SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!hSCM) return 1;

    SC_HANDLE hService = OpenService(hSCM, SERVICE_NAME, SERVICE_STOP);
    if (!hService) return 1;

    SERVICE_STATUS status;
    ControlService(hService, SERVICE_CONTROL_STOP, &status);

    printf("Service stopped\n");

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCM);

    return 0;
}

// ---------------- MAIN ----------------
int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        if (strcmp(argv[1], "install") == 0)
            return InstallService();

        if (strcmp(argv[1], "remove") == 0)
            return RemoveService();

        if (strcmp(argv[1], "start") == 0)
            return StartMyService();

        if (strcmp(argv[1], "stop") == 0)
            return StopMyService();

        if (strcmp(argv[1], "debug") == 0)
        {
            printf("Debug mode\n");
            ServiceMain(0, NULL);
            return 0;
        }
    }

    SERVICE_TABLE_ENTRY ServiceTable[] =
    {
        { SERVICE_NAME, ServiceMain },
        { NULL, NULL }
    };

    StartServiceCtrlDispatcher(ServiceTable);

    return 0;
}