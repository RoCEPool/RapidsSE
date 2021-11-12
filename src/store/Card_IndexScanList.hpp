/*============================================================================*/
/* Card_IndexScanList.hpp -- List of IndexScan Cards                          */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_IndexScanList : public Card
  {  
  protected:
    
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_IndexScanList(void);
   ~Card_IndexScanList(void) noexcept(false);

  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
    
    Footer* aFooter(void);
  };

  Card_IndexScanList* _pCard_IndexScanList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
