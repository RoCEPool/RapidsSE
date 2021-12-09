/*============================================================================*/
/* Card_By.hpp -- By Card                                                     */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"
#include "Table.hpp"
#include "eAL.h"

namespace STORE
{
  class Card_By : public Card
  {
  private:
    Column* m_pColumn;
    Table*  m_pTable;
  
    eAL     m_eAggLevel;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_By(void);
    Card_By(Column* pColumn, Table* pTable);
   ~Card_By(void) noexcept(false);

    eAL     eAggLevel(void);
    void    vAggLevel(eAL eAggLevel);
    Column* pColumn(void);
    Table*  pTable(void);

  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);

    Footer* aFooter(void);
  };

  Card_By* _pCard_By(void* aAddr);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
