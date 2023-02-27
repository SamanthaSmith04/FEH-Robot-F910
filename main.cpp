/* =========================================================================
*                       FEH Robot Project Group 910F
*
*Group Members: Searita Chen, Bennett Godinho-Nelson, Samantha Smith, John Ulm
*
*                             Robot Code
===========================================================================*/

//IMPORTS
#include <FEHLCD.h>
#include <FEHMotor.h>
#include <FEHServo.h>
#include <FEHIO.h>
#include <FEHUtility.h>

//CONSTANTS
#define LEFT_MOTOR_PORT FEHMotor::Motor0
#define LEFT_MOTOR_PORT FEHMotor::Motor1
#define ARM_MOTOR_PORT FEHMotor::Motor2
#define DROPPER_SERVO_PORT FEHServo::Servo0
#define CDS_SENSOR_PORT FEHIO::P0_0
#define CDS_START_THRESHOLD 0
#define BUMP_SWITCH_PORT FEHIO::P0_1
//Arbitrary Red Value
#define RED 1
#define CIRCUM (2*3.1415*3)


//GLOBAL VARIABLES
AnalogInputPin cdsCell(CDS_SENSOR_PORT);

//FUNCTION HEADERS
void testCDS();
void checkPoint1Code();


int main() {

}

/*
Used for testing the CDS cell values and prints the value to the screen
USED FOR TESTING PURPOSES ONLY, NOT FOR CHECKING COLORS WHILE ROBOT IS RUNNING
*/
void testCDS(){
    AnalogInputPin cdsCell(FEHIO::P0_0);
    while (1 == 1){
       LCD.Clear();
       LCD.Write(cdsCell.Value());
       Sleep(0.1);
   }
}

void checkPoint1Code(){
    //ANY PRE-DRIVE SETUP STUFF

    //WAIT FOR START LIGHT
    while (cdsCell.Value() > CDS_START_THRESHOLD){}
    //GO
    //
}
