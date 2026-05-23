#pragma once

#include <libfx/fxCore/fxMacros.hpp>
#include <libfx/fxCrypto/fxCryptoUtils.hpp>

#ifndef FX_DISABLE_CRYPTO

class fxRandom {
    prng_state prng;
public:
    fxRandom() {
        fortuna_start(&prng);
        fxCryptoUtils::get()->generate_entropy({
			.prng_name_ = "fortuna",
			.prng_state_ = &prng,
			.prng_entropy_fn_ = [this](const void* data, const std::size_t len) -> void {
				fortuna_add_entropy(FX_VAL_AS(const std::uint8_t*, data), len, &this->prng);
			}
		});
        fortuna_ready(&prng);
    }

    ~fxRandom() {
        fortuna_done(&prng);
    }

    fxRandom(const fxRandom&) = delete;
    fxRandom& operator=(const fxRandom&) = delete;

    [[nodiscard]] std::uint32_t random_u32() noexcept {
        unsigned char buf[4];
        fortuna_read(buf, 4, &prng);
        return FX_DEREF_PTR_AS(std::uint32_t, buf);
    }

    [[nodiscard]] std::uint64_t random_u64() noexcept {
        unsigned char buf[8];
        fortuna_read(buf, 8, &prng);
        return FX_DEREF_PTR_AS(std::uint64_t, buf);
    }
};

#endif