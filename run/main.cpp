#include <iostream>
#include <utils.h>
#include <HMM.h>
#include <Windows.h>
#include <string>
#include <fmt/core.h>
#include <grid.hpp>
#include <game.hpp>

static void set_project_root(){
	char buffer[1024];

	std::filesystem::path temp_path;

	GetModuleFileName(NULL, buffer, sizeof(buffer));

	temp_path = std::string(buffer);
	utils::project_root = temp_path.parent_path().parent_path().parent_path().string();

}



int main()
{	
	set_project_root();

	int choice;
	float count = 0;
	Grid top, bottom, pc;

	system("cls");
	fmt::print("{:-^100}", " BATTLESHIP ");
	printf("\n");
	printf("1: To do offline testing\n2: Play the game\nEnter choice:");
	scanf("%d", &choice);
	switch(choice){
		case 1:
			HMM::generate_observation_sequence(stage::TRAIN);

			printf("\n\n");
			fmt::print("{:-^102}", " TRAINING PHASE ");
			printf("\n");
			for(int i=0; i<10; ++i)
				HMM::converge(i);

			fmt::print("{:-^102}", "");
			printf("\n\n");
			HMM::generate_observation_sequence(stage::TEST);

			printf("\n\n");
			fmt::print("{:-^102}", " TESTING PHASE ");
			for(int i=0; i<10; ++i){
				HMM::read_observation_sequence(i, stage::TEST);
				printf("Actual digit is %d\n", i);
				count += HMM::test(i);
			}
			printf("Overall average is %lf%\n", count);
			fmt::print("{:-^102}", "");
			system("PAUSE");
			break;
		case 2:
			game::init_grids(top, bottom, pc);

			fmt::print("{:-^30}\n", " LET'S BEGIN THE GAME! ");

			game::gameplay(top, bottom, pc);
			break;
		default:
			printf("Wrong choice seleted!\n");
	}	
	return 0;
}