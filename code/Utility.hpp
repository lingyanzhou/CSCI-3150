#ifndef UTILITY_HPP
#define UTILITY_HPP
#include <string>
#include <system_error>
#include "System.hpp"
class SyncPipe
{
	private:
		int p[2];
	public:
		SyncPipe();
		~SyncPipe();
		void open();
		void wait();
		void signal();
};
class ProcessStatusChange 
{
	private:
		pid_t pid;
		int status;
	public:
		ProcessStatusChange( const pid_t pidTmp, const int statusTmp);
		pid_t getPid() const;
		bool noChange() const;
		bool isStop() const;
		bool isKill() const;
		bool isExit() const;
		bool isCont() const;
};
class Utility
{
	public :
		static size_t defaultPathSize;
		static int chdir(const char * path);
		static int chdir(const std::string &path);
		static int openFileIR(const char * path);
		static int openFileIR(const std::string &path);
		static int openFileORA(const char * path);
		static int openFileORA(const std::string &path);
		static int openFileORT(const char * path);
		static int openFileORT(const std::string &path);
		static ProcessStatusChange waitAnyChildBlock();
		static ProcessStatusChange waitAnyChildNonBlock();
		static std::string getcwd();
		static void initShellSighand();
		static void initChildSighand();
		static void blockAllSig();
		static void unblockAllSig();
		static std::string path2Name(const std::string path);
		static void setHandlerSIGCHLD( void (*handler)(int));
		static void blockSIG( int sig);
		static void unblockSIG( int sig);

};
#endif //UTILITY_HPP
