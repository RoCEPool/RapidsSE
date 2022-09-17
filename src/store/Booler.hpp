/*============================================================================*/
/* Booler.hpp --                                                              */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Dissector.hpp"

namespace STORE
{
  class Booler : public Object
  {
  private:

    Card_SubSet* m_pCard_SubSet;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Booler(Card_SubSet* pCard_SubSet);
   ~Booler(void) noexcept(false);

    void vBool(void);

  private:

    Footer* aFooter(void);
  };

  Booler* _pBooler(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
