    #ifndef PIXELSAMPLER_H
    #define PIXELSAMPLER_H

    #include "Vector3.h"
    #include "Point3.h"
    #include "Ray.h"
    #include "Color3.h"
    #include "Viewport.h"
    #include "Camera.h"
    #include "IntersectionManager.h"
    #include "World.h"
    #include "Material.h"

    #include "random.h"

    class PixelSampler{
        public:
            int samplesPerPixel;
            int maxDepth;

        public:
            double average;
            Viewport viewport;
            Camera camera;

            PixelSampler() {} 
            PixelSampler( Camera &camera, Viewport &viewport, int samplesPerPixel, int maxDepth ) : camera( camera ), viewport( viewport ), samplesPerPixel( samplesPerPixel ), maxDepth( maxDepth ) {
                average = 1 / double( samplesPerPixel );
            }

            Color3 samplePixel( int i, int j, World &world );
            Vector3 sample();
            
            Color3 processPixelColor( Ray &ray, World &world, int maxDepth );
    };

    Color3 PixelSampler::processPixelColor( Ray &ray, World &world, int maxDepth ){

        if( maxDepth <= 0 ){
            return Color3( 0, 0, 0 );
        }

        Color3 white( 1.0, 1.0, 1.0 );
        Color3 blue( 0.5, 0.7, 1.0 );
        Color3 red( 1.0, 0.0, 0.0 );
        Color3 black( 0.0, 0.0, 0.0 );

        IntersectionManager intersectionManager;

        bool hit = world.raycast( ray, Interval( 0.001, INF ), intersectionManager );

        if( hit ){

            if( auto solidMaterial = dynamic_cast<Solid*>(intersectionManager.material.get()) ){
                return solidMaterial -> getAlbedo();
            }

            if (dynamic_cast<Normal*>(intersectionManager.material.get()) ) {
                return 0.5 * Color3(
                    intersectionManager.normal.x() + 1,
                    intersectionManager.normal.y() + 1,
                    intersectionManager.normal.z() + 1
                );
            }

            Ray scattered;
            Color3 attenuation;

            if( intersectionManager.material -> scatter( ray, attenuation, scattered, intersectionManager ) ){
                Color3 value = processPixelColor( scattered, world, maxDepth - 1 );
                return attenuation * value;
            }
            return Color3( 0, 0, 0 );
        }

        Vector3 normalizedDirection = normalizeVector( unitVector( ray.direction() ) );
        Color3 color = lerpColor( white, blue, normalizedDirection.y() );
        
        return color;
    }

    Vector3 PixelSampler::sample() {
        return Vector3( generateRandomNumber() - 0.5, generateRandomNumber() - 0.5, 0 );
    }

    Color3 PixelSampler::samplePixel( int i, int j, World &world ){
        Color3 pixelColor( 0, 0, 0 );
        
        for( int count = 0; count < samplesPerPixel; count++ ){
            Vector3 offset = sample();
            Point3 pixelSample = viewport.pixel00Location + ( ( i + offset.x() ) * viewport.pixelDeltaHeight ) + ( ( j + offset.y() ) * viewport.pixelDeltaWidth );

            Point3 origin = ( camera.defocusAngle <= 0 ) ? camera.position : camera.defocusDiskSample();
            Ray ray( origin, ( pixelSample - origin ) );

            pixelColor += processPixelColor( ray, world, maxDepth );
        }

        return ( pixelColor * average );
    }

    #endif