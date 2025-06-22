#ifndef MATERIAL_H
#define MATERIAL_H

#include "Color3.h"
#include "Ray.h"
#include "IntersectionManager.h"
#include "Random.h"
#include "Vector3.h"

class Material {
    public:
        virtual ~Material() = default;

        virtual bool scatter( const Ray &ray, Color3 &attenuation, Ray &scattered, IntersectionManager &intersectionManager ) const = 0;
};

class Normal : public Material {
    public:
        Normal() {}

        bool scatter( const Ray &ray, Color3 &attenuation, Ray &scattered, IntersectionManager &intersectionManager ) const override {
            scattered = Ray( intersectionManager.point, intersectionManager.normal );
            attenuation = 0.5 * Color3( scattered.direction().x() + 1, scattered.direction().y() + 1, scattered.direction().z() + 1 );
        
            return false;
        }
};

class Solid : public Material {
    public:
        Solid( Color3 color ) : albedo( color ) {}

        bool scatter( const Ray &ray, Color3 &attenuation, Ray &scattered, IntersectionManager &intersectionManager ) const override {
            scattered = Ray( intersectionManager.point, intersectionManager.normal );
            attenuation = albedo;
        
            return false;
        }

        Color3 getAlbedo() const {
            return albedo;
        }

    private:
        Color3 albedo;
};

class Diffuse : public Material {
    public:
        Diffuse() {}
        Diffuse( Color3 color ) : albedo( color ) {}

        Vector3 reflected( const Vector3 &normal ) const{
            Vector3 unitVector = generateRandomUnitVector();
            if( dot( unitVector, normal ) > 0.0 ){
                return unitVector;
            }
            
            return -unitVector;
        }

        bool scatter( const Ray &ray, Color3 &attenuation, Ray &scattered, IntersectionManager &intersectionManager ) const override {
            Vector3 reflectedVector = intersectionManager.normal + reflected( intersectionManager.normal );

            if( reflectedVector.nearZero() ){
                reflectedVector = intersectionManager.normal;
            }
            
            scattered = Ray( intersectionManager.point, reflectedVector );
            attenuation = albedo;

            return true;
        }

    private:
        Color3 albedo;
};

class Metal : public Material {
    public:

        Metal( const Color3 &color, double fuzz ) : albedo( color ), fuzz( fuzz < 1 ? fuzz : 1 ) { }

        Vector3 reflected( const Vector3 & vector, const Vector3 &normal ) const{
            return vector - 2 * dot( vector, normal ) * normal;
        }

        bool scatter( const Ray &ray, Color3 &attenuation, Ray &scattered, IntersectionManager &intersectionManager ) const override {
            Vector3 reflectedVector = reflected( ray.direction(), intersectionManager.normal );
            reflectedVector = unitVector( reflectedVector ) + ( fuzz * generateRandomUnitVector() );
            scattered = Ray( intersectionManager.point, reflectedVector );
            attenuation = albedo;

            return ( dot( scattered.direction(), intersectionManager.normal ) > 0 );
        }

    private:
        Color3 albedo;
        double fuzz;
};

class Dielectric : public Material {
    public:

        Dielectric( double refractionIndex ) : refractionIndex( refractionIndex ) { }

        Vector3 refract( const Vector3 &vector, const Vector3 &normal, double etaiOverEtat ) const{
            double cosTheta = std::fmin( dot( -vector, normal ), 1.0 );
            Vector3 rOutPerpendicular = etaiOverEtat * ( vector + cosTheta * normal );
            Vector3 rOutParallel = -std::sqrt( std::fabs( 1.0 - rOutPerpendicular.lengthSquared())) * normal;

            return rOutPerpendicular + rOutParallel;
        }

        Vector3 reflected( const Vector3 & vector, const Vector3 &normal ) const{
            return vector - 2 * dot( vector, normal ) * normal;
        }

        double reflectance( double cosine, double RI ) const{
            double r0 = ( 1 - RI ) / ( 1 + RI );
            r0 = r0 * r0;
            return r0 + ( 1 - r0 ) * std::pow( ( 1 - cosine ), 5 );
        }

        bool scatter( const Ray &ray, Color3 &attenuation, Ray &scattered, IntersectionManager &intersectionManager ) const override {
            attenuation = Color3( 1.0, 1.0, 1.0 );
            double rI = intersectionManager.frontFace ? ( 1.0 / refractionIndex ) : refractionIndex;

            Vector3 unitDirection = unitVector( ray.direction() );
            
            double cosTheta = std::fmin( dot( -unitDirection, intersectionManager.normal ), 1.0 );
            double sinTheta = std::sqrt( 1.0 - cosTheta * cosTheta );

            bool cannotRefract = rI * sinTheta > 1.0;
            Vector3 direction;

            if( cannotRefract || reflectance( cosTheta, rI ) > generateRandomNumber() ){
                direction = reflected( unitDirection, intersectionManager.normal );
            }
            else{
                direction = refract( unitDirection, intersectionManager.normal, rI );
            }

            scattered = Ray( intersectionManager.point, direction );
            return true;
        } 

    private:
        double refractionIndex;
};

#endif