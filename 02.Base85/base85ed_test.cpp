#include <gtest/gtest.h>

#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "base85ed.h"

namespace
{

const std::vector<std::pair<const char *, const char *>> short_cases =
{
    {"", ""},
    {"F#", "1"},
    {"F){", "12"},
    {"F)}j", "123"},
    {"F)}kW", "1234"},
};

std::vector<uint8_t> cstr2v(const char *s)
{
    return std::vector<uint8_t>(s, s + std::string(s).size());
}

std::vector<uint8_t> make_bytes(std::size_t size)
{
    std::vector<uint8_t> data(size);
    for (std::size_t i = 0; i < size; ++i)
    {
        data[i] = static_cast<uint8_t>((i * 131U + 17U) % 256U);
    }
    return data;
}

} // namespace

TEST(Base85ShortsEncode, TrivialShortEncodes)
{
    for (const auto &item : short_cases)
    {
        EXPECT_EQ(base85::encode(cstr2v(item.second)), cstr2v(item.first));
    }
}

TEST(Base85ShortsDecode, TrivialShortDecodes)
{
    for (const auto &item : short_cases)
    {
        EXPECT_EQ(base85::decode(cstr2v(item.first)), cstr2v(item.second));
    }
}

TEST(Base85Roundtrip, TextDifferentLengths)
{
    const std::string seed =
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
        "Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.";

    for (std::size_t length = 0; length <= 300U; ++length)
    {
        const std::string text = seed.substr(0, std::min(length, seed.size())) +
                                 std::string(length > seed.size() ? length - seed.size() : 0U, 'x');
        const std::vector<uint8_t> input(text.begin(), text.end());
        EXPECT_EQ(base85::decode(base85::encode(input)), input) << "length=" << length;
    }
}

TEST(Base85Roundtrip, BinaryDifferentLengths)
{
    for (std::size_t length = 0; length <= 1024U; ++length)
    {
        const auto input = make_bytes(length);
        EXPECT_EQ(base85::decode(base85::encode(input)), input) << "length=" << length;
    }
}

TEST(Base85Decode, InvalidInputThrows)
{
    EXPECT_THROW(base85::decode(cstr2v("Z")), std::invalid_argument);
    EXPECT_THROW(base85::decode(cstr2v("~~~~~")), std::invalid_argument);
    EXPECT_THROW(base85::decode(cstr2v("abc[")), std::invalid_argument);
}
