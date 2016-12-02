
// This include file is for MXML (Mini-XML) support for Windows Mobile.
#ifndef _ERRNO_H
#define _ERRNO_H


#ifndef FLX_WIN32
static int errno = 0;
#endif

#ifndef WINCE
#define EAGAIN 0
#define strerror(a) ""
#endif

#endif