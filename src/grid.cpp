#include <grid.hpp>
#include <fmt/core.h>
#include <array>
#include <random>

static std::array<char, 5> top_block = {218, 196, 196, 196, 191};
static std::array<char, 5> middle_block = {179, 32, 32, 32, 179};
static std::array<char, 5> lower_block = {192, 196, 196, 196, 217};

Grid::Grid(const size_t& rows, const size_t& cols){
    this->x = rows;
    this->y = cols;
    this->states = std::vector<std::vector<State>>(x+1, std::vector<State>(y+1, State::EMPTY));
}

static char get_symbol(const State &state){
    if(state == State::EMPTY) return ' ';
    else if(state == State::DESTROYED) return '#';
    else if(state == State::OCCUPIED) return 'X';
    else if(state == State::PRESENT) return 'O';
}


void Grid::draw(){

    for(size_t row = 1; row <= y; ++row){   
        for(size_t col = 1; col <= x; ++col){
            for(size_t i = 0; i < top_block.size(); ++i)
                fmt::print("{}", top_block[i]);
            fmt::print(" ");
        }

        fmt::print("\n");

        for(size_t col = 1; col <= x; ++col){
            for(size_t i = 0; i < middle_block.size(); ++i){
                if(i == middle_block.size() / 2) 
                    fmt::print("{}", get_symbol(states[row][col]));
                else 
                    fmt::print("{}", middle_block[i]);
            }
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
    fflush(stdout);
}

void Grid::set_grid_random()
{
    auto randfn = [](){
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist5(1,5); // distribution in range [1,5]

        int retval = dist5(rng);
        return retval;
    };

	int i=1;
	while(i<=5)
	{
		int row = randfn();
		int col = randfn();

		if(this->states[row][col] == State::EMPTY)
		{
			this->states[row][col] = State::PRESENT;
			i++;
		}
	}
}

bool Grid::set_grid_manual(int row, int col)
{
    if(this->states[row][col] == State::EMPTY)
    {
        this->states[row][col] = State::PRESENT;
        return true;
    }
    return false;
}