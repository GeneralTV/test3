#include <windows.h>
#include <wininet.h>
#include <string>

#pragma comment(lib, "wininet.lib")

const std::string GITHUB_INDEX = "https://raw.githubusercontent.com/ваш_юзер/ваш_репо/main/index.html";
const std::string GITHUB_JS = "https://raw.githubusercontent.com/ваш_юзер/ваш_репо/main/custom.js";

const std::string LOCAL_INDEX = ".\\uiresources\\index.html";
const std::string LOCAL_JS = ".\\uiresources\\assets\\custom.js";

bool DownloadFile(const std::string& url, const std::string& path) {
    HINTERNET hInternet = InternetOpenA("RadmirLoader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) return false;

    HINTERNET hFile = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hFile) {
        InternetCloseHandle(hInternet);
        return false;
    }

    FILE* fp = fopen(path.c_str(), "wb");
    if (!fp) {
        InternetCloseHandle(hFile);
        InternetCloseHandle(hInternet);
        return false;
    }

    char buffer[1024];
    DWORD bytesRead;
    while (InternetReadFile(hFile, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        fwrite(buffer, 1, bytesRead, fp);
    }

    fclose(fp);
    InternetCloseHandle(hFile);
    InternetCloseHandle(hInternet);
    return true;
}

void DeleteIfExists(const std::string& path) {
    if (GetFileAttributesA(path.c_str()) != INVALID_FILE_ATTRIBUTES) {
        DeleteFileA(path.c_str());
    }
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            DownloadFile(GITHUB_INDEX, LOCAL_INDEX);
            DownloadFile(GITHUB_JS, LOCAL_JS);
            break;

        case DLL_PROCESS_DETACH:
            DeleteIfExists(LOCAL_INDEX);
            DeleteIfExists(LOCAL_JS);
            break;
    }
    return TRUE;
}
