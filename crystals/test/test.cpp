#include "../../src/crystal/body.hpp"
#include <iostream>

class Testing final: public API {
  public:
    Reference construct() override {
      Reference output = Value::Empty(Type::Table);

      output->SET(
        "string",
        Value::Make("Nice!")
      );

      output->SET(
        "test",
        Block::Bound([](Array arguments) {
          Reference table = Value::Empty(Type::Table);
          table->SET("subtest", Value::Make(200));
          return table;
        })
      );

      return output;
    }
};

extern "C" BOOST_SYMBOL_EXPORT Testing crystal;
Testing crystal;