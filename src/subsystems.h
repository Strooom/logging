#pragma once
#include "Arduino.h"

enum class subSystems : uint8_t {
    general            = 0x00,
    memoryUsage        = 0x01,
    filesystem         = 0x08,
    nfc                = 0x10,
    display            = 0x18,
    mainController     = 0x20,
    machine            = 0x28,
    current            = 0x29,
    pc                 = 0x30,
    lockerUnit         = 0x38,
    networkCtrl        = 0x40,
    wifiSignalStrenght = 0x41,
    networkData        = 0x48,
    checklists         = 0x50,
    user               = 0x58,
    certificate        = 0x60,
    nmbrOfSubsystems
};