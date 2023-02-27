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

    // Xgearing
    Bench*      m_pBench;

    i8          m_iXform;
    i8          m_iTrace;

    s32         m_sXhand;
    s256        m_sXform;
    s256        m_sXdesc;

    s32         m_sSqyNm;
    s32         m_sSqyArg;
    s32         m_sSqyCol;
    s32         m_sSqyRO;
    s32         m_sSqyRsl;

    PermGen*    m_pPermGen;

    Column*     m_pColumn;
    IdxVec*     m_pIdxVec;
    Index*      m_pIndex;
    IndexDef*   m_pIndexDef;
    StreamDef*  m_pStreamDef;
    Table*      m_pTable;

    CardCtl*    m_pCardCtl;

    Card* m_pCard_child;
    Card* m_pCard_copied;
    Card* m_pCard_dup;
    Card* m_pCard_found;
    Card* m_pCard_located;
    Card* m_pCard_member;
    Card* m_pCard_new;
    Card* m_pCard_parent;
    Card* m_pCard_popped;
    Card* m_pCard_resolved;
    Card* m_pCard_temp;
    Card* m_pCard_yanked;

    Card_Expr* m_pExpr_fold;
    Card_Expr* m_pExpr_corr;

    Card_Term* m_pTerm_0;
    Card_Term* m_pTerm_1;
    Card_Term* m_pTerm_2;
    Card_Term* m_pTerm_corrCol;
    Card_Term* m_pTerm_sqCall;

    Card_Correlation* m_pCorrelation;

    s256 m_sT;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    CardIter(void);
    CardIter(eCN eClass);
    CardIter(Card* pCard);
    CardIter(CardIter* pCardIter_src);
    CardIter(eCN eClass, Card* pCard);
    CardIter(eCN eClass, Bench* pBench);
    CardIter(Bench* pBench);
   ~CardIter(void) noexcept(false);

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
    void G_vCopyCard(Card* pCard);
    eOA  eCueUser(Stz* szPrompt);

    void G_vDeleteMember(eCK eMember);
    void G_vDeleteMember(Card* pMember);
    void G_vDeleteWhenEmptySub(eCK eMain, eCK eSub);
    void vDontDescend(void);
    void G_vDuplicateMember(eCK eMember);

    bool bEachBoard(void);
    Board* pEachBoard(void);
    Board* pEachBoard(i8& ri_Board);

    bool  bEachChild(void);
    Card* pEachChild(void);
    Card* pEachChild(i8& ri_Child);
    Card* pEachChild(eCK eCard);
    Card* pEachChild(eCK eCard, i8& ri_Main);
    Card* pEachChild(i8& ri_Main, eCK& reCard);
    bool  bEachChild_reverse(void);
    Card* pEachChild_reverse(void);
    Card* pEachChild_reverse(i8& ri_Child);
    Card* pEachChild_reverse(eCK eCard);
    Card* pEachChild_reverse_foldBreak(eCK eCard);

    bool G_bEachMain(void);
    bool G_bEachMain(eCK eOfMainKind);
    bool G_bEachMain_reverse_foldBreak(eCK eOfMainKind);
    bool G_bEachMainWithSub(eCK eOfMainKind, eCK eAndSubKind);
    bool G_bEachMainWithSub_reverse(eCK eOfMainKind, eCK eAndSubKind);
    bool G_bEachMainWithSub_reverse_foldBreak(eCK eOfMainKind, eCK eAndSubKind);
    bool G_bEachMainWithSub(eCK eOfMainKind, eCK eAndSubKind, Stz* szSubValue);
    bool G_bEachMainWith2Sub(eCK eOfMainKind, eCK eAndSubKind1, eCK eAndSubKind2);
    bool G_bEachMainWith3Sub(eCK eOfMainKind, eCK eAndSubKind1, eCK eAndSubKind2, eCK eAndSubKind3);
    bool G_bEachSubOfMain(eCK eSubKind);
    bool G_bEach2SubOfMain(eCK eSubKind1, eCK eSubKind2);
    bool G_bEach3SubOfMain(eCK eSubKind1, eCK eSubKind2, eCK eSubKind3);

    bool bEachSubQuery(void);
    SubQuery* pEachSubQuery(void);
    SubQuery* pEachSubQuery(i8& ri_SubQuery);

    void vEndTransform(void);

    bool G_bFindPpy(eCK eMember, Stz* szPpy, eCK eOfCard= CK_NONE);
    void vFocusOnMain(void);
    void vForward(void);

    Card* pGetChild(i8 i_Child);
    Card* G_pGetMember(eCK eMember);

    bool G_bHasPriorToMain(bool bUpdateMember = false);
    bool G_bHasPriorToMainIf(eCK eOfPriorKind, bool bUpdateMember = false);
    bool G_bHasSubsequentToMain(bool bUpdateMember = false);
    bool G_bHasSubsequentToMainIf(eCK eOfSubsequentKind, bool bUpdateMember = false);

    void G_vInsertAfter(Card* pCard, Card* pAfter);
    void G_vInsertBefore(Card* pCard, Card* pBefore);
    bool G_bIsBoard(Stz* szBoardNm);
    bool G_bIsNextMain(eCK eMember);
    bool bIsTracing(void);

    bool bJustRestarted(void);

    bool bLess(void);
    bool G_bLocateCard(eCK eCard,  i8 i_After, i8& ri_Located);
    bool G_bLocateCard(eCK eCard, Stz* szWithValue, i8 i_After);
    bool G_bLocateCard(eCK eCard, eCK eSubCard, Stz* szWithValue, i8 i_After);
    bool G_bLocateCard(eCK eCard, Stz* szWithValue, Card* pAfter);

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

    bool G_bPlumbForSubMember(eCK eMember, eCK eSubCard, bool bUpdateMember= true);
    bool G_bPlumbForSubMember(Card* pMember, eCK eSubCard, bool bUpdateMember= true);
    bool G_bPlumbForSubValue(eCK eMember, eCK eSub, Stz* szValue, bool bUpdateMember= true);
    bool G_bPlumbForSubValue(Card* pMember, eCK eSub, Stz* szValue, bool bUpdateMember= true);
    void G_vPopIter(void);
    Card* pPrevChild(void);
    Card* pPrevChildIf(eCK eCard);
    void vPrintBench(eBK eBoard, i8 i_Board=C_UNKNOWN);
    void vPrintBoard(i8 iLineNo=C_UNKNOWN);
    void G_vPushBoard(Board* pBoard);
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
    bool G_bResolveCard(eCK eCard, i8 i_Before);
    bool G_bResolveCard(eCK eCard,char cWithValue, i8 i_Before);
    bool G_bResolveCard(eCK eCard, Stz* szWithValue, i8 i_Before);
    bool G_bResolveCard(eCK eCard, Stz* szWithValue, Card* pBefore);
    bool G_bResolveCardWithSub(eCK eOfMainKind, eCK eAndSubKind, i8 i_Before);
    bool G_bResolveCardWithSub(eCK eOfMainKind, eCK eAndSubKind, Card* pBefore);
    bool G_bResolveSubCard(eCK eGrandSub, Stz* szWithValue, i8 i_Before);
    bool G_bResolveSubCard(eCK eSub, eCK eGrandSub, Stz* szWithValue, i8 i_Before);
    bool G_bResolveSubCard(eCK eSub, Stz* szSubValue, eCK eGrandSub, i8 i_Before);
    bool G_bResolveSubCard(eCK eSub, i8 i_Before);
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
    void vSet(i8 iChild, Card* pChild);
    void G_vSetBoard(Board* pBoard);
    void G_vSkipNextMain(bool bClearMembers=false, bool bClearRegisters=false);
    void vSubQueryIteration(void);
    bool G_bSubsequentToMain(bool bUpdateMember = false);
    void G_vSubsequentToMain(bool bUpdateMember = false);

    void vTracer(Stz* szFormat, ...);
    void vTransform(const char* azFunc, Stz* szXdesc);
    i8   iTransformDepth(void);

    void G_vUpdateMember(eCK eMember, Card* pCard, const char* asFuncNm);
    void vUsingBoard(eBK eBoard, i8 i_Board);

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
