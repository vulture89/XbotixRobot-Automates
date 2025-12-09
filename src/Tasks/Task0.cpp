#include "Globals.h"
#include "Tasks/Task0.h"
#include "Display.h"
#include "PoluluQTR/QTR.h"
#include "MotorControls.h"

// TODO: Add a button to reset the calibration

void executeTask0()
{
    bool calibrationSuccess = calibrateQTR();
    if (!calibrationSuccess)
    {
        displayMsg("Calibration FAILED!");
        taskManager.failCurrentTask();
    }

    displayMsg("Keep on start square!");
    delay(3000);

    bool onSquare = detectBlackSquare();
    if (!onSquare)
    {
        displayMsg("Square Detection FAILED!");
        taskManager.failCurrentTask();
    }

    bool roadFound = exitBlackSquare();
    if (!roadFound)
    {
        displayMsg("Road Detection FAILED!");
        taskManager.failCurrentTask();
    }

    displayMsg("Task 0 Complete!");
    delay(1000);

    taskManager.completeCurrentTask();
}

bool calibrateQTR()
{
    if (hasCalibrationData())
    {
        Serial.println("Loading calibration from EEPROM...");
        displayMsg("Loading QTR...");
        delay(1000);
        loadQTR();
        displayMsg("QTR Loaded!");
        delay(1500);
        return true;
    }
    else
    {
        displayMsg("Starting Calibration in 2s");
        delay(200);
        saveQTR();
        displayMsg("Calibration Complete!");
        delay(1500);
        return true;
    }
}

bool detectBlackSquare()
{
    displayMsg("Detecting Start square!");

    bool found = false;

    while (!found)
    {
        readMainQTR();
        readEdgeQTR();
        readTyreQTR();

        bool allMainBlack = true;
        for (uint8_t i = 0; i < MAIN_SENSOR_COUNT; i++)
        {
            if (!mainQTRDval[i])
            {
                allMainBlack = false;
            }
        }
        for (uint8_t i = 0; i < EDGE_SENSOR_COUNT; i++)
        {
            if (!edgeQTRDval[i])
            {
                allMainBlack = false;
            }
        }
        for (uint8_t i = 0; i < TYRE_SENSOR_COUNT; i++)
        {
            if (!tyreQTRDval[i])
            {
                allMainBlack = false;
            }
        }

        if (allMainBlack)
        {
            displayMsg("On Black Square!");
            delay(500);
            found = true;
            return true;
        }

        delay(50);
    }

    return false;
}

bool exitBlackSquare()
{
    displayMsg("Exiting black square!");

    bool roadFound = false;
    int attempts = 0;
    int maxAttempts = 50;

    while (!roadFound && attempts < maxAttempts)
    {
        forward(255, 255);
        delay(100);
        stop(50);

        readMainQTR();

        uint8_t blackCount = 0;
        for (uint8_t i = 0; i < MAIN_SENSOR_COUNT; i++)
        {
            if (mainQTRDval[i])
            {
                blackCount++;
            }
        }

        if (blackCount > 0 && blackCount < MAIN_SENSOR_COUNT)
        {
            roadFound = true;
            displayMsg("Road detected!");
            stop(200);
        }

        attempts++;
    }

    if (!roadFound)
    {
        displayMsg("Road not found!");
        stop(100);
        return false;
    }
    else
    {
        displayMsg("Ready to follow road!");
        delay(100);
        return true;
    }
}
