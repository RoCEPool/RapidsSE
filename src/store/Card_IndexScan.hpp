/*============================================================================*/
/* Card_IndexScan.hpp -- Index Scan Card                                      */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_Unit.hpp"
#include "Table.hpp"

namespace STORE
{
  class Card_IndexScan : public Card_Unit
  {
  protected:

  private:

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_IndexScan(eCK eKind, Index* pIndex);
   ~Card_IndexScan(void) noexcept(false);

  private:
    static void vDeleteThis_(Card* pCard);

    Footer* aFooter(void);
  };

  Card_IndexScan* _pCard_IndexScan(void* aAddr);
  bool _bIsCard_IndexScan(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
