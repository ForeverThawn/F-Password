#pragma comment(lib, "cryptlib.lib")
#include "Forever-Password-Manager-GUI.h"
#include "database.h"

Database d;
// 定义窗口类名
const char g_szClassName[] = "ForeverPasswordManagerWindowClass";

// 主函数参数全局化
HINSTANCE hInstance;
HINSTANCE hPrevInstance;
LPSTR lpCmdLine;
int nCmdShow;
 
struct desktop
{
    int width;                            // 桌面宽度
    int height;                           // 桌面高度
} desktop;

HWND hWnd_main;                           // 主程序窗口
HWND hWnd_subWindow_login;                // 输入密码子窗口
HWND hWnd_subWindow_add;                  // 添加 子窗口
HWND hWnd_subWindow_modify;               // 更改 子窗口
HWND hWnd_subWindow_new;                  // 新建数据库 子窗口

HWND loginButton;   // 0x0001             // 登入按钮
HWND viewButton;    // 0x0002             // 查看条目按钮
HWND passwordList;  // 0x1000             // 条目显示列表
HWND addButton;     // 0x0011             // 添加按钮
HWND deleteButton;  // 0x0012             // 删除按钮
HWND modifyButton;  // 0x0013             // 更改按钮
HWND subWindow_loginEdit;                 // 输入密码子窗口密码框
HWND subWindow_loginOKButton;             // 输入密码子窗口确认
HWND subWindow_loginCancelButton;         // 输入密码子窗口取消
HWND subWindow_addEditUser;               // 添加 子窗口 用户名框
HWND subWindow_addEditValue;              // 添加 子窗口 密码框
HWND subWindow_addEditDetails;            // 添加 子窗口 备注框
HWND subWindow_addOKButton;               // 添加 子窗口 确认
HWND subWindow_addCancelButton;           // 添加 子窗口 取消
HWND subWindow_modifyEditUser;            // 更改 子窗口 用户名框 (锁定 不可修改)
HWND subWindow_modifyEditValue;           // 更改 子窗口 密码框
HWND subWindow_modifyEditDetails;         // 更改 子窗口 备注框
HWND subWindow_modifyOKButton;            // 更改 子窗口 确认
HWND subWindow_modifyCancelButton;        // 更改 子窗口 取消
HWND subWindow_newEdit;                 // 输入密码子窗口密码框
HWND subWindow_newOKButton;             // 输入密码子窗口确认
HWND subWindow_newCancelButton;         // 输入密码子窗口取消

WNDCLASSEX mainWindow { 0 };              // 主窗口类
WNDCLASSEX subWindow_login { 0 };         // 登陆窗口类
WNDCLASSEX subWindow_add { 0 };           // 添加窗口类
WNDCLASSEX subWindow_modify { 0 };        // 更改窗口类
WNDCLASSEX subWindow_new { 0 };           // 新建窗口类


struct ListEntry
{
    std::string user;
    std::string value;
    std::string details;
};

std::vector<ListEntry> display_list;
int selected_index;                         // 列表选择的index

// 测试时候用的数据
/*
void __stdcall pushVec()
{
    PasswordEntry a, b, c;
    a.user = "a";
    b.user = "b";
    c.user = "c";
    a.value = "test1";
    b.value = "test2";
    c.value = "test3";
    a.details = "aa";
    b.details = "bb";
    c.details = "cc";

    g_passwords.push_back(a);
    g_passwords.push_back(b);
    g_passwords.push_back(c);
}
*/

inline void FollowMainWindow(HWND& sub_window)
{
    RECT main_rect;
    GetWindowRect(hWnd_main, &main_rect);
    MoveWindow(sub_window, main_rect.left + 50, main_rect.top + 30, 235, 135, FALSE);
    return;
}

inline bool CheckDatabaseAccessibility(void)
{
    ifstream test_path("database.dat"); // 与 database_path 一样
    return test_path.good();
}

namespace Gui 
{
    namespace Menu 
    {
        bool Command(HWND& hWnd, WPARAM& wParam) 
        {
            switch (LOWORD(wParam))
            {
            case ID_SAVE:
                d.save();
                MessageBox(hWnd_main, TEXT("保存成功！"), TEXT("提示"), MB_OK);
                return true;

            case ID_ABOUT:
                MessageBox(hWnd, TEXT("TODO 关于信息"), TEXT("关于"), MB_OK);
                return true;

            case ID_HELP:
                MessageBox(hWnd, help_doc.c_str(), TEXT("帮助"), MB_OK);
                return true;

            case ID_EXITSAVE:
#ifndef DEBUG
                d.save();
#endif
                PostQuitMessage(0);
                return true;
                
            case ID_EXIT:
                PostQuitMessage(0);
                return true;
                
            default:
                return false;
            }
        }
    }

