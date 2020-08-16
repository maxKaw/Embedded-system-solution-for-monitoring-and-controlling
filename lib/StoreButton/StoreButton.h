#ifndef StoreButton_h
#define StoreButton_h
#include "Arduino.h"
#include "Button.h"

/**
 * The store button class which adds a new functionality to a button.
 * 
 * \authors Maksymilian Kawula, Ewan Porter
 * \version 1.0
 */
class StoreButton
{
    long outputIntervals[6];
    unsigned long currentInterval;
    Button* button = NULL;
    ButtonStateEnum btnCurrent;
public:
    StoreButton();
    explicit StoreButton(byte buttonPin);
    void changeOutputInterval();
    void setIntervals(long intervals[6]);
    long checkInterval();
};

#endif /* StoreButton_h */
