#include <string>
#include <regex>
#include <stack>
#include <utility>

#include <boost/algorithm/string.hpp>
#include <boost/container/vector.hpp>
#include <boost/unordered_map.hpp>
#include <tsl/ordered_map.h>

#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>

namespace algorithm = boost::algorithm;

template <class L, class R> using unordered_map = boost::unordered_map<L, R>;
template <class T> using vector = boost::container::vector<T>;
template <class L, class R> using tuple = boost::tuple<L, R>;

using std::function;

using std::string;
using std::stringstream;
using std::numeric_limits;
using std::regex;
using std::stack;

using boost::make_tuple;
using boost::shared_ptr;
using boost::any;

#include "utils.hpp"