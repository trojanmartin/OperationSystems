#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define MAX_TOKENS 100
#define MAX_TOKEN_SIZE 20

char tokens[MAX_TOKENS][MAX_TOKEN_SIZE];
char *args[MAX_TOKENS];

int fillStream();
int procces();
int tokenize(char *inputStream, int length, int startRow);
void parse(int count);
void clearToknes();
void clearArgs(int count);
void cleanAfterCmd(int count, int);
int isSpecialChar(char symbol);
void print(char *arr[], int count);
int runCommand(char *command, char **argv);
void createArgs(int indexLstArg);
int getIndexOfSpecialChar(int count);
void redirectOutput(int index);
void redirectInput(int index);

int main(int args, char *argv[])
{

    int pid = fork();

    if (pid > 0)
    {

        while (1)
        {
            printf("@");
            wait(0);
            pid = fork();

            if (pid == 0)
                break;
        }
    }

    if (pid == 0)
    {
        procces();
    }

    exit(0);
}

int fillStream(char inputStream[MAX_TOKEN_SIZE])
{
    int i = 0;
    while (read(0, &inputStream[i++], sizeof(char)) > 0)
    {
        
        if (inputStream[i - 1] == '\n')
        {
            break;
        }
    }    
    return i;
}

int procces()
{
    char inputStream[MAX_TOKEN_SIZE];
    int length = fillStream(inputStream);
    close(0);
    int count = tokenize(inputStream, length,0);
    parse(count);
    clearToknes(count);
    return 0;
}

int tokenize(char *inputStream, int length, int startRow)
{

    int j = startRow;
    int k = 0;

    for (int i = 0; i < length; i++)
    {
        if (isSpecialChar(inputStream[i]) == 0)
        {
            tokens[j][k++] = inputStream[i];
        }
        else if (isSpecialChar(inputStream[i]) == 1)
        {
            k = 0;
            tokens[j][k] = inputStream[i];
        }
        else if (isSpecialChar(inputStream[i]) == -1)
        {
            if (isSpecialChar(inputStream[i + 1]) == -1)
            {
                continue;
            }
            else if (isSpecialChar(inputStream[i + 1]) != 2)
            {

                j++;
                k = 0;
            }
        }
    }
    return j + 1;
}

void parse(int count)
{

    int index = getIndexOfSpecialChar(count);

    int sysFD = -1;

    if (index != -1)
    {

        switch (tokens[index][0])
        {
        case '>':
            redirectOutput(index);
            sysFD = 1;
            break;

        case '<':       
            redirectInput(index);
            sysFD = 0;
            break;

        default:
            break;
        }

        createArgs(index);
    }
    else
    {
        createArgs(count);
    }

    runCommand(tokens[0], args);
    cleanAfterCmd(count, sysFD);
}

void redirectOutput(int index)
{

    int fd = open(tokens[index + 1], O_WRONLY | O_CREATE);

    close(1);
    dup(fd);
    close(fd);
}

void redirectInput(int index)
{
    
    
    int fd = open(tokens[index + 1], O_RDONLY);
    printf("%", fd);
   
}

void createArgs(int count)
{
    char inputStream[MAX_TOKEN_SIZE];
    int lastArg = count;
    int length = fillStream(inputStream);
  

    int specialIndex = (getIndexOfSpecialChar(count) == -1) ? count : getIndexOfSpecialChar(count);     
    if (length <= 1)
    {


        for (int i = 0; i < specialIndex  ; i++)
        {
            args[i] = &tokens[i][0];
        }       
    }
    else
    {

         
        lastArg = tokenize(inputStream, length, count + 1);


        int a = 0;
        args[a++] = &tokens[1][0];

        for (int i = specialIndex+1 ; i < lastArg; i++)
        {
           

            args[a++] = &tokens[i][0];

            
        }
        
        
        
    }
    args[lastArg] = 0;
}

int runCommand(char *command, char **argv)
{
    return exec(command, argv);
}

int getIndexOfSpecialChar(int count)
{
    for (int i = 0; i < count; i++)
    {
        if (isSpecialChar(tokens[i][0]) == 1)
            return i;
    }
    return -1;
}

void cleanAfterCmd(int count, int fdToClose)
{
    clearArgs(count);

    if (fdToClose != -1)
    {
        close(fdToClose);
    }
}

void clearArgs(int count)
{

    for (int i = 0; i < count; i++)
    {
        args[i] = 0;
    }
}

int isSpecialChar(char symbol)
{

    switch (symbol)
    {
    case '>':
        return 1;
        break;

    case '<':
        return 1;
        break;

    case '|':
        return 1;
        break;

    case ' ':
        return -1;
        break;

    case '\n':
        return 2;
        break;

    default:
        return 0;
        break;
    }
}

void clearToknes(int count)
{

    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < MAX_TOKEN_SIZE; j++)
        {
            tokens[i][j] = 0;
        }
    }
}

void print(char *arr[], int count)
{
    for (int i = 0; i < count; i++)
    {
        printf("%s \n", arr[i]);
    }
}