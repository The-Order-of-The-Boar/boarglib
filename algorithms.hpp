#ifndef BOAR_ALGORITHMS
#define BOAR_ALGORITHMS

#include <cstdlib>
#include <random>
#include <algorithm>
#include <array>
#include <cstdio>

namespace boar {
    
    class perlin_noise {

        // all of Perlin Noise core math was based on https://github.com/Reputeless/PerlinNoise

        private:

            std::array<uint8_t, 512> p;
            double octave_bias = 2;

        public:

            perlin_noise(uint32_t seed = 0) {
                this->reseed(seed);
            }

            void reseed(uint32_t seed) {

                for (uint16_t i = 0; i < 256; ++i) {
                    this->p[i] = static_cast<uint8_t>(i);
                }

                std::shuffle(std::begin(this->p), std::begin(this->p) + 256, std::default_random_engine(seed));

                for (uint16_t i = 0; i < 256; ++i) {
                    this->p[256 + i] = this->p[i];
                }
            }

        private:

            // ############################################################################
            // #                                                                          #
            // #                               internals                                  #
            // #                                                                          #
            // ############################################################################

            [[nodiscard]]
            static constexpr double fade(const double t) noexcept {
                
                return t * t * t * (t * (t * 6 - 15) + 10);
            }

            [[nodiscard]]
            static constexpr double lerp(const double t, const double a, const double b) noexcept {
                
                return a + t * (b - a);
            }

            [[nodiscard]]
            static constexpr double grad(const std::uint8_t hash, const double x, const double y, const double z) noexcept {
                
                const std::uint8_t h = hash & 15;
                const double u = h < 8 ? x : y;
                const double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
                return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
            }

            [[nodiscard]]
            static constexpr double weight(const std::int32_t octaves) noexcept {
                
                double value = 0;
                double amplitude = 1;

                for (std::int32_t i = 0; i < octaves; ++i) {
                    value += amplitude;
                    amplitude /= 2;
                }

                return value;
            }
        
        public:

            [[nodiscard]]
            double raw_noise(double x = 0, double y = 0, double z = 0) const noexcept {
                
                const std::int32_t X = static_cast<std::int32_t>(std::floor(x)) & 255;
                const std::int32_t Y = static_cast<std::int32_t>(std::floor(y)) & 255;
                const std::int32_t Z = static_cast<std::int32_t>(std::floor(z)) & 255;

                x -= std::floor(x);
                y -= std::floor(y);
                z -= std::floor(z);

                const double u = this->fade(x);
                const double v = this->fade(y);
                const double w = this->fade(z);

                const std::int32_t A = this->p[X] + Y, AA = this->p[A] + Z, AB = this->p[A + 1] + Z;
                const std::int32_t B = this->p[X + 1] + Y, BA = this->p[B] + Z, BB = this->p[B + 1] + Z;

                return this->lerp(
                    w, 
                    this->lerp(
                        v, 
                        this->lerp(u, this->grad(this->p[AA], x, y, z),
                            this->grad(this->p[BA], x - 1, y, z)
                        ),
                        this->lerp(u,
                            this->grad(this->p[AB], x, y - 1, z),
                            this->grad(this->p[BB], x - 1, y - 1, z)
                        )
                    ),
                    this->lerp(v,
                        this->lerp(u, 
                            this->grad(this->p[AA + 1], x, y, z - 1),
                            this->grad(this->p[BA + 1], x - 1, y, z - 1)
                        ),
                        this->lerp(u, 
                            this->grad(this->p[AB + 1], x, y - 1, z - 1),
                            this->grad(this->p[BB + 1], x - 1, y - 1, z - 1)
                        )
                    )
                );
            }
        
            
            // ############################################################################
            // #                                                                          #
            // #                               interfaces                                 #
            // #                                                                          #
            // ############################################################################

            [[nodiscard]]
            double noise1D(double x, const double octv, const double freq, double ampl) const noexcept {
                
                x = x / freq;
                double result = 0;
                double amp = 1;
                for (std::int32_t i = 0; i < octv; ++i) {
                    result += this->raw_noise(x, 0, 0) * amp;
                    x *= octave_bias;
                    amp /= octave_bias;
                }

                return (result / this->weight(octv)) * ampl;
            }
            
            [[nodiscard]]
            double noise2D(double x, double y, const double octv, const double freq, double ampl) const noexcept {
                
                x = x / freq;
                y = y / freq;
                double result = 0;
                double amp = 1;
                for (std::int32_t i = 0; i < octv; ++i) {
                    result += this->raw_noise(x, y, 0) * amp;
                    x *= octave_bias;
                    y *= octave_bias;
                    amp /= octave_bias;
                }

                return (result / this->weight(octv)) * ampl;
            }
            
            [[nodiscard]]
            double noise3D(double x, double y, double z, const double octv, const double freq, double ampl) const noexcept {
                
                x = x / freq;
                y = y / freq;
                z = z / freq;
                double result = 0;
                double amp = 1;
                for (std::int32_t i = 0; i < octv; ++i) {
                    result += this->raw_noise(x, y, z) * amp;
                    x *= octave_bias;
                    y *= octave_bias;
                    z *= octave_bias;
                    amp /= octave_bias;
                }

                return (result / this->weight(octv)) * ampl;
            }


            // ############################################################################ 
            // #                                                                          #
            // #                                   I/O                                    #
            // #                                                                          #
            // ############################################################################


            void serialize(std::array<std::uint8_t, 256>& s) const noexcept {
                for (std::size_t i = 0; i < 256; ++i)
                {
                    s[i] = this->p[i];
                }
            }

            void deserialize(const std::array<std::uint8_t, 256>& s) noexcept {
                for (std::size_t i = 0; i < 256; ++i)
                {
                    this->p[256 + i] = this->p[i] = s[i];
                }
            }

    };
}

#endif
