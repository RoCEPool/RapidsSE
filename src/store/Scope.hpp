/*============================================================================*/
/* Scope.hpp -- Table Scope                                                   */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "SubCall.hpp"

namespace STORE
{
  class Scope : public Object
  {
  private:

    // must be last
    Footer m_xFooter;
 
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Scope(void);
   ~Scope(void) noexcept(false);

  private:

    Footer* aFooter(void);
  };
  
  Scope* _pScope(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
