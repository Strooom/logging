#define unitTest
#include <string.h>
#include <unity.h>
#include "logging.h"

bool outputFunction0(const char* contents) {
    TEST_ASSERT_EQUAL_STRING("lorem ipse", contents);
    return true;
}

bool outputFunction1(const char* contents) {
    TEST_MESSAGE(contents);
    uint32_t contentsLength = strlen(contents);
    TEST_ASSERT_LESS_OR_EQUAL(logItem::maxItemLength, contentsLength);
    return true;
}

bool loggingTime(char* contents, uint32_t length) {
    strcpy(contents, "2022-01-29T19:46:51Z");
    return true;
}

/*
2022-01-29T19:46:51+00:00
2022-01-29T19:46:51Z
20220129T194651Z
*/

void test_uLog_initialization() {
    uLog aLog;
    aLog.setOutput(0, outputFunction0);
    TEST_ASSERT_EQUAL(nullptr, aLog.getTime);
    aLog.setTimeSource(loggingTime);
    TEST_ASSERT_NOT_EQUAL(nullptr, aLog.getTime);
    aLog.setLoggingLevel(0, subSystem::general, loggingLevel::Warning);
    TEST_ASSERT_EQUAL_UINT8(loggingLevel::Warning, aLog.getLoggingLevel(0, subSystem::general));
}

void test_uLog_filtering() {
    uLog aLog;
    // filtering of an individual output
    aLog.setOutput(0, outputFunction0);                                                            // activate a first output
    aLog.setLoggingLevel(0, subSystem::general, loggingLevel::Warning);                            // give it some level/subsystem settings
    TEST_ASSERT_TRUE(aLog.checkLoggingLevel(0, subSystem::general, loggingLevel::Warning));        // same level should be logged
    TEST_ASSERT_TRUE(aLog.checkLoggingLevel(0, subSystem::general, loggingLevel::Error));          // higher level should be logged
    TEST_ASSERT_FALSE(aLog.checkLoggingLevel(0, subSystem::general, loggingLevel::Info));          // lower level should NOT be logged

    // general filtering taking into account all outputs
    TEST_ASSERT_TRUE(aLog.checkLoggingLevel(subSystem::general, loggingLevel::Warning));        // same level should be logged
    TEST_ASSERT_TRUE(aLog.checkLoggingLevel(subSystem::general, loggingLevel::Error));          // higher level should be logged
    TEST_ASSERT_FALSE(aLog.checkLoggingLevel(subSystem::general, loggingLevel::Info));          // lower level should NOT be logged

    // now add a second output with
    aLog.setOutput(1, outputFunction1);
    aLog.setLoggingLevel(1, subSystem::general, loggingLevel::Info);                            //
    TEST_ASSERT_TRUE(aLog.checkLoggingLevel(subSystem::general, loggingLevel::Info));           // same level should be logged
    TEST_ASSERT_TRUE(aLog.checkLoggingLevel(subSystem::general, loggingLevel::Warning));        // higher level should be logged
    TEST_ASSERT_TRUE(aLog.checkLoggingLevel(subSystem::general, loggingLevel::Error));          // higher level should be logged
    TEST_ASSERT_FALSE(aLog.checkLoggingLevel(subSystem::general, loggingLevel::Debug));         // lower level should NOT be logged
}

