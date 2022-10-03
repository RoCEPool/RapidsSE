/*============================================================================*/
/* Card.hpp -- Abstract Card                                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "eAL.h"
#include "ePB.h"
#include "eCK.h"
#include "eCP.h"
#include "eGC.h"
#include "eRO.h"
#include "eTE.h"
#include "eYG.h"
#include "eXK.h"

#include "Table.hpp"

#include "Ribbon.hpp"
#include "ListExo2_t.hpp"


namespace STORE {
  class Board;
  class Card;
  class Card_SubSet;
}

namespace STORE {
  class Card_AbsJoin;
  class Card_AggElist;
  class Card_AggLevel;
  class Card_Aggreg;
  class Card_Aggregates;
  class Card_As;

  class Card_Calc;
  class Card_CalcBool;
  class Card_CandIndex;
  class Card_CardPtr;
  class Card_Combined;

  class Card_Debug;
  class Card_Direction;

  class Card_EBound;
  class Card_Exists;
  class Card_Expr;
  class Card_ExprBool;

  class Card_Filter;

  class Card_GbyElist;
  class Card_Generate;

  class Card_Hash;
  class Card_Having;

  class Card_IdxColNm;
  class Card_IdxColList;
  class Card_IndexPtr;
  class Card_IndexJoin;
  class Card_IndexList;
  class Card_IndexPred;
  class Card_IndexScan;

  class Card_JoinPred;

  class Card_LBound;
  class Card_Limit;

  class Card_Map;
  class Card_MapSet;
  class Card_Mode;

  class Card_StageNm;

  class Card_On;
  class Card_Option;
  class Card_OrdAgg;
  class Card_ObyElist;

  class Card_PredAnd;
  class Card_PredCmp;
  class Card_PredOr;
  class Card_Project;
  class Card_ProjElist;

  class Card_RBound;
  class Card_Rows;
  class Card_RpnTerms;

  class Card_SbyElist;
  class Card_Scan;
  class Card_ScanPred;
  class Card_Send;
  class Card_SendKind;
  class Card_Sends;
  class Card_Sort;
  class Card_Stage;
  class Card_StageTopo;
  class Card_Stream;
  class Card_StreamElist;
  class Card_StreamColList;
  class Card_SubSet;

  class Card_Table;
  class Card_TablePtr;
  class Card_Term;
  class Card_Type;

  class Card_UBound;
  class Card_Unique;
}

namespace STORE
{
  class CardSet : public ListExo2_t<Card>
  {
  public:
    CardSet(void);
   ~CardSet(void) noexcept(false);

    Card* pGetChild(i8 i_Child);
    Card* pGetChildCard(i8 i_Child, eCK& reCard);

    void vRemoveSubCards(eCK eCard);
  };

  typedef ListExoIter_t<Card> CardIter;


  class Card : public Object
             , public CardSet // ListExo2_t<Card>
  {
  protected:
    char m_caRef[256];

    eCK  m_eCard;
    i8   m_iCardNo;

    i8   m_iSeqNo;

    char m_cMark1;
    char m_cMark2;

    TagSet* m_pProperties;

    bool  m_bPrintNm;

    s256  m_sValue;
  
  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card(eCK eCard);
   ~Card(void) noexcept(false);

    static  void vCardLeaks(void);

    static  bool bCardsSameRecursive(Card* pCard_a, Card* pCard_b, i8 iDepth=0, bool bTestL0=false);

    static eCK  eToECard(Stz* szCardNm);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    
    void vAddProperty_nameOnly(Stz* szName);
    void vAddProperty_int(Stz* szName, i8 iValue);
    void vAddProperty_ptr(Stz* szName, Object* pValue);
    void vAddProperty_stz(Stz* szName, Stz* szValue);
    void vAppendDupSubCards(Card* pCard_SubSet);

    eCK  eCard(void);
    void vCard(eCK eCard);
    Stz* szCardNm(void);
    i8   iCardNo(void);
    void vCardNo(i8 iCardNo);
    i8   nChildCards(void);
    //SZ Stz* szColumnRef(void);
    i8   iColumnRef_columnOrd(void);
    char cColumnRef_tableSyn(void);
    //V void vColumnRef(Stz* szColumnRef);
    void vCutProperty(Stz* szName);
    void vCutSubCard(Card* pSubCard);

    void vDeleteChildCard(eCK eCard);
    void vDump(i8 iDepth=0, Stz* szTitle=nullptr);
    Card* pDupCardAndSubCards(void);
    Card_SubSet* pDupSubCards(void);

    void vEachSubCard(std::function<void(Card* pCard)> f_vClosure,
                      eCK eCard=CK_NONE);
    bool bEquivalent(Card* pCard_arg);

    Card* pFindChildCard(eCK eCard);
    Card* pFindChildCard(eCK eCard, i8& i_Child);
    Card* pFindParent(Card* pSubCard_arg);
    Card* pFindSubCard(eCK eCard);

    i8      iGetProperty(i8 i_Property);
    i8      iGetProperty(Stz* szName);
    Object* pGetProperty(i8 i_Property);
    Object* pGetProperty(Stz* szName);
    Stz*    szGetProperty(i8 i_Property);
    Stz*    szGetProperty(Stz* szName);
    Stz*    szGetPropertyNm(i8 i_Property);

    bool bHasEBound(void);
    void vHasEBound(bool bHasEBound);
    bool bHasLBound(void);
    void vHasLBound(bool bHasLBound);
    bool bHasProperty(Stz* szName);
    bool bHasSubCard(Card* pGivenCard);
    bool bHasSubCard(Card* pGivenCard, Card*& pParentCard);
    bool bHasUBound(void);
    void vHasUBound(bool bHasUBound);

    char cMark1(void);
    void vMark1(char cMark1);
    char cMark2(void);
    i8   iMark2(void);
    void vMark2(i8 iMark2);
    void vMark2(char cMark2);
    void vMoveToLastChild(Card* pChildCard);

    CardIter* pNewSubCardIter(void);

    i8   iOrdHi(void);
    void vOrdHi(i8 iOrdHi);
    i8   iOrdLo(void);
    void vOrdLo(i8 iOrdLo);

    void vPrintCard(Ribbon* pRibbon, i8 iDepth, bool& rbPrune);
    void vPrintCard_byForm(Ribbon* pRibbon, i8 iDepth, bool& rbPrune, eCP ePrintForm);
    void vPrintCard_compressed(Ribbon* pRibbon, i8 iDepth, bool& rbPrune, eCP ePrintForm);
    void vPrintCard_default(Ribbon* pRibbon, i8 iDepth, bool& rbPrune, eCP ePrintForm);
    void vPrintCardDetails(Ribbon* pRibbon, i8 iDepth, bool& rbPrune);
    void vPrintCardName(Ribbon* pRibbon, i8 iDepth, bool& rbPrune);
    void vPrintCardProperties(Ribbon* pRibbon, i8 iDepth, bool& brPrune, bool bPrintPtyName= true);
    void vPrintCardValue(Ribbon* pRibbon, i8 iDepth, bool& rbPrune);
    void vPrintCompressed_childPairs(Ribbon* pRibbon, i8 iDepth, bool& rbPrune);
    void vPrintHeader(Ribbon* pRibbon);
    i8   nProperties(void);
    eTN  eProperty(i8 i_Property);
    TagSet* pProperties(void);
    void vPropertiesEnsure(void);
    Stz* szPropertyNm(i8 i_Property);

    void vRemoveChild(Card* pCardRemove);
    void vRemoveSubCard(Card* pCardRemove);
    void vRenumberSubCards(Card* pParentCard);
    void vReplaceChildWithKids(Card* pChildCard);
    void vReplaceSubCard(Card* pSubCard, Card* pWithCard);


    Stz* sz(void);

    //SZ Stz* szTablePtn(void);
    //void vTablePtn(Stz* szTablePtn);
    char cTableSyn(void);
    void vTableSyn(char cTableSyn);
    void vTransferSubCards(Card* pCards);
    void vTransferFrom(Card* pCards, Card* pFrom,
                       Card* pAt=nullptr, bool bAfter=true);
    void vTransferSubset(Card* pCards, Card* pFrom, Card* pTill,
                         Card* pAt=nullptr, bool bAfter=true);
    void vTransferTill(Card* pCards, Card* pTill,
                       Card* pAt=nullptr, bool bAfter=true);

    Stz* szValue(void);
    void vValue(Stz* szValue);
    void vVisitAllSub(eCK eCard,std::function<bool(Card* pCard)> bfApply);


    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  
    eAL  eAggLevel(void);
    void vAggLevel(eAL eAggLevel);
    bool bAggOp(void);
    void vAggOp(void);
    //SZ Stz* szAnd(void);
    //void vAnd(Stz* szAnd);
    //SZ Stz* szAs(void);
    //void vAs(Stz* szAs);

    ePB  eBound(void);

    Card* pCard(void);
    void vCard(Card* pCard);
    void vCardPrint(Ribbon* pRibbon, i8 iDepth, bool rbPrune);
    void vClearAllMarks(void);
    //SZ Stz* szColNm(void);
    //void vColNm(Stz* szColNm);
    Column* pColumn(void);
    void vColumn(Column* pColumn);
    Column* pColumn1(void);
    void vColumn1(Column* pColumn1);
    Column* pColumn2(void);
    void vColumn2(Column* pColumn2);
    //SZ Stz* szComment(void);
    //void vComment(Stz* szComment);
    void vConcatTerms(Stz* szTerms);
    void vConcatTerms(Stz* szTerms, i8 i_From, i8 i_Till=C_UNKNOWN);

    //SZ Stz* szCmp(void);
    //void vCmp(Stz* szCmp);

    //SZ Stz* szDirn(void);
//    void vDirn(Stz* szAsNm);
//
    i8   iGenerate(void);
    void vGenerate(i8 iGenerate);
  
    bool bHasAggOp(void);
    Card_Term* pHeadTerm(void);

    Index* pIndex(void);
    void vIndex(Index* pIndex);
    bool bIsAggOp(void);
    bool bIsCard(eCK eCard);
    bool bIsColumnRef(void);
    bool bIsOrdinalColumnAtom(void);
    bool bIsOrdinalIndexAtom(void);
    bool bIsOrdinalStreamTerm(void);
    bool bIsOrdinalIndexTerm(void);
    bool bIsPred(void);
    bool bIsSame(Stz* szArg);
    bool bIsSeparator(void);

    Stz* szLevel(void);
    void vLevel(Stz* szLevel);

    Stz* szMapBack(void);
    void vMapBack(Stz* szMapBack);
    Stz* szMapFrom(void);
    void vMapFrom(Stz* szMapFrom);
    //SZ Stz* szMode(void);
    void vMode(Stz* szMode);
  
//    Stz* szStageNm(void);
//    void vStageNm(Stz* szStageNm);

    i8   iOption(void);
    void vOption(i8 iOption);
    //SZ Stz* szOption(void);
    //void vOption(Stz* szOption);
    //SZ Stz* szOr(void);
    //void vOr(Stz* szOr);
    //SZ Stz* szOrdinal(void);
    //void vOrdinal(Stz* szOrdinal);

    i8   iOrdinalStreamColNo(void);
    i8   iOrdinalIndexColNo(void);

    Card_Term* pPopTerm(void);
  
    //SZ Stz* szQueryNm(void);
    //void vQueryNm(Stz* szQueryNm);

    i8   iRank(void);
    void vRank(i8 iRank);
    void vRef(void);
    void vRef(Card* pParentCard);
    eRO  eRelOp(void);
    i8   nRows(void);
    void vRows(i8 nRows);
  
    eXK  eSendKind(void);
    void vSendKind(eXK eSendKind);
    i8   iSends(void);
    void vSends(i8 iSends);
    //SZ Stz* szStageNm(void);
    //void vStageNm(Stz* szStageNm);
    eYG  eStageTopo(void);
    void vStageTopo(eYG  eStageTopology);
    StreamDef* pStreamDef(void);
    void vStreamDef(StreamDef* pStreamDef);

    Table* pTable(void);
    void vTable(Table* pTable);
    Table* pTable1(void);
    void vTable1(Table* pTable1);
    Table* pTable2(void);
    void vTable2(Table* pTable2);
    //SZ Stz* szTerm(void);
    //void vTerm(Stz* szTerm);
    eTE  eTermEval(void);
    //SZ Stz* szType(void);
    char cTypeShort(void);
    //void vType(Stz* szType);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  

    void vCommentPrint(Ribbon* pRibbon, i8 iDepth);
    void vIndexPrint(Ribbon* pRibbon, i8 iDepth);
    void vStreamElistPrint(Ribbon* pRibbon, i8 iDepth);
    void vTablePrint(Ribbon* pRibbon, i8 iDepth);

    Card_As*            pSub_as(void);
    Card_CalcBool*      pSub_calcBool(void);
    Card_Filter*        pSub_filter(void);
    Card_GbyElist*      pSub_gbyElist(void);
    Card_IdxColList*    pSub_idxColList(void);
    Card_IndexJoin*     pSub_indexJoin(void);
    Card_IndexList*     pSub_indexList(void);
    Card_JoinPred*      pSub_joinPred(void);
    Card_LBound*        pSub_lBound(void);
    Card_PredCmp*       pSub_predCmp(void);
    Card_SbyElist*      pSub_sbyElist(void);
    Card_Scan*          pSub_scan(void);
    Card_ScanPred*      pSub_scanPred(void);
    Card_StageTopo*     pSub_stageTopo(void);
    Card_Stream*        pSub_stream(void);
    Card_StreamColList* pSub_streamColList(void);
    Card_Table*         pSub_table(void);
    Card_TablePtr*      pSub_tablePtr(void);
    Card_UBound*        pSub_uBound(void);

  private:

    Stz* szDecodeEnum(Stz* szEnum, i8 iEnum);
  
  protected:
    Footer* aFooter(void);
  };


  Card* _pCard(void* aAddr);
}

/*----------------------------------------------------------------------------*/
/* macro for test is class                                                    */
/*----------------------------------------------------------------------------*/
#define __bIsCard(SPACENM,CLASSNM,CK) \
bool SPACENM::_bIs##CLASSNM(void* aAddr) \
{ \
  Object* pObject = reinterpret_cast<Object*>(aAddr); \
  \
  if (CN_CARD != pObject->eGetClassNo()) \
    return false; \
  \
  Card* pCard = reinterpret_cast<Card*>(aAddr); \
  \
  if (CK != pCard->eCard()) \
    return false; \
  \
  return true; \
} \
EndDefine


