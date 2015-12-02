#ifndef TOKENLIST_HPP
#define TOKENLIST_HPP
#include <list>
#include "Token.hpp"

class TokenList {
	public :
		static void doGlob(std::list<Token> &tokenList);
		static char ** to2DArray(std::list<Token> &tokenList);
};
#endif
