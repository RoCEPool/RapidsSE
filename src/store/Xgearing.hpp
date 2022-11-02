/*============================================================================*/
/* Xgearing.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Bench.hpp"
#include "BoardIter.hpp"

#include "PermGen.hpp"
#include "TransformIterList.hpp"

#include "Table.hpp"
#include "PtnDef.hpp"

namespace STORE
{
  class Xgearing : public Xsentinals
  {
  protected:

    s32         m_sXhand;
    s256        m_sXform;
    s256        m_sXdesc;

    Bench*      m_pBench;
    CardCtl*    m_pCardCtl;
    bool        m_bTrace;
    i8          m_iTrace;


    PermGen*    m_pPermGen;

    Column*     m_pColumn;
    IdxVec*     m_pIdxVec;
    Index*      m_pIndex;
    IndexDef*   m_pIndexDef;
    StreamDef*  m_pStreamDef;
    Table*      m_pTable;

    Card* m_pCard_child;
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

    Card_Term* m_pTerm_0;
    Card_Term* m_pTerm_1;



    TransformIterList* m_pTransformIterList;

    s256 m_sT;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Xgearing(eCN eClass, Bench* pBench);
   ~Xgearing(void) noexcept(false);

    Board* pBoard(void);

    eOA eCueUser(Stz* szPrompt);

    void vEndTransform(void);


    void vPrintBench(eBK eBoard, i8 i_Board=C_UNKNOWN);
    void vPrintBoard(bool bCue=true, i8 iLineNo=C_UNKNOWN);

    void vSetPrintByKind(eCK eCard, bool bPrintByKind);
    void vSetPrintProperties(eCK eCard, bool bPrintProperties);
    void vSetPrintSubCards(eCK eCard, bool bPrintSubCards);

    void vTransform(const char* azTransformation);

    void vUsingBoard(eBK eBoard, i8 i_Board);

    Stz* szXdesc(void);
    void vXdesc(Stz* szDescription);
    Stz* szXform(void);
    void vXform(Stz* szTransformation);
    Stz* szXhand(void);
    void vXhand(const char* asXhand);

  protected:
    bool bIsTracing(void);

    void vTracer(Stz* szFormat, ...);

    void G_vAppendChild(eCK eParent, eCK eChild);
    void G_vAppendChild(eCK eParent, Card* pChild);
    void G_vDuplicateMember(eCK eMember);

    void G_vDeleteMember(eCK eMember);
    void G_vDeleteMember(Card* pMember);

    bool G_bEachMain(void);
    bool G_bEachMain(eCK eOfMainKind);
    bool G_bEachMainWithSub(eCK eOfMainKind, eCK eAndSubKind);
    bool G_bEachMainWithSub(eCK eOfMainKind, eCK eAndSubKind, Stz* szSubValue);
    bool G_bEachMainWith2Sub(eCK eOfMainKind, eCK eAndSubKind1, eCK eAndSubKind2);
    i8   G_i_Main(void);
    bool G_bMoreMain(void);
    void G_vRemoveMain(eCK eCrossCheck);
    void G_vRemoveDeleteMain(eCK eCrossCheck);
    void G_vSkipNextMain(bool bClearMembers=false, bool bClearRegisters=false);
    bool G_bSubsequentToMain(bool bUpdateMember = false);
    void G_vSubsequentToMain(bool bUpdateMember = false);

    bool G_bPlumbForSubMember(eCK eMember, eCK eSubCard, bool bUpdateMember= true);
    bool G_bPlumbForSubMember(Card* pMember, eCK eSubCard, bool bUpdateMember= true);
    bool G_bPlumbForSubValue(eCK eMember, eCK eSub, Stz* szValue, bool bUpdateMember= true);
    bool G_bPlumbForSubValue(Card* pMember, eCK eSub, Stz* szValue, bool bUpdateMember= true);

    Card* G_pGetMember(eCK eMember);

    bool G_bHasPriorToMain(bool bUpdateMember = false);
    bool G_bHasPriorToMainIf(eCK eOfPriorKind, bool bUpdateMember = false);
    bool G_bHasSubsequentToMain(bool bUpdateMember = false);
    bool G_bHasSubsequentToMainIf(eCK eOfSubsequentKind, bool bUpdateMember = false);

    bool G_bLocateCard(eCK eCard, Stz* szWithValue, i8 i_After);
    bool G_bLocateCard(eCK eCard, eCK eSubCard, Stz* szWithValue, i8 i_After);
    bool G_bLocateCard(eCK eCard, Stz* szWithValue, Card* pAfter);

    void G_vClearMembers(void);
    void G_vClearMembersRegisters(void);
    void G_vClearRegisters(void);
    void G_vClearRegistersIf(eCK eCard);
    void G_vClearRegistersIf(Card* pCard);
    void G_vPushBoard(Board* pBoard);
    bool G_bPushBoard(Stz* szBoardNm);
    void G_vPushIter(void);
    void G_vPushIter(Board* pBoard);
    void G_vPopBoard(void);
    void G_vPopIter(void);
    void G_vRestart(void);

    void G_vNewMember(eCK eMember, bool bUpdateMember= true);
    void G_vNewMember(eCK eMember, Stz* szValue, bool bUpdateMember= true);

    bool G_bResolveCard(eCK eCard, Stz* szWithValue, i8 i_Before);
    bool G_bResolveCard(eCK eCard, Stz* szWithValue, Card* pBefore);
    bool G_bResolveCardWithSub(eCK eOfMainKind, eCK eAndSubKind, i8 i_Before);
    bool G_bResolveCardWithSub(eCK eOfMainKind, eCK eAndSubKind, Card* pBefore);
    bool G_bResolveSubCard(eCK eSub, eCK eGrandSub, Stz* szWithValue, i8 i_Before);

    void G_vUpdateMember(eCK eMember, Card* pCard, const char* asFuncNm);

    bool G_bFindPpy(eCK eMember, Stz* szPpy, eCK eOfCard= CK_NONE);


    Stz* szT(eCK eMember);
    Stz* szMemberNm(eCK eMember);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
