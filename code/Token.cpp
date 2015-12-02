#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "Token.hpp"

const char * const Token::DelimString  = " \n\t\r";
const char * const Token::CMDRejString = " \t><|*!`'\"";
const char * const Token::ARGRejString = " \t><|!`'\"";
const char * const Token::GlobAcpString= "*";
const char * const Token::FileRejString= " \t><|*!`'\"";
const char * const Token::IRAcpString  = "<";
const char * const Token::ORTAcpString = ">";
const char * const Token::ORAAcpString = ">>";
const char * const Token::PipeAcpString= "|";

const char * const Token::builtin[] = {"cd", "fg", "jobs", "exit"};

const unsigned int Token::IsUnknown = 0;
const unsigned int Token::IsBuiltin = 0x0001;
const unsigned int Token::IsCMD = 0x0002;
const unsigned int Token::IsARGNG = 0x0004;
const unsigned int Token::IsARGG = 0x0008;
const unsigned int Token::IsARG = Token::IsARGNG | Token::IsARGG ;
const unsigned int Token::IsInFile = 0x0010;
const unsigned int Token::IsOutFile = 0x0020;
const unsigned int Token::IsInR = 0x0040;
const unsigned int Token::IsOutRT = 0x0080;
const unsigned int Token::IsOutRA = 0x0100;
const unsigned int Token::IsOutR = Token::IsOutRT | Token::IsOutRA;
const unsigned int Token::IsPipe = 0x0200;

const char* const Token::UnknownMsg = "Unknown";
const char* const Token::BuiltinMsg = "Built-in Command";
const char* const Token::CMDMsg = "Command Name";
const char* const Token::ARGNGMsg = "Argument (No Glob)";
const char* const Token::ARGGMsg = "Argument (Glob)";
const char* const Token::ARGMsg = "Argument";
const char* const Token::InFileMsg = "Input Filename";
const char* const Token::OutFileMsg = "Output Filename";
const char* const Token::IRMsg = "Redirect Input";
const char* const Token::ORTMsg = "Redirect Output (Truncate)";
const char* const Token::ORAMsg = "Redirect Output (Append)";
const char* const Token::ORMsg = "Redirect Output";
const char* const Token::PipeMsg = "Pipe";


Token::Token(const std::string & val, const int t)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	value = val;
	type=t;
}
Token::Token(const Token & t)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	this->value = t.value;
	this->type= t.type;
}
Token::~Token()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
}
Token& Token::operator=(const Token& t)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	this->value = t.value;
	this->type= t.type;
	return *this;
}
void Token::setValue(const std::string & val)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	value = val;
}
const std::string & Token::getValue() const
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return value;
}
void Token::setType(const int t)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	type = t;	
}
unsigned int Token::getType() const
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return type;	
}
const char * Token::getTypeStringSimple() const
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (type == Token::IsUnknown)
		return Token::UnknownMsg;
	else if (type == Token::IsBuiltin)
		return Token::BuiltinMsg;
	else if (type == Token::IsCMD)
		return Token::CMDMsg;
	else if (type & Token::IsARG)
		return Token::ARGMsg;
	else if (type == Token::IsInFile)
		return Token::InFileMsg;
	else if (type == Token::IsOutFile)
		return Token::OutFileMsg;
	else if (type == Token::IsInR)
		return Token::IRMsg;
	else if (type & Token::IsOutR)
		return Token::ORMsg;
	else if (type == Token::IsPipe)
		return Token::PipeMsg;
	else 
		return Token::UnknownMsg;;
}
const char * Token::getTypeStringFull() const
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (type == Token::IsUnknown)
		return Token::UnknownMsg;
	else if (type & Token::IsBuiltin)
		return Token::BuiltinMsg;
	else if (type & Token::IsCMD)
		return Token::CMDMsg;
	else if (type & Token::IsARGNG)
		return Token::ARGNGMsg;
	else if (type & Token::IsARGG)
		return Token::ARGGMsg;
	else if (type & Token::IsInFile)
		return Token::InFileMsg;
	else if (type & Token::IsOutFile)
		return Token::OutFileMsg;
	else if (type & Token::IsInR)
		return Token::IRMsg;
	else if (type & Token::IsOutRT)
		return Token::ORTMsg;
	else if (type & Token::IsOutRA)
		return Token::ORAMsg;
	else if (type & Token::IsPipe)
		return Token::PipeMsg;
	else 
		return Token::UnknownMsg;;
}
bool Token::testIsNULL( )
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return (value.empty());
}
bool Token::testIsBuiltin( )
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if ( testIsNULL() )
		return false;
	if (NULL==Token::builtin)
		return false;
	size_t n = sizeof(builtin)/sizeof(Token::builtin[0]);
	for (size_t i=0; i<n; i++) {
		if (0== value.compare( Token::builtin[i])) {
			type = Token::IsBuiltin;
			return true;
		}
	}
	return false;
}
bool Token::testIsCMD( )
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if ( testIsNULL() ) return false;
	if ( std::string::npos==value.find_first_of(Token::CMDRejString)) {
		type = Token::IsCMD;
		return true;
	}
	return false;
}
bool Token::testIsARG( )
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if ( testIsNULL() ) return false;
	if ( std::string::npos==value.find_first_of(Token::ARGRejString)) {
		if ( std::string::npos!=value.find_first_of(Token::GlobAcpString)) {
			type = Token::IsARGG;
		} else {
			type = Token::IsARGNG;
		}
		return true;
	}
	return false;
}
bool Token::testIsFile( )
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if ( this->testIsNULL() ) return false;
	if ( std::string::npos==value.find_first_of(Token::FileRejString)) {
		return true;
	}
	return false;
}
bool Token::testIsInFile( )
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (this->testIsFile()) {
		type = Token::IsInFile;
		return true;
	}
	return false;
}
bool Token::testIsOutFile( )
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (this->testIsFile()) {
		type = Token::IsOutFile;
		return true;
	}
	return false;
}
bool Token::testIsIR( )
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if ( this->testIsNULL() ) return false;
	if ( 0==value.compare(Token::IRAcpString)) {
		type = Token::IsInR;
		return true;
	}
	return false;
}
bool Token::testIsOR( )
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if ( this->testIsNULL() ) return false;
	if ( 0==value.compare(Token::ORTAcpString)) {
		type = Token::IsOutRT;
		return true;
	} else if (0==value.compare(Token::ORAAcpString)) {
		type = Token::IsOutRA;
		return true;
	}
	return false;
}
bool Token::testIsPipe()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if ( this->testIsNULL() ) return false;
	if ( 0==value.compare(Token::PipeAcpString)) {
		type = Token::IsPipe;
		return true;
	}
	return false;
}
bool Token::isBuiltin( ) const
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return (Token::IsBuiltin == type );
}
bool Token::isCMD( ) const
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return (type == Token::IsCMD);
}
bool Token::isARG( ) const
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return (type & Token::IsARG)!=0;
}
bool Token::isARGG( ) const
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return (type & Token::IsARGG)!=0;
}
bool Token::isInFile( ) const
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return (type == Token::IsInFile);
}
bool Token::isOutFile( ) const
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return (type == Token::IsOutFile);
}
bool Token::isIR( ) const
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return (type == Token::IsInR);
}
bool Token::isORT( ) const
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return (type == Token::IsOutRT);
}
bool Token::isORA( ) const
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return (type == Token::IsOutRA);
}
bool Token::isPipe() const
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	return (type == Token::IsPipe);
}
