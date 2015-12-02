#ifndef BEHAVIOR_HPP
#define BEHAVIOR_HPP
#include <memory>
#include <list>
#include "Shell.hpp"
#include "Job.hpp"
#include "Token.hpp"
class Behavior
{
	public:
		static Behavior * newBehavior(const std::list<Token> &tokenList);
		virtual void run(Shell &thisShell, std::list<Token> &tokenList, const std::string &line) =0;
};

class BehaviorNull : public  Behavior
{
	public:
		void run(Shell &thisShell, std::list<Token> &tokenList, const std::string &line);
};
class BehaviorCd : public  Behavior
{
	public:
		void run(Shell &thisShell, std::list<Token> &tokenList, const std::string &line);
};
class BehaviorJobs : public  Behavior
{
	public:
		void run(Shell &thisShell, std::list<Token> &tokenList, const std::string &line);
};
class BehaviorExit : public  Behavior
{
	public:
		void run(Shell &thisShell, std::list<Token> &tokenList, const std::string &line);
};
class BehaviorFg : public  Behavior
{
	public:
		void run(Shell &thisShell, std::list<Token> &tokenList, const std::string &line);
};
class BehaviorInvokeFg : public  Behavior
{
	public:
		void run(Shell &thisShell, std::list<Token> &tokenList, const std::string &line);
};
#endif //BEHAVIOR_HPP
