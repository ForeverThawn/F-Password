#pragma once

#include "resource.h"
#include <Windows.h>
#include <WinUser.h>
//#include <windowsx.h>
#include <vector>
#include <string>

#include "ExceptionErr.h"
//#include "database.h"

// cryptopp���ܿ�
#include "cryptopp/cryptlib.h"
#include "cryptopp/aes.h"
#include "cryptopp/modes.h"
#include "cryptopp/filters.h"
#include "cryptopp/hex.h"
#include "cryptopp/sha.h"

// JSONCPP
#include "json.h"
#include "json-forwards.h"

// �ұ����ʱ��windows.h���еĺ�Ҫ��amd64 �����ʱ�򰴱����������������Լ�define���þ���
#undef x86
#undef x86_64
#define amd64
// �س����Զ����OK ����ʵ���� �������� ���кö�bug
//#define SUBWINDOW_USE_RETURN_AS_OK_BTN

// ��ϰ�ߺ�DEBUG����������� DEBUGW����     // �ǿ���̨�����Ҳ���DEBUG
#define DEBUG 
#define DEBUGW(x) MessageBox(hWnd_main, x, "TODO", MB_OK)

#define MAX_PASSWORD_SIZE MAX_PATH         // ���������


#ifdef SUBWINDOW_USE_RETURN_AS_OK_BTN
static LONG subWindow_loginEdit_old;     // SetWindowLong����ֵ
#endif

inline void FollowMainWindow(HWND& sub_window); // ����������
inline bool CheckDatabaseAccessibility(void);  // ���ݿ����Ȩ�޼����

// ���ڹ��̻ص�����
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK subWndProc_login(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK subWndProc_add(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK subWndProc_modify(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK subWndProc_new(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 0x����[1λ] ��������[1λ] ��[2λ]
// ��Լ���뿪��
/*
constexpr auto BTN_LOGIN_CLICK                   { 0x0001 };    // �������ݿ� 
constexpr auto BTN_VIEW_CLICK                    { 0x0002 };    // �鿴���� 
constexpr auto BTN_VIEW_ENABLE                   { 0x0D02 };    // ���ò鿴���밴ť
constexpr auto LIST_                             { 0x1000 };    // ���ݿ���ʾ���б�
constexpr auto LIST_CLICK                        { 0x1E01 };    // �б����¼�
constexpr auto BTN_ADD_CLICK                     { 0x0011 };    // �����Ŀ
constexpr auto BTN_DEL_CLICK                     { 0x0012 };    // ɾ����Ŀ
constexpr auto BTN_EDIT_CLICK                    { 0x0013 };    // �༭��Ŀ
constexpr auto SUBWINDOW_LOGIN_                  { 0xB000 };    // ��֤��������Ӵ���
constexpr auto SUBWINDOW_LOGIN_EDIT_             { 0xB001 };    // ��֤��������Ӵ��ڵı༭
constexpr auto SUBWINDOW_LOGIN_BTN_OK_CLICK      { 0xB002 };    // ��֤�������Ӵ��� ȷ��
constexpr auto SUBWINDOW_LOGIN_BTN_CANCEL_CLICK  { 0xB003 };    // ��֤�������Ӵ��� ȡ��
constexpr auto SUBWINDOW_LOGIN_CHECK_PASSWORD    { 0xBE04 };    // ��֤�������Ӵ��� �������
constexpr auto SUBWINDOW_ADD_BTN_OK_CLICK        { 0xC002 };    // ��� ȷ��
constexpr auto SUBWINDOW_ADD_BTN_CANCEL_CLICK    { 0xC002 };    // ��� ȷ��
*/
#define BTN_LOGIN_CLICK                   0x0001    // �������ݿ� 
#define BTN_VIEW_CLICK                    0x0002    // �鿴���� 
#define BTN_VIEW_ENABLE                   0x0D02    // ���ò鿴���밴ť
#define LIST_                             0x1000    // ���ݿ���ʾ���б�
#define LIST_CLICK                        0x1E01    // �б����¼�
#define BTN_ADD_CLICK                     0x0011    // �����Ŀ
#define BTN_DEL_CLICK                     0x0012    // ɾ����Ŀ
#define BTN_MODIFY_CLICK                  0x0013    // ���İ�ť
#define SUBWINDOW_LOGIN_                  0xB000    // ��֤��������Ӵ���
#define SUBWINDOW_LOGIN_EDIT_             0xB001    // ��֤��������Ӵ��ڵı༭
#define SUBWINDOW_LOGIN_BTN_OK_CLICK      0xB002    // ��֤�������Ӵ���ȷ��
#define SUBWINDOW_LOGIN_BTN_CANCEL_CLICK  0xB003    // ��֤�������Ӵ���ȡ��
#define SUBWINDOW_LOGIN_CHECK_PASSWORD    0xBE04    // ��֤�������Ӵ���ȡ��
#define SUBWINDOW_ADD_                    0xC000    // ��� �Ӵ���
#define SUBWINDOW_ADD_EDIT_USER           0xC001    // ��� �Ӵ��� �û��� �༭��
#define SUBWINDOW_ADD_EDIT_VALUE          0xC002    // ��� �Ӵ��� ���� �༭��
#define SUBWINDOW_ADD_EDIT_DETAILS        0xC003    // ��� �Ӵ��� ��ע �༭��
#define SUBWINDOW_ADD_BTN_OK_CLICK        0xC004    // ��� ȷ��
#define SUBWINDOW_ADD_BTN_CANCEL_CLICK    0xC005    // ��� ȡ��
#define SUBWINDOW_MODIFY_                 0xD000    // ���� �Ӵ���
#define SUBWINDOW_MODIFY_EDIT_USER        0xD001    // ���� �Ӵ��� �û��� �༭��
#define SUBWINDOW_MODIFY_EDIT_VALUE       0xD002    // ���� �Ӵ��� ���� �༭��
#define SUBWINDOW_MODIFY_EDIT_DETAILS     0xD003    // ���� �Ӵ��� ��ע �༭��
#define SUBWINDOW_MODIFY_BTN_OK_CLICK     0xD004    // ���� ȷ��
#define SUBWINDOW_MODIFY_BTN_CANCEL_CLICK 0xD005    // ���� ȡ��
#define SUBWINDOW_NEW_                    0xE000    // �½����ݿ���������Ӵ���
#define SUBWINDOW_NEW_EDIT_               0xE001    // �½����ݿ���������Ӵ��ڵı༭
#define SUBWINDOW_NEW_BTN_OK_CLICK        0xE002    // �½����ݿ��������Ӵ���ȷ��
#define SUBWINDOW_NEW_BTN_CANCEL_CLICK    0xE003    // �½����ݿ��������Ӵ���ȡ��

const std::string help_doc{
    "�����ĵ���\n"
    "\n"
    "ע�⣺��GUI������������ڲ�����;\n"
    "����issues���ע\n"
    "https://github.com/ForeverThawn/Forever-Password-Manager\n"
    "\n"
    "ʹ�� ���������ݿ⡿ �������Ѵ��ڵļ������ݿ�\n"
    "ʹ�� �� �鿴���� �� ����ȡ������ѡ����Ŀ\n"
    "ʹ�� ��   ���   ��  �������ݿ����������Ŀ\n"
    "ʹ�� ��   ɾ��   ��  �������ݿ���ɾ��������Ŀ\n"
    "ʹ�� ��   ����   ��  �������ݿ��и���������Ŀ\n"
};