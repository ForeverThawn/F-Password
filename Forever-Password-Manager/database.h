#pragma once

#define loop while(true)

typedef unsigned char BINARY;

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <conio.h>

// 自己写的库
#include "ColorfulStr.h"
#include "ExceptionErr.h"
#include "database.h"
#include "mainSwitch.h"
//#ifdef _USING_SEQLIST
//#include "SeqList.h"    // 功能不如标准库 放弃了
//#endif

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
/*
* * * * * * * * * **** **** **重要** **** **** * * * * * * * * * *
* ==============================================================
*
* jsoncpp.cpp 并非原版 4396行代码被我修改 为了更好兼容ANSI下的中文字符
*
* ==============================================================
* * * * * * * * * **** **** **重要** **** **** * * * * * * * * * *
*/

#define FILE_END ios::end
#define FILE_START ios::beg
#define MAX_SHA256_LEN 0x40
#define NOISE_VALUE 72 //0x7FFF 干扰不用太大
#define DATABASE_HEADER "C92CBD0AD96BC9C601C326708F1E7AC027F706585E8D0E025F7ECB9E96B33E7D" // [Forever Password Manager]
#define DATABASE_VERSION_KEY "q84Jn+zJs2E7wpGhwNBGfy3LxTZW2l3M" // 随机生成的
#define SHA256_SIZE (0x40 * sizeof(char))
#define AES_SIZE (0x20 * sizeof(char))
#define INIT_DATABASE_JSON "{\"DatabaseInfo\":{\"user\": \"ForeverPasswordManager\", \"value\":\"1.0.0.0\", \"details\":\" \"},\"MetaInfo\":[]}"

#define AES_ECB_MODE
#define DATABASE_AES_KEY "pkUPwHfEYUDWq2ak0yuts0fyFXsd1y7m"  // 后续得改 数据库aeskey

#define DEBUG cout

using CryBytePtr = CryptoPP::byte*;
using namespace CryptoPP;
using namespace std;
//using namespace filesystem;

const size_t FULL_HEADER_LENGTH = SHA256_SIZE * 2/*文件头+主密码*/ + AES_SIZE/*版本号*/ + 3/*分隔符*/;

#ifdef AES_CBC_MODE
CryptoPP::byte iv[32];
#endif

namespace Encrypt {
    static string sha256(const string& text);

#ifdef AES_CBC_MODE // CBC模式
    static string aes(const string& key, const string& plain_text);
#endif

#ifdef AES_ECB_MODE // EDB模式
    static string aes(const string& key, const string& plain_text);
#endif

    static string advanced_noise(const string& message, uint32_t noise);     // 凯撒法
}

namespace Decrypt {
#ifdef AES_CBC_MODE
    static string aes(const string& key, const string& cipher);
#endif
#ifdef AES_ECB_MODE // ECB模式
    static string aes(const string& key, const string& cipher_text);
#endif
    static string advanced_noise(const string& message, uint32_t noise);     // 凯撒法
}

class SHA256Container {
public:
    string text;
    string encrypted;          // 加密
    string encrypted_advanced; // 凯撒

public:
    SHA256Container();
    SHA256Container(string plain_text);
    void encrypt(void);
};

class AESContainer {
public:
    string text;
    string encrypted;          // 加密
    string encrypted_advanced; // 凯撒

public:
    AESContainer();
    AESContainer(string key, string plain_text);
    void encrypt();
};

typedef struct Password {  // 从我另一个项目移植来的
    char password_string[100];

    int GetPassword()
    {
        int index = 0;
        while (1)
        {
            char ch;
            ch = _getch();
            if (ch == 8)   // 退格
            {
                if (index != 0)
                {
                    putchar('\b');
                    putchar(' ');
                    putchar('\b');
                    index--;
                }
            }
            else if (ch == '\r')  // 回车
            {
                password_string[index] = '\0';
                putchar('\n');
                break;
            }
            else
            {
                putchar('*');
                password_string[index++] = ch;
            }
        }
        return 0;
    }
} Password;


