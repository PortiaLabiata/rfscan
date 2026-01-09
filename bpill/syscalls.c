#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

/***************************************************************************/

__attribute__((used))
int _read_r(struct _reent *r, int file, char * ptr, int len) {
  (void)r;
  (void)file;
  (void)ptr;
  (void)len;
  __errno_r(r)  = EINVAL;
  return -1;
}

/***************************************************************************/

__attribute__((used))
int _lseek_r(struct _reent *r, int file, int ptr, int dir) {
  (void)r;
  (void)file;
  (void)ptr;
  (void)dir;

  return 0;
}

/***************************************************************************/

__attribute__((used))
int _write_r(struct _reent *r, int file, char * ptr, int len) {
  (void)r;
  (void)file;
  (void)ptr;
  return len;
}

/***************************************************************************/

__attribute__((used))
int _open_r(struct _reent *r, const char *p, int a, int b) {

  (void)r;
  (void)p;
  (void)a;
  (void)b;
  __errno_r(r) = EINVAL;
  return -1;
}

/***************************************************************************/

__attribute__((used))
int _close_r(struct _reent *r, int file) {
  (void)r;
  (void)file;

  return 0;
}

/***************************************************************************/

__attribute__((used))
caddr_t _sbrk_r(struct _reent *r, int incr) {
  (void)incr;
  __errno_r(r) = ENOMEM;
  return (caddr_t)-1;
}

/***************************************************************************/

__attribute__((used))
int _fstat_r(struct _reent *r, int file, struct stat * st) {
  (void)r;
  (void)file;

  memset(st, 0, sizeof(*st));
  st->st_mode = S_IFCHR;
  return 0;
}

/***************************************************************************/

__attribute__((used))
int _isatty_r(struct _reent *r, int fd) {
  (void)r;
  (void)fd;

  return 1;
}

/***************************************************************************/

__attribute__((used))
void _exit(int status) {

  (void) status;

  while (1)
	;
  abort();
}

/***************************************************************************/

__attribute__((used))
int _kill(int pid, int sig) {

  (void) pid;
  (void) sig;

  while (1)
	;
  abort();
}

/***************************************************************************/

__attribute__((used))
int _getpid(void) {

  return 1;
  abort();
}

/***************************************************************************/
/* Only used in C++ mode.*/
void __cxa_pure_virtual(void) {
	while (1)
		;
}

/*** EOF ***/
