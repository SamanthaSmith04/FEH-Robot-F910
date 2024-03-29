/* ==================================================================================
*                       FEH Robot Project Group 910F
*
*Group Members: Searita Chen, Bennett Godinho-Nelson, Samantha Smith, John Ulm
*
*                             Robot Code
==================================================================================*/

/* =========================================================
*                                PORTS
*   MOTORS
*       LEFT DRIVE         - MOTOR 3
*       RIGHT DRIVE        - MOTOR 1
*       ARM MOTOR          - MOTOR 2
*   SERVOS
*       DROPPER SERVO      - SERVO 0
*   ENCODERS
*       LEFT ENCODER       - P0_0
*       RIGHT ENCODER      - P0_1
*   OTHER COMPONENTS
*       CDS CELL           - P1_0
*       OPTO SENSOR R      - P1_1
*       OPTO SENSOR M      - P1_2
*       OPTO SENSOR L      - P1_3
*       BUMP SWITCH FRONT  - P3_0
*       BUMP SWITCH BACK L - P1_6
*       BUMP SWITCH BACK R - P1_7
===================================================*/

/*================================================== IMPORTS ==================================================*/

#include <FEHLCD.h>
#include <FEHMotor.h>
#include <FEHServo.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <ctime>
#include <cmath>
#include <FEHRPS.h>

/*================================================== CONSTANTS ==================================================*/

//PORTS
#define RIGHT_MOTOR_PORT FEHMotor::Motor1
#define LEFT_MOTOR_PORT FEHMotor::Motor3
#define ARM_MOTOR_PORT FEHMotor::Motor2
#define DROPPER_SERVO_PORT FEHServo::Servo0
#define CDS_SENSOR_PORT FEHIO::P1_0 // CDS for COLOR
#define OPT_R_PORT FEHIO::P1_1
#define OPT_M_PORT FEHIO::P1_2
#define OPT_L_PORT FEHIO::P1_3
#define BUMP_SWITCH_PORT FEHIO::P0_1
#define RIGHT_ENCODER_PORT FEHIO::P0_1
#define LEFT_ENCODER_PORT FEHIO::P0_0
#define FRONT_BUMP_PORT FEHIO::P3_0
#define RIGHT_BUMP_PORT FEHIO::P1_6
#define LEFT_BUMP_PORT FEHIO::P1_7

//VALUES FOR CALCULATIONS
#define CIRCUM (2 * 3.1415 * 3)
#define ROBOT_WIDTH 7.625
#define PI 3.14159

//SERVO VALUES
#define SERVO_MIN 1000
#define SERVO_MAX 2075
#define SERVO_CLOSED 180
#define SERVO_OPEN 45

//CDS CELL VALUES
#define RED_VALUE 0.25 //old 0.18

#define BLUE_VALUE 0.80 // Arbitrary Blue Value

//MOVEMENT PARAMETERS
#define PERCENT_SPEED 45
#define SLOW_SPEED 35
#define PAUSETIME 0.2

/*================================================== VARIABLES ==================================================*/

//ENCODERS
DigitalEncoder right_encoder(RIGHT_ENCODER_PORT);
DigitalEncoder left_encoder(LEFT_ENCODER_PORT);

//MOTORS
FEHMotor right_motor(RIGHT_MOTOR_PORT, 9.0);
FEHMotor left_motor(LEFT_MOTOR_PORT, 9.0);
FEHMotor arm_motor(ARM_MOTOR_PORT, 7.2);

//SENSING
AnalogInputPin cdsCell(CDS_SENSOR_PORT);
DigitalInputPin frontBump(FRONT_BUMP_PORT);
DigitalInputPin leftBump(LEFT_BUMP_PORT);
DigitalInputPin rightBump(RIGHT_BUMP_PORT);

//SERVOS
FEHServo dropperServo(DROPPER_SERVO_PORT); // 1000 open, 2075 closed

// GLOBAL VARIABLES
int x, y;
int side = 0;
double maxLight =3.3;

/*================================================== FUNCTION HEADERS ==================================================*/

//TESTING FUNCTIONS
void testCDS();
void driveTest();
void rpsPoints();

//CHECKPOINT CODE
void checkPoint1Code();
void checkPoint2Code();
void checkPoint3Code();
void checkPoint4Code();
void checkPoint5Code();

