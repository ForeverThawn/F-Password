#pragma comment(lib, "cryptlib.lib")
#include "Forever-Password-Manager-GUI.h"
#include "database.h"

Database d;
// ���崰������
const char g_szClassName[] = "ForeverPasswordManagerWindowClass";

// ����������ȫ�ֻ�
HINSTANCE hInstance;
HINSTANCE hPrevInstance;
LPSTR lpCmdLine;
int nCmdShow;
 
struct desktop
{
    int width;                            // ������
    int height;                           // ����߶�
} desktop;

HWND hWnd_main;                           // �����򴰿�
HWND hWnd_subWindow_login;                // ���������Ӵ���
HWND hWnd_subWindow_add;                  // ��� �Ӵ���
HWND hWnd_subWindow_modify;               // ���� �Ӵ���
HWND hWnd_subWindow_new;                  // �½����ݿ� �Ӵ���

HWND loginButton;   // 0x0001             // ���밴ť
HWND viewButton;    // 0x0002             // �鿴��Ŀ��ť
HWND passwordList;  // 0x1000             // ��Ŀ��ʾ�б�
HWND addButton;     // 0x0011             // ��Ӱ�ť
HWND deleteButton;  // 0x0012             // ɾ����ť
HWND modifyButton;  // 0x0013             // ���İ�ť
HWND subWindow_loginEdit;                 // ���������Ӵ��������
HWND subWindow_loginOKButton;             // ���������Ӵ���ȷ��
HWND subWindow_loginCancelButton;         // ���������Ӵ���ȡ��
HWND subWindow_addEditUser;               // ��� �Ӵ��� �û�����
HWND subWindow_addEditValue;              // ��� �Ӵ��� �����
HWND subWindow_addEditDetails;            // ��� �Ӵ��� ��ע��
HWND subWindow_addOKButton;               // ��� �Ӵ��� ȷ��
HWND subWindow_addCancelButton;           // ��� �Ӵ��� ȡ��
HWND subWindow_modifyEditUser;            // ���� �Ӵ��� �û����� (���� �����޸�)
HWND subWindow_modifyEditValue;           // ���� �Ӵ��� �����
HWND subWindow_modifyEditDetails;         // ���� �Ӵ��� ��ע��
HWND subWindow_modifyOKButton;            // ���� �Ӵ��� ȷ��
HWND subWindow_modifyCancelButton;        // ���� �Ӵ��� ȡ��
HWND subWindow_newEdit;                 // ���������Ӵ��������
HWND subWindow_newOKButton;             // ���������Ӵ���ȷ��
HWND subWindow_newCancelButton;         // ���������Ӵ���ȡ��

WNDCLASSEX mainWindow { 0 };              // ��������
WNDCLASSEX subWindow_login { 0 };         // ��½������
WNDCLASSEX subWindow_add { 0 };           // ��Ӵ�����
WNDCLASSEX subWindow_modify { 0 };        // ���Ĵ�����
WNDCLASSEX subWindow_new { 0 };           // �½�������


struct ListEntry
{
    std::string user;
    std::string value;
    std::string details;
};

std::vector<ListEntry> display_list;
int selected_index;                         // �б�ѡ���index

