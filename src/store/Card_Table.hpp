/*============================================================================*/
/* Card_Table.hpp -- Table Card                                              */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"
#include "Table.hpp"

namespace STORE
{
  class Card_Table : public Card
  {
  protected:
    Table* m_pTable;
    i8     m_jTableSize;
  
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Table(Table* pTable);
   ~Card_Table(void) noexcept(false);

    Table* pTable(void);
    i8     jTableSize(void);

    void vRemaindersPrint(Ribbon* pRibbon);

    void vPrintTable(Ribbon* pRibbon);
    void vPrintPartition(Ribbon* pRibbon);
    void vPrintPrimary(Ribbon* pRibbon);
    void vPrintSize(Ribbon* pRibbon);
    void vPrintColumns(Ribbon* pRibbon);

  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);

    Footer* aFooter(void);
  };

  Card_Table* _pCard_Table(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
