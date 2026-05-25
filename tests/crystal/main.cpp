#include "../../src/crystal/body.hpp"
#include <iostream>

struct Example final: public Bismuth::API {
  Reference construct(Runtime* instance) override {
    Reference exports = Value::Empty(Type::Table);

    exports->SET("string", Value::Make("Nice!"));
    exports->SET(
      "test",
      Block::Bound([](Array arguments) {
        Reference table = Value::Empty(Type::Table);
        table->SET("subtest", Value::Make(200));
        return table;
      })
    );

    return exports;
  }
};

extern "C" BOOST_SYMBOL_EXPORT Example crystal;
Example crystal;