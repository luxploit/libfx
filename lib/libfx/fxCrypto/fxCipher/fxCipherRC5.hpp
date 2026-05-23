#pragma once

#include <libfx/fxCrypto/fxCryptoUtils.hpp>
#include <libfx/fxCore/fxLogging.hpp>
#include <cstring>

#ifndef FX_DISABLE_CRYPTO

class fxCipherRC5 {
    symmetric_CBC cbc_{};
    bool initialized_ = false;
    int rounds_ = 12;
public:
    fxCipherRC5() = default;

    fxCipherRC5(const std::uint8_t* key, std::size_t keylen) {
        this->init(key, keylen);
    }

    fxCipherRC5(std::vector<std::uint8_t> key) {
        this->init(key.data(), key.size());
    }

    void set_rounds(int rounds) {
        this->rounds_ = rounds;
    }

    ~fxCipherRC5() {
        if (this->initialized_) {
            cbc_done(&this->cbc_);
        }
    }

    fxCipherRC5(const fxCipherRC5&) = delete;
    fxCipherRC5& operator=(const fxCipherRC5&) = delete;

    bool init(const std::uint8_t* key, const std::size_t keylen) noexcept {
        if (keylen < 1 || keylen > 255) {
            fxPrintf(fxLogError, "RC5 setup failed! keylen out of range");
            return false;
        }

        if (this->initialized_) {
            cbc_done(&this->cbc_);
            this->initialized_ = false;
        }

        std::uint8_t dummy_iv[8] = {0};
        auto errc = cbc_start(find_cipher("rc5"), dummy_iv, key, keylen, this->rounds_, &this->cbc_);
        FX_TOMCRYPT_ASSERT(errc, "RC5-CBC start failed!", {})

        this->initialized_ = true;
        return true;
    }

	bool init(std::vector<std::uint8_t> key) noexcept {
		return this->init(key.data(), key.size());
	}

    [[nodiscard]] std::vector<std::uint8_t> encrypt(const std::uint8_t* iv, const std::uint8_t* plaintext, const std::size_t pt_len) noexcept {
        if (!this->initialized_ || !plaintext || !iv || pt_len % 8 != 0) {
			fxPrintf(fxLogError, "RC5-CBC encrypt failed! requirements not met");
			return {};
		}

        std::memcpy(this->cbc_.IV, iv, 8);
        std::vector<std::uint8_t> ciphertext(pt_len);

        auto errc = cbc_encrypt(plaintext, ciphertext.data(), pt_len, &this->cbc_);
        FX_TOMCRYPT_ASSERT(errc, "RC5-CBC encrypt failed!", {})
        return ciphertext;
    }

	[[nodiscard]] std::vector<std::uint8_t> encrypt(const std::vector<std::uint8_t>& iv, const std::vector<std::uint8_t>& plaintext) noexcept {
		return this->encrypt(iv.data(), plaintext.data(), plaintext.size());
	}

    [[nodiscard]] std::vector<std::uint8_t> decrypt(const std::uint8_t* iv, const std::uint8_t* ciphertext, const std::size_t ct_len) noexcept {
        if (!this->initialized_ || !ciphertext || !iv || ct_len % 8 != 0) {
			fxPrintf(fxLogError, "RC5-CBC decrypt failed! requirements not met");
			return {};
		}

        std::memcpy(this->cbc_.IV, iv, 8);
        std::vector<std::uint8_t> plaintext(ct_len);

        auto errc = cbc_decrypt(ciphertext, plaintext.data(), ct_len, &this->cbc_);
        FX_TOMCRYPT_ASSERT(errc, "RC5-CBC decrypt failed!", {})
        return plaintext;
    }

	[[nodiscard]] std::vector<std::uint8_t> decrypt(const std::vector<std::uint8_t>& iv, const std::vector<std::uint8_t>& ciphertext) noexcept {
		return this->decrypt(iv.data(), ciphertext.data(), ciphertext.size());
	}
};

#endif