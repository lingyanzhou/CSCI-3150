#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <system_error>
#include <iostream>
#include "System.hpp"
int System::xchdir(const char * path)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	int ret;
	ret = chdir(path);
	if (0!=ret)
		throw std::system_error(errno, std::system_category(), path);
	return ret;
}
int System::xopen(const char *pathname,  const int flags)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	int ret;
	ret = open(pathname, flags);
	if (-1==ret)
		throw std::system_error(errno, std::system_category(), pathname);
	return ret;
}
int System::xopen(const char *pathname,  const int flags, mode_t mode)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	int ret;
	ret = open(pathname, flags, mode);
	if (-1==ret)
		throw std::system_error(errno, std::system_category(), pathname);
	return ret;
}
int System::xclose(const int fd)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	int ret;
	ret = close(fd);
	if (-1==ret)
		throw std::system_error(errno, std::system_category(), "close");
	return ret;
}
int System::xexecvp(const char *file, char *const argv[])
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	int ret;
	ret = execvp(file, argv);
	throw std::system_error(errno, std::system_category(), file);
	return ret;
}
pid_t System::xfork(void)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	pid_t ret;
	ret = fork();
	if (-1==ret)
		throw std::system_error(errno, std::system_category(), "fork");
	return ret;
}
long System::xpathconf(const char *path,  const int name)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	errno = 0;
	long ret;
	ret = pathconf(path, name);

	if ( -1 == ret && 0!=errno) {
		throw std::system_error(errno, std::system_category(), path);
	}
	return ret;
}
int System::xpipe(int pipefd[2])
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	int ret;
	ret = pipe(pipefd);
	if (-1==ret)
		throw std::system_error(errno, std::system_category(), "pipe");
	return ret;
}
int System::xdup2( const int oldfd,  const int newfd)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	int ret;
	ret = dup2(oldfd, newfd);
	if (-1==ret)
		throw std::system_error(errno, std::system_category(), "dup2");
	return ret;
}
pid_t System::xgetpid(void)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return getpid();
}
int System::xsetpgid(pid_t pid, pid_t pgid)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	int ret;
	ret = setpgid(pid, pgid);
	if (-1==ret)
		throw std::system_error(errno, std::system_category(), "setpgid");
	return ret;
}
pid_t System::xgetpgid(void)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return getpid();
}
pid_t System::xwaitpid( const pid_t pid, int * const status,  const int options)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	pid_t ret;
	ret = waitpid(pid, status, options);
	if (-1==ret)
		throw std::system_error(errno, std::system_category(), "waitpid");
	return ret;
}

int System::xtcsetpgrp( const int fd,  const pid_t pgrp)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	int ret;
	ret = tcsetpgrp( fd, pgrp);
	if (-1==ret)
		throw std::system_error(errno, std::system_category(), "tcsetpgrp");
	return ret;
}

pid_t System::xtcgetpgrp( const int fd)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	pid_t ret;
	ret = tcgetpgrp( fd);
	if (-1==ret)
		throw std::system_error(errno, std::system_category(), "tcgetpgrp");
	return ret;
}

int System::xkill(const pid_t pid,  const int sig)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	int ret;
	ret = kill(pid, sig);
	if (-1==ret)
		throw std::system_error(errno, std::system_category(), "kill");
	return ret;
}
char * System::xgetcwd( char * const buf,  const size_t size)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	char * ret;
	ret = getcwd(buf, size);
	if (NULL==ret)
		throw std::system_error(errno, std::system_category(), "getcwd");
	return ret;
}
int System::xisatty( const int fd)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	int ret;
	ret = isatty(fd);
	return ret;
}
int System::xsetenv(const char *name, const char *value, int overwrite)
{
	int ret;
	ret = setenv(name, value, overwrite);
	if (-1==ret)
		throw std::system_error(errno, std::system_category(), "setenv");
	return ret;
}

