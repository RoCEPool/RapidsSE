/*============================================================================*/
/* CardRegs.hpp -- Card Registers                                             */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"
#include "Card_PredCmp.hpp"
#include "eCK.h"

namespace STORE
{
  class CardRegs : public Object
  {
  protected:
  
    Card* m_paCardReg[CK_ENUMS];

    Card_PredCmp*    m_pPredCmp_ebound;
    Card_PredCmp*    m_pPredCmp_lbound;
    Card_PredCmp*    m_pPredCmp_ubound;

    Index*     m_pIndex;
    Table*     m_pTable;
  
  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    CardRegs(void);
   ~CardRegs(void) noexcept(false);
  
    Card* pCardReg(eCK eCard);
    void  vCardReg(Card* pCard);
    void  vCardReg(eCK eCard, Card* pCard);
  
    bool  bInCardReg(Card* pCard);
    Index* pIndex(void);
    void vIndex(Index* pIndex);

    Card_PredCmp* pPredCmp_ebound(void);
    void vPredCmp_ebound(Card_PredCmp* pPredCmp);
    Card_PredCmp* pPredCmp_lbound(void);
    void vPredCmp_lbound(Card_PredCmp* pPredCmp);
    Card_PredCmp* pPredCmp_ubound(void);
    void vPredCmp_ubound(Card_PredCmp* pPredCmp);

    void  vResetCardRegs(void);

    Card* pSearchReg(eCK eCard);

    Table* pTable(void);
    void vTable(Table* pTable);
  
  protected:
    Footer* aFooter(void);
  };

  CardRegs* _pCardRegs(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
