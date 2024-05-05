/*============================================================================*/
/* Card_KeyExists.hpp -- Key Exists                                           */
/*============================================================================*/
#pragma once

#include "Card_CONDITION.hpp"

namespace STORE
{
  class Card_KeyExists : public Card_CONDITION
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_KeyExists(void);
   ~Card_KeyExists(void) noexcept(false);

    void vAdd_transients(CardIter* pCI);

    static void vMorph_from_To_keyExists(CardIter *pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);


  };

  Card_KeyExists* _pCard_KeyExists(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
