#ifndef TOKENLIST2JOB_HPP
#define TOKENLIST2JOB_HPP
#include "Job.hpp"
#include "Process.hpp"
#include "Token.hpp"
#include <stdexcept>
#include <list>
#include <string>
class InvalidTokenQueueException: public std::logic_error
{
	public:
		InvalidTokenQueueException(const std::string& what_arg);
};
class TokenList2Job 
{
	public:
		static Job * newJob(std::list<Token> &tokenList, const std::string &l);
};
#endif //TOKENLIST2JOB_HPP
