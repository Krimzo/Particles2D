#pragma once

#include "math/math.h"

#include <vector>


namespace kl {
    class encryptor
    {
    public:
        using key = std::vector<uint8_t>;

        key keys[5] = {};

        encryptor();

        void regenerate_keys();

        void encrypt(void* data, uint64_t size) const;

        void decrypt(void* data, uint64_t size) const;

        template <typename T>
        void encrypt(T& obj) const
        {
            encrypt(&obj, sizeof(T));
        }

        template <typename T>
        void decrypt(T& obj) const
        {
            decrypt(&obj, sizeof(T));
        }
    };

    std::ostream& operator<<(std::ostream& os, const encryptor& encryptor);
}
