#ifndef BOAR_CORE
#define BOAR_CORE


#include <cmath>

using std::sqrt;
using std::pow;
using std::atan;

namespace boar {

    //Mathematical Bidimensional Vector
    class Vector2{

        int x;
        int y;

        public:
            // Init vector with X and Y coordinates
            Vector2(int x, int y): x{x}, y{y}{};

            // Returns the diagonal difference 
            auto distance_to(const Vector2& target) const noexcept{
                const auto distanceX = target.x - this->x;
                const auto distanceY = target.y - this->y;
                return sqrt(pow(distanceX,2) + pow(distanceY,2));                    
            }

            // Returns the angle between two vectors in radians
            auto angle_to(const Vector2& target) noexcept{
                const auto DistX = target.x - this->x;
                const auto DistY = target.y - this->y;
                const auto tg = DistY/DistX;
                return atan(tg);
            }

            auto dot_product(const Vector2& target){
                return (this->x * target.x) + (this->y * target.y);
            }


    };

    class Angle{

        static auto constexpr PI = 3.14159265;

        public:

            // Returns radians in degrees
            constexpr static auto radians_to_degrees(double rad) noexcept{
                return rad * (180.0/PI);
            }
            
            // Returns degrees in radians
            constexpr static auto degrees_to_radians(double deg) noexcept{
                return deg * (PI/180.0);
            }
    };
}


#endif

