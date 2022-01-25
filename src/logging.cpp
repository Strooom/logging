// #############################################################################
// ###                                                                       ###
// ### General Purpose Logging toolkit for MicroControllers                  ###
// ### https://github.com/Strooom/Logging                                    ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// ###                                                                       ###
// #############################################################################

#include <Arduino.h>
#include "logging.h"

void uLog::setLoggingLevel(loggingLevel newLevel) {
    for (uint8_t i = 0; i < static_cast<uint8_t>(subSystems::nmbrOfSubsystems); i++) {
        loggingLevels[i] = newLevel;
    }
}

void uLog::setLoggingLevel(subSystems theSubSystem, loggingLevel newLevel) {
    loggingLevels[static_cast<uint8_t>(theSubSystem)] = newLevel;
}

loggingLevel uLog::getLoggingLevel(subSystems theSubSystem) const {
    return loggingLevels[static_cast<uint8_t>(theSubSystem)];
}

void uLog::setOutputIsAvailable(bool newSetting) {
    outputIsAvailable = newSetting;
}

void uLog::setIncludeTimestamp(bool newSetting) {
    includeTimestamp = newSetting;
}

void uLog::setColoredOutput(bool newSetting) {
    coloredOutput = newSetting;
}

void uLog::setIndentLevel(uint32_t newLevel) {
    indentLevel = newLevel;
}

uint32_t uLog::getIndentLevel() {
    return indentLevel;
}

uLog::uLog() {
    logBuffer[0] = 0x00;        // initialize the logBuffer to empty string
}

void uLog::log(subSystems theSubSystem, loggingLevel itemLoggingLevel, const char* aText) {
    if (checkLoggingLevel(theSubSystem, itemLoggingLevel)) {
        uint32_t length = 0;        // keeps track of length of string fragments, goes into strncat()

        if (coloredOutput) {
            colorOutputPrefix(itemLoggingLevel);
        }

        if (includeTimestamp) {
            logTimestamp();
        }

        char tmpStr[17];        // temporary string to hold the description of the type of message

        switch (itemLoggingLevel) {
            case loggingLevel::Critical:
                strncpy(tmpStr, "Critical ", 16);
                break;
            case loggingLevel::Error:
                strncpy(tmpStr, "Error    ", 16);
                break;
            case loggingLevel::Warning:
                strncpy(tmpStr, "Warning  ", 16);
                break;
            case loggingLevel::Info:
                strncpy(tmpStr, "Info     ", 16);
                break;
            case loggingLevel::Debug:
                strncpy(tmpStr, "Debug    ", 16);
                break;
            case loggingLevel::None:
            default:
                strncpy(tmpStr, "", 16);
                break;
        }
        length = strnlen(tmpStr, 16);
        if (checkLogBufferLevel(length)) {
            strncat(logBuffer, tmpStr, length);
            bufferLevel += length;
        }

        if ((indentLevel > 0) && (checkLogBufferLevel(indentLevel))) {
            for (int i = 0; i < indentLevel; i++) {
                strncat(logBuffer, " ", 1);
            }
            bufferLevel += indentLevel;
        }

        length = strnlen(aText, bufferLength);
        if (checkLogBufferLevel(length)) {
            strncat(logBuffer, aText, length);
            bufferLevel += length;
        }

        if (coloredOutput) {
            colorOutputPostfix();
        }

        if (checkLogBufferLevel(1)) {
            strcat(logBuffer, "\n");
            bufferLevel++;
        }
    }
}

void uLog::output(subSystems theSubSystem, loggingLevel itemLoggingLevel, const char* aText) {
    log(theSubSystem, itemLoggingLevel, aText);
    output();
}

void uLog::snprintf(subSystems theSubSystem, loggingLevel itemLoggingLevel, const char* format, ...) {
    if (checkLoggingLevel(theSubSystem, itemLoggingLevel)) {
        va_list argList;
        char buffer[bufferLength];        // not initialized for performance
        va_start(argList, format);
        vsnprintf(buffer, bufferLength, format, argList);
        va_end(argList);
        output(theSubSystem, itemLoggingLevel, buffer);
    }
}

void uLog::flush() {
    output();
}

void uLog::output() {
    if (outputIsAvailable)        // only when output is available can we really send something there. If not we just keep it in the buffer for later..
    {
        if (bufferLevel > 0) {
#ifdef WIN32
            std::cout << logBuffer;
#else
            Serial.print(logBuffer);
#endif
            logBuffer[0] = 0x00;        // reset logBuffer to empty string : terminating zero
            bufferLevel  = 0;           // setting level back to zero
        }
    }
}

void uLog::logTimestamp() {
    char tmpStr[20];        // temporary string storage to prepare a timestamp string
    char spaces[20];        // temporary string storage to prepare leading spaces
#ifdef WIN32
    strcpy(tmpStr, "time");        // on Windows unit testing, we put some dummy value here...
#else
    itoa(millis(), tmpStr, 10);        // convert millis to a string
#endif
    uint32_t length     = strnlen(tmpStr, timestampLength);        // measure the length of the resulting string
    uint32_t nmbrSpaces = timestampLength - length;                // calculate how many leading spaces we need

    for (uint8_t x = 0; x < nmbrSpaces; x++) {
        spaces[x] = ' ';
    }
    spaces[nmbrSpaces] = 0x00;

    if (checkLogBufferLevel(timestampLength + 1)) {
        strcat(logBuffer, spaces);
        strcat(logBuffer, tmpStr);
        strcat(logBuffer, "-");
        bufferLevel += (timestampLength + 1);
    }
}

bool uLog::checkLoggingLevel(subSystems theSubSystem, loggingLevel itemLoggingLevel) const {
    return (itemLoggingLevel <= loggingLevels[static_cast<uint8_t>(theSubSystem)]);
}

bool uLog::checkLogBufferLevel(uint32_t itemLength) const {
    return (bufferLength >= (bufferLevel + itemLength));
}

void uLog::colorOutputPrefix(loggingLevel itemLoggingLevel) {
    if (checkLogBufferLevel(10U)) {
        switch (itemLoggingLevel) {
            case loggingLevel::Critical:
                strncat(logBuffer, "\033[37;41m", 10U);
                break;
            case loggingLevel::Error:
                strncat(logBuffer, "\033[31;40m", 10U);
                break;
            case loggingLevel::Warning:
                strncat(logBuffer, "\033[33;40m", 10U);
                break;
            case loggingLevel::Info:
                strncat(logBuffer, "\033[36;40m", 10U);
                break;
            case loggingLevel::Debug:
                strncat(logBuffer, "\033[37;40m", 10U);
                break;
            case loggingLevel::None:
            default:
                break;
        }
        bufferLevel += 8U;
    }
}

void uLog::colorOutputPostfix() {
    if (checkLogBufferLevel(4U)) {
        strncat(logBuffer, "\033[0m", 4U);
        bufferLevel += 4U;
    }
}
