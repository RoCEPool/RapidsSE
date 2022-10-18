/*============================================================================*/
/* TransformerTool.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Bench.hpp"
#include "BoardIter.hpp"

#include "PermGen.hpp"

#include "TransformIter.hpp"

namespace STORE
{
  class TransformerTool : public Object
  {
  protected:

    static const bool C_DONT_PRINT    = false;
    static const bool C_PRINT_BOARD   = true;
    static const bool C_UPDATE_MEMBER = true;

    Bench*      m_pBench;
    CardCtl*    m_pCardCtl;
    bool        m_bTracing;
    i8          m_iTrace;

    Board*      m_pBoard;
    eBK         m_eBoard;
    i8          m_i_SubBoard;
    Card_Deck*  m_pDeck;

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

    // the M iterator - - - - - - - - - - - - - - - - - - - - -
    struct
    {
      BoardIter pM;

      Card * p_PriorToMain;
      eCK    e_PriorToMain;
      Card * p_MainCard;
      eCK    e_MainCard;
      Card * p_SubOfMain;
      eCK    e_SubOfMain;
      Card * p_Sub2OfMain;
      eCK    e_Sub2OfMain;
      Card * p_SubsequentToMain;
      eCK    e_SubsequentToMain;

      Card * p_SubMember;
      eCK    e_SubMember;

      Card_AbsJoin *          p_absJoin;
      Card_Aggreg *           p_aggreg;
      Card_BoolCalc *         p_boolCalc;
      Card_Calc *             p_calc;
      Card_CmpPred *          p_cmpPred;
      Card_Combined *         p_combined;
      Card_Comment *          p_comment;
      Card_CoPtnJoin *        p_coPtnJoin;
      Card_CoPtnJoinList *    p_coPtnJoinList;
      Card_CoPtnPred *        p_coPtnPred;
      Card_CoPtnPredList *    p_coPtnPredList;
      Card_CrossPtnJoin *     p_crossPtnJoin;
      Card_CrossPtnJoinList * p_crossPtnJoinList;
      Card_Expr *             p_expr;
      Card_Filter *           p_filter;
      Card_GbyElist *         p_gbyElist;
      Card_IdxCol *           p_idxCol;
      Card_IdxColList *       p_idxColList;
      Card_IndexJoin *        p_indexJoin;
      Card_IndexList *        p_indexList;
      Card_IndexPtr *         p_indexPtr;
      Card_IndexScan *        p_indexScan;
      Card_JoinOrder *        p_joinOrder;
      Card_JoinOrderSeq *     p_joinOrderSeq;
      Card_JoinPred *         p_joinPred;
      Card_JoinPredList *     p_joinPredList;
      Card_KeyExists *        p_keyExists;
      Card_LBound *           p_lBound;
      Card_Param *            p_param;
      Card_ParamList *        p_paramList;
      Card_Permutation *      p_permutation;
      Card_Permutation *      p_permutation_low;
      Card_Project *          p_project;
      Card_PtndJoin *         p_ptndJoin;
      Card_PtndJoinSet *      p_ptndJoinSet;
      Card_SbyElist *         p_sbyElist;
      Card_Scan *             p_scan;
      Card_ScanPred *         p_scanPred;
      Card_Send *             p_send;
      Card_Sort *             p_sort;
      Card_Stage *            p_stage;
      Card_StageTopo *        p_stageTopo;
      Card_Stream *           p_stream;
      Card_StreamCol *        p_streamCol;
      Card_StreamColList *    p_streamColList;
      Card_SubSet *           p_subSet;
      Card_Table *            p_table;
      Card_TableList *        p_tableList;
      Card_TablePtn *         p_tablePtn;
      Card_TablePtr *         p_tablePtr;
      Card_Term *             p_term;
      Card_UBound *           p_uBound;
      Card_Where *            p_where;
    } M;

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
    bool E_bEachMainWith2Sub(eCK eOfMainKind, eCK eAndSubKind1, eCK eAndSubKind2);
    bool E_bMoreMain(void);
    void E_vRemoveMain(eCK eCrossCheck);
    void E_vRemoveDeleteMain(eCK eCrossCheck);
    void E_vSkipNextMain(bool bClearMembers=false, bool bClearRegisters=false);
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
