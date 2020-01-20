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
#include <share.h>

#include <stdio.h>

//struct FileData
//{
//	CHAR FileName;
//	FILETIME WriteTime;
//};

struct Log
{
	struct tm date;
	char argument[10];
};

//nie sprawdzam, czy nie przekroczylam tablicy (czy nie pisze po pamieci)!!
void IntToStr(int n, char * result)
{
	int temporaryIterator = 0;
	int resultIterator = 0;
	char temp[10];
	
	if (n < 0)
	{
		result[resultIterator++] = '-';
		n = -n;
	}
	 if (n >= 0 && n <= 9)
	{
		result[resultIterator++] = '0';
		result[resultIterator++] = n % 10 + 48;
		result[resultIterator] = '\0';
		return;
	}
	do
	{
		temp[temporaryIterator] = n % 10 + 48;
		temporaryIterator++;
		
		n = n - n % 10;
		n = n / 10;
	} while (n);
	while(temporaryIterator>=0)
	{
		
		result[resultIterator] = temp[temporaryIterator-1];
		resultIterator++;
		temporaryIterator--;
	}
	result[resultIterator] = '\0';
}

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
		struct Log log;
		struct tm* date;
		//write(1, optarg, strlen(optarg));
		//przygotowanie argumentu
		strncpy(log.argument, optarg, 9);
		log.argument[9] = '\0';
		//write(1, log.argument, strlen(log.argument));

		//pobranie aktualnego czasu
		time_t rawTime;
		time(&rawTime);
		date = localtime(&rawTime);
		log.date = *date;
		/*printf("Current local time and date: %s", asctime(&(log.date)));*/
		//printf("%d", date->tm_year+1900);

		//tworzenie napisu z nazwna szukanego pliku
		char* fileName = malloc(sizeof(char) * 21);
		char* buffer = malloc(sizeof(char) * 10);
		char* dot = malloc(sizeof(char) * 2);
		char* extension = malloc(sizeof(char) * 4);

		dot[0] = '.';
		dot[1] = '\0';

		extension[0] = 'l';
		extension[1] = 'o';
		extension[2] = 'g';
		extension[3] = '\0';

		//rok
		IntToStr(date->tm_year+1900, buffer);
		strcat(fileName, buffer);
		strcat(fileName, dot);

		//miesiac
		IntToStr(date->tm_mon + 1, buffer);
		strcat(fileName, buffer);
		strcat(fileName, dot);

		//dzien
		IntToStr(date->tm_mday, buffer);
		strcat(fileName, buffer);
		strcat(fileName, dot);

		//godzina
		IntToStr(date->tm_hour, buffer);
		strcat(fileName, buffer);
		strcat(fileName, dot);

		//minuty
		IntToStr(date->tm_min, buffer);
		strcat(fileName, buffer);
		strcat(fileName, dot);

		//rozszerzenie
		strcat(fileName, extension);
		//printf("%s", fileName);

		////tworzenie napisu-zawartosci pliku
		//char* content = malloc(sizeof(char) * 50);
		//char* newLine = malloc(sizeof(char) * 2);
		//newLine[0] = '\n';
		//newLine[1] = '\0';

		//strncat(content, asctime(&(log.date)),strlen(asctime(&(log.date)))-1);
		////strncat(content, fileName,strlen(fileName)-4);
		//strcat(content, " ");
		//strcat(content, log.argument);
		//strcat(content, newLine);
		//printf("%s", content);

		//praca na pliku
		int descriptor = _open(fileName, _O_WRONLY | _O_CREAT | _O_BINARY | _O_APPEND, _S_IREAD | _S_IWRITE);
		if (descriptor < 0) _write(2, "Open file error!\n", strlen("Open file error!\n"));
		//_setmode(descriptor, _O_BINARY);
		//FILE * f = fopen(fileName, "awb");

		int wr = _write(descriptor, &log, sizeof(struct Log));
		//printf("Written: %d", wr);
		if (wr<0) _write(2, "Write file error!\n", strlen("Write file error!\n"));
		//fwrite(content, sizeof(char), strlen(content), f);

		int cl = _close(descriptor);
		if(cl<0) _write(2, "Close file error!\n", strlen("Close file error!\n"));
		//fclose(f);

		//zwolnienie zaalokowanej pamieci
		free(fileName);
		free(buffer);
		free(dot);
		//free(content);
		//free(newLine);
	}
	if (opt == 'r')
	{
		WIN32_FIND_DATAA findFileData;
		HANDLE hFindFile;
		//struct FileData* fileDataArray = malloc(sizeof(struct FileData) * 20); //it means that my app require max 20 files in the directory
		//struct FileData fileDataArray[35];
		struct Log log;
		int fileDataArrayIterator = 0;

		//retreving info about all (max 20) *.log files
		hFindFile = FindFirstFileA("*.log", &findFileData);
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			_write(2, "Searching for latest file error - file doesn't exist!\n", strlen("Searching for latest file error - file doesn't exist!\n"));
			return 1;
		}
		if (hFindFile == INVALID_HANDLE_VALUE)
		{
			_write(2, "Searching for latest file error - wrong file name!\n", strlen("Searching for latest file error - wrong file name!\n"));
			return 1;
		}
		
		//else
		//{
		//	fileDataArray[fileDataArrayIterator].FileName = *(findFileData.cFileName);
		//	fileDataArray[fileDataArrayIterator].WriteTime = findFileData.ftLastWriteTime;
		//	/*printf("przed");
		//	printf("%s", &(fileDataArray[fileDataArrayIterator].FileName));
		//	printf("po");*/
		//	fileDataArrayIterator++;
		//}

		do
		{
			/*printf("%s\n", findFileData.cFileName);
			fileDataArray[fileDataArrayIterator].FileName = *(findFileData.cFileName);
			fileDataArray[fileDataArrayIterator].WriteTime = findFileData.ftLastWriteTime;
			fileDataArrayIterator++;*/

		} while (FindNextFile(hFindFile, &findFileData) != 0);
		////////////////////////////////////////////////////

		////sorting fileDataArray by WriteTime
		//int j;
		//struct FileData temp;
		//for (int i = 1; i < fileDataArrayIterator; i++)
		//{
		//	//wstawienie elementu w odpowiednie miejsce
		//	temp = fileDataArray[i]; //ten element bêdzie wstawiony w odpowiednie miejsce
		//	j = i - 1;

		//	//przesuwanie elementów wiêkszych od pom
		//	while (j >= 0 && CompareFileTime(&(fileDataArray[j].WriteTime), &(temp.WriteTime)))
		//	{
		//		fileDataArray[j + 1] = fileDataArray[j]; //przesuwanie elementów
		//		--j;
		//	}
		//	fileDataArray[j + 1] = temp; //wstawienie pom w odpowiednie miejsce
		//}
		//////////////////////////////////////////////////////

		FindClose(hFindFile);
		printf("%s", findFileData.cFileName);
		int descriptor = _open(findFileData.cFileName, _O_RDONLY | _O_BINARY);
		if (descriptor < 0) _write(2, "Open file error!\n", strlen("Open file error!\n"));

		//printf("size of struct Log: %d", sizeof(struct Log));
		int ls = lseek(descriptor, (-1)*(int)sizeof(struct Log), SEEK_END);
		//int ls = lseek(descriptor, 0, SEEK_END);
		//printf("Current position: %d", ls);
		if(ls<0)_write(2, "Seek error!\n", strlen("Seek error!\n"));
		

		int rd = read(descriptor, &log, sizeof(struct Log));
		//printf("Read: %d", rd);
		if (rd < 0) _write(2, "Read file error!\n", strlen("Read file error!\n"));
		
		int cl = _close(descriptor);
		if (cl < 0) _write(2, "Close file error!\n", strlen("Close file error!\n"));
		
		//char* content = malloc(sizeof(char) * 100);
		//printf("%s", asctime(&(log.date)));
		//printf("size of log: %d", sizeof(log));
		/*strncat(content, asctime(&(log.date)), strlen(asctime(&(log.date))) - 1);
		printf("ok");
		strcat(content, " ");
		
		strcat(content, log.argument);*/
		

		write(1, asctime(&(log.date)), strlen(asctime(&(log.date))));
		write(1, log.argument, strlen(log.argument));

		//free(content);
	}

	return 0;
}