class Database {
public:
    static const string internal_version;

private:
    const string database_path = "database.dat";
    ifstream iDatabase;                  // 输入流
    ofstream oDatabase;                  // 输出流
    string full_header;                  // 完整文件头 长度 FULL_HEADER_LENGTH
    bool fDatabaseValid = false;         // 数据库可用flag
    char* dBuf_str = nullptr;            // new char[0x7FFF];  // 数据库buffer 用于输入   **** 释放内存 ****
    string database_version;
    Json::Value buf_json;                // 用json存
    enum DatabaseJsonIndex {             // get之后存的vector下标
        User,
        Value,
        Details
    };

#ifdef _STRUCT_INLINE
    struct dBuf {
#endif
        string buf_string;               // 数据库buffer 用于输出
#ifdef _USING_SEQLIST
        SeqList<string> user;  // 用户名
        SeqList<string> value;  // 密码值
        SeqList<string> details;  // 备注其他
        void saveToBuffer() {   // 输出
            for (size_t i = 0; i < user.size(); i++)
                buf_string = user.at(i);
            for (size_t i = 0; i < value.size(); i++)
                buf_string += value.at(i);
            for (size_t i = 0; i < details.size(); i++)
                buf_string += details.at(i);
        }
#endif

        void parseFromChar(void) {  // 输入
#ifdef _USING_SEQLIST
            DEBUG << dBuf_str << endl;
            buf_string = string(dBuf_str);
            DEBUG << buf_string << endl;

            vector<string> parse_buffer = ColorfulStr::StringSplit(buf_string, "s");   // 整体段分割
            vector<string> parse_user = ColorfulStr::StringSplit(parse_buffer[0], "s");     // 数据分割
            vector<string> parse_value = ColorfulStr::StringSplit(parse_buffer[1], "s");    // 数据分割
            vector<string> parse_details = ColorfulStr::StringSplit(parse_buffer[2], "s");  // 数据分割

            for (size_t i = 0; i < parse_user.size(); i++)
                user.push(parse_user[i]);
            for (size_t i = 0; i < parse_value.size(); i++)
                value.push(parse_value[i]);
            for (size_t i = 0; i < parse_details.size(); i++)
                details.push(parse_details[i]);
            buf_string = string(dBuf_str);

            //*
            for (const auto& i : parse_buffer)
                cout << i << "  ";
            for (size_t i = 0; i < parse_user.size(); i++)
                cout << user.at(i) << "  ";
            for (size_t i = 0; i < parse_value.size(); i++)
                cout << value.at(i) << "  ";
            for (size_t i = 0; i < parse_details.size(); i++)
                cout << details.at(i) << "  ";
#else
            buf_string = string(dBuf_str);
            size_t end_of_string_index = buf_string.rfind('=');
            buf_string.erase(end_of_string_index);
#endif  
        }

#ifdef _STRUCT_INLINE
    } dBuf;
#endif
    streampos database_size_except_header;
    streampos database_size;
    Password password;


public:
    Database();
    ~Database();

    inline static const string version() {
        return internal_version;
    }

    void initDatabase(void);
/*
*           文件头 版本号 主密码   (s分割 =文件尾)
*           数据 ["user", "value", "details"]
*
*           读的时候都读到dBuf_str 然后用parseFromChar()得到buf_json
*           存的时候从buf_json开始封装存到database.dat
*
*/
    ErrorLevel CreateDatabase(void);

    ErrorLevel save(void);// 保存dBuf流 

    void add(const string& user, const string& value, const string& details);

    // 从user判断存在
    bool exists(const string& user);

    // 从user找value details
    bool get(/*in*/const string& user, /*out*/ string& ret_value, /*out*/ string& ret_details);

    // 遍历user列表
    vector<string> travel(void);

    // 从user删除数据
    ErrorLevel erase(const string& user);


private:
    Json::Value parseFromJson(const string& str) {
        Json::CharReaderBuilder ReaderBuilder;
        ReaderBuilder["emitUTF8"] = true;  //utf8支持 否则中文字符会\uxxx

        unique_ptr<Json::CharReader> charread(ReaderBuilder.newCharReader());

        Json::Value ret;

        string strerr;
        bool isok = charread->parse(str.c_str(), str.c_str() + str.size(), &ret, &strerr);
        if (!isok || strerr.size() != 0) {
            Err::JsonParseError();  // json解析错误
        }

        return ret;
    }


};


