#include <stdio.h>
#include <string.h>

#define STR_LEN 1000
#define MAX_LINES 1000

char checkSyntax(char stack[][STR_LEN], int lineCount);
char findExpected(char stackChar);

struct ParenthesisInfo
{
    char parenthesis;
    int lineNumber;
};

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
    struct ParenthesisInfo parenthesesStack[STR_LEN];
    int stackIndex = 0;
    int inSingleLineComment = 0;
    int inMultiLineComment = 0;
    int inDoubleQuote = 0;
    int inSingleQuote = 0;

    // Loop through each line
    for (int i = 0; i < lineCount; i++)
    {
        // Loop through each char in current line
        for (int j = 0; j < strlen(stack[i]); j++)
        {
            // Check for single-line comments
            if (!inMultiLineComment && !inDoubleQuote && !inSingleQuote && stack[i][j] == '/' && stack[i][j + 1] == '/')
            {
                break; // Ignore the rest of the line
            }
            // Check for opening multi-line comments
            else if (!inSingleLineComment && !inMultiLineComment && !inDoubleQuote && !inSingleQuote && stack[i][j] == '/' && stack[i][j + 1] == '*')
            {
                inMultiLineComment = 1;
                j++; // Skip the '*' character
            }
            // Check for closing multi-line comments
            else if (inMultiLineComment && !inDoubleQuote && !inSingleQuote && stack[i][j] == '*' && stack[i][j + 1] == '/')
            {
                inMultiLineComment = 0;
                j++; // Skip the '/' character
            }
            // Check for opening double quotes
            else if (!inSingleLineComment && !inMultiLineComment && !inDoubleQuote && !inSingleQuote && stack[i][j] == '"')
            {
                inDoubleQuote = 1;
            }
            // Check for closing double quotes
            else if (!inSingleLineComment && !inMultiLineComment && !inSingleQuote && inDoubleQuote && stack[i][j] == '"' && stack[i][j - 1] != '\\')
            {
                inDoubleQuote = 0;
            }
            else if (!inSingleLineComment && !inMultiLineComment && !inDoubleQuote && !inSingleQuote && stack[i][j] == '\'')
            {
                inSingleQuote = 1;
            }
            // Check for single double quotes
            else if (!inSingleLineComment && !inMultiLineComment && !inDoubleQuote && inSingleQuote && stack[i][j] == '\'' && stack[i][j - 1] != '\\')
            {
                inSingleQuote = 0;
            }
            else if (!inSingleLineComment && !inMultiLineComment && !inDoubleQuote && !inSingleQuote)
            {
                // Check for opening (,{,[
                if (stack[i][j] == '(' || stack[i][j] == '{' || stack[i][j] == '[')
                {
                    parenthesesStack[stackIndex].parenthesis = stack[i][j];
                    parenthesesStack[stackIndex].lineNumber = lineNumber; // Push to stack
                    stackIndex++;
                }
                // Check for closing (,{,[
                else if (stack[i][j] == ')' || stack[i][j] == '}' || stack[i][j] == ']')
                {
                    // Check for matching (,{,[
                    if (stackIndex == 0 ||
                        (stack[i][j] == ')' && parenthesesStack[stackIndex - 1].parenthesis != '(') ||
                        (stack[i][j] == '}' && parenthesesStack[stackIndex - 1].parenthesis != '{') ||
                        (stack[i][j] == ']' && parenthesesStack[stackIndex - 1].parenthesis != '['))
                    {
                        parenthesesStack[stackIndex].parenthesis = stack[i][j];
                        parenthesesStack[stackIndex].lineNumber = lineNumber;
                        stackIndex++;
                    }
                    else
                    {
                        stackIndex--;
                    }
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
            if (parenthesesStack[i].parenthesis != '\0')
            {
                char c = parenthesesStack[i].parenthesis;
                for (size_t j = stackIndex - 1; j > 0; j--)
                {
                    if (parenthesesStack[j].parenthesis == findExpected(c))
                    {
                        stackIndex -= 2;
                        parenthesesStack[i].parenthesis = '\0';
                        parenthesesStack[j].parenthesis = '\0';
                        break;
                    }
                }
            }
        }
        if (stackIndex != 0)
        {
            char missing = ')';
            int line = 0;
            for (size_t i = 0; i <= sizeof(parenthesesStack); i++)
            {
                if (parenthesesStack[i].parenthesis == ')' || parenthesesStack[i].parenthesis == '(' || parenthesesStack[i].parenthesis == '}' || parenthesesStack[i].parenthesis == '{' || parenthesesStack[i].parenthesis == ']' || parenthesesStack[i].parenthesis == '[')
                {
                    missing = findExpected(parenthesesStack[i].parenthesis);
                    line = parenthesesStack[i].lineNumber;
                    break;
                }
            }

            printf("Error: missing a '%c', detected at line %d\n", missing, line);
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