    namespace Window
    {
        bool Command(HWND& hWnd, WPARAM& wParam)
        {
            switch (LOWORD(wParam))
            {
            case BTN_LOGIN_CLICK:
            { 
                // 没有就新建数据库
                if (!CheckDatabaseAccessibility())
                {
                    if (!Exc::DatabaseNotFound(hWnd_main)) // 取消直接break
                        return true;
                    ShowWindow(hWnd_subWindow_new, SW_SHOW);
                    FollowMainWindow(hWnd_subWindow_new);
                    return true;
                }
                ShowWindow(hWnd_subWindow_login, SW_SHOW);
                FollowMainWindow(hWnd_subWindow_login);
                return true;
            }

            case BTN_ADD_CLICK:  // 添加按钮
            {
                if (d.isValid())
                {
                    // 放主窗口周围 显示
                    ShowWindow(hWnd_subWindow_add, SW_SHOW);
                    FollowMainWindow(hWnd_subWindow_add);
                }
                return true;
            }

            case BTN_VIEW_CLICK:  // 查看按钮
            {
                // 得加载数据库成功且数据库有效才执行
                if (selected_index != LB_ERR && d.isValid() == true)
                {
                    const ListEntry& entry = display_list[selected_index];
                    std::string buf = "账号:\t";
                    buf += entry.user;
                    buf += "\n\n密码:\t";
                    buf += entry.value;
                    buf += "\n\n备注:\t";
                    buf += entry.details;
                    // 弹出密码对话框并显示对应的密码
                    MessageBox(hWnd, buf.c_str(), "条目明细", MB_ICONINFORMATION | MB_OK);
                }
                return true;
            }      

            case BTN_DEL_CLICK:
            {
                // 得加载数据库成功且数据库有效才执行
                if (selected_index != LB_ERR && d.isValid() == true) 
                {
                    const ListEntry& entry = display_list[selected_index];

                    std::string buf = "确定删除条目:  ";
                    buf += entry.user;
                    buf += "  吗?";

                    int ret = MessageBox(hWnd, buf.c_str(), "删除确认", MB_ICONINFORMATION | MB_OKCANCEL);

                    if (ret != IDOK)
                        return false;

                    // 删除索引为user的条目
                    d.erase(entry.user);
                    display_list.erase(display_list.begin() + selected_index);

                    // 刷新
                    // 清空列表框中的内容
                    SendMessage(passwordList, LB_RESETCONTENT, 0, 0);

                    // 将用户名添加到列表框中
                    for (const auto& e : display_list)
                    {
                        SendMessage(passwordList, LB_ADDSTRING, 0, (LPARAM)e.user.c_str());
                    }

                    // 弹出密码对话框并显示对应的密码
                    if (!d.exists(entry.user))
                        MessageBox(hWnd, "已删除1个条目", "成功", MB_OK);
                }
                return true;
            }

            case BTN_MODIFY_CLICK:
                // 得加载数据库成功且数据库有效才执行
                if (selected_index != LB_ERR && d.isValid() == true)
                {
                    // 放主窗口周围 显示
                    ShowWindow(hWnd_subWindow_modify, SW_SHOW);
                    FollowMainWindow(hWnd_subWindow_modify);
                    // 自动补充
                    SetWindowText(subWindow_modifyEditUser, 
                                    display_list[selected_index].user.c_str());
                    SetWindowText(subWindow_modifyEditValue, 
                                    display_list[selected_index].value.c_str());
                    SetWindowText(subWindow_modifyEditDetails, 
                                    display_list[selected_index].details.c_str());
                    ShowWindow(hWnd_subWindow_modify, SW_SHOW);
                    return true;
                }
                return false;

            case LIST_CLICK:  // 列表点击
            {
                if (HIWORD(wParam) == LBN_SELCHANGE)
                    selected_index = SendMessage(passwordList, LB_GETCURSEL, 0, 0);

                break;
            }

            default:
                return false;
            }
        }
    }

    namespace SubWindow
    {
        namespace Login
        {
            bool Command(HWND& hWnd, WPARAM& wParam)
            {
                switch (LOWORD(wParam))
                {
                case SUBWINDOW_LOGIN_BTN_OK_CLICK:
                {
                    char password_in_edit[MAX_PASSWORD_SIZE];
                    size_t length = GetWindowTextLength(subWindow_loginEdit);
                    GetWindowText(subWindow_loginEdit, password_in_edit, length + 1); //长度要+1

                    //DEBUGW(password_in_edit);

                    // 从这初始化 密码正确初始化完毕 密码错误弹messagebox
                    if (!d.initDatabase(password_in_edit))
                    {
                        Exc::PasswordWrong(hWnd_subWindow_login);
                        SetWindowText(subWindow_loginEdit, "");
                        return false;
                    }

                    // 销毁子窗口
                    DestroyWindow(hWnd_subWindow_login);

                    vector<string> user_list = d.travel();  // 获取用户名列表

                    // 载入数据到显示列表
                    for (const auto& i : user_list)
                    {
                        string value;
                        string details;
                        if (d.get(i, value, details))
                            display_list.push_back({ i, value, details });
                    }

                    // 显示密码按钮可用
                    //SetWindowLong(viewButton, GWL_STYLE, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON);
                    // 此处直接显示
                    // 清空列表框中的内容
                    SendMessage(passwordList, LB_RESETCONTENT, 0, 0);

                    // 将用户名添加到列表框中
                    for (const auto& entry : display_list)
                    {
                        SendMessage(passwordList, LB_ADDSTRING, 0, (LPARAM)entry.user.c_str());
                    }

                    // 加载数据库按钮不可见
                    //ShowWindow(loginButton, SW_HIDE);

                    return true;
                }

                case SUBWINDOW_LOGIN_BTN_CANCEL_CLICK:
                    // 清空并隐藏
                    ShowWindow(hWnd_subWindow_login, SW_HIDE);
                    SetWindowText(subWindow_loginEdit, "");
                    return true;

                default:
                    return false;
                }
            }
        }

