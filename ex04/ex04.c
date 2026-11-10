#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    int sursa, dest;
    char buffer[1024];
    int n;

    if (argc != 3)
        return 1;

    if (strcmp(argv[1], argv[2]) == 0)
    {
        printf("Fisierele sunt identice!\n");
        return 1;
    }

    sursa = _open(argv[1], _O_RDONLY | _O_BINARY);

    if (sursa == -1)
    {
        printf("Nu se poate deschide fisierul sursa!\n");
        return 1;
    }

    dest = _open(argv[2], _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);

    if (dest == -1)
    {
        _close(sursa);
        printf("Nu se poate crea fisierul destinatie!\n");
        return 1;
    }

    while ((n = _read(sursa, buffer, sizeof(buffer))) > 0)
    {
        _write(dest, buffer, n);
    }

    _close(sursa);
    _close(dest);

    return 0;
}
