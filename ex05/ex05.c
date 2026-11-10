#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    int fisier;
    int eof, crt;
    char c;
    char buffer[100];
    int nr = 0;

    if (argc != 2)
        return 1;

    fisier = _open(argv[1], _O_RDWR | _O_BINARY);

    if (fisier == -1)
        return 1;

    eof = _lseek(fisier, 0, SEEK_END);
    _lseek(fisier, 0, SEEK_SET);

    while (_tell(fisier) < eof)
    {
        if (_read(fisier, &c, 1) != 1)
            break;

        if (isdigit(c))
        {
            buffer[nr] = c;
            nr++;

            if (nr == 100)
            {
                crt = _tell(fisier);
                _lseek(fisier, 0, SEEK_END);
                _write(fisier, buffer, nr);
                _lseek(fisier, crt, SEEK_SET);
                nr = 0;
            }
        }
    }

    if (nr > 0)
    {
        _lseek(fisier, 0, SEEK_END);
        _write(fisier, buffer, nr);
    }

    _close(fisier);

    return 0;
}