        namespace New
        {
            bool Command(HWND& hWnd, WPARAM& wParam)
            {
                switch (LOWORD(wParam))
                {
                case SUBWINDOW_NEW_BTN_OK_CLICK:
                {
                    char password_in_edit[MAX_PASSWORD_SIZE];
                    size_t length = GetWindowTextLength(subWindow_newEdit);
                    GetWindowText(subWindow_newEdit, password_in_edit, length + 1); //长度要+1

                    //DEBUGW(password_in_edit);

                    // 新建数据库
                    d.CreateDatabase(password_in_edit);

                    // 销毁子窗口
                    DestroyWindow(hWnd_subWindow_new);

                    return true;
                }

                case SUBWINDOW_NEW_BTN_CANCEL_CLICK:
                    // 清空并隐藏
                    ShowWindow(hWnd_subWindow_new, SW_HIDE);
                    SetWindowText(subWindow_newEdit, "");
                    return true;

                default:
                    return false;
                }
            }
        }

        namespace Add
        {
            bool Command(HWND& hWnd, WPARAM& wParam)
            {
                switch (LOWORD(wParam))
                {
                case SUBWINDOW_ADD_BTN_OK_CLICK:
                {
                    const ListEntry& entry = display_list[selected_index];

                    char user_in_edit[MAX_PASSWORD_SIZE];
                    char value_in_edit[MAX_PASSWORD_SIZE];
                    char details_in_edit[MAX_PASSWORD_SIZE];

                    size_t user_length = GetWindowTextLength(subWindow_addEditUser);
                    size_t value_length = GetWindowTextLength(subWindow_addEditValue);
                    size_t details_length = GetWindowTextLength(subWindow_addEditDetails);
                    GetWindowText(subWindow_addEditUser, user_in_edit, user_length + 1); //长度要+1
                    GetWindowText(subWindow_addEditValue, value_in_edit, value_length + 1);
                    GetWindowText(subWindow_addEditDetails, details_in_edit, details_length + 1);

                    std::string buf = "请确认添加条目: \n\n账号:  ";
                    buf += user_in_edit;
                    buf += "\n密\160码:  ";
                    buf += value_in_edit;
                    buf += "\n备\160注:  ";
                    buf += details_in_edit;

                    int ret = MessageBox(hWnd_subWindow_modify,
                        buf.c_str(),
                        "更改确认",
                        MB_ICONINFORMATION | MB_OKCANCEL);

                    if (ret != IDOK)
                        return false;

                    // 数据库不可用时不添加
                    if (!d.isValid())
                    {
                        Exc::DatabaseNotFound(hWnd_subWindow_add);
                        SetWindowText(subWindow_addEditUser, "");
                        SetWindowText(subWindow_addEditValue, "");
                        SetWindowText(subWindow_addEditDetails, "");
                        return false;
                    }

                    // 用户名重名不添加
                    if (d.exists(string(user_in_edit)))
                    {
                        Exc::UserAlreadyExists(hWnd_subWindow_add);
                        return false;
                    }

                    d.add(string(user_in_edit), 
                          string(value_in_edit), 
                          string(details_in_edit));

                    // 清空并隐藏
                    ShowWindow(hWnd_subWindow_add, SW_HIDE);
                    SetWindowText(subWindow_addEditUser, "");
                    SetWindowText(subWindow_addEditValue, "");
                    SetWindowText(subWindow_addEditDetails, "");

                    display_list.push_back({ user_in_edit, value_in_edit, details_in_edit });
                    
                    // 显示密码按钮可用
                    //SetWindowLong(viewButton, GWL_STYLE, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON);
                    // 此处直接显示
                    // 清空列表框中的内容
                    SendMessage(passwordList, LB_RESETCONTENT, 0, 0);

                    // 将用户名添加到列表框中
                    for (const auto& entry : display_list)
                    {
                        SendMessage(passwordList, LB_ADDSTRING, 0, (LPARAM)entry.user.c_str());
                    }

                    // 加载数据库按钮不可见
                    //ShowWindow(loginButton, SW_HIDE);

                    // 完毕通知
                    MessageBox(hWnd_main, "添加完毕！", "成功", MB_OK);

                    return true;
                }

                case SUBWINDOW_ADD_BTN_CANCEL_CLICK:
                    // 清空并隐藏
                    ShowWindow(hWnd_subWindow_add, SW_HIDE);
                    SetWindowText(subWindow_addEditUser, "");
                    SetWindowText(subWindow_addEditValue, "");
                    SetWindowText(subWindow_addEditDetails, "");
                    return true;

                default:
                    return false;
                }
            }
        }