// ����ʱ���õ�����
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
    ifstream test_path("database.dat"); // �� database_path һ��
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
                MessageBox(hWnd_main, TEXT("����ɹ���"), TEXT("��ʾ"), MB_OK);
                return true;

            case ID_ABOUT:
                MessageBox(hWnd, TEXT("TODO ������Ϣ"), TEXT("����"), MB_OK);
                return true;

            case ID_HELP:
                MessageBox(hWnd, help_doc.c_str(), TEXT("����"), MB_OK);
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
                // û�о��½����ݿ�
                if (!CheckDatabaseAccessibility())
                {
                    if (!Exc::DatabaseNotFound(hWnd_main)) // ȡ��ֱ��break
                        return true;
                    ShowWindow(hWnd_subWindow_new, SW_SHOW);
                    FollowMainWindow(hWnd_subWindow_new);
                    return true;
                }
                ShowWindow(hWnd_subWindow_login, SW_SHOW);
                FollowMainWindow(hWnd_subWindow_login);
                return true;
            }

            case BTN_ADD_CLICK:  // ��Ӱ�ť
            {
                if (d.isValid())
                {
                    // ����������Χ ��ʾ
                    ShowWindow(hWnd_subWindow_add, SW_SHOW);
                    FollowMainWindow(hWnd_subWindow_add);
                }
                return true;
            }

            case BTN_VIEW_CLICK:  // �鿴��ť
            {
                // �ü������ݿ�ɹ������ݿ���Ч��ִ��
                if (selected_index != LB_ERR && d.isValid() == true)
                {
                    const ListEntry& entry = display_list[selected_index];
                    std::string buf = "�˺�:\t";
                    buf += entry.user;
                    buf += "\n\n����:\t";
                    buf += entry.value;
                    buf += "\n\n��ע:\t";
                    buf += entry.details;
                    // ��������Ի�����ʾ��Ӧ������
                    MessageBox(hWnd, buf.c_str(), "��Ŀ��ϸ", MB_ICONINFORMATION | MB_OK);
                }
                return true;
            }      

            case BTN_DEL_CLICK:
            {
                // �ü������ݿ�ɹ������ݿ���Ч��ִ��
                if (selected_index != LB_ERR && d.isValid() == true) 
                {
                    const ListEntry& entry = display_list[selected_index];

                    std::string buf = "ȷ��ɾ����Ŀ:  ";
                    buf += entry.user;
                    buf += "  ��?";

                    int ret = MessageBox(hWnd, buf.c_str(), "ɾ��ȷ��", MB_ICONINFORMATION | MB_OKCANCEL);

                    if (ret != IDOK)
                        return false;

                    // ɾ������Ϊuser����Ŀ
                    d.erase(entry.user);
                    display_list.erase(display_list.begin() + selected_index);

                    // ˢ��
                    // ����б���е�����
                    SendMessage(passwordList, LB_RESETCONTENT, 0, 0);

                    // ���û�����ӵ��б����
                    for (const auto& e : display_list)
                    {
                        SendMessage(passwordList, LB_ADDSTRING, 0, (LPARAM)e.user.c_str());
                    }

                    // ��������Ի�����ʾ��Ӧ������
                    if (!d.exists(entry.user))
                        MessageBox(hWnd, "��ɾ��1����Ŀ", "�ɹ�", MB_OK);
                }
                return true;
            }

            case BTN_MODIFY_CLICK:
                // �ü������ݿ�ɹ������ݿ���Ч��ִ��
                if (selected_index != LB_ERR && d.isValid() == true)
                {
                    // ����������Χ ��ʾ
                    ShowWindow(hWnd_subWindow_modify, SW_SHOW);
                    FollowMainWindow(hWnd_subWindow_modify);
                    // �Զ�����
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

            case LIST_CLICK:  // �б���
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
                    GetWindowText(subWindow_loginEdit, password_in_edit, length + 1); //����Ҫ+1

                    //DEBUGW(password_in_edit);

                    // �����ʼ�� ������ȷ��ʼ����� �������messagebox
                    if (!d.initDatabase(password_in_edit))
                    {
                        Exc::PasswordWrong(hWnd_subWindow_login);
                        SetWindowText(subWindow_loginEdit, "");
                        return false;
                    }

                    // �����Ӵ���
                    DestroyWindow(hWnd_subWindow_login);

                    vector<string> user_list = d.travel();  // ��ȡ�û����б�

                    // �������ݵ���ʾ�б�
                    for (const auto& i : user_list)
                    {
                        string value;
                        string details;
                        if (d.get(i, value, details))
                            display_list.push_back({ i, value, details });
                    }

                    // ��ʾ���밴ť����
                    //SetWindowLong(viewButton, GWL_STYLE, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON);
                    // �˴�ֱ����ʾ
                    // ����б���е�����
                    SendMessage(passwordList, LB_RESETCONTENT, 0, 0);

                    // ���û�����ӵ��б����
                    for (const auto& entry : display_list)
                    {
                        SendMessage(passwordList, LB_ADDSTRING, 0, (LPARAM)entry.user.c_str());
                    }

                    // �������ݿⰴť���ɼ�
                    //ShowWindow(loginButton, SW_HIDE);

                    return true;
                }

                case SUBWINDOW_LOGIN_BTN_CANCEL_CLICK:
                    // ��ղ�����
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
                    GetWindowText(subWindow_newEdit, password_in_edit, length + 1); //����Ҫ+1

                    //DEBUGW(password_in_edit);

                    // �½����ݿ�
                    d.CreateDatabase(password_in_edit);

                    // �����Ӵ���
                    DestroyWindow(hWnd_subWindow_new);

                    return true;
                }

                case SUBWINDOW_NEW_BTN_CANCEL_CLICK:
                    // ��ղ�����
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
                    GetWindowText(subWindow_addEditUser, user_in_edit, user_length + 1); //����Ҫ+1
                    GetWindowText(subWindow_addEditValue, value_in_edit, value_length + 1);
                    GetWindowText(subWindow_addEditDetails, details_in_edit, details_length + 1);

                    std::string buf = "��ȷ�������Ŀ: \n\n�˺�:  ";
                    buf += user_in_edit;
                    buf += "\n��\160��:  ";
                    buf += value_in_edit;
                    buf += "\n��\160ע:  ";
                    buf += details_in_edit;

                    int ret = MessageBox(hWnd_subWindow_modify,
                        buf.c_str(),
                        "����ȷ��",
                        MB_ICONINFORMATION | MB_OKCANCEL);

                    if (ret != IDOK)
                        return false;

                    // ���ݿⲻ����ʱ�����
                    if (!d.isValid())
                    {
                        Exc::DatabaseNotFound(hWnd_subWindow_add);
                        SetWindowText(subWindow_addEditUser, "");
                        SetWindowText(subWindow_addEditValue, "");
                        SetWindowText(subWindow_addEditDetails, "");
                        return false;
                    }

                    // �û������������
                    if (d.exists(string(user_in_edit)))
                    {
                        Exc::UserAlreadyExists(hWnd_subWindow_add);
                        return false;
                    }

                    d.add(string(user_in_edit), 
                          string(value_in_edit), 
                          string(details_in_edit));

                    // ��ղ�����
                    ShowWindow(hWnd_subWindow_add, SW_HIDE);
                    SetWindowText(subWindow_addEditUser, "");
                    SetWindowText(subWindow_addEditValue, "");
                    SetWindowText(subWindow_addEditDetails, "");

                    display_list.push_back({ user_in_edit, value_in_edit, details_in_edit });
                    
                    // ��ʾ���밴ť����
                    //SetWindowLong(viewButton, GWL_STYLE, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON);
                    // �˴�ֱ����ʾ
                    // ����б���е�����
                    SendMessage(passwordList, LB_RESETCONTENT, 0, 0);

                    // ���û�����ӵ��б����
                    for (const auto& entry : display_list)
                    {
                        SendMessage(passwordList, LB_ADDSTRING, 0, (LPARAM)entry.user.c_str());
                    }

                    // �������ݿⰴť���ɼ�
                    //ShowWindow(loginButton, SW_HIDE);

                    // ���֪ͨ
                    MessageBox(hWnd_main, "�����ϣ�", "�ɹ�", MB_OK);

                    return true;
                }

                case SUBWINDOW_ADD_BTN_CANCEL_CLICK:
                    // ��ղ�����
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
                    GetWindowText(subWindow_modifyEditUser, user_in_edit, user_length + 1); //����Ҫ+1
                    GetWindowText(subWindow_modifyEditValue, value_in_edit, value_length + 1);
                    GetWindowText(subWindow_modifyEditDetails, details_in_edit, details_length + 1);

                    std::string buf = "��ȷ�ϸ�����Ŀ:  \n";
                    buf += entry.user;
                    buf += "\n\n��\160��:  ";
                    buf += entry.value;
                    buf += " ==����Ϊ=> ";
                    buf += value_in_edit;
                    buf += "\n\n��\160ע:  "; 
                    buf += entry.details;
                    buf += " ==����Ϊ=> ";
                    buf += details_in_edit;

                    int ret = MessageBox(hWnd_subWindow_modify, 
                                         buf.c_str(),
                                         "����ȷ��",
                                         MB_ICONINFORMATION | MB_OKCANCEL);

                    if (ret != IDOK)
                        return false;

                    // ���ݿⲻ����ʱ�����
                    if (!d.isValid())
                    {
                        Exc::DatabaseNotFound(hWnd_subWindow_modify);
                        SetWindowText(subWindow_modifyEditUser, "");
                        SetWindowText(subWindow_modifyEditValue, "");
                        SetWindowText(subWindow_modifyEditDetails, "");
                        return false;
                    }

                    // ɾ��ԭ�ȵ�
                    d.erase(user_in_edit);

                    // �����µ�
                    d.add(string(user_in_edit),
                        string(value_in_edit),
                        string(details_in_edit));


                    // ��ղ�����
                    ShowWindow(hWnd_subWindow_modify, SW_HIDE);
                    SetWindowText(subWindow_modifyEditUser, "");
                    SetWindowText(subWindow_modifyEditValue, "");
                    SetWindowText(subWindow_modifyEditDetails, "");


                    display_list.erase(display_list.begin() + selected_index);
                    display_list.push_back({ user_in_edit, value_in_edit, details_in_edit });

                    // ��ʾ���밴ť����
                    //SetWindowLong(viewButton, GWL_STYLE, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON);
                    // �˴�ֱ����ʾ
                    // ����б���е�����
                    SendMessage(passwordList, LB_RESETCONTENT, 0, 0);

                    // ���û�����ӵ��б����
                    for (const auto& entry : display_list)
                    {
                        SendMessage(passwordList, LB_ADDSTRING, 0, (LPARAM)entry.user.c_str());
                    }

                    // �������ݿⰴť���ɼ�
                    //ShowWindow(loginButton, SW_HIDE);

                    return true;
                }

                case SUBWINDOW_MODIFY_BTN_CANCEL_CLICK:
                    // ��ղ�����
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

