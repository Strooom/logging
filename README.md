https://github.com/Strooom/logging/workflows/Arduino%20Library%20Checks/badge.svg
![GitHub release (latest by date)](https://img.shields.io/github/v/release/strooom/logging?color=green)

# logging
simple logging for Arduino framework

When developing on microcontrollers, it's often not easy to know what's going on inside.
A basic solution is to include 'print' statements in your code, showing where your code was running, what are variable values etc.
Still there are a number of problems with this approach :
* how to monitor what is going on before the output to print is available, eg. in the constructors. You need some kind of buffering until the output is available
* if you have many print statements, you'll have a lot of output, and it becomes harder to find the output you need. You need some kind of filtering.
* sometimes in time-critical parts of the code, doing a print can slow things down and change the behaviour. Here you need a quick way to log something, then you output it some later time.

This logging class provides a solution :
* It creates a global object, so it is available from the real start of your program
* It will buffer the logged items, until the output becomes available
* It can easily be adapt to any kind of output

V 1.0.0 25-08-2020 First release
V 2.0.0 08-10-2020 Made it simpler by providing less public methods, but making them more flexible. Did extensive unit testing for robustness.
Example of using snprintf :  theLog.snprintf(loggingLevel::Debug, "Error in %s on line %d\n", __FILE__, __LINE__);
V2.1.0 11-11-2020 : Added the concept of a stack for loggingLevels (currently fixed to 4 elements depth). This allows you to temporarily change the loggingLevel and afterwards return to the previous level with simple push pop operation
V2.2.0 05-12-2020 : Added output to std::cout when compiled for Win32 io Arduino target
