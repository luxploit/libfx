#include "fxCore.hpp"

void fxCore::init() {
	if (fxCore::initialized_) {
		return fxPrintf(fxLogWarn, "Tried to call init() without calling quit() first!");
	}

	fxCore::initialized_ = true;
}

void fxCore::quit() {
	if (!fxCore::initialized_) {
		return fxPrintf(fxLogWarn, "Tried to call quit() without calling init() first!");
	}

	fxSingletonRegistry::get()->clean_up();
	fxCore::initialized_ = false;
}
