#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <string>

namespace kfl {

  class Exception {
  public:
    Exception(const char * msg);
    virtual ~Exception();
    const std::string & msg() const;

  private:
    const std::string m_msg;
  };

} /* end namespace kfl */

#endif /* __EXCEPTION_H__ */
