#pragma once

#include <concepts>
#include <span>

namespace printer {

enum padding { left, right };
enum radix { binary = 2, octal = 8, decimal = 10, hex = 16 };

template<typename Output>
struct printer {
    Output& output;
    static constexpr char hexDigits[] = "0123456789abcdef";

    auto operator()(std::integral auto value, radix r = decimal, padding p = left,
        unsigned int width = 0, char fill = ' ') -> void
    {
        if (value == 0) {
            (*this)(std::string_view { "0" }, p, width, fill);
            return;
        }

        char buffer[16];
        size_t n = std::size(buffer);
        bool negative = value < 0;

        if (negative) {
            value = -value;
        }

        while (value > 0 && n > 0) {
            auto c = value % r;
            buffer[--n] = (r == hex ? hexDigits[c] : static_cast<unsigned char>(c + 48));
            value = value / r;
        }
        if (negative) {
            buffer[--n] = '-';
        }
        (*this)(std::string_view { buffer + n, std::size(buffer) - n }, p, width, fill);
    }

    auto operator()(
        std::string_view value, padding p = left, unsigned int width = 0, char fill = ' ') -> void
    {
        ptrdiff_t n = width - value.size();
        if (p == right) {
            while (n > 0) {
                emit(fill);
                n--;
            }
        }
        for (auto c : value) {
            emit(c);
        }
        if (p == left) {
            while (n > 0) {
                emit(fill);
                n--;
            }
        }
    }

    auto operator()(
        char const* const value, padding p = left, unsigned int width = 0, char fill = ' ') -> void
    {
        size_t size = 0;
        while (value[size] != 0) {
            ++size;
        }
        (*this)(std::string_view { value, size }, p, width, fill);
    }

    auto emit(unsigned char value) -> void { output.send(value); }
};

} // namespace printer
