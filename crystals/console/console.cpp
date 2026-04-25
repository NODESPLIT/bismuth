#include "../../src/crystal/body.hpp"
#include <iostream>

class Console final: public API {
  public:
    Reference construct() override {
      Reference output = Value::Empty(Type::Table);

      output->SET(
        "clear",
        Block::Bound([](Array arguments) {
          std::cout << "\b";
          return Value::Empty();
        })
      );

      return output;
    }
};

extern "C" BOOST_SYMBOL_EXPORT Console crystal;
Console crystal;