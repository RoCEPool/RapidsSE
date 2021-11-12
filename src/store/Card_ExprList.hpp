/*============================================================================*/
/* Card_ExprList.hpp -- List of Expression Cards                              */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_ExprList : public Card
  {  
  protected:
    
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_ExprList(void);
   ~Card_ExprList(void) noexcept(false);

  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
    
    Footer* aFooter(void);
  };

  Card_ExprList* _pCard_ExprList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
