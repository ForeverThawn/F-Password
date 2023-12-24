#include "database.h"

extern HWND hWnd_main;                           // �����򴰿�
extern HWND hWnd_subWindow_login;                // ���������Ӵ���


static string Encrypt::sha256(const string& text)
{
    string digest;

    SHA256 hash;

    StringSource(text, true,
        new HashFilter(hash,
            new HexEncoder(
                new StringSink(digest)
            )));

    return digest;
}

static string Encrypt::aes(const string& key, const string& plain_text)
{
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


static string Encrypt::advanced_noise(const string& message, uint32_t noise)
{      // ������
    char* buffer = new char[message.length() + 1];
    for (int i = 0; i < message.length(); i++)
    {
        buffer[i] = message.at(i) + noise;
    }
    buffer[message.length()] = '\0';
    string encrypted(buffer);
    delete[] buffer;
    return encrypted;
}

static string Decrypt::aes(const string& key, const string& cipher_text)
{
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

static string Decrypt::advanced_noise(const string& message, uint32_t noise)       // ������
{
    char* buffer = new char[message.length() + 1];
    for (int i = 0; i < message.length(); i++)
    {
        buffer[i] = (message.at(i) - noise);
    }
    buffer[message.length()] = '\0';
    string encrypted(buffer);
    delete[] buffer;
    return encrypted;
}

void SHA256Container::encrypt()
{
    encrypted = Encrypt::sha256(text);
    encrypted_advanced = Encrypt::advanced_noise(encrypted, NOISE_VALUE);
}

SHA256Container::SHA256Container(string plain_text)
{
    text = plain_text;
    encrypted = Encrypt::sha256(plain_text);
    encrypted_advanced = Encrypt::advanced_noise(encrypted, NOISE_VALUE);
}

void AESContainer::encrypt()
{
    encrypted = Encrypt::sha256(text);
    encrypted_advanced = Encrypt::advanced_noise(encrypted, NOISE_VALUE);
}

AESContainer::AESContainer(string key, string plain_text)
{
    text = plain_text;
    encrypted = Encrypt::aes(key, plain_text);
    encrypted_advanced = Encrypt::advanced_noise(encrypted, NOISE_VALUE);
}


Database::Database()
{
    //char tempPath[MAX_PATH] { 0 };
    //GetTempPath(MAX_PATH, tempPath);
    //system_temp_path = tempPath;
    //SetCurrentDirectory(tempPath);
    //iDatabase = ifstream(database_path, ios::binary);
}

Database::~Database()
{
    iDatabase.close();
    oDatabase.close();
    delete dBuf_str;
}


bool Database::initDatabase(const string& password)
{
    iDatabase = ifstream(database_path, ios::binary);
    if (!iDatabase) {  // ���������ݿ�   Ŀǰ��ʱ�޷����������ݿ�
        /*
        *  �ļ�ͷ �汾�� ������   (s�ָ� =�ļ�β)
        *  ���� ["user", "value", "details"]
        *
        *  ����ʱ�򶼶���dBuf_str Ȼ����parseFromChar()�õ�buf_json
        *  ���ʱ���buf_json��ʼ��װ�浽database.dat
        *
        */
        // CreateDatabase();
    }

    iDatabase.seekg(0, FILE_START);
    char* buffer = new char[70];
    if (iDatabase.read(buffer, SHA256_SIZE))
    {
        buffer[SHA256_SIZE] = '\0';   // ���ַ������ֽ�ĩβ�������\0 �ֶ���
        /***����Ҫ��*********************************************************************************/
        if (STRCMP(buffer, DATABASE_HEADER))
            fDatabaseValid = true;
        else
            Err::DatabaseInvalid();   // ���ݿⲻ����
    }
    else
        Err::DatabaseInvalid();   // ���ݿⲻ����

    //cout << database.tellg() << endl;

    /* �汾�� */
    delete[] buffer;
    buffer = new char[35];
    if (!(iDatabase.seekg(SHA256_SIZE + 1, FILE_START)))   // ��λָ�뵽 65
        Err::DatabaseError();    // ���ݿ����

    if (iDatabase.read(buffer, AES_SIZE))
    {
        buffer[AES_SIZE] = '\0';   // ���ַ������ֽ�ĩβ�������\0 �ֶ���
        database_version = Decrypt::aes(DATABASE_VERSION_KEY, buffer);
        //DEBUGW(buffer);
        // database_version = Decrypt::aes(DATABASE_AES_KEY, buffer);
    }
    else
        Err::DatabaseVersionReadInvalid();    // ���ݿ�汾����


    /* im not able to understand how the last 16 bytes are generated
     * 
     * dont trow InvalidCiphertext error
     * ˭����ĵ����ɵ�ƶ����� ������˭д���������
     * plz dont throw any error
     * plz dont throw any error
     * plz dont throw any error
     * plz dont throw any error
     * plz dont throw any error
     * plz dont throw any error
     * plz dont throw any error
     * plz dont throw any error
     *
     * �����ɵ�Ʊ�thwrow��
     *
     */

    /* ������ */
    delete[] buffer;
    buffer = new char[70];
    iDatabase.seekg(SHA256_SIZE/*�ļ�ͷ*/ + AES_SIZE/*�汾��*/ + 2/*�ָ���*/, FILE_START);   // ��λָ�뵽 98

    if (iDatabase.read(buffer, SHA256_SIZE))
    {
        buffer[SHA256_SIZE] = '\0';   // ���ֽ�ĩβ�������\0 �ֶ���
        //ColorfulStr::prints("���������ݿ����� >>", VIOLET);
        //Password varify_password;
        //varify_password.GetPassword();
        if (!STRCMP(Encrypt::sha256(password).c_str(), buffer))
        {
            //Exc::PasswordWrong();
            //ColorfulStr::prints("\r\n�������������ݿ����� >>", YELLOW);
            delete[] buffer; // �����buffer
            return false;    // ��һ��Ҫcallback��messagebox
        }
    }
    else
        Err::DatabasePasswordReadInvalid();    // ���ݿ����

    // ��������ļ�ͷ
    char full_header[FULL_HEADER_LENGTH];   // �����ļ�ͷ ���� FULL_HEADER_LENGTH
    iDatabase.seekg(0, FILE_START);
    iDatabase.read(full_header, FULL_HEADER_LENGTH);
    this->full_header = string(full_header);

    iDatabase.seekg(0, FILE_END);
    database_size_except_header = iDatabase.tellg();
    iDatabase.seekg(FULL_HEADER_LENGTH, FILE_START);
    database_size_except_header -= (SHA256_SIZE * 2 + AES_SIZE + 3);

    dBuf_str = new char[0x7fff];
    iDatabase.read(dBuf_str, database_size_except_header);

    iDatabase.close();

    delete[] buffer;

    // �ӹ������ó�������Ūһ������ɹ�messagebox
    if (fDatabaseValid)
    {
        parseFromChar();
        string print{ "���ݿ������룡\n�汾" };
        print += database_version;

        MessageBox(hWnd_main, print.c_str(), "�ɹ�", MB_OK);

        buf_json = parseFromJson(Decrypt::aes(DATABASE_AES_KEY, buf_string));
    }
}

ErrorLevel Database::CreateDatabase(const string& main_password)
{
    ofstream new_database("database.dat", ios::binary);

    // ͷ
    SHA256Container title_header("[Forever Password Manager]");
    /***����Ҫ��*********************************************************************************/
    new_database.write(title_header.encrypted.c_str(), SHA256_SIZE);  // 64
    new_database.write("s", 1);  // �ָ���
    // ����һ�� ��ֹ��Щ�������ü��±����򿪾��ܿ��������ܷ�ʽ

    // �汾��
    AESContainer version(DATABASE_VERSION_KEY, internal_version);
    /***����Ҫ��*********************************************************************************/
    new_database.write(version.encrypted.c_str(), AES_SIZE);  // 32
    //new_database.write(internal_version.c_str(), AES_SIZE);  // 32
    new_database.write("s", 1);  // �ָ���

    // ������
    SHA256Container pwd_pass(main_password);
    /***����Ҫ��*********************************************************************************/
    new_database.write(pwd_pass.encrypted.c_str(), SHA256_SIZE);  // 64
    new_database.write("s", 1);  // �ָ���

    new_database.write(Encrypt::aes(DATABASE_AES_KEY, INIT_DATABASE_JSON).c_str(),
        Encrypt::aes(DATABASE_AES_KEY, INIT_DATABASE_JSON).length());

    new_database.write("=", 1);  // �ļ�β

    // ����
    //new_database.write("\n121212swd54631\ntesta", 22);
    //cout << title_header.encrypted_advanced << "    " << endl;
    new_database.close();

    iDatabase = ifstream("database.dat", ios::binary);
    return Ok;
}

void Database::parseFromChar(void) {  // ���ļ�����
    buf_string = string(dBuf_str);
    size_t end_of_string_index = buf_string.rfind('=');
    buf_string.erase(end_of_string_index);
}

ErrorLevel Database::save(void)
{   // ����dBuf�� 
    if (iDatabase)
        iDatabase.close();
    oDatabase = ofstream(database_path, ios::binary);

    string sav_buf = full_header;
    sav_buf += Encrypt::aes(DATABASE_AES_KEY, (string)(Json::FastWriter().write(buf_json)));
    

    oDatabase << sav_buf << '=';
    oDatabase.close();
    iDatabase = ifstream(database_path, ios::binary);
}

void Database::add(const string& user, const string& value, const string& details)
{
    Json::Value new_json;
    new_json["user"] = user;
    new_json["value"] = value;
    new_json["details"] = details;

    buf_json["MetaInfo"].append(Json::Value(new_json));
}

// ��user�жϴ���
bool Database::exists(const string& user)
{
    string user_with_prefix_and_suffix = user;
    user_with_prefix_and_suffix.insert(0, "\"");
    user_with_prefix_and_suffix += "\"\n";

    for (const auto& i : buf_json["MetaInfo"])
        if (i["user"].toStyledString() == user_with_prefix_and_suffix)
            return true;

    return false;
}

// ��user��value details
bool Database::get(/*in*/const string& user, /*out*/ string& ret_value, /*out*/ string& ret_details)
{
    string user_with_prefix_and_suffix = user;
    user_with_prefix_and_suffix.insert(0, "\"");
    user_with_prefix_and_suffix += "\"\n";

    for (const auto& i : buf_json["MetaInfo"])
    {
        if (i["user"].toStyledString() == user_with_prefix_and_suffix)
        {
            ret_value = (string)Json::FastWriter().write(i["value"]);

            // �ٵ�ͷβjson������
            ret_value.erase(ret_value.length() - 2, 2);
            ret_value.erase(0, 1);
            ret_details = (string)Json::FastWriter().write(i["details"]);

            ret_details.erase(ret_details.length() - 2, 2);
            ret_details.erase(0, 1);
            return true;
        }
        //#endif
    }
    //#ifdef DEBUG
    //        Exc::UserNotFound();
    //#endif
    return false;
    //return buf_json["MetaInfo"][user].toStyledString();
}

// ����user�б�
vector<string> Database::travel() {
    Json::Value travel_buffer = buf_json.get("MetaInfo", "Error");
    vector<string> ret_buffer;
    size_t travel_buffer_length = travel_buffer.size();
    for (const auto& i : travel_buffer) {
        string output_formatted = i.get("user", "Error").toStyledString();
        // ��ͷȥβ
        output_formatted.erase(output_formatted.length() - 2, 2);
        output_formatted.erase(0, 1);
        ret_buffer.push_back(output_formatted);
    }
    return ret_buffer;
}

// ɾ��Ԫ��
ErrorLevel Database::erase(const string& user)
{
    Json::Value& buffer = buf_json["MetaInfo"];
    if (buffer.isArray())
    {
        for (uint32_t i = 0; i < buffer.size(); i++)
        {
            Json::Value& item = buffer[i];
            if (item.isMember("user") && item["user"].isString() && item["user"].asString() == user) {
                buffer.removeIndex(i, &item);
                return Ok;
            }
        }
    }
    return Error;
}