//MOVEMENT
    //Linear
        void moveForward(int, double);
        void moveBackward(int, double);
        void moveBackward(int);
        void moveUntilBump(int, int, int);
        void moveUpdateMaxLight(int, double);
    //Rotation
        void rotateLeft(int, double);
        void rotateLeftRPS(int, double, int);
        void rotateRight(int, double);
    //Lever Arm
        void moveArm(int, double);
    //Stopping
        void stopDriving();

//TASK FUNCTIONS
void startToRampTopR(int);
void startToRampTopRWithRPS(int);
void passportStamp(int);
void luggageDrop(int);
void checkBoardingPass(int);
void pressBoardingPass(int);
void goDownLRamp(int);
void fuelLevers(int);
void finalButton(int);


/*================================================== MAIN FUNCTION ==================================================*/
int main()
{
    // initialize all values
    dropperServo.SetMin(SERVO_MIN);
    dropperServo.SetMax(SERVO_MAX);

    dropperServo.SetDegree(SERVO_CLOSED);

    RPS.InitializeTouchMenu();

    Sleep(0.3);
    //testCDS();
    passportStamp(PERCENT_SPEED); //Starts at charger, ends at passport stamp facing 180
    luggageDrop(PERCENT_SPEED); //Starts at passport facing 180, ends at high luggage facing 180
    checkBoardingPass(PERCENT_SPEED); //starts at high luggage facing 180, ends at BP Light facing 180
    pressBoardingPass(PERCENT_SPEED); //starts at BP light facing 180, ends at left wall facing 90
    goDownLRamp(SLOW_SPEED); //starts at left wall facing 90, ends at bottom of L ramp facing 0
    fuelLevers(SLOW_SPEED); //starts at bottom of L ramp facing 0, ends at middle of bottom course section facing 90
    finalButton(PERCENT_SPEED); //starts at bottom course section facing 90, ends pressing final button
    
}

/*================================================== TASK FUNCTIONS ==================================================*/
/*
=========== TASK ORDER ===========
    1. Move to top ramp
    2. Passport Stamp
    3. Luggage Drop
    4. Check Boarding Pass Color
    5. Press Boarding Pass Button
    6. Go down left ramp
    7. Fuel Lever task
    8. Press final button
*/

void passportStamp(int motor_percent) {
    int arm_percent = 45;
    double arm_time = 0.8;

    int moveForward1 = 5;
    int turnToLever = 90;
    int moveToLever = 4;

    startToRampTopRWithRPS(SLOW_SPEED); // USING RPS VALUES
    Sleep(PAUSETIME);
    rotateLeftRPS(motor_percent, 90, 1);
    Sleep(PAUSETIME);
    moveUntilBump(-SLOW_SPEED, 2, 270);
    Sleep(PAUSETIME);

    moveForward(motor_percent, moveForward1);
    Sleep(PAUSETIME);
    moveArm(arm_percent, arm_time); // arm is now down
    rotateRight(motor_percent, turnToLever);
    Sleep(PAUSETIME);

    moveForward(motor_percent, moveToLever);
    Sleep(PAUSETIME);

    moveArm(-arm_percent * 2, 0.3); //arm is now back up
}

void luggageDrop(int motor_percent) {
    int backUpFromPassport = 5;

    int moveForwardToDropper = 8;
    int turnTowardsDropper = 90;

    int backupFromDropper = 5;
    int turnToLWall = 90;
    int moveAwayFromWall = 1;
    int turnToRamp = 90;
    int driveDownRamp = 25;
    int turnToRightWall = 90;
    int moveToRightWall = 10;
    int turnToButton = 45; // WONT BE 90

    moveBackward(motor_percent, backUpFromPassport);
    Sleep(PAUSETIME);
    rotateLeft(motor_percent, 90);
    Sleep(PAUSETIME);
    moveForward(motor_percent, moveForwardToDropper);
    Sleep(PAUSETIME);
    rotateRight(motor_percent, 90);
    Sleep(PAUSETIME);
    moveUntilBump(-SLOW_SPEED, 3, 0);
    Sleep(PAUSETIME);
    moveBackward(motor_percent);
    Sleep(PAUSETIME);
    stopDriving();
    dropperServo.SetDegree(SERVO_OPEN);
    Sleep(0.4);
}

