#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    int fisier;
    int n, i;
    char a, b;

    if (argc != 2)
        return 1;

    fisier = _open(argv[1], _O_RDWR | _O_BINARY);

    if (fisier == -1)
        return 1;

    n = _lseek(fisier, 0, SEEK_END);

    for (i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            _lseek(fisier, j, SEEK_SET);
            _read(fisier, &a, 1);

            _read(fisier, &b, 1);

            if (a > b)
            {
                _lseek(fisier, j, SEEK_SET);
                _write(fisier, &b, 1);
                _write(fisier, &a, 1);
            }
        }
    }

    _close(fisier);

    return 0;
}
