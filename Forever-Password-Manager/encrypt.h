#pragma once
#include <iostream>

#ifdef AES_CBC_MODE
CryptoPP::byte iv[32];
#endif

namespace Encrypt {
    static std::string sha256(const std::string& text);

#ifdef AES_CBC_MODE // CBCģʽ
    static std::string aes(const std::string& key, const std::string& plain_text);
#endif

#ifdef AES_ECB_MODE // EDBģʽ
    static std::string aes(const std::string& key, const std::string& plain_text);
#endif

    static std::string advanced_noise(const std::string& message, uint32_t noise);     // ������
}