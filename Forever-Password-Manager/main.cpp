#pragma comment(lib, "cryptlib.lib")
#include "database.h"     // ��ͷ�ļ�

ErrorLevel main(const int argc, char** argv)
{
#ifdef AES_CBC_MODE
    for (int i = 0; i < AES::BLOCKSIZE; i++)
        iv[i] = 0;

    string strIv = DATABASE_AES_KEY;
    memcpy(iv, strIv.data(), strIv.length());
#endif

    HWND self = GetForegroundWindow();
    MainSwitch ms(argc, argv, "hva");

    if (ms << 'v')
    {
        ColorfulStr::prints("Forever��������� ", GREEN);
        ColorfulStr::prints("1.0-zh_CN\r\n", GREY);
        ColorfulStr::printEnter();
        return 0;
    }

    if (ms << 'h') 
    {
        ShowHelp();
        system("pause>nul");
        return 0;
    }

    /*  // ȡ���˹���
    if (ms << 'a')
    {
        ColorfulStr::prints("Forever��������� ", GREEN);
        ColorfulStr::prints("1.0-zh_CN\r\n", GREY);
        ColorfulStr::printEnter();
        Database d;
        ColorfulStr::prints("����³�Ա ", YELLOW);
        ColorfulStr::prints(">>", RESET);

    }
    */

    if (Err::HookOnExit())  // �ڴ���չ���
    {
        ColorfulStr::prints("Forever��������� ", GREEN);
        ColorfulStr::prints("1.0-zh_CN\r\n", GREY);
        ColorfulStr::printEnter();
        Database d;
        string cmd;
        ColorfulStr::prints("\r\nʹ�� ", GREY);
        ColorfulStr::prints(":h", LAKE_BLUE);
        ColorfulStr::prints(" ����ȡ����\r\n", GREY);
        loop{
            ColorfulStr::printEnter();
            GetCmd(cmd, d, "Database", false);
            if (cmd == ":b")
                continue;

            if (cmd.find_first_of(':') == 0)  // ��������ģʽ
            {
                // ���
                if (cmd == ":add") 
                {
                    ColorfulStr::prints("�������Ŀ\r\n", LAKE_BLUE);
                    string new_user;
                    string new_value;
                    string new_details;
                    GetCmd(new_user, d, "�û�");
                    if (new_user == ":b")
                        continue;

                    else 
                    {
                        ColorfulStr::prints("����", VIOLET);
                        ColorfulStr::prints(">>", RESET);
                        Password new_p;
                        new_p.GetPassword();
                        new_value = new_p.password_string;
                        GetCmd(new_details, d, "��ע");
                        if (new_details == ":b")
                            continue;
                    }
                    if (d.exists(new_user)) 
                    {
                        Exc::UserAlreadyExists();
                    }
                    else 
                    {
                        d.add(new_user, new_value, new_details);
                        ColorfulStr::prints("��ӳɹ���", GREEN);
                        ColorfulStr::prints("    ���ύδ����! ", RED);
                        ColorfulStr::prints("ʹ��", RESET);
                        ColorfulStr::prints(" :s ", YELLOW);
                        ColorfulStr::prints("������\r\n", RESET);
                    }
                }
                // ���user�б�
                if (cmd == ":check") 
                {
                    vector<string> ret = d.travel();
                    ColorfulStr::prints("�������� ", RESET);
                    ColorfulStr::prints(ret.size(), LAKE_BLUE);
                    ColorfulStr::prints(" ����Ŀ\r\n", RESET);
                    for (const auto& i : ret) {
                        ColorfulStr::prints(i.c_str(), LAKE_BLUE);
                        ColorfulStr::printEnter();
                    }
                }
                if (cmd == ":del")
                {
                    string del_user;
                    ColorfulStr::prints("Delete", RED);
                    GetCmd(del_user, d);
                    if (del_user == ":b")
                        continue;
                    if (d.erase(del_user))
                    {
                        ColorfulStr::prints("��ɾ�� ", RESET);
                        ColorfulStr::prints("1", LAKE_BLUE);
                        ColorfulStr::prints(" ����Ŀ\r\n", RESET);
                    }
                    else if (del_user.find_first_of(':') != 0)
                        Exc::UserNotFound();
                }
                if (cmd == ":version" || cmd == ":v") 
                {
                    cout << Database::version() << endl;
                }
                if (cmd == ":clear" || cmd == ":c" || cmd == ":cls") 
                {
                    system("cls");
                }
                if (cmd == ":save" || cmd == ":s") 
                {
                    d.save();
                    ColorfulStr::prints("�������!\r\n", GREEN);
                }
            }
            else {
                string value, details;
                if (d.get(cmd, value, details)) 
                {
                    string mb_buf = "�û�: ";
                    mb_buf += cmd;
                    mb_buf += "\n\n����: ";
                    mb_buf += value;
                    mb_buf += "\n\n��ע: ";
                    mb_buf += details;
                    MessageBoxA(self, mb_buf.c_str(), "��Ŀ��ϸ", MB_OK | MB_ICONINFORMATION);
                }
                else
                    Exc::UserNotFound();
            }
        }
    }
    return 0;
}

