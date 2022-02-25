/*============================================================================*/
/* Card_IndexScan.hpp -- Index Scan                                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_MAIN.hpp"

namespace STORE
{
  class Card_IndexScan : public Card_MAIN
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_IndexScan(void);
   ~Card_IndexScan(void) noexcept(false);
  };

  Card_IndexScan* _pCard_IndexScan(void* aAddr);
  bool _bIsCard_IndexScan(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
