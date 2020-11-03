#ifndef BOAR_ALGORITHMS
#define BOAR_ALGORITHMS

#include <cstdlib>
#include <random>
#include <algorithm>
#include <array>
#include <cstdio>
#include <optional>

using std::vector;

#include "core.hpp"

namespace boar {
    

    class A_Star {

        enum class node_state {
            open,
            closed,
            none
        };

        class Node {

            public:
                
                Vector2ui pos;
                uint32_t h;
                uint8_t g;

                node_state state;

                bool root;
                Node* parent;

                Node(Vector2ui pos, bool root, Node* parent)
                : pos(pos), root(root), parent(parent) {
                    g = 0;
                    h = 0;
                    this->state = node_state::none;
                }

                [[nodiscard]]
                inline uint32_t get_f() const noexcept {
                    return this->g + this->h;
                }

        };

        private:

            uint8_t ORTHOGONAL_COST = 10;
            uint8_t DIAGONAL_COST   = 14;
            
            bool diagonal_move;
            vector<Vector2i> directions;

            Vector2ui* target = nullptr;
            Vector2ui* current = nullptr;

            const uint32_t MAP_SIZE_X;
            const uint32_t MAP_SIZE_Y;

            bool at_side;
            bool add_target_to_result;

            const std::function<bool(Vector2i&)> extern_validade_tile;

            vector<vector<Node*>> internal_nodes;


        public:

            A_Star (bool diagonal_move, uint32_t map_size_x, uint32_t map_size_y,
                    std::function<bool(Vector2i&)> extern_validade_tile)
            : diagonal_move(diagonal_move), MAP_SIZE_X(map_size_x), 
              MAP_SIZE_Y(map_size_y), extern_validade_tile(extern_validade_tile) {

                  this->at_side = false;
                  this->add_target_to_result = false;

                this->directions.push_back({0, -1});
                this->directions.push_back({1, 0});
                this->directions.push_back({0, 1});
                this->directions.push_back({-1, 0});

                if (this->diagonal_move) {
                    this->directions.push_back({1, -1});
                    this->directions.push_back({1, 1});
                    this->directions.push_back({-1, -1});
                    this->directions.push_back({-1, 1});
                }

                this->internal_nodes.resize(this->MAP_SIZE_X);
                for (auto& i: this->internal_nodes) {
                    i.resize(this->MAP_SIZE_Y);
                }
                for (auto& i: this->internal_nodes) {
                    for (uint32_t j = 0; j < this->MAP_SIZE_Y; j++) {
                        i[j] = nullptr;
                    }
                }
            }

            ~A_Star() {
                for (auto& i: this->internal_nodes) {
                    for (auto& j: i) {
                        if (j != nullptr) {
                            delete j;
                        }
                    }
                }
            }

            void set_at_side(bool at_side, bool add_target_to_result) noexcept {
                this->at_side = at_side;
                this->add_target_to_result = add_target_to_result;
            }

