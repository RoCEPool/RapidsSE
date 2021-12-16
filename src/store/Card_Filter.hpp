/*============================================================================*/
/* Card_Filter.hpp -- Filter Card                                             */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_Unit.hpp"

namespace STORE
{
  class Card_Filter : public Card_Unit
  {
  protected:
  
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Filter(eCK eKind);
   ~Card_Filter(void) noexcept(false);

   private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);

    Footer* aFooter(void);
  };

  Card_Filter* _pCard_Filter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
