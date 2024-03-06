/*============================================================================*/
/* Card_OP.hpp -- Abstract Op                                                 */
/*============================================================================*/
#pragma once

#include "Card_MAIN.hpp"
#include "IndexDef.hpp"
#include "ColMap.hpp"

namespace STORE
{
  class Card_OP : public Card_MAIN
  {  
  public:
    Card_OP(eCK eCard, i8 iFixedDepth);
   ~Card_OP(void) noexcept(false);

    static bool bIsOp(Card* pCard);

    void vAnalyse_injectable(CardIter* pCI);
    void vApply_lambdaFn_ToAll_term(CardIter* pCI,
                                    std::function<bool(Card* pCard)> bfLambdaFn);

    void vUpdate_expCol(Card_Emit* p_arriving, bool bAlt=false);

    void vCull_filter_IfEmpty(Card_Filter* p_filter);

    void vEarlyProject_root(CardIter* pCI);
    void vEarlyProject_backstep(CardIter* pCI);

    Card_Filter* pEnsure_filter(void);
    void vEnsure_stream(CardIter* pCI);

    bool bIsEarlyLeaf(CardIter* pCI);
    bool bIsEarlyRoot(void);
    bool bIsNodeLevel(void);
    bool bIsOriginLevel(void);
    bool bIsPartitionLevel(void);

    void vMapColumns(CardIter *pCI);

    bool bPeg_Chosen_index(CardIter* pCI);
    bool bPeg_Key(CardIter* pCI);
    bool bPeg_Projection(CardIter* pCI);

    void vRestate_emit(CardIter* pCI);

    void vRpn_aggreg      (String* psRpnExpr, Card_Aggreg*    p_aggreg);
    void vRpn_crossJoin   (String* psRpnExpr, Card_CrossJoin* p_crossJoin);
    void vRpn_filter      (String* psRpnExpr, Card_Filter*    p_filter);
    void vRpn_gbyAgg      (String* psRpnExpr, Card_GbyAgg*    p_gbyAgg);
    void vRpn_hash        (String* psRpnExpr, Card_Hash*      p_hash);
    void vRpn_having      (String* psRpnExpr, Card_Having*    p_having);
    void vRpn_indexJoin   (String* psRpnExpr, Card_IndexJoin* p_indexJoin);
    void vRpn_indexJoin_constraint(String* psRpnExpr, Card_IndexJoin* p_indexJoin);
    void vRpn_indexScan   (String* psRpnExpr, Card_IndexScan* p_indexScan);
    void vRpn_inflate     (String* psRpnExpr, Card_Inflate*   p_inflate);
    void vRpn_keyExists   (String* psRpnExpr, Card_KeyExists* p_keyExists);
    void vRpn_keyExists_constraint(String* psRpnExpr, Card_KeyExists* p_keyExists);
    void vRpn_keyNotExists(String* psRpnExpr, Card_KeyNotExists* p_keyNotExists);
    void vRpn_keyNotExists_constraint(String* psRpnExpr, Card_KeyNotExists* p_keyNotExists);
    void vRpn_limit       (String* psRpnExpr, Card_Limit*     p_limit);
    void vRpn_ordAgg      (String* psRpnExpr, Card_OrdAgg*    p_ordAgg);
    void vRpn_project     (String* psRpnExpr, Card_Project*   p_project);
    void vRpn_scan        (String* psRpnExpr, Card_Scan*      p_scan);
    void vRpn_sort        (String* psRpnExpr, Card_Sort*      p_sort);
    void vRpn_top         (String* psRpnExpr, Card_Top*       p_top);
    void vRpn_where       (String* psRpnExpr, Card_Where*     p_where);

    void vRpn_expr        (String* psRpnExpr, Card_Expr*      p_expr);
    void vRpn_rpnCalc     (String* psRpnExpr, Card_RpnCalc*   p_rpnCalc);
    void vRpn_term        (String* psRpnExpr, Card_Term*      p_term);

    Index*     pGetIndex_chosenIndex(void);
    IndexDef*  pCraftIndexDef_key(void);
    StreamDef* pCraftStreamDef_emitted(ColMap** ppColMap);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_OP* _pCard_OP(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
