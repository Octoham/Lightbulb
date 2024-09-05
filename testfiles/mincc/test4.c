#include <stdio.h>

int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		printf("Please input an argument\n");
	}
	else
	{
		int i = 0;
		i += argv[1];
		i += argv[2];
		printf(i);
	}
	return 0;
}