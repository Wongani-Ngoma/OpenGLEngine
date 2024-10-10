// please killl me 
#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING // to make vs shut tf up about std::iterator derived classes
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS				   // in rapidjson, may not be necessary, if so delete so that vs doesent timid when real errors come about

#include "Engine/Engine.h"

int main() {
	Engine::init();
 	Engine::run();
	Engine::shutdown();
	return 0;
}
