/*============================================================================*/
/* Card.hpp -- Abstract Card                                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "eAL.h"
#include "eCK.h"
#include "eYG.h"
#include "eXK.h"

#include "Table.hpp"

#include "Ribbon.hpp"
#include "ListExo2_t.hpp"


namespace STORE { class Board;
                  class Card_Properties; class Card_Term; }

namespace STORE
{
  class Card : public Object
             , public ListExo2_t<Card>
  {
  protected:
    i8   m_iCardNo;
    s32  m_sCardNm;
    eCK  m_eCard;
  
    s256 m_sText;   // TO BE REMOVED

    TagSet* m_pProperties;

    bool  m_bPrintNm;
  
  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card(eCK eCard);
   ~Card(void) noexcept(false);


    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    
    void vAddProperty(Stz* szName);
    void vAddProperty(Stz* szName, i8 iValue);
    void vAddProperty(Stz* szName, Object* pValue);
    void vAddProperty(Stz* szName, Stz* szValue);

    eCK  eCard(void);
    void vCardHeader(Ribbon* pRibbon);
    Stz* szCardNm(void);
    i8   iCardNo(void);
    void vCardNo(i8 iCardNo);
    void vCutProperty(Stz* szName);
  
    void vDump(i8 iDepth=0);
  
    void vEachSubCard(std::function<void(Card* pCard)> f_vClosure,
                      eCK eCard=CK_NONE);
  
    Card* pFindSubCard(eCK eCard);

    i8      iGetProperty(i8 i_Property);
    i8      iGetProperty(Stz* szName);
    Object* pGetProperty(i8 i_Property);
    Object* pGetProperty(Stz* szName);
    Stz*    szGetProperty(i8 i_Property);
    Stz*    szGetProperty(Stz* szName);
    Stz*    szGetPropertyNm(i8 i_Property);

    bool bHasProperty(Stz* szName);
  
    i8   iOfText(void);
    i8   nOfText(void);

    i8   nProperties(void);
    eTN  eProperty(i8 i_Property);
    Stz* szPropertyNm(i8 i_Property);

    void vRenumberSubCards(void);

    Stz* szText(void);
    void vText(Stz* szText);

    void vTransferAll(Card* pCards);
    void vTransferFrom(Card* pCards, Card* pFrom,
                       Card* pAt=nullptr, bool bAfter=true);
    void vTransferSubset(Card* pCards, Card* pFrom, Card* pTill,
                         Card* pAt=nullptr, bool bAfter=true);
    void vTransferTill(Card* pCards, Card* pTill,
                       Card* pAt=nullptr, bool bAfter=true);
  
    void vWalkAllCards(std::function<void(Card* pCard)> f_vClosure);

  
  
    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  
    eAL  eAggLvl(void);
    void vAggLvl(eAL eAggLvl);
    Stz* szAsNm(void);
    void vAsNm(Stz* szAsNm);
    Stz* szAtom(void);
    void vAtom(Stz* szAtom);
  
    Column* pColumn(void);
    void vColumn(Column* pColumn);
    Column* pColumn1(void);
    void vColumn1(Column* pColumn1);
    Column* pColumn2(void);
    void vColumn2(Column* pColumn2);

    bool bHasAggOp(void);

    Index* pIndex(void);
    void vIndex(Index* pIndex);
    bool bIsAggOp(void);
    bool bIsOrdinalColumnAtom(void);
    bool bIsOrdinalIndexAtom(void);
    bool bIsOrdinalColumnTerm(void);
    bool bIsOrdinalIndexTerm(void);
    bool bIsSame(Stz* szArg);
    bool bIsSeparator(void);

    Stz* szLevel(void);
    void vLevel(Stz* szLeve);

    Stz* szOption(void);
    void vOption(Stz* szOption);
    i8   iOrdinalColumnValue(void);
    i8   iOrdinalIndexValue(void);

    Card_Term* pPopTerm(void);
  
    Stz* szQueryNm(void);
    void vQueryNm(Stz* szQueryNm);

    Stz* szRows(void);
    void vRows(Stz* szRows);
  
    eXK  eSendKind(void);
    void vSendKind(eXK eSendKind);
    Stz* szStageNm(void);
    void vStageNm(Stz* szStageNm);
    eYG  eStageTopology(void);
    void vStageTopology(eYG  eStageTopology);
    StreamDef* pStreamDef(void);
    void vStreamDef(StreamDef* pStreamDef);

    Table* pTable(void);
    void vTable(Table* pTable);
    Table* pTable1(void);
    void vTable1(Table* pTable1);
    Table* pTable2(void);
    void vTable2(Table* pTable2);
    Stz* szTerm(void);
    void vTerm(Stz* szTerm);
    Stz* szType(void);
    void vType(Stz* szType);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  
    void vPrintCard(Ribbon* pRibbon, i8 iDepth, Board* pBoard);
    void vPrintDetails(Ribbon* pRibbon, i8 iDepth);
    void vPrintProperties(Ribbon* pRibbon, i8 iDepth, bool bPrintName=true);

    void vIndexPrint(Ribbon* pRibbon, i8 iDepth);
    void vOnPrint(Ribbon* pRibbon, i8 iDepth);
    void vPredicatePrint(Ribbon* pRibbon, i8 iDepth);
    void vProjectionPrint(Ribbon* pRibbon, i8 iDepth);
    void vScanPrint(Ribbon* pRibbon, i8 iDepth);
    void vSortPrint(Ribbon* pRibbon, i8 iDepth);
    void vStagePrint(Ribbon* pRibbon, i8 iDepth);
    void vStreamDefPrint(Ribbon* pRibbon, i8 iDepth);
    void vTablePrint(Ribbon* pRibbon, i8 iDepth);
    void vTermPrint(Ribbon* pRibbon, i8 iDepth);

  private:

    Stz* szDecodeEnum(Stz* szEnum, i8 iEnum);
  
  protected:
    Footer* aFooter(void);
  
  
  
  /*
  
    Card_Aggreg
    Card_AggregPred
    Card_AndPred
    Card_As

    Card_By
    Card_ByList
    Card_ByPred

    Card_Combine
    Card_ComparePred
    Card_CorrSubquery
    Card_CreateTable

    Card_Debug
    Card_DropTable

    Card_Exists
    Card_Expr

    Card_Filter

    Card_Hash
    Card_HashCast
    Card_Having

    Card_Index
    Card_IndexJoin
    Card_IndexPred
    Card_IndexScan

    Card_Limit

    Card_MainSubquery

    Card_NodeCast
    Card_NoncSubquery

    Card_On
    Card_OrdAgg
    Card_OriginCast
    Card_OrPred

    Card_PART
    Card_PRED
    Card_Predicate
    Card_Project
    Card_Projection
    Card_ProjectPred

    Card_Scan
    Card_Send
    Card_Sort
    Card_Stage
    Card_StreamDef

    Card_Table
    Card_Term
    Card_TermList
    Card_Type

    Card_UNIT

  */
  
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
