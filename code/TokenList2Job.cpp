#include "TokenList2Job.hpp"
#include "Process.hpp"
#include "Job.hpp"
#include "Token.hpp"
#include "TokenList.hpp"
#include "Utility.hpp"
#include <stdlib.h>
#include <string>
#include <memory>
#include <iostream>
InvalidTokenQueueException::InvalidTokenQueueException(const std::string& what_arg)
:std::logic_error(what_arg+": Invalid token queue")
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
}

Job * TokenList2Job::newJob(std::list<Token> &tokenList, const std::string & l)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (tokenList.empty() || !tokenList.front().isCMD())
		throw InvalidTokenQueueException("TokenList2Job::newJob");

	Job * job = new Job();
	job->setOrgLine(l);
	while ( !tokenList.empty()) {
		if (tokenList.front().isCMD()) {
			std::shared_ptr<Process> proc ( new RealProcess());
			std::list<Token> argList;
			proc->setCMD( tokenList.front().getValue());
			tokenList.front().setValue(Utility::path2Name(tokenList.front().getValue()));
			argList.splice(argList.end(), tokenList, tokenList.begin());
#ifdef DEBUG
			std::cout<<argList.front().getValue()<<" ";
#endif
			while (!tokenList.empty()) {
				if (tokenList.front().isCMD()) {
					break;
				} else if (tokenList.front().isARG()) {
					argList.splice(argList.end(), tokenList, tokenList.begin());
#ifdef DEBUG
					std::cout<<argList.front().getValue()<<" ";
#endif
				} else if (tokenList.front().isORT()) {
					job->setOutMode(Job::OutTruncate);
					tokenList.pop_front();
				} else if (tokenList.front().isORA()) {
					job->setOutMode(Job::OutAppend);
					tokenList.pop_front();
				} else if (tokenList.front().isInFile()) {
					job->setInFile(tokenList.front().getValue());
					tokenList.pop_front();
				} else if (tokenList.front().isOutFile()) {
					job->setOutFile(tokenList.front().getValue());
					tokenList.pop_front();
				} else {
					tokenList.pop_front();
				}
			}

			proc->setArgv(TokenList::to2DArray(argList));
			job->attachProcess(proc);
		}
	}
	return job;
}
