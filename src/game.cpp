#include <game.hpp>
#include <iostream>
#include <utils.h>
#include <HMM.h>
#include <Windows.h>
#include <string>
#include <fmt/core.h>
#include <stdlib.h>
#include <random>

static void clear_screen(){
	system("cls");
}

void game::init_grids(Grid &top, Grid &bottom, Grid &pc)
{
	clear_screen();

	pc.set_grid_random();

	int row=1,col=1,i=1;
	char ch;

	fmt::print("{:=^30}\n"," SETTING YOUR BOARD ");

	fmt::print("{:-^30}\n", " BOTTOM BOARD ");
	bottom.draw();
	fmt::print("{:-^30}\n", "");

	while(i<=5)
	{
		// std::cout<<"Row Number is: ";
		// std::cin>>row;

		std::cout << "\nSpeak the row number [1-5]\n";
		system("PAUSE");
		do{
			row = HMM::perform_online_test();
			std::cout << "Have you said " << row << " ? (Y/N/M) ";
			std::cin >> ch;
		}while(ch == 'n' || ch == 'N');
		if(ch == 'm' || ch == 'M'){
			std::cout << "Enter row number manually: ";
			std::cin >> row;
		}


		if(row>5 || row==0)
		{
			std::cout<<"Invalid input.\n";
			system("PAUSE");
			continue;
		}

		// std::cout<<"Col Number is: ";
		// std::cin>>col;
		std::cout << "\nSpeak the column number [1-5]\n";
		system("PAUSE");
		do{
			col = HMM::perform_online_test();
			std::cout << "Have you said " << col << " ? (Y/N/M) ";
			std::cin >> ch;
		}while(ch == 'n' || ch == 'N');
		if(ch == 'm' || ch == 'M'){
			std::cout << "Enter col number manually: ";
			std::cin >> col;
		}

		if(col>5 || col==0)
		{
			std::cout<<"Invalid input.\n";
			system("PAUSE");
			continue;
		}

		if(bottom.set_grid_manual(row,col) == false)
		{
			std::cout<<"Ship already present.\n";
			system("PAUSE");
			continue;
		}
		else
		{
			std::cout<<"Ship added!\n";
			i++;
		}

		clear_screen();
	
		/*
		fmt::print("{:-^30}\n", " TOP BOARD ");
		top.draw();
		fmt::print("{: ^30}\n", "");
		*/

		fmt::print("{:-^30}\n", " BOTTOM BOARD ");
		bottom.draw();
		fmt::print("{:-^30}\n", "");
	}
}

void game::gameplay(Grid &top, Grid &bottom, Grid &pc)
{
	auto randfn = [](){
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist5(1,5); // distribution in range [1,5]

        int retval = dist5(rng);
        return retval;
    };

	bool gameover = false;

	int death_count_pc = 0;
	int death_count_user = 0;

	while(!gameover)
	{
		clear_screen();

		fmt::print("{:-^30}\n", fmt::format(" TOP BOARD [{}] ", death_count_pc));
		top.draw();
		fmt::print("{: ^30}\n", "");

		fmt::print("{:-^30}\n", fmt::format(" BOTTOM BOARD [{}] ", death_count_user));
		bottom.draw();
		fmt::print("{:-^30}\n", "");

		int row=1,col=1;
		char ch;

		//FOR USER
		fmt::print("{: ^30}\n", " USER'S CHANCE ");
		std::cout << "\nSpeak the row number\n";
		system("PAUSE");
		do{
			row = HMM::perform_online_test();
			std::cout << "Have you said " << row << " ? (Y/N/M) ";
			std::cin >> ch;
		}while(ch == 'n' || ch == 'N');
		if(ch == 'm' || ch == 'M'){
			std::cout << "Enter row number manually: ";
			std::cin >> row;
		}

		if(row>5 || row==0)
		{
			std::cout<<"Invalid input.\n";
			system("PAUSE");
			continue;
		}

		// std::cout<<"Col Number is: ";
		// std::cin>>col;
		std::cout << "\nSpeak the column number\n";
		system("PAUSE");
		do{
			col = HMM::perform_online_test();
			std::cout << "Have you said " << col << " ? (Y/N/M) ";
			std::cin >> ch;
		}while(ch == 'n' || ch == 'N');
		if(ch == 'm' || ch == 'M'){
			std::cout << "Enter col number manually: ";
			std::cin >> col;
		}

		//USER status message
		clear_screen();
		if(col>5 || col==0)
		{
			std::cout<<"Invalid input.\n";
			system("PAUSE");
			continue;
		}

		if(top.states[row][col] == State::OCCUPIED || top.states[row][col] == State::DESTROYED)
		{
			std::cout<<"You already accessed the square!\n";
			system("PAUSE");
			continue;
		}

		if(pc.states[row][col] == State::PRESENT)
		{
			std::cout<<"It's a hit!!\n";
			pc.states[row][col] = State::DESTROYED;
			top.states[row][col] = State::DESTROYED;
			death_count_pc++;
			system("PAUSE");
		}
		else if(pc.states[row][col] == State::EMPTY)
		{
			std::cout<<"It's a miss :(\n";
			pc.states[row][col] = State::OCCUPIED;
			top.states[row][col] = State::OCCUPIED;
			system("PAUSE");
		}

		if(death_count_pc == 5)
		{
			gameover = true;
			fmt::print("{: ^30}\n", " CONGRATULATIONS! You have won :) ");
			system("PAUSE");
			return;
		}

		//FOR PC
		clear_screen();
		fmt::print("{: ^30}\n", " COMPUTER'S CHANCE ");

		int flag = 0;
		while(flag == 0)
		{
			row = randfn();
			col = randfn();

			if(row>5 || row==0)
				continue;
			
			if(col>5 || col==0)
				continue;

			if(bottom.states[row][col] == State::OCCUPIED || bottom.states[row][col] == State::DESTROYED)
				continue;
			
			flag = 1;
		}


		//PC Status Message
		if(bottom.states[row][col] == State::PRESENT)
		{
			std::cout<<"The computer has hit you :(\n";
			bottom.states[row][col] = State::DESTROYED;
			death_count_user++;
			system("PAUSE");
		}
		else if(bottom.states[row][col] == State::EMPTY)
		{
			std::cout<<"The computer has missed :)\n";
			bottom.states[row][col] = State::OCCUPIED;
			system("PAUSE");
		}

		if(death_count_user == 5)
		{
			gameover = true;
			fmt::print("{: ^30}\n", " You have lost :(  Better luck next time. ");
			system("PAUSE");
			return;
		}
	}
}