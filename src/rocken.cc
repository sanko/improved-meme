#include "rocken.hh"

#ifndef _MSC_VER
#include <codecvt>
#include <locale>
#include <string>
#include <string_view>
#endif

#include <cstdint>

namespace Rocken
{

inline bool decode_codepoint(const char *s8, size_t l, size_t &bytes, char32_t &cp) {
    if (l) {
        auto b = static_cast<uint8_t>(s8[0]);
        if ((b & 0x80) == 0) {
            bytes = 1;
            cp = b;
            return true;
        }
        else if ((b & 0xE0) == 0xC0) {
            if (l >= 2) {
                bytes = 2;
                cp = ((static_cast<char32_t>(s8[0] & 0x1F)) << 6) |
                     (static_cast<char32_t>(s8[1] & 0x3F));
                return true;
            }
        }
        else if ((b & 0xF0) == 0xE0) {
            if (l >= 3) {
                bytes = 3;
                cp = ((static_cast<char32_t>(s8[0] & 0x0F)) << 12) |
                     ((static_cast<char32_t>(s8[1] & 0x3F)) << 6) |
                     (static_cast<char32_t>(s8[2] & 0x3F));
                return true;
            }
        }
        else if ((b & 0xF8) == 0xF0) {
            if (l >= 4) {
                bytes = 4;
                cp = ((static_cast<char32_t>(s8[0] & 0x07)) << 18) |
                     ((static_cast<char32_t>(s8[1] & 0x3F)) << 12) |
                     ((static_cast<char32_t>(s8[2] & 0x3F)) << 6) |
                     (static_cast<char32_t>(s8[3] & 0x3F));
                return true;
            }
        }
    }
    return false;
}

inline size_t decode_codepoint(const char *s8, size_t l, char32_t &cp) {
    size_t bytes;
    if (decode_codepoint(s8, l, bytes, cp)) { return bytes; }
    return 0;
}

inline char32_t decode_codepoint(const char *s8, size_t l) {
    char32_t cp = 0;
    decode_codepoint(s8, l, cp);
    return cp;
}
inline std::u32string decode(const char *s8, size_t l) {
    std::u32string out;
    size_t i = 0;
    while (i < l) {
        auto beg = i++;
        while (i < l && (s8[i] & 0xc0) == 0x80) {
            i++;
        }
        out += decode_codepoint(&s8[beg], (i - beg));
    }
    return out;
}

#ifndef _MSC_VER
std::u32string to_utf32(std::string_view s) {
    //~ std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
    //~ return conv.from_bytes(s.begin(), s.end());

    return decode(s.begin(), s.length());
}

std::string to_utf8(std::u32string_view s) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
    return conv.to_bytes(s.begin(), s.end());
}
#endif // _MSC_VER

}; // namespace Rocken
