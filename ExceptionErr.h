#pragma once
#include "ColorfulStr.h"

class Exc {
public:
    static void DatabaseError() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x00000000", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("���ݿ����\n", RESET);
    }

    static void DatabaseNotFound() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x0000001A", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("�Ҳ������ݿ⣡\n", RESET);
    }

    static void PasswordInvalid() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x000000C0", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("���벻���ã�\n", RESET);
    }

    static void PasswordWrong() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x000000CB", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("�������\n", RESET);
    }

    static void PasswordNotAccepted() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x000000CE", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("���벻һ�£�\n", RESET);
    }

};


#define ONEXIT HookOnExit

class Err {
public:
    static bool HookOnExit() {  //����˳�
        return (SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE)) ? true : false;
    }

    static bool CtrlHandler(DWORD fdwctrltype) {
        switch (fdwctrltype) {
        case CTRL_C_EVENT:
            Sleep(200);
            
            return false;

        case CTRL_CLOSE_EVENT:  //�رտ���̨�ĳ���
            Sleep(200);

            return false;

        case CTRL_BREAK_EVENT:  //���������˳�����
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
        ColorfulStr::prints("���ݿⲻ���ã�\n", RESET);
        system("pause>nul");
        exit(1);
    }
};