        namespace Modify
        {
            bool Command(HWND& hWnd, WPARAM& wParam)
            {
                switch (LOWORD(wParam))
                {
                case SUBWINDOW_MODIFY_BTN_OK_CLICK:
                {
                    const ListEntry& entry = display_list[selected_index];

                    char user_in_edit[MAX_PASSWORD_SIZE];
                    char value_in_edit[MAX_PASSWORD_SIZE];
                    char details_in_edit[MAX_PASSWORD_SIZE];

                    size_t user_length = GetWindowTextLength(subWindow_modifyEditUser);
                    size_t value_length = GetWindowTextLength(subWindow_modifyEditValue);
                    size_t details_length = GetWindowTextLength(subWindow_modifyEditDetails);
                    GetWindowText(subWindow_modifyEditUser, user_in_edit, user_length + 1); //长度要+1
                    GetWindowText(subWindow_modifyEditValue, value_in_edit, value_length + 1);
                    GetWindowText(subWindow_modifyEditDetails, details_in_edit, details_length + 1);

                    std::string buf = "请确认更改条目:  \n";
                    buf += entry.user;
                    buf += "\n\n密\160码:  ";
                    buf += entry.value;
                    buf += " ==更改为=> ";
                    buf += value_in_edit;
                    buf += "\n\n备\160注:  "; 
                    buf += entry.details;
                    buf += " ==更改为=> ";
                    buf += details_in_edit;

                    int ret = MessageBox(hWnd_subWindow_modify, 
                                         buf.c_str(),
                                         "更改确认",
                                         MB_ICONINFORMATION | MB_OKCANCEL);

                    if (ret != IDOK)
                        return false;

                    // 数据库不可用时不添加
                    if (!d.isValid())
                    {
                        Exc::DatabaseNotFound(hWnd_subWindow_modify);
                        SetWindowText(subWindow_modifyEditUser, "");
                        SetWindowText(subWindow_modifyEditValue, "");
                        SetWindowText(subWindow_modifyEditDetails, "");
                        return false;
                    }

                    // 删除原先的
                    d.erase(user_in_edit);

                    // 开个新的
                    d.add(string(user_in_edit),
                        string(value_in_edit),
                        string(details_in_edit));


                    // 清空并隐藏
                    ShowWindow(hWnd_subWindow_modify, SW_HIDE);
                    SetWindowText(subWindow_modifyEditUser, "");
                    SetWindowText(subWindow_modifyEditValue, "");
                    SetWindowText(subWindow_modifyEditDetails, "");


                    display_list.erase(display_list.begin() + selected_index);
                    display_list.push_back({ user_in_edit, value_in_edit, details_in_edit });

                    // 显示密码按钮可用
                    //SetWindowLong(viewButton, GWL_STYLE, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON);
                    // 此处直接显示
                    // 清空列表框中的内容
                    SendMessage(passwordList, LB_RESETCONTENT, 0, 0);

                    // 将用户名添加到列表框中
                    for (const auto& entry : display_list)
                    {
                        SendMessage(passwordList, LB_ADDSTRING, 0, (LPARAM)entry.user.c_str());
                    }

                    // 加载数据库按钮不可见
                    //ShowWindow(loginButton, SW_HIDE);

                    return true;
                }

                case SUBWINDOW_MODIFY_BTN_CANCEL_CLICK:
                    // 清空并隐藏
                    ShowWindow(hWnd_subWindow_modify, SW_HIDE);
                    SetWindowText(subWindow_modifyEditUser, "");
                    SetWindowText(subWindow_modifyEditValue, "");
                    SetWindowText(subWindow_modifyEditDetails, "");
                    return true;

                default:
                    return false;
                }
            }
        }

    }
}

