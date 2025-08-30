#ifndef BOX_H
#define BOX_H

#include "Point3.h"
#include "Material.h"
#include "Parallelogram.h"
#include "Mesh.h"

#include <memory>
#include <vector>

using std::shared_ptr;

class Box : public Mesh {

    public:
        std::vector< shared_ptr< Parallelogram >> faces;

    Box( Point3 a, Point3 b, shared_ptr< Material > material ){
        Point3 min = Point3( std::fmin( a.x(), b.x() ), std::fmin( a.y(), b.y() ), std::fmin( a.z(), b.z() ) );
        Point3 max = Point3( std::fmax( a.x(), b.x() ), std::fmax( a.y(), b.y() ), std::fmax( a.z(), b.z() ) );

        Vector3 sideX = Vector3( max.x() - min.x(), 0, 0 );
        Vector3 sideY = Vector3( 0, max.y() - min.y(), 0 );
        Vector3 sideZ = Vector3( 0, 0, max.z() - min.z() );

        faces.push_back( make_shared< Parallelogram >( Point3( min.x(), min.y(), max.z() ), sideX, sideY, material ) );
        faces.push_back( make_shared< Parallelogram >( Point3( max.x(), min.y(), max.z() ), -sideZ, sideY, material ) );
        faces.push_back( make_shared< Parallelogram >( Point3( max.x(), min.y(), min.z() ), -sideX, sideY, material ) );
        faces.push_back( make_shared< Parallelogram >( Point3( min.x(), min.y(), min.z() ), sideZ, sideY, material ) );
        faces.push_back( make_shared< Parallelogram >( Point3( min.x(), max.y(), max.z() ), sideX, -sideZ, material ) );
        faces.push_back( make_shared< Parallelogram >( Point3( min.x(), min.y(), min.z() ), sideX, sideZ, material ) );
    }

    std::vector< shared_ptr< Parallelogram >> getFaces(){
        return faces;
    }
};

#endif