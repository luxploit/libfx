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

    fxCipherChaCha20(std::vector<std::uint8_t> key) {
        this->init(key.data(), key.size());
    }

    ~fxCipherChaCha20() {
        if (this->initialized_) {
            chacha_done(&this->state_);
        }
    }

    fxCipherChaCha20(const fxCipherChaCha20&) = delete;
    fxCipherChaCha20& operator=(const fxCipherChaCha20&) = delete;

    bool init(const std::uint8_t* key, const std::size_t keylen) noexcept {
        if (keylen != 32) {
            fxPrintf(fxLogError, "ChaCha20 setup failed! keylen %zu is not 32", keylen);
            return false;
        }

        if (this->initialized_) {
            chacha_done(&this->state_);
            this->initialized_ = false;
        }

        auto errc = chacha_setup(&this->state_, key, keylen, 20);
        FX_TOMCRYPT_ASSERT(errc, "ChaCha20 setup failed!", {})

        this->initialized_ = true;
        return true;
    }

	bool init(std::vector<std::uint8_t> key) noexcept {
		return this->init(key.data(), key.size());
	}

    [[nodiscard]] std::vector<std::uint8_t> encrypt(const std::uint8_t* iv, const std::uint8_t* plaintext, const std::size_t pt_len) noexcept {
        if (!this->initialized_ || !plaintext || !iv) {
			fxPrintf(fxLogError, "ChaCha20 crypt failed! requirements not met");
			return {};
		}

        // Set IV and initial counter (0). Assumes standard 12-byte IETF nonce.
        auto errc = chacha_ivctr32(&this->state_, iv, 12, 0);
        FX_TOMCRYPT_ASSERT(errc, "ChaCha20 IV setup failed!", {})

        std::vector<std::uint8_t> ciphertext(pt_len);
        errc = chacha_crypt(&this->state_, plaintext, pt_len, ciphertext.data());
        FX_TOMCRYPT_ASSERT(errc, "ChaCha20 encrypt failed!", {})

        return ciphertext;
    }

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