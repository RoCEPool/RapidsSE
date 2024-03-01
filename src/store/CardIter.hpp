/*============================================================================*/
/* CardIter.hpp --                                                            */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "CardIterMembers.hpp"
#include "PermGen.hpp"
#include "Index.hpp"
#include "Bench.hpp"
#include "eXF.h"

namespace STORE
{
  const i8 C_CARD_ANCESTRY = 16;

  class CardIter : public Object
                 , public ListExo2_t<CardIterMembers>
 {
  public:

    CardIterMembers G;

    Bench*      m_pBench;
    CardCtl*    m_pCardCtl;
    Symbol      m_xSymbol_tableSyn;

    i8          m_i_RecursionDepth;

    struct {
      Card_OP*      m_pOP_root;
      Card_OP *     m_pOP_leaf;
      Card_Needed * m_pNeeded_later;
    } Early;

    struct {
      Card_Emit * m_pArriving;
    } Flux;

    struct {
      Card_Emit * m_p_expr;
      Card_Emit * m_p_key;
    } Dic;

    struct {
      Card_Project * m_pProject;
      Card_Emit * m_pEmitted;
    } Next;

    struct {
      i8         m_i_SubQuery;
      SubQuery*  m_pSubQuery;

      Card_Term* m_pTerm_sqyArg;
      Card_Term* m_pTerm_corrCol;
      Card_Term* m_pTerm_sqyCall;
      Card_CONJ* m_pConj_sqyCall;
      Card_Correlation* m_pCorrelation;
      i8         m_i_FoldCard;
      Card_Expr* m_pExpr_fold;
      Card_Expr* m_pExpr_corr;
      Card*      m_pCard_sqyCopy;
    } Sqy;

    struct {
      Card_StreamKey * m_pStreamKey;
      Card_Emit *   m_pEmitted;
    } StreamOrder;

    struct {
      Card_EXPRLIST * m_pExprList_a;
      Card_EXPRLIST * m_pExprList_i;
      Card_EXPRLIST * m_pExprList_k;
    } X;

    i8 m_nAncestors;
    Card* m_apAncestry[C_CARD_ANCESTRY];

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    CardIter(Bench* pBench);
   ~CardIter(void) noexcept(false);

    Card* pAdvanceChild(void);
    void vAppendChild(eCK eParent, eCK eChild);
    void vAppendChild(eCK eParent, Card* pChild);
    void vAppendNewChild(eCK eParent, eCK eChild);
    void vAppendNewChild(Card* pParent, eCK eChild);
    void vAppendNewChild(Card* pParent, eCK eChild, Symbol* pSymbol);
    void vAppendNewChild(Card* pParent, eCK eChild, Symbol xSymbol);

    Card* pAncestor(i8 i_Ancestor);
    void  vAncestorDump(Card* pCard = nullptr);
    Card* pAncestorFind(eCK eCard, i8* ai_Ancestor=nullptr);
    bool  bAncestorIncludes(eCK eCard);
    Card* pAncestorParent(void);
    void  vAncestorPop(void);
    void  vAncestorPush(Card* pCard);
    i8    nAncestors(void);

    // todo: merge these two [A294]
    Card_Expr* pBackSearch_expr_For_col(Symbol* pSym_term_arg, i8 i_Before);
    Card_Expr* pBackSearch_expr_For_colNm(Symbol* pSym_term, i8 i_Before=C_UNKNOWN);

    void vBeginBoardIteration(void);
    void vBeginEach(void);
    void vBeginEachReverse(void);
    Board* pBoard(void);
    void vBurst(Card* pCard);

    void vChangeCard(Card* pCard);
    i8   i_Child(void);
    Card* pChild(void);
    void vClearRegisters(void);
    void vClearRegistersIf(Card* pCard);
    void vClearRestart(void);
    eOA  eCueUser(String* psPrompt);

    void vDeleteMember(Card* pMember);
    void vDeleteWhenEmptySub(eCK eMain, eCK eSub);
    void vDontDescend(bool bDontDescend);

    bool   bEachBoard(void);
    Board* pEachBoard(void);
    Board* pEachBoard(i8& ri_Board);

    bool bEachMain_NONE(void);
    bool bEachMain(eCK eOfMainKind);
    bool bEachMain_foldBreak(eCK eOfMainKind);
    bool bEachMainWithSub(eCK eOfMainKind, eCK eAndSubKind);
    bool bEachMainWithSub(eCK eOfMainKind, eCK eAndSubKind, Stz* szSubValue);
    bool bEachMainWith2Sub(eCK eOfMainKind, eCK eAndSubKind1, eCK eAndSubKind2);
    bool bEachMainWith3Sub(eCK eOfMainKind, eCK eAndSubKind1, eCK eAndSubKind2, eCK eAndSubKind3);

    bool bEachMain_reverse_NONE(void);
    bool bEachMain_reverse_b(eCK eOfMainKind);
    bool bEachMain_reverse_foldBreak_b(eCK eOfMainKind);
    bool bEachMainWithSub_reverse(eCK eOfMainKind, eCK eAndSubKind);
    bool bEachMainWithSub_reverse_foldBreak_b(eCK eOfMainKind, eCK eAndSubKind);

    bool bEachSubOfMain(eCK eSubKind);
    bool bEach2SubOfMain(eCK eSubKind1, eCK eSubKind2);
    bool bEach3SubOfMain(eCK eSubKind1, eCK eSubKind2, eCK eSubKind3);

    void vEnd(void);
    void vEndPhase(void);
    void vEndTransform(i8 iLineNo=C_UNKNOWN);

    void vFocusOnMain(void);
    bool bForwards(void);
    void vForwards(bool bForward);

    Card* pGetChild(i8 i_Child);
    Card* pGetMember(eCK eMember);

    void vInsertAfter(Card* pCard, Card* pAfter);
    void vInsertBefore(Card* pCard, Card* pBefore);
    bool bIsNextMain(eCK eMember);

    bool bLambdaWalk(Card* pCard, eCK eToSubs, std::function<bool(Card* pCard)> bfLambdaFn);

    void vMainResynch(void);
    void vMainResynch(Card* pCard);
    void vMember(eCK eMember, Card* pCard, const char* asFuncNm);
    void vMember(Card* pCard);
    bool bMore(void);

    void  vNewMember(eCK eMember);
    void  vNewMember(eCK eMember, Symbol xSymbol);
    Card* pNextChild(void);
    Card* pNextChildIf(eCK eCard);

     void vPopBoard(void);
    Card* pPrevChild(void);
    Card* pPrevChildIf(eCK eCard);
    void vPrintBoard(const char* azFile, const char* azFunc, i8 iLineNo, bool bForce=false);
    void vSnapFluxArriving(void);
    void vPushBoard(Board* pBoard);
    bool bPushBoard(Symbol* pSymbol);
    void vPushNewChild(Card* pParent, eCK eChild);
    void vPushIter(void);

    void vRecurseBegin(Card* pCard);
    Card* pRecurseEach(void);
    Card* pRecurseEach(eCK eCard);
    void vRecurseEnd(void);
    void vRemoveChild(Card* pChild);
    void vRemoveCurr(eCK eCrossCheck);
    void vRemoveDeleteCurr(eCK eCrossCheck);
    void vRemoveDeleteMain(eCK eCrossCheck);
    void vResetMembers(bool bNull_OP=true);
    Card_Expr* pResolvePrior_expr(Card_Term* p_term);
    Card_Emit* pResolvePrior_projection(void);
    void vRestore(void);
    Card* pRewindChild(void);
    bool bRewindMain(void);

    void vSave(void);
    void vSet(Card* pChild);
    void vSet(CardIter* pCardIter_src);
    void vSet(i8 i_Child);
    void vSetBoard(Board* pBoard);

    void vTransform(const char* azXfile, const char* azFunc, eXF eXform);

    void vUsingBoard(i8 i_Board);
    void vUsingBoard(Board* pBoard);

    void vValidateBench(void);
    void vValidateChildren(Card* pCard, i8 iDepth);

  protected:

    bool bPopRecurseIter(void);
    void vPushRecurseIter(void);

  private:

    Footer* aFooter(void);
  };

  CardIter* _pCardIter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
