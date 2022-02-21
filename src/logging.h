// #############################################################################
// ###                                                                       ###
// ### General Purpose Logging toolkit for MicroControllers                  ###
// ### https://github.com/Strooom/Logging                                    ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// ###                                                                       ###
// #############################################################################

// When developing on microcontrollers, it's often not easy to know what's going on inside.
// A basic solution is to include 'print' statements in your code, which will output to some kind of monitor.
// But this monitoring output could differ from case to case : UART, SD-card, ...
// And further, this output may not be available until after some initialization.
// This logging solution provides a solution :
// * It creates a global object, so it is available from the real start of your program
// * It will buffer the logged items, until the output becomes available
// * It can easily be adapt to any kind of output
//
// V 1.0.0 25-08-2020 First release
// V 2.0.0 08-10-2020 Made it simpler by providing less public methods, but making them more flexible. Did extensive unit testing for robustness.
// Example of using snprintf :  theLog.snprintf(loggingLevel::Debug, "Error in %s on line %d\n", __FILE__, __LINE__);
// V2.1.0 11-11-2020 : Added the concept of a stack for loggingLevels (currently fixed to 4 elements depth). This allows you to temporarily change the loggingLevel and afterwards return to the previous level with simple push pop operation
// V2.2.0 05-12-2020 : Added output to std::cout when compiled for Win32 io Arduino target
// V2.3.0            : Added colored output
// V3.0.0 03-11-2021 : Added a concept of subsystem, where each subsystem has its own loggingLevel
// V4.0.0 27-01-2022 : Added the concept of configurable outputs and time provider

#pragma once

#include <stdarg.h>               // requires for variadic functions
#include "subsystems.h"           //
#include "logginglevels.h"        //
#include "logitem.h"
#include "logoutput.h"

class uLog {
  public:
    explicit uLog();        // constructor

    static constexpr uint32_t maxNmbrOutputs = 2;        //
    static constexpr uint32_t length         = 4;        // length of the items circular buffer for items

    // ------------------------------
    // configuring the logging object
    // ------------------------------
    void setOutput(uint32_t outputIndex, bool (*aFunction)(const char*));                                     // sets a pointer to a function handling the output of the logging to eg serial, network or file on SD card, etc.
    bool outputIsActive(uint32_t outputIndex);                                                                // is this output active
    void setTimeSource(bool (*aFunction)(char*, uint32_t));                                                   // sets a pointer to a function providing the timestamp prefix string.
    void setLoggingLevel(uint32_t outputIndex, subSystem theSubSystem, loggingLevel itemLoggingLevel);        //
    loggingLevel getLoggingLevel(uint32_t outputIndex, subSystem theSubSystem);                               //
    void setColoredOutput(uint32_t outputIndex, bool newSetting);                                             // set the colorize output option
    bool isColoredOutput(uint32_t outputIndex);                                                               // set the colorize output option
    void setIncludeTimestamp(uint32_t outputIndex, bool newSetting);                                          // set the includeTimestamp option
    bool hasTimestampIncluded(uint32_t outputIndex);                                                          // set the includeTimestamp option

    // ------------------------------
    // logging services
    // ------------------------------
    void log(subSystem theSubSystem, loggingLevel theLevel, const char* aText);                   // appends msg to loggingBuffer whithout trying to output immediately
    void output(subSystem theSubSystem, loggingLevel theLevel, const char* aText);                // appends msg and tries to output immediately - this output may be blocking
    void snprintf(subSystem theSubSystem, loggingLevel theLevel, const char* format, ...);        // does a printf() style of output to the logBuffer. It will truncate the output according to the space available in the logBuffer
    void flush();                                                                                 // outputs everything already in the buffer

    // ----------------------------------
    // internal data and helper functions
    // ----------------------------------

#ifndef unitTest
  private:
#endif
    bool checkLoggingLevel(subSystem theSubSystem, loggingLevel itemLoggingLevel) const;                              // check if this msg needs to be logged, comparing msg level vs logger level
    bool checkLoggingLevel(uint32_t outputIndex, subSystem theSubSystem, loggingLevel itemLoggingLevel) const;        // check if this msg needs to be sent to this output, comparing msg level vs logger level
    bool (*getTime)(char*, uint32_t){nullptr};                                                                        // pointer to function returning timestamp as a string

    logOutput outputs[maxNmbrOutputs];        // create a number of outputs, eg 2, one for serial, and one for network
    logItem items[length];                    // create an array of items to be logged
    uint32_t head{0};                         // readIndex of the items circular buffer
    uint32_t level{0};                        // filling level of the items circular buffer

    char contents[logItem::maxItemLength];        // in this cstring we will format the final contents for each output
    void format(uint32_t outputIndex);

    uint32_t pushItem();        // returns index of position where to write new item data..
    void popItem();
    void output();                                           // send to one or more outputs
    void addColorOutputPrefix(loggingLevel theLevel);        // add color output escape codes
    void addColorOutputPostfix();                            // add color output escape codes
    void addLevel(loggingLevel theLoggingLevel);
};