void ShowHelp(void) {
    ColorfulStr::printEnter();
    ColorfulStr::prints("用法:\r\n", RESET);
    //ColorfulStr::prints("    \r\n", RESET);
    //ColorfulStr::prints("\r\n", RESET);
    //ColorfulStr::prints("\r\n", RESET);
    //ColorfulStr::prints("\r\n", RESET);
    //ColorfulStr::prints("\r\n", RESET);
    ColorfulStr::prints("控制台命令:", RESET);
    ColorfulStr::prints("    直接输入用户名即可查询相应数据\r\n", GREY);
    ColorfulStr::prints("    :add        ", YELLOW);
    ColorfulStr::prints("添加条目到数据库\r\n", RESET);
    ColorfulStr::prints("    :check      ", YELLOW);
    ColorfulStr::prints("检索数据库用户名列表\r\n", RESET);
    ColorfulStr::prints("    :del        ", YELLOW);
    ColorfulStr::prints("从数据库中删除条目\r\n", RESET);
    ColorfulStr::prints("    :version    ", YELLOW);
    ColorfulStr::prints("查看版本号\r\n", RESET);
    ColorfulStr::prints("    :c          ", YELLOW);
    ColorfulStr::prints("清屏\r\n", RESET);
    ColorfulStr::prints("    :s          ", YELLOW);
    ColorfulStr::prints("保存\r\n", RESET);
    ColorfulStr::prints("\r\n", RESET);
    ColorfulStr::prints("全局命令:", RESET);
    ColorfulStr::prints("    命令行程序任意界面输入\r\n", GREY);
    ColorfulStr::prints("    :q          ", YELLOW);
    ColorfulStr::prints("不保存并清屏退出\r\n", RESET);
    ColorfulStr::prints("    :qs         ", YELLOW);
    ColorfulStr::prints("保存并清屏退出\r\n", RESET);
    ColorfulStr::prints("    :qn         ", YELLOW);
    ColorfulStr::prints("不保存直接退出\r\n", RESET);
    ColorfulStr::prints("    :b          ", YELLOW);
    ColorfulStr::prints("返回上一层\r\n", RESET);
    ColorfulStr::prints("\r\n", RESET);
    ColorfulStr::prints("未实现命令行直接操作数据库, 因为那样的操作将被视为危险操作\r\n", RED);
    ColorfulStr::prints("无参数则直接进入shell\r\n", GREY);
    return;
}

// 内部版本号
const string Database::internal_version = "1.0-zh_CN";

/*
* cmd      获得的command
* Database 数据库实例 便于delete
* prefix   输入前导
*/
ErrorLevel __stdcall GetCmd(string& cmd, 
                            Database& database_instance,
                            const string prefix = "" ,
                            bool print_inaccessible_exception = true 
                            )
{
    if (!prefix.empty())
        ColorfulStr::prints(prefix.c_str(), YELLOW);

    ColorfulStr::prints(">>", RESET);
    // std::cin >> cmd;
    getline(cin, cmd);
    if (cmd.find_first_of(':') == 0)
    {
        if (cmd == ":q") {
            database_instance.~Database();
            Err::UserBreakProcessWithCls();
        }
        if (cmd == ":qs") {
            database_instance.save();
            database_instance.~Database();
            Err::UserBreakProcessWithCls();
        }
        if (cmd == ":qn") {
            database_instance.~Database();
            Err::UserBreakProcess();
        }
        if (cmd == ":b")
            return Ok;  // 在主cpp里 continue一次
        if (cmd == ":help" || cmd == ":h")
        {
            ShowHelp();
            return Ok;
        }
        if (cmd == ":s" ||
            cmd == ":save" ||
            cmd == ":c" ||
            cmd == ":cls" ||
            cmd == ":check" ||
            cmd == ":add" ||
            cmd == ":del" ||
            cmd == ":version" ||
            cmd == ":v")
        {
            if (print_inaccessible_exception)
                Exc::CommandInaccessible();
            return Ok;  // 忽略
        }
        else
            Exc::CommandInvalid();
    }
    return Ok;
}

// C92CBD0AD96BC9C601C326708F1E7AC027F706585E8D0E025F7ECB9E96B33E7DsD90AA7305DAFD6E3D5915D0436F757D5sA665A45920422F9D417E4867EFDC4FB8A04A1F3FFF1FA07E998E86F7F7A27AE3s9B6D7AADBDAEC9B6F925F79ABD7CFF09B20F3ABA5E486B2597EB8397979DEE7301F6D495455C3ED50FE93ABA1401B1FF1C351239509D337613F6BB2D6D94556E3A77A342277EFA43427C4A69018BFA5CD3A26E65FA149C9190112BC0BDA9EF947750E76F0A313A98EE8B91DAD9BC674D1ECFD17ACCA4BCDBDDB1FBA4F36463514D337951B286FEE5265C725B59FD4398D70F51A237DAC69FC98CB8011264F2A57750E76F0A313A98EE8B91DAD9BC674D1ECFD17ACCA4BCDBDDB1FBA4F364635123652DE4669FE5018C5CF829735EC4BAD4BEBD0B9488137FDC0A1FA94CDF97F3CF5ADDBFB80DAB4AD11039FB5EACAC69=