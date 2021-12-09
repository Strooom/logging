#pragma once
#include "Arduino.h"

enum class subSystems : uint8_t {
    general        = 0x00,
    filesystem     = 0x01,
    nfc            = 0x02,
    display        = 0x03,
    mainController = 0x04,
    machine        = 0x05,
    pc             = 0x06,
    lockerUnit     = 0x07,
    networkCtrl    = 0x08,
    networkData    = 0x09,
    checklists     = 0x0A,
    user           = 0x0B,
    certificate    = 0x0C,
    nmbrOfSubsystems
};