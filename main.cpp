#include "Renderer.h"

#include<iostream>
#include "Ray.h"
#include "Camera.h"
#include "Image.h"
#include "World.h"
#include "Sphere.h"
#include "Random.h"

#include <memory>

using std::make_shared;

int main(){
    
    const int IMAGE_WIDTH = 1920;
    const double ASPECT_RATIO = 16.0 / 9.0;
    Image image( IMAGE_WIDTH, ASPECT_RATIO );

    World world;

    auto ground_material = make_shared<Diffuse>(Color3(0.5, 0.5, 0.5));
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
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
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

    renderer.samplesPerPixel = 500;
    renderer.maxDepth = 50;
    renderer.vFOV = 20.0;
    renderer.lookFrom = Point3( 13,2,3 );
    renderer.lookAt = Point3( 0,0,0 );
    renderer.vUp = Vector3( 0, 1, 0 );

    renderer.defocusAngle = 0.6;
    renderer.focusDistance = 10.0;

    renderer.initialize();
    renderer.render();

    return 0;
}