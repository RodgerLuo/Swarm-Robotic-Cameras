/**
  This example uses line sensors and infraed sensors for autonomous walking
  in a restricted area with obstacles.

  Rodger (Jieliang) Luo
  Feb 9th, 2017
*/

#include <Wire.h>
#include <Zumo32U4.h>

#define DIST_THRESHOLD     10.5  // brightness level, for infraed sensors
#define QTR_THRESHOLD     500  // microseconds, for line sensors

// These might need to be tuned for different motor types.
#define REVERSE_SPEED     200  // 0 is stopped, 400 is full speed
#define TURN_SPEED        200
#define FORWARD_SPEED     350
#define REVERSE_DURATION  200  // ms
#define TURN_DURATION     300  // ms

Zumo32U4LCD lcd;
Zumo32U4ButtonA buttonA;
Zumo32U4Buzzer buzzer;
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;
Zumo32U4ProximitySensors proxSensors;

#define NUM_SENSORS 3
unsigned int lineSensorValues[NUM_SENSORS];

void waitForButtonAndCountDown()
{
  ledYellow(1);
  lcd.clear();
  lcd.print(F("Press A"));

  buttonA.waitForButton();

  ledYellow(0);
  lcd.clear();

  // Play audible countdown.
  for (int i = 0; i < 3; i++)
  {
    delay(1000);
    buzzer.playNote(NOTE_G(3), 200, 15);
  }
  delay(1000);
  buzzer.playNote(NOTE_G(4), 500, 15);
  delay(1000);
}

void setup()
{
  // Uncomment if necessary to correct motor directions:
  //motors.flipLeftMotor(true);
  //motors.flipRightMotor(true);

  //Initial two groups of sensors
  proxSensors.initThreeSensors();
  lineSensors.initThreeSensors();

  waitForButtonAndCountDown();
}

void loop()
{
  if (buttonA.isPressed())
  {
    // If button is pressed, stop and wait for another press to
    // go again.
    motors.setSpeeds(0, 0);
    buttonA.waitForRelease();
    waitForButtonAndCountDown();
  }

  proxSensors.read();
  uint8_t sum = proxSensors.countsFrontWithRightLeds() + proxSensors.countsFrontWithLeftLeds();

  lineSensors.read(lineSensorValues);

  if (sum > DIST_THRESHOLD ||
      lineSensorValues[0] < QTR_THRESHOLD ||
      lineSensorValues[NUM_SENSORS - 1] < QTR_THRESHOLD)
  {
    // If leftmost sensor detects border, reverse and turn to the right.
    if (proxSensors.countsFrontWithRightLeds() < proxSensors.countsFrontWithLeftLeds() ||
        lineSensorValues[0] < QTR_THRESHOLD)
    {
      motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
      delay(REVERSE_DURATION);
      motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
      delay(TURN_DURATION);
      motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    }

    // If rightmost sensor detects border, reverse and turn to the left.
    else if(proxSensors.countsFrontWithRightLeds() > proxSensors.countsFrontWithLeftLeds() ||
        lineSensorValues[NUM_SENSORS - 1] < QTR_THRESHOLD)
    {
      motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
      delay(REVERSE_DURATION);
      motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
      delay(TURN_DURATION);
      motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    }
  }

  // Otherwise, go straight.
  else if (sum == 0)
  {
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
}
