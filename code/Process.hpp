#ifndef PROCESS_HPP
#define PROCESS_HPP
#include <sys/types.h>
#include <sys/wait.h>
#include <string>

class Process
{
	protected:
		std::string cmd;
		char ** argv;
		int infd;
		int outfd;
		pid_t pid;
		int status;
	public:
		static const int IsRunning;
		static const int IsStopped;
		static const int IsExited;
		static const int IsKilled;
		Process();
		virtual ~Process();

		virtual pid_t invoke(const int gpid, SyncPipe &sync, int pipeWriteFd = -1) =0 ;
		virtual void setCMD(const std::string &c) =0 ;
		virtual void setArgv(char ** arg) =0 ;
		virtual void setInFd(const int fd) =0 ;
		virtual void setOutFd(const int fd) =0 ;
		virtual void setPid(const int pid) =0 ;
		virtual pid_t getPid() const =0 ;
		virtual void setStatus(const int s) =0 ;
		virtual bool isStopped() const =0 ;
		virtual bool isRunning() const =0 ;
		virtual bool isExited() const =0 ;
		virtual bool isKilled() const =0 ;
};
class RealProcess : public Process
{
	public:
		RealProcess();
		pid_t invoke(const int gpid, SyncPipe &sync, int pipeWriteFd = -1);
		void setCMD(const std::string & c);
		void setArgv(char ** arg);
		void setInFd(const int fd);
		void setOutFd(const int fd);
		void setPid(const int pid);
		pid_t getPid() const;
		void setStatus(const int s);
		bool isStopped() const;
		bool isRunning() const;
		bool isExited() const;
		bool isKilled() const;
};
#endif //PROCESS_HPP
