#ifndef __LISTTYPE_H__
#define __LISTTYPE_H__

#include <string>

#include "NamedType.h"

namespace kfl {

  class ListType : public NamedType
  {
    typedef NamedType super;
  public:
    static const std::string cName;
    ListType();
    virtual ~ListType();
  };

} /* end namespace kfl */

#endif /* __LISTTYPE_H__ */
