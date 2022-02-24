// #############################################################################
// ###                                                                       ###
// ### General Purpose Logging toolkit for MicroControllers                  ###
// ### https://github.com/Strooom/Logging                                    ###
// ### Author(s) : Pascal Roobrouck - @strooom                               ###
// ### License : https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode ###
// ###                                                                       ###
// #############################################################################

#include <Arduino.h>
#include <logging.h>        // Step 1. add an #include to include the library into your project

uLog theLog;        // Step 2. Create a global object to log things, before any other code or global objects.

bool outputToSerial(const char* contents) {        // Step 3. Add a function which sends the output of the logging to the right hw channel, eg Serial port
    size_t nmbrBytesSent;
    nmbrBytesSent = Serial.print(contents);
    if (nmbrBytesSent > 0) {
        return true;
    } else {
        return false;
    }
}

bool loggingTime(char* contents, uint32_t length) {        // Step 4. Add a function which generates a timestamp string, so your logging events can get timestamped. Simple example is using millis()
    itoa(millis(), contents, 10);                          // convert millis to a string
    return true;
}

void setup() {
    theLog.setTimeSource(loggingTime);        // Step 5. Tell theLog to get its input for timestamps, from this function, defined somewhere in your application
    theLog.setLoggingLevel(0, subSystem::general, loggingLevel::Debug);
    theLog.setColoredOutput(0, true);
    theLog.output(subSystem::general, loggingLevel::Info, "Entering Setup()");

    Serial.begin(115200);        //		Configure Serial, which is the output channel for this example
    Serial.flush();              //		Clean up the Serial output
    delay(3000);
    theLog.setOutput(0U, outputToSerial);                                                // Step 4. Tell theLog to send its output to the "outputToSerial(char * contents)" function in this application
    theLog.output(subSystem::general, loggingLevel::Info, "Exiting Setup(\n\n)");        // Step 6. Adds another msg to the log, then outputs everyting
    theLog.output(subSystem::general, loggingLevel::Debug, "Some output");               // Step 6. Adds another msg to the log, then outputs everyting
    theLog.output(subSystem::general, loggingLevel::Info, "Some output");                // Step 6. Adds another msg to the log, then outputs everyting
    theLog.output(subSystem::general, loggingLevel::Warning, "Some output");             // Step 6. Adds another msg to the log, then outputs everyting
    theLog.output(subSystem::general, loggingLevel::Error, "Some output");               // Step 6. Adds another msg to the log, then outputs everyting
    theLog.output(subSystem::general, loggingLevel::Critical, "Some output");            // Step 6. Adds another msg to the log, then outputs everyting
}

void loop() {
    // delay(1000);
    // Serial.print(".");
}
