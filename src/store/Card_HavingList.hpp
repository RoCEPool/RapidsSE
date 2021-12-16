/*============================================================================*/
/* Card_HavingList.hpp -- List of Having Cards        Card_Filter                        */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_HavingList : public Card
  {  
  protected:
    
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_HavingList(eCK eKind);
   ~Card_HavingList(void) noexcept(false);

  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
    
    Footer* aFooter(void);
  };

  Card_HavingList* _pCard_HavingList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