#ifdef AES_CBC_MODE
CryptoPP::byte iv[32];
#endif


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

#ifdef AES_CBC_MODE
// CBCģʽ
static string Encrypt::aes(const string& key, const string& plain_text)
{

    CryptoPP::byte key[AES::MAX_KEYLENGTH];
    CryptoPP::byte* k = (CryptoPP::byte*)key.c_str();

    for (int i = 0; i < AES::MAX_KEYLENGTH; i++)
        if (i < sizeof(k))
            key[i] = k[i];
        else
            key[i] = 0;
    string ciphertextEncode, ciphertext;
    cout << "\nplain_text text :" << plain_text;
    cout << "\n key to encrypt: " << key;
    ciphertextEncode.clear();
    ciphertext.clear();
    CryptoPP::AES::Encryption aesEncryption(key, AES::MAX_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, iv);

    CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(ciphertext));
    stfEncryptor.Put(reinterpret_cast<const unsigned char*> (plain_text.c_str()), plain_text.length() + 1);
    stfEncryptor.MessageEnd();
    cout << "\nencrypted text: " << ciphertext;
    StringSource ss(ciphertext, true, new HexEncoder(new StringSink(ciphertextEncode)));
    cout << "\nencoded encrypted text: " << ciphertextEncode;
    return ciphertextEncode;
}
#endif

#ifdef AES_ECB_MODE    // EDBģʽ

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

#endif

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

#ifdef AES_CBC_MODE    // CBCģʽ
static string Decrypt::aes(const string& key, const string& cipher)
{
    CryptoPP::byte key[AES::MAX_KEYLENGTH];
    CryptoPP::byte* k = (CryptoPP::byte*)key.c_str();

    for (int i = 0; i < AES::MAX_KEYLENGTH; i++)
        if (i < sizeof(k))
            key[i] = k[i];
        else
            key[i] = 0;
    string ciphertextDecode, decryptedtext;
    cout << "\ncipher text : " << cipher;
    cout << "\n key to decrypt: " << key;
    ciphertextDecode.clear();
    decryptedtext.clear();

    StringSource ss(cipher, true, new HexDecoder(new StringSink(ciphertextDecode)));
    cout << "\n cipher decoded: " << ciphertextDecode;
    CryptoPP::AES::Decryption aesDecryption(key, AES::MAX_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv);

    CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink(decryptedtext));
    stfDecryptor.Put(reinterpret_cast<const unsigned char*> (ciphertextDecode.c_str()), ciphertextDecode.size());
    stfDecryptor.MessageEnd();

    cout << "\ndecrypted text: " << decryptedtext;
    return decryptedtext;
}
#endif

#ifdef AES_ECB_MODE    // ECBģʽ

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
#endif

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

