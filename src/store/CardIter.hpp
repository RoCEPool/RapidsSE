/*============================================================================*/
/* CardIter.hpp --                                                            */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "CardIterMembers.hpp"
#include "PermGen.hpp"
#include "Index.hpp"
#include "Bench.hpp"

namespace STORE
{
  class CardIter : public Object
                 , public ListExo2_t<CardIterMembers>
 {
  public:

    CardIterMembers G;

    Bench*      m_pBench;
    CardCtl*    m_pCardCtl;

    i8          m_iXform;
    i8          m_iTrace;
    s32         m_sXhand;
    s256        m_sXform;
    s256        m_sXdesc;

    struct {
      Card_OP*      m_pOP_root;
      Card_OP *     m_pOP_leaf;
      Card_Needed * m_pNeeded_later;
    } Early;

    struct {
      Card_Projection * m_pProjection_prior;
    } Analysis;

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
      Card_Key *        m_pKey_running;
      Card_Projection * m_pProjection_running;
    } StreamOrder;


  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    CardIter(Card* pCard);
    CardIter(CardIter* pCardIter_src);
    CardIter(Bench* pBench);
   ~CardIter(void) noexcept(false);
    void vInit(void);

    void G_vAppendChild(eCK eParent, eCK eChild);
    void G_vAppendChild(eCK eParent, Card* pChild);
    void G_vAppendNewChild(Card* pParent, eCK eChild);
    void G_vAppendNewChild(Card* pParent, eCK eChild, Stz* szValue);

    Board* pBoard(void);
    void vBoardIteration(void);

    void vChangeCard(Card* pCard);
    i8   i_Child(void);
    Card* pChild(void);
    void G_vClearMembers(void);
    void G_vClearRegisters(void);
    void G_vClearRegistersIf(Card* pCard);
    eOA  eCueUser(Stz* szPrompt);

    void G_vDeleteMember(eCK eMember);
    void G_vDeleteMember(Card* pMember);
    void G_vDeleteWhenEmptySub(eCK eMain, eCK eSub);
    void vDontDescend(void);

    bool   bEachBoard(void);
    Board* pEachBoard(void);
    Board* pEachBoard(i8& ri_Board);

    bool G_bEachMain(eCK eOfMainKind=CK_NONE);
    bool G_bEachMain_foldBreak(eCK eOfMainKind=CK_NONE);
    bool G_bEachMain_reverse(eCK eOfMainKind=CK_NONE);
    bool G_bEachMain_reverse_foldBreak(eCK eOfMainKind=CK_NONE);

    bool G_bEachMainWithSub(eCK eOfMainKind, eCK eAndSubKind);
    bool G_bEachMainWithSub_reverse(eCK eOfMainKind, eCK eAndSubKind);
    bool G_bEachMainWithSub_reverse_foldBreak(eCK eOfMainKind, eCK eAndSubKind);
    bool G_bEachMainWithSub(eCK eOfMainKind, eCK eAndSubKind, Stz* szSubValue);
    bool G_bEachMainWith2Sub(eCK eOfMainKind, eCK eAndSubKind1, eCK eAndSubKind2);
    bool G_bEachMainWith3Sub(eCK eOfMainKind, eCK eAndSubKind1, eCK eAndSubKind2, eCK eAndSubKind3);

    bool G_bEachSubOfMain(eCK eSubKind);
    bool G_bEach2SubOfMain(eCK eSubKind1, eCK eSubKind2);
    bool G_bEach3SubOfMain(eCK eSubKind1, eCK eSubKind2, eCK eSubKind3);

    void vEndTransform(void);

    Card_Expr* pExpr_For_col(Card_Term* p_term, i8 i_Before=C_UNKNOWN);
    Card_Expr* pExpr_For_colNm(Card_Term* p_term, i8 i_Before=C_UNKNOWN);

    void vFocusOnMain(void);
    void vForward(void);

    Card* pGetChild(i8 i_Child);
    Card* G_pGetMember(eCK eMember);

    void G_vInsertAfter(Card* pCard, Card* pAfter);
    void G_vInsertBefore(Card* pCard, Card* pBefore);
    bool G_bIsBoardNm(Stz* szBoardNm);
    bool G_bIsNextMain(eCK eMember);
    bool bIsTracing(void);

    bool bJustRestarted(void);

    void G_vMainResynch(void);
    void G_vMainResynch(Card* pCard);
    void G_vMember(eCK eMember, Card* pCard, const char* asFuncNm);
    Stz* szMemberNm(eCK eMember);
    bool bMore(void);
    bool G_bMoreMain(void);
    void G_vMorphCard(Card* pCard, eCK eCard);

    void  G_vNewMember(eCK eMember, bool bUpdateMember= true);
    void  G_vNewMember(eCK eMember, Stz* szValue, bool bUpdateMember= true);
    void G_vNewMemberBefore(eCK eMember, Stz* szValue, eCK eOf, bool bUpdateMember= true);
    void G_vNewMemberOf(eCK eMember, Stz* szValue, eCK eOf, bool bUpdateMember= true);
    Card* pNextChild(void);
    Card* pNextChildIf(eCK eCard);

    void G_vPopBoard(void);
    void G_vPopIter(void);
    Card* pPrevChild(void);
    Card* pPrevChildIf(eCK eCard);
    void vPrintBench(eBK eBoard, i8 i_Board=C_UNKNOWN);
    void vPrintBoard(i8 iLineNo=C_UNKNOWN);
    void G_vPushBoard(Board* pBoard);
    void G_vPushBoard(Stz* szBoardNm);
    bool G_bPushBoard(i8 i_Board);
    bool G_bPushBoard(Stz* szBoardNm);
    void G_vPushNewChild(Card* pParent, eCK eChild);
    void G_vPushNewChild(Card* pParent, eCK eChild, Stz* szValue);
    void G_vPushIter(void);

    Card* pRecurseEach(void);
    Card* pRecurseEach(eCK eCard);
    void vRemoveChild(Card* pChild);
    void vRemoveCurr(eCK eCrossCheck);
    void vRemoveDeleteChild(Card*& rpChild);
    void vRemoveDeleteCurr(eCK eCrossCheck);
    void G_vRemoveDeleteMain(eCK eCrossCheck);
    void G_vRemoveMain(eCK eCrossCheck);
    Card_Expr* pResolvePrior_expr(Card_Term* p_term);
    Card_Projection* pResolvePrior_projection(void);
    void vRestart(void);
    void G_vRestart(void);
    void vRestore(void);
    void vRewind(void);
    Card* pRewindChild(void);
    bool G_bRewindMain(void);

    void vSave(void);
    i8   iSentinalsDepth(void);
    void vSet(Card* pChild);
    void vSet(CardIter* pCardIter_src);
    void vSet(i8 i_Child);
    void G_vSetBoard(Board* pBoard);
    void G_vSkipNextMain(bool bClearMembers=false, bool bClearRegisters=false);
    void vSubQueryIteration(void);

    void vTracer(Stz* szFormat, ...);
    void vTransform(const char* azFunc, Stz* szXdesc);
    i8   iTransformDepth(void);

    void vUsingBoard(i8 i_Board);
    void vUsingBoard(Board* pBoard);

    Stz* szXdesc(void);
    void vXdesc(Stz* szDescription);
    Stz* szXform(void);
    void vXform(Stz* szTransformation);
    Stz* szXhand(void);
    void vXhand(const char* asXhand);

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