void test_uLog_circular_buffer() {
    uLog aLog;
    TEST_ASSERT_EQUAL_UINT32(0, aLog.level);        // empty after creation
    TEST_ASSERT_EQUAL_UINT32(0, aLog.head);

    uint32_t newIndex;

    for (int i = 0; i < aLog.length; i++) {            // fill the buffer
        TEST_ASSERT_EQUAL_UINT32(0, aLog.head);        // before pushing
        TEST_ASSERT_EQUAL_UINT32(i, aLog.level);
        newIndex = aLog.pushItem();
        TEST_ASSERT_EQUAL_UINT32(i, newIndex);        // after pushing
        TEST_ASSERT_EQUAL_UINT32(0, aLog.head);
        TEST_ASSERT_EQUAL_UINT32((i + 1), aLog.level);
    }
    newIndex = aLog.pushItem();
    TEST_ASSERT_EQUAL_UINT32(0, newIndex);        // test overflow
    TEST_ASSERT_EQUAL_UINT32(0, aLog.head);
    TEST_ASSERT_EQUAL_UINT32(aLog.length, aLog.level);

    for (int i = 0; i < aLog.length; i++) {        // empty the buffer
        TEST_ASSERT_EQUAL_UINT32(i, aLog.head);
        TEST_ASSERT_EQUAL_UINT32((aLog.length - i), aLog.level);
        aLog.popItem();
        TEST_ASSERT_EQUAL_UINT32((i + 1) % aLog.length, aLog.head);
        TEST_ASSERT_EQUAL_UINT32((aLog.length - (i + 1)), aLog.level);
    }
    aLog.popItem();        // test underflow
    TEST_ASSERT_EQUAL_UINT32(0, aLog.head);
    TEST_ASSERT_EQUAL_UINT32(0, aLog.level);
}

void test_uLog_output() {
    uLog aLog;
    aLog.setOutput(0, outputFunction1);
    aLog.setColoredOutput(0, false);
    aLog.setIncludeTimestamp(0, true);
    aLog.setLoggingLevel(0, subSystem::general, loggingLevel::Info);
    //    aLog.output(subSystem::general, loggingLevel::Info, "0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF");
    aLog.output(subSystem::general, loggingLevel::Info, "00 - 0123456789 01 - 0123456789 02 - 0123456789 03 - 0123456789 04 - 0123456789 05 - 0123456789 06 - 0123456789 07 - 0123456789 ");
}

void test_uLog_getTime() {
    uLog aLog;
    aLog.setOutput(0, outputFunction1);
    // aLog.setTimeSource(loggingTime);
    aLog.setColoredOutput(0, false);
    aLog.setIncludeTimestamp(0, true);
    aLog.setLoggingLevel(0, subSystem::general, loggingLevel::Info);
    //    aLog.output(subSystem::general, loggingLevel::Info, "0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF");
    aLog.output(subSystem::general, loggingLevel::Info, "00 - 0123456789 01 - 0123456789 02 - 0123456789 03 - 0123456789 04 - 0123456789 05 - 0123456789 06 - 0123456789 07 - 0123456789 ");
}

void test_uLog_boundaries() {
    uLog aLog;
    uint32_t indexOutOfBounds = aLog.maxNmbrOutputs;        // valid indexes are 0 .. (maxNmbrOutputs - 1)

    aLog.setOutput(indexOutOfBounds, outputFunction1);
    TEST_ASSERT_FALSE(aLog.outputIsActive(indexOutOfBounds));

    aLog.setLoggingLevel(indexOutOfBounds, subSystem::general, loggingLevel::Info);
    TEST_ASSERT_EQUAL(loggingLevel::None, aLog.getLoggingLevel(indexOutOfBounds, subSystem::general));

    aLog.setColoredOutput(indexOutOfBounds, true);
    TEST_ASSERT_FALSE(aLog.isColoredOutput(indexOutOfBounds));

    aLog.setIncludeTimestamp(indexOutOfBounds, true);
    TEST_ASSERT_FALSE(aLog.hasTimestampIncluded(indexOutOfBounds));
}

int main(int argc, char** argv) {
    UNITY_BEGIN();
    RUN_TEST(test_uLog_initialization);
    RUN_TEST(test_uLog_filtering);
    RUN_TEST(test_uLog_circular_buffer);
    RUN_TEST(test_uLog_output);
    RUN_TEST(test_uLog_getTime);
    RUN_TEST(test_uLog_boundaries);
    UNITY_END();
}