SHA256Container::SHA256Container()   // �չ���
{
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


AESContainer::AESContainer()
{

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
    iDatabase = ifstream("database.dat", ios::binary);
    initDatabase();
    if (fDatabaseValid) 
    {
        parseFromChar();
        ColorfulStr::prints("���ݿ������룡", RESET);

        ColorfulStr::prints("�汾 ", RESET);
        ColorfulStr::prints(database_version.c_str(), GREY);
        ColorfulStr::printEnter();

        buf_json = parseFromJson(Decrypt::aes(DATABASE_AES_KEY, buf_string));
    }
}

Database::~Database() 
{
    iDatabase.close();
    oDatabase.close();
    delete dBuf_str;
}


void Database::initDatabase(void) 
{
    if (!iDatabase)   // ���������ݿ� 
        CreateDatabase();

    iDatabase.seekg(0, FILE_START);
    char* buffer = new char[70];
    if (iDatabase.read(buffer, SHA256_SIZE)) 
    {
        buffer[SHA256_SIZE] = '\0';   // ���ַ������ֽ�ĩβ�������\0 �ֶ���
        /***����Ҫ��*********************************************************************************/
#ifdef DEBUG
        if (STRCMP(buffer, DATABASE_HEADER))
        {
#else 
        if (Decrypt::advanced_noise(buffer, NOISE_VALUE) == DATABASE_HEADER) 
        {
#endif

            fDatabaseValid = true;
        }
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
        // DEBUG << buffer;
        database_version = Decrypt::aes(DATABASE_VERSION_KEY, buffer);
        //database_version = Decrypt::aes(DATABASE_AES_KEY, buffer);
    }
    else
        Err::DatabaseVersionReadInvalid();    // ���ݿ�汾����

    /* ������ */
    delete[] buffer;
    buffer = new char[70];
    iDatabase.seekg(SHA256_SIZE/*�ļ�ͷ*/ + AES_SIZE/*�汾��*/ + 2/*�ָ���*/, FILE_START);   // ��λָ�뵽 98

    if (iDatabase.read(buffer, SHA256_SIZE))
    {
        buffer[SHA256_SIZE] = '\0';   // ���ֽ�ĩβ�������\0 �ֶ���
        ColorfulStr::prints("���������ݿ����� >>", VIOLET);
        Password varify_password;
        loop{
            varify_password.GetPassword();
            if (STRCMP(Encrypt::sha256(varify_password.password_string).c_str(), buffer)) 
            {
                ColorfulStr::prints("����ƥ��!\r\n", GREEN);
                break;
            }
            else {
                Exc::PasswordWrong();
                //DEBUG << buffer << endl;
                //DEBUG << varify_password.password_string << endl;
                ColorfulStr::prints("\r\n�������������ݿ����� >>", VIOLET);
            }
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
}

ErrorLevel Database::CreateDatabase(void) 
{
    /*
        *  �ļ�ͷ �汾�� ������   (s�ָ� =�ļ�β)
        *  ���� ["user", "value", "details"]
        *
        *  ����ʱ�򶼶���dBuf_str Ȼ����parseFromChar()�õ�buf_json
        *  ���ʱ���buf_json��ʼ��װ�浽database.dat
        *
        */

    Exc::DatabaseNotFound();  // ��ʾ������Ϣ

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
    new_database.write("s", 1);  // �ָ���

    // ������
    Password new_password, new_password_again;
    cout << "���������� >>";
    new_password.GetPassword();
    cout << "���������� >>";
    new_password_again.GetPassword();

    while (!STRCMP(new_password.password_string, new_password_again.password_string))
    {
        Exc::PasswordNotAccepted();
        cout << "���������� >>";
        new_password.GetPassword();
        cout << "���������� >>";
        new_password_again.GetPassword();
    }

    STRCPY(password.password_string, new_password.password_string);
    SHA256Container pwd_pass(password.password_string);
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
    ColorfulStr::prints("���Զ��������ݿ�\r\n", RED);
    return Ok;
}

ErrorLevel Database::save(void) 
{   // ����dBuf�� 
    if (iDatabase)
        iDatabase.close();
    oDatabase = ofstream("database.dat", ios::binary);

    if (!oDatabase)
        return Error;

    string sav_buf = full_header;
    sav_buf += Encrypt::aes(DATABASE_AES_KEY, (string)(Json::FastWriter().write(buf_json)));
    //DEBUG 
    //sav_buf += Encrypt::aes(DATABASE_AES_KEY, "{\"DatabaseInfo\":{\"details\":\" \",\"user\":\"ForeverPasswordManager\",\"value\":\"1.0.0.0\"},\"MetaInfo\":[{\"details\":\"nodetails\",\"user\":\"Forever.test1\",\"value\":\"123456\"},{\"details\":\"nodetails\",\"user\":\"Forever.test2\",\"value\":\"789abc\"}]}");        DEBUG << sav_buf << endl;

    oDatabase << sav_buf << '=';
    oDatabase.close();
    iDatabase = ifstream("database.dat", ios::binary);
    if (!iDatabase)
        return Error;
    return Ok;
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
                // ��!
                buffer.removeIndex(i, &item);
                return Ok;
            }
        }
    }
    return Error;
}