#ifndef __TYPE_H__
#define __TYPE_H__

#include <string>

namespace kfl {

  class Type
  {
  public:
    virtual ~Type();
    virtual const std::string & name() const = 0;
    virtual bool equals(const Type & other) const;
  };

}  /* end namespace kfl */

#endif /* __TYPE_H__ */
