#ifndef INTERVAL_H
#define INTERVAL_H

#include <limits>

const double INF = std::numeric_limits<double>::infinity();

class Interval {
    public:
        double min, max;

        Interval() : min( +INF ), max( -INF ) { }
        Interval( double min, double max ) : min( min ), max( max ) {}

        double size() const {
            return max - min;
        }

        bool contains( double x ) const {
            return min <= x && x <= max;
        }

        bool surrounds( double x ) const {
            return min < x && x < max;
        }

        double clamp( double x ) const {
            if( x < min ) return min;
            if( x > max ) return max;
            return x;
        }

        static const Interval empty, universe;
};

const Interval Interval::empty = Interval( +INF, -INF );
const Interval Interval::universe = Interval( -INF, +INF );

#endif              