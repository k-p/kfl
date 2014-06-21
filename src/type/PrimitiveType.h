#include "NamedType.h"

namespace kfl {

  class PrimitiveType : public NamedType
  {
    typedef NamedType super;
  protected:
    PrimitiveType(const std::string & name);
  };

  class IntType : public PrimitiveType
  {
    typedef PrimitiveType super;
  public:
    IntType();
  private:
    static const std::string typeName;
  };

} /* end namespace kfl */
