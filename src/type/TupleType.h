#ifndef __TUPLETYPE_H__
#define __TUPLETYPE_H__

#include <string>
#include <vector>

#include "Type.h"

namespace kfl {

  class TupleType : public Type
  {
    typedef Type super;
  public:
    typedef std::vector<Type*> TypeVector;
    TupleType();
    TupleType(const TypeVector & types);
    virtual ~TupleType();
    virtual const std::string & name() const;
  protected:
    virtual std::string computeName() const;
  private:
    const TypeVector m_types;
    mutable std::string m_cachedName;
  };

} /* end namespace kfl */

#endif /* __TUPLETYPE_H__ */
