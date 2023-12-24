#pragma once
#include "ColorfulStr.h"
#define Ok(x) { return x }  

typedef int ErrorLevel;

enum ReturnStatus {
    Error,
    Ok
};

class Exc {
public:
    static void PasswordInvalid() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x000000C0", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("密码不可用！\r\n", RESET);
    }

    static void PasswordWrong() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x000000CB", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("密码错误！\r\n", RESET);
    }

    static void PasswordNotAccepted() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x000000CE", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("密码不一致！\r\n", RESET);
    }

    static void DatabaseNotFound() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x0000001A", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("找不到数据库！\r\n", RESET);
    }

    static void UserNotFound() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x00002000", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("用户名不存在！\r\n", RESET);
    }

    static void UserAlreadyExists() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x0000200A", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("用户名已经存在！ \r\n[Tips]: 可以尝试更改另一种写法以存储\r\n", RESET);
    }

    static void CommandInvalid() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x00004000", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("未知命令 \r\n", RESET);
        ColorfulStr::prints("[Tips]: ", RESET);
        ColorfulStr::prints("使用 ", GREY);
        ColorfulStr::prints(":h", LAKE_BLUE);
        ColorfulStr::prints(" 来获取帮助\r\n", GREY);
    }

    static void CommandInaccessible() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x00004001", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("当前作用域该命令不可用 \r\n", RESET);
        ColorfulStr::prints("[Tips]: ", RESET);
        ColorfulStr::prints("使用 ", GREY);
        ColorfulStr::prints(":h", LAKE_BLUE);
        ColorfulStr::prints(" 来获取帮助\r\n", GREY);
    }
};


#define ONEXIT HookOnExit

class Err {
public:
    inline static bool HookOnExit() {  //检测退出
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

    static void DatabaseError() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x00000000", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("数据库错误！\r\n", RESET);
        system("pause>nul");
        exit(1);
    }

    static void DatabaseInvalid() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x0000001E", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("数据库不可用！\r\n", RESET);
        system("pause>nul");
        exit(1);
    }

    static void DatabaseVersionReadInvalid() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x0000001F", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("数据库不可用！\r\n", RESET);
        system("pause>nul");
        exit(1);
    }

    static void DatabasePasswordReadInvalid() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x0000001D", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("数据库不可用！\r\n", RESET);
        system("pause>nul");
        exit(1);
    }

    static void JsonParseError(void) {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x00000070", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("JSON解析出错！\r\n", RESET);
        system("pause>nul");
        exit(1);
    }
    
    inline static void UserBreakProcessWithCls(void) {  // 用户 :q 退出
        system("cls");
        exit(2);
    }

    inline static void UserBreakProcess(void) {  // 用户 :qn 退出
        exit(2);
    }
};