// 窗口过程回调函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        // 加载数据库 输入主密码
        loginButton = CreateWindow("BUTTON", "加载数据库", 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            10, 10, 100, 30, hWnd, reinterpret_cast<HMENU>(BTN_LOGIN_CLICK), nullptr, nullptr);

        // 主窗体密码列表
        passwordList = CreateWindow("LISTBOX", "条目", 
            WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_NOTIFY,
            10, 50, 300, 200, hWnd, reinterpret_cast<HMENU>(LIST_CLICK), nullptr, nullptr);

        // 添加
        addButton = CreateWindow("BUTTON", "添加", 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            315, 50, 70, 40, hWnd, reinterpret_cast<HMENU>(BTN_ADD_CLICK), nullptr, nullptr);

        // 删除
        deleteButton = CreateWindow("BUTTON", "删除", 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            315, 100, 70, 40, hWnd, reinterpret_cast<HMENU>(BTN_DEL_CLICK), nullptr, nullptr);

        // 更改
        modifyButton = CreateWindow("BUTTON", "更改", 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            315, 150, 70, 40, hWnd, reinterpret_cast<HMENU>(BTN_MODIFY_CLICK), nullptr, nullptr);

        
        viewButton = CreateWindow("BUTTON", "查看密码", 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, //WS_DISABLED // 不禁用，改了逻辑 
            120, 10, 100, 30, hWnd, reinterpret_cast<HMENU>(BTN_VIEW_CLICK), nullptr, nullptr);

        break;
    }

    case WM_CLOSE:
    {
        if (!d.isValid())  // 未加载直接终止
            DefWindowProc(hWnd, msg, wParam, lParam);

        int confirm = MessageBox(hWnd, "退出时保存？", "退出", MB_ICONQUESTION | MB_YESNOCANCEL);
        switch (confirm)
        {
        case IDYES:
            d.save();
            MessageBox(hWnd_main, "保存成功！", "提示", MB_OK);
            return DefWindowProc(hWnd, msg, wParam, lParam);  // 停止

        case IDNO:
            return DefWindowProc(hWnd, msg, wParam, lParam);  // 停止

        case IDCANCEL:
            return 0;

            //DestroyWindow(hWnd);
            //break;
        }
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_COMMAND:
    {
        if (Gui::Menu::Command(hWnd, wParam))
            break;
        else if (Gui::Window::Command(hWnd, wParam))
            break;
        else
            break;
    }
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK subWndProc_login(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
    {
        // 密码框
        subWindow_loginEdit = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_PASSWORD,
            10, 10, 200, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_LOGIN_EDIT_), nullptr, nullptr);

        // 创建确定按钮
        subWindow_loginOKButton = CreateWindow("BUTTON", "确定", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            20, 50, 80, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_LOGIN_BTN_OK_CLICK), nullptr, nullptr);

        // 创建取消按钮
        subWindow_loginCancelButton = CreateWindow("BUTTON", "取消", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            120, 50, 80, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_LOGIN_BTN_CANCEL_CLICK), nullptr, nullptr);

        // 存个LONG 然后用新的函数处理禁用编辑框右键菜单和回车事件
#ifdef SUBWINDOW_USE_RETURN_AS_OK_BTN
#ifdef amd64
        subWindow_loginEdit_old = SetWindowLong(subWindow_loginEdit, GWLP_WNDPROC, (LONG)subWndProc_loginEdit);
#elif x86
        subWindow_loginEdit_old = SetWindowLong(subWindow_loginEdit, GWL_WNDPROC, (LONG)subWndProc_loginEdit);
#elif x86_64
        subWindow_loginEdit_old = SetWindowLong(subWindow_loginEdit, GWLP_WNDPROC, (LONG)subWndProc_loginEdit);
#endif
#endif
        break;
    }        

    case WM_CLOSE:
        // 不关闭 子窗口要清空并隐藏
        ShowWindow(hWnd_subWindow_login, SW_HIDE);
        SetWindowText(subWindow_loginEdit, "");
        //DestroyWindow(hWnd_subWindow_login);
        break;

    case WM_DESTROY:
        //PostQuitMessage(0);
        break;

    case WM_COMMAND:
    {
        if (Gui::SubWindow::Login::Command(hWnd, wParam))
            break;
        else
            break;
    }
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK subWndProc_add(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
    {
        // 密码框
        subWindow_addEditUser = CreateWindow("EDIT", "", 
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            10, 10, 200, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_ADD_EDIT_USER), nullptr, nullptr);

        subWindow_addEditValue = CreateWindow("EDIT", "", 
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_PASSWORD,
            10, 45, 200, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_ADD_EDIT_VALUE), nullptr, nullptr);

        subWindow_addEditDetails = CreateWindow("EDIT", "", 
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            10, 80, 200, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_ADD_EDIT_DETAILS), nullptr, nullptr);



        // 创建确定按钮
        subWindow_addOKButton = CreateWindow("BUTTON", "确定",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            20, 120, 80, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_ADD_BTN_OK_CLICK), nullptr, nullptr);

        // 创建取消按钮
        subWindow_addCancelButton = CreateWindow("BUTTON", "取消",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            120, 120, 80, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_ADD_BTN_CANCEL_CLICK), nullptr, nullptr);

        // 存个LONG 然后用新的函数处理禁用编辑框右键菜单和回车事件
#ifdef SUBWINDOW_USE_RETURN_AS_OK_BTN
#ifdef amd64
        subWindow_loginEdit_old = SetWindowLong(subWindow_loginEdit, GWLP_WNDPROC, (LONG)subWndProc_loginEdit);
#elif x86
        subWindow_loginEdit_old = SetWindowLong(subWindow_loginEdit, GWL_WNDPROC, (LONG)subWndProc_loginEdit);
