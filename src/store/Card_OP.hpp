/*============================================================================*/
/* Card_OP.hpp -- Abstract Op                                                 */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "Card_symbol.hpp"
#include "IndexDef.hpp"
#include "ColMap.hpp"

namespace STORE
{
  class Card_OP : public Card_symbol<Card_OP>
  {  
  public:
    Card_OP(eCK eCard, i8 iFixedDepth);
   ~Card_OP(void) noexcept(false);

    static bool bIsOp(Card* pCard);

    void vAnalyse_injectable(CardIter* pCI);
    void vApply_lambdaFn_ToAll_term(CardIter* pCI,
                                    std::function<bool(Card* pCard)> bfLambdaFn);

    void vUpdate_expCol(CardIter* pCI, Card_Emit* p_arriving, bool bAlt=false);

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

    void vRpn_aggreg      (CardIter* pCI, String* psRpnExpr, Card_Aggreg*    p_aggreg);
    void vRpn_crossJoin   (CardIter* pCI, String* psRpnExpr, Card_CrossJoin* p_crossJoin);
    void vRpn_filter      (CardIter* pCI, String* psRpnExpr, Card_Filter*    p_filter);
    void vRpn_gbyAgg      (CardIter* pCI, String* psRpnExpr, Card_GbyAgg*    p_gbyAgg);
    void vRpn_hash        (CardIter* pCI, String* psRpnExpr, Card_Hash*      p_hash);
    void vRpn_having      (CardIter* pCI, String* psRpnExpr, Card_Having*    p_having);
    void vRpn_indexJoin   (CardIter* pCI, String* psRpnExpr, Card_IndexJoin* p_indexJoin);
    void vRpn_indexJoin_constraint(CardIter* pCI, String* psRpnExpr, Card_IndexJoin* p_indexJoin);
    void vRpn_indexScan   (CardIter* pCI, String* psRpnExpr, Card_IndexScan* p_indexScan);
    void vRpn_inflate     (CardIter* pCI, String* psRpnExpr, Card_Inflate*   p_inflate);
    void vRpn_keyExists   (CardIter* pCI, String* psRpnExpr, Card_KeyExists* p_keyExists);
    void vRpn_keyExists_constraint(CardIter* pCI, String* psRpnExpr, Card_KeyExists* p_keyExists);
    void vRpn_keyNotExists(CardIter* pCI, String* psRpnExpr, Card_KeyNotExists* p_keyNotExists);
    void vRpn_keyNotExists_constraint(CardIter* pCI, String* psRpnExpr, Card_KeyNotExists* p_keyNotExists);
    void vRpn_limit       (CardIter* pCI, String* psRpnExpr, Card_Limit*     p_limit);
    void vRpn_ordAgg      (CardIter* pCI, String* psRpnExpr, Card_OrdAgg*    p_ordAgg);
    void vRpn_project     (CardIter* pCI, String* psRpnExpr, Card_Project*   p_project);
    void vRpn_scan        (CardIter* pCI, String* psRpnExpr, Card_Scan*      p_scan);
    void vRpn_sort        (CardIter* pCI, String* psRpnExpr, Card_Sort*      p_sort);
    void vRpn_top         (CardIter* pCI, String* psRpnExpr, Card_Top*       p_top);
    void vRpn_where       (CardIter* pCI, String* psRpnExpr, Card_Where*     p_where);

    void vRpn_expr        (CardIter* pCI, String* psRpnExpr, Card_Expr*      p_expr);
    void vRpn_rpnCalc     (CardIter* pCI, String* psRpnExpr, Card_RpnCalc*   p_rpnCalc);
    void vRpn_term        (CardIter* pCI, String* psRpnExpr, Card_Term*      p_term);
    void vRpn_termSeq     (CardIter* pCI, String* psRpnExpr, Card_Expr*      p_expr);

    Index*     pGetIndex_chosenIndex(void);
    IndexDef*  pCraftIndexDef_key(CardIter* pCI);
    StreamDef* pCraftStreamDef_emitted(ColMap** ppColMap);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_OP* _pCard_OP(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
