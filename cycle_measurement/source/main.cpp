#include <wiiuse/wpad.h>

#include <cstdio>
#include <cstdlib>

static constexpr size_t expectedTickDifference = 1;

static void InitVideo();

int main(int /* argc */, char * /* argv */[]) {
    InitVideo();
    WPAD_Init();

    printf("\x1b[2;0H");
    printf("Dolphin Anti-Emulation Techniques by MikeIsAStar\n\n");

    size_t tickDifference;
    {
        size_t startTick;
        size_t endTick;

        u32 interruptsEnabled = IRQ_Disable();
        {
            asm volatile("sync");
            asm volatile("mftbl %0" : "=r"(startTick));
            asm volatile("sync");
            asm volatile("mftbl %0" : "=r"(endTick));
            asm volatile("sync");
        }
        IRQ_Restore(interruptsEnabled);

        tickDifference = endTick - startTick;
    }

    const char *platform;
    if (tickDifference != expectedTickDifference) {
        platform = "Emulator";
    } else {
        platform = "Console";
    }

    printf("Platform: %s", platform);

    while (true) {
        WPAD_ScanPads();

        u32 buttonsDown = WPAD_ButtonsDown(0);
        if (buttonsDown & WPAD_BUTTON_HOME) {
            break;
        }

        VIDEO_WaitVSync();
    }

    return EXIT_SUCCESS;
}

static void InitVideo() {
    VIDEO_Init();

    GXRModeObj *renderMode = VIDEO_GetPreferredMode(NULL);
    void *xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(renderMode));
    CON_Init(xfb, 20, 20, renderMode->fbWidth, renderMode->xfbHeight,
            renderMode->fbWidth * VI_DISPLAY_PIX_SZ);

    VIDEO_Configure(renderMode);
    VIDEO_SetNextFramebuffer(xfb);
    VIDEO_SetBlack(false);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    if (renderMode->viTVMode & VI_NON_INTERLACE) {
        VIDEO_WaitVSync();
    }
}
