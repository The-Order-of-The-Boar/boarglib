#ifndef BOAR_CORE
#define BOAR_CORE


#include <cmath>

using std::sqrt;
using std::pow;
using std::atan;

namespace boar{

    template<typename Type>
    //Mathematical Bidimensional Vector
    class Vector2{


        public:

            Type x;
            Type y;

            //Init vector with X and Y coordinates
            Vector2(Type x, Type y): x{x}, y{y} {};

            ///////////////////////Mathematical Methods/////////////////////////////////

            //Returns diagonal distance between vectors
            double DistanceTo(const Vector2& target) noexcept{

                const auto DistX = target.x - this->x;
                const auto DistY = target.y - this->y;
                return sqrt( pow(DistX,2)+pow(DistY,2) );

            }
            
            // Returns the angle between two vectors in radians
            double AngleTo(const Vector2& target) noexcept{
                
                const auto DistX = target.x - this->x;
                const auto DistY = target.y - this->y;
                const double tg = (double)DistY/DistX;
                return atan(tg);

            }


            //Returns the dot product of two vectors
            double DotProduct(const Vector2& target) noexcept{
                return (this->x * target.x) + (this->y * target.y);
            }

            ///////////////////////Operators/////////////////////////////////

            Vector2<Type> operator+(const Vector2<Type>& target) noexcept{
                return Vector2<Type>(this->x+target.x,this->y+target.y);
            }

            Vector2<Type> operator-(const Vector2<Type>& target) noexcept{
                return Vector2<Type>(this->x-target.x,this->y-target.y);
            }

            Vector2<Type> operator*(const Vector2<Type>& target) noexcept{
                return Vector2<Type>(this->x*target.x,this->y*target.y);
            }

            Vector2<Type> operator/(const Vector2<Type>& target) noexcept{
                return Vector2<Type>(this->x/target.x,this->y/target.y);
            }




    };

    class Angle{

        static double constexpr  PI = 3.14159265;

        public:
            // Returns radians in degrees
            constexpr static double RadToDeg(double rad){
                //Recieves an angle in radians and return in degrees
                double deg = rad * (180.0/PI);
                
                return deg;

            };

            // Returns degrees in radians
            constexpr static double DegToRad(double deg){
                //Recieves an angle in degrees and return in radians
                double rad = deg * (PI/180.0);
                
                return rad;

            };
    };
}


#endif

