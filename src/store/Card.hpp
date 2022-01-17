/*============================================================================*/
/* Card.hpp -- Card of cards                                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Ribbon.hpp"
#include "ListExo2_t.hpp"

#include "eAL.h"
#include "eCK.h"
#include "eYG.h"
#include "eXK.h"


namespace STORE { class Board;
                  class Card_Properties; class Card_Term; }

namespace STORE
{
  class Card : public Object
             , public ListExo2_t<Card>
  {
  protected:
    i8    m_iCardNo;
  
    eCK   m_eCard;
    s256  m_sName;
    s256  m_sText;

    s32   m_sCardNm;

    TagSet* m_pProperties;

    bool  m_bPrintNm;

    void (*m_fpBoardPrint)(Card* pCard, Ribbon* pRibbon, i8 iDepth);
    void (*m_fpCardHeader)(Card* pCard, Ribbon* pRibbon);
  
  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card(eCK eKind);
   ~Card(void) noexcept(false);

    void vInitialise(eCK eKind);

    static void vCardHeader_(Card* pCard, Ribbon* pRibbon);
    static void vCardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);

    void vBoardPrint(Ribbon* pRibbon, i8 iDepth, Board* pBoard);
    void vCardHeader(Ribbon* pRibbon, i8 i_Column);


    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    
    void vAddProperty(Stz* szName);
    void vAddProperty(Stz* szName, i8 iValue);
    void vAddProperty(Stz* szName, Object* pValue);
    void vAddProperty(Stz* szName, Stz* szValue);

    eCK  eCard(void);
    Stz* szCardNm(void);
    i8   iCardNo(void);
    void vCardNo(i8 iCardNo);
    void vCutProperty(Stz* szName);
  
    void vEachSubCard(std::function<void(Card* pCard)> f_vClosure,
                      eCN eClassNo=CN_NONE);
  
    Card* pFindSubCard(eCK eKind);
    Card* pFindSubCard(eCN eClassNo);

    i8      iGetProperty(i8 i_Property);
    i8      iGetProperty(Stz* szName);
    Object* pGetProperty(i8 i_Property);
    Object* pGetProperty(Stz* szName);
    Stz*    szGetProperty(i8 i_Property);
    Stz*    szGetProperty(Stz* szName);

    bool bHasProperty(Stz* szName);
  
    i8   iOfText(void);
    i8   nOfText(void);

    i8   nProperties(void);
    eTN  eProperty(i8 i_Property);
    Stz* szPropertyNm(i8 i_Property);

    void vRenumberSubCards(void);

    Stz* sz(void);
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
  
    static void vIndexPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
    static void vOnPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
    static void vPredicatePrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
    static void vSortPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
    static void vStagePrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
    static void vStreamDefPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);


  private:
    static void vDeleteThis_(Card* pCard);
  
  protected:
    Footer* aFooter(void);
  
  
  
  /*
  
      Card_Aggreg
      Card_AggregPred
      Card_AndPred
      Card_As
      Card_Atom
    
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
   
      Card_Part
      Card_Pred
      Card_Predicate
      Card_Project
      Card_Projection
      Card_ProjectPred
      Card_Properties
      Card_Property
   
      Card_Scan
      Card_Send
      Card_Sort
      Card_Stage
      Card_StreamDef
   
      Card_Table
      Card_Term
      Card_TermList
      Card_Type
   
      Card_Unit

   
  */
  
  
  // x void Card_Aggreg::vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
  // x void Card_As::vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
  // x void Card_Atom::vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
  // c void Card_Index::vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
  // x void Card_Limit::vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
  // c void Card_On::vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
  // x void Card_OrdAgg::vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
  // c void Card_Predicate::vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
  // x void Card_Property::vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
  // x void Card_Send::vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
  // c void Card_Sort::vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
  // c void Card_Stage::vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
  // c void Card_StreamDef::vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
  // x void Card_Term::vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)
  // x void Card_Type::vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth)

  //   Card_By
  //   Card_CorrSubquery 1
  //   Card_Debug 1
  //   Card_Index m_pIndex
  //   Card_IndexJoin m_pIndex
  //   Card_IndexScan m_pIndex
  //   Card_MainSubquery m_sQueryNm
  //   Card_NoncSubquery m_sQueryNm
  //   Card_On
  //   Card_Send m_eSendKind
  //   Card_Sort
  //   Card_Stage
  //   Card_StreamDef m_pStreamDef

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
