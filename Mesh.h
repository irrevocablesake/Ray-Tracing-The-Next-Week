#ifndef MESH_H
#define MESH_H

#include "Ray.h"
#include "Interval.h"
#include "IntersectionManager.h"
#include "AABB.h"

class Mesh {
    public:
        virtual ~Mesh() = default;
};

class Primitive : public Mesh {
    public: 
        virtual ~Primitive() = default;
        virtual bool hit( const Ray &ray, Interval interval, IntersectionManager &intersectionManager ) const = 0;
        virtual AABB getBoundingBox() const = 0;
};

#endif