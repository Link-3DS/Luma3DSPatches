#pragma once

#include <string.h>
#include <3ds.h>

void WriteString(Handle hDebug, const char* text, u32 address) {
    u32 len = (u32)(strlen(text) + 1);
    svcWriteProcessMemory(hDebug, text, address, len);
}