#ifndef STATE_HPP
#define STATE_HPP
#include "Token.hpp"

class State 
{
	private:
		static State* instance;

	public:
		static const int IsRun;
		static const int IsEnd;
		static const int IsErr;
		static State* getInstance();
		virtual State * doTransition( Token & tok) = 0;
		virtual int getStatus();
};
class ErrorState : public State 
{
	private:
		static State* instance;
	public:
		static State* getInstance();
		State * doTransition( Token & tok);
		int getStatus();
};
class EndState : public State
{
	private:
		static State* instance;
	public:
		static State* getInstance();
		State * doTransition( Token & tok);
		int getStatus();
};
class WaitFirstTokenState : public State
{
	private:
		static State* instance;
	public:
		static State* getInstance();
		State * doTransition( Token & tok);
};
class WaitCMDState : public State
{
	private:
		static State* instance;
	public:
		static State* getInstance();
		State * doTransition( Token & tok);
};
class NoMoreState : public State
{
	private:
		static State* instance;
	public:
		static State* getInstance();
		State * doTransition( Token & tok);
};
class WaitARGBPState : public State
{
	private:
		static State* instance;
	public:
		static State* getInstance();
		State * doTransition( Token & tok);
};
class WaitARGState : public State
{
	private:
		static State* instance;
	public:
		static State* getInstance();
		State * doTransition( Token & tok);
};
class WaitBltinARGState : public State
{
	private:
		static State* instance;
	public:
		static State* getInstance();
		State * doTransition( Token & tok);
};
class WaitOneARGState : public State
{
	private:
		static State* instance;
	public:
		static State* getInstance();
		State * doTransition( Token & tok);
};
class WaitInFileState : public State
{
	private:
		static State* instance;
	public:
		static State* getInstance();
		State * doTransition( Token & tok);
};
class WaitOutFileState : public State
{
	private:
		static State* instance;
	public:
		static State* getInstance();
		State * doTransition( Token & tok);
};
class WaitOutFileAPState : public State
{
	private:
		static State* instance;
	public:
		static State* getInstance();
		State * doTransition( Token & tok);
};
class WaitLastInFileState : public State
{
	private:
		static State* instance;
	public:
		static State* getInstance();
		State * doTransition( Token & tok);
};
class WaitLastOutFileState : public State
{
	private:
		static State* instance;
	public:
		static State* getInstance();
		State * doTransition( Token & tok);
};
class WaitIRState : public State
{
	private:
		static State* instance;
	public:
		static State* getInstance();
		State * doTransition( Token & tok);
};
class WaitORState : public State
{
	private:
		static State* instance;
	public:
		static State* getInstance();
		State * doTransition( Token & tok);
};

#endif //STATE_HPP
