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
  
  private:

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Index(eCK eKind, Index* pIndex);
   ~Card_Index(void) noexcept(false);
  
  private:
    static void vDeleteThis_(Card* pCard);

    Footer* aFooter(void);
  };

  Card_Index* _pCard_Index(void* aAddr);
  bool _bIsCard_Index(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
