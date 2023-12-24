#pragma once

#define loop while(true)

typedef unsigned char BINARY;

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <conio.h>

// �Լ�д�Ŀ�
#include "ColorfulStr.h"
#include "ExceptionErr.h"
#include "database.h"
#include "mainSwitch.h"
//#ifdef _USING_SEQLIST
//#include "SeqList.h"    // ���ܲ����׼�� ������
//#endif

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
/*
* * * * * * * * * **** **** **��Ҫ** **** **** * * * * * * * * * *
* ==============================================================
*
* jsoncpp.cpp ����ԭ�� 4396�д��뱻���޸� Ϊ�˸��ü���ANSI�µ������ַ�
*
* ==============================================================
* * * * * * * * * **** **** **��Ҫ** **** **** * * * * * * * * * *
*/

#define FILE_END ios::end
#define FILE_START ios::beg
#define MAX_SHA256_LEN 0x40
#define NOISE_VALUE 72 //0x7FFF ���Ų���̫��
#define DATABASE_HEADER "C92CBD0AD96BC9C601C326708F1E7AC027F706585E8D0E025F7ECB9E96B33E7D" // [Forever Password Manager]
#define DATABASE_VERSION_KEY "q84Jn+zJs2E7wpGhwNBGfy3LxTZW2l3M" // ������ɵ�
#define SHA256_SIZE (0x40 * sizeof(char))
#define AES_SIZE (0x20 * sizeof(char))
#define INIT_DATABASE_JSON "{\"DatabaseInfo\":{\"user\": \"ForeverPasswordManager\", \"value\":\"1.0.0.0\", \"details\":\" \"},\"MetaInfo\":[]}"

#define AES_ECB_MODE
#define DATABASE_AES_KEY "pkUPwHfEYUDWq2ak0yuts0fyFXsd1y7m"  // �����ø� ���ݿ�aeskey

#define DEBUG cout

using CryBytePtr = CryptoPP::byte*;
using namespace CryptoPP;
using namespace std;
//using namespace filesystem;

const size_t FULL_HEADER_LENGTH = SHA256_SIZE * 2/*�ļ�ͷ+������*/ + AES_SIZE/*�汾��*/ + 3/*�ָ���*/;

#ifdef AES_CBC_MODE
CryptoPP::byte iv[32];
#endif

namespace Encrypt {
    static string sha256(const string& text);

#ifdef AES_CBC_MODE // CBCģʽ
    static string aes(const string& key, const string& plain_text);
#endif

#ifdef AES_ECB_MODE // EDBģʽ
    static string aes(const string& key, const string& plain_text);
#endif

    static string advanced_noise(const string& message, uint32_t noise);     // ������
}

namespace Decrypt {
#ifdef AES_CBC_MODE
    static string aes(const string& key, const string& cipher);
#endif
#ifdef AES_ECB_MODE // ECBģʽ
    static string aes(const string& key, const string& cipher_text);
#endif
    static string advanced_noise(const string& message, uint32_t noise);     // ������
}

class SHA256Container {
public:
    string text;
    string encrypted;          // ����
    string encrypted_advanced; // ����

public:
    SHA256Container();
    SHA256Container(string plain_text);
    void encrypt(void);
};

class AESContainer {
public:
    string text;
    string encrypted;          // ����
    string encrypted_advanced; // ����

public:
    AESContainer();
    AESContainer(string key, string plain_text);
    void encrypt();
};

