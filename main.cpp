#include <jni.h>
#include <android/log.h>
#include <dlfcn.h>
#include <unistd.h>
#include <fstream>
#include <string>
#include <pthread.h>
#include "dobby.h"

#define LOG_TAG "LongRangeHook"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

bool isLongRangeEnabled = false;
float (*orig_get_farClipPlane)(void* __this) = nullptr;

// Ito yung papalit sa original na function
float hooked_get_farClipPlane(void* __this) {
    if (isLongRangeEnabled) {
        return 10000.0f; // Maximum view distance
    }
    return orig_get_farClipPlane(__this);
}

// Thread na magbabasa sa file natin
void* MonitorFileThread(void* arg) {
    while (true) {
        std::ifstream file("/storage/emulated/0/DaddyHok/longrange.txt");
        if (file.is_open()) {
            std::string line;
            if (std::getline(file, line)) {
                if (line == "1") isLongRangeEnabled = true;
                else isLongRangeEnabled = false;
            }
            file.close();
        }
        sleep(1); // Check every 1 second
    }
    return nullptr;
}

// Auto-run kapag na-load yung .so
__attribute__((constructor))
void on_load() {
    LOGI("Long Range Hook Loaded!");
    
    void* handle = dlopen("libil2cpp.so", RTLD_LAZY);
    if (!handle) {
        LOGI("Error: Cannot find libil2cpp.so");
        return;
    }

    Dl_info info;
    if (dladdr(handle, &info) != 0) {
        uintptr_t base = (uintptr_t)info.dli_fbase;
        void* target = (void*)(base + 0x502200C); // RVA ng get_farClipPlane
        
        DobbyHook(target, (void*)hooked_get_farClipPlane, (void**)&orig_get_farClipPlane);
        LOGI("Hooked farClipPlane successfully at %p", target);
        
        // Simulan yung thread na magbabasa ng file
        pthread_t t;
        pthread_create(&t, nullptr, MonitorFileThread, nullptr);
    }
}
