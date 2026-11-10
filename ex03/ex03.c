#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <io.h>
#include <direct.h>
#include <sys/stat.h>

void cauta(char *director, char *tipar)
{
    struct _finddata_t f;
    intptr_t h;
    char cale[300];

    sprintf(cale, "%s\\%s", director, tipar);
    h = _findfirst(cale, &f);

    if (h != -1)
    {
        do
        {
            if (!(f.attrib & _A_SUBDIR))
            {
                sprintf(cale, "%s\\%s", director, f.name);
                printf("%s\n", cale);
            }
        } while (_findnext(h, &f) == 0);

        _findclose(h);
    }

    sprintf(cale, "%s\\*.*", director);
    h = _findfirst(cale, &f);

    if (h != -1)
    {
        do
        {
            if ((f.attrib & _A_SUBDIR) &&
                strcmp(f.name, ".") != 0 &&
                strcmp(f.name, "..") != 0)
            {
                sprintf(cale, "%s\\%s", director, f.name);
                cauta(cale, tipar);
            }
        } while (_findnext(h, &f) == 0);

        _findclose(h);
    }
}

int main(int argc, char *argv[])
{
    char director[300];

    if (argc != 2)
        return 1;

    _getcwd(director, sizeof(director));

    cauta(director, argv[1]);

    return 0;
}