#elif x86_64
        subWindow_loginEdit_old = SetWindowLong(subWindow_loginEdit, GWLP_WNDPROC, (LONG)subWndProc_loginEdit);
#endif
#endif
        break;
    }

    case WM_CLOSE:
        // 不关闭 子窗口要清空并隐藏
        ShowWindow(hWnd_subWindow_add, SW_HIDE);
        SetWindowText(subWindow_addEditUser, "");
        SetWindowText(subWindow_addEditValue, "");
        SetWindowText(subWindow_addEditDetails, "");
        //DestroyWindow(hWnd_subWindow_login);
        break;

    case WM_DESTROY:
        //PostQuitMessage(0);
        break;

    case WM_COMMAND:
    {
        if (Gui::SubWindow::Add::Command(hWnd, wParam))
            break;
        else
            break;
    }
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK subWndProc_modify(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
    {
        // 密码框
        subWindow_modifyEditUser = CreateWindow("EDIT", "",
            WS_CHILD | WS_VISIBLE | WS_BORDER | WS_DISABLED, // 栏位锁定
            10, 10, 200, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_MODIFY_EDIT_USER), nullptr, nullptr);

        subWindow_modifyEditValue = CreateWindow("EDIT", "",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_PASSWORD,
            10, 45, 200, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_MODIFY_EDIT_VALUE), nullptr, nullptr);

        subWindow_modifyEditDetails = CreateWindow("EDIT", "",
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            10, 80, 200, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_MODIFY_EDIT_DETAILS), nullptr, nullptr);

        // 创建确定按钮
        subWindow_modifyOKButton = CreateWindow("BUTTON", "确定",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            20, 120, 80, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_MODIFY_BTN_OK_CLICK), nullptr, nullptr);

        // 创建取消按钮
        subWindow_modifyCancelButton = CreateWindow("BUTTON", "取消",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            120, 120, 80, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_MODIFY_BTN_CANCEL_CLICK), nullptr, nullptr);

        // 存个LONG 然后用新的函数处理禁用编辑框右键菜单和回车事件
#ifdef SUBWINDOW_USE_RETURN_AS_OK_BTN
#ifdef amd64
        subWindow_loginEdit_old = SetWindowLong(subWindow_loginEdit, GWLP_WNDPROC, (LONG)subWndProc_loginEdit);
#elif x86
        subWindow_loginEdit_old = SetWindowLong(subWindow_loginEdit, GWL_WNDPROC, (LONG)subWndProc_loginEdit);
#elif x86_64
        subWindow_loginEdit_old = SetWindowLong(subWindow_loginEdit, GWLP_WNDPROC, (LONG)subWndProc_loginEdit);
#endif
#endif
        break;
    }

    case WM_CLOSE:
        // 不关闭 子窗口要清空并隐藏
        ShowWindow(hWnd_subWindow_modify, SW_HIDE);
        SetWindowText(subWindow_modifyEditUser, "");
        SetWindowText(subWindow_modifyEditValue, "");
        SetWindowText(subWindow_modifyEditDetails, "");
        //DestroyWindow(hWnd_subWindow_login);
        break;

    case WM_DESTROY:
        //PostQuitMessage(0);
        break;

    case WM_COMMAND:
    {
        if (Gui::SubWindow::Modify::Command(hWnd, wParam))
            break;
        else
            break;
    }
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK subWndProc_new(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
    {
        // 密码框
        subWindow_newEdit = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER /*| ES_PASSWORD*/,
            10, 10, 200, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_NEW_EDIT_), nullptr, nullptr);

        // 创建确定按钮
        subWindow_newOKButton = CreateWindow("BUTTON", "确定", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            20, 50, 80, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_NEW_BTN_OK_CLICK), nullptr, nullptr);

        // 创建取消按钮
        subWindow_newCancelButton = CreateWindow("BUTTON", "取消", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            120, 50, 80, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_NEW_BTN_CANCEL_CLICK), nullptr, nullptr);
        break;
    }

    case WM_CLOSE:
        // 不关闭 子窗口要清空并隐藏
        ShowWindow(hWnd_subWindow_new, SW_HIDE);
        SetWindowText(subWindow_newEdit, "");
        //DestroyWindow(hWnd_subWindow_login);
        break;

    case WM_DESTROY:
        //PostQuitMessage(0);
        break;

    case WM_COMMAND:
    {
        if (Gui::SubWindow::New::Command(hWnd, wParam))
            break;
        else
            break;
    }
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
}
    return 0;
}

#ifdef SUBWINDOW_USE_RETURN_AS_OK_BTN
LRESULT CALLBACK subWndProc_loginEdit(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
        if (wParam == VK_RETURN)
        {
            PostMessage(subWindow_OKButton, WM_LBUTTONDOWN, 0, 0);
            PostMessage(subWindow_OKButton, WM_LBUTTONUP, 0, 0);
        }
        break;
    }
    // call 回去
    return CallWindowProc((WNDPROC)subWndProc_login, hWnd, message, wParam, lParam);
}
#endif

