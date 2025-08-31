#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include "Mesh.h"
#include "Ray.h"
#include "AABB.h"
#include "Vector3.h"

class Translate : public Mesh {
    public:
        Translate( shared_ptr< Mesh > mesh, const Vector3 offset ) : mesh( mesh ), offset( offset ) {
            boundingBox = mesh -> getBoundingBox() + offset;
        }

        bool hit( const Ray &ray, Interval interval, IntersectionManager &intersectionManager ) const override {
            Ray offsetRay( ray.origin() - offset, ray.direction(), ray.time() );

            if( !( mesh -> hit( offsetRay, interval, intersectionManager ) )){
                return false;
            }

            intersectionManager.point = intersectionManager.point + offset;

            return true;
        }

        AABB getBoundingBox() const override {
            return boundingBox;
        }

    private:
        shared_ptr< Mesh > mesh;
        Vector3 offset;
        AABB boundingBox;

};

#endif