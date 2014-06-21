#ifndef __EXPR_BUILDER_H__
#define __EXPR_BUILDER_H__

#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "boost/noncopyable.hpp"
#include "boost/scoped_ptr.hpp"

namespace kfl {

  template<class T>
  class ExprBuilder {
    typedef std::string Id;
    typedef int Num;
    static Expr<T> * createVar(const Id & name);
    static Expr<T> * createNum(const Num & n);
  };

}

#endif /* __EXPR_BUILDER_H__ */
