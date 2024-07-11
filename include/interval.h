#pragma once

#include "utils.h"

class Interval {
    public:
        double min;
        double max;

        static const Interval empty;
        static const Interval universe;

        Interval() : min(-kInfinity), max(kInfinity) {}
        Interval(double _min, double _max) : min(_min), max(_max) {}

        double size() const { return max - min; }

        double clamp(double x) const {
            if (x < min) return min;
            if (x > max) return max;
            return x;
        }
        bool contains(double x) const { return min <= x && x <= max; }
        bool surrounds(double x) const{ return min < x && x < max; }

};

const Interval Interval::empty = Interval(kInfinity, -kInfinity);
const Interval Interval::universe = Interval(-kInfinity, kInfinity);