#include <iostream>
#include <utils.h>
#include <HMM.h>
#include <Windows.h>
#include <string>
#include <fmt/core.h>
#include <grid.hpp>

static void set_project_root(){
	char buffer[1024];

	std::filesystem::path temp_path;

	GetModuleFileName(NULL, buffer, sizeof(buffer));

	temp_path = std::string(buffer);
	utils::project_root = temp_path.parent_path().parent_path().parent_path().string();

}

static void clear_screen(){
	system("cls");
}

int main()
{	
	Grid bottom, top;
	while(true){
		fmt::print("{:-^60}\n", " TOP BOARD ");
		
		top.draw();

		fmt::print("{:-^60}\n", "")
		clear_screen();
	}
	return 0;
}