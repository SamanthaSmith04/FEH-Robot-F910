/* =========================================================================
*                       FEH Robot Project Group 910F
*
*Group Members: Searita Chen, Bennett Godinho-Nelson, Samantha Smith, John Ulm
*
*                             Robot Code
===========================================================================*/

/* =========================================================================
*                                PORTS
*   MOTORS
*       LEFT DRIVE         - MOTOR 0
*       RIGHT DRIVE        - MOTOR 1
*       ARM MOTOR          - MOTOR 2
*   SERVOS
*       DROPPER SERVO      -
*   ENCODERS
*       LEFT ENCODER       - P0_0
*       RIGHT ENCODER      - P0_1
*   OTHER COMPONENTS
*       CDS CELL (COLOR)   - P1_0
*       OPTO SENSOR R      - P1_1
*       OPTO SENSOR M      - P1_2
*       OPTO SENSOR L      - P1_3
*       BUMP SWITCH FRONT  -
*       BUMP SWITCH BACK L -
*       BUMP SWITCH BACK R -
===========================================================================*/

// IMPORTS
#include <FEHLCD.h>
#include <FEHMotor.h>
#include <FEHServo.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include<ctime>
#include <FEHRPS.h>

// CONSTANTS
#define RIGHT_MOTOR_PORT FEHMotor::Motor1
#define LEFT_MOTOR_PORT FEHMotor::Motor0
#define ARM_MOTOR_PORT FEHMotor::Motor2
#define DROPPER_SERVO_PORT FEHServo::Servo0
#define CDS_SENSOR_PORT FEHIO::P1_0 // CDS for COLOR
#define CDS_R_PORT FEHIO::P1_1
#define CDS_M_PORT FEHIO::P1_2
#define CDS_L_PORT FEHIO::P1_3
#define BUMP_SWITCH_PORT FEHIO::P0_1
#define CIRCUM (2 * 3.1415 * 3)
#define RIGHT_ENCODER_PORT FEHIO::P0_1
#define LEFT_ENCODER_PORT FEHIO::P0_0
#define FRONT_BUMP_PORT FEHIO::P3_0
#define RIGHT_BUMP_PORT FEHIO::P1_6
#define LEFT_BUMP_PORT FEHIO::P1_7
#define RED_VALUE 0.18
#define BLUE_VALUE 0.80 // Arbitrary Blue Value
#define ROBOT_WIDTH 7.625
#define PI 3.14159
#define SERVO_MIN 1000
#define SERVO_MAX 2075
#define SERVO_CLOSED 180
#define SERVO_OPEN 45

// COMPONENTS
DigitalEncoder right_encoder(RIGHT_ENCODER_PORT);
DigitalEncoder left_encoder(LEFT_ENCODER_PORT);
FEHMotor right_motor(RIGHT_MOTOR_PORT, 9.0);
FEHMotor left_motor(LEFT_MOTOR_PORT, 9.0);
FEHMotor arm_motor(ARM_MOTOR_PORT, 7.2);
AnalogInputPin cdsCell(CDS_SENSOR_PORT);
DigitalInputPin frontBump(FRONT_BUMP_PORT);
DigitalInputPin leftBump(LEFT_BUMP_PORT);
DigitalInputPin rightBump(RIGHT_BUMP_PORT);
FEHServo dropperServo(DROPPER_SERVO_PORT); //1000 open, 2075 closed

// GLOBAL VARIABLES
int x, y;

// FUNCTION HEADERS
void testCDS();
void checkPoint1Code();
void checkPoint2Code();
void checkPoint3Code();
void checkPoint4Code();
void checkPoint5Code();
void moveForward(int, double);
void moveBackward(int, double);
void moveBackward(int);
void stopDriving();
void rotateLeft(int, double);
void driveTest();
void rotateRight(int, double);
void startToRampTopR(int);
void moveArm(int, double);
void moveUntilBump(int, int);

int main()
{
    //initialize all values
    dropperServo.SetMin(SERVO_MIN);
    dropperServo.SetMax(SERVO_MAX); 


    dropperServo.SetDegree(SERVO_CLOSED); 

    RPS.InitializeTouchMenu();
    checkPoint4Code();
    checkPoint5Code();

}

