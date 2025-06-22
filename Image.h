#ifndef IMAGE_H
#define IMAGE_H

#include<iostream>

class Image{
    public:
        double aspectRatio;

        int width;
        int height;

        Image() : width( 640 ), height( 480 ), aspectRatio( double( 16.0 ) / double( 9.0 ) ) {}
        Image( const int &width, const double &aspectRatio ) : width( width ), aspectRatio( aspectRatio ) {

            if( width < 1 ){
                throw std::invalid_argument("Image Width must be atleast 1");
            }

            this -> height = width / aspectRatio;

            if( this -> height < 1 ){
                throw std::invalid_argument("Image Height must be atleast 1");
            }
        }
};

#endif