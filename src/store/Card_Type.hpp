/*============================================================================*/
/* Card_Type.hpp -- Type                                                      */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"
#include "eCT.h"


namespace STORE
{
  class Card_Type : public Card
  {  
  protected:
    eCT m_eColumnType;    // column type (CT_INTEGER etc)

  private:
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Type(eCT eColumnType);
   ~Card_Type(void) noexcept(false);
    
    eCT  eColumnType(void);
    void vColumnType(eCT eColumnType);

    void vPrintType(Ribbon* pRibbon);
    
  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
  
    Footer* aFooter(void);
  };

  Card_Type* _pCard_Type(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
