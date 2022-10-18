/*============================================================================*/
/* TransformerTool.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Bench.hpp"
#include "BoardIter.hpp"

#include "PermGen.hpp"

#include "TransformIterList.hpp"

namespace STORE
{
  class TransformerTool : public TransformIter
  {
  protected:

    static const bool C_DONT_PRINT    = false;
    static const bool C_PRINT_BOARD   = true;
    static const bool C_UPDATE_MEMBER = true;

    Bench*      m_pBench;
    CardCtl*    m_pCardCtl;
    bool        m_bTracing;
    i8          m_iTrace;

    bool        m_bPrintTransformation;
    bool        m_bInteractive;
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
    Card* m_pCard_new;
    Card* m_pCard_popped;
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

     eCK  eDecodeMain(Card* pCard);

    void vEndTransformation(void);

    Stz* szDescription(void);
    void vDescription(Stz* szDescription);

    Stz* szPhase(void);
    void vPhase(const char* szPhase);
    void vPrintBoard(void);

    void vSetPrintByKind(eCK eCard, bool bPrintByKind);
    void vSetPrintProperties(eCK eCard, bool bPrintProperties);
    void vSetPrintSubCards(eCK eCard, bool bPrintSubCards);

    Stz* szTransformation(void);
    void vTransformation(const char* azTransformation, bool bPrintTransformation=true);

    void vUsingBoard(eBK eBoard, i8 i_SubBoard);

  protected:
    bool bIsTracing(void);

    void vTracing(bool bTracing);
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
    bool E_bMoreMain(void);
    void E_vRemoveMain(eCK eCrossCheck);
    void E_vRemoveDeleteMain(eCK eCrossCheck);
    void E_vSkipNextMain(bool bClearMembers=false, bool bClearRegisters=false);
    bool E_bSubsequentToMain(bool bUpdateMember = false);
    void E_vSubsequentToMain(bool bUpdateMember = false);

    Card* F_pFetchMember(eCK eMember);

    bool H_bHasPriorToMain(bool bUpdateMember = false);
    bool H_bHasPriorToMainIf(eCK eOfPriorKind, bool bUpdateMember = false);
    bool H_bHasSubsequentToMain(bool bUpdateMember = false);
    bool H_bHasSubsequentToMainIf(eCK eOfSubsequentKind, bool bUpdateMember = false);

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

    bool S_bSearchForSubMember(eCK eMember, eCK eSubCard, bool bUpdateMember=true);
    bool S_bSearchForSubMember(Card* pMember, eCK eSubCard, bool bUpdateMember=true);

    void U_vUpdateMember(eCK eMember, Card* pCard, const char* asFuncNm);

    Stz* szT(eCK eMember);
    Stz* szMemberNm(eCK eMember);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
