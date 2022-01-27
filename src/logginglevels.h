#pragma once
#include <stdint.h>

enum class loggingLevel : uint8_t {
    None     = 0,        // Not logging anything at all
    Critical = 1,        // only logging critical errors
    Error    = 2,        // logging all errors
    Warning  = 3,        // logging all warnings and all errors
    Info     = 4,        // logging errors, warnings and useful info (release)
    Debug    = 5,        // logging everything (including debug info)
    nmbrLoggingLevels
};
