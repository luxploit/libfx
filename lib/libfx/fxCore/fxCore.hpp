#pragma once
#include <atomic>

#include <libfx/fxCore/fxException.hpp>
#include <libfx/fxCore/fxLogging.hpp>
#include <libfx/fxCore/fxSingleton.hpp>

#define FX_VERSION "0.0.0"

struct fxCoreConfig {

};

class fxCore {
	fxCore() = delete;
public:
	static void init();
	static void quit();

	static inline std::atomic<bool> initialized_ = false;
};