/*============================================================================*/
/* AbstractDef.hpp -- Abstract Definition                                     */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class AbstractDef : public Object
  {
  public:
    s256 m_sFile;
    i8   m_iLine;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

     AbstractDef(eCN eClass, const char* azFile=nullptr, i8 iLine=C_UNKNOWN);
    ~AbstractDef(void) noexcept(false);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
