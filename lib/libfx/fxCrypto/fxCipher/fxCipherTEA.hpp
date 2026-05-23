#pragma once

#include <libfx/fxCrypto/fxCryptoUtils.hpp>
#include <libfx/fxCore/fxLogging.hpp>
#include <cstring>

#ifndef FX_DISABLE_CRYPTO

// WARNING: TEA (Tiny Encryption Algorithm) has a 64-bit block size making it
// vulnerable to sweet32 attacks. It also suffers from equivalent keys.
class fxCipherTEA {
    symmetric_CBC cbc_{};
    bool initialized_ = false;
public:
    fxCipherTEA() = default;

    fxCipherTEA(const std::uint8_t* key, std::size_t keylen) {
        this->init(key, keylen);
    }

    fxCipherTEA(std::vector<std::uint8_t> key) {
        this->init(key.data(), key.size());
    }

    ~fxCipherTEA() {
        if (this->initialized_) {
            cbc_done(&this->cbc_);
        }
    }

    fxCipherTEA(const fxCipherTEA&) = delete;
    fxCipherTEA& operator=(const fxCipherTEA&) = delete;

    bool init(const std::uint8_t* key, const std::size_t keylen) noexcept {
        if (keylen != 16) {
            fxPrintf(fxLogError, "TEA setup failed! keylen %zu is not 16 (128-bit)", keylen);
            return false;
        }

        if (this->initialized_) {
            cbc_done(&this->cbc_);
            this->initialized_ = false;
        }

        std::uint8_t dummy_iv[8] = {0};

        // 32 is the standard number of rounds for TEA (64 Feistel cycles)
        auto errc = cbc_start(find_cipher("tea"), dummy_iv, key, keylen, 32, &this->cbc_);
        FX_TOMCRYPT_ASSERT(errc, "TEA-CBC start failed!", {})

        this->initialized_ = true;
        return true;
    }

	bool init(std::vector<std::uint8_t> key) noexcept {
		return this->init(key.data(), key.size());
	}

    [[nodiscard]] std::vector<std::uint8_t> encrypt(const std::uint8_t* iv, const std::uint8_t* plaintext, const std::size_t pt_len) noexcept {
        if (!this->initialized_ || !plaintext || !iv || pt_len % 8 != 0) {
			fxPrintf(fxLogError, "TEA-CBC encrypt failed! requirements not met");
			return {};
		}

        std::memcpy(this->cbc_.IV, iv, 8);
        std::vector<std::uint8_t> ciphertext(pt_len);

        auto errc = cbc_encrypt(plaintext, ciphertext.data(), pt_len, &this->cbc_);
        FX_TOMCRYPT_ASSERT(errc, "TEA-CBC encrypt failed!", {})
        return ciphertext;
    }

	[[nodiscard]] std::vector<std::uint8_t> encrypt(const std::vector<std::uint8_t>& iv, const std::vector<std::uint8_t>& plaintext) noexcept {
		return this->encrypt(iv.data(), plaintext.data(), plaintext.size());
	}

    [[nodiscard]] std::vector<std::uint8_t> decrypt(const std::uint8_t* iv, const std::uint8_t* ciphertext, const std::size_t ct_len) noexcept {
        if (!this->initialized_ || !ciphertext || !iv || ct_len % 8 != 0) {
			fxPrintf(fxLogError, "TEA-CBC decrypt failed! requirements not met");
			return {};
		}

        std::memcpy(this->cbc_.IV, iv, 8);
        std::vector<std::uint8_t> plaintext(ct_len);

        auto errc = cbc_decrypt(ciphertext, plaintext.data(), ct_len, &this->cbc_);
        FX_TOMCRYPT_ASSERT(errc, "TEA-CBC decrypt failed!", {})
        return plaintext;
    }

	[[nodiscard]] std::vector<std::uint8_t> decrypt(const std::vector<std::uint8_t>& iv, const std::vector<std::uint8_t>& ciphertext) noexcept {
		return this->decrypt(iv.data(), ciphertext.data(), ciphertext.size());
	}
};

#endif