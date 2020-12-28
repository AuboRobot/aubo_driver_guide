#include "freedrive.h"

int main(int argc, char *argv[])
{
    FreeDrive free_drive;
    //free_drive.setSim();
    free_drive.connectToRobot();
    free_drive.join();
    return 0;
}
