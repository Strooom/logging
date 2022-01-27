// #############################################################################
// ###                                                                       ###
// ### General Purpose Logging toolkit for MicroControllers                  ###
// ### https://github.com/Strooom/Logging                                    ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// ###                                                                       ###
// #############################################################################

#include <stdint.h>         // required for uint8_t and similar type definitions
#include <string.h>         // required for strncpy()
#include <stdio.h>          // required for vsnprintf()
#include "logging.h"        //


uLog::uLog() {
}

bool uLog::checkLoggingLevel(subSystem theSubSystem, loggingLevel itemLoggingLevel) const {
    bool result{false};
    for (uint32_t i = 0; i < maxNmbrOuputs; i++) {        // for all outputs
        if (outputs[i].isActive()) {
            if (itemLoggingLevel <= outputs[i].getLoggingLevel(theSubSystem)) {
                result = true;
            }
        }
    }
    return result;
}

void uLog::log(subSystem theSubSystem, loggingLevel itemLoggingLevel, const char* aText) {
    if (checkLoggingLevel(theSubSystem, itemLoggingLevel)) {
        uint32_t newItemIndex = pushItem();
        strncpy(items[newItemIndex].contents, aText, logItem::maxItemLength);
        items[newItemIndex].theLoggingLevel = itemLoggingLevel;
        items[newItemIndex].theSubSystem    = theSubSystem;

        if (getTime != nullptr) {
            if (getTime(items[newItemIndex].timestamp, logItem::timestampLength)) {
            } else {
            }
        }
    }
}

void uLog::output(subSystem theSubSystem, loggingLevel itemLoggingLevel, const char* aText) {
    log(theSubSystem, itemLoggingLevel, aText);
    output();
}

// void uLog::snprintf(subSystem theSubSystem, loggingLevel itemLoggingLevel, const char* format, ...) {
//     if (checkLoggingLevel(theSubSystem, itemLoggingLevel)) {
//         va_list argList;
//         char buffer[bufferLength];        // not initialized for performance
//         va_start(argList, format);
//         vsnprintf(buffer, bufferLength, format, argList);
//         va_end(argList);
//         output(theSubSystem, itemLoggingLevel, buffer);
//     }
// }

void uLog::flush() {
    output();
}

void uLog::output() {
    if (level > 0) {                                          // if any items in buffer :
        bool success{false};                                  // remembering if any of the outputs worked
        for (uint32_t i = 0; i < maxNmbrOuputs; i++) {        // for all outputs
            if (outputs[i].isActive()) {
                if (outputs[i].write(items[head].contents)) {
                    success = true;
                }
            }
        }
        if (success) {
            popItem();
        }
    }
}

void uLog::addColorOutputPrefix(loggingLevel itemLoggingLevel) {
    switch (itemLoggingLevel) {
        case loggingLevel::Critical:
            strncat(contents, "\033[37;41m", 10U);
            break;
        case loggingLevel::Error:
            strncat(contents, "\033[31;40m", 10U);
            break;
        case loggingLevel::Warning:
            strncat(contents, "\033[33;40m", 10U);
            break;
        case loggingLevel::Info:
            strncat(contents, "\033[36;40m", 10U);
            break;
        case loggingLevel::Debug:
            strncat(contents, "\033[37;40m", 10U);
            break;
        case loggingLevel::None:
        default:
            break;
    }
}

void uLog::addColorOutputPostfix() {
    if (strnlen(contents, logItem::maxItemLength) <= (logItem::maxItemLength - 4U)) {
        strncat(contents, "\033[0m", 4U);
    } else {
        strncpy((contents + (logItem::maxItemLength - 4U)), "\033[0m", 4U);
    }
}

void uLog::popItem() {
    head  = (head + 1) % length;
    level = level - 1;
}
uint32_t uLog::pushItem() {
    return 0;
}

void uLog::prepare(uint32_t outputIndex) {
    contents[0] = 0;        // clear temp buffer

    if (outputs[outputIndex].isColored()) {
        addColorOutputPrefix(items[head].theLoggingLevel);
    }

    if (outputs[outputIndex].hasTimestamp()) {
        strcat(contents, items[head].timestamp);        // add timestamp string
    }

    addLevel(items[head].theLoggingLevel);

    strncat(contents, items[head].contents, 100U);        // add raw contents

    if (outputs[outputIndex].isColored()) {
        addColorOutputPostfix();
    }

    strcat(contents, "\n");        // final newLine ??
}

void uLog::addLevel(loggingLevel theLoggingLevel) {
    switch (theLoggingLevel) {
        case loggingLevel::Critical:
            strcat(contents, "C ");
            break;
        case loggingLevel::Error:
            strcat(contents, "E ");
            break;
        case loggingLevel::Warning:
            strcat(contents, "W ");
            break;
        case loggingLevel::Info:
            strcat(contents, "I ");
            break;
        case loggingLevel::Debug:
            strcat(contents, "D ");
            break;
        case loggingLevel::None:
        default:
            break;
    }
}

void uLog::setTimeSource(bool (*aFunction)(char*, uint32_t)) {
    getTime = aFunction;
}

void uLog::setOutput(uint32_t outputIndex, bool (*aFunction)(const char*)) {
    outputs[outputIndex].setOutputDestination(aFunction);
}
