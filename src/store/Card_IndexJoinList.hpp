/*============================================================================*/
/* Card_IndexJoinList.hpp -- List of IndexJoin Cards                          */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_IndexJoinList : public Card
  {  
  protected:
    
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_IndexJoinList(void);
   ~Card_IndexJoinList(void) noexcept(false);

  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
    
    Footer* aFooter(void);
  };

  Card_IndexJoinList* _pCard_IndexJoinList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
