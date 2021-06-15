// #############################################################################
// ###                                                                       ###
// ### General Purpose Logging toolkit for MicroControllers                  ###
// ### https://github.com/Strooom/Logging                                    ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// ###                                                                       ###
// #############################################################################

#include <Arduino.h>
#include <logging.h>

uLog theLog;

void setup() {
    Serial.begin(115200);
    Serial.flush();
    Serial.println();
    delay(500);

    theLog.setOutputIsAvailable(true);
    theLog.setColoredOutput(true);                                    // enable colored output - remember to set 'monitor_flags = --raw' in platformio.ini
    theLog.output(loggingLevel::Info, "This is some info");           //
    theLog.output(loggingLevel::Warning, "This is a warning");        //
    theLog.output(loggingLevel::Error, "This is an error");           //
}

void loop() {
}
