#include <libfx/fxBootstrap.hpp>

#ifndef FX_DISABLE_CRYPTO
[[gnu::constructor]] void ltc_init() {
	fxCryptoUtils::get()->init_ltc();
}
#endif

void fxBootstrap::init() {
	if (fxBootstrap::initialized_) {
		return fxPrintf(fxLogWarn, "Tried to call init() without calling quit() first!");
	}


	fxBootstrap::initialized_ = true;
}

void fxBootstrap::quit() {
	if (!fxBootstrap::initialized_) {
		return fxPrintf(fxLogWarn, "Tried to call quit() without calling init() first!");
	}

	fxSingletonRegistry::get()->clean_up();
	fxBootstrap::initialized_ = false;
}
