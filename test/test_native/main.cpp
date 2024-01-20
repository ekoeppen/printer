#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest.h>
#include <ranges>
#include <span>
#include <sstream>

#include "printer.h"

struct console {
    char buffer[256];
    size_t end;

    auto reset() -> void
    {
        end = 0;
        memset(buffer, 0, std::size(buffer));
    }
    auto send(const unsigned char c) -> auto
    {
        buffer[end] = c;
        ++end;
    }
};

auto equals(std::string a, const char* b) -> bool
{
    if (a.size() != strlen(b))
        return false;
    for (size_t i = 0; i < a.size(); i++) {
        if (a[i] != b[i])
            return false;
    }
    return true;
}

TEST_CASE("Formatted printing")
{
    console c;
    printer::printer<console> p { c };

    c.reset();
    p(-123, printer::decimal);
    CHECK(equals(c.buffer, "-123"));

    c.reset();
    p(4294967295, printer::hex, printer::right, 16);
    CHECK(equals(c.buffer, "        ffffffff"));

    c.reset();
    p(0x55, printer::hex, printer::right, 8, '0');
    CHECK(equals(c.buffer, "00000055"));

    c.reset();
    p("789");
    CHECK(equals(c.buffer, "789"));

    c.reset();
    p("abc", printer::left, 20);
    CHECK(equals(c.buffer, "abc                 "));

    c.reset();
    p("def", printer::right, 20);
    CHECK(equals(c.buffer, "                 def"));

    c.reset();
    p(0);
    CHECK(equals(c.buffer, "0"));

    c.reset();
    p(0, printer::decimal, printer::left, 4, '.');
    CHECK(equals(c.buffer, "0..."));

    c.reset();
    p("abcd", printer::left, 20);
    CHECK(equals(c.buffer, "abcd                "));

    c.reset();
    p("abcd", printer::right, 20);
    CHECK(equals(c.buffer, "                abcd"));

    c.reset();
    p(0b1001'1001, printer::binary, printer::right, 20);
    CHECK(equals(c.buffer, "            10011001"));

    c.reset();
    p(0377, printer::octal, printer::right, 4);
    CHECK(equals(c.buffer, " 377"));
}

int main(int argc, char** argv)
{
    doctest::Context context;
    context.setOption("success", true);
    context.setOption("no-exitcode", true);
    context.applyCommandLine(argc, argv);
    return context.run();
}
