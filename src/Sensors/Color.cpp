#include "CnPD.h"

int rMin = 10000, gMin = 10000, bMin = 10000;
int rMax = 0, gMax = 0, bMax = 0;

void setupColorSensor()
{
    pinMode(COLOR_SENSOR_S0, OUTPUT);
    pinMode(COLOR_SENSOR_S1, OUTPUT);
    pinMode(COLOR_SENSOR_S2, OUTPUT);
    pinMode(COLOR_SENSOR_S3, OUTPUT);
    pinMode(COLOR_SENSOR_OUT, INPUT);

    switch (COLOR_SENSOR_SCALE)
    {
    case 2:
        digitalWrite(COLOR_SENSOR_S0, LOW);
        digitalWrite(COLOR_SENSOR_S1, LOW);
        break;
    case 20:
        digitalWrite(COLOR_SENSOR_S0, HIGH);
        digitalWrite(COLOR_SENSOR_S1, LOW);
        break;
    case 100:
        digitalWrite(COLOR_SENSOR_S0, LOW);
        digitalWrite(COLOR_SENSOR_S1, HIGH);
        break;
    case 1600:
        digitalWrite(COLOR_SENSOR_S0, HIGH);
        digitalWrite(COLOR_SENSOR_S1, HIGH);
        break;
    default:
        digitalWrite(COLOR_SENSOR_S0, HIGH);
        digitalWrite(COLOR_SENSOR_S1, LOW);
        break;
    }
}

String detectColor()
{
    digitalWrite(COLOR_SENSOR_S2, LOW);
    digitalWrite(COLOR_SENSOR_S3, LOW);
    delay(10);
    int redValue = pulseIn(COLOR_SENSOR_OUT, LOW);

    digitalWrite(COLOR_SENSOR_S2, HIGH);
    digitalWrite(COLOR_SENSOR_S3, HIGH);
    delay(10);
    int greenValue = pulseIn(COLOR_SENSOR_OUT, LOW);

    digitalWrite(COLOR_SENSOR_S2, LOW);
    digitalWrite(COLOR_SENSOR_S3, HIGH);
    delay(10);
    int blueValue = pulseIn(COLOR_SENSOR_OUT, LOW);

    if (redValue >= COLOR_SENSOR_RED_MIN && redValue <= COLOR_SENSOR_RED_MAX &&
        greenValue >= COLOR_SENSOR_GREEN_MIN && greenValue <= COLOR_SENSOR_GREEN_MAX &&
        blueValue >= COLOR_SENSOR_BLUE_MIN && blueValue <= COLOR_SENSOR_BLUE_MAX)
    {

        if (redValue < greenValue && redValue < blueValue)
        {
            return "Red";
        }
        else if (greenValue < redValue && greenValue < blueValue)
        {
            return "Green";
        }
        else if (blueValue < redValue && blueValue < greenValue)
        {
            return "Blue";
        }
        else
        {
            return "Unknown";
        }
    }
    else
    {
        return "Out of Range";
    }
}

void calibrate()
{
    Serial.println("Calibrating color sensor. Please place white and black surfaces in front of the sensor.");
    unsigned long start = millis();
    while (millis() - start < 5000)
    {
        int r = readRed();
        int g = readGreen();
        int b = readBlue();

        if (r < rMin)
            rMin = r;
        if (r > rMax)
            rMax = r;
        if (g < gMin)
            gMin = g;
        if (g > gMax)
            gMax = g;
        if (b < bMin)
            bMin = b;
        if (b > bMax)
            bMax = b;
    }

    Serial.println("Calibration done.");
}

int readRed()
{
    digitalWrite(COLOR_SENSOR_S2, LOW);
    digitalWrite(COLOR_SENSOR_S3, LOW);
    delay(10);
    return pulseIn(COLOR_SENSOR_OUT, LOW);
}

int readGreen()
{
    digitalWrite(COLOR_SENSOR_S2, HIGH);
    digitalWrite(COLOR_SENSOR_S3, HIGH);
    delay(10);
    return pulseIn(COLOR_SENSOR_OUT, LOW);
}

int readBlue()
{
    digitalWrite(COLOR_SENSOR_S2, LOW);
    digitalWrite(COLOR_SENSOR_S3, HIGH);
    delay(10);
    return pulseIn(COLOR_SENSOR_OUT, LOW);
}