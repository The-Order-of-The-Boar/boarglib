#ifndef BOAR_CORE
#define BOAR_CORE


#include <cmath>
#include <functional>
#include <cstdint>
#include <type_traits>

using std::sqrt;
using std::pow;
using std::atan;

namespace boar{
    
    //Mathematical Bidimensional Vector
    template<typename Type, typename = std::enable_if_t<std::is_arithmetic_v<Type>>>
    class Vector2{

        public:

            Type x;
            Type y;

            //Init vector with X and Y coordinates
            Vector2(Type x, Type y): x{x}, y{y} {};

            ///////////////////////Mathematical Methods/////////////////////////////////

            //Returns diagonal distance between vectors
            inline double DistanceTo(const Vector2& target) const noexcept {
                // const double DistX = (double)target.x - (double)this->x;
                // const double DistY = (double)target.y - (double)this->y;
                return sqrt(pow(((double)target.x - (double)this->x), 2) + pow(((double)target.y - (double)this->y), 2));
            }
            
            // Returns the angle between two vectors in radians
            inline double AngleTo(const Vector2& target) const noexcept {  
                // const double DistX = (double)target.x - (double)this->x;
                // const double DistY = (double)target.y - (double)this->y;
                return atan(((double)target.y - (double)this->y) / ((double)target.x - (double)this->x));
            }


            //Returns the dot product of two vectors
            inline double DotProduct(const Vector2& target) const noexcept {
                return ((double)this->x * (double)target.x) + ((double)this->y * (double)target.y);
            }

            //Returns the magnitude of the Vector2
            [[nodiscard]]
            inline double Magnitude() const noexcept{
                return this->DistanceTo(Vector2<Type>{0,0});
            }

            //Returns the Vector2 normalized
            [[nodiscard]]
            inline Vector2<double> Normalized() const noexcept{
                const auto magnitude = this->Magnitude();
                return Vector2<double>{this->x/magnitude, this->y/magnitude};

            }
            
            [[nodiscard]]
            inline bool OrthogonalTo(const Vector2<Type>& other) const noexcept {
                return (this->x == other.x || this->y == other.y);
            }

            [[nodiscard]]
            inline bool DiagonalTo(const Vector2<Type>& other) const noexcept {
                return (this->x != other.x && this->y != other.y);
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
            inline Vector2<Type> operator*(const Vector2<Type>& target) const  noexcept {
                return Vector2<Type>(this->x*target.x,this->y*target.y);
            }

            [[nodiscard]]
            inline Vector2<Type> operator/(const Vector2<Type>& target) const noexcept {
                return Vector2<Type>(this->x/target.x,this->y/target.y);
            }

            inline Vector2<Type>& operator+=(const Vector2<Type>& other) const noexcept {
                this->x += other.x;
                this->y += other.y;
                return *this;
            }
            
            inline Vector2<Type>& operator-=(const Vector2<Type>& other) const noexcept {
                this->x -= other.x;
                this->y -= other.y;
                return *this;
            }

            inline Vector2<Type>& operator*=(const Vector2<Type>& other) const noexcept {
                this->x *= other.x;
                this->y *= other.y;
                return *this;
            }

            inline Vector2<Type>& operator/=(const Vector2<Type>& other) const noexcept {
                this->x /= other.x;
                this->y /= other.y;
                return *this;
            }
            
            ///////////////////////cast/////////////////////////////////

            inline operator Vector2<double>() const noexcept {
                return Vector2<double>{(double)this->x, (double)this->y};
            }

            inline operator Vector2<int32_t>() const noexcept {
                return Vector2<int32_t>{(int32_t)this->x, (int32_t)this->y};
            }

            inline operator Vector2<uint32_t>() const noexcept {
                return Vector2<uint32_t>{(uint32_t)this->x, (uint32_t)this->y};
            }

            inline operator Vector2<int64_t>() const noexcept {
                return Vector2<int64_t>{(int64_t)this->x, (int64_t)this->y};
            }

            // SFML compatibility
            #ifdef SFML_VECTOR2_HPP

                inline operator sf::Vector2i() const noexcept {
                    return sf::Vector2i{(int)this->x, (int)this->y};
                }

                inline operator sf::Vector2f() const noexcept {
                    return sf::Vector2f{(float)this->x, (float)this->y};
                }

                inline operator sf::Vector2u() const noexcept {
                    return sf::Vector2u{(unsigned int)this->x, (unsigned int)this->y};
                }

            #endif


    };

    using Vector2i  = Vector2<int32_t>;
    using Vector2ui = Vector2<uint32_t>;
    using Vector2f  = Vector2<double>;
    using Vector2li = Vector2<int64_t>;

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

