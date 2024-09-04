#include "Lightbulb.h"

int main(int argc, char* argv[])
{
	char* path = "D:\\repos\\vs\\Lightbulb\\Lightbulb\\Lexer.cpp";
	if (argc < 2)
	{
		printf("Please input a filepath as an argument\n");
		//return 0;
	}
	
	char* filedata = ReadFile(path);
	if (filedata == NULL)
	{
		return 1;
	}

	{
		char* tfiledata = Preprocess(filedata);
		free(filedata);
		//CompactInput(tfiledata);
		filedata = tfiledata; 
	}

	//printf("%s\n", filedata);

	TokenArray tokenarray;
	tokenarray = LexInput(filedata);
    if (tokenarray.tokens == NULL)
    {
        printf("Error lexing file\n");
        free(filedata);
        return 1;
    }

	for (int i = 0; i < tokenarray.count; i++)
	{
		//printf("type: %-12s	content: %s\n", TokenTypeToString(tokenarray.tokens[i].type), tokenarray.tokens[i].contents);
	}

	free(filedata);

    // do parsing stuff here

    DeleteTokens(tokenarray);

	return 0;
}

char* ReadFile(char* filepath)
{
    FILE* pfile = fopen(filepath, "rb");
    if (pfile == NULL)
    {
        perror("Error opening file");
        return NULL;
    }

    if (fseek(pfile, 0L, SEEK_END) != 0)
    {
        perror("Error seeking to end of file");
        fclose(pfile);
        return NULL;
    }

    long size = ftell(pfile);
    if (size < 0)
    {
        perror("Error getting file size");
        fclose(pfile);
        return NULL;
    }

    if (fseek(pfile, 0L, SEEK_SET) != 0)
    {
        perror("Error seeking to start of file");
        fclose(pfile);
        return NULL;
    }

    char* filedata = (char*)malloc(sizeof(char) * size + 1);
    if (filedata == NULL)
    {
        perror("Error allocating memory");
        fclose(pfile);
        return NULL;
    }

    size_t bytes_read = fread(filedata, sizeof(char), size, pfile);
    if (bytes_read != size)
    {
        perror("Error reading file");
        free(filedata);
        fclose(pfile);
        return NULL;
    }

    filedata[size] = '\0';

    fclose(pfile);

    for (int i = 0; i < size + 1; i++)
    {
        if (filedata[i] < 1 || filedata[i] > 128)
        {
            filedata[i] = '\0';
            size = i;
            break;
        }
    }

    return filedata;
}