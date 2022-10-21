/*============================================================================*/
/* Card_Partition.hpp -- Partition (of table)                                 */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Partition : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Partition(void);
   ~Card_Partition(void) noexcept(false);

    char cTypeShort(void);
  };

  Card_Partition* _pCard_Partition(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
