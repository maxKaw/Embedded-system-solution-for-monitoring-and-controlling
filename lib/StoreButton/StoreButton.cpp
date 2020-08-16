#include <StoreButton.h>
#include "Arduino.h"
#include "Button.h"

/**
 * The store button class which adds a new functionality to a button.
 * 
 * \authors Maksymilian Kawula, Ewan Porter
 * \version 1.0
 */
StoreButton::StoreButton(byte buttonPin)
{
    button = new Button(buttonPin);
    currentInterval = 0;
}

StoreButton::StoreButton()
{
}

/**
 * The method that is capable of checking whether the button was pressed
 * and performing an appropriate action.
 */
long StoreButton::checkInterval()
{
    ButtonStateEnum btnRead = button->checkStateHeld1Second();
    if (btnRead != btnCurrent)
    {
        btnCurrent = btnRead;
        switch (btnCurrent)
        {
        case ButtonStateEnum::ON:
            StoreButton::changeOutputInterval();
            break;
        case ButtonStateEnum::OFF:
            break;
        }
    }

    return outputIntervals[currentInterval];
}

/**
 * The method that is used for changing the current interval
 */
void StoreButton::changeOutputInterval()
{
    if (currentInterval < 5)
    {
        currentInterval++;
    }
    else
    {
        currentInterval = 0;
    }
}

/**
 * The method that is used for setting the intervals
 */
void StoreButton::setIntervals(long intervals[6])
{
    for (int i = 0; i < 6; i++)
    {
        outputIntervals[i] = intervals[i];
    }
}

