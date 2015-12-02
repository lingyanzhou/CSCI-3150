#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <signal.h>
#include <list>
#include <string>
#include <memory>
#include <stdexcept>
#include <system_error>
#include <iterator>
#include "Utility.hpp"
#include "System.hpp"
#include "Token.hpp"
#include "TokenList.hpp"
#include "Job.hpp"
#include "Behavior.hpp"
#include "FiniteStateMachine.hpp"

static Shell * gShellPtr=NULL;
static void handlerSIGCHLD(int sig) throw()
{
	if (gShellPtr!=NULL) {
		try {
			gShellPtr->waitAnyJobNB();
		} catch (...) {}
	}
}

WrongArgNoException::WrongArgNoException(const std::string& what_arg)
	:std::runtime_error(what_arg+": Wrong number of arguments")
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
}
Shell::~Shell()
{
}
void Shell::prompt()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	std::cout<<"[3150 shell:"<<cwd<<"]$ ";
	std::cout.flush();
}
void Shell::init()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif

	cwd = Utility::getcwd();
	int terminalfd = STDIN_FILENO;
	if ( ! System::xisatty (terminalfd)) exit(EXIT_FAILURE);

	while (System::xtcgetpgrp (terminalfd) != System::xgetpgid())
		System::xkill (- System::xgetpgid(), SIGTTIN);

	Utility::initShellSighand();
	Utility::setHandlerSIGCHLD(handlerSIGCHLD);

	pid_t pid= System::xgetpid();
	System::xsetpgid (pid, pid);
	System::xtcsetpgrp (terminalfd, pid);

	System::xsetenv( "PATH", "/bin:/usr/bin:./", 1);
}
void Shell::reportAndCleanJobs()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif

	if (jobs.size()==0)
		return;
	int i=0;
	for (std::list<std::shared_ptr<Job>>::iterator it = jobs.begin(); it!=jobs.end(); ++it) {
		i++;
		Job * ptr = it->get();
		if (ptr->needReport()) {
			std::cout<<"["<<i<<"] "<<Job::getStatusMsg(ptr->getStatus())<<" "<<ptr->getOrgLine()<<std::endl;
			ptr->clearReportFlag();
		}
		int status = ptr->getStatus();
		if (status == Job::IsExited || status == Job::IsKilled) {
			it = jobs.erase(it);
			continue;
		}
	}
}
bool Shell::hasChild()
{
	for (std::list<std::shared_ptr<Job>>::iterator it = jobs.begin(); it!=jobs.end(); ++it) {
		int jobStatus = it->get()->getStatus();
		if (jobStatus==Job::IsStopped || it->get()->getStatus()==Job::IsRunning )
			return true;
	}
	return false;

}
void Shell::waitFgJob(std::shared_ptr<Job> &jobPtr)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	do {
		ProcessStatusChange change = Utility::waitAnyChildBlock();
		int status;
		if (change.isStop()) {
			status = Process::IsStopped;
		} else if (change.isExit()) {
			status = Process::IsExited;
		} else if (change.isKill()){
			status = Process::IsKilled;
		} else {
			status = Process::IsRunning;
		}
		for (std::list<std::shared_ptr<Job>>::iterator it = jobs.begin(); it!=jobs.end(); ++it) {
			try{
				Job * jPtr = (*it).get();
				Process * pPtr = jPtr->getProcByPid(change.getPid()).get();
				if (jPtr==jobPtr.get()) {
					pPtr->setStatus(status);
				} else {
					int prev = jPtr->getStatus();
					pPtr->setStatus(status);
					if (prev != jPtr->getStatus())
						jPtr->setReportFlag();
				}
			} catch (NoSuchProcException &e) {
			}
		}
	} while (jobPtr->getStatus()==Job::IsRunning);
	System::xtcsetpgrp(STDIN_FILENO, System::xgetpid());
}
void Shell::waitAnyJobNB()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	while (hasChild()) {
		ProcessStatusChange change = Utility::waitAnyChildNonBlock();
		if (change.noChange())
			break;
		int status;
		if (change.isStop()) {
			status = Process::IsStopped;
		} else if (change.isExit()) {
			status = Process::IsExited;
		} else if (change.isKill()){
			status = Process::IsKilled;
		} else {
			status = Process::IsRunning;
		}
		for (std::list<std::shared_ptr<Job>>::iterator it = jobs.begin(); it!=jobs.end(); ++it) {
			try{
				Job * jPtr = (*it).get();
				Process * pPtr = jPtr->getProcByPid(change.getPid()).get();
				int prev = jPtr->getStatus();
				pPtr->setStatus(status);
				if (prev != jPtr->getStatus())
					jPtr->setReportFlag();
			} catch (NoSuchProcException &e) {
			}
		}
	}
}
void Shell::printTokenList(const std::list<Token> &tokenList)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif

	if (tokenList.empty()) {
		return;
	}
	int i=1;
	for (std::list<Token>::const_iterator it = tokenList.begin(); it!= tokenList.end(); ++it ) {
		std::cout<<"Token "<<i<<": \""<<it->getValue()<<"\" ("
			<<it->getTypeStringSimple()<<")"<<std::endl;
		++i;
	}
}
void Shell::addJob(std::shared_ptr<Job> &jobPtr)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif

	jobs.push_back(jobPtr);
}
void Shell::cdBltin(const std::string &path)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif

	try {
		Utility::chdir(path);
		cwd = Utility::getcwd();
	} catch (std::system_error &e) {
		std::cout<<e.what()<<std::endl;
	}
}
void Shell::fgBltin(const std::string &jid)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif

	if (std::string::npos!=jid.find_first_not_of("0123456789")) {
		std::cout<<"fg: No such job"<<std::endl;
		return;
	}
	size_t jidInt;
	try{
		jidInt = (size_t)std::stoi(jid);
	} catch (std::logic_error &e) {
		std::cout<<"fg: No such job"<<std::endl;
		return;
	}
	if (jidInt>jobs.size() || jidInt==0){
		std::cout<<"fg: No such job"<<std::endl;
		return;
	}

	std::list<std::shared_ptr<Job>>::iterator it = jobs.begin();
	std::advance(it,jidInt-1);
	Job * ptr = (*it).get();
	std::cout<<"Job wake up: "<<ptr->getOrgLine()<<std::endl;
	ptr->continuFg();
	waitFgJob(*it);
}
void Shell::jobsBltin()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif

	if (0==jobs.size()) {
		std::cout<<"No suspended jobs"<<std::endl;
		return;
	}
	int i=1;
	for (std::list<std::shared_ptr<Job>>::iterator it = jobs.begin(); it!=jobs.end(); ++it) {
		Job* ptr = it->get();
		std::cout<<"["<<i<<"]"<<" "<<ptr->getOrgLine()<<std::endl;
		i++;
	}
}

