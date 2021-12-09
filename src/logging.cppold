// #############################################################################
// ###                                                                       ###
// ### General Purpose Logging toolkit for MicroControllers                  ###
// ### https://github.com/Strooom/Logging                                    ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// ###                                                                       ###
// #############################################################################

#include "logging.h"

void uLog::pushLoggingLevel(loggingLevel newLevel) {
    theLoggingLevel[3U] = theLoggingLevel[2U];
    theLoggingLevel[2U] = theLoggingLevel[1U];
    theLoggingLevel[1U] = theLoggingLevel[0];
    theLoggingLevel[0] = newLevel;
}

loggingLevel uLog::popLoggingLevel() {
    theLoggingLevel[0] = theLoggingLevel[1U];
    theLoggingLevel[1U] = theLoggingLevel[2U];
    theLoggingLevel[2U] = theLoggingLevel[3U];
    return theLoggingLevel[0];
}

loggingLevel uLog::getLoggingLevel() const {
    return theLoggingLevel[0];
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

uLog::uLog(const loggingLevel initialLoggingLevel, const bool initialIncludeTimestamp) : includeTimestamp(initialIncludeTimestamp) {
    theLoggingLevel[0] = initialLoggingLevel;        // set the complete loggingLevel stack to the initial level
    theLoggingLevel[1] = initialLoggingLevel;        //
    theLoggingLevel[2] = initialLoggingLevel;        //
    theLoggingLevel[3] = initialLoggingLevel;        //
    logBuffer[0]       = 0x00;                       // initialize the logBuffer to empty string
}

void uLog::log(loggingLevel itemLoggingLevel, const char* aText) {
    if (checkLoggingLevel(itemLoggingLevel)) {
        uint32_t length{0};        // keeps track of length of string fragments, goes into strncat()

        if (coloredOutput) {
            colorOutputPrefix(itemLoggingLevel);
        }

        if (includeTimestamp) {
            logTimestamp();
        }

        char tmpStr[10U];        // temporary string to hold the description of the type of message

        switch (itemLoggingLevel) {
            case loggingLevel::Critical:
                strncpy(tmpStr, "Critical ", 10U);
                break;
            case loggingLevel::Error:
                strncpy(tmpStr, "Error    ", 10U);
                break;
            case loggingLevel::Warning:
                strncpy(tmpStr, "Warning  ", 10U);
                break;
            case loggingLevel::Info:
                strncpy(tmpStr, "Info     ", 10U);
                break;
            case loggingLevel::Debug:
                strncpy(tmpStr, "Debug    ", 10U);
                break;
            case loggingLevel::None:
            default:
                strcpy(tmpStr, "");
                break;
        }
        length = strnlen(tmpStr, 10U);
        if (checkLogBufferLevel(length)) {
            strncat(logBuffer, tmpStr, length);
            bufferLevel += length;
        }

        length = strnlen(aText, maxItemLength);
        if (checkLogBufferLevel(length)) {
            strncat(logBuffer, aText, length);
            bufferLevel += length;
        }

        if (coloredOutput) {
            colorOutputPostfix();
        }

        if (checkLogBufferLevel(1U)) {
            strcat(logBuffer, "\n");
            bufferLevel++;
        }
    }
}

void uLog::output(loggingLevel itemLoggingLevel, const char* aText) {
    log(itemLoggingLevel, aText);
    output();
}

void uLog::snprintf(loggingLevel itemLoggingLevel, const char* format, ...) {
    if (checkLoggingLevel(itemLoggingLevel)) {
        va_list argList;
        char buffer[512] = {0};
        va_start(argList, format);
        vsnprintf(buffer, 512, format, argList);
        va_end(argList);
        output(itemLoggingLevel, buffer);
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
    char tmpStr[20U];        // temporary string storage to prepare a timestamp string
    char spaces[20U];        // temporary string storage to prepare leading spaces
#ifdef WIN32
    strcpy(tmpStr, "time");        // on Windows unit testing, we put some dummy value here...
#else
    itoa(millis(), tmpStr, 10U);        // convert millis to a string
#endif
    uint32_t length     = strnlen(tmpStr, timestampLength);        // measure the length of the resulting string
    uint32_t nmbrSpaces = timestampLength - length;                // calculate how many leading spaces we need

    for (uint8_t x = 0; x < nmbrSpaces; x++) {
        spaces[x] = ' ';
    }
    spaces[nmbrSpaces] = 0x00;

    if (checkLogBufferLevel(timestampLength + 1U)) {
        strcat(logBuffer, spaces);
        strcat(logBuffer, tmpStr);
        strcat(logBuffer, "-");
        bufferLevel += (timestampLength + 1U);
    }
}

bool uLog::checkLoggingLevel(loggingLevel itemLoggingLevel) const {
    return (itemLoggingLevel <= theLoggingLevel[0]);
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
