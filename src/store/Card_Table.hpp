/*============================================================================*/
/* Card_Table.hpp -- Table                                                    */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Table : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Table(void);
    Card_Table(Stz* szTableNm);
   ~Card_Table(void) noexcept(false);

    void vBounds_DeriveFrom_filter(Card_Filter* p_filter,
                                   bool bParamAllowedInBounds);
  };

  Card_Table* _pCard_Table(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
