#pragma comment(lib, "cryptlib.lib")
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <conio.h>
#include "ColorfulStr.h"
#include "ExceptionErr.h"
#include "SeqList.h"

#include "cryptopp/cryptlib.h"
#include "cryptopp/aes.h"
#include "cryptopp/modes.h"
#include "cryptopp/filters.h"
#include "cryptopp/hex.h"
#include "cryptopp/sha.h"

#define FILE_END ios::end
#define FILE_START ios::beg
#define MAX_SHA256_LEN 0x40
#define NOISE_VALUE 72 //0x7FFF 干扰不用太大
#define DATABASE_HEADER "C92CBD0AD96BC9C601C326708F1E7AC027F706585E8D0E025F7ECB9E96B33E7D" // [Forever Password Manager]
#define DATABASE_VERSION_KEY "q84Jn+zJs2E7wpGhwNBGfy3LxTZW2l3M" // 随机生成的
#define SHA256_SIZE (0x41 * sizeof(char))
#define AES_SIZE (0x21 * sizeof(char))
#define DEBUG cout

using CryBytePtr = CryptoPP::byte*;
using namespace CryptoPP;
using namespace std;
//using namespace filesystem;


namespace Encrypt {
    static string sha256(const string& text) {
        string digest;

        SHA256 hash;

        StringSource(text, true,
            new HashFilter(hash,
                new HexEncoder(
                    new StringSink(digest)
                )));

        return digest;
    }

    static string aes(const string& key, const string& plain_text) {
        string ciphertext;

        ECB_Mode<AES>::Encryption encryptor((const CryBytePtr)key.c_str(), key.size());
        StringSource(plain_text, true,
            new StreamTransformationFilter(encryptor,
                new HexEncoder(
                    new StringSink(ciphertext)
                )
            )
        );

        return ciphertext;
    }

    static string advanced_noise(const string& message, uint32_t noise) {      // 凯撒法
        char* buffer = new char[message.length() + 1];
        for (int i = 0; i < message.length(); i++) {
            buffer[i] = message.at(i) + noise;
        }
        buffer[message.length()] = '\0';
        string encrypted(buffer);
        delete[] buffer;
        return encrypted;
    }

};

namespace Decrypt {
    static string aes(const string& key, const string& cipher_text) {
        string decryptedtext;

        ECB_Mode<AES>::Decryption decryptor((const CryBytePtr)key.c_str(), key.size());
        StringSource(cipher_text, true,
            new HexDecoder(
                new StreamTransformationFilter(decryptor,
                    new StringSink(decryptedtext)
                )
            )
        );

        return decryptedtext;
    }

    static string advanced_noise(const string& message, uint32_t noise) {      // 凯撒法
        char* buffer = new char[message.length() + 1];
        for (int i = 0; i < message.length(); i++) {
            buffer[i] = (message.at(i) - noise);
        }
        buffer[message.length()] = '\0';
        string encrypted(buffer);
        delete[] buffer;
        return encrypted;
    }
}

class SHA256Container {
public:
    string text;
    string encrypted;          // 加密
    string encrypted_advanced; // 凯撒

public:
    SHA256Container()
    {

    }

    void encrypt() {
        encrypted = Encrypt::sha256(text);
        encrypted_advanced = Encrypt::advanced_noise(encrypted, NOISE_VALUE);
    }

    SHA256Container(string plain_text)
    {
        text = plain_text;
        encrypted = Encrypt::sha256(plain_text);
        encrypted_advanced = Encrypt::advanced_noise(encrypted, NOISE_VALUE);
    }
};

class AESContainer {
public:
    string text;
    string encrypted;          // 加密
    string encrypted_advanced; // 凯撒

public:
    AESContainer()
    {

    }

    void encrypt() {
        encrypted = Encrypt::sha256(text);
        encrypted_advanced = Encrypt::advanced_noise(encrypted, NOISE_VALUE);
    }
    AESContainer(string key, string plain_text)
    {
        text = plain_text;
        encrypted = Encrypt::aes(key, plain_text);
        encrypted_advanced = Encrypt::advanced_noise(encrypted, NOISE_VALUE);
    }
};

typedef struct Password {  // 从我另一个项目移植来的
    char password_string[100];

    int GetPassword() {
        int index = 0;
        while (1) {
            char ch;
            ch = _getch();
            if (ch == 8) {  // 退格
                if (index != 0) {
                    putchar('\b');
                    putchar(' ');
                    putchar('\b');
                    index--;
                }
            }
            else if (ch == '\r') {  // 回车
                password_string[index] = '\0';
                putchar('\n');
                break;
            }
            else {
                putchar('*');
                password_string[index++] = ch;
            }
        }
        return 0;
    }
} Password;

class Database {  
public:
    const string internal_version = "1.0.0.0";

private:
    const string database_path = "database.dat";
    ifstream database;
    bool fDatabaseValid = false;  // 数据库可用flag
    char* dBuf_str = nullptr;  // new char[0x7FFF];        // 数据库buffer 用于输入          ******* 释放内存 *******
    //struct dBuf {
        string buf_string;                       // 数据库buffer 用于输出
        SeqList<string> user;  // 用户名
        SeqList<string> value;  // 密码值
        SeqList<string> details;  // 备注其他

