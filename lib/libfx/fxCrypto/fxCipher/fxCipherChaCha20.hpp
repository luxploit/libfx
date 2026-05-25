#pragma once

#include <libfx/fxCrypto/fxCryptoUtils.hpp>
#include <libfx/fxCore/fxLogging.hpp>
#include <cstring>

#ifndef FX_DISABLE_CRYPTO

class fxCipherChaCha20 {
    chacha_state state_{};
    bool initialized_ = false;
public:
    fxCipherChaCha20() = default;

    fxCipherChaCha20(const std::uint8_t* key, std::size_t keylen) {
        this->init(key, keylen);
    }

    fxCipherChaCha20(const std::vector<std::uint8_t>& key) {
        this->init(key.data(), key.size());
    }

    ~fxCipherChaCha20() {
        if (this->initialized_) {
            chacha_done(&this->state_);
        }
    }

    fxCipherChaCha20(const fxCipherChaCha20&) = delete;
    fxCipherChaCha20& operator=(const fxCipherChaCha20&) = delete;

    bool init(const std::uint8_t* key, const std::size_t keylen) noexcept;
	bool init(const std::vector<std::uint8_t>& key) noexcept {
		return this->init(key.data(), key.size());
	}

    [[nodiscard]] std::vector<std::uint8_t> encrypt(const std::uint8_t* iv, const std::uint8_t* plaintext, const std::size_t pt_len) noexcept;
	[[nodiscard]] std::vector<std::uint8_t> encrypt(const std::vector<std::uint8_t>& iv, const std::vector<std::uint8_t>& plaintext) noexcept {
		return this->encrypt(iv.data(), plaintext.data(), plaintext.size());
	}

    [[nodiscard]] std::vector<std::uint8_t> decrypt(const std::uint8_t* iv, const std::uint8_t* ciphertext, const std::size_t ct_len) noexcept {
        return this->encrypt(iv, ciphertext, ct_len);
    }

	[[nodiscard]] std::vector<std::uint8_t> decrypt(const std::vector<std::uint8_t>& iv, const std::vector<std::uint8_t>& ciphertext) noexcept {
		return this->decrypt(iv.data(), ciphertext.data(), ciphertext.size());
	}
};

#endif