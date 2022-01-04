/*============================================================================*/
/* Card_Project.hpp -- List of Project Cards                              */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_Unit.hpp"

namespace STORE
{
  class Card_Project : public Card_Unit
  {  
  protected:
    
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Project(eCK eKind);
    Card_Project(eCK eKind, eCN eClassNo, Stz* szCardNm);
   ~Card_Project(void) noexcept(false);

  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
    
    Footer* aFooter(void);
  };

  Card_Project* _pCard_Project(void* aAddr);
  bool _bIsCard_Project(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
