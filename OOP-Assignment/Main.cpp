#include "Manager.h"

int main(int argc, char* argv[])
{
    // Create object of manager class and give it window size
    Manager manager(640, 480);

    // Use manager object's menu function to run the menu
    manager.menu();

    return 0;
}