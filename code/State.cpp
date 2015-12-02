#include <stdio.h>
#include <iostream>
#include "State.hpp"
#include "Token.hpp"
const int State::IsRun = 0x00;
const int State::IsEnd = 0x01;
const int State::IsErr = 0x02;
///State Base Class
State* State::instance = NULL;
State* State::getInstance()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (instance == NULL)
		instance = WaitFirstTokenState::getInstance();
	return instance;
}
int State::getStatus()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return State::IsRun;
}
///ErrorState Class
State* ErrorState::instance = NULL;
State* ErrorState::getInstance()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (instance == NULL)
		instance = new ErrorState();
	return instance;
}
State * ErrorState::doTransition(  Token & tok)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return this;
}
int ErrorState::getStatus()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return State::IsErr;
}
///EndState Class
State* EndState::instance = NULL;
State* EndState::getInstance()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (instance == NULL)
		instance = new EndState();
	return instance;
}
State * EndState::doTransition(  Token & tok)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return this;
}
int EndState::getStatus()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return State::IsEnd;
}
///NoMoreState Class
State* NoMoreState::instance = NULL;
State* NoMoreState::getInstance()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (instance == NULL)
		instance = new NoMoreState();
	return instance;
}
State * NoMoreState::doTransition(  Token & tok)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (!tok.testIsNULL()) {
		return ErrorState::getInstance( );
	} else
		return EndState::getInstance();
}
///WaitFirstTokenState Class
State* WaitFirstTokenState::instance = NULL;
State* WaitFirstTokenState::getInstance()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (instance == NULL)
		instance = new WaitFirstTokenState();
	return instance;
}
State * WaitFirstTokenState::doTransition(  Token & tok)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (tok.testIsNULL()) {
		return EndState::getInstance();
	} else if (tok.testIsBuiltin()) {
		return WaitBltinARGState::getInstance();
	} else if (tok.testIsCMD()) {
		return WaitARGBPState::getInstance();
	} else {
		return ErrorState::getInstance();
	}
}
///WaitCMDState Class
State* WaitCMDState::instance = NULL;
State* WaitCMDState::getInstance()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (instance == NULL)
		instance = new WaitCMDState();
	return instance;
}
State * WaitCMDState::doTransition(  Token & tok)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (tok.testIsNULL())
		return ErrorState::getInstance();
	else if (tok.testIsBuiltin()) {
		return ErrorState::getInstance();
	} else if (tok.testIsCMD()) {
		return WaitARGState::getInstance();
	} else {
		return ErrorState::getInstance();
	}
}
/// WaitARGState Class
State* WaitARGState::instance = NULL;
State* WaitARGState::getInstance()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (instance == NULL)
		instance = new WaitARGState();
	return instance;
}
State * WaitARGState::doTransition(  Token & tok)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (tok.testIsNULL())
		return EndState::getInstance();
	else if (tok.testIsARG()) {
		return WaitARGState::getInstance();
	} else if (tok.testIsPipe()) {
		return WaitCMDState::getInstance();
	} else if (tok.testIsOR()) {
		return WaitOutFileAPState::getInstance();
	} else {
		return ErrorState::getInstance();
	}
}
/// WaitARGBPState Class
State* WaitARGBPState::instance = NULL;
State* WaitARGBPState::getInstance()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (instance == NULL)
		instance = new WaitARGBPState();
	return instance;
}
State * WaitARGBPState::doTransition(  Token & tok)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (tok.testIsNULL())
		return EndState::getInstance();
	else if (tok.testIsARG()) {
		return WaitARGBPState::getInstance();
	} else if (tok.testIsPipe()) {
		return WaitCMDState::getInstance();
	} else if (tok.testIsIR()) {
		return WaitInFileState::getInstance();
	} else if (tok.testIsOR()) {
		return WaitOutFileState::getInstance();
	} else {
		return ErrorState::getInstance();
	}
}
/// WaitBltinARGState Class
State* WaitBltinARGState::instance = NULL;
State* WaitBltinARGState::getInstance()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (instance == NULL)
		instance = new WaitBltinARGState();
	return instance;
}
State * WaitBltinARGState::doTransition(  Token & tok)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (tok.testIsNULL()) {
		return EndState::getInstance( );
	} else if (tok.testIsARG()) {
		return WaitBltinARGState::getInstance();
	}
	else {
		return ErrorState::getInstance();
	}
}
/// WaitOneARGState Class
State* WaitOneARGState::instance = NULL;
State* WaitOneARGState::getInstance()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (instance == NULL)
		instance = new WaitOneARGState();
	return instance;
}
State * WaitOneARGState::doTransition(  Token & tok)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (tok.testIsNULL()) {
		return ErrorState::getInstance( );
	} else if (tok.testIsARG()) {
		return NoMoreState::getInstance();
	}
	else {
		return ErrorState::getInstance();
	}
}
/// WaitInFileState Class
State* WaitInFileState::instance = NULL;
State* WaitInFileState::getInstance()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (instance == NULL)
		instance = new WaitInFileState();
	return instance;
}
State * WaitInFileState::doTransition(  Token & tok)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (tok.testIsNULL())
		return ErrorState::getInstance();
	else if (tok.testIsInFile()) {
		return WaitORState::getInstance();
	}
	else {
		return ErrorState::getInstance();
	}
}
/// WaitOutFileState Class
State* WaitOutFileState::instance = NULL;
State* WaitOutFileState::getInstance()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (instance == NULL)
		instance = new WaitOutFileState();
	return instance;
}
State * WaitOutFileState::doTransition(  Token & tok)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (tok.testIsNULL())
		return ErrorState::getInstance();
	else if (tok.testIsOutFile()) {
		return WaitIRState::getInstance();
	}
	else {
		return ErrorState::getInstance();
	}
}
/// WaitOutFileState Class
State* WaitOutFileAPState::instance = NULL;
State* WaitOutFileAPState::getInstance()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (instance == NULL)
		instance = new WaitOutFileAPState();
	return instance;
}
State * WaitOutFileAPState::doTransition(  Token & tok)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (tok.testIsNULL())
		return ErrorState::getInstance();
	else if (tok.testIsOutFile()) {
		return NoMoreState::getInstance();
	}
	else {
		return ErrorState::getInstance();
	}
}