            std::optional<vector<Vector2ui>> find(Vector2ui start, Vector2ui target) noexcept {
                this->target = &target;

                bool success = false;
                std::vector<Vector2ui> result;

                {
                    uint8_t i = 0;
                    for (; i < this->directions.size(); i ++) {
                        if (this->validate_neighbor((Vector2i)target + this->directions[i])) {
                            break;
                        }
                    }
                    if (i == this->directions.size()){
                        return {};
                    }
                }

                std::vector<Node*> open_list;
                std::vector<Node*> closed_list;

                {
                    Node* start_node = this->get_node(start, true, nullptr);
                    start_node->g = 0;
                    this->calculate_h(*start_node);
                    
                    start_node->state = node_state::open;
                    open_list.push_back(start_node);
                }

                while (open_list.size() != 0) {

                    std::sort(open_list.begin(), open_list.end(),
                        [](Node* a, Node* b) {
                            return a->get_f() < b->get_f();
                        }
                    );

                    Node* current = open_list.at(0);
                    current->state = node_state::closed;
                    closed_list.push_back(current);
                    open_list.erase(open_list.begin());


                    this->current = &current->pos;

                    if (current->pos == target) {
                        result = this->get_path(current);
                        success = true;
                        goto end;
                    }
                    
                    Vector2ui neighbor_pos {0, 0};
                    Vector2i poss_neighbor_pos {0, 0};
                    Node* neighbor = nullptr;

                    for (uint8_t i = 0; i < this->directions.size(); i++) {
                        
                        poss_neighbor_pos = current->pos + this->directions[i];

                        if (this->at_side && poss_neighbor_pos == target) { 
                            if (this->diagonal_move || current->pos.OrthogonalTo(poss_neighbor_pos)) {
                                
                                result = this->get_path(current);
                                if (this->add_target_to_result) {
                                    result.push_back(target);
                                }
                                success = true;
                                goto end;
                            } 
                        }

                        if (!this->validate_neighbor(poss_neighbor_pos)) {
                            continue;
                        }

                        neighbor_pos = poss_neighbor_pos;

                        if (this->internal_nodes[neighbor_pos.x][neighbor_pos.y] != nullptr) {

                            if (this->internal_nodes[neighbor_pos.x][neighbor_pos.y]->state == node_state::closed) {
                                continue;
                            }

                            else if (this->internal_nodes[neighbor_pos.x][neighbor_pos.y]->state == node_state::open) {

                                neighbor = this->internal_nodes[neighbor_pos.x][neighbor_pos.y];

                                uint8_t mov_cost = this->ORTHOGONAL_COST;
                                if (this->diagonal_move) {
                                    if (current->pos.DiagonalTo(neighbor->pos)) {
                                        mov_cost = DIAGONAL_COST;
                                    }
                                }

                                uint8_t better_g = current->g + mov_cost;
                                if (better_g < neighbor->g) {
                                    neighbor->parent = current;
                                    neighbor->g = better_g;
                                }

                            }
                        }

                        if (this->internal_nodes[neighbor_pos.x][neighbor_pos.y] == nullptr ||
                            this->internal_nodes[neighbor_pos.x][neighbor_pos.y]->state == node_state::none) {

                            neighbor = this->get_node(neighbor_pos, false, current);
                            neighbor->h = this->calculate_h(*neighbor);
                            neighbor->g = this->calculate_g(*current, *neighbor);
                            
                            neighbor->state = node_state::open;
                            open_list.push_back(neighbor);
                            neighbor = nullptr;
                        }
                    }
                }

                end:;

                for (auto& i: open_list) {
                    i->state = node_state::none;
                }
                for (auto& i: closed_list) {
                    i->state = node_state::none;
                }

                if (success)
                    return {result};
                else
                    return {};

            }

        private:

            vector<Vector2ui> get_path(Node* curr) const noexcept {

                vector<Vector2ui> result;
                while (true) {
                    if (curr->root) {
                        return result;
                    }
                    else {
                        result.insert(result.begin(), curr->pos);
                        curr = curr->parent;
                    }
                }

            }

            inline uint32_t calculate_h(const Node& node) const noexcept {

                Vector2i delta {abs(node.pos.x - (*this->target).x), abs(node.pos.y - (*this->target).y)};

                if (this->diagonal_move) {
                    // euclidean
                    return (10 * sqrt(pow(delta.x, 2) + pow(delta.y, 2)));
                }
                else {
                    // manhattan
                    return (10 * (delta.x + delta.y));
                }
            }

            [[nodiscard]]
            inline uint8_t calculate_g(const Node& curr, const Node& node) const noexcept {

                if (this->diagonal_move && (node.pos.x == curr.pos.x || node.pos.y == curr.pos.y)) {
                    return this->ORTHOGONAL_COST + curr.g;
                }
                else {
                    return this->DIAGONAL_COST + curr.g;
                }
            }

            [[nodiscard]]
            inline Node* get_node(Vector2ui& pos, bool root, Node* parent) noexcept {
                
                if (this->internal_nodes[pos.x][pos.y] != nullptr) {
                    Node* node = this->internal_nodes[pos.x][pos.y];
                    node->parent = parent;
                    node->root = root;
                    node->pos = pos;
                    node->state = node_state::none;
                    return node;
                }
                else {
                    Node* new_node = new Node(pos, root, parent);
                    this->internal_nodes[pos.x][pos.y] = new_node;
                    return new_node;
                }

            }
    
            [[nodiscard]]
            inline bool validate_neighbor(Vector2i pos) const noexcept {

                if (pos.x < 0 || (uint32_t)pos.x == this->MAP_SIZE_X || pos.y < 0 || (uint32_t)pos.y == MAP_SIZE_Y) {
                    return false;
                }
                return this->extern_validade_tile(pos);
            }
    
    };


    class PerlinNoise {

        // all of Perlin Noise core math was based on https://github.com/Reputeless/PerlinNoise

        private:

            std::array<uint8_t, 512> p;
            double octave_bias = 2;

        public:

            PerlinNoise(uint32_t seed = 0) {
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