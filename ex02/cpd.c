#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <io.h>
#include <direct.h>
#include <sys/stat.h>

int ascunse = 0;
int nivelMax = 100;
long lungimeMax = 999999999;

void muta(char *sursa, char *dest, int nivel)
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
                muta(s, d, nivel + 1);
                rmdir(s);
            }
        }
        else
        {
            if (ascunse || !(f.attrib & _A_HIDDEN))
            {
                if (f.size <= lungimeMax)
                {
                    in = fopen(s, "rb");
                    out = fopen(d, "wb");

                    if (in != NULL && out != NULL)
                    {
                        while ((n = fread(buffer, 1, sizeof(buffer), in)) > 0)
                            fwrite(buffer, 1, n, out);

                        fclose(in);
                        fclose(out);
                        remove(s);
                    }
                    else
                    {
                        if (in != NULL) fclose(in);
                        if (out != NULL) fclose(out);
                    }
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
        else if (argv[i][1] == 'd')
            nivelMax = atoi(argv[i] + 2);
        else if (argv[i][1] == 'l')
            lungimeMax = atol(argv[i] + 2);
    }

    _fullpath(sursa, argv[argc - 2], 300);
    _fullpath(dest, argv[argc - 1], 300);

    if (strstr(dest, sursa) == dest)
    {
        printf("Destinatia nu poate fi in sursa!\n");
        return 1;
    }

    mkdir(dest);
    muta(sursa, dest, 1);

    return 0;
}
