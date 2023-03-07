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
*       LEFT DRIVE       - MOTOR 0
*       RIGHT DRIVE      - MOTOR 1
*       ARM MOTOR        -  
*   SERVOS
*       DROPPER SERVO    - 
*   ENCODERS
*       LEFT ENCODER     - P0_0
*       RIGHT ENCODER    - P0_1
*   OTHER COMPONENTS
*       CDS CELL (COLOR) - P1_0
*       CDS CELL RIGHT   - P1_1
*       CDS CELL MIDDLE  - P1_2
*       CDS CELL LEFT    - P1_3
*       BUMP SWITCH      - 
===========================================================================*/

//IMPORTS
#include <FEHLCD.h>
#include <FEHMotor.h>
#include <FEHServo.h>
#include <FEHIO.h>
#include <FEHUtility.h>

//CONSTANTS
#define RIGHT_MOTOR_PORT FEHMotor::Motor1 
#define LEFT_MOTOR_PORT FEHMotor::Motor0
#define ARM_MOTOR_PORT FEHMotor::Motor2
#define DROPPER_SERVO_PORT FEHServo::Servo0
#define CDS_SENSOR_PORT FEHIO::P1_0 //CDS for COLOR
#define CDS_R_PORT FEHIO::P1_1
#define CDS_M_PORT FEHIO::P1_2
#define CDS_L_PORT FEHIO::P1_3
#define BUMP_SWITCH_PORT FEHIO::P0_1
#define CIRCUM (2*3.1415*3)
#define RIGHT_ENCODER_PORT FEHIO::P0_1
#define LEFT_ENCODER_PORT FEHIO::P0_0
#define RED_VALUE 0.18
#define BLUE_VALUE 1 //Arbitrary Blue Value
#define ROBOT_WIDTH 7.625
#define PI 3.14159

//COMPONENTS
DigitalEncoder right_encoder(RIGHT_ENCODER_PORT);
DigitalEncoder left_encoder(LEFT_ENCODER_PORT);
FEHMotor right_motor(RIGHT_MOTOR_PORT, 9.0);
FEHMotor left_motor(LEFT_MOTOR_PORT, 9.0);
AnalogInputPin cdsCell(CDS_SENSOR_PORT);

//GLOBAL VARIABLES
int x, y;

//FUNCTION HEADERS
void testCDS();
void checkPoint1Code();
void checkPoint2Code();
void moveForward(int, double);
void moveBackward(int, double);
void moveBackward(int);
void stopDriving();
void rotateLeft(int, double);
void driveTest();
void rotateRight(int, double);
void startToRampTopR(int);


int main() {
    //testCDS();
    checkPoint2Code();
    //driveTest();
}

