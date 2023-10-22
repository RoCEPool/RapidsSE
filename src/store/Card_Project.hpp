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

    static void vUnproject_foldExpr(CardIter* pCI,
                                    CarryForward* pCarryForward);
    //static Card_Project* pNew_project(Card_Emit* p_prjection);

    void vBump_Rightwards(i8 nBumpBy);

    void vComputeProjection(CardIter* pCI); // prototype
    Card_Emit* pCraft_projection_early(CardIter* pCI);

    void vDedupe_as(void);

    void vDerive_streamKey(CardIter* pCI);

    void vEarlyProject_given_leaf(CardIter* pCI);
    void vEarlyProject_recurse_needed(CardIter* pCI);
    void vEarlyProject_root(CardIter* pCI);

    void vInject_foldExpr(CardIter* pCI);
    bool bIsCompound(void);
    bool bIsLiterals(void);

    void vSmooth_postAgg(CardIter* pCI, i8 nBumpBy);
    void vSmooth_postAgg(CardIter* pCI, CarryForward* pCarryForward);

    void vUpdate_projection_off_running(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Project* _pCard_Project(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
