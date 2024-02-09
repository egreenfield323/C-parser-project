#include <stdio.h>
#include <string.h>

#define STR_LEN 1000
#define MAX_LINES 1000

char checkSyntax(char stack[][STR_LEN], int lineCount);
char findExpected(char stackChar);

int main()
{
    char fileName[STR_LEN];
    char fileContent[STR_LEN];
    char stack[MAX_LINES][STR_LEN] = {{0}};
    int lineCount = 0;
    FILE *file;

    printf("Enter a file name\n");

    scanf("%s", fileName);

    if ((file = fopen(fileName, "r")) == NULL)
    {
        printf("Invalid input: \"%s\"\n", fileName);
        return 1;
    }
    else
    {
        while (fgets(fileContent, sizeof(fileContent), file) != NULL)
        {

            strncpy(stack[lineCount], fileContent, strlen(fileContent));
            lineCount++;
        }

        fclose(file);

        if (lineCount <= 0)
        {
            return 0;
        }
        else
        {
            checkSyntax(stack, lineCount);
        }
    }

    return 0;
}

char checkSyntax(char stack[][STR_LEN], int lineCount)
{
    int lineNumber = 1;
    char parenthesesStack[STR_LEN];
    int stackIndex = 0;

    // Loop through each line
    for (int i = 0; i < lineCount; i++)
    {
        // Loop through each char in current line
        for (int j = 0; j < strlen(stack[i]); j++)
        {
            // Check for comments
            if (stack[i][j] == '/' && stack[i][j + 1] == '/')
            {
                break;
            }
            // Check for opening (,{,[
            else if (stack[i][j] == '(' || stack[i][j] == '{' || stack[i][j] == '[')
            {
                parenthesesStack[stackIndex++] = stack[i][j]; // Push to stack
            }
            // Check for closing (,{,[
            else if (stack[i][j] == ')' || stack[i][j] == '}' || stack[i][j] == ']')
            {
                // Check for matching (,{,[
                if (stackIndex == 0 ||
                    (stack[i][j] == ')' && parenthesesStack[stackIndex - 1] != '(') ||
                    (stack[i][j] == '}' && parenthesesStack[stackIndex - 1] != '{') ||
                    (stack[i][j] == ']' && parenthesesStack[stackIndex - 1] != '['))
                {
                    parenthesesStack[stackIndex++] = stack[i][j];
                }
                else
                {
                    stackIndex--;
                }
            }
        }
        lineNumber++;
    }

    // Check for remaining parentheses
    if (stackIndex != 0)
    {
        for (size_t i = 0; i < stackIndex; i++)
        {
            if (parenthesesStack[i] != '\0')
            {
                char c = parenthesesStack[i];
                for (size_t j = stackIndex - 1; j > 0; j++)
                {
                    if (parenthesesStack[j] == findExpected(c))
                    {
                        stackIndex -= 2;
                        parenthesesStack[i] = '\0';
                        parenthesesStack[j] = '\0';
                        break;
                    }
                }
            }
        }
        if (stackIndex != 0)
        {
            char missing = ')';
            for (size_t i = 0; i <= sizeof(parenthesesStack); i++)
            {
                if (parenthesesStack[i] == ')' || parenthesesStack[i] == '(' || parenthesesStack[i] == '}' || parenthesesStack[i] == '{' || parenthesesStack[i] == ']' || parenthesesStack[i] == '[')
                {
                    missing = findExpected(parenthesesStack[i]);
                    break;
                }
            }

            printf("Error: missing a '%c'\n", missing);
        }

        return 1;
    }

    // No syntax errors found
    return 0;
}

char findExpected(char c)
{
    switch (c)
    {
    case ')':
        return '(';
    case '(':
        return ')';
    case '}':
        return '{';
    case '{':
        return '}';
    case ']':
        return '[';
    case '[':
        return ']';
    default:
        return '\0';
    }
}