/*============================================================================*/
/* Card_OnList.hpp -- List of On Cards                                        */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_OnList : public Card
  {  
  protected:
    
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_OnList(void);
   ~Card_OnList(void) noexcept(false);
    
  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
    
    Footer* aFooter(void);
  };

  Card_OnList* _pCard_OnList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
