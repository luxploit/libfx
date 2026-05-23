#include <libfx/fxBootstrap.hpp>

void fxBootstrap::init() {
	if (fxBootstrap::initialized_) {
		return fxPrintf(fxLogWarn, "Tried to call init() without calling quit() first!");
	}

#ifndef FX_DISABLE_CRYPTO
	fxCryptoUtils::get()->init_ltc();
#endif

	fxBootstrap::initialized_ = true;
}

void fxBootstrap::quit() {
	if (!fxBootstrap::initialized_) {
		return fxPrintf(fxLogWarn, "Tried to call quit() without calling init() first!");
	}

	fxSingletonRegistry::get()->clean_up();
	fxBootstrap::initialized_ = false;
}
