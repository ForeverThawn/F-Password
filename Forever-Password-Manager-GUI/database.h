#pragma once
#pragma comment(lib, "cryptlib.lib")
#include "Forever-Password-Manager-GUI.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

// 自己写的库
#include "ExceptionErr.h"
#include "database.h"
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

#define loop while(true)

typedef unsigned char BINARY;

#include <iostream>
#include "easystrcmp.h"

#ifdef AES_CBC_MODE
CryptoPP::byte iv[32];
#endif

namespace Encrypt {
    static std::string sha256(const std::string& text);
    static std::string aes(const std::string& key, const std::string& plain_text);
    static std::string advanced_noise(const std::string& message, uint32_t noise);     // 凯撒法
}

namespace Decrypt {
    static std::string aes(const std::string& key, const std::string& cipher_text);
    static std::string advanced_noise(const std::string& message, uint32_t noise);     // 凯撒法
}

class SHA256Container {
public:
    string text;
    string encrypted;          // 加密
    string encrypted_advanced; // 凯撒

public:
    SHA256Container(string plain_text);
    void encrypt(void);
};

class AESContainer {
public:
    string text;
    string encrypted;          // 加密
    string encrypted_advanced; // 凯撒

public:
    AESContainer(string key, string plain_text);
    void encrypt(void);
};

class Database {
public:
    ///*static*/ const string internal_version{"1.0-zh_CN"};
    /*static*/ const string internal_version{"1.1g-zh_CN"};   // GUI独有版本号


private:
    // 路径全改成临时路径了 外边要套加密壳儿
    //string system_temp_path;
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
    string buf_string;               // 数据库buffer 用于输出

    void parseFromChar(void);            // 从文件输入

    streampos database_size_except_header;
    streampos database_size;

    //Password password;   非命令行程序不用这种方式 这个在我命令行版本里用到


public:
    Database();
    ~Database();

    inline /*static*/ const string version() {
        return internal_version;
    }

    /*
    *           文件头 版本号 主密码   (s分割 =文件尾)
    *           数据 ["user", "value", "details"]
    *
    *           读的时候都读到dBuf_str 然后用parseFromChar()得到buf_json
    *           存的时候从buf_json开始封装存到database.dat
    *
    */

    // 从GUI界面输入密码  返回 密码正确错误
    bool initDatabase(const string& password);  

    ErrorLevel CreateDatabase(const string& main_password);

    ErrorLevel save(void); // 保存dBuf流 

    void add(const string& user, const string& value, const string& details);

    // 从user判断存在
    bool exists(const string& user);

    // 从user找value details
    bool get(/*in*/const string& user, /*out*/ string& ret_value, /*out*/ string& ret_details);

    // 遍历user列表
    vector<string> travel(void);

    // 从user删除数据
    ErrorLevel erase(const string& user);

    // 数据库有效判定 (GUI版本独有)
    inline bool isValid(void) {
        return fDatabaseValid;
    }


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