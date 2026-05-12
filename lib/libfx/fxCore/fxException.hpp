#pragma once

#include <exception>
#include <format>
#include <string>
#include <utility>

class fxException : public std::exception {
    std::string msg_;
public:
    explicit fxException(std::string m) : msg_(std::move(m)) {}

    const char* what() const noexcept override {
        return this->msg_.c_str();
    }
};

template <typename... Args>
[[noreturn]] inline void fxError(std::format_string<Args...> fmt, Args&&... args) {
    throw fxException(std::format(fmt, std::forward<Args>(args)...));
}
