/*============================================================================*/
/* Card_OrdAgg.hpp -- Ordered Aggregate                                       */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_Project.hpp"
#include "eAL.h"

namespace STORE
{
  class Card_OrdAgg : public Card_Project
  {  
  protected:

  private:

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_OrdAgg(eCK eKind, Stz* szLevel);
   ~Card_OrdAgg(void) noexcept(false);

  private:
    static void vDeleteThis_(Card* pCard);
    
    Footer* aFooter(void);
  };

  Card_OrdAgg* _pCard_OrdAgg(void* aAddr);
  bool _bIsCard_OrdAgg(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
