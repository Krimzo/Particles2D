#pragma once

#include <iostream>


namespace kl {
    class color
    {
    public:
        uint8_t b, g, r, a;

        color();
        color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

        bool operator==(const color& obj) const;
        bool operator!=(const color& obj) const;

        [[nodiscard]] color gray() const;

        [[nodiscard]] color inverted() const;

        [[nodiscard]] char as_ascii() const;

        [[nodiscard]] color mix(const color& color, float ratio) const;
        [[nodiscard]] color mix(const color& color) const;
    };

    std::ostream& operator<<(std::ostream& stream, const color& object);
}

namespace kl::colors {
    inline const color console = color(204, 204, 204);
    inline const color black = color(0, 0, 0);
    inline const color white = color(255, 255, 255);
    inline const color gray = color(50, 50, 50);
    inline const color red = color(255, 0, 0);
    inline const color green = color(0, 255, 0);
    inline const color blue = color(0, 0, 255);
    inline const color cyan = color(30, 180, 170);
    inline const color purple = color(220, 0, 220);
    inline const color yellow = color(220, 220, 0);
    inline const color orange = color(255, 140, 0);
    inline const color magenta = color(155, 0, 155);
    inline const color crimson = color(100, 0, 0);
    inline const color wheat = color(245, 220, 180);
    inline const color sky = color(190, 245, 255);
}
