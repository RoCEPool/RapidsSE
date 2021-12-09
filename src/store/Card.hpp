/*============================================================================*/
/* Card.hpp -- Card of cards                                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Ribbon.hpp"
#include "ListExo2_t.hpp"
#include "eCK.h"

namespace STORE { class Card_Properties; }

namespace STORE
{
  class Card : public Object
             , public ListExo2_t<Card>
  {
  protected:
    i8    m_iCardNo;
    s32   m_sCardNm;
    eCK   m_eKind;
    s256  m_sText;
  
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
  
    Card(eCK eKind, eCN eClass, Stz* szCardNm, Stz* szText=""_Stz);
   ~Card(void) noexcept(false);
      
    static void vCardHeader_(Card* pCard, Ribbon* pRibbon);

    void vDelete(void);
    void vBoardPrint(Ribbon* pRibbon, i8 iDepth);
    void vCardHeader(Ribbon* pRibbon, i8 i_Column);
  
    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    
    void vAddProperty(Stz* szName);
    void vAddProperty(Stz* szName, Stz* szValue);

    Stz* szCardNm(void);

    i8   iCardNo(void);
    void vCardNo(i8 iCardNo);

    void vCutProperty(Stz* szName);
  
    void vEachSubCard(std::function<void(Card* pCard)> f_vClosure,
                      eCN eClassNo=CN_NONE);
  
    eCK   eKind(void);
    Card* pFindSubCard(eCN eClassNo);

    bool bHasProperty(Stz* szName);
  
    i8   iOfText(void);
    i8   nOfText(void);

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



  private:
    Footer* aFooter(void);
  };

  Card* _pCard(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
