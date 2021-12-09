/*============================================================================*/
/* Card_Index.hpp -- Index Card                                               */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"
#include "Table.hpp"

namespace STORE
{
  class Card_Index : public Card
  {
  protected:
    Index* m_pIndex;
  
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Index(Index* pIndex);
   ~Card_Index(void) noexcept(false);

    Index* pIndex(void);
  
    void vPrintIndex(Ribbon* pRibbon);

  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);

    Footer* aFooter(void);
  };

  Card_Index* _pCard_Index(void* aAddr);
  bool _bIsCard_Index(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
