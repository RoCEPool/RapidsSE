/*============================================================================*/
/* Card_OP.hpp -- Abstract Op                                                 */
/*============================================================================*/
#pragma once

#include "Card_MAIN.hpp"
#include "IndexDef.hpp"
#include "ColsReq.hpp"
#include "ColMap.hpp"

namespace STORE
{
  class Card_OP : public Card_MAIN
  {  
  public:
    Card_OP(eCK eCard, i8 iFixedDepth);
   ~Card_OP(void) noexcept(false);

    static bool bIsOp(Card* pCard);

    void vAdd_Pred_Under_filter(Card_PRED *p_PRED);
    void vApply_lambdaFn_ToAll_term(CardIter* pCI,
                                    std::function<bool(Card* pCard)> bfLambdaFn);

    void vConvert_stmColOdn_To_tblColOdn(CardIter* pCI);

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

    bool bPeg_Chosen_index(CardIter* pCI);
    bool bPeg_Key(CardIter* pCI);
    bool bPeg_Projection(CardIter* pCI);

    void vUpdate_constraint(CardIter* pCI);
    void vUpdate_projection(CardIter* pCI);

    void vRpnAggreg      (String* psRpnExpr, Card_Aggreg*    pCard_Aggreg);
    void vRpnCombined    (String* psRpnExpr, Card_Combined*  pCard_Combined);
    void vRpnCrossJoin   (String* psRpnExpr, Card_CrossJoin* pCard_CrossJoin);
    void vRpnFilter      (String* psRpnExpr, Card_Filter*    pCard_Filter);
    void vRpnGbyAgg      (String* psRpnExpr, Card_GbyAgg*    pCard_GbyAgg);
    void vRpnHash        (String* psOpArgs, Card_Hash*      pCard_Hash);
    void vRpnHaving      (String* psRpnExpr, Card_Having*    pCard_Having);
    void vRpnIndexJoin   (String* psRpnExpr, Card_IndexJoin* pCard_IndexJoin);
    void vRpnIndexJoin_constraint(String* psRpnExpr, Card_IndexJoin* pCard_IndexJoin);
    void vRpnIndexScan   (String* psRpnExpr, Card_IndexScan* pCard_IndexScan);
    void vRpnKeyExists   (String* psRpnExpr, Card_KeyExists* pCard_KeyExists);
    void vRpnKeyExists_constraint(String* psRpnExpr, Card_KeyExists* pCard_KeyExists);
    void vRpnKeyNotExists(String* psRpnExpr, Card_KeyNotExists* pCard_KeyNotExists);
    void vRpnKeyNotExists_constraint(String* psRpnExpr, Card_KeyNotExists* pCard_KeyNotExists);
    void vRpnLimit       (String* psRpnExpr, Card_Limit*     pCard_Limit);
    void vRpnOrdAgg      (String* psRpnExpr, Card_OrdAgg*    pCard_OrdAgg);
    void vRpnProject     (String* psRpnExpr, Card_Project*   pCard_Project);
    void vRpnScan        (String* psRpnExpr, Card_Scan*      pCard_Scan);
    void vRpnSort        (String* psRpnExpr, Card_Sort*      pCard_Sort);
    void vRpnTop         (String* psRpnExpr, Card_Top*       pCard_Top);
    void vRpnWhere       (String* psRpnExpr, Card_Where*     pCard_Where);

    void vExpr        (String* psRpnExpr, Card_Expr*      pCard_Expr);
    void vBoolCalc    (String* psRpnExpr, Card_BoolCalc*  pCard_BoolCalc);
    void vBoolPart    (String* psRpnExpr, Card*           p_part, i8 i);
    void vRpnCalc     (String* psRpnExpr, Card_RpnCalc*   pCard_RpnCalc);

    void vAs          (String* psRpnExpr, Card_As*        pCard_As);
    void vTerm        (String* psRpnExpr, Card_Term*      pCard_Term);

    ColsReq*  pCraftColMap_gbyAgg(StreamDef* pStreamDef_emitted,
                                  IndexDef*  pIndexDef_gbyAgg);

    Index*    pCraftIndex_indexJoin(void);
    Index*    pCraftIndex_indexScan(void);

    IndexDef* pCraftIndexDef_gbyAgg(void);
    IndexDef* pCraftIndexDef_sort(void);
    IndexDef* pCraftIndexDef_top(void);

    StreamDef* pCraftStreamDef_emitted(ColMap** ppColMap);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_OP* _pCard_OP(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
