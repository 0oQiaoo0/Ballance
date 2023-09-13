#include "Logger.h"
#include "stdio.h"
#include "windows.h"
void printRedText(const std::string& text)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
    std::cout << text;
    //white
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}