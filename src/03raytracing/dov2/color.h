#pragma once
//==============================================================================================
// Originally written in 2020 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication
// along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==============================================================================================

#include "interval.h"
#include "vec3.h"

using color = vec3;


inline double linear_to_gamma(double linear_component)
{
    if (linear_component > 0)
        return sqrt(linear_component);

    return 0;
}


uint32_t compute_color(const color& c) {
    auto r = c.x();
    auto g = c.y();
    auto b = c.z();

    int rbyte = r < 0 ? 0 : (r >= 1 ? 255 : int(256 * sqrt(r)));
    int gbyte = g < 0 ? 0 : (g >= 1 ? 255 : int(256 * sqrt(g)));
    int bbyte = b < 0 ? 0 : (b >= 1 ? 255 : int(256 * sqrt(b)));

    return 0xff000000 | (bbyte << 16) | (gbyte << 8) | rbyte;
}

void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Apply a linear to gamma transform for gamma 2
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Translate the [0,1] component values to the byte range [0,255].
    static const interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    // Write out the pixel color components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