ErrorLevel WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
//#ifdef DEBUGW
//    pushVec();
//#endif
    desktop.width = GetSystemMetrics(0);
    desktop.height = GetSystemMetrics(1);
 
    // 主窗口
    mainWindow.cbSize = sizeof(WNDCLASSEX);
    mainWindow.lpfnWndProc = WndProc;
    mainWindow.hInstance = hInstance;
    mainWindow.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    mainWindow.lpszClassName = g_szClassName;
    mainWindow.lpszMenuName = (char*)IDC_MAINMENU;
    mainWindow.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FOREVERPASSWORDMANAGERGUI));
    mainWindow.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
    if (!RegisterClassEx(&mainWindow))
    {
        Err::RegisterWindowFailed();  // 注册主窗口失败
        return -1;
    }

    // 子窗口 验证主密码
    subWindow_login.cbSize = sizeof(WNDCLASSEX);
    subWindow_login.lpfnWndProc = subWndProc_login;
    subWindow_login.hInstance = hInstance;
    subWindow_login.lpszClassName = "LoginWindow";
    subWindow_login.lpszMenuName = NULL;
    subWindow_login.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FOREVERPASSWORDMANAGERGUI));
    if (!RegisterClassEx(&subWindow_login))
    {
        Err::RegisterWindowFailed();  // 注册子窗口失败
        return -1;
    }

    // 子窗口 新建数据库主密码
    subWindow_new.cbSize = sizeof(WNDCLASSEX);
    subWindow_new.lpfnWndProc = subWndProc_new;
    subWindow_new.hInstance = hInstance;
    subWindow_new.lpszClassName = "NewDatabaseWindow";
    subWindow_new.lpszMenuName = NULL;
    subWindow_new.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FOREVERPASSWORDMANAGERGUI));
    if (!RegisterClassEx(&subWindow_new))
    {
        Err::RegisterWindowFailed();  // 注册子窗口失败
        return -1;
    }

    // 子窗口 添加新条目
    subWindow_add.cbSize = sizeof(WNDCLASSEX);
    subWindow_add.lpfnWndProc = subWndProc_add;
    subWindow_add.hInstance = hInstance;
    subWindow_add.lpszClassName = "AddWindow";
    subWindow_add.lpszMenuName = NULL;
    subWindow_add.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FOREVERPASSWORDMANAGERGUI));
    if (!RegisterClassEx(&subWindow_add))
    {
        Err::RegisterWindowFailed();  // 注册子窗口失败
        return -1;
    }

    // 子窗口 编辑条目
    subWindow_modify.cbSize = sizeof(WNDCLASSEX);
    subWindow_modify.lpfnWndProc = subWndProc_modify;
    subWindow_modify.hInstance = hInstance;
    subWindow_modify.lpszClassName = "ModifyWindow";
    subWindow_modify.lpszMenuName = NULL;
    subWindow_modify.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FOREVERPASSWORDMANAGERGUI));
    if (!RegisterClassEx(&subWindow_modify))
    {
        Err::RegisterWindowFailed();  // 注册子窗口失败
        return -1;
    }


    // 创建窗口
    hWnd_main = CreateWindowEx(0, g_szClassName, "密码管理器",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT,
        405, 320, nullptr, nullptr, hInstance, nullptr);
    if (hWnd_main == NULL)
    {
        Err::WindowInitFailed();
        return -1;
    }

    // 设置图标
    SendMessage(hWnd_main, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FOREVERPASSWORDMANAGERGUI)));
    SendMessage(hWnd_main, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)));

    // 锁窗口大小
    SetWindowLong(hWnd_main, GWL_STYLE, GetWindowLong(hWnd_main, GWL_STYLE) & ~(WS_SIZEBOX | WS_MAXIMIZEBOX));

    MoveWindow(hWnd_main, (desktop.width >> 1) - 202, (desktop.height >> 1) - 160, 405, 320, FALSE);
    
    // 显示并更新窗口
    ShowWindow(hWnd_main, nCmdShow);
    UpdateWindow(hWnd_main);

    // 子窗口预备  64位用GWLP_HINSTANCE 32位用GWL_HINSTANCE
