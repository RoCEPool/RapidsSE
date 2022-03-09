/*============================================================================*/
/* Card.hpp -- Abstract Card                                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "eAL.h"
#include "eBK.h"
#include "eCK.h"
#include "eGC.h"
#include "eRO.h"
#include "eTE.h"
#include "eYG.h"
#include "eXK.h"

#include "Table.hpp"

#include "Ribbon.hpp"
#include "ListExo2_t.hpp"


namespace STORE { class Board;
                  class Card; class Card_Properties;
                  class Card_SubSet; class Card_Term; }

namespace STORE
{
  class CardSet : public ListExo2_t<Card>
  {
  public:
    CardSet(void);
   ~CardSet(void) noexcept(false);

    Card* pGetSubCard(i8 i_SubCard);
    Card* pGetSubCard(i8 i_SubCard, eCK& reCard);

    void vRemoveSubCards(eCK eCard);
  };



  class Card : public Object
             , public CardSet // ListExo2_t<Card>
  {
  protected:
    eCK  m_eCard;
    eGC  m_eCatg;
    i8   m_iCardNo;
  
    char m_cMark1;
    char m_cMark2;

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

    static bool bIsMain(eCK eCard);
    static bool bIsOther(eCK eCard);
    static bool bIsSub(eCK eCard);
    static eCK  eParentECard(eCK eChildCard);
    static eCK  eToECard(Stz* szCardNm);
    static eGC  eToECatg(eCK eCard);
    static eGC  eToECatg(Stz* szCardNm);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    
    void vAddProperty(Stz* szName);
    void vAddProperty(Stz* szName, i8 iValue);
    void vAddProperty(Stz* szName, Object* pValue);
    void vAddProperty(Stz* szName, Stz* szValue);

    eCK  eCard(void);
    eGC  eCatg(void);
    Stz* szCardNm(void);
    i8   iCardNo(void);
    void vCardNo(i8 iCardNo);
    void vCutProperty(Stz* szName);
    void vCutSubCard(Card* pSubCard);

    void vDump(i8 iDepth=0);
    Card* pDupCardAndSubCards(void);
    Card_SubSet* pDupSubCards(void);

    void vEachSubCard(std::function<void(Card* pCard)> f_vClosure,
                      eCK eCard=CK_NONE);
    bool bEquivalent(Card* pCard_arg);

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
    bool bHasUBound(void);
    void vHasUBound(bool bHasUBound);

    bool bIsMain(void);
    bool bIsOther(void);
    bool bIsSub(void);

    char cMark1(void);
    void vMark1(char cMark1);
    char cMark2(void);
    void vMark2(char cMark2);

    i8   nProperties(void);
    eTN  eProperty(i8 i_Property);
    TagSet* pProperties(void);
    void vPropertiesEnsure(void);
    Stz* szPropertyNm(i8 i_Property);

    void vRemoveSubCard(Card* pCardRemove);
    void vRenumberCards(void);
 
    i8   nSubCards(void);
  
    void vTransferAll(Card* pCards);
    void vTransferFrom(Card* pCards, Card* pFrom,
                       Card* pAt=nullptr, bool bAfter=true);
    void vTransferSubset(Card* pCards, Card* pFrom, Card* pTill,
                         Card* pAt=nullptr, bool bAfter=true);
    void vTransferTill(Card* pCards, Card* pTill,
                       Card* pAt=nullptr, bool bAfter=true);
  

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  
    eAL  eAggLevel(void);
    void vAggLevel(eAL eAggLevel);
    bool bAggOp(void);
    void vAggOp(void);
    Stz* szAs(void);
    void vAs(Stz* szAs);
  
    eBK  eBound(void);
  
    Column* pColumn(void);
    void vColumn(Column* pColumn);
    Column* pColumn1(void);
    void vColumn1(Column* pColumn1);
    Column* pColumn2(void);
    void vColumn2(Column* pColumn2);

    Stz* szDirn(void);
    void vDirn(Stz* szAsNm);

    i8   iGenerate(void);
    void vGenerate(i8 iGenerate);
  
    bool bHasAggOp(void);
    Card_Term* pHeadTerm(void);

    Index* pIndex(void);
    void vIndex(Index* pIndex);
    bool bIsAggOp(void);
    bool bIsCard(eCK eCard);
    bool bIsOrdinalColumnAtom(void);
    bool bIsOrdinalIndexAtom(void);
    bool bIsOrdinalStreamTerm(void);
    bool bIsOrdinalIndexTerm(void);
    bool bIs_MAIN(void);
    bool bIsPred(void);
    bool bIsSame(Stz* szArg);
    bool bIsSeparator(void);
    bool bIs_SUB(void);

    Stz* szLevel(void);
    void vLevel(Stz* szLevel);

    Stz* szMode(void);
    void vMode(Stz* szMode);
  
    Stz* szName(void);
    void vName(Stz* szMode);

    i8   iOption(void);
    void vOption(i8 iOption);
    Stz* szOption(void);
    void vOption(Stz* szOption);
    i8   iOrdinalStreamColNo(void);
    i8   iOrdinalIndexColNo(void);

    Card_Term* pPopTerm(void);
  
    Stz* szQueryNm(void);
    void vQueryNm(Stz* szQueryNm);

    i8   iRank(void);
    void vRank(i8 iRank);
    eRO  eRelOp(void);
    i8   nRows(void);
    void vRows(i8 nRows);
  
    eXK  eSendKind(void);
    void vSendKind(eXK eSendKind);
    i8   iSends(void);
    void vSends(i8 iSends);
    Stz* szStageNm(void);
    void vStageNm(Stz* szStageNm);
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
    Stz* szTerm(void);
    void vTerm(Stz* szTerm);
    eTE  eTermEval(void);
    Stz* szType(void);
    char cTypeShort(void);
    void vType(Stz* szType);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  
    void vPrintCard(Ribbon* pRibbon, i8 iDepth, i8 iSee);
    void vPrintDetails(Ribbon* pRibbon, i8 iDepth, bool& bPrune);
    void vPrintHeader(Ribbon* pRibbon);
    void vPrintName(Ribbon* pRibbon, i8 iDepth);
    void vPrintProperties(Ribbon* pRibbon, i8 iDepth, bool bPrintName=true);

    void vIndexPrint(Ribbon* pRibbon, i8 iDepth);
    void vStreamElistPrint(Ribbon* pRibbon, i8 iDepth, bool& bPrune);
    void vTablePrint(Ribbon* pRibbon, i8 iDepth);
    void vTermPrint(Ribbon* pRibbon, i8 iDepth);

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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
