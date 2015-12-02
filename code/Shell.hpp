#ifndef SHELL_HPP
#define SHELL_HPP
#include <unistd.h>
#include <string>
#include <list>
#include <memory>
#include <stdexcept>
#include "Job.hpp"
#include "Token.hpp"
class WrongArgNoException : public std::runtime_error
{
	public :
		explicit WrongArgNoException(const std::string& what_arg);
};
class Shell
{
	protected:
		virtual bool hasChild();
		virtual void prompt();
		virtual void reportAndCleanJobs();
		std::list<std::shared_ptr<Job>> jobs;
		std::string cwd;
		bool exitCalled;
		
	public:
		virtual ~Shell();
		virtual void init();
		virtual void mainloop()=0;
		virtual void addJob(std::shared_ptr<Job> &jobPtr);
		virtual void cdBltin(const std::string &path);
		virtual void fgBltin(const std::string &jid);
		virtual void jobsBltin();
		virtual void exitBltin();
		virtual void waitAnyJobNB();
		virtual void waitFgJob(std::shared_ptr<Job> &jobPtr);
		virtual void printTokenList(const std::list<Token> &tokenList);
};
class Phase1Shell : public Shell
{
	public:
		void mainloop();

};
class Phase1ExtShell : public Phase1Shell
{
	public:
		void mainloop();
		void printTokenList(const std::list<Token> &tokenList);
};
class Phase2Shell : public Shell
{
	public:
		void mainloop();
};
#endif //SHELL_HPP
