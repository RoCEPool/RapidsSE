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

  private:

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Sort(eCK eKind, Column* pColumn, Table* pTable);
   ~Card_Sort(void) noexcept(false);

  private:
    static void vDeleteThis_(Card* pCard);
  };

  Card_Sort* _pCard_Sort(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
