// SO IS1 213A LAB07
// Monika Rozmarynowska
// rm44478@zut.edu.pl
#include <io.h>
#include <sys\stat.h>
#include <sys\types.h>
#include <fcntl.h>
#include <string.h>
#include <windows.h>
#include <unistd.h> //getopt()
#include <time.h> //ctime()

int main(int argc, char *argv[])
{
	int opt = getopt(argc, argv, ":w:r");
	//OptString description:
	//first colon (at the beginning of OptString) says - return ':' instead of '?' if parameter didn't get an argument 
	//colon after 'w' means - this parameter takes an argument

	if (opt == ':')
	{
		write(2, "Error: -w <string> required",strlen("Error: -w <string> required"));
		return 1;
	}
	if (opt == '?')
	{
		write(2, "Unknown parameter!", strlen("Unknown parameter!"));
		return 1;
	}
	if (opt == -1)
	{
		write(2, "-w <string> or -r parameter required!", strlen("-w <string> or -r parameter required!"));
		return 1;
	}
	if (opt == 'w')
	{
		write(1, optarg, strlen(optarg));
	}
	if (opt == 'r')
	{

	}

	return 0;
}