#ifndef WORLD_H
#define WORLD_H

#include "Mesh.h"
#include "Interval.h"
#include "IntersectionManager.h"
#include "AABB.h"
#include "BVHNode.h"
#include "Box.h"

#include <vector>
#include <memory>

using std::shared_ptr;
using std::make_shared;

class World{
    public:
        std::vector< shared_ptr< Primitive > > meshList;
        shared_ptr< BVHNode > tree;

        World() {}

        void clear() {
            meshList.clear();
        }

        void add( shared_ptr< Mesh > mesh ){
            if( auto box = std::dynamic_pointer_cast<Box>( mesh) ){
                auto faces = box -> getFaces();

                for( auto &face : faces ){
                    meshList.push_back( face );
                    boundingBox = AABB( boundingBox, face -> getBoundingBox() );
                }
            }
            else{
                auto castedPrimitive = std::dynamic_pointer_cast<Primitive>( mesh );

                meshList.push_back( castedPrimitive );
                boundingBox = AABB( boundingBox, castedPrimitive -> getBoundingBox() );
            }
        }

        bool raycast( Ray &ray, Interval interval, IntersectionManager &intersectionManager ){
            IntersectionManager record;
            bool hitAnything = false;
            double closestSoFar = interval.max;
            
            if( tree -> hit( ray, Interval( interval.min, closestSoFar ), record ) ){
                hitAnything = true;
                closestSoFar = record.t;
                intersectionManager = record;
            }

            return hitAnything;
        }

        void setupAccelerationStructure(){
            tree = make_shared< BVHNode >( meshList, 0, meshList.size() );
        }

        AABB getBoundingBox() const {
            return boundingBox;
        }

    private:
        AABB boundingBox;
};

#endif