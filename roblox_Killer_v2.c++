#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

void KillProcess(DWORD processID) {
    HANDLE processHandle = OpenProcess(PROCESS_TERMINATE, FALSE, processID);
    if (processHandle == NULL) {
        std::cout << "Could not open process." << std::endl;
        return;
    }

    if (!TerminateProcess(processHandle, 0)) {
        std::cout << "Could not terminate process." << std::endl;
    } else {
        std::cout << "Roblox process terminated successfully." << std::endl;
    }
    CloseHandle(processHandle);
}

DWORD GetRobloxProcessId() {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    DWORD processId = 0;
    PROCESSENTRY32 pe = { sizeof(pe) };
    for (BOOL ret = Process32First(hSnapshot, &pe); ret; ret = Process32Next(hSnapshot, &pe)) {
        if (_stricmp(pe.szExeFile,"RobloxPlayerBeta.exe") == 0) {
            processId = pe.th32ProcessID;
            break;
        }
    }
    CloseHandle(hSnapshot);
    return processId;
}

int main() {
    DWORD processID = GetRobloxProcessId();
    if (processID == 0) {
        std::cout << "Could not find Roblox process." << std::endl;
        return 1;
    }
    KillProcess(processID);
    return 0;
}
