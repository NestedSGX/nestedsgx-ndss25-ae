#ifndef _DEBUG_PRINT_H
#define _DEBUG_PRINT_H

//#ifndef BUFSIZE_H
//char buf[BUFSIZE];
//#define BUFSIZE_H
//#else
//extern char *buf;
//#endif

//#define DEBUGP

#ifdef DEBUGP
#define DEBUG_PRINT(...) snprintf( buf, BUFSIZE-1, __VA_ARGS__ );ocall_print_string(buf);memset(buf,'\0',BUFSIZE)
#else
#define DEBUG_PRINT(...) do{ } while ( 0 )
#endif

#endif
