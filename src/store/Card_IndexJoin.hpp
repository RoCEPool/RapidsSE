/*============================================================================*/
/* Card_IndexJoin.hpp -- Index Join Card                                     */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"
#include "Table.hpp"

namespace STORE
{
  class Card_IndexJoin : public Card
  {
  protected:
    Table* m_pTable;
    Index* m_pIndex;

  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_IndexJoin(Table* pTable, Index* pIndex);
   ~Card_IndexJoin(void) noexcept(false);

    Index* pIndex(void);
    void vIndex(Index* pIndex);

    Table* pTable(void);
    void vTable(Table* pTable);

  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);

    Footer* aFooter(void);
  };

  Card_IndexJoin* _pCard_IndexJoin(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
