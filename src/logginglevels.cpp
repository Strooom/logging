#include "logginglevels.h"

const char* toString(loggingLevel aLoggingLevel) {
    switch (aLoggingLevel) {
        case loggingLevel::None:
            return "None";
            break;
        case loggingLevel::Critical:
            return "Critical";
            break;
        case loggingLevel::Error:
            return "Error";
            break;
        case loggingLevel::Warning:
            return "Warning";
            break;
        case loggingLevel::Info:
            return "Info";
            break;
        case loggingLevel::Debug:
            return "Debug";
            break;
        case loggingLevel::nmbrLoggingLevels:
        default:
            return "";
            break;
    }
}

const char* toStringShort(loggingLevel aLoggingLevel) {
    switch (aLoggingLevel) {
        case loggingLevel::None:
            return " N ";
            break;
        case loggingLevel::Critical:
            return " C ";
            break;
        case loggingLevel::Error:
            return " E ";
            break;
        case loggingLevel::Warning:
            return " W ";
            break;
        case loggingLevel::Info:
            return " I ";
            break;
        case loggingLevel::Debug:
            return " D ";
            break;
        case loggingLevel::nmbrLoggingLevels:
        default:
            return " ";
            break;
    }
}

// https://misc.flogisoft.com/bash/tip_colors_and_formatting


const char* colorPrefix(loggingLevel itemLoggingLevel) {
    switch (itemLoggingLevel) {
        case loggingLevel::Critical:
            return "\033[31;1m";
            break;
        case loggingLevel::Error:
            return "\033[31;40m";
            break;
        case loggingLevel::Warning:
            return "\033[33;40m";
            break;
        case loggingLevel::Info:
            return "\033[32;40m";
            break;
        case loggingLevel::Debug:
            return "\033[37;40m";
            break;
        default:
            return "";
            break;
    }
}

const char* colorPostfix() {
    return "\033[0m";
}
