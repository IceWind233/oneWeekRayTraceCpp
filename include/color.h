#ifndef COLOR_H
#define COLOR_H

#include <iostream>

#include "vec3.h"
#include "interval.h"

using color = vec3;

inline double linear2gamma(double linear){
    if (linear > 0)
    {
        return sqrt(linear);
    }
    return 0;
}

void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    r = linear2gamma(r);
    g = linear2gamma(g);
    b = linear2gamma(b);

    static const Interval intensity(0., 0.999);

    // Translate the [0,1] component values to the byte range [0,255].
    int rbyte = int( 256 * intensity.clamp(r) );
    int gbyte = int( 256 * intensity.clamp(g) );
    int bbyte = int( 256 * intensity.clamp(b) );

    // Write out the pixel color components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif