#pragma once

#include <3ds.h>

static const uint8_t ssl_patch_payload[] = {
    0xE7, 0x1D, 0xF9, 0x37, 0x3A, 0x1B, 0x90, 0x18, 0x1B, 0x30, 0x05, 0x43,
    0xBD, 0x62, 0x00, 0x2E, 0x04, 0xD0, 0x20, 0x1C, 0x20, 0x30, 0x31, 0x1C
};

void PatchSSLModule(uint32_t targetPid) {
    Handle hProc = 0, hDbg = 0;

    if (R_SUCCEEDED(svcOpenProcess(&hProc, targetPid))) {
        if (R_SUCCEEDED(svcDebugActiveProcess(&hDbg, targetPid))) {

            for (;;) {
                DebugEventInfo dbgEvent;
                Result res = svcGetProcessDebugEvent(&dbgEvent, hDbg);
                if (res) {
                    if (res == (s32)0xd8402009)
                        break;
                }
                svcContinueDebugEvent(hDbg, (DebugFlags)3);
            }

            svcWriteProcessMemory(
                hDbg,
                ssl_patch_payload,
                0x00106C56,
                sizeof(ssl_patch_payload)
            );
            svcCloseHandle(hDbg);
        }
        svcCloseHandle(hProc);
    }
}