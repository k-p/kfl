#include "Exception.h"

kfl::Exception::Exception(const char * msg)
  : m_msg(msg)
{
}

kfl::Exception::~Exception()
{
}

const std::string & kfl::Exception::msg() const
{
  return m_msg;
}