/*
DRIVING TEST CODE
*/
void driveTest()
{
    int percent = 25;
    LCD.SetBackgroundColor(BLACK);
    // forward
    LCD.Write("Going forward 6in");
    moveForward(percent, 6);
    LCD.Clear();
    LCD.Write("Stopping");
    Sleep(0.5);

    // backward
    LCD.Write("Going backward 6in");
    moveBackward(percent, 6);
    LCD.Clear();
    LCD.Write("Stopping");
    Sleep(0.5);

    // turn right
    LCD.Write("Turning right 90 degrees");
    rotateRight(percent, 90);
    LCD.Clear();
    LCD.Write("Stopping");
    Sleep(0.5);

    // turn left
    LCD.Write("Turning right 90 degrees");
    rotateLeft(percent, 90);
    LCD.Clear();
    LCD.Write("Stopping");
    Sleep(0.5);
}


/*
Used for testing the CDS cell values and prints the value to the screen
USED FOR TESTING PURPOSES ONLY, NOT FOR CHECKING COLORS WHILE ROBOT IS RUNNING
*/
void testCDS()
{
    while (1 == 1)
    {
        // display color to screen
        if (cdsCell.Value() < RED_VALUE)
        {
            LCD.SetBackgroundColor(RED);
        }
        else if (cdsCell.Value() < BLUE_VALUE)
        {
            LCD.SetBackgroundColor(BLUE);
        }
        else
        {
            LCD.SetBackgroundColor(YELLOW);
        }
        LCD.Clear();
        LCD.Write(cdsCell.Value());
        Sleep(0.2);
    }
}

/*
The main drive code for checkpoint 1
Drives up the ramp, touches the kiosk, and drives back down the ramp
*/
void checkPoint1Code()
{
    // ANY PRE-DRIVE SETUP STUFF
    int motor_percent = 40;
    double first_movement = 4;
    double first_turn = 30;
    double second_movement = 34;
    double third_movement = 10;
    double fourth_movement = 20;

    // backing up
    double backup_turn = 30;
    double backup1 = 25;
    double backup_turn_2 = 60;
    double backup2 = 20;

    // WAIT FOR START LIGHT
    while (cdsCell.Value() > RED_VALUE)
    {
    }
    // GO
    //
    moveForward(motor_percent, first_movement);

    // turn to ramp
    rotateLeft(motor_percent, first_turn);
    Sleep(0.3);

    // go up ramp
    moveForward(motor_percent, second_movement);
    Sleep(0.3);

    // turn 90
    rotateLeft(motor_percent, 90);
    Sleep(0.3);

    // go forward
    moveForward(motor_percent, third_movement);
    Sleep(0.3);

    // turn 90
    rotateRight(motor_percent, 90);
    Sleep(0.3);

    // move straight
    moveForward(motor_percent, fourth_movement);
    Sleep(0.3);

    // BACK
    rotateRight(motor_percent, backup_turn);
    Sleep(0.3);

    moveBackward(motor_percent, backup1);
    Sleep(0.3);

    rotateLeft(motor_percent, backup_turn_2);
    Sleep(0.3);

    moveBackward(motor_percent, backup2);
}

/*
The main drive code for checkpoint 2
Drives up the ramp, touches the kiosk, and drives back down the ramp
*/
void checkPoint2Code()
{
    int motor_percent = 35;
    /*unused distances
    int moveForward1 = 0; // move towards wall - A
    int turn1 = 0; //turn towards ramp - TA
    int moveForward2 = 0; // move up ramp - B*/

    int turn2 = 90;            // turn towards left side wall - TB
    int moveForward3 = 19;     // move towards left side wall - C
    int turn3 = 90;            // turn towards light sensor - TC
    int moveForward4 = 16;     // move towards light - D //may be eliminated
    int backUp5 = 10;          // back up from light - E
    int turn5 = 90;            // turn to right wall - TE
    int moveForwardBlue = 6;   // move distance needed for blue button - F
    int turnBlue = 90;         // turn to blue button - TF
    int moveForwardBlue2 = 15; // move to blue button - H
    int moveForwardRed = 9;    // move distance needed for red button - G
    int turnRed = 90;          // turn towards red button - TG
    int moveForwardRed2 = 15;  // move to red button - I

    // move from the start to the top of the right ramp, end facing towards top of field
    startToRampTopR(motor_percent);
    Sleep(0.1);

    rotateLeft(motor_percent, turn2);
    Sleep(0.5);

    moveBackward(motor_percent);
    Sleep(1.0);
    stopDriving();
    Sleep(0.1);

    moveForward(motor_percent, moveForward3);
    Sleep(0.1);

    rotateRight(motor_percent, turn3);
    Sleep(0.1);

    moveForward(motor_percent, moveForward4); // LIGHT IS CHECKED HERE

    int side;
    double CDSValue = cdsCell.Value();

    // display color to screen
    if (CDSValue <= RED_VALUE)
    {
        LCD.SetBackgroundColor(RED);
        side = 1;
    }
    else if (CDSValue <= BLUE_VALUE)
    {
        LCD.SetBackgroundColor(BLUE);
        side = 0;
    }
    else
    {
        LCD.SetBackgroundColor(BLUEVIOLET);
        side = 0; // default to blue
    }
    LCD.Clear();
    LCD.Write(CDSValue);
    Sleep(0.2);

    moveBackward(motor_percent, backUp5);
    Sleep(0.2);

    rotateRight(motor_percent, turn5);
    Sleep(0.2);

    moveBackward(motor_percent + 15);
    Sleep(1.0);
    stopDriving();
    Sleep(0.2);

    moveForward(motor_percent, 4);

    // robot now decides which button to go to

    if (side == 0)
    {
        moveForward(motor_percent, moveForwardBlue);
        Sleep(0.2);
        rotateLeft(motor_percent, turnBlue);
        Sleep(0.2);
        moveForward(motor_percent, moveForwardBlue2);
        Sleep(0.2);
    }
    else if (side == 1)
    {
        moveForward(motor_percent, moveForwardRed);
        Sleep(0.2);
        rotateLeft(motor_percent, turnRed);
        Sleep(0.2);
        moveForward(motor_percent, moveForwardRed2);
        Sleep(0.2);
    }

    Sleep(0.5);
    // moveBackward(motor_percent, 5);
}

