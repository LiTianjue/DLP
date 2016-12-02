#ifndef _IO_H
#define _IO_H

// These are POSIX functions declared to keep the "fd" functions in
// the MXML library happy.
// NOTE: "fd" functions are not currently supported since read and write
// are not ANSI C.
int read(int fd, char* pBuffer, int nBufferLen)
{
    return 0;
}

int write(int fd, char* pBuffer, int nBufferLen)
{
    return 0;
}

#endif