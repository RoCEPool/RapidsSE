/*============================================================================*/
/* TransformerTool.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Bench.hpp"
#include "BoardIter.hpp"

#include "PermGen.hpp"
#include "TransformIterList.hpp"

#include "Table.hpp"

namespace STORE
{
  class TransformerTool : public TransformIter
  {
  protected:


    Bench*      m_pBench;
    CardCtl*    m_pCardCtl;
    bool        m_bTrace;
    i8          m_iTrace;

    bool        m_bPrintPhase;
    bool        m_bPrintXform;
    bool        m_bInteractive;
    b8          m_bBeenCued;
    b8          m_bCueingDisabled;

    s32         m_sPhase;
    s256        m_sTransformation;
    s256        m_sDescription;


    PermGen*    m_pPermGen;

    Table*      m_pTable;
    StreamDef*  m_pStreamDef;
    IdxVec*     m_pIdxVec;
    Index*      m_pIndex;

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

    Card_Term* m_pTerm_0;
    Card_Term* m_pTerm_1;

    TransformIterList* m_pTransformIterList;

    s256 m_sT;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    TransformerTool(eCN eClass, Bench* pBench);
   ~TransformerTool(void) noexcept(false);

    Board* pBoard(void);

    eOA eCueUser(Stz* szPhase, Stz* szTransformation= nullptr);

    void vEndTransformation(void);

    Stz* szDescription(void);
    void vDescription(Stz* szDescription);

    Stz* szPhase(void);
    void vPhase(bool bPrintPhase, const char* szPhase);
    void vPrintBench(eBK eBoard, i8 i_SubBoard=C_UNKNOWN);
    void vPrintBoard(void);

    void vSetPrintByKind(eCK eCard, bool bPrintByKind);
    void vSetPrintProperties(eCK eCard, bool bPrintProperties);
    void vSetPrintSubCards(eCK eCard, bool bPrintSubCards);

    Stz* szTransformation(void);
    void vTransformation(const char* azTransformation, bool bPrintTransformation=true);

    void vUsingBoard(eBK eBoard, i8 i_SubBoard);

  protected:
    bool bIsTracing(void);

    void vTracing(bool bPrintXform, bool bTrace);
    void vTracer(Stz* szFormat, ...);

    void C_vAppendChild(eCK eParent, eCK eChild);
    void C_vAppendChild(eCK eParent, Card* pChild);
    void C_vDuplicateMember(eCK eMember);
    void C_vRemoveChild(eCK eParent, eCK eChild);
    void C_vRemoveDeleteChild(eCK eParent, eCK eChild);
    void C_vRemoveDeleteMain(eCK eMain);
    void C_vPromoteChildren(eCK eParent);

    void D_vDeleteMember(eCK eMember);
    void D_vDeleteMember(Card* pMember);

    bool E_bEachMain(void);
    bool E_bEachMain(eCK eOfMainKind);
    bool E_bEachMainWithSub(eCK eOfMainKind, eCK eAndSubKind);
    bool E_bEachMainWithSub(eCK eOfMainKind, eCK eAndSubKind, Stz* szSubValue);
    bool E_bEachMainWith2Sub(eCK eOfMainKind, eCK eAndSubKind1, eCK eAndSubKind2);
    i8   E_i_Main(void);
    bool E_bMoreMain(void);
    void E_vRemoveMain(eCK eCrossCheck);
    void E_vRemoveDeleteMain(eCK eCrossCheck);
    void E_vSkipNextMain(bool bClearMembers=false, bool bClearRegisters=false);
    bool E_bSubsequentToMain(bool bUpdateMember = false);
    void E_vSubsequentToMain(bool bUpdateMember = false);

    Card* G_pGetMember(eCK eMember);

    bool H_bHasPriorToMain(bool bUpdateMember = false);
    bool H_bHasPriorToMainIf(eCK eOfPriorKind, bool bUpdateMember = false);
    bool H_bHasSubsequentToMain(bool bUpdateMember = false);
    bool H_bHasSubsequentToMainIf(eCK eOfSubsequentKind, bool bUpdateMember = false);

    bool L_bLocateCard(eCK eCard, Stz* szWithValue, i8 i_After);
    bool L_bLocateCard(eCK eCard, Stz* szWithValue, Card* pAfter);

    void M_vClearMembers(void);
    void M_vClearMembersRegisters(void);
    void M_vClearRegisters(void);
    void M_vClearRegistersIf(eCK eCard);
    void M_vClearRegistersIf(Card* pCard);
    void M_vPushIter(void);
    void M_vPushIter(Board* pBoard);
    void M_vPopIter(void);
    void M_vRestart(void);

    void N_vNewMember(eCK eMember, bool bUpdateMember=true);
    void N_vNewMember(eCK eMember, Stz* szValue, bool bUpdateMember=true);

    bool R_bResolveCard(eCK eCard, Stz* szWithValue, i8 i_Before);
    bool R_bResolveCard(eCK eCard, Stz* szWithValue, Card* pBefore);

    bool P_bPlumbForSubMember(eCK eMember, eCK eSubCard, bool bUpdateMember= true);
    bool P_bPlumbForSubMember(Card* pMember, eCK eSubCard, bool bUpdateMember= true);

    void U_vUpdateMember(eCK eMember, Card* pCard, const char* asFuncNm);

    bool Y_bFindPpy(eCK eMember, Stz* szPpy, eCK eOfCard=CK_NONE);


    Stz* szT(eCK eMember);
    Stz* szMemberNm(eCK eMember);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
