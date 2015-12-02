#include <memory>
#include <iostream>
#include <list>
#include "Shell.hpp"
#include "Token.hpp"
#include "Job.hpp"
#include "Behavior.hpp"
#include "TokenList2Job.hpp"

Behavior * Behavior::newBehavior(const std::list<Token> &tokenList)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (tokenList.empty()) {
		return new BehaviorNull();
	} else if (tokenList.front().isBuiltin() ) {
		if (0==tokenList.front().getValue().compare("cd")) {
			return new BehaviorCd();
		} else if (0==tokenList.front().getValue().compare("fg")) {
			return new BehaviorFg();
		} else if (0==tokenList.front().getValue().compare("exit")) {
			return new BehaviorExit();
		} else if (0==tokenList.front().getValue().compare("jobs")) {
			return new BehaviorJobs();
		} else {
			return new BehaviorNull();
		}
	} else if (tokenList.front().isCMD()){
		return new BehaviorInvokeFg();
	} else {
		return new BehaviorNull();
	}
}
void BehaviorNull::run(Shell &thisShell, std::list<Token> &tokenList, const std::string &line)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return;
}
void BehaviorCd::run(Shell &thisShell, std::list<Token> &tokenList, const std::string &line)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	tokenList.pop_front();
	if (tokenList.size()!=1)
		throw WrongArgNoException("cd");
	thisShell.cdBltin(tokenList.front().getValue());
}
void BehaviorFg::run(Shell &thisShell, std::list<Token> &tokenList, const std::string &line)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	tokenList.pop_front();
	if (tokenList.size()!=1) 
		throw WrongArgNoException("fg");
	thisShell.fgBltin(tokenList.front().getValue());
}
void BehaviorExit::run(Shell &thisShell, std::list<Token> &tokenList, const std::string &line)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	tokenList.pop_front();
	if (!tokenList.empty()) 
		throw WrongArgNoException("exit");
	thisShell.exitBltin();
}
void BehaviorJobs::run(Shell &thisShell, std::list<Token> &tokenList, const std::string &line)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	tokenList.pop_front();
	if (!tokenList.empty()) 
		throw WrongArgNoException("jobs");
	thisShell.jobsBltin();
}
void BehaviorInvokeFg::run(Shell &thisShell, std::list<Token> &tokenList, const std::string &line)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	std::shared_ptr<Job> jPtr(TokenList2Job::newJob(tokenList, line));
	thisShell.addJob(jPtr);
	jPtr->invoke();
	jPtr->putToFg();
	thisShell.waitFgJob(jPtr);
}
