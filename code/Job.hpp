#ifndef JOB_HPP
#define JOB_HPP
#include <memory>
#include <list>
#include <string>
#include <stdexcept>
#include "Utility.hpp"
#include "Process.hpp"
#include "Job.hpp"
class NoSuchProcException: public std::runtime_error
{
		public :
		explicit NoSuchProcException(const std::string& what_arg);
};
class Job
{
	protected: 
		std::string orgLine;
		std::list<std::shared_ptr<Process>> procs;
		pid_t pgid;
		std::string inFile;
		std::string outFile;
		int outMode;
		bool report;
		SyncPipe sync; 
	public:
		static const int IsRunning;
		static const int IsStopped;
		static const int IsExited;
		static const int IsKilled;
		static const int OutTruncate;
		static const int OutAppend;
		static const char * const IsRunningString;
		static const char * const IsStoppedString;
		static const char * const IsExitedString;
		static const char * const IsKilledString;
		static const char * getStatusMsg(int stat);
		Job();
		virtual void invoke();
		virtual void setInFile( const std::string &f );
		virtual void setOutFile( const std::string &f );
		virtual void setOrgLine(const std::string & l);
		virtual const std::string getOrgLine() const;
		virtual void setOutMode(int mode);
		virtual int getStatus() const;
		virtual std::shared_ptr<Process> getProcByPid(pid_t pid);
		virtual void attachProcess(std::shared_ptr<Process> & p);
		virtual void putToFg();
		virtual void continuFg();
		virtual bool needReport() const;
		virtual void setReportFlag();
		virtual void clearReportFlag();
};
#endif //JOB_HPP
