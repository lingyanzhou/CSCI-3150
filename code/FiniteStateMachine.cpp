#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <memory>
#include <list>
#include <stdexcept>
#include "FiniteStateMachine.hpp"
#include "State.hpp"
#include "Token.hpp"

InvalidInputException::InvalidInputException(const std::string& what_arg)
:std::runtime_error(what_arg+": Invalid input command line")
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
}

FiniteStateMachine::FiniteStateMachine()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	curState = State::getInstance();
}
void FiniteStateMachine::parse(const std::string &l, std::list<Token> &tokenList) throw(InvalidInputException)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	curState = State::getInstance();
	tokenList.clear();
	std::unique_ptr<char[]> line(new char [l.length()+1]);
	strcpy(line.get(), l.c_str());
	for (int i=0; ; i++) {
		char * tokPtr;
		if (0==i) 
			tokPtr = strtok(line.get(), Token::DelimString);
		else
			tokPtr = strtok(NULL, Token::DelimString);
		std::string oneTokenVal;
		if (NULL==tokPtr)
			oneTokenVal="";
		else oneTokenVal=tokPtr;
		Token nextToken(oneTokenVal);
		curState = curState->doTransition(nextToken);
		if ( curState->getStatus()!= State::IsRun) {
			break;
		}
		tokenList.push_back(nextToken);
	}
	if ( curState->getStatus()== State::IsErr) {
		throw InvalidInputException("Error");
	}
}
