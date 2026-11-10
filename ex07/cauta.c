#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <io.h>
#include <direct.h>
#include <sys/stat.h>
#include <fcntl.h>

int cautaText(char *fisier, char *text)
{
    FILE *f;
    char linie[1024];
    char *p;
    int nr = 0;

    f = fopen(fisier, "r");

    if (f == NULL)
        return 0;

    while (fgets(linie, sizeof(linie), f) != NULL)
    {
        p = linie;

        while ((p = strstr(p, text)) != NULL)
        {
            nr++;
            p++;
        }
    }

    fclose(f);

    return nr;
}

void cauta(char *director, char *ext, char *text, int minim)
{
    struct _finddata_t f;
    intptr_t h;
    char cale[300];
    char nume[100];
    char *punct;

    sprintf(cale, "%s\\*.*", director);
    h = _findfirst(cale, &f);

    if (h == -1)
        return;

    do
    {
        if (strcmp(f.name, ".") == 0 || strcmp(f.name, "..") == 0)
            continue;

        sprintf(cale, "%s\\%s", director, f.name);

        if (f.attrib & _A_SUBDIR)
        {
            cauta(cale, ext, text, minim);
        }
        else
        {
            punct = strrchr(f.name, '.');

            if (punct != NULL && strcmp(punct, ext) == 0)
            {
                if (cautaText(cale, text) >= minim)
                    printf("%s\n", cale);
            }
        }

    } while (_findnext(h, &f) == 0);

    _findclose(h);
}

int main(int argc, char *argv[])
{
    char director[300];
    char ext[100];
    char *text;
    int minim = 1;

    if (argc < 3 || argc > 4)
        return 1;

    if (argc == 4)
    {
        if (argv[1][0] == '-' && argv[1][1] == 'c')
            minim = atoi(argv[1] + 2);
        else
            return 1;

        if (minim < 1)
            return 1;

        strcpy(ext, argv[2]);
        text = argv[3];
    }
    else
    {
        strcpy(ext, argv[1]);
        text = argv[2];
    }

    if (ext[0] != '.')
    {
        char aux[100];
        strcpy(aux, ext);
        sprintf(ext, ".%s", aux);
    }

    _getcwd(director, sizeof(director));

    cauta(director, ext, text, minim);

    return 0;
}
