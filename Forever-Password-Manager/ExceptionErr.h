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
        ColorfulStr::prints("���벻���ã�\r\n", RESET);
    }

    static void PasswordWrong() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x000000CB", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("�������\r\n", RESET);
    }

    static void PasswordNotAccepted() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x000000CE", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("���벻һ�£�\r\n", RESET);
    }

    static void DatabaseNotFound() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x0000001A", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("�Ҳ������ݿ⣡\r\n", RESET);
    }

    static void UserNotFound() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x00002000", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("�û��������ڣ�\r\n", RESET);
    }

    static void UserAlreadyExists() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x0000200A", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("�û����Ѿ����ڣ� \r\n[Tips]: ���Գ��Ը�����һ��д���Դ洢\r\n", RESET);
    }

    static void CommandInvalid() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x00004000", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("δ֪���� \r\n", RESET);
        ColorfulStr::prints("[Tips]: ", RESET);
        ColorfulStr::prints("ʹ�� ", GREY);
        ColorfulStr::prints(":h", LAKE_BLUE);
        ColorfulStr::prints(" ����ȡ����\r\n", GREY);
    }

    static void CommandInaccessible() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x00004001", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("��ǰ�������������� \r\n", RESET);
        ColorfulStr::prints("[Tips]: ", RESET);
        ColorfulStr::prints("ʹ�� ", GREY);
        ColorfulStr::prints(":h", LAKE_BLUE);
        ColorfulStr::prints(" ����ȡ����\r\n", GREY);
    }
};


#define ONEXIT HookOnExit

class Err {
public:
    inline static bool HookOnExit() {  //����˳�
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

    static void DatabaseError() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x00000000", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("���ݿ����\r\n", RESET);
        system("pause>nul");
        exit(1);
    }

    static void DatabaseInvalid() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x0000001E", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("���ݿⲻ���ã�\r\n", RESET);
        system("pause>nul");
        exit(1);
    }

    static void DatabaseVersionReadInvalid() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x0000001F", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("���ݿⲻ���ã�\r\n", RESET);
        system("pause>nul");
        exit(1);
    }

    static void DatabasePasswordReadInvalid() {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x0000001D", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("���ݿⲻ���ã�\r\n", RESET);
        system("pause>nul");
        exit(1);
    }

    static void JsonParseError(void) {
        ColorfulStr::prints("ERROR: ", RED);
        ColorfulStr::prints("[", RESET);
        ColorfulStr::prints("0x00000070", YELLOW);
        ColorfulStr::prints("]", RESET);
        ColorfulStr::prints("JSON��������\r\n", RESET);
        system("pause>nul");
        exit(1);
    }
    
    inline static void UserBreakProcessWithCls(void) {  // �û� :q �˳�
        system("cls");
        exit(2);
    }

    inline static void UserBreakProcess(void) {  // �û� :qn �˳�
        exit(2);
    }
};