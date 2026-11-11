#include "preprocessor.h"

void handle_include(FILE *src, FILE *dest)
{
    char line[1024];
    char filename[100];
    FILE *fp;
    int ch;

    while (fgets(line, sizeof(line), src))
    {
        // Handle user-defined header files
        if (sscanf(line, "#include \"%99[^\"]\"", filename) == 1)
        {
            fp = fopen(filename, "r");

            if (fp == NULL)
            {
                printf("Error: Cannot open include file %s\n", filename);
                continue;
            }

            while ((ch = fgetc(fp)) != EOF)
            {
                fputc(ch, dest);
            }

            fclose(fp);
        }

        // Preserve system header files
        else
        {
            fputs(line, dest);
        }
    }
}
