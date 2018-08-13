  #include <Stepper.h>
  #include "trajectory_model.h"
  #include <math.h>
   
  // change this to the number of steps on your motor
  #define STEPS 200
  #define MAX_SPEED 100
  #define MIN_SPEED 20
  #define STEPS_PER_RAD (200 * 38 / (2 * PI))
  #define RAD_PER_SEC_TO_RPM (60 / (2 * PI))
   
  // create an instance of the stepper class, specifying
  // the number of steps of the motor and the pins it's
  // attached to
  Stepper stepper(STEPS, 1, 2, 3, 4);

  int targetIndex = 0;
  double currentAngle = HALF_PI; // Angle in radians
  double targetAngle = 0;
  double angleToTarget = 0;
  unsigned long currentTime = 0;
  unsigned long timeToTarget = 0;
  int stepperSpeed = 0;
  int numSteps = 0;
   
  void setup()
  {
    /*pinMode(0, OUTPUT); // sleep pin
    digitalWrite(0, HIGH); // enable stepper
    // set the speed of the motor to 30 RPMs
    stepper.setSpeed(30);
    // Step for 4x longer than it should require
    stepper.step(-1800);
    // Disable stepper to limit current draw
    digitalWrite(0, LOW);*/
  }
   
  void loop()
  {
    currentTime = millis();
    targetIndex = currentTime / 1000 + 2;
    if (targetIndex < 419) {
      timeToTarget = targetIndex * 1000 - currentTime;
      targetAngle = atan2(altitudes[targetIndex], ranges[targetIndex]);
      angleToTarget = targetAngle - currentAngle;
      
      stepperSpeed = angleToTarget / (timeToTarget / 1000.0) * RAD_PER_SEC_TO_RPM;
      if(stepperSpeed > MAX_SPEED){
        stepperSpeed = MAX_SPEED;
      }
      if(stepperSpeed < MIN_SPEED){
        stepperSpeed = MIN_SPEED;
      }
      stepper.setSpeed(stepperSpeed);
      
      numSteps = (int)(angleToTarget * STEPS_PER_RAD);
      stepper.step(numSteps);
      currentAngle += (double)numSteps / STEPS_PER_RAD;
    }
    delay(100);
  }
