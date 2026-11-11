#include "preprocessor.h"

int main(int argc, char *argv[])
{
    FILE *src, *temp1, *temp2, *dest;
    char output[100];

    if (argc != 2)
    {
        printf("Usage: ./mypreprocessor <filename.c>\n");
        return 1;
    }

    src = fopen(argv[1], "r");

    if (src == NULL)
    {
        printf("Error: Cannot open input file\n");
        return 1;
    }

    // Create temporary files
    temp1 = fopen("temp1.txt", "w+");
    temp2 = fopen("temp2.txt", "w+");

    if (temp1 == NULL || temp2 == NULL)
    {
        printf("Error creating temporary files\n");
        fclose(src);

        if (temp1 != NULL)
            fclose(temp1);

        if (temp2 != NULL)
            fclose(temp2);

        return 1;
    }

    // Step 1: Remove comments
    remove_comments(src, temp1);

    rewind(temp1);

    // Step 2: Handle file inclusion
    handle_include(temp1, temp2);

    rewind(temp2);

    // Step 3: Handle macro substitution
    snprintf(output, sizeof(output), "%s", argv[1]);

    char *dot = strrchr(output, '.');

    if (dot != NULL)
        *dot = '\0';

    strcat(output, ".i");

    dest = fopen(output, "w");

    if (dest == NULL)
    {
        printf("Error creating output file\n");

        fclose(src);
        fclose(temp1);
        fclose(temp2);

        return 1;
    }

    handle_macro(temp2, dest);

    printf("Preprocessing completed successfully!\n");
    printf("Output file: %s\n", output);

    fclose(src);
    fclose(temp1);
    fclose(temp2);
    fclose(dest);

    remove("temp1.txt");
    remove("temp2.txt");

    return 0;
}