// ���ڹ��̻ص�����
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        // �������ݿ� ����������
        loginButton = CreateWindow("BUTTON", "�������ݿ�", 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            10, 10, 100, 30, hWnd, reinterpret_cast<HMENU>(BTN_LOGIN_CLICK), nullptr, nullptr);

        // �����������б�
        passwordList = CreateWindow("LISTBOX", "��Ŀ", 
            WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_NOTIFY,
            10, 50, 300, 200, hWnd, reinterpret_cast<HMENU>(LIST_CLICK), nullptr, nullptr);

        // ���
        addButton = CreateWindow("BUTTON", "���", 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            315, 50, 70, 40, hWnd, reinterpret_cast<HMENU>(BTN_ADD_CLICK), nullptr, nullptr);

        // ɾ��
        deleteButton = CreateWindow("BUTTON", "ɾ��", 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            315, 100, 70, 40, hWnd, reinterpret_cast<HMENU>(BTN_DEL_CLICK), nullptr, nullptr);

        // ����
        modifyButton = CreateWindow("BUTTON", "����", 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            315, 150, 70, 40, hWnd, reinterpret_cast<HMENU>(BTN_MODIFY_CLICK), nullptr, nullptr);

        
        viewButton = CreateWindow("BUTTON", "�鿴����", 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, //WS_DISABLED // �����ã������߼� 
            120, 10, 100, 30, hWnd, reinterpret_cast<HMENU>(BTN_VIEW_CLICK), nullptr, nullptr);

        break;
    }

    case WM_CLOSE:
    {
        if (!d.isValid())  // δ����ֱ����ֹ
            DefWindowProc(hWnd, msg, wParam, lParam);

        int confirm = MessageBox(hWnd, "�˳�ʱ���棿", "�˳�", MB_ICONQUESTION | MB_YESNOCANCEL);
        switch (confirm)
        {
        case IDYES:
            d.save();
            MessageBox(hWnd_main, "����ɹ���", "��ʾ", MB_OK);
            return DefWindowProc(hWnd, msg, wParam, lParam);  // ֹͣ

        case IDNO:
            return DefWindowProc(hWnd, msg, wParam, lParam);  // ֹͣ

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
        // �����
        subWindow_loginEdit = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_PASSWORD,
            10, 10, 200, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_LOGIN_EDIT_), nullptr, nullptr);

        // ����ȷ����ť
        subWindow_loginOKButton = CreateWindow("BUTTON", "ȷ��", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            20, 50, 80, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_LOGIN_BTN_OK_CLICK), nullptr, nullptr);

        // ����ȡ����ť
        subWindow_loginCancelButton = CreateWindow("BUTTON", "ȡ��", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            120, 50, 80, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_LOGIN_BTN_CANCEL_CLICK), nullptr, nullptr);

        // ���LONG Ȼ�����µĺ���������ñ༭���Ҽ��˵��ͻس��¼�
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
        // ���ر� �Ӵ���Ҫ��ղ�����
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
        // �����
        subWindow_addEditUser = CreateWindow("EDIT", "", 
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            10, 10, 200, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_ADD_EDIT_USER), nullptr, nullptr);

        subWindow_addEditValue = CreateWindow("EDIT", "", 
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_PASSWORD,
            10, 45, 200, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_ADD_EDIT_VALUE), nullptr, nullptr);

        subWindow_addEditDetails = CreateWindow("EDIT", "", 
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            10, 80, 200, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_ADD_EDIT_DETAILS), nullptr, nullptr);



        // ����ȷ����ť
        subWindow_addOKButton = CreateWindow("BUTTON", "ȷ��",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            20, 120, 80, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_ADD_BTN_OK_CLICK), nullptr, nullptr);

        // ����ȡ����ť
        subWindow_addCancelButton = CreateWindow("BUTTON", "ȡ��",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            120, 120, 80, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_ADD_BTN_CANCEL_CLICK), nullptr, nullptr);

        // ���LONG Ȼ�����µĺ���������ñ༭���Ҽ��˵��ͻس��¼�
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
        // ���ر� �Ӵ���Ҫ��ղ�����
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
        // �����
        subWindow_modifyEditUser = CreateWindow("EDIT", "",
            WS_CHILD | WS_VISIBLE | WS_BORDER | WS_DISABLED, // ��λ����
            10, 10, 200, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_MODIFY_EDIT_USER), nullptr, nullptr);

        subWindow_modifyEditValue = CreateWindow("EDIT", "",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_PASSWORD,
            10, 45, 200, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_MODIFY_EDIT_VALUE), nullptr, nullptr);

        subWindow_modifyEditDetails = CreateWindow("EDIT", "",
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            10, 80, 200, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_MODIFY_EDIT_DETAILS), nullptr, nullptr);

        // ����ȷ����ť
        subWindow_modifyOKButton = CreateWindow("BUTTON", "ȷ��",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            20, 120, 80, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_MODIFY_BTN_OK_CLICK), nullptr, nullptr);

        // ����ȡ����ť
        subWindow_modifyCancelButton = CreateWindow("BUTTON", "ȡ��",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            120, 120, 80, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_MODIFY_BTN_CANCEL_CLICK), nullptr, nullptr);

        // ���LONG Ȼ�����µĺ���������ñ༭���Ҽ��˵��ͻس��¼�
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
        // ���ر� �Ӵ���Ҫ��ղ�����
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
        // �����
        subWindow_newEdit = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER /*| ES_PASSWORD*/,
            10, 10, 200, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_NEW_EDIT_), nullptr, nullptr);

        // ����ȷ����ť
        subWindow_newOKButton = CreateWindow("BUTTON", "ȷ��", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            20, 50, 80, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_NEW_BTN_OK_CLICK), nullptr, nullptr);

        // ����ȡ����ť
        subWindow_newCancelButton = CreateWindow("BUTTON", "ȡ��", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            120, 50, 80, 30, hWnd, reinterpret_cast<HMENU>(SUBWINDOW_NEW_BTN_CANCEL_CLICK), nullptr, nullptr);
        break;
    }

    case WM_CLOSE:
        // ���ر� �Ӵ���Ҫ��ղ�����
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
    // call ��ȥ
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
 
    // ������
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
        Err::RegisterWindowFailed();  // ע��������ʧ��
        return -1;
    }

    // �Ӵ��� ��֤������
    subWindow_login.cbSize = sizeof(WNDCLASSEX);
    subWindow_login.lpfnWndProc = subWndProc_login;
    subWindow_login.hInstance = hInstance;
    subWindow_login.lpszClassName = "LoginWindow";
    subWindow_login.lpszMenuName = NULL;
    subWindow_login.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FOREVERPASSWORDMANAGERGUI));
    if (!RegisterClassEx(&subWindow_login))
    {
        Err::RegisterWindowFailed();  // ע���Ӵ���ʧ��
        return -1;
    }

    // �Ӵ��� �½����ݿ�������
    subWindow_new.cbSize = sizeof(WNDCLASSEX);
    subWindow_new.lpfnWndProc = subWndProc_new;
    subWindow_new.hInstance = hInstance;
    subWindow_new.lpszClassName = "NewDatabaseWindow";
    subWindow_new.lpszMenuName = NULL;
    subWindow_new.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FOREVERPASSWORDMANAGERGUI));
    if (!RegisterClassEx(&subWindow_new))
    {
        Err::RegisterWindowFailed();  // ע���Ӵ���ʧ��
        return -1;
    }

    // �Ӵ��� �������Ŀ
    subWindow_add.cbSize = sizeof(WNDCLASSEX);
    subWindow_add.lpfnWndProc = subWndProc_add;
    subWindow_add.hInstance = hInstance;
    subWindow_add.lpszClassName = "AddWindow";
    subWindow_add.lpszMenuName = NULL;
    subWindow_add.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FOREVERPASSWORDMANAGERGUI));
    if (!RegisterClassEx(&subWindow_add))
    {
        Err::RegisterWindowFailed();  // ע���Ӵ���ʧ��
        return -1;
    }

    // �Ӵ��� �༭��Ŀ
    subWindow_modify.cbSize = sizeof(WNDCLASSEX);
    subWindow_modify.lpfnWndProc = subWndProc_modify;
    subWindow_modify.hInstance = hInstance;
    subWindow_modify.lpszClassName = "ModifyWindow";
    subWindow_modify.lpszMenuName = NULL;
    subWindow_modify.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FOREVERPASSWORDMANAGERGUI));
    if (!RegisterClassEx(&subWindow_modify))
    {
        Err::RegisterWindowFailed();  // ע���Ӵ���ʧ��
        return -1;
    }


    // ��������
    hWnd_main = CreateWindowEx(0, g_szClassName, "���������",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT,
        405, 320, nullptr, nullptr, hInstance, nullptr);
    if (hWnd_main == NULL)
    {
        Err::WindowInitFailed();
        return -1;
    }

    // ����ͼ��
    SendMessage(hWnd_main, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FOREVERPASSWORDMANAGERGUI)));
    SendMessage(hWnd_main, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)));

    // �����ڴ�С
    SetWindowLong(hWnd_main, GWL_STYLE, GetWindowLong(hWnd_main, GWL_STYLE) & ~(WS_SIZEBOX | WS_MAXIMIZEBOX));

    MoveWindow(hWnd_main, (desktop.width >> 1) - 202, (desktop.height >> 1) - 160, 405, 320, FALSE);
    
    // ��ʾ�����´���
    ShowWindow(hWnd_main, nCmdShow);
    UpdateWindow(hWnd_main);

    // �Ӵ���Ԥ��  64λ��GWLP_HINSTANCE 32λ��GWL_HINSTANCE
