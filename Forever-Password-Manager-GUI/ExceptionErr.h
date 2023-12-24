#pragma once
#include <Windows.h>
//#include "ColorfulStr.h"
#define Ok(x) { return }  

typedef int ErrorLevel;

enum ReturnStatus {
    Error,
    Ok
};

class Exc {
public:
    inline static void PasswordInvalid(HWND hWnd = nullptr) {
        // 0x000000C0
        MessageBox(hWnd, "密码不可用！", "注意", MB_ICONEXCLAMATION | MB_OK);
    }

    inline static void PasswordWrong(HWND hWnd = nullptr) {
        // 0x000000CB
        MessageBox(hWnd, "密码错误！", "注意", MB_ICONEXCLAMATION | /*MB_RETRYCANCEL*/ MB_OK);
    }

    inline static void PasswordNotAccepted(HWND hWnd = nullptr) {
        // 0x000000CE
        MessageBox(hWnd, "密码不一致！", "注意", MB_ICONEXCLAMATION | MB_OK);
    }

    inline static bool DatabaseNotFound(HWND hWnd = nullptr) {
        int choice = MessageBox(hWnd, "找不到数据库文件！\n点击 [确定] 重建，[取消] 退出", "注意", MB_ICONEXCLAMATION | MB_OKCANCEL);

        switch (choice)
        {
        case IDOK:
            return true;

        case IDCANCEL:
        default:
            return false;
        }

    }

    inline static void UserNotFound(HWND hWnd = nullptr) {
        // 0x00002000
        MessageBox(hWnd, "用户名不存在！", "注意", MB_ICONEXCLAMATION | MB_OK);
    }

    inline static void UserAlreadyExists(HWND hWnd = nullptr) {
        // 0x0000200A
        MessageBox(hWnd, "用户名已经存在！\n[Tips]: 可以尝试更改另一种写法以存储", "注意", MB_ICONEXCLAMATION | MB_OK);
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

    inline static void RegisterWindowFailed(void) {
        MessageBox(NULL, "窗口无法初始化！\n点击 [确定] 退出程序", "警告", MB_ICONERROR | MB_OK);
    }

    inline static void DatabaseError(HWND hWnd = nullptr) {
        // 0x00000000
        MessageBox(hWnd, "数据库错误！", "警告", MB_ICONERROR | MB_OK);
    }

    inline static void DatabaseInvalid(HWND hWnd = nullptr) {
        // 0x0000001E
        MessageBox(hWnd, "数据库不可用！", "警告", MB_ICONERROR | MB_OK);
    }

    inline static void DatabaseVersionReadInvalid(HWND hWnd = nullptr) {
        // 0x0000001F
        MessageBox(hWnd, "数据库不可用！", "警告", MB_ICONERROR | MB_OK);
        exit(1);
    }

    inline static void DatabasePasswordReadInvalid(HWND hWnd = nullptr) {
        // 0x0000001D
        MessageBox(hWnd, "数据库不可用！", "警告", MB_ICONERROR | MB_OK);
        exit(1);
    }

    inline static void JsonParseError(HWND hWnd = nullptr) {
        // 0x00000070
        MessageBox(hWnd, "JSON解析出错！", "警告", MB_ICONERROR | MB_OK);
        exit(1);
    }

    inline static void UserBreakProcessWithCls(HWND hWnd = nullptr) {  // 用户 :q 退出
        system("cls");
        exit(2);
    }

    inline static void UserBreakProcess(HWND hWnd = nullptr) {  // 用户 :qn 退出
        exit(2);
    }

    inline static void WindowInitFailed(void) {
        MessageBox(NULL, "窗口初始化失败！", "错误", MB_ICONERROR | MB_OK);
        exit(-1);
    }

};

