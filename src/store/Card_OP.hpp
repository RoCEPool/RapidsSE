/*============================================================================*/
/* Card_OP.hpp -- Abstract Op                                                 */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_OP : public Card
  {  
  public:
    Card_OP(eCK eCard);
   ~Card_OP(void) noexcept(false);

    static bool bIsOp(Card* pCard);

    void vAddLevel(void);
    void vAdd_Pred_Under_filter(Card_PRED *p_PRED);

    void vConvert_rpnCalc_To_boolCalc(CardIter* pCI);
    void vConvert_stmColOdn_To_tblColOdn(CardIter* pCI);
    void vConvert_tblColOdn_To_stmColOdn(CardIter* pCI);

    void vCorrectStmOdn(Card_Projection* p_projection_prior);

    void vDeleteFilterIfEmpty(Card_Filter* p_filter);

    void vEarlyProject_root(CardIter* pCI);
    void vEarlyProject_backstep(CardIter* pCI);

    bool bIsEarlyLeaf(CardIter* pCI);
    bool bIsEarlyRoot(void);
    bool bIsNodeLevel(void);
    bool bIsOriginLevel(void);
    bool bIsPartitionLevel(void);

    bool bNeedsProjectionAnalysis(void);
    bool bNeedsProjectionCorrection(void);

    bool bPeg_Chosen_index(CardIter* pCI);
    bool bPeg_Key(CardIter* pCI);
    bool bPeg_Projection(CardIter* pCI);

    void vRelegate_table(CardIter* pCI, Card_OP* p_under);

    void vUpdate_projection(CardIter* pCI);
  };

  Card_OP* _pCard_OP(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
