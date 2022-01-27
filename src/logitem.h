#pragma once
#include <stdint.h>
#include "logginglevels.h"
#include "subsystems.h"

class logItem {
  public:
    static constexpr uint32_t timestampLength{6U};        // Maximum number of digits to use for timestamps
    char timestamp[timestampLength];                      // cstring holding the timestamps
    static constexpr uint32_t maxItemLength{128U};        // Maximum length of new item to be logged. Will be an upper limit to all C-style string like strnlen()
    char contents[maxItemLength];                         // cstring holding the to-be-logged contents
    loggingLevel theLoggingLevel{loggingLevel::None};        // level of this item
    subSystem theSubSystem{subSystem::general};              // subSystem of this item

  private:
};
