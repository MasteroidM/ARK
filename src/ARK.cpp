#include "ARK.h"

int main(int argc, char* args[])
{
	Status status = CreateWindow();

	if (status == SUCCESS)
		std::cout << "Success" << std::endl;
	else
		std::cout << "Failure" << std::endl;

	return 0;
}