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
      Card_Projection * m_pProjection;
    } Prior;

    struct {
      i8         m_i_SubQuery;
      SubQuery*  m_pSubQuery;

      s32        m_sSqyNm;
      s32        m_sSqyArg;
      Card_Term* m_pTerm_corrCol;
      Card_Term* m_pTerm_sqCall;
      Card_Correlation* m_pCorrelation;
      i8         m_i_FoldCard;
      Card_Expr* m_pExpr_fold;
      Card_Expr* m_pExpr_corr;
      Card*      m_pCard_sqCopy;
    } Sqy;

    struct {
     Card_StreamKey *  m_pStreamKey_running;
      Card_Projection * m_pProjection_running;
    } StreamOrder;

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
    bool bApplyRecursive(Card* pCard, eCK eToSubs, std::function<bool(Card* pCard)> bfLambdaFn);

    void G_vAppendChild(eCK eParent, eCK eChild);
    void G_vAppendChild(eCK eParent, Card* pChild);
    void G_vAppendNewChild(eCK eParent, eCK eChild);
    void G_vAppendNewChild(Card* pParent, eCK eChild);
    void G_vAppendNewChild(Card* pParent, eCK eChild, Symbol* pSymbol);
    void G_vAppendNewChild(Card* pParent, eCK eChild, Symbol xSymbol);

    Card* pAncestor(i8 i_Ancestor);
    void  vAncestorDump(Card* pCard = nullptr);
    Card* pAncestorFind(eCK eCard);
    bool  bAncestorIncludes(eCK eCard);
    void  vAncestorPop(void);
    void  vAncestorPush(Card* pCard);
    i8    nAncestors(void);

    void vBeginEach(void);
    void vBeginEachReverse(void);
    Board* pBoard(void);
    void vBoard(Board* pBoard);
    void vBoardIteration(void);
    void vBurst(Card* pCard);
    void vBurstOP(void);

    void vChangeCard(Card* pCard);
    i8   i_Child(void);
    Card* pChild(void);
    void G_vClearRegisters(void);
    void G_vClearRegistersIf(Card* pCard);
    void vClearRestart(void);
    eOA  eCueUser(String* psPrompt);

    void G_vDeleteMember(Card* pMember);
    void G_vDeleteWhenEmptySub(eCK eMain, eCK eSub);
    void vDontDescend(bool bDontDescend);

    bool   bEachBoard(void);
    Board* pEachBoard(void);
    Board* pEachBoard(i8& ri_Board);

    bool G_bEachMain_NONE(void);
    bool G_bEachMain(eCK eOfMainKind);
    bool G_bEachMain_foldBreak(eCK eOfMainKind);
    bool G_bEachMainWithSub(eCK eOfMainKind, eCK eAndSubKind);
    bool G_bEachMainWithSub(eCK eOfMainKind, eCK eAndSubKind, Stz* szSubValue);
    bool G_bEachMainWith2Sub(eCK eOfMainKind, eCK eAndSubKind1, eCK eAndSubKind2);
    bool G_bEachMainWith3Sub(eCK eOfMainKind, eCK eAndSubKind1, eCK eAndSubKind2, eCK eAndSubKind3);

    bool G_bEachMain_reverse_NONE(void);
    bool G_bEachMain_reverse_b(eCK eOfMainKind);
    bool G_bEachMain_reverse_foldBreak_b(eCK eOfMainKind);
    bool G_bEachMainWithSub_reverse(eCK eOfMainKind, eCK eAndSubKind);
    bool G_bEachMainWithSub_reverse_foldBreak_b(eCK eOfMainKind, eCK eAndSubKind);

    bool G_bEachSubOfMain(eCK eSubKind);
    bool G_bEach2SubOfMain(eCK eSubKind1, eCK eSubKind2);
    bool G_bEach3SubOfMain(eCK eSubKind1, eCK eSubKind2, eCK eSubKind3);

    void vEnd(void);
    void vEndPhase(void);
    void vEndTransform(void);

    Card_Expr* pExpr_For_col(Card_Term* p_term, i8 i_Before=C_UNKNOWN);
    Card_Expr* pExpr_For_colNm(Card_Term* p_term, i8 i_Before=C_UNKNOWN);

    void vFocusOnMain(void);
    bool bForwards(void);
    void vForwards(bool bForward);

    Card* pGetChild(i8 i_Child);
    Card* G_pGetMember(eCK eMember);

    void G_vInsertAfter(Card* pCard, Card* pAfter);
    void G_vInsertBefore(Card* pCard, Card* pBefore);
    bool G_bIsBoardNm(Stz* szBoardNm);
    bool G_bIsNextMain(eCK eMember);


    void G_vMainResynch(void);
    void G_vMainResynch(Card* pCard);
    void G_vMember(eCK eMember, Card* pCard, const char* asFuncNm);
    void vMember(Card* pCard);
    bool bMore(void);

    void  G_vNewMember(eCK eMember);
    void  G_vNewMember(eCK eMember, Symbol xSymbol);
    Card* pNextChild(void);
    Card* pNextChildIf(eCK eCard);
    void vNullMembers(bool bNull_OP=true);

    bool bPastPhase(eOH ePhase);
    void vPhase(eOH ePhase);
    void G_vPopBoard(void);
    void G_vPopIter(void);
    Card* pPrevChild(void);
    Card* pPrevChildIf(eCK eCard);
    void vPrintBoard(i8 iLineNo=C_UNKNOWN, bool bForce=false);
    void vSnapPriorProjection(void);
    void G_vPushBoard(Board* pBoard);
    void G_vPushBoard(Stz* szBoardNm);
    bool G_bPushBoard(i8 i_Board);
    bool G_bPushBoard(Stz* szBoardNm);
    void G_vPushNewChild(Card* pParent, eCK eChild);
    void G_vPushIter(void);

    void vRecurseBegin(Card* pCard);
    Card* pRecurseEach(void);
    Card* pRecurseEach(eCK eCard);
    void vRecurseEnd(void);
    void vRemoveChild(Card* pChild);
    void vRemoveCurr(eCK eCrossCheck);
    void vRemoveDeleteChild(Card*& rpChild);
    void vRemoveDeleteCurr(eCK eCrossCheck);
    void G_vRemoveDeleteMain(eCK eCrossCheck);
    void G_vRemoveMain(eCK eCrossCheck);
    void vRenumberCards(void);
    Card_Expr* pResolvePrior_expr(Card_Term* p_term);
    Card_Expr* pResolvePrior_expr_prior(Card_Term* p_term);
    Card_Projection* pResolvePrior_projection(void);
    void vRestore(void);
    Card* pRewindChild(void);
    bool G_bRewindMain(void);

    void vSave(void);
    void vSet(Card* pChild);
    void vSet(CardIter* pCardIter_src);
    void vSet(i8 i_Child);
    void G_vSetBoard(Board* pBoard);

    void vTransform(const char* azFunc, eXF eXform, Stz* szXdesc);

    void vUsingBoard(i8 i_Board);
    void vUsingBoard(Board* pBoard);

    void vValidateBench(void);
    void vValidateChildren(Card* pCard, i8 iDepth);

    Stz* szXdesc(void);
    i8   iXform(void);
    Stz* szXfunc(void);
    Stz* szXhand(void);

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
