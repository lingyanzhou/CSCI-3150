#include <stdlib.h>
#include <iostream>
#include <stdexcept>
#include <iterator>
#include <system_error>
#include <string.h>
#include "Job.hpp"
#include "Process.hpp"
#include "System.hpp"

NoSuchProcException::NoSuchProcException(const std::string& what_arg)
	:std::runtime_error(what_arg+": No such process")
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
}
const int Job::IsRunning=0;
const int Job::IsStopped=1;
const int Job::IsExited=2;
const int Job::IsKilled=3;
const int Job::OutTruncate = 0;
const int Job::OutAppend = 1;
const char * const Job::IsRunningString = "Running";
const char * const Job::IsStoppedString = "Stopped";
const char * const Job::IsExitedString = "Exited";
const char * const Job::IsKilledString = "Killed";
const char * Job::getStatusMsg(int stat)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	switch (stat) {
		case Job::IsRunning:
			return Job::IsRunningString;
			break;
		case Job::IsStopped:
			return Job::IsStoppedString;
			break;
		case Job::IsExited:
			return Job::IsExitedString;
			break;
		case Job::IsKilled:
			return Job::IsKilledString;
			break;
		default: return "";
	}
}
Job::Job()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	report = false;
	pgid = 0;
}
void Job::invoke()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (procs.empty()) throw std::logic_error("Job has an empty process list");
	sync.open();
	int infd= STDIN_FILENO, outfd= STDOUT_FILENO;
	if (!inFile.empty()){
		try {
			infd = Utility::openFileIR(inFile);
		} catch (std::system_error &e) {
			std::cout<<e.what()<<std::endl;
			infd = Utility::openFileIR("/dev/null");
		}
	}
	if (!outFile.empty()) {
		try {
			if (Job::OutTruncate == outMode) {
				outfd = Utility::openFileORT(outFile);
			} else if (Job::OutAppend == outMode) {
				outfd = Utility::openFileORA(outFile);
			}
		} catch (std::system_error &e) {
			std::cout<<e.what()<<std::endl;
			outfd = Utility::openFileORA("/dev/null");
		}
	}

	int p[2] = {-1,-1};
	std::list<std::shared_ptr<Process>>::iterator lastIt = procs.end();
	--lastIt;
	for (std::list<std::shared_ptr<Process>>::iterator it = procs.begin(); it!=procs.end(); ++it ) {
		Process * pPtr = (*it).get();
		if ( procs.begin()==it) {
			pPtr->setInFd(infd);
		} else {
			pPtr->setInFd(p[0]);
		}
		if ( lastIt==it) {
			pPtr->setOutFd(outfd);
      p[0] = -1;
		} else {
			System::xpipe(p);
			pPtr->setOutFd(p[1]);
		}
    pid_t pid = pPtr->invoke(pgid, sync, p[0]);
		if (0==pgid)
			pgid = pid;
	}
}
void Job::setInFile(  const std::string &f )
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	inFile = f;
}
void Job::setOutFile(  const std::string &f)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	outFile = f;
}
void Job::setOrgLine( const std::string & l )
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	orgLine = l;
}
const std::string Job::getOrgLine( ) const
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return orgLine;
}
void Job::setOutMode(int mode)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	outMode = mode;
}
int Job::getStatus() const
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	bool hasRunning = false;
	bool hasStopped = false;
	bool hasKilled = false;
	if (procs.empty())
		return Job::IsExited;
	for (std::list<std::shared_ptr<Process>>::const_iterator it = procs.begin(); it!=procs.end(); ++it) {
		Process * pPtr = it->get();
		if (pPtr->isStopped())
			hasStopped = true;
		if (pPtr->isRunning())
			hasRunning = true;
		if (pPtr->isKilled())
			hasKilled = true;
	}
	if (hasRunning)
		return Job::IsRunning;
	else if (hasStopped)
		return Job::IsStopped;
	else if (hasKilled)
		return Job::IsKilled;
	else
		return Job::IsExited;
}
std::shared_ptr<Process> Job::getProcByPid(pid_t pid)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	for (std::list<std::shared_ptr<Process>>::iterator it = procs.begin(); it!=procs.end(); ++it) {
		Process * pPtr = it->get();
		if (pPtr->getPid()==pid)
			return *it;
	}
	throw NoSuchProcException("Job::getProcByPid");
}
void Job::attachProcess(std::shared_ptr<Process> & p)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	procs.push_back(p);
}
void Job::putToFg()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	System::xtcsetpgrp(STDIN_FILENO, pgid);
	sync.signal();
}
void Job::continuFg()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	System::xtcsetpgrp(STDIN_FILENO, pgid);
	System::xkill(-pgid, SIGCONT);
}
bool Job::needReport() const
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return report;
}
void Job::setReportFlag()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	report = true;
}
void Job::clearReportFlag()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	report = false;
}
