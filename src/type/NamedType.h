#ifndef __NAMEDTYPE_H__
#define __NAMEDTYPE_H__

#include "Type.h"

#include <string>

namespace kfl {

  class NamedType : public Type
  {
    using super = Type;

  public:
    const std::string & name() const override;

  protected:
    NamedType(const std::string & name);

  private:
    const std::string m_name;
  };

}  /* end namespace kfl */

#endif /* __NAMEDTYPE_H__ */
