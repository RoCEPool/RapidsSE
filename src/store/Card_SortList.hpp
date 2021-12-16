/*============================================================================*/
/* Card_SortList.hpp -- List of Sort Cards                                    */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_SortList : public Card
  {  
  protected:
    
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_SortList(eCK eKind);
   ~Card_SortList(void) noexcept(false);

  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
    
    Footer* aFooter(void);
  };

  Card_SortList* _pCard_SortList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
