/*============================================================================*/
/* Card_KeyNotExists.hpp -- Key Not Exists                                    */
/*============================================================================*/
#pragma once

#include "Card_COND.hpp"

namespace STORE
{
  class Card_KeyNotExists : public Card_COND
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_KeyNotExists(void);
   ~Card_KeyNotExists(void) noexcept(false);

    void vComputeProjection(CardIter* pCI); // prototype

    void vEarlyProject_add_transients(CardIter* pCI);

    void vEarlyProject_recurse_add(CardIter* pCI);

    static void vMorph_table_To_keyNotExists(CardIter* pCI);

    void vUpdate_projection(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_KeyNotExists* _pCard_KeyNotExists(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
