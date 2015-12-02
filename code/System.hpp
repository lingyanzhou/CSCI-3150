#ifndef SYSTEM_HPP
#define SYSTEM_HPP
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
class System
{
	public:
		static int xchdir(const char * path);
		static int xopen(const char *pathname,  const int flags);
		static int xopen(const char *pathname,  const int flags,  const mode_t mode);
		static int xclose(const int fd);
		static int xexecvp(const char *file, char *const argv[]);
		static pid_t xfork(void);
		static long xpathconf(const char *path, int name);
		static int xpipe(int pipefd[2]);
		static int xdup2(const int oldfd, const int newfd);
		static pid_t xgetpid(void);
		static int xsetpgid( const pid_t pid,  const pid_t pgid);
		static pid_t xgetpgid(void);
		static pid_t xwaitpid( const pid_t pid, int * const status,  const int options);
		static int xtcsetpgrp( const int fd,  const pid_t pgrp);
		static pid_t xtcgetpgrp( const int fd);
		static int xkill( const pid_t pid,  const int sig);
		static char * xgetcwd(char * const buf,  const size_t size);
		static int xisatty(const int fd);
		static int xsetenv(const char *name, const char *value, int overwrite);

};
#endif //SYSTEM_HPP
