#ifndef RENDERER_H
#define RENDERER_H

#include "Vector3.h"
#include "Point3.h"
#include "Color3.h"
#include "Camera.h"
#include "Ray.h"
#include "Image.h"
#include "Viewport.h"
#include "World.h"
#include "Interval.h"
#include "IntersectionManager.h"
#include "PixelSampler.h"
#include<iostream>

class Renderer{
    private:
        Image image;
        Viewport viewport;
        Camera camera;
        World world;
        PixelSampler pixelSampler;

    public:
        int samplesPerPixel = 10;
        int maxDepth = 10;
        float vFOV = 90;
        Point3 lookFrom{ 0, 0, 0 };
        Point3 lookAt{ 0, 0, -1 };
        Vector3 vUp{ 0, 1, 0 };
        float defocusAngle = 1.0;
        float focusDistance = 1.0;

    public: 
        Renderer( const World &world, const Image &image ) : world( world ), image( image ) {}

        void initialize();

        void render();
        void writePixelColor( std::ostream &out, const Color3 &pixelColor );
};

inline double linearToGamma( double linearComponent ){
    if( linearComponent > 0 ){
        return std::sqrt( linearComponent );
    }

    return 0;
}

void Renderer::writePixelColor( std::ostream &out, const Color3 &pixelColor ){
    double rNormalized = pixelColor.r();
    double gNormalized = pixelColor.g();
    double bNormalized = pixelColor.b();
            
    double rGammaCorrected = linearToGamma( rNormalized );
    double gGammaCorrected = linearToGamma( gNormalized );
    double bGammaCorrected = linearToGamma( bNormalized );

    static const Interval intensity( 0.000, 1.000 );
    int rChannel = 255 * intensity.clamp( rGammaCorrected );
    int gChannel = 255 * intensity.clamp( gGammaCorrected );
    int bChannel = 255 * intensity.clamp( bGammaCorrected );

    std::cout << rChannel << ' ' << gChannel << ' ' << bChannel << '\n';
}

void Renderer::initialize(){

    camera.vFOV = vFOV;
    camera.lookFrom = lookFrom;
    camera.lookAt = lookAt;
    camera.vup = vUp;

    camera.updatePosition();
    camera.updateOrientation();

    camera.defocusAngle = defocusAngle;

    if( focusDistance <= 0 ){
        camera.focusDistance = ( lookFrom - lookAt ).length();
    }
    else{
        camera.focusDistance = focusDistance;
    }

    viewport = Viewport( image, camera );
    pixelSampler = PixelSampler( camera, viewport, samplesPerPixel, maxDepth );
}

void Renderer::render(){

    std::cout << "P3\n" << image.width << ' ' << image.height << '\n' << "255\n";

    for( int i = 0; i < image.height; i++ ){

        std::clog << "\rScanlines remaining: " << image.height - ( i + 1 ) << ' ' <<  std::flush; 

        for( int j = 0; j < image.width; j++ ){
            Color3 pixelColor = pixelSampler.samplePixel( i, j, world );
            writePixelColor( std::cout, pixelColor );
        }
    }

    std::clog << "\rDone.                               \n";
}

#endif