#pragma once
#include <atomic>

#include <libfx/fxConfig.hpp>

#include <libfx/fxCore/fxException.hpp>
#include <libfx/fxCore/fxLogging.hpp>
#include <libfx/fxCore/fxSingleton.hpp>

#ifndef FX_DISABLE_CRYPTO
#include <libfx/fxCrypto/fxCipher.hpp>
#endif

#define FX_VERSION "0.0.0"

struct fxBootstrapConfig {

};

class fxBootstrap {
	fxBootstrap() = delete;
public:
	static void init();
	static void quit();

	static inline std::atomic<bool> initialized_ = false;
};
