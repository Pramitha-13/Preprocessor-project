#include "preprocessor.h"
#include <ctype.h>

struct macro
{
    char name[50];
    char value[200];
    char argument[50];
    int is_function;
};

struct macro macros[100];
int macro_count = 0;

/* Find a macro by name */
int find_macro(char *name)
{
    int i;

    for (i = 0; i < macro_count; i++)
    {
        if (strcmp(macros[i].name, name) == 0)
            return i;
    }

    return -1;
}

/* Store macro definitions */
void store_macro(char *line)
{
    char *ptr;
    char *start;
    char *end;
    int i = macro_count;

    ptr = line + 8;

    while (isspace((unsigned char)*ptr))
        ptr++;

    start = ptr;

    while (isalnum((unsigned char)*ptr) || *ptr == '_')
        ptr++;

    if (start == ptr || macro_count >= 100)
        return;

    snprintf(macros[i].name, sizeof(macros[i].name),
             "%.*s", (int)(ptr - start), start);

    macros[i].is_function = 0;
    macros[i].argument[0] = '\0';

    /* Function-like macro */
    if (*ptr == '(')
    {
        macros[i].is_function = 1;

        ptr++;
        start = ptr;

        while (*ptr && *ptr != ')')
            ptr++;

        if (*ptr != ')')
            return;

        snprintf(macros[i].argument,
                 sizeof(macros[i].argument),
                 "%.*s", (int)(ptr - start), start);

        ptr++;
    }

    while (isspace((unsigned char)*ptr))
        ptr++;

    end = strchr(ptr, '\n');

    if (end)
        *end = '\0';

    snprintf(macros[i].value,
             sizeof(macros[i].value), "%s", ptr);

    macro_count++;
}

/* Replace argument inside function-like macro */
void replace_argument(char *value, char *argument,
                      char *actual, char *result)
{
    int i = 0, j = 0;
    int len = strlen(argument);

    while (value[i] && j < 900)
    {
        if (strncmp(value + i, argument, len) == 0 &&
            (i == 0 || (!isalnum((unsigned char)value[i - 1]) &&
                        value[i - 1] != '_')) &&
            (!isalnum((unsigned char)value[i + len]) &&
             value[i + len] != '_'))
        {
            int k = 0;

            while (actual[k] && j < 900)
                result[j++] = actual[k++];

            i += len;
        }
        else
        {
            result[j++] = value[i++];
        }
    }

    result[j] = '\0';
}

/* Expand macros in a source line */
void expand_macros(char *line, char *output)
{
    int i = 0, j = 0;

    while (line[i] && j < 900)
    {
        if (isalpha((unsigned char)line[i]) || line[i] == '_')
        {
            char token[50];
            int k = 0;
            int index;

            while ((isalnum((unsigned char)line[i]) ||
                    line[i] == '_') && k < 49)
            {
                token[k++] = line[i++];
            }

            token[k] = '\0';

            index = find_macro(token);

            if (index == -1)
            {
                int m = 0;

                while (token[m] && j < 900)
                    output[j++] = token[m++];
            }
            else if (!macros[index].is_function)
            {
                int m = 0;

                while (macros[index].value[m] && j < 900)
                    output[j++] = macros[index].value[m++];
            }
            else
            {
                int pos = i;

                while (isspace((unsigned char)line[pos]))
                    pos++;

                if (line[pos] == '(')
                {
                    char actual[200] = "";
                    char replaced[900];
                    int depth = 1, m = 0;

                    pos++;

                    while (line[pos] && depth > 0)
                    {
                        if (line[pos] == '(')
                            depth++;

                        if (line[pos] == ')')
                            depth--;

                        if (depth > 0 && m < 199)
                            actual[m++] = line[pos];

                        pos++;
                    }

                    actual[m] = '\0';

                    if (depth == 0)
                    {
                        replace_argument(macros[index].value,
                                         macros[index].argument,
                                         actual, replaced);

                        m = 0;

                        while (replaced[m] && j < 900)
                            output[j++] = replaced[m++];

                        i = pos;
                    }
                    else
                    {
                        int m = 0;

                        while (token[m] && j < 900)
                            output[j++] = token[m++];
                    }
                }
                else
                {
                    int m = 0;

                    while (token[m] && j < 900)
                        output[j++] = token[m++];
                }
            }
        }
        else
        {
            output[j++] = line[i++];
        }
    }

    output[j] = '\0';
}

/* Main macro handler */
void handle_macro(FILE *src, FILE *dest)
{
    char line[1024];
    char output[1024];

    while (fgets(line, sizeof(line), src))
    {
        if (strncmp(line, "#define", 7) == 0 &&
            isspace((unsigned char)line[7]))
        {
            store_macro(line);
        }
        else
        {
            expand_macros(line, output);
            fputs(output, dest);
        }
    }
}
