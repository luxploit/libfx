#pragma once

#include <libfx/fxCrypto/fxCryptoUtils.hpp>
#include <libfx/fxCore/fxLogging.hpp>

#ifndef FX_DISABLE_CRYPTO

class fxCipherRC4 {
    rc4_state state_{};
    std::vector<std::uint8_t> key_storage{};
    bool initialized_ = false;
public:
    fxCipherRC4() = default;

    fxCipherRC4(const std::uint8_t* key, std::size_t keylen) {
        this->init(key, keylen);
    }

    fxCipherRC4(std::vector<std::uint8_t> key) {
        this->init(key.data(), key.size());
    }

    ~fxCipherRC4() = default;

    fxCipherRC4(const fxCipherRC4&) = delete;
    fxCipherRC4& operator=(const fxCipherRC4&) = delete;

    bool init(const std::uint8_t* key, const std::size_t keylen) noexcept {
        if (keylen < 1 || keylen > 256) {
            fxPrintf(fxLogError, "RC4 setup failed! keylen out of range");
            return false;
        }

        this->key_storage.assign(key, key + keylen);
        rc4_stream_setup(&this->state_, this->key_storage.data(), this->key_storage.size());
        this->initialized_ = true;
        return true;
    }

	bool init(std::vector<std::uint8_t> key) noexcept {
		return this->init(key.data(), key.size());
	}

    [[nodiscard]] std::vector<std::uint8_t> encrypt(const std::uint8_t* plaintext, const std::size_t pt_len) noexcept {
        if (!this->initialized_ || !plaintext) {
			fxPrintf(fxLogError, "RC4 crypt failed! requirements not met");
			return {};
		}

        std::vector<std::uint8_t> ciphertext(pt_len);
        rc4_stream_crypt(&this->state_, plaintext, pt_len, ciphertext.data());
        return ciphertext;
    }

	[[nodiscard]] std::vector<std::uint8_t> encrypt(const std::vector<std::uint8_t>& plaintext) noexcept {
		return this->encrypt(plaintext.data(), plaintext.size());
	}

    [[nodiscard]] std::vector<std::uint8_t> decrypt(const std::uint8_t* ciphertext, const std::size_t ct_len) noexcept {
        return this->encrypt(ciphertext, ct_len);
    }

	[[nodiscard]] std::vector<std::uint8_t> decrypt(const std::vector<std::uint8_t>& ciphertext) noexcept {
		return this->decrypt(ciphertext.data(), ciphertext.size());
	}
};

#endif