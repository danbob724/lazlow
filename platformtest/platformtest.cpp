#include <time.h>
#include <iostream>

//this nonsense is to get the current working directory
#include <stdio.h>
#ifdef WIN32
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
#endif

int main() {
	std::cout << "Clocks per second:\t" << CLOCKS_PER_SEC << std::endl;
	clock_t clock0;
	clock_t clock1;
	return 0;
}