#pragma once

#include <libfx/fxCore/fxException.hpp>
#include <libfx/fxCore/fxLogging.hpp>

#include <functional>
#include <vector>

class fxSingletonRegistry;

template <typename T>
class fxSingleton {
	static inline T* instance_;
	using self = fxSingleton<T>;
protected:
	fxSingleton() = default;
public:
	[[nodiscard]] static inline T* get();

	static inline void replace(T* inst) {
		if (self::instance_) {
			delete self::instance_;
		}

		self::instance_ = inst;
	}

	static inline void destroy() {
		delete self::instance_;
	}
};

using fxSingletonCleanupCallback = std::function<void()>;

class fxSingletonRegistryInternal {
	std::atomic<bool> cleaning_up_;
	std::vector<fxSingletonCleanupCallback> cleanup_list_;

protected:
	friend class fxSingleton<fxSingletonRegistryInternal>;

	fxSingletonRegistryInternal()
		: cleaning_up_(false)
	{}
public:
	[[nodiscard]] bool add(fxSingletonCleanupCallback cb) noexcept {
		if (this->cleaning_up_) {
			fxPrintf(fxLogError, "cannot add a new fxSingleton whilst cleaning up");
			return false;
		}

		this->cleanup_list_.emplace_back(cb);
		return true;
	}

	void clean_up() noexcept {
		this->cleaning_up_ = true;
		for (const auto& dtor : this->cleanup_list_) {
			dtor();
		}
	}
};

class fxSingletonRegistry : public fxSingleton<fxSingletonRegistryInternal> {
protected:
	fxSingletonRegistry() = default;
};

template <typename T>
[[nodiscard]] inline T* fxSingleton<T>::get() {
	if (self::instance_) {
		return self::instance_;
	}

	self::instance_ = new T();
	if (!self::instance_) {
		return fxError("failed to create singleton instance of type: {}", typeid(T).name());
	}

	if (!fxSingletonRegistry::get()->add(fxSingleton<T>::destroy)) {
		delete self::instance_;
		return fxError("failed to add singleton of type '{}' to registry", typeid(T).name());
	}

	return self::instance_;
}
