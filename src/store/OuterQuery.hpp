/*============================================================================*/
/* OuterQuery.hpp -- Outer Query                                              */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class OuterQuery : public Object
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    OuterQuery(void);
   ~OuterQuery(void) noexcept(false);

  private:
    Footer* aFooter(void);
  };

  OuterQuery* _pOuterQuery(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
