/*============================================================================*/
/* Card_OP.hpp -- Abstract Op                                                 */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "IndexDef.hpp"

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

    void vDeleteFilterIfEmpty(Card_Filter* p_filter);

    void vEarlyProject_root(CardIter* pCI);
    void vEarlyProject_backstep(CardIter* pCI);

    bool bIsDataInclusion(void);
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

    void vCraftArg(String* pString);


    void vRpnAggreg      (Stz* szRpnExpr, Card_Aggreg*    pCard_Aggreg);
    void vRpnCombined    (Stz* szRpnExpr, Card_Combined*  pCard_Combined);
    void vRpnCrossJoin   (Stz* szRpnExpr, Card_CrossJoin* pCard_CrossJoin);
    void vRpnFilter      (Stz* szRpnExpr, Card_Filter*    pCard_Filter);
    void vRpnGbyAgg      (Stz* szRpnExpr, Card_GbyAgg*    pCard_GbyAgg);
    void vRpnHaving      (Stz* szRpnExpr, Card_Having*    pCard_Having);
    void vRpnIndexJoin   (Stz* szRpnExpr, Card_Index*     pCard_Index);
    void vRpnIndexScan   (Stz* szRpnExpr, Card_Index*     pCard_Index);
    void vRpnKeyExists   (Stz* szRpnExpr, Card_KeyExists* pCard_KeyExists);
    void vRpnKeyExists_constraint(Stz* szRpnExpr, Card_KeyExists* pCard_KeyExists);
    void vRpnKeyNotExists(Stz* szRpnExpr, Card_KeyNotExists* pCard_KeyNotExists);
    void vRpnKeyNotExists_constraint(Stz* szRpnExpr, Card_KeyNotExists* pCard_KeyNotExists);
    void vRpnLimit       (Stz* szRpnExpr, Card_Limit*     pCard_Limit);
    void vRpnOrdAgg      (Stz* szRpnExpr, Card_OrdAgg*    pCard_OrdAgg);
    void vRpnProject     (Stz* szRpnExpr, Card_Project*   pCard_Project);
    void vRpnScan        (Stz* szRpnExpr, Card_Scan*      pCard_Scan);
    void vRpnSort        (Stz* szRpnExpr, Card_Sort*      pCard_Sort);
    void vRpnWhere       (Stz* szRpnExpr, Card_Where*     pCard_Where);

    void vExpr        (Stz* szRpnExpr, Card_Expr*      pCard_Expr);
    void vBoolCalc    (Stz* szRpnExpr, Card_BoolCalc*  pCard_BoolCalc);
    void vBoolPart    (Stz* szRpnExpr, Card*           p_part, i8 i);
    void vRpnCalc     (Stz* szRpnExpr, Card_RpnCalc*   pCard_RpnCalc);

    void vAs          (Stz* szRpnExpr, Card_As*        pCard_As);
    void vTerm        (Stz* szRpnExpr, Card_Term*      pCard_Term);

    Index*    pCraftIndex_indexScan(void);
    IndexDef* pCraftIndexDef_join(void);
    IndexDef* pCraftIndexDef_gbyAgg(void);

    StreamDef* pCraftStreamDef_arriving(void);
    StreamDef* pCraftStreamDef_emitted(void);
  };

  Card_OP* _pCard_OP(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