/*
The main drive code for checkpoint 3
Drives towards the fuel levers, checks for which lever is to be flipped, flips lever down, waits 5 seconds, flips lever back up
SETUP: Robot is touching right side on stop button, 4 in away from and parallel to the wall
*/
void checkPoint3Code()
{
    int fuelLever = RPS.GetCorrectLever();
    LCD.Write(fuelLever);
    int motor_percent = 35;
    int arm_percent = 45;
    double arm_time = 0.6;
    int driveForward = 10;                 // move towards ramp
    int turn1 = 90;                        // turn to face left wall
    int driveToLever1 = 14;                // drive to fuel lever 1 position
    int driveToLever2 = driveToLever1 + 4; // drive to fuel lever 2 position
    int driveToLever3 = driveToLever1 + 8; // drive to fuel lever 3 position
    int turnToLevers = 90;                 // turn towards the levers
    int moveToLever = 2;                   // drive forward to lever

    Sleep(0.4);
    // WAIT FOR START LIGHT
    while (cdsCell.Value() > RED_VALUE)
    {
    }

    moveForward(motor_percent, driveForward);
    Sleep(0.2);

    rotateLeft(motor_percent, turn1);
    Sleep(0.1);

    moveBackward(motor_percent + 10); // get lined up against wall
    Sleep(0.5);
    stopDriving();

    Sleep(0.2);
    if (fuelLever == 0)
    {
        moveForward(motor_percent, driveToLever1);
    }
    else if (fuelLever == 1)
    {
        moveForward(motor_percent, driveToLever2);
    }
    else if (fuelLever == 2)
    {
        moveForward(motor_percent, driveToLever3);
    }
    Sleep(0.2);

    rotateLeft(motor_percent, turnToLevers);
    Sleep(0.2);

    moveForward(motor_percent, moveToLever);
    Sleep(0.2);

    moveArm(arm_percent, arm_time);  // arm is now down
    moveArm(-arm_percent, arm_time); // arm is now up
    moveBackward(motor_percent, moveToLever);

    Sleep(5.0);
    moveArm(arm_percent, arm_time);
    moveForward(motor_percent, moveToLever);

    Sleep(0.2);
    moveArm(-arm_percent, arm_time); // arm is now up

    moveBackward(motor_percent, moveToLever);
}

void checkPoint4Code()
{
    int motor_percent = 35;
    int arm_percent = 45;
    double arm_time = 0.4;

    int moveForward1 = 5;
    int turnToLever = 90;
    int moveToLever = 4;

    startToRampTopRWithRPS(motor_percent); //USING RPS VALUES
    Sleep(0.2);
    rotateLeft(motor_percent, 90);
    Sleep(0.5);
    moveUntilBump(-motor_percent, 2);
    Sleep(0.2);

    moveForward(motor_percent, moveForward1);
    Sleep(0.2);
    moveArm(arm_percent, arm_time);  // arm is now down
    rotateRight(motor_percent, turnToLever);
    Sleep(0.2);

    moveForward(motor_percent, moveToLever);
    Sleep(0.2);

    moveArm(-arm_percent * 2, arm_time/2);
}

