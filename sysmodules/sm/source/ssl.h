#pragma once

#include <3ds.h>

static const uint8_t g_sslPatchBlob[] = {
    0xE7, 0x1D, 0xF9, 0x37, 0x3A, 0x1B, 0x90, 0x18, 0x1B, 0x30, 0x05, 0x43,
    0xBD, 0x62, 0x00, 0x2E, 0x04, 0xD0, 0x20, 0x1C, 0x20, 0x30, 0x31, 0x1C
};

#define SSL_PATCH_TARGET_ADDRESS (0x00106C56u)

void SSLPatchs(u32 procId)
{
    Handle hProc = 0, hDbg = 0;
    Result status = svcOpenProcess(&hProc, procId);
    if (status < 0)
        return;

    status = svcDebugActiveProcess(&hDbg, procId);
    if (status < 0) {
        svcCloseHandle(hProc);
        return;
    }

    while (1) {
        DebugEventInfo evtInfo;
        Result evtRes = svcGetProcessDebugEvent(&evtInfo, hDbg);
        if (evtRes == (Result)0xd8402009) break;
        svcContinueDebugEvent(hDbg, (DebugFlags)3);
    }

    svcWriteProcessMemory(
        hDbg,
        g_sslPatchBlob,
        SSL_PATCH_TARGET_ADDRESS,
        sizeof(g_sslPatchBlob)
    );

    svcCloseHandle(hDbg);
    svcCloseHandle(hProc);
}