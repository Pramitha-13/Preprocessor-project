#include "preprocessor.h"

void remove_comments(FILE *src, FILE *dest)
{
    int ch, next;

    while ((ch = fgetc(src)) != EOF)
    {
        if (ch == '/')
        {
            next = fgetc(src);

            // Single-line comment
            if (next == '/')
            {
                while ((ch = fgetc(src)) != EOF && ch != '\n');

                if (ch == '\n')
                    fputc('\n', dest);
            }

            // Multi-line comment
            else if (next == '*')
            {
                while ((ch = fgetc(src)) != EOF)
                {
                    if (ch == '*')
                    {
                        next = fgetc(src);

                        if (next == '/')
                            break;
                    }
                }
            }

            else
            {
                fputc('/', dest);

                if (next != EOF)
                    ungetc(next, src);
            }
        }
        else
        {
            fputc(ch, dest);
        }
    }
}