void checkPoint5Code(){
    int motor_percent = 35;

    int backUpFromPassport = 5;

    int moveForwardToDropper = 9;
    int turnTowardsDropper =  90;

    int backupFromDropper = 5;
    int turnToLWall = 90;
    int moveAwayFromWall = 5;
    int turnToRamp = 90;
    int driveDownRamp = 10;
    int turnToRightWall = 0;
    int moveToRightWall = 0;
    int turnToButton = 0; //WONT BE 90
    int driveToButton = 0;

    moveBackward(motor_percent, backUpFromPassport);
    Sleep(0.2);
    rotateLeft(motor_percent, 90);
    Sleep(0.2);
    moveForward(motor_percent, moveForwardToDropper);
    Sleep(0.2);
    rotateRight(motor_percent, 90);
    Sleep(0.2);
    moveUntilBump(-motor_percent, 3);
    Sleep(0.2);
    dropperServo.SetDegree(SERVO_OPEN);
    Sleep(0.4);
    //LUGGAGE TASK NOW COMPLETE
    moveForward(motor_percent, backupFromDropper);
    Sleep(0.2);
    rotateRight(motor_percent, turnToLWall);
    Sleep(0.2);
    moveBackward(motor_percent, 10);
    Sleep(0.2);
    moveUntilBump(-motor_percent, 2); //now touching l wall
    Sleep(0.2);
    moveForward(motor_percent, moveAwayFromWall);
    Sleep(0.2);
    rotateRight(motor_percent, turnToButton);
    Sleep(0.2);
    moveForward(motor_percent, driveDownRamp);
    Sleep(0.2);
    rotateLeft(motor_percent, turnToRightWall);
    Sleep(0.2);
    moveForward(motor_percent, moveToRightWall);
    Sleep(0.2);
    rotateRight(motor_percent, turnToButton);
    Sleep(0.2);
    moveUntilBump(motor_percent, 1);
    Sleep(0.3);
}
/*
Lever Arm Function
@param speed
    the speed that the motor will run at
    Forward is positive (arm moves down)
    Backwards is negative (arm moves up)
@param timeToRun
    The amount of time that the arm will move
*/
void moveArm(int speed, double timeToRun)
{
    arm_motor.SetPercent(speed);
    Sleep(timeToRun);
    arm_motor.Stop();
}

/*
Drive up Ramp Function
Gets into the proper position at the top of the ramp after the light turns on
*/
void startToRampTopR(int motor_percent)
{
    double first_movement = 3;
    double first_turn = 28; // 33
    double second_movement = 34;

    // WAIT FOR START LIGHT
    while (cdsCell.Value() > RED_VALUE)
    {
    }
    // GO
    //
    moveForward(motor_percent, first_movement);

    Sleep(0.15);

    // turn to ramp
    rotateLeft(motor_percent, first_turn);
    Sleep(0.15);

    // go up ramp
    moveForward(motor_percent, second_movement);
    Sleep(0.15);
}

void startToRampTopRWithRPS(int motor_percent)
{
    double first_movement = 3;
    double first_turn = 28; // 33
    double second_movement = 34;

    double initialHeading = RPS.Heading();
    // WAIT FOR START LIGHT
    while (cdsCell.Value() > RED_VALUE)
    {
    }
    // GO
    //
    moveForward(motor_percent, first_movement);

    Sleep(0.15);
    double desiredHeading = 180;
    // turn to ramp
    rotateLeft(motor_percent, desiredHeading - initialHeading);
    Sleep(0.15);

    // go up ramp
    moveForward(motor_percent, second_movement);
    Sleep(0.15);
}

