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

    void vComputeProjection(CardIter* pCI); // prototype

    void vEarlyProject_add_transients(CardIter* pCI);

    void vEarlyProject_recurse_add(CardIter* pCI);

    static void vMorph_table_To_keyExists(CardIter* pCI);

    void vUpdate_emitted(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);


  };

  Card_KeyExists* _pCard_KeyExists(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
