#include "CImg-3.5.0_pre12042411/CImg.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <filesystem>

using namespace cimg_library;
//      NIE KOMPILOWAĆ W IDE TYLKO W KONSOLI UŻYWAJĄC PONIŻSZEJ KOMENDY
//    g++ -o TeoriaKodowania6 main.cpp -lX11

int Qvalue (double A ,double C, double q1 , double q2 ) {
    return std::floor(((q1*A) + (q2*C)+ 0.5 ));
}

int Yvalue(double A , double G , double y1 , double y2 ) {
    return std::floor(((y1*A)+(y2*G) + 0.5 ));
}

int Zvalue (double A , double C , double G , double I , double z1 , double z2 , double z3 ,double z4) {
    return std::floor(((z1*A)+(z2*C)+(z3*G)+(z4*I)+0.5 ));
}

int roundUpDivideByTwo(int value) {
    return (value + 1) / 2; // Dodanie 1 przed dzieleniem zapewnia zaokrąglenie w górę
}

struct PSNR {
    char* imageName;
    CImg<unsigned char> image;
    CImg<unsigned char> compressImage;
    CImg<unsigned char> encodedImage;
    int heightOrigin , widthOrigin;
    int heightScale , widthScale;

    PSNR(char* absolutePath) {
        CImg<unsigned char> image(absolutePath);
        widthOrigin = image.width() , widthScale = image.width()/2;
        heightOrigin = image.height() , heightScale = image.height()/2;
        int totalPixels = widthOrigin * heightOrigin;
        std::vector compressImage(heightScale, std::vector<unsigned char>(widthScale));
        for (int y = 0; y < heightScale; y++) {
            for (int x = 0; x < widthScale; x++) {
                compressImage[y][x] = image(x * 2, y * 2, 0, 0);
            }
        }

        this->compressImage = CImg<unsigned char>(widthScale, heightScale, 1, 1, 0); // Inicjalizacja
        this->encodedImage = CImg<unsigned char>(widthScale, heightScale, 1, 1, 0);
        for (int y = 0; y < heightScale; y++) {
            for (int x = 0; x < widthScale; x++) {
                this->compressImage(x, y, 0, 0) = compressImage[y][x];
                this->encodedImage(x, y, 0, 0) = compressImage[y][x];
            }
        }

    }

    void delYoungestBits(int count ) {
        for (int y = 0; y < heightScale; y++) {
            for (int x = 0; x < widthScale; x++) {
                this->compressImage ( x , y , 0 , 0 ) >>= count;
                this->compressImage ( x , y , 0 , 0 ) <<= count;
            }
        }
    }

    void differentialEncoder () {
        for(int y = 0; y < heightScale; y++) {
            for(int x = 0; x < widthScale; x++) {
                if( x == 0 && y == 0 ) {
                    continue;
                }else if ( y == 0 ) {
                    encodedImage(x, y, 0, 0) = compressImage(x, y, 0, 0) - compressImage(x - 1, y, 0, 0);
                } else if ( x == 0) {
                    encodedImage(x, y, 0, 0) = compressImage(x, y, 0, 0) - compressImage(x, y-1, 0, 0);
                }
                else {
                    unsigned char p1 = compressImage(x - 1, y, 0, 0);
                    unsigned char p2 = compressImage(x, y - 1, 0, 0);
                    unsigned char p3 = compressImage(x - 1, y - 1, 0, 0);
                    unsigned char med  = 0;
                    if ( p3 >= p1 && p3 >=  p2 ) {
                        p1 <= p2 ? med = p1 : med = p2;
                        encodedImage(x, y, 0, 0) = compressImage(x, y, 0, 0) - med;
                    }
                    else if (p3 <= p2 && p3 <= p1 ) {
                        p1 >= p2 ? med = p1 : med = p2;
                    }
                    else {
                        med = p1 + p2 - p3;
                    }

                }
            }
        }
    }
};

int main() {
    std::vector<char*> imgs = {
        "/home/bolo/CLionProjects/TeoriaKodowania6/cmake-build-debug/airplane.bmp",
    "/home/bolo/CLionProjects/TeoriaKodowania6/cmake-build-debug/baboonTMW.bmp",
    "/home/bolo/CLionProjects/TeoriaKodowania6/cmake-build-debug/balloon.bmp",
    "/home/bolo/CLionProjects/TeoriaKodowania6/cmake-build-debug/BARB.bmp",
    "/home/bolo/CLionProjects/TeoriaKodowania6/cmake-build-debug/BARB2.bmp",
    "/home/bolo/CLionProjects/TeoriaKodowania6/cmake-build-debug/camera256.bmp",
    "/home/bolo/CLionProjects/TeoriaKodowania6/cmake-build-debug/couple256.bmp",
    "/home/bolo/CLionProjects/TeoriaKodowania6/cmake-build-debug/GOLD.bmp",
    "/home/bolo/CLionProjects/TeoriaKodowania6/cmake-build-debug/lennagrey.bmp",
    "/home/bolo/CLionProjects/TeoriaKodowania6/cmake-build-debug/peppersTMW.bmp"
    };

    return 0;
}