        void parseFromChar() {  // 输入
            DEBUG << dBuf_str << endl;
            buf_string = string(dBuf_str);
            DEBUG << buf_string << endl;
            
            vector<string> parse_buffer = ColorfulStr::StringSplit(buf_string, "\n");   // 整体段分割
            //vector<string> parse_user = ColorfulStr::StringSplit(parse_buffer[0], "s");     // 数据分割
            //vector<string> parse_value = ColorfulStr::StringSplit(parse_buffer[1], "s");    // 数据分割
            //vector<string> parse_details = ColorfulStr::StringSplit(parse_buffer[2], "s");  // 数据分割
            
            //for (size_t i = 0; i < parse_user.size(); i++)
            //    user.push(parse_user[i]);
            /*for (size_t i = 0; i < parse_value.size(); i++)
                value.push(parse_value[i]);
            for (size_t i = 0; i < parse_details.size(); i++)
                details.push(parse_details[i]);*/
            buf_string = string(dBuf_str);

            //*
            for (const auto& i : parse_buffer)
                cout << i << "  ";
            /*for (size_t i = 0; i < parse_user.size(); i++)
                cout << user.at(i) << "  ";
            for (size_t i = 0; i < parse_value.size(); i++)
                cout << value.at(i) << "  ";
            for (size_t i = 0; i < parse_details.size(); i++)
                cout << details.at(i) << "  ";*/
                
        } 
        void saveToBuffer() {   // 输出
            for (size_t i = 0; i < user.size(); i++)
                buf_string = user.at(i);
            for (size_t i = 0; i < value.size(); i++)
                buf_string += value.at(i);
            for (size_t i = 0; i < details.size(); i++)
                buf_string += details.at(i);
        }
    //} dBuf;
    streampos database_size_except_header;
    streampos database_size;
    Password password;
    

public:
    Database() {
        database = ifstream("database.dat", ios::binary);
        initDatabase();
        if (fDatabaseValid) {
            parseFromChar();
            ColorfulStr::prints("数据库已载入！", RESET);
            
            ColorfulStr::prints("版本", RESET);
            

            ColorfulStr::prints("请输入密码 >>", GREY);
            password.GetPassword();
            cout << password.password_string;
            
        }
    }

    ~Database() {
        database.close();
        delete dBuf_str;
    }

    void initDatabase() {
        if (!database) {  // 创建新数据库 
            /*
            *  文件头 版本号 主密码
            *  数据
            * 
            */

            Exc::DatabaseNotFound();  // 提示错误信息

            ofstream new_database("database.dat", ios::binary);

            // 头
            SHA256Container title_header("[Forever Password Manager]"); 
            new_database.write(title_header.encrypted_advanced.c_str(), SHA256_SIZE);  // 64
            // 凯撒一下 防止有些老油条拿记事本儿打开就能看出来加密方式
            
            // 版本号
            AESContainer version(DATABASE_VERSION_KEY, internal_version);
            new_database.write(version.encrypted_advanced.c_str(), AES_SIZE);  // 32

            // 建密码
            Password new_password, new_password_again;
            cout << "请设置密码 >>";
            new_password.GetPassword();
            cout << "请重新输入 >>";
            new_password_again.GetPassword();

            while (!STRCMP(new_password.password_string, new_password_again.password_string)) {
                Exc::PasswordNotAccepted();
                cout << "请设置密码 >>";
                new_password.GetPassword();
                cout << "请重新输入 >>";
                new_password_again.GetPassword();
            }

            STRCPY(password.password_string, new_password.password_string);
            SHA256Container pwd_pass(password.password_string);
            new_database.write(pwd_pass.encrypted_advanced.c_str(), SHA256_SIZE);  // 64

            // 测试
            //new_database.write("\n121212swd54631\ntesta", 22);
            //cout << title_header.encrypted_advanced << "    " << endl;
            new_database.close();

            database = ifstream("database.dat", ios::binary);
            ColorfulStr::prints("已自动创建数据库", RED);
        }

        database.seekg(0, FILE_START);
        char* buffer = new char[70];
        if (database.read(buffer, sizeof(char) * 65))
            if (Decrypt::advanced_noise(buffer, NOISE_VALUE) == DATABASE_HEADER)
                fDatabaseValid = true;
        else 
            Err::DatabaseInvalid();   // 提示错误信息

        //cout << database.tellg() << endl;

        database.seekg(0, FILE_END);
        database_size_except_header = database.tellg();
        database.seekg(sizeof(char) * 64, FILE_START);

        database_size = database_size_except_header;
        database_size_except_header -= 65;

        this->dBuf_str = new char[0x7fff];
        database.read(this->dBuf_str, database_size_except_header);
        DEBUG << this->dBuf_str << database.gcount() << endl;

        DEBUG << database_size << " " << database_size_except_header;

        delete[] buffer;
    }

    //void add(const string& act, const string& pwd, const string& dtl) { // account password details
    //    cin << act
    //
    //    Password new_value;
    //
    //}



};

int main(const int argc, const char** argv)
{
    /*
    ifstream database("database.dat", ios::binary);
    
    if (!database) {
        ofstream new_database("database.dat", ios::binary);
        const char* data = "dedetest";
        new_database.write(data, sizeof(data));

        new_database.close();
    }

    else {
        database.seekg(0, FILE_END);
        streampos database_size = database.tellg();
        database.seekg(0, FILE_START);

        char* buffer = new char[database_size];

        database.read(buffer, database_size);

        cout << buffer << endl;

        delete[] buffer;
        
    }


    database.close();
    //int data = 123456789;
    //cout << reinterpret_cast<const char*>(&data) << endl;
    */

    if (Err::HookOnExit()) {
        Database d;
        //cout << Encrypt::aes(DATABASE_VERSION_KEY, "1.0.0.0");
    }


    return 0;

}