/*----------------------------------------------------------------------------*/
/* RASSERT_CARD macro                                                         */
/*----------------------------------------------------------------------------*/
#define RASSERT_CARD(pOBJECT,eCARD) \
  \
  if (pOBJECT && CN_CARD != pOBJECT->eGetClassNo()) \
    BASE::vThrowType(FK_TYPE, \
                     __func__,__FILE__,__LINE__, \
                     pOBJECT,CN_CARD); \
  \
  if (pOBJECT && eCARD != ((Card*)pOBJECT)->eCard()) \
    BASE::vThrowType(FK_TYPE,\
                     __func__,__FILE__,__LINE__, \
                     pOBJECT,CN_CARD); \
  \
EndDefine

/*----------------------------------------------------------------------------*/
/* macro for standard safe cast function                                      */
/*----------------------------------------------------------------------------*/
#define __pCastFunction_Card(SPACENM,CLASSNM,CN) \
  CLASSNM* SPACENM::_p##CLASSNM(void* aAddr) \
  { \
    CLASSNM* pObject = reinterpret_cast<CLASSNM*>(aAddr); \
    \
    RASSERT_CARD(pObject,CN); \
    \
    return pObject; \
  } \
EndDefine

/*----------------------------------------------------------------------------*/
/* macro for standard safe cast function                                      */
/*----------------------------------------------------------------------------*/
#define __pGetSubFunction_Card(SPACENM,CARDNM,CN) \
  Card_##CARDNM* SPACENM::_pGetSub_##CARDNM(Card* pCard) \
  { \
    return _pCard_##CARDNM(pCard->pFindSubCard(CN)); \
  } \
EndDefine

/*----------------------------------------------------------------------------*/
/* macro for deleting card                                                    */
/*----------------------------------------------------------------------------*/
#define DELETE_CARD(__pCard) \
  {                          \
    delete __pCard;          \
    __pCard = nullptr;       \
  }                          \
EndDefine

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