void checkBoardingPass(int motor_percent) {
    int dropperToAlmostLight = 14;
    int wallToLight = 4;
    moveForward(motor_percent, 5);
        Sleep(PAUSETIME);

    rotateRight(motor_percent, 90);
        Sleep(PAUSETIME);

    moveUntilBump(-SLOW_SPEED, 2, 90);
        Sleep(PAUSETIME);


    moveForward(motor_percent, wallToLight);
        Sleep(PAUSETIME);

    rotateLeft(motor_percent, 90);
        Sleep(PAUSETIME);

    moveUpdateMaxLight(25, dropperToAlmostLight);


    Sleep(0.5);
    double CDSValue = cdsCell.Value();
    side = 1;
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
        LCD.SetBackgroundColor(YELLOW);
        side = 0; // default to blue
    }
    LCD.Clear();
    LCD.Write(CDSValue);
    Sleep(PAUSETIME);
    rotateRight(motor_percent, RPS.Heading() - 180);
    Sleep(PAUSETIME);
}

void pressBoardingPass(int motor_percent) {
    int backUp5 = 10;          // back up from light 
    int turn5 = 90;            // turn to right wall 
    int moveForwardBlue = 6;   // move distance needed for blue button 
    int turnBlue = 90;         // turn to blue button 
    int moveForwardRed = 9;    // move distance needed for red button 
    int turnRed = 90;          // turn towards red button 
    int backupDist = 15; 
    
    moveBackward(motor_percent, backUp5);
    Sleep(PAUSETIME);

    rotateRight(motor_percent, turn5);
    Sleep(PAUSETIME);

    moveUntilBump(-SLOW_SPEED, 2, 90);
    Sleep(PAUSETIME);

    moveForward(motor_percent, 4);
    Sleep(PAUSETIME);

    // robot now decides which button to go to

    if (side == 0)
    {
        moveForward(motor_percent, moveForwardBlue);
        Sleep(PAUSETIME);
        rotateLeft(motor_percent, turnBlue);
        Sleep(PAUSETIME);
    }
    else if (side == 1)
    {
        moveForward(motor_percent, moveForwardRed);
        Sleep(PAUSETIME);
        rotateLeft(motor_percent, turnRed);
        Sleep(PAUSETIME);
    }
    moveBackward(-motor_percent); //hitting button
    Sleep(2.0);
    moveBackward(motor_percent, backupDist);
    Sleep(PAUSETIME);
    rotateRight(motor_percent, 90);
    Sleep(PAUSETIME);
    moveBackward(motor_percent, 3);
    moveUntilBump(-SLOW_SPEED, 2, 90);
    Sleep(PAUSETIME);
}

void goDownLRamp(int motor_percent) {
    int moveAwayFromWall = 1;
    int turnToRamp = 90;
    int driveDownRamp = 27;

    moveForward(motor_percent, moveAwayFromWall);
    Sleep(PAUSETIME);
    rotateLeft(motor_percent, turnToRamp);
    Sleep(PAUSETIME);
    moveBackward(motor_percent, driveDownRamp);
    Sleep(PAUSETIME); //now at bottom of ramp in front of levers
}

void fuelLevers(int motor_percent) {
    rotateLeft(motor_percent, 180);
    Sleep(PAUSETIME);
    int fuelLeverNum = RPS.GetCorrectLever();
    int arm_percent = 45;
    double arm_time = 0.6;
    int moveToLever = 2;     
    int lever1 = 5;
    int lever2 = 9; 
    rotateLeft(motor_percent, 90);
    moveUntilBump(-SLOW_SPEED, 2, 90);
    moveForward(motor_percent, 2);


    if (fuelLeverNum == 2) {

        rotateRight(motor_percent, 90);
    }
    else if (fuelLeverNum == 1) {
        moveForward(motor_percent, lever1);
        rotateRight(motor_percent, 90);
    }
    else if (fuelLeverNum == 0) {
        moveForward(motor_percent, lever2);
        rotateRight(motor_percent, 90);
    }
    
    moveForward(motor_percent, moveToLever);
    Sleep(PAUSETIME);

    moveArm(arm_percent, arm_time);  // arm is now down
    moveArm(-arm_percent, arm_time); // arm is now up
    moveBackward(motor_percent, moveToLever);

    Sleep(5.0);
    moveArm(arm_percent, arm_time);
    moveForward(motor_percent, moveToLever);

    Sleep(PAUSETIME);
    moveArm(-arm_percent, arm_time); // arm is now up

    moveBackward(motor_percent, moveToLever);
}

