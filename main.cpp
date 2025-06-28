#include "Renderer.h"

#include<iostream>
#include "Ray.h"
#include "Camera.h"
#include "Image.h"
#include "World.h"
#include "Sphere.h"
#include "Random.h"
#include "Texture.h"
#include "shapes2D.h"

#include <memory>

using std::make_shared;

void classicScene(){
    const int IMAGE_WIDTH = 400;
    const double ASPECT_RATIO = 16.0 / 9.0;
    Image image( IMAGE_WIDTH, ASPECT_RATIO );

    World world;

    auto checkerTexture = make_shared< CheckerTexture >( 0.32, Color3( .2, .3, .1 ), Color3( .9, .9, .9 ));
    auto ground_material = make_shared<Diffuse>( checkerTexture );

    world.add(make_shared<Sphere>(Point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = generateRandomNumber();
            Point3 center(a + 0.9*generateRandomNumber(), 0.2, b + 0.9*generateRandomNumber());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color3( generateRandomNumber(), generateRandomNumber(), generateRandomNumber() );
                    sphere_material = make_shared<Diffuse>(albedo);

                    Point3 center2 = center + Vector3( 0, generateRandomNumber( 0, 0.5 ), 0 );

                    world.add(make_shared<Sphere>(center, center2, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color3( generateRandomNumber( 0.5, 1 ), generateRandomNumber( 0.5, 1 ), generateRandomNumber( 0.5, 1 ) );
                    auto fuzz = generateRandomNumber(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Diffuse>(Color3(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Color3(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    Renderer renderer( world, image );

    renderer.samplesPerPixel = 50;
    renderer.maxDepth = 10;
    renderer.vFOV = 20.0;
    renderer.lookFrom = Point3( 13,2,3 );
    renderer.lookAt = Point3( 0,0,0 );
    renderer.vUp = Vector3( 0, 1, 0 );

    renderer.defocusAngle = 0.6;
    renderer.focusDistance = 10.0;

    renderer.initialize();
    renderer.render();
}

void checkerTextureScene(){
    const int IMAGE_WIDTH = 400;
    const double ASPECT_RATIO = 16.0 / 9.0;
    Image image( IMAGE_WIDTH, ASPECT_RATIO );

    World world;

    auto checker = make_shared< CheckerTexture >(0.32, Color3(.2, .3, .1), Color3(.9, .9, .9));

    world.add(make_shared<Sphere>(Point3(0,-10, 0), 10, make_shared<Diffuse>(checker)));
    world.add(make_shared<Sphere>(Point3(0, 10, 0), 10, make_shared<Diffuse>(checker)));

    Renderer renderer( world, image );

    renderer.samplesPerPixel = 50;
    renderer.maxDepth = 10;
    renderer.vFOV = 20.0;
    renderer.lookFrom = Point3( 13,2,3 );
    renderer.lookAt = Point3( 0,0,0 );
    renderer.vUp = Vector3( 0, 1, 0 );

    renderer.defocusAngle = 0;
    renderer.focusDistance = 10.0;

    renderer.initialize();
    renderer.render();
}

void texturedSphere() {
    const int IMAGE_WIDTH = 400;
    const double ASPECT_RATIO = 16.0 / 9.0;
    Image image( IMAGE_WIDTH, ASPECT_RATIO );

    World world;

    auto texture = make_shared< ImageTexture >("wood.jpg");
    auto surface = make_shared< Diffuse >( texture );
    auto sphere = make_shared< Sphere >( Point3( 0, 0, 0 ), 2, surface );

    world.add( sphere );

    Renderer renderer( world, image );

    renderer.samplesPerPixel = 50;
    renderer.maxDepth = 10;
    renderer.vFOV = 20.0;
    renderer.lookFrom = Point3( 0,0,12 );
    renderer.lookAt = Point3( 0,0,0 );
    renderer.vUp = Vector3( 0, 1, 0 );

    renderer.defocusAngle = 0;
    renderer.focusDistance = 10.0;

    renderer.initialize();
    renderer.render();
}

void parallelogramScene() {
    const int IMAGE_WIDTH = 400;
    const double ASPECT_RATIO = 1.0;
    Image image( IMAGE_WIDTH, ASPECT_RATIO );

    World world;

    auto leftMaterial     = make_shared<Diffuse>(Color3(1.0, 0.2, 0.2));
    auto backMaterial   = make_shared<Diffuse>(Color3(0.2, 1.0, 0.2));
    auto rightMaterial   = make_shared<Diffuse>(Color3(0.2, 0.2, 1.0));
    auto upMaterial = make_shared<Diffuse>(Color3(1.0, 0.5, 0.0));
    auto downMaterial   = make_shared<Diffuse>(Color3(0.2, 0.8, 0.8));

    auto leftMesh = make_shared< Parallelogram >( Point3(-3,-2, 5), Vector3(0, 0,-4), Vector3(0, 4, 0), leftMaterial );
    auto backMesh = make_shared< Parallelogram >(Point3(-2,-2, 0), Vector3(4, 0, 0), Vector3(0, 4, 0), backMaterial);
    auto rightMesh = make_shared< Parallelogram >(Point3( 3,-2, 1), Vector3(0, 0, 4), Vector3(0, 4, 0), rightMaterial);
    auto upMesh = make_shared< Parallelogram >(Point3(-2, 3, 1), Vector3(4, 0, 0), Vector3(0, 0, 4), upMaterial);
    auto downMesh = make_shared< Parallelogram >(Point3(-2,-3, 5), Vector3(4, 0, 0), Vector3(0, 0,-4), downMaterial);

    world.add( leftMesh );
    world.add( backMesh );
    world.add( rightMesh );
    world.add( upMesh );
    world.add( downMesh );

    Renderer renderer( world, image );

    renderer.samplesPerPixel = 100;
    renderer.maxDepth = 50;
    renderer.vFOV = 80.0;
    renderer.lookFrom = Point3( 0,0,9 );
    renderer.lookAt = Point3( 0,0,0 );
    renderer.vUp = Vector3( 0, 1, 0 );

    renderer.defocusAngle = 0;
    renderer.focusDistance = 10.0;

    renderer.initialize();
    renderer.render();
}

int main(){
    
    int scene = 4;

    switch( scene ){
        case 1: classicScene();  break;
        case 2: checkerTextureScene(); break; 
        case 3: texturedSphere(); break;
        case 4: parallelogramScene(); break;
    }
    
    return 0;
}