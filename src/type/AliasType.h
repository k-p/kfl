#ifndef __ALIASTYPE_H__
#define __ALIASTYPE_H__

#include <string>

#include "NamedType.h"

namespace kfl {

  class AliasType : public NamedType
  {
    typedef NamedType super;
  public:
    AliasType(const std::string & name, const Type & type);
    virtual bool equals(const Type & other) const;
    const Type & type() const;
  private:
    const Type & m_type;
  };

}  /* end namespace kfl */

#endif /* __ALIASTYPE_H__ */
