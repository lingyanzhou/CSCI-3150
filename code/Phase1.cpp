#include "Shell.hpp"
#include <memory>
#include <exception>
#include <iostream>
int main()
{
#ifdef DEBUG
	std::cerr<<__FILE__<<": "<<__func__<<": "<<__LINE__<<std::endl;
#endif
	std::unique_ptr<Shell> shellPtr( new Phase1Shell());
	try{
		shellPtr->init();
		shellPtr->mainloop();
	} catch (std::exception &e) {
		std::cout<<e.what()<<std::endl;
	}
}
