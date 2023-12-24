#pragma once
#include <iostream>

namespace Decrypt {
#ifdef AES_CBC_MODE
    static std::string aes(const std::string& key, const std::string& cipher);
#endif

#ifdef AES_ECB_MODE // ECBÄ£Ê½
    static std::string aes(const std::string& key, const std::string& cipher_text);
#endif
    static std::string advanced_noise(const std::string& message, uint32_t noise);     // ¿­Èö·¨
}