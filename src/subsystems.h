#pragma once
#include <stdint.h>

enum class subSystem : uint8_t {
    general,
    memoryUsage,
    filesystem,
    nfc,
    display,
    mainController,
    machine,
    current,
    pc,
    lockerUnit,
    networkCtrl,
    wifiSignalStrenght,
    networkData,
    checklists,
    user,
    certificate,
    nmbrOfSubsystems
};