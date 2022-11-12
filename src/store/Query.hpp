/*============================================================================*/
/* Query.hpp -- Query analysis                                                */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class Query : public Object
  {
  protected:

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Query(void);
   ~Query(void) noexcept(false);


  protected:
    Footer* aFooter(void);
  };

  Query* _pQuery(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
