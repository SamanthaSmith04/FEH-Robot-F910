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
*       LEFT DRIVE    - MOTOR 0
*       RIGHT DRIVE   - MOTOR 1
*       ARM MOTOR     - 
*   SERVOS
*       DROPPER SERVO - 
*   ENCODERS
*       LEFT ENCODER  - P0_0
*       RIGHT ENCODER - P0_1
*   OTHER COMPONENTS
*       CDS CELL      - P1_0
*       BUMP SWITCH   - 
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
#define CDS_SENSOR_PORT FEHIO::P1_0
#define BUMP_SWITCH_PORT FEHIO::P0_1
#define CIRCUM (2*3.1415*3)
#define RIGHT_ENCODER_PORT FEHIO::P0_1 //
#define LEFT_ENCODER_PORT FEHIO::P0_0 //
#define RED_VALUE 0.18
#define BLUE_VALUE 1 //Arbitrary Blue Value
#define ROBOT_WIDTH 7.75
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
void moveForward(int, double);
void moveBackward(int, double);
void stopDriving();
void rotateLeft(int, double);
void driveTest();
void rotateRight(int, double);


int main() {
    //testCDS();
    checkPoint1Code();
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