/*
DRIVING TEST CODE
*/
void driveTest(){
    int percent = 25;
    LCD.SetBackgroundColor(BLACK);
    //forward
    LCD.Write("Going forward 6in");
    moveForward(percent, 6);
    LCD.Clear();
    LCD.Write("Stopping");
    Sleep(0.5);

    //backward
    LCD.Write("Going backward 6in");
    moveBackward(percent, 6);
    LCD.Clear();
    LCD.Write("Stopping");
    Sleep(0.5);

    //turn right
    LCD.Write("Turning right 90 degrees");
    rotateRight(percent, 90);
    LCD.Clear();
    LCD.Write("Stopping");
    Sleep(0.5);

    //turn left
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
void testCDS(){
    while (1 == 1){
       LCD.Clear();
       LCD.Write(cdsCell.Value());
       Sleep(0.1);
   }
}

/*
The main drive code for checkpoint 1
Drives up the ramp, touches the kiosk, and drives back down the ramp*/
void checkPoint1Code(){
    //ANY PRE-DRIVE SETUP STUFF
    int motor_percent = 40;
    double first_movement = 4;
    double first_turn = 30;
    double second_movement = 34;
    double third_movement = 10;
    double fourth_movement = 20;

    //backing up
    double backup_turn = 30;
    double backup1 = 25;
    double backup_turn_2 = 60;
    double backup2 = 20;

    
    //WAIT FOR START LIGHT
    while (cdsCell.Value() > RED_VALUE){}
    //GO
    //
    moveForward(motor_percent,first_movement);

    //turn to ramp
    rotateLeft(motor_percent, first_turn);
    Sleep(0.3);

    //go up ramp
    moveForward(motor_percent, second_movement);
    Sleep(0.3);

    //turn 90
    rotateLeft(motor_percent, 90);
    Sleep(0.3);

    //go forward
    moveForward(motor_percent, third_movement);
    Sleep(0.3);

    //turn 90
    rotateRight(motor_percent, 90);
    Sleep(0.3);

    //move straight
    moveForward(motor_percent, fourth_movement);
    Sleep(0.3);

    //BACK
    rotateRight(motor_percent, backup_turn);
    Sleep(0.3);

    moveBackward(motor_percent, backup1);
    Sleep(0.3);

    rotateLeft(motor_percent, backup_turn_2);
    Sleep(0.3);

    moveBackward(motor_percent, backup2);

}


/*
The main drive code for checkpoint 1
Drives up the ramp, touches the kiosk, and drives back down the ramp*/ 
void checkPoint2Code(){
    int CDSValue = 3.3;
    int motor_percent = 40;
    /*unused distances
    int moveForward1 = 0; // move towards wall - A
    int turn1 = 0; //turn towards ramp - TA
    int moveForward2 = 0; // move up ramp - B*/

    int turn2 = 90; //turn towards left side wall - TB
    int moveForward3 = 20; // move towards left side wall - C
    int turn3 = 90; // turn towards light sensor - TC
    int moveForward4 = 14; //move towards light - D //may be eliminated
    int backUp5 = 0; // back up from light - E
    int turn5 = 0; // turn to right wall - TE
    int moveForwardBlue = 0; //move distance needed for blue button - F
    int turnBlue = 0; //turn to blue button - TF
    int moveForwardBlue2 = 0; // move to blue button - H
    int moveForwardRed = 0; //move distance needed for red button - G
    int turnRed = 0; // turn towards red button - TG
    int moveForwardRed2 = 0; // move to red button - I

    //move from the start to the top of the right ramp, end facing towards top of field
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

    moveForward(motor_percent, moveForward4); //LIGHT IS CHECKED HERE
    Sleep(1.0);

    //display color to screen
    if (CDSValue < RED_VALUE){
        LCD.SetBackgroundColor(RED);
    }
    else if (CDSValue < BLUE_VALUE){
        LCD.SetBackgroundColor(BLUE);
    }
    else {
        LCD.SetBackgroundColor(YELLOW);
    }
    LCD.Clear();

    moveBackward(motor_percent, backUp5);

    rotateRight(motor_percent, turn5); //robot now decides which button to go to

    if (CDSValue < RED_VALUE){
        moveForward(motor_percent, moveForwardBlue);
        rotateLeft(motor_percent, turnBlue);
        moveForward(motor_percent, moveForwardBlue2);
    }
    else if (CDSValue < BLUE_VALUE){
        moveForward(motor_percent, moveForwardRed);
        rotateLeft(motor_percent, turnRed);
        moveForward(motor_percent, moveForwardRed2);
    }
    

    
}

void startToRampTopR(int motor_percent){
    double first_movement = 3;
    double first_turn = 33;
    double second_movement = 34;

    //WAIT FOR START LIGHT
    while (cdsCell.Value() > RED_VALUE){}
    //GO
    //
    moveForward(motor_percent,first_movement);

    //turn to ramp
    rotateLeft(motor_percent, first_turn);
    Sleep(0.15);

    //go up ramp
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
void moveForward(int percent, double inches) //using encoders
{
    int counts = (int) ((inches/CIRCUM)*318);
    int fullSpeedCounts = (counts * 0.90);
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);

    //While the average of the left and right encoder is less than 90% of the distance,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < fullSpeedCounts * 2);

    //drop speed to 75% of max speed
    right_motor.SetPercent(percent * 0.8);
    left_motor.SetPercent(percent * 0.8);

    //While the average of the left and right encoder is less than the full distance,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts * 2);


    //Turn off motors
    stopDriving();
}

/*
Drive function
@param percent
    the speed that the robot should move backward
@param inches
    the distance that the robot will move
*/
void moveBackward(int percent, double inches) //using encoders
{
    int counts = (int) ((inches/CIRCUM)*318);
    int fullSpeedCounts = (counts * 0.85);
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(-1*percent);
    left_motor.SetPercent(-1*percent);

    //While the average of the left and right encoder is less than 90% of the distance,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < fullSpeedCounts * 2);

    //drop speed to 75% of max speed
    right_motor.SetPercent(-1*percent * 0.7);
    left_motor.SetPercent(-1*percent * 0.7);

    //While the average of the left and right encoder is less than the full distance,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts * 2){}


    //Turn off motors
    stopDriving();
}

/*
Overloaded Drive backward function
Only backs up, no stopping
*/
void moveBackward(int percent){
    right_motor.SetPercent(-1*percent);
    left_motor.SetPercent(-1*percent);
    
}

/*
Stop function
*/
void stopDriving(){
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
void rotateLeft(int percent, double degrees){
    int counts = (ROBOT_WIDTH/2 * degrees * (PI/180) * 318)/ CIRCUM;
    double speedMultiplier = 0.7;
    int fullSpeedCounts = (counts * 0.80);


    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();


    //Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(-1*percent);


    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((right_encoder.Counts()<fullSpeedCounts * 2));
    right_motor.SetPercent(percent * speedMultiplier);
    left_motor.SetPercent(percent * speedMultiplier * -1);
    while((right_encoder.Counts()<counts * 2));


    //Turn off motors
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
void rotateRight(int percent, double degrees){
    int counts = (ROBOT_WIDTH/2 * degrees * (PI/180) * 318)/ CIRCUM;
    double speedMultiplier = 0.7;
    int fullSpeedCounts = (counts * 0.80);


    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();


    //Set both motors to desired percent
    right_motor.SetPercent(-1*percent);
    left_motor.SetPercent(percent);


    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((right_encoder.Counts()<fullSpeedCounts * 2));
    right_motor.SetPercent(percent * speedMultiplier * -1);
    left_motor.SetPercent(percent * speedMultiplier);
    while((right_encoder.Counts()<counts * 2));


    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

