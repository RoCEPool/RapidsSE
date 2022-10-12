/*============================================================================*/
/* Card_ELIST.hpp -- Abstract Expression List                                 */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "Card_StreamColList.hpp"

namespace STORE
{
  class Card_ELIST : public Card
  {  
  public:
    Card_ELIST(eCK eCard);
   ~Card_ELIST(void) noexcept(false);

   Card_StreamColList* pDerive_StreamColList(void);
  };

  Card_ELIST* _pCard_ELIST(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
