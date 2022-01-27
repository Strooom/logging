#include "logoutput.h"

logOutput::logOutput() {}

bool logOutput::isActive() const {
    return writeOutput != nullptr;
}

void logOutput::setLoggingLevel(loggingLevel newLevel) {
    for (uint8_t i = 0; i < static_cast<uint8_t>(subSystem::nmbrOfSubsystems); i++) {
        theLoggingLevel[i] = newLevel;
    }
}

void logOutput::setLoggingLevel(subSystem theSubSystem, loggingLevel newLevel) {
    theLoggingLevel[static_cast<uint8_t>(theSubSystem)] = newLevel;
}

loggingLevel logOutput::getLoggingLevel(subSystem theSubSystem) const {
    return theLoggingLevel[static_cast<uint8_t>(theSubSystem)];
}

void logOutput::setColoredOutput(bool newSetting) {
    colorOutput = newSetting;
}

void logOutput::setIncludeTimestamp(bool newSetting) {
    addTimestamp = newSetting;
}

void logOutput::setOutputDestination(bool (*aFunction)(const char *)) {
    writeOutput = aFunction;
}

bool logOutput::write(const char *theContents) const {
    return (*writeOutput)(theContents);
}

bool logOutput::isColoredOutput() const {
    return colorOutput;
}

bool logOutput::hasTimestampIncluded() const {
    return addTimestamp;
}