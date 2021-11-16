/*============================================================================*/
/* Card_ProjectList.hpp -- List of Project Cards                              */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_ProjectList : public Card
  {  
  protected:
    
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_ProjectList(void);
   ~Card_ProjectList(void) noexcept(false);

  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
    
    Footer* aFooter(void);
  };

  Card_ProjectList* _pCard_ProjectList(void* aAddr);
  bool _bIsCard_ProjectList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
