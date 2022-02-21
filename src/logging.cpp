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
strlcat(char *dst, const char *src, size_t maxlen) {
    const size_t srclen = strlen(src);
    const size_t dstlen = strnlen(dst, maxlen);
    if (dstlen == maxlen) return maxlen + srclen;
    if (srclen < maxlen - dstlen) {
        memcpy(dst + dstlen, src, srclen + 1);
    } else {
        memcpy(dst + dstlen, src, maxlen - 1);
        dst[dstlen + maxlen - 1] = '\0';
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
                strcpy(items[newItemIndex].timestamp, "");
            }
        } else {
            strcpy(items[newItemIndex].timestamp, "");
        }
    }
}

void uLog::snprintf(subSystem theSubSystem, loggingLevel itemLoggingLevel, const char *format, ...) {
    if (checkLoggingLevel(theSubSystem, itemLoggingLevel)) {
        va_list argList;
        char buffer[logItem::maxItemLength];        // not initialized for performance
        va_start(argList, format);
        vsnprintf(buffer, logItem::maxItemLength, format, argList);
        va_end(argList);
        output(theSubSystem, itemLoggingLevel, buffer);
    }
}

void uLog::output(subSystem theSubSystem, loggingLevel itemLoggingLevel, const char *aText) {
    log(theSubSystem, itemLoggingLevel, aText);
    output();
}

void uLog::flush() {
    output();
}

void uLog::output() {
    if (level > 0) {                                           // if any items in buffer :
        bool success{false};                                   // remembering if any of the outputs worked
        for (uint32_t i = 0; i < maxNmbrOutputs; i++) {        // for all outputs
            if (outputs[i].isActive()) {                       // if this output is active..
                format(i);                                     // format the item according to the output's settings
                if (outputs[i].write(contents)) {              // and send it out
                    success = true;
                }
            }
        }
        if (success) {        // if any of the outputs succeeded...
            popItem();        // remove the item from the buffer
        }
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
        strcpy(contents, colorPrefix(items[head].theLoggingLevel));
    }
    if (outputs[outputIndex].hasTimestampIncluded()) {
        strcat(contents, items[head].timestamp);        // add timestamp string
    }
    strcat(contents, toStringShort(items[head].theLoggingLevel));
    uint32_t tmpLength = strnlen(contents, logItem::maxItemLength);

    // TODO show trunctation with trailing...

    if (outputs[outputIndex].isColoredOutput()) {
        strncat(contents, items[head].contents, (logItem::maxItemLength - (tmpLength + 5U)));        // add raw contents
        strcat(contents, colorPostfix());
    } else {
        strncat(contents, items[head].contents, (logItem::maxItemLength - (tmpLength + 1U)));        // add raw contents
    }
    strcat(contents, "\n");        // final newLine ??
}

void uLog::setTimeSource(bool (*aFunction)(char *, uint32_t)) {
    getTime = aFunction;
}

void uLog::setOutput(uint32_t outputIndex, bool (*aFunction)(const char *)) {
    if (outputIndex < maxNmbrOutputs) {
        outputs[outputIndex].setOutputDestination(aFunction);
    }
}

bool uLog::outputIsActive(uint32_t outputIndex) {
    if (outputIndex < maxNmbrOutputs) {
        return outputs[outputIndex].isActive();
    } else {
        return false;
    }
}

void uLog::setLoggingLevel(uint32_t outputIndex, subSystem theSubSystem, loggingLevel theLoggingLevel) {
    if (outputIndex < maxNmbrOutputs) {
        outputs[outputIndex].setLoggingLevel(theSubSystem, theLoggingLevel);
    }
}

void uLog::setColoredOutput(uint32_t outputIndex, bool newSetting) {
    if (outputIndex < maxNmbrOutputs) {
        outputs[outputIndex].setColoredOutput(newSetting);
    }
}

void uLog::setIncludeTimestamp(uint32_t outputIndex, bool newSetting) {
    if (outputIndex < maxNmbrOutputs) {
        outputs[outputIndex].setIncludeTimestamp(newSetting);
    }
}

loggingLevel uLog::getLoggingLevel(uint32_t outputIndex, subSystem theSubSystem) {
    if (outputIndex < maxNmbrOutputs) {
        return outputs[outputIndex].getLoggingLevel(theSubSystem);
    } else {
        return loggingLevel::None;
    }
}

bool uLog::isColoredOutput(uint32_t outputIndex) {
    if (outputIndex < maxNmbrOutputs) {
        return outputs[outputIndex].isColoredOutput();
    } else {
        return false;
    }
}

bool uLog::hasTimestampIncluded(uint32_t outputIndex) {
    if (outputIndex < maxNmbrOutputs) {
        return outputs[outputIndex].hasTimestampIncluded();
    } else {
        return false;
    }
}
