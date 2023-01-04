/*============================================================================*/
/* Xgearing.hpp --                                                            */
/*============================================================================*/
#pragma once

#include "CardIter.hpp"
#include "Bench.hpp"

#include "PermGen.hpp"

#include "Table.hpp"
#include "PtnDef.hpp"

namespace STORE
{
  class Xgearing : public CardIter
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Xgearing(eCN eClass, Bench* pBench);
   ~Xgearing(void) noexcept(false);

    Board* pBoard(void);

    eOA eCueUser(Stz* szPrompt);

    void vEndTransform(void);

    void vFocusOnMain(void);

    void vPrintBench(eBK eBoard, i8 i_Board=C_UNKNOWN);
    void vPrintBoard(i8 iLineNo=C_UNKNOWN);

    void vSetPrintByKind(eCK eCard, bool bPrintByKind);
    void vSetPrintProperties(eCK eCard, bool bPrintProperties);
    void vSetPrintSubCards(eCK eCard, bool bPrintSubCards);

    void vTransform(const char* azFunc, Stz* szXdesc);
    i8   iTransformDepth(void);

    void vUsingBoard(eBK eBoard, i8 i_Board);

    Stz* szXdesc(void);
    void vXdesc(Stz* szDescription);
    void vXform(Stz* szTransformation);
    void vXhand(const char* asXhand);

  protected:
    void G_vCopyCard(Card* pCard);

    void G_vDeleteMember(eCK eMember);
    void G_vDeleteMember(Card* pMember);
    void G_vDeleteWhenEmptySub(eCK eMain, eCK eSub);
    void G_vDuplicateMember(eCK eMember);

    bool G_bEachMain(void);
    bool G_bEachMain(eCK eOfMainKind);
    bool G_bEachMainWithSub(eCK eOfMainKind, eCK eAndSubKind);
    bool G_bEachMainWithSub_reverse(eCK eOfMainKind, eCK eAndSubKind);
    bool G_bEachMainWithSub(eCK eOfMainKind, eCK eAndSubKind, Stz* szSubValue);
    bool G_bEachMainWith2Sub(eCK eOfMainKind, eCK eAndSubKind1, eCK eAndSubKind2);
    bool G_bEachMainWith3Sub(eCK eOfMainKind, eCK eAndSubKind1, eCK eAndSubKind2, eCK eAndSubKind3);

    bool G_bEachSubOfMain(eCK eSubKind);
    bool G_bEach2SubOfMain(eCK eSubKind1, eCK eSubKind2);
    bool G_bEach3SubOfMain(eCK eSubKind1, eCK eSubKind2, eCK eSubKind3);

    bool G_bFindPpy(eCK eMember, Stz* szPpy, eCK eOfCard= CK_NONE);


    bool G_bHasPriorToMain(bool bUpdateMember = false);
    bool G_bHasPriorToMainIf(eCK eOfPriorKind, bool bUpdateMember = false);
    bool G_bHasSubsequentToMain(bool bUpdateMember = false);
    bool G_bHasSubsequentToMainIf(eCK eOfSubsequentKind, bool bUpdateMember = false);

    void G_vInsertAfter(Card* pCard, Card* pAfter);
    void G_vInsertBefore(Card* pCard, Card* pBefore);
    bool G_bIsBoard(Stz* szBoardNm);
    bool G_bIsNextMain(eCK eMember);

    bool G_bLocateCard(eCK eCard, Stz* szWithValue, i8 i_After);
    bool G_bLocateCard(eCK eCard, eCK eSubCard, Stz* szWithValue, i8 i_After);
    bool G_bLocateCard(eCK eCard, Stz* szWithValue, Card* pAfter);

    bool G_bMoreMain(void);
    void G_vMorphCard(Card* pCard, eCK eCard);

    void G_vNewMemberBefore(eCK eMember, Stz* szValue, eCK eOf, bool bUpdateMember= true);
    void G_vNewMemberOf(eCK eMember, Stz* szValue, eCK eOf, bool bUpdateMember= true);

    bool G_bPlumbForSubMember(eCK eMember, eCK eSubCard, bool bUpdateMember= true);
    bool G_bPlumbForSubMember(Card* pMember, eCK eSubCard, bool bUpdateMember= true);
    bool G_bPlumbForSubValue(eCK eMember, eCK eSub, Stz* szValue, bool bUpdateMember= true);
    bool G_bPlumbForSubValue(Card* pMember, eCK eSub, Stz* szValue, bool bUpdateMember= true);
    void G_vPushBoard(Board* pBoard);
    bool G_bPushBoard(i8 i_Board);
    bool G_bPushBoard(Stz* szBoardNm);
    void G_vPushNewChild(Card* pParent, eCK eChild);
    void G_vPushNewChild(Card* pParent, eCK eChild, Stz* szValue);
    void G_vPushIter(void);
    void G_vPopIter(void);

    void G_vRemoveMain(eCK eCrossCheck);
    void G_vRemoveDeleteMain(eCK eCrossCheck);
    bool G_bResolveCard(eCK eCard, i8 i_Before);
    bool G_bResolveCard(eCK eCard,char cWithValue, i8 i_Before);
    bool G_bResolveCard(eCK eCard, Stz* szWithValue, i8 i_Before);
    bool G_bResolveCard(eCK eCard, Stz* szWithValue, Card* pBefore);
    bool G_bResolveCardWithSub(eCK eOfMainKind, eCK eAndSubKind, i8 i_Before);
    bool G_bResolveCardWithSub(eCK eOfMainKind, eCK eAndSubKind, Card* pBefore);
    bool G_bResolveSubCard(eCK eSub, eCK eGrandSub, Stz* szWithValue, i8 i_Before);
    bool G_bResolveSubCard(eCK eSub, i8 i_Before);
    void G_vRestart(void);
    bool G_bRewindMain(void);

    void G_vSetBoard(Board* pBoard);
    void G_vSkipNextMain(bool bClearMembers=false, bool bClearRegisters=false);
    bool G_bSubsequentToMain(bool bUpdateMember = false);
    void G_vSubsequentToMain(bool bUpdateMember = false);



    Stz* szT(eCK eMember);
    void vTracer(Stz* szFormat, ...);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
