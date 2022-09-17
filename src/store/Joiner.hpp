/*============================================================================*/
/* Joiner.hpp --                                                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Dissector.hpp"

namespace STORE
{
  class Joiner : public Object
  {
  private:

    Card_SubSet* m_pCard_SubSet;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Joiner(Card_SubSet* pCard_SubSet);
  
   ~Joiner(void) noexcept(false);


  private:

    Footer* aFooter(void);
  };

  Joiner* _pJoiner(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
