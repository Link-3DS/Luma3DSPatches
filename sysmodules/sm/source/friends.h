#pragma once

#include "common.h"
#include <3ds.h>

#define FRIENDS_NASC_PROD_URL "https://nasc.nintendowifi.net/ac"
#define FRIENDS_NASC_TEST_URL "https://nasc.pretendo.cc/ac"
#define FRIENDS_NASC_DEV_URL "https://nasc.nextendo.online/ac"

void PatchFriendsModule(uint32_t targetPid) {
    Handle proc = 0, dbg = 0;

    Result openResult = svcOpenProcess(&proc, targetPid);
    if (!R_SUCCEEDED(openResult))
        return;

    Result debugResult = svcDebugActiveProcess(&dbg, targetPid);
    if (R_SUCCEEDED(debugResult)) {
        DebugEventInfo info;
        Result r;
        while (1) {
            r = svcGetProcessDebugEvent(&info, dbg);
            if (r != 0) {
                if (r == (s32)(0xd8402009))
                    break;
            }
            svcContinueDebugEvent(dbg, (DebugFlags)3);
        }

        struct {
            u32 addr;
            const char* str;
        } patches[] = {
            { 0x00161279, FRIENDS_NASC_PROD_URL },
            { 0x0016129a, FRIENDS_NASC_TEST_URL },
            { 0x001612c0, FRIENDS_NASC_DEV_URL }
        };

        for (int i = 0; i < 3; ++i)
            WriteString(dbg, patches[i].str, patches[i].addr);

        svcCloseHandle(dbg);
    }

    svcCloseHandle(proc);
}