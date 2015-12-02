#ifndef FINITESTATEMACHINE_HPP
#define FINITESTATEMACHINE_HPP
#include "State.hpp"
#include "Token.hpp"
#include <stdexcept>

class InvalidInputException : public std::runtime_error
{
	public :
		explicit InvalidInputException(const std::string& what_arg);
};


class FiniteStateMachine
{
	private:
		State *curState;
	public:
		FiniteStateMachine();
		void parse(const std::string & l, std::list<Token> &tokenList) throw(InvalidInputException);
};
#endif //FINITESTATEMACHINE_HPP