#ifdef amd64
    hWnd_subWindow_login = CreateWindowEx(0, "LoginWindow", "��������", 
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX | WS_BORDER | WS_CHILDWINDOW | WS_POPUP , CW_USEDEFAULT, CW_USEDEFAULT,
        235, 135, hWnd_main, nullptr, (HINSTANCE)GetWindowLong(hWnd_main, GWLP_HINSTANCE), nullptr);

    hWnd_subWindow_add = CreateWindowEx(0, "AddWindow", "<�˺�> <����> <��ע>",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX | WS_BORDER | WS_CHILDWINDOW | WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT,
        235, 195, hWnd_main, nullptr, (HINSTANCE)GetWindowLong(hWnd_main, GWLP_HINSTANCE), nullptr);

    hWnd_subWindow_modify = CreateWindowEx(0, "ModifyWindow", "�༭��Ŀ",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX | WS_BORDER | WS_CHILDWINDOW | WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT,
        235, 195, hWnd_main, nullptr, (HINSTANCE)GetWindowLong(hWnd_main, GWLP_HINSTANCE), nullptr);

    hWnd_subWindow_new = CreateWindowEx(0, "NewDatabaseWindow", "����������",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX | WS_BORDER | WS_CHILDWINDOW | WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT,
        235, 135, hWnd_main, nullptr, (HINSTANCE)GetWindowLong(hWnd_main, GWLP_HINSTANCE), nullptr);