#ifdef amd64
    hWnd_subWindow_login = CreateWindowEx(0, "LoginWindow", "输入密码", 
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX | WS_BORDER | WS_CHILDWINDOW | WS_POPUP , CW_USEDEFAULT, CW_USEDEFAULT,
        235, 135, hWnd_main, nullptr, (HINSTANCE)GetWindowLong(hWnd_main, GWLP_HINSTANCE), nullptr);

    hWnd_subWindow_add = CreateWindowEx(0, "AddWindow", "<账号> <密码> <备注>",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX | WS_BORDER | WS_CHILDWINDOW | WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT,
        235, 195, hWnd_main, nullptr, (HINSTANCE)GetWindowLong(hWnd_main, GWLP_HINSTANCE), nullptr);

    hWnd_subWindow_modify = CreateWindowEx(0, "ModifyWindow", "编辑条目",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX | WS_BORDER | WS_CHILDWINDOW | WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT,
        235, 195, hWnd_main, nullptr, (HINSTANCE)GetWindowLong(hWnd_main, GWLP_HINSTANCE), nullptr);

    hWnd_subWindow_new = CreateWindowEx(0, "NewDatabaseWindow", "创建新密码",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX | WS_BORDER | WS_CHILDWINDOW | WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT,
        235, 135, hWnd_main, nullptr, (HINSTANCE)GetWindowLong(hWnd_main, GWLP_HINSTANCE), nullptr);
#elif x86
    hWnd_subWindow_login = CreateWindowEx(0, "LoginWindow", "输入密码", /*WS_CHILD*/ WS_POPUP | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        150, 80, hWnd_main, nullptr, (HINSTANCE)GetWindowLong(hWnd_main, GWL_HINSTANCE), nullptr);

    hWnd_subWindow_add = CreateWindowEx(0, "AddWindow", "<账号> <密码> <备注>",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX | WS_BORDER | WS_CHILDWINDOW | WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT,
        235, 195, hWnd_main, nullptr, (HINSTANCE)GetWindowLong(hWnd_main, GWL_HINSTANCE), nullptr);

    hWnd_subWindow_modify = CreateWindowEx(0, "ModifyWindow", "编辑条目",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX | WS_BORDER | WS_CHILDWINDOW | WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT,
        235, 195, hWnd_main, nullptr, (HINSTANCE)GetWindowLong(hWnd_main, GWL_HINSTANCE), nullptr);

    hWnd_subWindow_new = CreateWindowEx(0, "NewDatabaseWindow", "创建新密码",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX | WS_BORDER | WS_CHILDWINDOW | WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT,
        235, 135, hWnd_main, nullptr, (HINSTANCE)GetWindowLong(hWnd_main, GWL_HINSTANCE), nullptr);
#elif x86_64
    hWnd_subWindow_login = CreateWindowEx(0, "LoginWindow", "输入密码", /*WS_CHILD*/ WS_POPUP | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        150, 80, hWnd_main, nullptr, (HINSTANCE)GetWindowLong(hWnd_main, GWL_HINSTANCE), nullptr);

    hWnd_subWindow_add = CreateWindowEx(0, "AddWindow", "<账号> <密码> <备注>",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX | WS_BORDER | WS_CHILDWINDOW | WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT,
        235, 195, hWnd_main, nullptr, (HINSTANCE)GetWindowLong(hWnd_main, GWL_HINSTANCE), nullptr);

    hWnd_subWindow_modify = CreateWindowEx(0, "ModifyWindow", "编辑条目",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX | WS_BORDER | WS_CHILDWINDOW | WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT,
        235, 195, hWnd_main, nullptr, (HINSTANCE)GetWindowLong(hWnd_main, GWL_HINSTANCE), nullptr);

    hWnd_subWindow_new = CreateWindowEx(0, "NewDatabaseWindow", "创建新密码",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX | WS_BORDER | WS_CHILDWINDOW | WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT,
        235, 135, hWnd_main, nullptr, (HINSTANCE)GetWindowLong(hWnd_main, GWL_HINSTANCE), nullptr);
#endif
    if (hWnd_subWindow_modify == NULL || hWnd_subWindow_add == NULL || hWnd_subWindow_login == NULL)
        Err::WindowInitFailed();

    // 消息循环
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

/*                            _ooOoo_
 *                           o8888888o
 *                           88" . "88
 *                           (| -_- |)
 *                            O\ = /O
 *                        ____/`---'\____
 *                      .   ' \\| |// `.
 *                       / \\||| : |||// \
 *                     / _||||| -:- |||||- \
 *                       | | \\\ - /// | |
 *                     | \_| ''\---/'' | |
 *                      \ .-\__ `-` ___/-. /
 *                   ___`. .' /--.--\ `. . __
 *                ."" '< `.___\_<|>_/___.' >'"".
 *               | | : `- \`.;`\ _ /`;.`/ - ` : | |
 *                 \ \ `-. \_ __\ /__ _/ .-` / /
 *         ======`-.____`-.___\_____/___.-`____.-'======
 *                            `=---='
 *
 *         .............................................
 *                            佛祖镇楼
 *          佛曰:
 *                  写字楼里写字间，写字间里程序员；
 *                  程序人员写程序，又拿程序换酒钱。
 *                  酒醒只在网上坐，酒醉还来网下眠；
 *                  酒醉酒醒日复日，网上网下年复年。
 *                  但愿老死电脑间，不愿鞠躬老板前；
 *                  奔驰宝马贵者趣，公交自行程序员。
 *                  别人笑我忒疯癫，我笑自己命太贱；
 *                  不见满街漂亮妹，哪个归得程序员？
 */