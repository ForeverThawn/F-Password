#pragma once
#include "ColorfulStr.h"

class Exc {
public:
    static void DatabaseError() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x00000000", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("数据库错误！\n", RESET);
    }

    static void DatabaseNotFound() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x0000001A", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("找不到数据库！\n", RESET);
    }

    static void PasswordInvalid() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x000000C0", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("密码不可用！\n", RESET);
    }

    static void PasswordWrong() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x000000CB", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("密码错误！\n", RESET);
    }

    static void PasswordNotAccepted() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x000000CE", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("密码不一致！\n", RESET);
    }

};


#define ONEXIT HookOnExit

class Err {
public:
    static bool HookOnExit() {  //检测退出
        return (SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE)) ? true : false;
    }

    static bool CtrlHandler(DWORD fdwctrltype) {
        switch (fdwctrltype) {
        case CTRL_C_EVENT:
            Sleep(200);
            
            return false;

        case CTRL_CLOSE_EVENT:  //关闭控制台的出口
            Sleep(200);

            return false;

        case CTRL_BREAK_EVENT:  //程序正常退出出口
            Sleep(200);
            
            return false;

        case CTRL_LOGOFF_EVENT:
        case CTRL_SHUTDOWN_EVENT:
            return false;

        default:
            return false;
        }
    }

    static void DatabaseInvalid() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x0000001E", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("数据库不可用！\n", RESET);
        system("pause>nul");
        exit(1);
    }
};