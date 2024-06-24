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
      Card_Emit *      m_pEmitted_prior;
      Card_StreamKey * m_pStreamKey;
    } Arriving;

    struct {
      Card_Emit * m_p_expr;
      Card_Emit * m_p_key;
    } Dic;

    struct {
      Card_OP *     m_pOp_tip;
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
        bool m_bMorph_sqlOp_To_EQ;
        bool bReplace_sqCall_With_result;
      } Where_preAgg;

      struct
      {
        bool m_bMorph_To_gbyAgg;
      } Aggreg;

      struct
      {
        bool m_bInject_foldExpr;
      } GbyAgg;

      struct
      {
        bool m_bSeen;
      } Project_postAgg;

      struct
      {
        bool m_bCorrect_After_Inject;
      } Having;

      struct
      {
        bool m_bCorrect_After_Inject;
      } Sort;

      struct
      {
        bool m_bUnproject_foldExpr;
      } Unproject;

    } Smooth;

    struct
    {
      bool m_bUnproject_rowRef;
    } RowRef;


    i8    m_nAncestors;
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
    void vBeginEach_tail(void);
    void vBeginTarget(eCK ePrimary, eCK eTarget);
    void vBeginTarget(Card* pPrimary, eCK eTarget);
    Board* pBoard(void);
    void vBurst(Card* pCard);

    i8   i_Child(void);
    Card* pChild(void);
    Card* pChild(i8 i_Child);
    void vChild(Card* pChild);
    void vClearRegisters(void);
    eOA  eCueUser(String* psPrompt);

    void vDeleteWhenEmptySub(eCK eOp, eCK eSub);
    void vDontDescend(bool bDontDescend);

    bool   bEachBoard(void);
    Board* pEachBoard(void);
    Board* pEachBoard(i8& ri_Board);

    bool bEachOp(void);
    bool bEachOp(eCK eOp);
    bool bEachOp_break(eCK eOp, eCK eBreak= CK_o_FOLD);
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

    Card_OP* pEarlyTip(void);
    void vEarlyTip(Card_OP* p_Op_tip);

    void vEndPhase(void);
    Card* pEndTarget(void);
    void  vEndTarget(void);
    void vEndTransform(i8 iLineNo);

    Card_OP* pFind_from_By_tblSyn(Symbol* pSym_tblSyn, i8 i_Before);
    void vFocusOnOp(void);

    bool bIsQuery(Stz* szQueryNm);
    void vInsertAfter(Card* pCard, Card* pAfter);
    void vInsertBefore(Card* pCard, Card* pBefore);

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
    Card* pPeekPrevIf(eCK eCard);
    void vPopEach(void);
    bool bPositionAt(eCK eKind_at, i8 /* eCK */ iKind_First, ...);
    void vPrintBoard(const char* azFunc, const char* azFile, i8 iLineNo, bool bForce=false);
    Card_Emit* pPrior_emit(void);
    Card_Expr* pPrior_expr(Symbol* pSymbol_arg);
    void vPushChild(Card* pParent, eCK eChild);
    void vPushEach(void);
    void vPushEach(Board* pBoard);
    void vPushEach(Card* pParent);

    void vRemoveDeleteMain(eCK eCrossCheck);
    void vResetMembers(void);
    void vRepositionOp(void);
    void vRepositionOp(Card_OP* p_Op);
    bool bRewindOp(void);

    void vSnap_arriving(Card_Emit* p_emit, Card_StreamKey * pStreamKey);
    void vSnapIf_arriving(Card_Emit* p_emit, Card_StreamKey * pStreamKey);
    void vSnapIf_arriving(bool bSub_emit=true);
    void vSnapIf_arriving_till(eCK eTill);

    i8   i_Target(void);
    void vTransform(const char* azXfile, const char* azFunc, eXF eXform);

    void vUseBoard(Board* pBoard);
    void vUseCard(Card* pCard);

    void vValidateBench(void);
    void vValidateChildren(Card* pCard, i8 iDepth);

    Stz* szFMT_CAT_LEFT           (void) { return "%s"_Stz; }
    Stz* szFMT_CAT_RIGHT          (void) { return "%18s"_Stz; }
    Stz* szFMT_DOTS               (void) { return "%s%lld"_Stz; }
    Stz* szFMT_ENUM               (void) { return "%s"_Stz; }
    Stz* szFMT_FACET_UNKNOWN      (void) { return "%s"_Stz; }
    Stz* szFMT_FACET_SECONDARY    (void) { return "<%s"_Stz; }
    Stz* szFMT_FACET_PRIMARY      (void) { return "%s"_Stz; }
    Stz* szFMT_FACET_HEADER       (void) { return "===%02lld-%c--------------"_Stz; }
    Stz* szFMT_FACET_NO           (void) { return "~%lld"_Stz; }
    Stz* szFMT_MARKS              (void) { return "%2s%s"_Stz; }
    Stz* szFMT_NAME_LEFT          (void) { return "%s"_Stz; }
    Stz* szFMT_NAME_RIGHT         (void) { return "%18s"_Stz; }
    Stz* szFMT_PPY_INTEGER        (void) { return "}%s=%lld"_Stz; }
    Stz* szFMT_PPY_INTEGER_decode (void) { return "}%s"_Stz; }
    Stz* szFMT_PPY_STRING         (void) { return "}%s=%s"_Stz; }
    Stz* szFMT_PPY_STRING_decode  (void) { return "}%s"_Stz; }
    Stz* szFMT_PPY_SEQUENCE       (void) { return "#%lld"_Stz; }
    Stz* szFMT_SPACES             (void) { return "%s"_Stz; }
    Stz* szFMT_TYPE               (void) { return "%s"_Stz; }
    Stz* szFMT_TYPE_LENGTH        (void) { return "%s(%lld)"_Stz; }
    Stz* szFMT_TYPE_LENGTH_SCALE  (void) { return "%s(%lld,%lld)"_Stz; }

  private:
    Footer* aFooter(void);
  };

  CardIter* _pCardIter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
