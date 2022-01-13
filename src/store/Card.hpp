/*============================================================================*/
/* Card.hpp -- Card of cards                                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Ribbon.hpp"
#include "ListExo2_t.hpp"
#include "eCK.h"

namespace STORE { class Board; class Card_Properties; }

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

    void (*m_fpDeleteThis)(Card* pCard);
    void (*m_fpBoardPrint)(Card* pCard, Ribbon* pRibbon, i8 iDepth);
    void (*m_fpCardHeader)(Card* pCard, Ribbon* pRibbon);
  
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card(eCK eKind, eCN eClass, Stz* szCardNm);
    Card(eCK eKind, eCN eClass, Stz* szCardNm,
         Stz* szProperty, i8 iValue);
    Card(eCK eKind, eCN eClass, Stz* szCardNm,
         Stz* szProperty, Object* pValue);
    Card(eCK eKind, eCN eClass, Stz* szCardNm,
         Stz* szProperty, Stz* szValue);
   ~Card(void) noexcept(false);

    void vInitialise(eCK eKind, eCN eClass, Stz* szCardNm);

    static void vCardHeader_(Card* pCard, Ribbon* pRibbon);
    static void vCardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);

    void vDelete(void);
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

  

    static void vIndexPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
    static void vOnPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
    static void vPredicatePrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
    static void vSortPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
    static void vStagePrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
    static void vStreamDefPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);


  private:
    static void vDeleteThis_(Card* pCard);
    Footer* aFooter(void);
  
  
  public:
  
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

   
      Card_MainSubquery
   
      Card_NodeCast
      Card_NoncSubquery
   
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
