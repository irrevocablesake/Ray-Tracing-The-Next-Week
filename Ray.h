#ifndef RAY_H
#define RAY_H

#include "Point3.h"
#include "Vector3.h"

class Ray{
    public:
        Ray() {}
        Ray( const Point3 &origin, const Vector3 &displacement ) : _origin( origin ), _vector( displacement ) {}

        const Point3 &origin() const;
        const Vector3 &direction() const;

        Point3 at( double t ) const;

    public:
        Point3 point;
        double t;
        Vector3 normal;

    private:
        Point3 _origin;
        Vector3 _vector;
};

const Point3& Ray::origin() const {
    return _origin;
}

const Vector3& Ray::direction() const {
    return _vector;
}

inline Point3 operator+( const Point3 &point, const Vector3 &vector ) {
    return Point3( point.x() + vector.x(), point.y() + vector.y(), point.z() + vector.z() );
}

inline Point3 operator-( const Point3 &point, const Vector3 &vector ) {
    return Point3( point.x() - vector.x(), point.y() - vector.y(), point.z() - vector.z() );
}

inline Vector3 operator-( const Point3 &pointOne, const Point3 &pointTwo ) {
    return Vector3( pointOne.x() - pointTwo.x(), pointOne.y() - pointTwo.y(), pointOne.z() - pointTwo.z() );
}

Point3 Ray::at( double t ) const {
    return _origin + t * _vector;
}

#endif