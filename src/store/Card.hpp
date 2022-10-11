/*============================================================================*/
/* Card.hpp -- Abstract Card                                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "eAL.h"
#include "ePB.h"
#include "eCK.h"
#include "eGC.h"
#include "eRO.h"
#include "eTE.h"
#include "eYG.h"
#include "eXK.h"

#include "CardSet.hpp"

#include "Table.hpp"
#include "Ribbon.hpp"

namespace STORE
{
  class Card_AbsJoin;
  class Card_AggElist;
  class Card_AggLevel;
  class Card_Aggreg;
  class Card_Aggregates;
  class Card_As;

  class Card_Board;

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

  class Card_OrdAgg;
  class Card_ObyElist;

  class Card_AndPred;
  class Card_CmpPred;
  class Card_OrPred;
  class Card_Project;
  class Card_ProjElist;

  class Card_RBound;
  class Card_RpnTerms;

  class Card_SbyElist;
  class Card_Scan;
  class Card_ScanPred;
  class Card_Send;
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

static Stz* PPY_AggLevel     = "eAggLevel"_Stz;
static Stz* PPY_AggOp        = "bAggOp"_Stz;
static Stz* PPY_Card         = "pCard"_Stz;
static Stz* PPY_DebugOption  = "szDebugOption"_Stz;
static Stz* PPY_Exclude      = "bExclude"_Stz;
static Stz* PPY_Index        = "pIndex"_Stz;
static Stz* PPY_Invalid      = "bInvalid"_Stz;
static Stz* PPY_JoinedBuff   = "nJoinedBuff"_Stz;
static Stz* PPY_Network      = "nNetwork"_Stz;
static Stz* PPY_OrdHi        = "iOrdHi"_Stz;
static Stz* PPY_OrdLo        = "iOrdLo"_Stz;
static Stz* PPY_Permutation  = "i_Permutation"_Stz;
static Stz* PPY_PtndJoin     = "i_PtndJoin"_Stz;
static Stz* PPY_Rank         = "iRank"_Stz;
static Stz* PPY_SendKind     = "eSendKind"_Stz;
static Stz* PPY_SentOut      = "nSentOut"_Stz;
static Stz* PPY_StageTopo    = "eStageTopo"_Stz;
static Stz* PPY_Table        = "pTable"_Stz;
static Stz* PPY_TableSyn     = "cTableSyn"_Stz;
static Stz* PPY_TableUsed    = "bTableUsed"_Stz;

namespace STORE
{
  class Card : public Object
             , public CardSet
  {
  protected:
    char md_Bearing[256];

    eCK  m_eCard;
    i8   m_iCardNo;

    s256 m_sValue;
    char m_cMark1;
    char m_cMark2;

    TagSet* m_pProperties;

    i8   md__SeqNo;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card(eCK eCard);
    Card(eCK eCard, Stz* szValue);
    Card(eCK eCard, Stz* szValue, Stz* szPptName, i8 iPptValue);
    Card(eCK eCard, Stz* szValue, Stz* szPptName, Object* pPptValue);
    Card(eCK eCard, Stz* szValue, Stz* szPptName, Stz* szPptValue);
    void vInit(eCK eCard);

   ~Card(void) noexcept(false);

    static  void vCardLeaks(void);

    static  bool bCardsSameRecursive(Card* pCard_a, Card* pCard_b, i8 iDepth=0, bool bTestL0=false);

    static eCK  eTo_eCK(Stz* szCardNm);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    void vAddProperty_chr(Stz* szPpyName, char cPpyValue);
    void vAddProperty_int(Stz* szPpyName, i8 iPpyValue);
    void vAddProperty_nameOnly(Stz* szPpyName);
    void vAddProperty_ptr(Stz* szPpyName, Object* pPpyValue);
    void vAddProperty_stz(Stz* szPpyName, Stz* szPpyValue);
    void vAppendDupSubCards(Card* pFromCard);

    void vBearing(void);
    void vBearing(Card* pParentCard);

    eCK  eCard(void);
    void vCard(eCK eCard);
    Stz* szCardNm(void);
    i8   iCardNo(void);
    void vCardNo(i8 iCardNo);
    i8   nChildCards(void);
    void vClearAllMarks(void);

    void vDeleteChildCard(eCK eCard);
    void vDeleteProperty(Stz* szPptName);
    void vDeletePropertyRecursively(Stz* szPptName);
    void vDump(i8 iDepth=0, Stz* szTitle=nullptr);
    Card* pDupCardAndSubCards(void);
    Card_SubSet* pDupSubCards(void);

    bool bEquivalent(Card* pToCard);

    i8    iFindChild(Card* pChild);
    Card* pFindChild(eCK eCard);
    Card* pFindChild(eCK eCard, i8& i_Child);
    Card* pFindParent(Card* pSubCard);
    Card* pFindSubCard(eCK eCard);

    Card* pGetChild(i8 i_Child);
    Card* pGetChild(i8 i_Child, eCK& reCard);

    bool bHasChildWithValue(Stz* szValue);
    bool bHasProperty(Stz* szPpyName);
    bool bHasSubCard(Card* pSubCard);
    bool bHasSubCard(Card* pSubCard, Card*& rpParentCard);
    bool bHasValue(void);

    bool bIsCard(eCK eCard);
    bool bIsValue(Stz* szValue);

    char cMark1(void);
    void vMark1(char cMark1);
    char cMark2(void);
    i8   iMark2(void);
    void vMark2(i8 iMark2);
    void vMark2(char cMark2);
    void vMoveToLastChild(Card* pChildCard);

    char cPpy(i8 i_Property);
    char cPpy(Stz* szPpyNm);
    i8   iPpy(i8 i_Property);
    i8   iPpy(Stz* szPpyNm);
    Object* pPpy(i8 i_Property);
    Object* pPpy(Stz* szPptNm);
    Stz* szPpy(i8 i_Property);
    Stz* szPpy(Stz* szPptNm);
    Stz* szPpyNm(i8 i_Property);
    void vPrintCard(Ribbon* pRibbon, i8 iDepth, bool& rbPrune);
    void vPrintCard_byCustom(Ribbon* pRibbon, i8 iDepth, Stz* szCustomName);
    void vPrintCard_byKind(Ribbon* pRibbon, i8 iDepth);
    void vPrintCard_bySettings(Ribbon* pRibbon, i8 iDepth);
    void vPrintHeader(Ribbon* pRibbon);
    void vPrintName(Ribbon* pRibbon, i8 iDepth, Stz* szCustomName=nullptr);
    void vPrintNameValue(Ribbon* pRibbon, i8 iDepth);
    void vPrintProperties(Ribbon* pRibbon, i8 iDepth);
    void vPrintValue(Ribbon* pRibbon, i8 iDepth);
    void vPrintSubShort(Ribbon* pRibbon, i8 iDepth);
    i8   nProperties(void);
    eTN  eProperty(i8 i_Property);
    TagSet* pProperties(void);
    void vPropertiesEnsure(void);

    void vRemoveChild(Card* pChildCard);
    void vRemoveSubCard(Card* pSubCard);
    void vRenumberSubCards(Card* pParentCard);
    void vReplaceChildWithItsSubs(Card* pChildCard);
    void vReplaceChild(Card* pChildCard, Card* pWithCard);

    Stz* sz(void);

    void vTransferChildren(Card* pFromCard);
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
  

    Card_As*            pSub_as(void);
    Card_CalcBool*      pSub_calcBool(void);
    Card_Filter*        pSub_filter(void);
    Card_GbyElist*      pSub_gbyElist(void);
    Card_IdxColList*    pSub_idxColList(void);
    Card_IndexJoin*     pSub_indexJoin(void);
    Card_IndexList*     pSub_indexList(void);
    Card_IndexPtr*      pSub_indexPtr(void);
    Card_JoinPred*      pSub_joinPred(void);
    Card_LBound*        pSub_lBound(void);
    Card_CmpPred*       pSub_predCmp(void);
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
/* macro for find childCard function                                          */
/*----------------------------------------------------------------------------*/
#define __pChildFunction_Card(SPACENM,CARDNM,CN) \
  Card_##CARDNM* SPACENM::_pSub_##CARDNM(Card* pCard) \
  { \
    return _pCard_##CARDNM(pCard->pFindChild(CN)); \
  } \
EndDefine

/*----------------------------------------------------------------------------*/
/* macro for find subCard function                                            */
/*----------------------------------------------------------------------------*/
#define __pSubFunction_Card(SPACENM,CARDNM,CN) \
  Card_##CARDNM* SPACENM::_pSub_##CARDNM(Card* pCard) \
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
