/*============================================================================*/
/* Card_TableList.hpp -- List of Table Cards                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_TableList : public Card
  {  
  protected:
    
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_TableList(eCK eKind);
   ~Card_TableList(void) noexcept(false);

  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
    
    Footer* aFooter(void);
  };

  Card_TableList* _pCard_TableList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
