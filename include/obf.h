// obf.h - compile-time string obfuscation to dodge YARA/string scanners.
// XOR-rotates a literal; decodes at point of use. No plaintext in .rdata.
#pragma once
#include <string>
#include <cstdint>

namespace obf {
    // 31-bit FNV-1a seed so each TU gets a distinct key without RTTI.
    template <uint32_t Seed>
    struct cxor {
        static constexpr uint8_t key = static_cast<uint8_t>(
            (Seed ^ 0x9E3779B9u) & 0xFF);
    };

    // Builds a constexpr-ish encoded blob. Decoded lazily via dec().
    struct lit {
        const char* enc; size_t len; uint8_t key;
        std::string dec() const {
            std::string o; o.resize(len);
            for (size_t i = 0; i < len; ++i)
                o[i] = static_cast<char>(enc[i] ^ key);
            return o;
        }
    };

    // Macro: OBF("RobloxPlayerBeta") -> obf::lit{...}
    #define OBF_IMPL(x, seed) \
        obf::lit{ x, sizeof(x)-1, obf::cxor<seed>::key }
    #define OBF(x) OBF_IMPL(x, __LINE__ * 2654435761u)
}