/// WaitLastInFileState Class

State* WaitLastInFileState::instance = NULL;
State* WaitLastInFileState::getInstance()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (instance == NULL)
		instance = new WaitLastInFileState();
	return instance;
}
State * WaitLastInFileState::doTransition(  Token & tok)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (tok.testIsNULL())
		return ErrorState::getInstance();
	else if (tok.testIsInFile()) {
		return NoMoreState::getInstance();
	}
	else {
		return ErrorState::getInstance();
	}
}
/// WaitLastOutFileState Class
State* WaitLastOutFileState::instance = NULL;
State* WaitLastOutFileState::getInstance()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (instance == NULL)
		instance = new WaitLastOutFileState();
	return instance;
}
State * WaitLastOutFileState::doTransition(  Token & tok)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (tok.testIsNULL())
		return ErrorState::getInstance();
	else if (tok.testIsOutFile()) {
		return NoMoreState::getInstance();
	}
	else {
		return ErrorState::getInstance();
	}
}
/// WaitORState Class
State* WaitORState::instance = NULL;
State* WaitORState::getInstance()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (instance == NULL)
		instance = new WaitORState();
	return instance;
}
State * WaitORState::doTransition(  Token & tok)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (tok.testIsNULL())
		return EndState::getInstance();
	else if (tok.testIsPipe()) {
		return WaitCMDState::getInstance();
	}
	else if (tok.testIsOR()) {
		return WaitLastOutFileState::getInstance();
	}
	else {
		return ErrorState::getInstance();
	}
}
/// WaitIRState Class
State* WaitIRState::instance = NULL;
State* WaitIRState::getInstance()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (instance == NULL)
		instance = new WaitIRState();
	return instance;
}
State * WaitIRState::doTransition(  Token & tok)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (tok.testIsNULL()) 
		return EndState::getInstance();
	else if (tok.testIsIR()) {
		return WaitLastInFileState::getInstance();
	}
	else {
		return ErrorState::getInstance();
	}
}
