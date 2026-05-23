#pragma once

#include <libfx/fxCrypto/fxCryptoUtils.hpp>
#include <libfx/fxCore/fxLogging.hpp>
#include <cstring>

#ifndef FX_DISABLE_CRYPTO

//! WARNING: XTEA fixes the equivalent key issue of TEA but retains the 64-bit block size
//! making it vulnerable to sweet32 attacks. Vulnerable to related-key attacks.

class fxCipherXTEA {
    symmetric_CBC cbc_{};
    bool initialized_ = false;
public:
    fxCipherXTEA() = default;

    fxCipherXTEA(const std::uint8_t* key, std::size_t keylen) {
        this->init(key, keylen);
    }

    fxCipherXTEA(std::vector<std::uint8_t> key) {
        this->init(key.data(), key.size());
    }

    ~fxCipherXTEA() {
        if (this->initialized_) {
            cbc_done(&this->cbc_);
        }
    }

    fxCipherXTEA(const fxCipherXTEA&) = delete;
    fxCipherXTEA& operator=(const fxCipherXTEA&) = delete;

    bool init(const std::uint8_t* key, const std::size_t keylen) noexcept {
        if (keylen != 16) {
            fxPrintf(fxLogError, "XTEA setup failed! keylen %zu is not 16 (128-bit)", keylen);
            return false;
        }

        if (this->initialized_) {
            cbc_done(&this->cbc_);
            this->initialized_ = false;
        }

        std::uint8_t dummy_iv[8] = {0};

        auto errc = cbc_start(find_cipher("xtea"), dummy_iv, key, keylen, 32, &this->cbc_); // 32 = 64 Feistel cycles for XTEA
        TOMCRYPT_ASSERT(errc, "XTEA-CBC start failed!", {})

        this->initialized_ = true;
        return true;
    }

    [[nodiscard]] std::vector<std::uint8_t> encrypt(const std::uint8_t* iv, const std::uint8_t* plaintext, const std::size_t pt_len) noexcept {
        if (!this->initialized_ || !plaintext || !iv || pt_len % 8 != 0) {
			fxPrintf(fxLogError, "XTEA-CBC encrypt failed! requirements not met");
			return {};
		}

        std::memcpy(this->cbc_.IV, iv, 8);
        std::vector<std::uint8_t> ciphertext(pt_len);

        auto errc = cbc_encrypt(plaintext, ciphertext.data(), pt_len, &this->cbc_);
        TOMCRYPT_ASSERT(errc, "XTEA-CBC encrypt failed!", {})
        return ciphertext;
    }

    [[nodiscard]] std::vector<std::uint8_t> decrypt(const std::uint8_t* iv, const std::uint8_t* ciphertext, const std::size_t ct_len) noexcept {
        if (!this->initialized_ || !ciphertext || !iv || ct_len % 8 != 0) {
			fxPrintf(fxLogError, "XTEA-CBC decrypt failed! requirements not met");
			return {};
		}

        std::memcpy(this->cbc_.IV, iv, 8);
        std::vector<std::uint8_t> plaintext(ct_len);

        auto errc = cbc_decrypt(ciphertext, plaintext.data(), ct_len, &this->cbc_);
        TOMCRYPT_ASSERT(errc, "XTEA-CBC decrypt failed!", {})
        return plaintext;
    }
};

#endif