#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <io.h>
#include <direct.h>
#include <sys/stat.h>
#include <fcntl.h>

int ascunse = 0;
int suprascriere = 1;
int sterge = 0;
int nivelMax = 100;

void copiaza(char *sursa, char *dest, int nivel)
{
    struct _finddata_t f;
    intptr_t h;
    char s[300], d[300];
    FILE *in, *out;
    char buffer[1024];
    int n;

    sprintf(s, "%s\\*.*", sursa);
    h = _findfirst(s, &f);

    if (h == -1)
        return;

    do
    {
        if (strcmp(f.name, ".") == 0 || strcmp(f.name, "..") == 0)
            continue;

        sprintf(s, "%s\\%s", sursa, f.name);
        sprintf(d, "%s\\%s", dest, f.name);

        if (f.attrib & _A_SUBDIR)
        {
            if (nivel < nivelMax)
            {
                mkdir(d);
                copiaza(s, d, nivel + 1);
            }
        }
        else
        {
            if (!ascunse && (f.attrib & _A_HIDDEN))
                continue;

            if (suprascriere || access(d, 0) != 0)
            {
                in = fopen(s, "rb");
                out = fopen(d, "wb");

                if (in != NULL && out != NULL)
                {
                    while ((n = fread(buffer, 1, sizeof(buffer), in)) > 0)
                        fwrite(buffer, 1, n, out);

                    fclose(in);
                    fclose(out);

                    if (sterge)
                        remove(s);
                }
                else
                {
                    if (in != NULL)
                        fclose(in);

                    if (out != NULL)
                        fclose(out);
                }
            }
        }

    } while (_findnext(h, &f) == 0);

    _findclose(h);
}

int main(int argc, char *argv[])
{
    char sursa[300], dest[300];
    int i;

    if (argc < 3)
        return 1;

    for (i = 1; i < argc - 2; i++)
    {
        if (strcmp(argv[i], "-a") == 0)
            ascunse = 1;
        else if (strcmp(argv[i], "-o") == 0)
            suprascriere = 0;
        else if (strcmp(argv[i], "-u") == 0)
            sterge = 1;
        else if (argv[i][1] == 'd')
            nivelMax = atoi(argv[i] + 2);
    }

    if (_fullpath(sursa, argv[argc - 2], 300) == NULL)
        return 1;

    if (_fullpath(dest, argv[argc - 1], 300) == NULL)
        return 1;

    if (_strnicmp(sursa, dest, strlen(sursa)) == 0 &&
        (dest[strlen(sursa)] == '\\' || dest[strlen(sursa)] == '\0'))
    {
        printf("Destinatia este in directorul sursa!\n");
        return 1;
    }

    mkdir(dest);
    copiaza(sursa, dest, 1);

    return 0;
}