#elif x86
    hWnd_subWindow_login = CreateWindowEx(0, "LoginWindow", "��������", /*WS_CHILD*/ WS_POPUP | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        150, 80, hWnd_main, nullptr, (HINSTANCE)GetWindowLong(hWnd_main, GWL_HINSTANCE), nullptr);

    hWnd_subWindow_add = CreateWindowEx(0, "AddWindow", "<�˺�> <����> <��ע>",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX | WS_BORDER | WS_CHILDWINDOW | WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT,
        235, 195, hWnd_main, nullptr, (HINSTANCE)GetWindowLong(hWnd_main, GWL_HINSTANCE), nullptr);

    hWnd_subWindow_modify = CreateWindowEx(0, "ModifyWindow", "�༭��Ŀ",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX | WS_BORDER | WS_CHILDWINDOW | WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT,
        235, 195, hWnd_main, nullptr, (HINSTANCE)GetWindowLong(hWnd_main, GWL_HINSTANCE), nullptr);

    hWnd_subWindow_new = CreateWindowEx(0, "NewDatabaseWindow", "����������",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX | WS_BORDER | WS_CHILDWINDOW | WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT,
        235, 135, hWnd_main, nullptr, (HINSTANCE)GetWindowLong(hWnd_main, GWL_HINSTANCE), nullptr);
