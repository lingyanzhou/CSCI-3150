#include <string>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <memory>
#include "System.hpp"
#include "Utility.hpp"
#include <iostream>

SyncPipe::SyncPipe()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	p[0]=-1;
	p[1]=-1;
}
SyncPipe::~SyncPipe()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (p[0]!=-1)
		System::xclose(p[0]);
	if (p[1]!=-1)
		System::xclose(p[1]);
}
void SyncPipe::open()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	System::xpipe(p);
}
void SyncPipe::wait()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	System::xclose(p[1]);
	p[1] = -1;
	char c;
	read(p[0], &c, 1);
	System::xclose(p[0]);
	p[0] = -1;
}
void SyncPipe::signal()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	System::xclose(p[0]);
	p[0] = -1;
	System::xclose(p[1]);
	p[1] = -1;
}

ProcessStatusChange::ProcessStatusChange( const pid_t pidTmp, const int statusTmp)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	pid = pidTmp;
	status = statusTmp;
}
pid_t  ProcessStatusChange::getPid() const
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return pid;
}
bool ProcessStatusChange::noChange() const
{
	return pid==0;
}
bool ProcessStatusChange::isStop() const
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (0==pid)
		return false;
	return WIFSTOPPED(status);
}
bool ProcessStatusChange::isExit() const
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (0==pid)
		return false;
	return WIFEXITED(status);
}
bool ProcessStatusChange::isKill() const
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (0==pid)
		return false;
	return WIFSIGNALED(status);
}
bool ProcessStatusChange::isCont() const
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (0==pid)
		return false;
	return WIFCONTINUED(status);
}

size_t Utility::defaultPathSize = 1024;

int Utility::chdir(const char * path)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return System::xchdir(path);
}
int Utility::chdir(const std::string &path)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return System::xchdir(path.c_str());
}
int Utility::openFileIR(const char * path)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return System::xopen(path, O_RDONLY);
}
int Utility::openFileIR(const std::string &path)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return System::xopen(path.c_str(), O_RDONLY);
}
int Utility::openFileORA(const char * path)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return System::xopen(path, O_WRONLY | O_CREAT | O_APPEND,
			S_IRWXU | S_IRWXG | S_IRWXO);
}
int Utility::openFileORA(const std::string &path)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return System::xopen(path.c_str(), O_WRONLY | O_CREAT | O_APPEND,
			S_IRWXU | S_IRWXG | S_IRWXO);
}
int Utility::openFileORT(const char * path)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return System::xopen(path, O_WRONLY | O_CREAT | O_TRUNC,
			S_IRWXU | S_IRWXG | S_IRWXO);
}
int Utility::openFileORT(const std::string &path)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return System::xopen(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC,
			S_IRWXU | S_IRWXG | S_IRWXO);
}
ProcessStatusChange Utility::waitAnyChildBlock()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	pid_t pid;
	int status;
	pid = System::xwaitpid(-1, &status, WUNTRACED | WCONTINUED);
	return ProcessStatusChange(pid, status);
}
ProcessStatusChange Utility::waitAnyChildNonBlock()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	pid_t pid;
	int status;
	pid = System::xwaitpid(-1, &status, WUNTRACED | WCONTINUED | WNOHANG );
	return ProcessStatusChange(pid, status);
}
std::string Utility::getcwd()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	size_t cwdSize = System::xpathconf("/", _PC_PATH_MAX);
	std::unique_ptr<char[]> cwd (new char[cwdSize+1]);
	#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	System::xgetcwd( cwd.get(),cwdSize+1);
	std::string ret = cwd.get();
	return ret;
}
void Utility::initShellSighand()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif

	Utility::blockAllSig();
	signal (SIGINT, SIG_IGN);
	signal (SIGTERM, SIG_IGN);
#ifndef DEBUG
	signal (SIGQUIT, SIG_IGN);
#endif
	signal (SIGTSTP, SIG_IGN);
	signal (SIGTTIN, SIG_IGN);
	signal (SIGTTOU, SIG_IGN);
	signal (SIGCHLD, SIG_DFL);
	signal (SIGSTOP, SIG_DFL);
	signal (SIGKILL, SIG_DFL);
	Utility::unblockAllSig();
}
void Utility::initChildSighand()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	Utility::blockAllSig();
	signal (SIGINT, SIG_DFL);
	signal (SIGTERM, SIG_DFL);
	signal (SIGQUIT, SIG_DFL);
	signal (SIGTSTP, SIG_DFL);
	signal (SIGTTIN, SIG_DFL);
	signal (SIGTTOU, SIG_DFL);
	signal (SIGCHLD, SIG_DFL);
	signal (SIGSTOP, SIG_DFL);
	signal (SIGKILL, SIG_DFL);
	Utility::unblockAllSig();
}
void Utility::blockAllSig()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	sigset_t fullSet;
	sigfillset(&fullSet);
	sigprocmask(SIG_SETMASK, &fullSet, NULL);
}
void Utility::unblockAllSig()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	sigset_t fullSet;
	sigfillset(&fullSet);
	sigprocmask(SIG_UNBLOCK, &fullSet, NULL);
}
std::string Utility::path2Name(const std::string path)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	size_t pos = path.find_last_of ("/");
	if ( std::string::npos==pos ) {
		return path;
	} else if (pos+1<path.length()) {
		return path.substr(pos+1);
	}
	return path;
}
void Utility::setHandlerSIGCHLD( void (*handler)(int))
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	Utility::blockAllSig();
	struct sigaction sigAct;
	memset(&sigAct, 0, sizeof(sigAct));
	sigAct.sa_handler = handler;
	sigAct.sa_flags = SA_RESTART;
	sigfillset(&sigAct.sa_mask);
	sigaction( SIGCHLD, &sigAct, NULL);
	Utility::unblockAllSig();
}
void Utility::blockSIG( int sig)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	sigset_t set;
	sigaddset(&set, sig);
	sigprocmask(SIG_BLOCK, &set, NULL);
}
void Utility::unblockSIG( int sig)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	sigset_t set;
	sigaddset(&set, sig);
	sigprocmask(SIG_UNBLOCK, &set, NULL);
}
