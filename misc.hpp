
#include <cmath>
#include <cstdio>

using std::sqrt;
using std::pow;
using std::ata

namespace misc{

    class Vector2{
    //Mathematical Bidimensional Vector

        int x;
        int y;

        public:
            Vector2(int x, int y){
                //Recieves two integers, the first one to X position and the second one to Y p
                //position
                
                this->x = x;
                this->y = y;
            }

            float DistanceTo(Vector2* target){
                //Recieves a Vector2  target and returns the diagonal distance between this 
                //Vector2 and the target*/

                int DistX = target->x - this->x;
                int DistY = target->y - this->y;
                return sqrt( pow(DistX,2)+pow(DistY,2) );

            }



    class Angle{

        static double constexpr  PI = 3.14159265;

        public:

            static float RadToDeg(double rad){
                //Recieves an angle in radians and return in degrees
                double deg = rad * (180.0/PI);
                
                return deg;

            };

            static double DegToRad(double deg){
                //Recieves an angle in degrees and return in radians
                double rad = deg * (PI/180.0);
                
                return rad;

            };
    };
}


