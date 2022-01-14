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
  
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Table(eCK eKind, Table* pTable);
   ~Card_Table(void) noexcept(false);

    static void vDeleteThis_(Card* pCard);

  private:
    Footer* aFooter(void);
  };

  Card_Table* _pCard_Table(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
