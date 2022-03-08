/*============================================================================*/
/* CardRegs.hpp -- Card Registers                                             */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"
#include "eCK.h"

namespace STORE
{
  class CardRegs : public Object
  {
  protected:
    Card* m_paCardReg[CK_ENUMS];

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
  
    void  vClearCardReg(eCK eCard);
    void  vClearCardRegs(void);

    bool  bInCardReg(Card* pCard);
    Index* pIndex(void);
    void vIndex(Index* pIndex);

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
