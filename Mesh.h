#ifndef MESH_H
#define MESH_H

#include "Ray.h"
#include "Interval.h"
#include "IntersectionManager.h"

class Mesh {
    public: 
        virtual bool hit( Ray &ray, Interval interval, IntersectionManager &intersectionManager ) const = 0;
};

#endif