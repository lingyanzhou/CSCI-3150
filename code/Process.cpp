#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <system_error>
#include <stdexcept>
#include "Utility.hpp"
#include "Process.hpp"
const int Process::IsRunning = 0;
const int Process::IsStopped = 1;
const int Process::IsExited = 2;
const int Process::IsKilled = 2;
Process::Process()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	argv = NULL;
	infd = STDIN_FILENO;
	outfd = STDOUT_FILENO;
	pid = 0;
	status = Process::IsExited;
}
Process::~Process()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (NULL!=argv) {
		for (int i =0; argv[i]!=NULL; i++) {
			free(argv[i]);
		}
		free(argv);
	}
}
RealProcess::RealProcess( )
: Process()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
}

pid_t RealProcess::invoke(int pgid, SyncPipe &sync, int pipeWriteFd)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	Utility::blockAllSig();
	pid_t cpid = System::xfork();
	if (0==cpid) { //child
		try {
      if (pipeWriteFd != -1) {
        System::xclose(pipeWriteFd);
      }
			if (0==pgid)
				pgid = System::xgetpid();
			System::xsetpgid(System::xgetpid(), pgid);
			if ( STDIN_FILENO != infd) {
				System::xdup2(infd, STDIN_FILENO);
        System::xclose(infd);
      }
			if ( STDOUT_FILENO != outfd) {
				System::xdup2(outfd, STDOUT_FILENO);
        System::xclose(outfd);
      }
			sync.wait();
			Utility::initChildSighand();
			Utility::unblockAllSig();
			try {
				System::xexecvp(cmd.c_str(), argv);
			} catch (std::system_error &e) {
				std::cerr<<e.what()<<std::endl;
			}
		} catch (std::exception &e) {
			std::cerr<<e.what()<<std::endl;
		} catch (...) {}
		exit(EXIT_FAILURE);
	} else { //parent
		Utility::unblockAllSig();
		Utility::blockSIG(SIGCHLD);
		if (0==pgid)
			pgid = cpid;
		System::xsetpgid(cpid, pgid);
		if ( STDIN_FILENO!= infd) {
			System::xclose(infd);
		}
		if ( STDOUT_FILENO!= outfd) {
			System::xclose(outfd);
		}
		this->pid = cpid;
		status = Process::IsRunning;
	}
	return cpid;

}
void RealProcess::setCMD(const std::string & c)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	cmd = c;
}
void RealProcess::setArgv(char ** arg)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (NULL!=this->argv || NULL==arg)
		throw std::logic_error("setArgv: Argv already set or new argv == NULL");
	this->argv = arg;
}
void RealProcess::setInFd(const int fd)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	infd = fd;
}
void RealProcess::setOutFd(const int fd)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	outfd = fd;
}
void RealProcess::setPid(const int pid)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	this->pid = pid;
}
pid_t RealProcess::getPid() const
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return pid;
}
void RealProcess::setStatus(const int s)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	status =s;
}
bool RealProcess::isStopped() const 
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return status == Process::IsStopped;
}
bool RealProcess::isRunning() const 
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return status == Process::IsRunning;
}
bool RealProcess::isExited() const
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return status == Process::IsExited;
}
bool RealProcess::isKilled() const
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return status == Process::IsKilled;
}
