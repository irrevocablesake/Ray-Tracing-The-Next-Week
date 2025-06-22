#ifndef WORLD_H
#define WORLD_H

#include "Mesh.h"
#include "Interval.h"
#include "IntersectionManager.h"

#include <vector>
#include <memory>

using std::shared_ptr;

class World{
    public:
        std::vector< shared_ptr< Mesh > > meshList;

        World() {}

        void clear() {
            meshList.clear();
        }

        void add( shared_ptr< Mesh > mesh ){
            meshList.push_back( mesh );
        }

        bool raycast( Ray &ray, Interval interval, IntersectionManager &intersectionManager ){
            IntersectionManager record;
            bool hitAnything = false;
            double closestSoFar = interval.max;

            for( const auto &mesh : meshList ){
                if( mesh -> hit( ray, Interval( interval.min, closestSoFar ), record ) ){
                    hitAnything = true;
                    closestSoFar = record.t;
                    intersectionManager = record;
                }
            }

            return hitAnything;
        }
};

#endif