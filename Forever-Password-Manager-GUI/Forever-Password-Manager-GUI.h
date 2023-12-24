#pragma once

#include "resource.h"
#include <Windows.h>
#include <WinUser.h>
//#include <windowsx.h>
#include <vector>
#include <string>

#include "ExceptionErr.h"
//#include "database.h"

// cryptopp加密库
#include "cryptopp/cryptlib.h"
#include "cryptopp/aes.h"
#include "cryptopp/modes.h"
#include "cryptopp/filters.h"
#include "cryptopp/hex.h"
#include "cryptopp/sha.h"

// JSONCPP
#include "json.h"
#include "json-forwards.h"

// 我编译的时候windows.h里有的宏要求amd64 编译的时候按编译器来看这三种自己define启用就行
#undef x86
#undef x86_64
#define amd64
// 回车键自动点击OK 不想实现了 精力不足 会有好多bug
//#define SUBWINDOW_USE_RETURN_AS_OK_BTN

// 我习惯宏DEBUG用来测试输出 DEBUGW弹窗     // 非控制台程序我不用DEBUG
#define DEBUG 
#define DEBUGW(x) MessageBox(hWnd_main, x, "TODO", MB_OK)

#define MAX_PASSWORD_SIZE MAX_PATH         // 密码最长长度


#ifdef SUBWINDOW_USE_RETURN_AS_OK_BTN
static LONG subWindow_loginEdit_old;     // SetWindowLong接收值
#endif

inline void FollowMainWindow(HWND& sub_window); // 跟随主窗口
inline bool CheckDatabaseAccessibility(void);  // 数据库访问权限检测用

// 窗口过程回调函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK subWndProc_login(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK subWndProc_add(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK subWndProc_modify(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK subWndProc_new(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 0x属性[1位] 操作分类[1位] 项[2位]
// 节约编译开销
/*
constexpr auto BTN_LOGIN_CLICK                   { 0x0001 };    // 登入数据库 
constexpr auto BTN_VIEW_CLICK                    { 0x0002 };    // 查看密码 
constexpr auto BTN_VIEW_ENABLE                   { 0x0D02 };    // 启用查看密码按钮
constexpr auto LIST_                             { 0x1000 };    // 数据库显示主列表
constexpr auto LIST_CLICK                        { 0x1E01 };    // 列表点击事件
constexpr auto BTN_ADD_CLICK                     { 0x0011 };    // 添加条目
constexpr auto BTN_DEL_CLICK                     { 0x0012 };    // 删除条目
constexpr auto BTN_EDIT_CLICK                    { 0x0013 };    // 编辑条目
constexpr auto SUBWINDOW_LOGIN_                  { 0xB000 };    // 验证主密码的子窗口
constexpr auto SUBWINDOW_LOGIN_EDIT_             { 0xB001 };    // 验证主密码的子窗口的编辑
constexpr auto SUBWINDOW_LOGIN_BTN_OK_CLICK      { 0xB002 };    // 验证主密码子窗口 确认
constexpr auto SUBWINDOW_LOGIN_BTN_CANCEL_CLICK  { 0xB003 };    // 验证主密码子窗口 取消
constexpr auto SUBWINDOW_LOGIN_CHECK_PASSWORD    { 0xBE04 };    // 验证主密码子窗口 检查密码
constexpr auto SUBWINDOW_ADD_BTN_OK_CLICK        { 0xC002 };    // 添加 确认
constexpr auto SUBWINDOW_ADD_BTN_CANCEL_CLICK    { 0xC002 };    // 添加 确认
*/
#define BTN_LOGIN_CLICK                   0x0001    // 登入数据库 
#define BTN_VIEW_CLICK                    0x0002    // 查看密码 
#define BTN_VIEW_ENABLE                   0x0D02    // 启用查看密码按钮
#define LIST_                             0x1000    // 数据库显示主列表
#define LIST_CLICK                        0x1E01    // 列表点击事件
#define BTN_ADD_CLICK                     0x0011    // 添加条目
#define BTN_DEL_CLICK                     0x0012    // 删除条目
#define BTN_MODIFY_CLICK                  0x0013    // 更改按钮
#define SUBWINDOW_LOGIN_                  0xB000    // 验证主密码的子窗口
#define SUBWINDOW_LOGIN_EDIT_             0xB001    // 验证主密码的子窗口的编辑
#define SUBWINDOW_LOGIN_BTN_OK_CLICK      0xB002    // 验证主密码子窗口确认
#define SUBWINDOW_LOGIN_BTN_CANCEL_CLICK  0xB003    // 验证主密码子窗口取消
#define SUBWINDOW_LOGIN_CHECK_PASSWORD    0xBE04    // 验证主密码子窗口取消
#define SUBWINDOW_ADD_                    0xC000    // 添加 子窗口
#define SUBWINDOW_ADD_EDIT_USER           0xC001    // 添加 子窗口 用户名 编辑栏
#define SUBWINDOW_ADD_EDIT_VALUE          0xC002    // 添加 子窗口 密码 编辑栏
#define SUBWINDOW_ADD_EDIT_DETAILS        0xC003    // 添加 子窗口 备注 编辑栏
#define SUBWINDOW_ADD_BTN_OK_CLICK        0xC004    // 添加 确认
#define SUBWINDOW_ADD_BTN_CANCEL_CLICK    0xC005    // 添加 取消
#define SUBWINDOW_MODIFY_                 0xD000    // 更改 子窗口
#define SUBWINDOW_MODIFY_EDIT_USER        0xD001    // 更改 子窗口 用户名 编辑栏
#define SUBWINDOW_MODIFY_EDIT_VALUE       0xD002    // 更改 子窗口 密码 编辑栏
#define SUBWINDOW_MODIFY_EDIT_DETAILS     0xD003    // 更改 子窗口 备注 编辑栏
#define SUBWINDOW_MODIFY_BTN_OK_CLICK     0xD004    // 更改 确认
#define SUBWINDOW_MODIFY_BTN_CANCEL_CLICK 0xD005    // 更改 取消
#define SUBWINDOW_NEW_                    0xE000    // 新建数据库主密码的子窗口
#define SUBWINDOW_NEW_EDIT_               0xE001    // 新建数据库主密码的子窗口的编辑
#define SUBWINDOW_NEW_BTN_OK_CLICK        0xE002    // 新建数据库主密码子窗口确认
#define SUBWINDOW_NEW_BTN_CANCEL_CLICK    0xE003    // 新建数据库主密码子窗口取消

const std::string help_doc{
    "帮助文档：\n"
    "\n"
    "注意：该GUI交互界面仅用于测试用途\n"
    "详情issues请关注\n"
    "https://github.com/ForeverThawn/Forever-Password-Manager\n"
    "\n"
    "使用 【加载数据库】 来加载已存在的加密数据库\n"
    "使用 【 查看密码 】 来读取并解密选中条目\n"
    "使用 【   添加   】  来向数据库中添加新条目\n"
    "使用 【   删除   】  来向数据库中删除已有条目\n"
    "使用 【   更改   】  来向数据库中更改已有条目\n"
};