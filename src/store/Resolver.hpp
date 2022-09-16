/*============================================================================*/
/* Resolver.hpp --                                                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Bench.hpp"

namespace STORE
{
  class Resolver : public Object
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Resolver(TagSet* p_JobTags);
  
   ~Resolver(void) noexcept(false);

  private:

    Footer* aFooter(void);
  };

  Resolver* _pResolver(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