/*
Drive function
@param percent
    the speed that the robot should move forward
@param inches
    the distance that the robot will move
*/
void moveForward(int percent, double inches) // using encoders
{
    int counts = (int)((inches / CIRCUM) * 318);
    int fullSpeedCounts = (counts * 0.90);
    // Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    // Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);

    // While the average of the left and right encoder is less than 90% of the distance,
    // keep running motors
    while ((left_encoder.Counts() + right_encoder.Counts()) / 2. < fullSpeedCounts * 2)
        ;

    // drop speed to 75% of max speed
    right_motor.SetPercent(percent * 0.8);
    left_motor.SetPercent(percent * 0.8);

    // While the average of the left and right encoder is less than the full distance,
    // keep running motors
    while ((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts * 2)
        ;

    // Turn off motors
    stopDriving();
}

/*Drive function
@param percent
    the speed that the robot should move forward
@param inches
    the distance that the robot will move
*/
void moveForward(int percent)
{
    left_motor.SetPercent(percent);
    right_motor.SetPercent(percent);
}

/*
Drive function
@param percent
    the speed that the robot should move backward
@param inches
    the distance that the robot will move
*/
void moveBackward(int percent, double inches) // using encoders
{
    int counts = (int)((inches / CIRCUM) * 318);
    int fullSpeedCounts = (counts * 0.85);
    // Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    // Set both motors to desired percent
    right_motor.SetPercent(-1 * percent);
    left_motor.SetPercent(-1 * percent);

    // While the average of the left and right encoder is less than 90% of the distance,
    // keep running motors
    while ((left_encoder.Counts() + right_encoder.Counts()) / 2. < fullSpeedCounts * 2)
        ;

    // drop speed to 75% of max speed
    right_motor.SetPercent(-1 * percent * 0.7);
    left_motor.SetPercent(-1 * percent * 0.7);

    // While the average of the left and right encoder is less than the full distance,
    // keep running motors
    while ((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts * 2)
    {
    }

    // Turn off motors
    stopDriving();
}

/*
Overloaded Drive backward function
Only backs up, no stopping
*/
void moveBackward(int percent)
{
    right_motor.SetPercent(-1 * percent);
    left_motor.SetPercent(-1 * percent);
}

/*
Stop function
*/
void stopDriving()
{
    right_motor.Stop();
    left_motor.Stop();
}

/*
Move until a bump switch is hit
@param percent
    magnitude is speed, sign is direction
@param bumpSwitchSide
    which bump switches are being used
    1 - Front extrusion
    2 - Back two bump switches
    3 - Back up but triggers when one is pressed
*/
void moveUntilBump(int percent, int bumpSwitchSide)
{
    if (percent < 0)
    {
        moveBackward(-percent);
    }
    else {
        moveForward(percent);
    }
    bool completed = false;
    float startTime = TimeNow();

    

    if (bumpSwitchSide == 1)
    {
        while (frontBump.Value()) {}
    }
    else if (bumpSwitchSide == 2) {
        while ((leftBump.Value() || rightBump.Value())){
            if (TimeNow() - startTime > 2 && !(leftBump.Value() || rightBump.Value())){
                stopDriving();
                //move away from wall
                if (percent < 0)
                {
                    moveBackward(percent);
                }
                else {
                    moveForward(-percent);
                }
                Sleep(0.2);
                startTime = TimeNow();

                float currentHeading = RPS.Heading();
                LCD.Write(currentHeading);
                int angleCorrection = 10;
                if (currentHeading < 270) {
                    rotateRight(percent, angleCorrection);
                }
                else {
                    rotateLeft(percent, angleCorrection);
                }
                Sleep(0.2);

                //go into wall again
                if (percent < 0)
                {
                    moveBackward(-percent);
                }
                else {
                    moveForward(percent);
                }
            }
            Sleep(0.2);
        }
    }
    else if (bumpSwitchSide == 3) {
        while ((leftBump.Value() && rightBump.Value())){}
    }
    Sleep(0.1);
    stopDriving();
}


/*
Turn Right function
@param percent
    the speed that the robot should turn
@param degrees
    the angle that the robot will move
*/
void rotateLeft(int percent, double degrees)
{
    int counts = (ROBOT_WIDTH / 2 * degrees * (PI / 180) * 318) / CIRCUM;
    double speedMultiplier = 0.7;
    int fullSpeedCounts = (counts * 0.80);

    // Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    // Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(-1 * percent);

    // While the average of the left and right encoder is less than counts,
    // keep running motors
    while ((right_encoder.Counts() < fullSpeedCounts * 2))
        ;
    right_motor.SetPercent(percent * speedMultiplier);
    left_motor.SetPercent(percent * speedMultiplier * -1);
    while ((right_encoder.Counts() < counts * 2))
        ;

    // Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

/*
Turn Left function
@param percent
    the speed that the robot should turn
@param degrees
    the angle that the robot will move
*/
void rotateRight(int percent, double degrees)
{
    int counts = (ROBOT_WIDTH / 2 * degrees * (PI / 180) * 318) / CIRCUM;
    double speedMultiplier = 0.7;
    int fullSpeedCounts = (counts * 0.80);

    // Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    // Set both motors to desired percent
    right_motor.SetPercent(-1 * percent);
    left_motor.SetPercent(percent);

    // While the average of the left and right encoder is less than counts,
    // keep running motors
    while ((right_encoder.Counts() < fullSpeedCounts * 2))
        ;
    right_motor.SetPercent(percent * speedMultiplier * -1);
    left_motor.SetPercent(percent * speedMultiplier);
    while ((right_encoder.Counts() < counts * 2))
        ;

    // Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}
