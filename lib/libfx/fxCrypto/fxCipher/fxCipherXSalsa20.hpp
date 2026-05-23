#pragma once

#include <libfx/fxCrypto/fxCryptoUtils.hpp>
#include <libfx/fxCore/fxLogging.hpp>
#include <cstring>

#ifndef FX_DISABLE_CRYPTO

class fxCipherXSalsa20 {
    salsa20_state state_{};
    std::vector<std::uint8_t> key_storage{};
    int rounds_ = 20;
    bool initialized_ = false;
public:
    fxCipherXSalsa20() = default;

    fxCipherXSalsa20(const std::uint8_t* key, std::size_t keylen) {
        this->init(key, keylen);
    }

    fxCipherXSalsa20(std::vector<std::uint8_t> key) {
        this->init(key.data(), key.size());
    }

    ~fxCipherXSalsa20() = default;

    fxCipherXSalsa20(const fxCipherXSalsa20&) = delete;
    fxCipherXSalsa20& operator=(const fxCipherXSalsa20&) = delete;

    bool init(const std::uint8_t* key, const std::size_t keylen) noexcept {
        if (keylen != 32) {
            fxPrintf(fxLogError, "XSalsa20 setup failed! keylen %zu is not 32", keylen);
            return false;
        }

        this->key_storage.assign(key, key + keylen);
        this->initialized_ = true;
        return true;
    }

	bool init(std::vector<std::uint8_t> key) noexcept {
		return this->init(key.data(), key.size());
	}

    [[nodiscard]] std::vector<std::uint8_t> encrypt(const std::uint8_t* iv, const std::uint8_t* plaintext, const std::size_t pt_len) noexcept {
        if (!this->initialized_ || !plaintext || !iv) {
			fxPrintf(fxLogError, "XSalsa20 crypt failed! requirements not met");
			return {};
		}

        // XSalsa20 fundamentally requires the IV during setup because it uses
        // the first 16 bytes of the 24-byte nonce to derive a subkey via HSalsa20.
        auto errc = xsalsa20_setup(&this->state_, this->key_storage.data(), 32, iv, 24, this->rounds_);
        FX_TOMCRYPT_ASSERT(errc, "XSalsa20 setup failed!", {})

        // Once setup with the IV is complete, it behaves like standard Salsa20
        std::vector<std::uint8_t> ciphertext(pt_len);
        errc = salsa20_crypt(&this->state_, plaintext, pt_len, ciphertext.data());
        FX_TOMCRYPT_ASSERT(errc, "XSalsa20 encrypt failed!", {})

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