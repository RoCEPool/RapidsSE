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
    void vBump_Rightwards(i8 nBumpBy);

    void vComputeProjection(CardIter* pCI); // prototype
    void vConvert_stmColOdn_terms_To_tblColOdn(CardIter* pCI);

    void vInject_foldExpr(CardIter* pCI);
    void vSmooth(CardIter* pCI,
                 CarryForward* pCarryForward);
  };

  Card_Project* _pCard_Project(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
