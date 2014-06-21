#ifndef __NAMEDTYPE_H__
#define __NAMEDTYPE_H__

#include <string>

#include "Type.h"

namespace kfl {

  class NamedType : public Type
  {
    typedef Type super;
  public:
    virtual const std::string & name() const;
  protected:
    NamedType(const std::string & name);
  private:
    const std::string m_name;
  };

}  /* end namespace kfl */

#endif /* __NAMEDTYPE_H__ */
