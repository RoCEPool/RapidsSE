/*============================================================================*/
/* Card_Project.hpp -- Project                                                */
/*============================================================================*/
#pragma once

#include "Card_OP.hpp"

namespace STORE
{
  class CarryForward;

  class Card_Project : public Card_OP
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Project(eCK eCard);
   ~Card_Project(void) noexcept(false);

    static void vUnproject_foldExpr(CardIter* pCI);
    static void vUnproject_rowRef(CardIter* pCI);

    Card_Emit* pCraft_emit_early(CardIter* pCI);

    void vDerive_streamKey(CardIter* pCI);

    void vSmooth_postAgg(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Project* _pCard_Project(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
