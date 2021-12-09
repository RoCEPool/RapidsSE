/*============================================================================*/
/* Card_Sort.hpp -- Sort Card                                                 */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"
#include "Table.hpp"

namespace STORE
{
  class Card_Sort : public Card
  {
  protected:
    Column* m_pColumn;
    Table*  m_pTable;

  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Sort(Column* pColumn, Table* pTable);
   ~Card_Sort(void) noexcept(false);

    Column* pColumn(void);
    Table*  pTable(void);

  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);

    Footer* aFooter(void);
  };

  Card_Sort* _pCard_Sort(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
