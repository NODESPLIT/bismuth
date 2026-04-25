#include <boost/config.hpp>
#include <string>

class BOOST_SYMBOL_VISIBLE API {
public:
   virtual Reference construct(Runtime* instance) = 0;
   virtual ~API() = default;
};