void finalButton(int motor_percent) {
    Sleep(PAUSETIME);
    int fuelLeverNum = RPS.GetCorrectLever();
    rotateRight(motor_percent, 90);
    Sleep(PAUSETIME);
    if (fuelLeverNum == 2) {
        moveBackward(motor_percent, 12);
        rotateLeft(motor_percent, 2);
    }
    else if (fuelLeverNum == 1) {
        moveBackward(motor_percent, 8);
    }
    else if (fuelLeverNum == 0) {
        moveBackward(motor_percent, 4);
    }
    Sleep(PAUSETIME);

    moveBackward(motor_percent, 2); //this may need to change
    Sleep(PAUSETIME);   

    rotateRight(motor_percent, 45);
    Sleep(PAUSETIME);
    moveBackward(70);

}

/*================================================== TESTING FUNCTIONS ==================================================*/
/*
Driving test code.
Moves forward 6 in, then back 6in. then turns 90 degrees both directions
Ideally should return the robot to the inital position
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
        Sleep(PAUSETIME);
    }
}

void rpsPoints(){
        // Write initial screen info
    LCD.WriteRC("X Position:", 11, 0);
    LCD.WriteRC("Y Position:", 12, 0);
    LCD.WriteRC("   Heading:", 13, 0);

    LCD.WriteRC(RPS.X(), 11, 12);       // update the x coordinate
    LCD.WriteRC(RPS.Y(), 12, 12);       // update the y coordinate
    LCD.WriteRC(RPS.Heading(), 13, 12); // update the heading

    Sleep(100); // wait for 100ms to avoid updating the screen too quickly
}

/*================================================== CHECKPOINTS ==================================================*/
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
    Sleep(PAUSETIME);

    moveBackward(motor_percent, backUp5);
    Sleep(PAUSETIME);

    rotateRight(motor_percent, turn5);
    Sleep(PAUSETIME);

    moveBackward(motor_percent + 15);
    Sleep(1.0);
    stopDriving();
    Sleep(PAUSETIME);

    moveForward(motor_percent, 4);

    // robot now decides which button to go to

    if (side == 0)
    {
        moveForward(motor_percent, moveForwardBlue);
        Sleep(PAUSETIME);
        rotateLeft(motor_percent, turnBlue);
        Sleep(PAUSETIME);
        moveForward(motor_percent, moveForwardBlue2);
        Sleep(PAUSETIME);
    }
    else if (side == 1)
    {
        moveForward(motor_percent, moveForwardRed);
        Sleep(PAUSETIME);
        rotateLeft(motor_percent, turnRed);
        Sleep(PAUSETIME);
        moveForward(motor_percent, moveForwardRed2);
        Sleep(PAUSETIME);
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
    Sleep(PAUSETIME);

    rotateLeft(motor_percent, turn1);
    Sleep(0.1);

    moveBackward(motor_percent + 10); // get lined up against wall
    Sleep(0.5);
    stopDriving();

    Sleep(PAUSETIME);
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
    Sleep(PAUSETIME);

    rotateLeft(motor_percent, turnToLevers);
    Sleep(PAUSETIME);

    moveForward(motor_percent, moveToLever);
    Sleep(PAUSETIME);

    moveArm(arm_percent, arm_time);  // arm is now down
    moveArm(-arm_percent, arm_time); // arm is now up
    moveBackward(motor_percent, moveToLever);

    Sleep(5.0);
    moveArm(arm_percent, arm_time);
    moveForward(motor_percent, moveToLever);

    Sleep(PAUSETIME);
    moveArm(-arm_percent, arm_time); // arm is now up

    moveBackward(motor_percent, moveToLever);
}

