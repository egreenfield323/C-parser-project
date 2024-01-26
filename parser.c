#include <stdio.h>
#include <string.h>

#define STR_LEN 100000

char checkSyntax(char stack[]);

int main()
{
    char fileName[STR_LEN];
    char fileContent[STR_LEN];
    char stack[STR_LEN];
    FILE *file;

    printf("Enter a file name\n");

    scanf("%s", fileName);

    if ((file = fopen(fileName, "r")) == NULL)
    {
        printf("Invalid input: \"%s\"\n", fileName);
    }
    else
    {
        while (fgets(fileContent, sizeof(fileContent), file) != NULL)
        {
            for (size_t i = 0; i < strlen(fileContent); i++)
            {
                stack[i] += fileContent[i];
            }
            checkSyntax(stack);
        }
        fclose(file);
    }

    return 0;
}

char checkSyntax(char stack[])
{
    printf("%s\n", &stack[0]);
    int parenthesis = 0;
    for (int i = 0; i < STR_LEN; i++)
    {
        if (stack[i] == '(')
        {
            parenthesis++;
        }
    }
    printf("%d", parenthesis);
}