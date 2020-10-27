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
            double DistanceTo(const Vector2& target) const noexcept {
                const double DistX = (double)target.x - (double)this->x;
                const double DistY = (double)target.y - (double)this->y;
                return sqrt( pow(DistX,2)+pow(DistY,2) );
            }
            
            // Returns the angle between two vectors in radians
            double AngleTo(const Vector2& target) const noexcept {  
                const double DistX = (double)target.x - (double)this->x;
                const double DistY = (double)target.y - (double)this->y;
                return atan(DistY/DistX);
            }


            //Returns the dot product of two vectors
            double DotProduct(const Vector2& target) const noexcept {
                return ((double)this->x * (double)target.x) + ((double)this->y * (double)target.y);
            }

            ///////////////////////Operators/////////////////////////////////

            [[nodiscard]]
            inline Vector2<Type> operator+(const Vector2<Type>& target) const noexcept {
                return Vector2<Type>(this->x+target.x,this->y+target.y);
            }

            [[nodiscard]]
            inline Vector2<Type> operator-(const Vector2<Type>& target) const noexcept {
                return Vector2<Type>(this->x-target.x,this->y-target.y);
            }

            [[nodiscard]]
            inline Vector2<Type> operator*(const Vector2<Type>& target) noexcept {
                return Vector2<Type>(this->x*target.x,this->y*target.y);
            }

            [[nodiscard]]
            inline Vector2<Type> operator/(const Vector2<Type>& target) noexcept {
                return Vector2<Type>(this->x/target.x,this->y/target.y);
            }

            inline Vector2<Type>& operator+=(const Vector2<Type>& other) noexcept {
                this->x += other.x;
                this->y += other.y;
                return *this;
            }
            
            inline Vector2<Type>& operator-=(const Vector2<Type>& other) noexcept {
                this->x -= other.x;
                this->y -= other.y;
                return *this;
            }

            inline Vector2<Type>& operator*=(const Vector2<Type>& other) noexcept {
                this->x *= other.x;
                this->y *= other.y;
                return *this;
            }

            inline Vector2<Type>& operator/=(const Vector2<Type>& other) noexcept {
                this->x /= other.x;
                this->y /= other.y;
                return *this;
            }

    };

    class Angle{

        static double constexpr  PI = 3.14159265;

        public:
            // Returns radians in degrees
            constexpr static double RadToDeg(double rad) noexcept {
                //Recieves an angle in radians and return in degrees
                double deg = rad * (180.0/PI);
                return deg;
            };

            // Returns degrees in radians
            constexpr static double DegToRad(double deg) noexcept {
                //Recieves an angle in degrees and return in radians
                double rad = deg * (PI/180.0);
                return rad;
            };
    };
}


#endif

