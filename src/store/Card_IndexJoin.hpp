/*============================================================================*/
/* Card_IndexJoin.hpp -- Index Join Card                                     */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_Unit.hpp"
#include "Table.hpp"

namespace STORE
{
  class Card_IndexJoin : public Card_Unit
  {
  protected:

  private:

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_IndexJoin(eCK eKind, Index* pIndex);
   ~Card_IndexJoin(void) noexcept(false);


  private:
    static void vDeleteThis_(Card* pCard);

    Footer* aFooter(void);
  };

  Card_IndexJoin* _pCard_IndexJoin(void* aAddr);
  bool _bIsCard_IndexJoin(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
