/*============================================================================*/
/* Card_Table.hpp -- Table                                                    */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Table : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Table(void);
    Card_Table(Stz* szTableNm);
   ~Card_Table(void) noexcept(false);

    void vTablePrint(Ribbon* pRibbon, i8 iDepth);
  };

  Card_Table* _pCard_Table(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