#elif x86_64
    hWnd_subWindow_login = CreateWindowEx(0, "LoginWindow", "��������", /*WS_CHILD*/ WS_POPUP | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        150, 80, hWnd_main, nullptr, (HINSTANCE)GetWindowLong(hWnd_main, GWL_HINSTANCE), nullptr);

    hWnd_subWindow_add = CreateWindowEx(0, "AddWindow", "<�˺�> <����> <��ע>",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX | WS_BORDER | WS_CHILDWINDOW | WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT,
        235, 195, hWnd_main, nullptr, (HINSTANCE)GetWindowLong(hWnd_main, GWL_HINSTANCE), nullptr);

    hWnd_subWindow_modify = CreateWindowEx(0, "ModifyWindow", "�༭��Ŀ",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX | WS_BORDER | WS_CHILDWINDOW | WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT,
        235, 195, hWnd_main, nullptr, (HINSTANCE)GetWindowLong(hWnd_main, GWL_HINSTANCE), nullptr);

    hWnd_subWindow_new = CreateWindowEx(0, "NewDatabaseWindow", "����������",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX | WS_BORDER | WS_CHILDWINDOW | WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT,
        235, 135, hWnd_main, nullptr, (HINSTANCE)GetWindowLong(hWnd_main, GWL_HINSTANCE), nullptr);
#endif
    if (hWnd_subWindow_modify == NULL || hWnd_subWindow_add == NULL || hWnd_subWindow_login == NULL)
        Err::WindowInitFailed();

    // ��Ϣѭ��
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
 *                            ������¥
 *          ��Ի:
 *                  д��¥��д�ּ䣬д�ּ������Ա��
 *                  ������Աд�������ó��򻻾�Ǯ��
 *                  ����ֻ���������������������ߣ�
 *                  ��������ո��գ����������긴�ꡣ
 *                  ��Ը�������Լ䣬��Ը�Ϲ��ϰ�ǰ��
 *                  ���۱������Ȥ���������г���Ա��
 *                  ����Ц��߯��񲣬��Ц�Լ���̫����
 *                  ��������Ư���ã��ĸ���ó���Ա��
 */