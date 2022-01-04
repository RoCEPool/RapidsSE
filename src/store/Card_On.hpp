/*============================================================================*/
/* Card_On.hpp -- On Card                                                     */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"
#include "Table.hpp"

namespace STORE
{
  class Card_On : public Card
  {
  protected:
    Column* m_pColumn1;
    Table*  m_pTable1;
    
    Column* m_pColumn2;
    Table*  m_pTable2;

  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_On(eCK eKind,
            Column* pColumn1, Table* pTable1,
            Column* pColumn2, Table* pTable2);
   ~Card_On(void) noexcept(false);

    Column* pColumn1(void);
    Column* pColumn2(void);

    Table*  pTable1(void);
    Table*  pTable2(void);

  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
  
    Footer* aFooter(void);
  };

  Card_On* _pCard_On(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
