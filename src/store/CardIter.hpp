/*============================================================================*/
/* CardIter.hpp --                                                            */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "EachMembers.hpp"
#include "PermGen.hpp"
#include "Index.hpp"
#include "Bench.hpp"
#include "eXF.h"

#define PRINTBOARD_dbg() vPrintBoard(__func__,__FILE__,__LINE__,true)

namespace STORE
{
  const i8 C_CARD_ANCESTRY = 16;

  class CardIter : public Object
                 , public ListExo2_t<EachMembers>
 {
  public:

    Bench*      m_pBench;
    Symbol      m_xSymbol_tableSyn;

    i8          m_i_RecursionDepth;

    EachMembers E;

    struct {
      Card_Emit *      m_pEmit;
      Card_StreamKey * m_pStreamKey;
    } Arriving;

    struct {
      Card_Emit * m_p_expr;
      Card_Emit * m_p_key;
    } Dic;

    struct {
      Card_OP*      m_pOP_root;
      Card_OP *     m_pOP_leaf;
      Card_Needed * m_pNeeded_later;
    } Early;

    struct {
      Card_Project * m_pProject;
      Card_Emit *   m_pEmitted;
    } Next;

    struct {

      SubQuery* m_pSqy_inner;

      union {
        Card_CONJ*    m_p_conj_iqyCall;
        Card_RlnConj* m_p_conj_rslTest;
      };
      Card_Term* m_p_term_sqyCall;

      Card_CONJ* m_p_conj_crlKey;
      Card_Term* m_p_term_crlKey;
      Card_Expr* m_p_expr_crlKey;

      Card_Correlation* m_pCorrelation;

      i8         m_i_fold;
      Card_Expr* m_pExpr_fold;
      Card*      m_pCopy_inner;
    } Flat;

    struct {
      Card_Emit *      m_pEmit;
      Card_StreamKey * m_pStreamKey;
    } StreamOrder;

    struct {
      Card_EXPRLIST * m_pExprList_a;
      Card_EXPRLIST * m_pExprList_i;
      Card_EXPRLIST * m_pExprList_k;
    } X;

    struct
    {
      struct
      {
        bool m_bSeen;
        i8 m_nTables;
        bool m_bReproject;
      } Table;

      struct
      {
        bool m_bSeen;
        bool m_bMorph_sqlOp_To_EQ;
        bool bReplace_sqCall_With_result;
      } Where_preAgg;

      struct
      {
        bool m_bSeen;
        bool m_bMorph_To_gbyAgg;
      } Aggreg;

      struct
      {
        bool m_bSeen;
        bool m_bInject_foldExpr;
      } GbyAgg;

      struct
      {
        bool m_bSeen;
      } Project_postAgg;

      struct
      {
        bool m_bSeen;
        bool m_bCorrect_After_Inject;
      } Having;

      struct
      {
        bool m_bSeen;
        bool m_bCorrect_After_Inject;
      } Sort;

      struct
      {
        bool m_bSeen;
        bool m_bUnproject_foldExpr;
      } Unproject;

    } Carry;



    i8 m_nAncestors;
    Card* m_paAncestry[C_CARD_ANCESTRY];
    i8    m_iaAncestry[C_CARD_ANCESTRY];

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    CardIter(Bench* pBench);
   ~CardIter(void) noexcept(false);

    Card* pAdvanceChild(void);
    void vAppendChild(eCK eParent, Card* pChild);
    void vAppendNewChild(eCK eParent, eCK eChild);
    void vAppendNewChild(Card* pParent, eCK eChild);
    void vAppendNewChild(Card* pParent, eCK eChild, Symbol* pSymbol);
    void vAppendNewChild(Card* pParent, eCK eChild, Symbol xSymbol);

    bool  bAncestor(eCK eCard);
    i8    i_Ancestor(Card* pAncestor);
    Card* pAncestor(i8 i_Ancestor);
    Card* pAncestor(eCK eCard, i8* ai_Ancestor= nullptr);
    void  vAncestorDump(Card* pCard = nullptr);
    Card* pAncestorParent(void);
    void  vAncestorPop(void);
    void  vAncestorPush(Card* pCard);
    i8    nAncestors(void);

    i8    iAncestorCounter(i8 i_Ancestor);
    i8    iAncestorCounter(Card* pAncestor);
    i8    iAncestorCounterInc(Card* pAncestor, i8 iInc=1);
    void  vAncestorCounterInc(Card* pAncestor, i8 iInc=1);
    void  vAncestorCounterReset(i8 iTo=0);
    void  vAncestorCounterResetAll(i8 iTo=0);

    // todo: merge these two [A294]
    Card_Expr* pBackSearch_expr_For_col(Symbol* pSym_term_arg, i8 i_Before);
    Card_Expr* pBackSearch_expr_For_colNm(Symbol* pSym_term, i8 i_Before=C_UNKNOWN);

    void vBackstepTarget(void);
    void vBeginBoard(void);
    void vBeginEach(eCK eOp=CK_NONE);
    void vBeginEach_reverse(void);
    void vBeginMember(eCK eSub_Of_Op);
    void vBeginTarget(eCK ePrimary, eCK eTarget);
    void vBeginTarget(Card* pPrimary, eCK eTarget);
    Board* pBoard(void);
    void vBurst(Card* pCard);

    i8   i_Child(void);
    Card* pChild(void);
    Card* pChild(i8 i_Child);
    void vChild(Card* pChild);
    void vClearRegisters(void);
    void vClearRegistersIf(Card* pCard);
    eOA  eCueUser(String* psPrompt);

    void vDeleteMember(Card* pMember);
    void vDeleteWhenEmptySub(eCK eOp, eCK eSub);
    void vDontDescend(bool bDontDescend);

    bool   bEachBoard(void);
    Board* pEachBoard(void);
    Board* pEachBoard(i8& ri_Board);

    bool bEachMember(void);
    bool bEachMember(eCK eMember2, eCK eMember3);
    bool bEachOp(void);
    bool bEachOp(eCK eOp);
    bool bEachOp_break(eCK eOp, eCK eBreak= CK_c_FOLD);
    bool bEachOp(eCK eOp, eCK eSub);
    bool bEachOp(eCK eOp, eCK eSub, Stz* szSubFacet);
    bool bEachOp(eCK eOp, eCK eSub1, eCK eSub2);
    bool bEachOp(eCK eOp, eCK eSub1, eCK eSub2, eCK eSub3);

    bool bEachOp_reverse(void);
    bool bEachOp_reverse(eCK eOp);
    bool bEachOp_reverse_foldBreak(eCK eOp);
    bool bEachOp_reverse(eCK eOp, eCK eSub);
    bool bEachOp_reverse_foldBreak(eCK eOp, eCK eSub);

    bool bEachOp_bySub(eCK eSub);
    bool bEachOp_by2Sub(eCK eSub1, eCK eSub2);
    bool bEachOp_by3Sub(eCK eSub1, eCK eSub2, eCK eSub3);

    bool bEachTarget(void);

    Card* pEndMember(eCK eMember);
    void vEndMember(void);
    void vEndPhase(void);
    Card* pEndTarget(void);
    void  vEndTarget(void);
    void vEndTransform(i8 iLineNo);

    Card_OP* pFind_from_By_tblSyn(Symbol* pSym_tblSyn, i8 i_Before);
    void vFocusOnOp(void);
    bool bForwards(void);
    void vForwards(bool bForward);

    void vInsertAfter(Card* pCard, Card* pAfter);
    void vInsertBefore(Card* pCard, Card* pBefore);

    bool bLambdaWalk(Card* pCard, eCK eToSubs, std::function<bool(Card* pCard)> bfLambdaFn);

    Card* pMember(eCK eCard);
    void vMember(eCK eCard, Card* pCard);
    bool bMore(void);
    bool bMoreTarget(void);

    void  vNewMember(eCK eCard);
    void  vNewMember(eCK eCard, Symbol xSymbol);

    eCK  ePeekNext(void);
    Card* pPeekNext(void);
    Card* pPeekNextIf(eCK eCard);
    Card* pPeekPrev(void);
    void vPopEach(void);
    bool bPositionAt(eCK eKind_at, i8 /* eCK */ iKind_First, ...);
    void vPrintBoard(const char* azFunc, const char* azFile, i8 iLineNo, bool bForce=false);
    Card_Emit* pPrior_emit(void);
    Card_Expr* pPrior_expr(Symbol* pSymbol_arg);
    void vPushChild(Card* pParent, eCK eChild);
    void vPushEach(void);

    bool bRecurseActive(void);
    void vRecurseBegin(Card* pCard);
    Card* pRecurseEach(void);
    Card* pRecurseEach(eCK eCard);
    void vRecurseEnd(void);
    void vRemoveDeleteMain(eCK eCrossCheck);
    void vResetMembers(void);
    void vResynchOp(void);
    void vResynchOp(Card* pCard);
    bool bRewindOp(void);

    void vSnap_arriving(Card_Emit* p_emit, Card_StreamKey * pStreamKey);
    void vSnapIf_arriving(Card_Emit* p_emit, Card_StreamKey * pStreamKey);
    void vSnapIf_arriving(bool bSub_emit=true);

    i8   i_Target(void);
    void vTransform(const char* azXfile, const char* azFunc, eXF eXform);

    void vUseBoard(Board* pBoard);
    void vUseCard(Card* pCard);

    void vValidateBench(void);
    void vValidateChildren(Card* pCard, i8 iDepth);

  private:
    Footer* aFooter(void);
  };

  CardIter* _pCardIter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
