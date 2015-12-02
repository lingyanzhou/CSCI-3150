#include <stdlib.h>
#include <string>
#include <string.h>
#include <list>
#include <glob.h>
#include <stdexcept>
#include <iostream>
#include "Token.hpp"
#include "TokenList.hpp"

void TokenList::doGlob(std::list<Token> &tokenList)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	if (tokenList.empty()) return;
	for (std::list<Token>::iterator it = tokenList.begin(); it!=tokenList.end(); ++it) {
		if (it->isARGG()) {
			glob_t globbuf;
			if (GLOB_NOMATCH == glob(it->getValue().c_str(), 0, NULL, &globbuf)) {
				it->setType(Token::IsARGNG);
				globfree( &globbuf);
			} else {
				it = tokenList.erase(it);
				for (size_t i=0; i< globbuf.gl_pathc; i++) {
					tokenList.insert(it, Token( globbuf.gl_pathv[i], Token::IsARGNG));
				}
        --it;
			}
		}
	}
}
char ** TokenList::to2DArray(std::list<Token> &tokenList)
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	char ** ret = new char*[tokenList.size()+1];
	int i = 0;
	for (std::list<Token>::iterator it = tokenList.begin(); it!=tokenList.end(); ++it ) {
		const char * tokVal = it->getValue().c_str();
		size_t valLen = it->getValue().length();
		ret[i] = new char[valLen+1];
		strcpy(ret[i], tokVal);
		i++;
	}
	ret[tokenList.size()] = NULL;
#ifdef DEBUG
	for (size_t j = 0; j<tokenList.size(); ++j) {
		std::cout<<ret[j]<<" ";
	}
	std::cout<<std::endl;
#endif
	return ret;
}
