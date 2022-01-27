#pragma once
#include <stdint.h>
#include "logginglevels.h"
#include "subsystems.h"

// represents a destination to which we can send logging output, such as serial port, network, SD card, etc.

class logOutput {
  public:
    explicit logOutput();

    void setOutputDestination(bool (*aFunction)(const char *));                 // sets a pointer to a function handling the output of the logging to eg serial, network or file on SD card, etc.
    bool isActive() const;                                                      //
    void setColoredOutput(bool newSetting);                                     // set the colorize output option
    bool isColoredOutput() const;                                                     //
    void setIncludeTimestamp(bool newSetting);                                  // set the includeTimestamp option
    bool hasTimestampIncluded() const;                                                  //
    void setLoggingLevel(loggingLevel newLevel);                                // set the loggingLevel for a all subsystem
    void setLoggingLevel(subSystem theSubSystem, loggingLevel newLevel);        // set the loggingLevel for a given subsystem
    loggingLevel getLoggingLevel(subSystem theSubSystem) const;                 // returns the current loggingLevel for given subsystem
    bool write(const char *) const;                                             // send cstyle string to the output

  private:
    bool (*writeOutput)(const char *){nullptr};                                                                 // pointer to function outputting the logged data - 1 goes to eg serial port
    bool colorOutput{false};                                                                                    // does this output wants colorization
    bool addTimestamp{false};                                                                                   // does this output wants timestamps added
    loggingLevel theLoggingLevel[static_cast<uint8_t>(subSystem::nmbrOfSubsystems)]{loggingLevel::None};        // for each subsystem, the loggingLevel for this output
};
