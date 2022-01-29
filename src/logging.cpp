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

#ifndef strlcpy
// on some platforms, string.h does not contain strlcpy, so in this case we add it here directly
size_t
strlcpy(char *dst, const char *src, size_t maxlen) {
    const size_t srclen = strlen(src);
    if (srclen + 1 < maxlen) {
        memcpy(dst, src, srclen + 1);
    } else if (maxlen != 0) {
        memcpy(dst, src, maxlen - 1);
        dst[maxlen - 1] = '\0';
    }
    return srclen;
}
#endif

#ifndef strlcat
// on some platforms, string.h does not contain strlcpy, so in this case we add it here directly
size_t
strlcat(char * dst, const char * src, size_t maxlen) {
    const size_t srclen = strlen(src);
    const size_t dstlen = strnlen(dst, maxlen);
    if (dstlen == maxlen) return maxlen+srclen;
    if (srclen < maxlen-dstlen) {
        memcpy(dst+dstlen, src, srclen+1);
    } else {
        memcpy(dst+dstlen, src, maxlen-1);
        dst[dstlen+maxlen-1] = '\0';
    }
    return dstlen + srclen;
}
#endif

uLog::uLog() {
}

bool uLog::checkLoggingLevel(subSystem theSubSystem, loggingLevel itemLoggingLevel) const {
    bool result{false};
    for (uint32_t i = 0; i < maxNmbrOutputs; i++) {        // for all outputs
        if (outputs[i].isActive()) {
            if (itemLoggingLevel <= outputs[i].getLoggingLevel(theSubSystem)) {
                result = true;
            }
        }
    }
    return result;
}

bool uLog::checkLoggingLevel(uint32_t outputIndex, subSystem theSubSystem, loggingLevel itemLoggingLevel) const {
    bool result{false};
    if (outputIndex < maxNmbrOutputs) {
        if (outputs[outputIndex].isActive()) {
            if (itemLoggingLevel <= outputs[outputIndex].getLoggingLevel(theSubSystem)) {
                result = true;
            }
        }
    }
    return result;
}

void uLog::log(subSystem theSubSystem, loggingLevel itemLoggingLevel, const char *aText) {
    if (checkLoggingLevel(theSubSystem, itemLoggingLevel)) {        // if any output is interested in this item, we store it in the buffer
        uint32_t newItemIndex = pushItem();
        strlcpy(items[newItemIndex].contents, aText, logItem::maxItemLength);
        items[newItemIndex].theLoggingLevel = itemLoggingLevel;
        items[newItemIndex].theSubSystem    = theSubSystem;

        if (getTime != nullptr) {
            if (getTime(items[newItemIndex].timestamp, logItem::timestampLength)) {
            } else {
            }
        }
    }
}

void uLog::output(subSystem theSubSystem, loggingLevel itemLoggingLevel, const char *aText) {
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
    if (level > 0) {                                           // if any items in buffer :
        bool success{false};                                   // remembering if any of the outputs worked
        for (uint32_t i = 0; i < maxNmbrOutputs; i++) {        // for all outputs
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
        strcat(contents, "\033[0m");
    } else {
        strncpy((contents + (logItem::maxItemLength - 4U)), "\033[0m", 4U);
    }
}

void uLog::popItem() {
    if (level > 0) {
        level = level - 1;
        head  = (head + 1) % length;
    }
}

uint32_t uLog::pushItem() {
    uint32_t newIndex = ((head + level) % length);
    if (level < length) {
        level = level + 1;
    }
    return newIndex;
}

void uLog::format(uint32_t outputIndex) {
    contents[0] = 0;        // clear temp buffer

    if (outputs[outputIndex].isColoredOutput()) {
        addColorOutputPrefix(items[head].theLoggingLevel);
    }

    if (outputs[outputIndex].hasTimestampIncluded()) {
        strcat(contents, items[head].timestamp);        // add timestamp string
    }

    addLevel(items[head].theLoggingLevel);

    strncat(contents, items[head].contents, 100U);        // add raw contents

    if (outputs[outputIndex].isColoredOutput()) {
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

void uLog::setTimeSource(bool (*aFunction)(char *, uint32_t)) {
    getTime = aFunction;
}

void uLog::setOutput(uint32_t outputIndex, bool (*aFunction)(const char *)) {
    if (outputIndex < maxNmbrOutputs) {
        outputs[outputIndex].setOutputDestination(aFunction);
    }
}

void uLog::setLoggingLevel(uint32_t outputIndex, subSystem theSubSystem, loggingLevel theLoggingLevel) {
    outputs[outputIndex].setLoggingLevel(theSubSystem, theLoggingLevel);
}

void uLog::setColoredOutput(uint32_t outputIndex, bool newSetting) {
    outputs[outputIndex].setColoredOutput(newSetting);
}

void uLog::setIncludeTimestamp(uint32_t outputIndex, bool newSetting) {
    outputs[outputIndex].setIncludeTimestamp(newSetting);
}

loggingLevel uLog::getLoggingLevel(uint32_t outputIndex, subSystem theSubSystem) {
    return outputs[outputIndex].getLoggingLevel(theSubSystem);
}

bool uLog::isColoredOutput(uint32_t outputIndex) {
    return outputs[outputIndex].isColoredOutput();
}

bool uLog::hasTimestampIncluded(uint32_t outputIndex) {
    return outputs[outputIndex].hasTimestampIncluded();
}
