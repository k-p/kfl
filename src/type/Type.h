#ifndef __TYPE_H__
#define __TYPE_H__

#include <ostream>
#include <string>

namespace kfl {

  class Type
  {
  public:
    virtual ~Type();

    inline bool operator==(const Type & rhs) const {
      return rhs.equals(*this);
    }

    virtual const std::string & name() const = 0;

  protected:
    virtual bool equals(const Type & other) const;
  };

}  /* end namespace kfl */

inline std::ostream & operator<<(std::ostream & os, const kfl::Type & type) {
  return os << "[Type: " << type.name() << ']';
}

#endif /* __TYPE_H__ */
