#pragma once
#pragma comment(lib, "cryptlib.lib")
#include "Forever-Password-Manager-GUI.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

// �Լ�д�Ŀ�
#include "ExceptionErr.h"
#include "database.h"
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
    static std::string advanced_noise(const std::string& message, uint32_t noise);     // ������
}

namespace Decrypt {
    static std::string aes(const std::string& key, const std::string& cipher_text);
    static std::string advanced_noise(const std::string& message, uint32_t noise);     // ������
}

class SHA256Container {
public:
    string text;
    string encrypted;          // ����
    string encrypted_advanced; // ����

public:
    SHA256Container(string plain_text);
    void encrypt(void);
};

class AESContainer {
public:
    string text;
    string encrypted;          // ����
    string encrypted_advanced; // ����

public:
    AESContainer(string key, string plain_text);
    void encrypt(void);
};

class Database {
public:
    ///*static*/ const string internal_version{"1.0-zh_CN"};
    /*static*/ const string internal_version{"1.1g-zh_CN"};   // GUI���а汾��


private:
    // ·��ȫ�ĳ���ʱ·���� ���Ҫ�׼��ܿǶ�
    //string system_temp_path;
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
    string buf_string;               // ���ݿ�buffer �������

    void parseFromChar(void);            // ���ļ�����

    streampos database_size_except_header;
    streampos database_size;

    //Password password;   �������г��������ַ�ʽ ������������а汾���õ�


public:
    Database();
    ~Database();

    inline /*static*/ const string version() {
        return internal_version;
    }

    /*
    *           �ļ�ͷ �汾�� ������   (s�ָ� =�ļ�β)
    *           ���� ["user", "value", "details"]
    *
    *           ����ʱ�򶼶���dBuf_str Ȼ����parseFromChar()�õ�buf_json
    *           ���ʱ���buf_json��ʼ��װ�浽database.dat
    *
    */

    // ��GUI������������  ���� ������ȷ����
    bool initDatabase(const string& password);  

    ErrorLevel CreateDatabase(const string& main_password);

    ErrorLevel save(void); // ����dBuf�� 

    void add(const string& user, const string& value, const string& details);

    // ��user�жϴ���
    bool exists(const string& user);

    // ��user��value details
    bool get(/*in*/const string& user, /*out*/ string& ret_value, /*out*/ string& ret_details);

    // ����user�б�
    vector<string> travel(void);

    // ��userɾ������
    ErrorLevel erase(const string& user);

    // ���ݿ���Ч�ж� (GUI�汾����)
    inline bool isValid(void) {
        return fDatabaseValid;
    }


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