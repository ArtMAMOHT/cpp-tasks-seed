#include "base85ed.h"

#include <algorithm>
#include <array>
#include <limits>
#include <stdexcept>

namespace
{

constexpr char kAlphabet[] =
    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!#$%&()*+-;<=>?@^_`{|}~";

std::array<int, 256> make_reverse_table()
{
    std::array<int, 256> table{};
    table.fill(-1);
    for (int index = 0; index < 85; ++index)
    {
        table[static_cast<unsigned char>(kAlphabet[index])] = index;
    }
    return table;
}

const std::array<int, 256> kReverseTable = make_reverse_table();

void append_encoded_block(uint32_t value, std::vector<uint8_t>& out, std::size_t count)
{
    char block[5];
    for (int pos = 4; pos >= 0; --pos)
    {
        block[pos] = kAlphabet[value % 85U];
        value /= 85U;
    }
    out.insert(out.end(), block, block + count);
}

} // namespace

std::vector<uint8_t> base85::encode(const std::vector<uint8_t> &bytes)
{
    std::vector<uint8_t> out;
    out.reserve((bytes.size() / 4U) * 5U + (bytes.size() % 4U == 0U ? 0U : bytes.size() % 4U + 1U));

    std::size_t pos = 0;
    while (pos + 4U <= bytes.size())
    {
        const uint32_t value = (static_cast<uint32_t>(bytes[pos]) << 24U) |
                               (static_cast<uint32_t>(bytes[pos + 1U]) << 16U) |
                               (static_cast<uint32_t>(bytes[pos + 2U]) << 8U) |
                               static_cast<uint32_t>(bytes[pos + 3U]);
        append_encoded_block(value, out, 5U);
        pos += 4U;
    }

    const std::size_t tail = bytes.size() - pos;
    if (tail > 0U)
    {
        uint32_t value = 0;
        for (std::size_t i = 0; i < tail; ++i)
        {
            value |= static_cast<uint32_t>(bytes[pos + i]) << (24U - 8U * i);
        }
        append_encoded_block(value, out, tail + 1U);
    }

    return out;
}

std::vector<uint8_t> base85::decode(const std::vector<uint8_t> &b85str)
{
    std::vector<uint8_t> out;
    out.reserve((b85str.size() / 5U) * 4U + (b85str.size() % 5U == 0U ? 0U : b85str.size() % 5U - 1U));

    std::size_t pos = 0;
    while (pos < b85str.size())
    {
        const std::size_t chunk_size = std::min<std::size_t>(5U, b85str.size() - pos);
        if (chunk_size == 1U)
        {
            throw std::invalid_argument("base85: invalid tail length");
        }

        uint64_t value = 0;
        for (std::size_t i = 0; i < chunk_size; ++i)
        {
            const int digit = kReverseTable[b85str[pos + i]];
            if (digit < 0)
            {
                throw std::invalid_argument("base85: invalid character");
            }
            value = value * 85U + static_cast<uint32_t>(digit);
        }
        for (std::size_t i = chunk_size; i < 5U; ++i)
        {
            value = value * 85U + 84U;
        }
        if (value > std::numeric_limits<uint32_t>::max())
        {
            throw std::invalid_argument("base85: invalid block value");
        }

        const uint32_t block_value = static_cast<uint32_t>(value);
        const uint8_t block[] =
        {
            static_cast<uint8_t>((block_value >> 24U) & 0xFFU),
            static_cast<uint8_t>((block_value >> 16U) & 0xFFU),
            static_cast<uint8_t>((block_value >> 8U) & 0xFFU),
            static_cast<uint8_t>(block_value & 0xFFU),
        };
        const std::size_t out_count = (chunk_size == 5U) ? 4U : chunk_size - 1U;
        out.insert(out.end(), block, block + out_count);
        pos += chunk_size;
    }

    return out;
}
