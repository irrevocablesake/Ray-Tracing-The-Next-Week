#ifndef SPHERE_H
#define SPHERE_H

#include "Vector3.h"
#include "Mesh.h"
#include "Material.h"

#include <cmath>
#include <memory>

using std::shared_ptr;
using std::make_shared;

class Sphere : public Mesh{
    public:  

        Ray position;
        double radius;
        shared_ptr< Material > material;

        Sphere() : position( Point3( 0, 0, -1.5 ), Vector3( 0, 0, 0 ) ), radius( 0.5 ), material( make_shared<Normal>() ) { }
        Sphere( const Point3 &position, const double radius, shared_ptr< Material> material ) : position( position, Vector3( 0, 0, 0) ), radius( radius ), material( material ) { }
        Sphere( const Point3 &position, const double radius ) : position( position, Vector3( 0, 0, 0 ) ), radius( radius ), material( make_shared<Normal>() ) { }

        Sphere( const Point3 &startCenter, const Point3 &endCenter, double radius, shared_ptr< Material > material ) : position( startCenter, endCenter - startCenter ), radius( radius ), material( material ) { }
        Sphere( const Point3 &startCenter, const Point3 &endCenter, double radius ) : position( startCenter, endCenter - startCenter ), radius( radius ), material( make_shared<Normal>() ) { }

        bool hit( Ray &ray, Interval interval, IntersectionManager &intersectionManager ) const override {
            
            Point3 positionAtTime = position.at( ray.time() );

            Vector3 spherePositionRay = positionAtTime - ray.origin();
            double a = ray.direction().lengthSquared();
            double h = dot( ray.direction(), spherePositionRay );
            double c = spherePositionRay.lengthSquared() - radius * radius;

            double discriminant = h * h - a * c;
            if( discriminant < 0 ){
                return false;
            }

            double sqrtDisriminant = sqrt( discriminant );

            double root = ( h - sqrtDisriminant ) / a;
            if( !interval.surrounds( root ) ){
                root = ( h + sqrtDisriminant ) / a;
                if( !interval.surrounds( root ) ){
                    return false;
                }
            }

            intersectionManager.t = root;
            intersectionManager.point = ray.at( intersectionManager.t );
            intersectionManager.material = material;
            intersectionManager.setFaceNormal( ray, ( intersectionManager.point - positionAtTime ) / radius );

            return true;
        }
};

#endif