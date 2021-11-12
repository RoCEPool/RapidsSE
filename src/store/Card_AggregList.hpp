/*============================================================================*/
/* Card_AggregList.hpp -- List of Aggregate Cards                               */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_AggregList : public Card
  {  
  protected:
    
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_AggregList(void);
   ~Card_AggregList(void) noexcept(false);

  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
    
    Footer* aFooter(void);
  };

  Card_AggregList* _pCard_AggregList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
