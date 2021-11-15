#include <iostream>
#include <array>
#include <fmt/core.h>

std::array<char, 5> top_block = {218, 196, 196, 196, 191};
std::array<char, 5> middle_block = {179, 32, 32, 32, 179};
std::array<char, 5> lower_block = {192, 196, 196, 196, 217};

void draw(const size_t &x, const size_t &y){

    for(size_t row = 1; row <= y; ++row){   
        for(size_t col = 1; col <= x; ++col){
            for(size_t i = 0; i < top_block.size(); ++i)
                fmt::print("{}", top_block[i]);
            fmt::print(" ");
        }

        fmt::print("\n");

        for(size_t col = 1; col <= x; ++col){
            for(size_t i = 0; i < middle_block.size(); ++i)
                fmt::print("{}", middle_block[i]);
            fmt::print(" ");
        }

        fmt::print("\n");

        for(size_t col = 1; col <= x; ++col){
            for(size_t i = 0; i < lower_block.size(); ++i)
                fmt::print("{}", lower_block[i]);
            fmt::print(" ");
        }

        fmt::print("\n");
    }



}

int main()
{   
    draw(10, 10);
    return 0;
}