typedef struct Password {  // ������һ����Ŀ��ֲ����
    char password_string[100];

    int GetPassword()
    {
        int index = 0;
        while (1)
        {
            char ch;
            ch = _getch();
            if (ch == 8)   // �˸�
            {
                if (index != 0)
                {
                    putchar('\b');
                    putchar(' ');
                    putchar('\b');
                    index--;
                }
            }
            else if (ch == '\r')  // �س�
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
    ifstream iDatabase;                  // ������
    ofstream oDatabase;                  // �����
    string full_header;                  // �����ļ�ͷ ���� FULL_HEADER_LENGTH
    bool fDatabaseValid = false;         // ���ݿ����flag
    char* dBuf_str = nullptr;            // new char[0x7FFF];  // ���ݿ�buffer ��������   **** �ͷ��ڴ� ****
    string database_version;
    Json::Value buf_json;                // ��json��
    enum DatabaseJsonIndex {             // get֮����vector�±�
        User,
        Value,
        Details
    };

#ifdef _STRUCT_INLINE
    struct dBuf {
#endif
        string buf_string;               // ���ݿ�buffer �������
#ifdef _USING_SEQLIST
        SeqList<string> user;  // �û���
        SeqList<string> value;  // ����ֵ
        SeqList<string> details;  // ��ע����
        void saveToBuffer() {   // ���
            for (size_t i = 0; i < user.size(); i++)
                buf_string = user.at(i);
            for (size_t i = 0; i < value.size(); i++)
                buf_string += value.at(i);
            for (size_t i = 0; i < details.size(); i++)
                buf_string += details.at(i);
        }
#endif

        void parseFromChar(void) {  // ����
#ifdef _USING_SEQLIST
            DEBUG << dBuf_str << endl;
            buf_string = string(dBuf_str);
            DEBUG << buf_string << endl;

            vector<string> parse_buffer = ColorfulStr::StringSplit(buf_string, "s");   // ����ηָ�
            vector<string> parse_user = ColorfulStr::StringSplit(parse_buffer[0], "s");     // ���ݷָ�
            vector<string> parse_value = ColorfulStr::StringSplit(parse_buffer[1], "s");    // ���ݷָ�
            vector<string> parse_details = ColorfulStr::StringSplit(parse_buffer[2], "s");  // ���ݷָ�

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
*           �ļ�ͷ �汾�� ������   (s�ָ� =�ļ�β)
*           ���� ["user", "value", "details"]
*
*           ����ʱ�򶼶���dBuf_str Ȼ����parseFromChar()�õ�buf_json
*           ���ʱ���buf_json��ʼ��װ�浽database.dat
*
*/
    ErrorLevel CreateDatabase(void);

    ErrorLevel save(void);// ����dBuf�� 

    void add(const string& user, const string& value, const string& details);

    // ��user�жϴ���
    bool exists(const string& user);

    // ��user��value details
    bool get(/*in*/const string& user, /*out*/ string& ret_value, /*out*/ string& ret_details);

    // ����user�б�
    vector<string> travel(void);

    // ��userɾ������
    ErrorLevel erase(const string& user);


private:
    Json::Value parseFromJson(const string& str) {
        Json::CharReaderBuilder ReaderBuilder;
        ReaderBuilder["emitUTF8"] = true;  //utf8֧�� ���������ַ���\uxxx

        unique_ptr<Json::CharReader> charread(ReaderBuilder.newCharReader());

        Json::Value ret;

        string strerr;
        bool isok = charread->parse(str.c_str(), str.c_str() + str.size(), &ret, &strerr);
        if (!isok || strerr.size() != 0) {
            Err::JsonParseError();  // json��������
        }

        return ret;
    }


};


void ShowHelp(void) {
    ColorfulStr::printEnter();
    ColorfulStr::prints("�÷�:\r\n", RESET);
    //ColorfulStr::prints("    \r\n", RESET);
    //ColorfulStr::prints("\r\n", RESET);
    //ColorfulStr::prints("\r\n", RESET);
    //ColorfulStr::prints("\r\n", RESET);
    //ColorfulStr::prints("\r\n", RESET);
    ColorfulStr::prints("����̨����:", RESET);
    ColorfulStr::prints("    ֱ�������û������ɲ�ѯ��Ӧ����\r\n", GREY);
    ColorfulStr::prints("    :add        ", YELLOW);
    ColorfulStr::prints("�����Ŀ�����ݿ�\r\n", RESET);
    ColorfulStr::prints("    :check      ", YELLOW);
    ColorfulStr::prints("�������ݿ��û����б�\r\n", RESET);
    ColorfulStr::prints("    :del        ", YELLOW);
    ColorfulStr::prints("�����ݿ���ɾ����Ŀ\r\n", RESET);
    ColorfulStr::prints("    :version    ", YELLOW);
    ColorfulStr::prints("�鿴�汾��\r\n", RESET);
    ColorfulStr::prints("    :c          ", YELLOW);
    ColorfulStr::prints("����\r\n", RESET);
    ColorfulStr::prints("    :s          ", YELLOW);
    ColorfulStr::prints("����\r\n", RESET);
    ColorfulStr::prints("\r\n", RESET);
    ColorfulStr::prints("ȫ������:", RESET);
    ColorfulStr::prints("    �����г��������������\r\n", GREY);
    ColorfulStr::prints("    :q          ", YELLOW);
    ColorfulStr::prints("�����沢�����˳�\r\n", RESET);
    ColorfulStr::prints("    :qs         ", YELLOW);
    ColorfulStr::prints("���沢�����˳�\r\n", RESET);
    ColorfulStr::prints("    :qn         ", YELLOW);
    ColorfulStr::prints("������ֱ���˳�\r\n", RESET);
    ColorfulStr::prints("    :b          ", YELLOW);
    ColorfulStr::prints("������һ��\r\n", RESET);
    ColorfulStr::prints("\r\n", RESET);
    ColorfulStr::prints("δʵ��������ֱ�Ӳ������ݿ�, ��Ϊ�����Ĳ���������ΪΣ�ղ���\r\n", RED);
    ColorfulStr::prints("�޲�����ֱ�ӽ���shell\r\n", GREY);
    return;
}

// �ڲ��汾��
const string Database::internal_version = "1.0-zh_CN";

/*
* cmd      ��õ�command
* Database ���ݿ�ʵ�� ����delete
* prefix   ����ǰ��
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
            return Ok;  // ����cpp�� continueһ��
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
            return Ok;  // ����
        }
        else
            Exc::CommandInvalid();
    }
    return Ok;
}

// C92CBD0AD96BC9C601C326708F1E7AC027F706585E8D0E025F7ECB9E96B33E7DsD90AA7305DAFD6E3D5915D0436F757D5sA665A45920422F9D417E4867EFDC4FB8A04A1F3FFF1FA07E998E86F7F7A27AE3s9B6D7AADBDAEC9B6F925F79ABD7CFF09B20F3ABA5E486B2597EB8397979DEE7301F6D495455C3ED50FE93ABA1401B1FF1C351239509D337613F6BB2D6D94556E3A77A342277EFA43427C4A69018BFA5CD3A26E65FA149C9190112BC0BDA9EF947750E76F0A313A98EE8B91DAD9BC674D1ECFD17ACCA4BCDBDDB1FBA4F36463514D337951B286FEE5265C725B59FD4398D70F51A237DAC69FC98CB8011264F2A57750E76F0A313A98EE8B91DAD9BC674D1ECFD17ACCA4BCDBDDB1FBA4F364635123652DE4669FE5018C5CF829735EC4BAD4BEBD0B9488137FDC0A1FA94CDF97F3CF5ADDBFB80DAB4AD11039FB5EACAC69=