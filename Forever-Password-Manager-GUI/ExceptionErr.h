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
        MessageBox(hWnd, "���벻���ã�", "ע��", MB_ICONEXCLAMATION | MB_OK);
    }

    inline static void PasswordWrong(HWND hWnd = nullptr) {
        // 0x000000CB
        MessageBox(hWnd, "�������", "ע��", MB_ICONEXCLAMATION | /*MB_RETRYCANCEL*/ MB_OK);
    }

    inline static void PasswordNotAccepted(HWND hWnd = nullptr) {
        // 0x000000CE
        MessageBox(hWnd, "���벻һ�£�", "ע��", MB_ICONEXCLAMATION | MB_OK);
    }

    inline static bool DatabaseNotFound(HWND hWnd = nullptr) {
        int choice = MessageBox(hWnd, "�Ҳ������ݿ��ļ���\n��� [ȷ��] �ؽ���[ȡ��] �˳�", "ע��", MB_ICONEXCLAMATION | MB_OKCANCEL);

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
        MessageBox(hWnd, "�û��������ڣ�", "ע��", MB_ICONEXCLAMATION | MB_OK);
    }

    inline static void UserAlreadyExists(HWND hWnd = nullptr) {
        // 0x0000200A
        MessageBox(hWnd, "�û����Ѿ����ڣ�\n[Tips]: ���Գ��Ը�����һ��д���Դ洢", "ע��", MB_ICONEXCLAMATION | MB_OK);
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

    inline static void RegisterWindowFailed(void) {
        MessageBox(NULL, "�����޷���ʼ����\n��� [ȷ��] �˳�����", "����", MB_ICONERROR | MB_OK);
    }

    inline static void DatabaseError(HWND hWnd = nullptr) {
        // 0x00000000
        MessageBox(hWnd, "���ݿ����", "����", MB_ICONERROR | MB_OK);
    }

    inline static void DatabaseInvalid(HWND hWnd = nullptr) {
        // 0x0000001E
        MessageBox(hWnd, "���ݿⲻ���ã�", "����", MB_ICONERROR | MB_OK);
    }

    inline static void DatabaseVersionReadInvalid(HWND hWnd = nullptr) {
        // 0x0000001F
        MessageBox(hWnd, "���ݿⲻ���ã�", "����", MB_ICONERROR | MB_OK);
        exit(1);
    }

    inline static void DatabasePasswordReadInvalid(HWND hWnd = nullptr) {
        // 0x0000001D
        MessageBox(hWnd, "���ݿⲻ���ã�", "����", MB_ICONERROR | MB_OK);
        exit(1);
    }

    inline static void JsonParseError(HWND hWnd = nullptr) {
        // 0x00000070
        MessageBox(hWnd, "JSON��������", "����", MB_ICONERROR | MB_OK);
        exit(1);
    }

    inline static void UserBreakProcessWithCls(HWND hWnd = nullptr) {  // �û� :q �˳�
        system("cls");
        exit(2);
    }

    inline static void UserBreakProcess(HWND hWnd = nullptr) {  // �û� :qn �˳�
        exit(2);
    }

    inline static void WindowInitFailed(void) {
        MessageBox(NULL, "���ڳ�ʼ��ʧ�ܣ�", "����", MB_ICONERROR | MB_OK);
        exit(-1);
    }

};