/*
The main drive code for checkpoint 4
Drives up the ramp to the passport stamp and flips the lever
*/
void checkPoint4Code()
{
    int motor_percent = 35;
    int arm_percent = 45;
    double arm_time = 0.4;

    int moveForward1 = 5;
    int turnToLever = 90;
    int moveToLever = 4;

    startToRampTopRWithRPS(motor_percent); // USING RPS VALUES
    Sleep(PAUSETIME);
    rotateLeftRPS(motor_percent, 90, 1);
    Sleep(0.5);
    moveUntilBump(-SLOW_SPEED, 2, 270);
    Sleep(PAUSETIME);

    moveForward(motor_percent, moveForward1);
    Sleep(PAUSETIME);
    moveArm(arm_percent, arm_time); // arm is now down
    rotateRight(motor_percent, turnToLever);
    Sleep(PAUSETIME);

    moveForward(motor_percent, moveToLever);
    Sleep(PAUSETIME);

    moveArm(-arm_percent * 2, arm_time / 2);
}

/*
The main drive code for checkpoint 5
STARTS FROM END POSITION OF CHECKPOINT 4
Leaves passport stamp task and drops off luggage in high luggage drop. Goes down left ramp and hits end button
*/
void checkPoint5Code()
{
    int motor_percent = 35;

    int backUpFromPassport = 5;

    int moveForwardToDropper = 9;
    int turnTowardsDropper = 90;

    int backupFromDropper = 5;
    int turnToLWall = 90;
    int moveAwayFromWall = 1;
    int turnToRamp = 90;
    int driveDownRamp = 25;
    int turnToRightWall = 90;
    int moveToRightWall = 10;
    int turnToButton = 45; // WONT BE 90

    moveBackward(motor_percent, backUpFromPassport);
    Sleep(PAUSETIME);
    rotateLeft(motor_percent, 90);
    Sleep(PAUSETIME);
    moveForward(motor_percent, moveForwardToDropper);
    Sleep(PAUSETIME);
    rotateRight(motor_percent, 90);
    Sleep(PAUSETIME);
    moveUntilBump(-SLOW_SPEED, 3, 0);
    Sleep(PAUSETIME);
    dropperServo.SetDegree(SERVO_OPEN);
    Sleep(0.4);
    // LUGGAGE TASK NOW COMPLETE
    moveForward(motor_percent, backupFromDropper);
    Sleep(PAUSETIME);
    rotateRight(motor_percent, turnToLWall);
    Sleep(PAUSETIME);
    moveBackward(motor_percent, 1);
    Sleep(PAUSETIME);
    moveUntilBump(-SLOW_SPEED, 2, 90); // now touching l wall
    Sleep(PAUSETIME);
    moveForward(motor_percent, moveAwayFromWall);
    Sleep(PAUSETIME);
    rotateLeft(motor_percent, turnToRamp);
    Sleep(PAUSETIME);
    moveBackward(motor_percent, driveDownRamp);
    Sleep(PAUSETIME); //now at bottom of ramp in front of levers
    rotateLeft(motor_percent, turnToRightWall); //WILL NEED TO ADD ANOTHER TURN FOR FUEL LEVERS
    Sleep(PAUSETIME);
    moveBackward(motor_percent, moveToRightWall);
    Sleep(PAUSETIME);
    rotateRight(motor_percent, turnToButton);
    Sleep(PAUSETIME);
    moveUntilBump(-SLOW_SPEED, 1, 0);
    Sleep(0.3);
}


/*================================================== MOVEMENT ==================================================*/
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

