/*============================================================================*/
/* Card.hpp -- Card of cards                                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Ribbon.hpp"
#include "ListExo2_t.hpp"

namespace STORE
{
  class Card : public Object
             , public ListExo2_t<Card>
  {
  protected:
    s32  m_sCardNm;
  
    i8   m_iSubNo;
  
    bool m_bPrintNm;

    void (*m_fpDeleteThis)(Card* pCard);
    void (*m_fpBoardPrint)(Card* pCard, Ribbon* pRibbon, i8 iDepth);
    void (*m_fpCardHeader)(Card* pCard, Ribbon* pRibbon);
  
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card(Stz* m_sCardNm);
    Card(eCN eClass, Stz* m_sCardNm);
   ~Card(void) noexcept(false);
      
    static void vCardHeader_(Card* pCard, Ribbon* pRibbon);

    void vDelete(void);
    void vBoardPrint(Ribbon* pRibbon, i8 iDepth);
    void vCardHeader(Ribbon* pRibbon, i8 i_Column);
  
    Stz* szCardNm(void);
  
    void vEachSubCard(std::function<void(Card* pCard)> vfClosure,
                      eCN eClassNo=CN_NONE);
  
    Card* pFindSubCard(eCN eClassNo);

    void vRenumberSubCards(void);

    i8   iSubNo(void);
    void vSubNo(i8 iSubNo);

    void vTransferAll(Card* pCards);
    void vTransferFrom(Card* pCards, Card* pFrom,
                       Card* pAt=nullptr, bool bAfter=true);
    void vTransferSubset(Card* pCards, Card* pFrom, Card* pTill,
                         Card* pAt=nullptr, bool bAfter=true);
    void vTransferTill(Card* pCards, Card* pTill,
                       Card* pAt=nullptr, bool bAfter=true);
  
    void vWalkSubCards(std::function<void(Card* pCard)> vfClosure);



  private:
    Footer* aFooter(void);
  };

  Card* _pCard(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
