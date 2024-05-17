/*============================================================================*/
/* Card_KeyNotExists.hpp -- Key Not Exists                                    */
/*============================================================================*/
#pragma once

#include "Card_CONDITION.hpp"

namespace STORE
{
  class Card_KeyNotExists : public Card_CONDITION
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_KeyNotExists(void);
   ~Card_KeyNotExists(void) noexcept(false);

    static void vMorph_from_To_keyNotExists(CardIter* pCI);

    void vAdd_transients(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_KeyNotExists* _pCard_KeyNotExists(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
