#ifndef BOAR_CORE
#define BOAR_CORE


#include <cmath>
#include <functional>

using std::sqrt;
using std::pow;
using std::atan;

namespace boar{
    
    //Mathematical Bidimensional Vector
    template<typename Type>
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

            [[nodiscard]]
            inline Vector2<Type> GetDelta(const Vector2<Type>& other) const noexcept {
                return Vector2<Type> {abs(this->x - other.x), abs(this->y - other.y)};
            }

            ///////////////////////Operators/////////////////////////////////

            [[nodiscard]]
            inline bool operator==(const Vector2<Type>& other) const noexcept {
                return ((this->x == other.x) && (this->y == other.y));
            }

            [[nodiscard]]
            inline bool operator!=(const Vector2<Type>& other) const noexcept {
                return ((this->x != other.x) || (this->y != other.y));
            }



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

    typedef Vector2<int32_t> Vector2i;
    typedef Vector2<uint32_t> Vector2ui;
    typedef Vector2<double>  Vector2f;

    class Angle{

        static double constexpr  PI = 3.14159265;

        public:
            // Returns radians in degrees
            constexpr static double RadToDeg(const double rad) noexcept {
                double deg = rad * (180.0/PI);
                return deg;
            };

            // Returns degrees in radians
            constexpr static double DegToRad(const double deg) noexcept {
                double rad = deg * (PI/180.0);
                return rad;
            };
    };


    class Draw{
    public:
        // Receives a function with two int arguments and returns nothing to draw
        using PlotType = std::function<void (int,int)>;   
        PlotType plot_func;
        Draw(PlotType plot_func): plot_func{plot_func}{}
        
        // Draw and line with plot_func using Bresenham's_line_algorithm
        template<typename T>
        void line(const Vector2<T>& init, const Vector2<T>& end) const;
        template<typename T>
        void line(const Vector2<T>& init, int x_end, int y_end) const;
        
        void circle();
    };

}


#endif