/*
Drive up Ramp Function using RPS
Gets into the proper position at the top of the ramp after the light turns on using RPS to get angle
*/
void startToRampTopRWithRPS(int motor_percent)
{
    double first_movement = 3;
    double second_movement = 34;
    // WAIT FOR START LIGHT
    while (cdsCell.Value() > RED_VALUE)
    {
        //rpsPoints();
    }
    double initialHeading = RPS.Heading();
    LCD.Write(initialHeading);
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
Drive forward using encoders
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

/*
Drive forward (starts driving, doesnt stop)
@param percent
    the speed that the robot should move forward
*/
void moveForward(int percent)
{
    left_motor.SetPercent(percent);
    right_motor.SetPercent(percent);
}

/*
Drive backwards using encoders
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
Drive Backward (starts driving, doesnt stop)
@param percent
    the speed that the robot should move backward
*/
void moveBackward(int percent)
{
    right_motor.SetPercent(-1 * percent);
    left_motor.SetPercent(-1 * percent);
}

/*
Stop function
Stops both drive motors
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
@param correctionHeading
    the desired heading after the robot aligns to the wall
*/
void moveUntilBump(int percent, int bumpSwitchSide, int correctionHeading)
{
    if (percent < 0)
    {
        moveBackward(-percent);
    }
    else
    {
        moveForward(percent);
    }
    bool completed = false;
    float startTime = TimeNow();

    if (bumpSwitchSide == 1)
    {
        while (frontBump.Value())
        {
        }
    }
    else if (bumpSwitchSide == 2)
    {
        while ((leftBump.Value() || rightBump.Value()))
        {
            if (TimeNow() - startTime > 1) {
                stopDriving();
                // move away from wall
                if (percent < 0)
                {
                    moveBackward(percent);
                }
                else
                {
                    moveForward(-percent);
                }
                Sleep(0.15);
                startTime = TimeNow();

                float currentHeading = RPS.Heading();
                LCD.Write(currentHeading);
                if (correctionHeading < 180)
                {
                    if (currentHeading < correctionHeading)
                    {
                        right_motor.SetPercent(-1 * percent);
                        left_motor.SetPercent(percent);
                    }
                    else
                    {
                        right_motor.SetPercent(percent);
                        left_motor.SetPercent(-percent);
                    }
                }
                else
                {
                    if (currentHeading < correctionHeading)
                    {
                        right_motor.SetPercent(-percent);
                        left_motor.SetPercent(percent);      
                    }
                    else
                    {
                        right_motor.SetPercent(percent);
                        left_motor.SetPercent(-percent);           
                    }
                }
                Sleep(0.08);

                // go into wall again
                if (percent < 0)
                {
                    moveBackward(-percent);
                }
                else
                {
                    moveForward(percent);
                }
            }
            Sleep(0.1);
        }
    }
    else if (bumpSwitchSide == 3)
    {

        while ((leftBump.Value() && rightBump.Value()))
        {
            if (TimeNow() - startTime > 4) {
                break;
            }
        }
    }
    Sleep(0.1);
    stopDriving();
}

/*
Turn Left function
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
Drive forward using encoders and updates maxLight
Used to get the lowest light value to determine boarding pass color
@param percent
    the speed that the robot should move forward
@param inches
    the distance that the robot will move
*/
void moveUpdateMaxLight(int percent, double inches){
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
    while ((left_encoder.Counts() + right_encoder.Counts()) / 2. < fullSpeedCounts * 2){
        if(cdsCell.Value()>maxLight){
            maxLight=cdsCell.Value();
        }
    }

    // drop speed to 75% of max speed
    right_motor.SetPercent(percent * 0.8);
    left_motor.SetPercent(percent * 0.8);

    // While the average of the left and right encoder is less than the full distance,
    // keep running motors
    while ((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts * 2){
        if(cdsCell.Value()>maxLight){
            maxLight=cdsCell.Value();
        }
    }

    // Turn off motors
    stopDriving();

}

/*
Turn Left RPS function
@param percent
    the speed that the robot should turn
@param degrees
    the angle that the robot will move
@param directionCorrection
    positive for forward, negative for backwards
*/
void rotateLeftRPS(int percent, double degrees, int directionCorrection)
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
    float timer = TimeNow();
    while ((right_encoder.Counts() < fullSpeedCounts * 2)) {
        if (TimeNow() - timer > 2){
            moveForward(percent * directionCorrection, 1);
            fullSpeedCounts += (int)((1 / CIRCUM) * 318);
            Sleep(0.1);
            right_motor.SetPercent(percent);
            left_motor.SetPercent(-1 * percent);
            timer = TimeNow();
        }
    }
    right_motor.SetPercent(percent * speedMultiplier);
    left_motor.SetPercent(percent * speedMultiplier * -1);
    while ((right_encoder.Counts() < counts * 2))
        ;

    // Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

/*
Turn Right function
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
    float timer = TimeNow();
    while ((right_encoder.Counts() < fullSpeedCounts * 2)) {
    }
    right_motor.SetPercent(percent * speedMultiplier * -1);
    left_motor.SetPercent(percent * speedMultiplier);
    while ((right_encoder.Counts() < counts * 2));

    // Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}