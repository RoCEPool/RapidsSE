/*============================================================================*/
/* Card_TblSyn.hpp -- Table Synonym                                           */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_TblSyn : public Card_symbol<Card_TblSyn>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_TblSyn(void);
   ~Card_TblSyn(void) noexcept(false);
  };

  Card_TblSyn* _pCard_TblSyn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
