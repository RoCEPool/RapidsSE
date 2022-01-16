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

  private:

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_On(eCK eKind,
            Column* pColumn1, Table* pTable1,
            Column* pColumn2, Table* pTable2);
   ~Card_On(void) noexcept(false);

  private:
    static void vDeleteThis_(Card* pCard);
  
    Footer* aFooter(void);
  };

  Card_On* _pCard_On(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
