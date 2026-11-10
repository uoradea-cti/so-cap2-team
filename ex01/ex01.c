#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <io.h>
#include <direct.h>
#include <sys/stat.h>

void stergeDirector(char *cale)
{
    struct _finddata_t fisier;
    intptr_t handle;
    char copil[260];

    if (_chdir(cale) != 0)
        return;

    handle = _findfirst("*.*", &fisier);

    if (handle != -1)
    {
        do
        {
            if (strcmp(fisier.name, ".") == 0 ||
                strcmp(fisier.name, "..") == 0)
                continue;

            if (fisier.attrib & _A_SUBDIR)
            {
                sprintf(copil, "%s\\%s", cale, fisier.name);
                stergeDirector(copil);
            }
            else
            {
                _chmod(fisier.name, _S_IREAD | _S_IWRITE);
                remove(fisier.name);
            }

        } while (_findnext(handle, &fisier) == 0);

        _findclose(handle);
    }

    _chdir("..");
    _rmdir(cale);
}

int main(int argc, char *argv[])
{
    char cale[260];

    if (argc != 2)
        return 1;

    if (_fullpath(cale, argv[1], sizeof(cale)) == NULL)
        return 1;

    stergeDirector(cale);

    return 0;
}
