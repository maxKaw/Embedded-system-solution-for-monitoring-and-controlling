#include <Arduino.h>
#include <Temperature.h>
#include <CurrentRangeStateEnum.h>
#include <Buzzer.h>
#include <Humidity.h>
#include <CurrentRangeStateEnum.h>
#include <SDReader.h>
#include <unity.h>



//Test for Buzzer
void test_checkStatus_LED_Amber() {
    const byte a = 01;
    Buzzer* buzzer;
    buzzer = new Buzzer(a, a);
    BuzzerStateEnum buzzerState = buzzer->checkStatus(CurrentRangeStateEnum::AMBER, true);

    TEST_ASSERT_EQUAL(buzzerState, BuzzerStateEnum::AMBER_BUZZER);

    delete buzzer;
}

//Test for Buzzer
void test_checkStatus_LED_Green() {
    const byte a = 01;
    Buzzer* buzzer;
    buzzer = new Buzzer(a, a);
    BuzzerStateEnum buzzerState = buzzer->checkStatus(CurrentRangeStateEnum::GREEN, true);

    TEST_ASSERT_EQUAL(buzzerState, BuzzerStateEnum::NONE);

    delete buzzer;
}

//Test for Buzzer
void test_checkStatus_LED_Red() {
    const byte a = 01;
    Buzzer* buzzer;
    buzzer = new Buzzer(a, a);
    BuzzerStateEnum buzzerState = buzzer->checkStatus(CurrentRangeStateEnum::RED, true);

    TEST_ASSERT_EQUAL(buzzerState, BuzzerStateEnum::RED_BUZZER);

    delete buzzer;
}

//Test for temperature
void test_temperatureCheckRangeState_Red_Below() {
    Temperature* temp;
    temp = new Temperature();

    temp->setCurrentTemp(15);
    CurrentRangeStateEnum rangeState = temp->checkRangeTemp();

    TEST_ASSERT_EQUAL(rangeState, CurrentRangeStateEnum::RED);

    delete temp;
}

//Test for temperature
void test_temperatureCheckRangeState_Red_Above() {
    Temperature* temp;
    temp = new Temperature();

    temp->setCurrentTemp(28);
    CurrentRangeStateEnum rangeState = temp->checkRangeTemp();

    TEST_ASSERT_EQUAL(rangeState, CurrentRangeStateEnum::RED);

    delete temp;
}

//Test for temperature
void test_temperatureCheckRangeState_Amber() {
    Temperature* temp;
    temp = new Temperature();

    temp->setCurrentTemp(16);
    CurrentRangeStateEnum rangeState = temp->checkRangeTemp();

    TEST_ASSERT_EQUAL(rangeState, CurrentRangeStateEnum::AMBER);

    delete temp;
}

//Test for temperature
void test_temperatureCheckRangeState_Green() {
    Temperature* temp;
    temp = new Temperature();

    temp->setCurrentTemp(24);
    CurrentRangeStateEnum rangeState = temp->checkRangeTemp();

    TEST_ASSERT_EQUAL(rangeState, CurrentRangeStateEnum::GREEN);

    delete temp;
}


//Test for humidity
void test_humidityCheckRangeState_Red_Below() {
    Humidity* hum;
    hum = new Humidity();

    hum->setCurrentHum(23);
    CurrentRangeStateEnum rangeState = hum->checkRange();

    TEST_ASSERT_EQUAL(rangeState, CurrentRangeStateEnum::RED);

    delete hum;
}

//Test for humidity
void test_humidityCheckRangeState_Red_Above() {
    Humidity* hum;
    hum = new Humidity();

    hum->setCurrentHum(80);
    CurrentRangeStateEnum rangeState = hum->checkRange();

    TEST_ASSERT_EQUAL(rangeState, CurrentRangeStateEnum::RED);

    delete hum;
}

//Test for humidity
void test_humidityCheckRangeState_Amber() {
    Humidity* hum;
    hum = new Humidity();

    hum->setCurrentHum(30);
    CurrentRangeStateEnum rangeState = hum->checkRange();

    TEST_ASSERT_EQUAL(rangeState, CurrentRangeStateEnum::AMBER);

    delete hum;
}

//Test for humidity
void test_humidityCheckRangeState_Green() {
    Humidity* hum;
    hum = new Humidity();

    hum->setCurrentHum(43);
    CurrentRangeStateEnum rangeState = hum->checkRange();

    TEST_ASSERT_EQUAL(rangeState, CurrentRangeStateEnum::GREEN);

    delete hum;
}

//Test for SD Card reader
void test_SDReader() {
    SDReader* sdReader;
    sdReader = new SDReader();
    const byte pin = 5;
    bool set = true;
    bool test;

    try{
        sdReader->SDSetup(pin);
        test = true;
    }
    catch (const std::exception &e){
        test = false;
    }

    TEST_ASSERT_EQUAL(set, test);

    delete sdReader;
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_temperatureCheckRangeState_Red_Below);
    RUN_TEST(test_temperatureCheckRangeState_Red_Above);
    RUN_TEST(test_temperatureCheckRangeState_Amber);
    RUN_TEST(test_temperatureCheckRangeState_Green);

    RUN_TEST(test_humidityCheckRangeState_Red_Below);
    RUN_TEST(test_humidityCheckRangeState_Red_Above);
    RUN_TEST(test_humidityCheckRangeState_Amber);
    RUN_TEST(test_humidityCheckRangeState_Green);


    RUN_TEST(test_checkStatus_LED_Amber);
    RUN_TEST(test_checkStatus_LED_Red);
    RUN_TEST(test_checkStatus_LED_Green);

    RUN_TEST(test_SDReader);

    UNITY_END();
}

void loop() {

}