void Shell::exitBltin()
{
#ifdef DEBUG
	std::cout<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif

	if (jobs.size()==0)
		exitCalled = true;
	else
		std::cout<<"There is at least one suspended job"<<std::endl;
}

void Phase1Shell::mainloop()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif

	exitCalled = 0;
	while (!exitCalled) {
		try {
			std::cin.clear();
			prompt();
			std::string line;
			std::getline(std::cin, line);
			if (!std::cin.eof()) {
				std::list<Token> tokenList;
				FiniteStateMachine fsm;
				fsm.parse(line, tokenList);
				printTokenList(tokenList);
			} else {
				std::cout<<std::endl;
				exitBltin();
			}
		} catch (std::runtime_error &e) {
			std::cout<<e.what()<<std::endl;;
		}
	}
}
void Phase1ExtShell::printTokenList(const std::list<Token> &tokenList)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif

	if (tokenList.empty()) {
		return;
	}
	int i=1;
	for (std::list<Token>::const_iterator it = tokenList.begin(); it!= tokenList.end(); ++it ) {
		std::cout<<"Token "<<i<<": \""<<it->getValue()<<"\" ("
			<<it->getTypeStringFull()<<")"<<std::endl;
		++i;
	}
}
void Phase1ExtShell::mainloop()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif

	exitCalled = 0;
	while (!exitCalled) {
		try {
			std::cin.clear();
			prompt();
			std::string line;
			std::getline(std::cin, line);
			if (!std::cin.eof()) {
				std::list<Token> tokenList;
				FiniteStateMachine fsm;
				fsm.parse(line, tokenList);
				TokenList::doGlob(tokenList);
				printTokenList(tokenList);
			} else {
				std::cout<<std::endl;
				exitBltin();
			}
		} catch (std::runtime_error &e) {
			std::cout<<e.what()<<std::endl;;
		}
	}	
}
void Phase2Shell::mainloop()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif

	exitCalled = 0;
	Utility::blockSIG(SIGCHLD);
	while (!exitCalled) {
		try {
			std::cin.clear();
			reportAndCleanJobs();
			prompt();
			std::string line;

			gShellPtr = this;

			Utility::unblockSIG(SIGCHLD);
			while(true) {
				std::getline(std::cin, line);
				if (std::cin.eof() ||std::cin.good() ){
					break;
				}

			}
			Utility::blockSIG(SIGCHLD);

			if (!std::cin.eof()) {
				std::cin.clear();
				std::list<Token> tokenList;
				FiniteStateMachine fsm;
				fsm.parse(line, tokenList);
				TokenList::doGlob(tokenList);
				std::unique_ptr<Behavior> behavior (Behavior::newBehavior(tokenList) );
				behavior->run(*this,tokenList, line);
			} else {
				std::cin.clear();
				std::cout<<std::endl;
				exitBltin();
			}
		} catch (std::runtime_error &e) {
			std::cout<<e.what()<<std::endl;
		}
	}
}
