/*============================================================================*/
/* Card_Project.hpp -- Project                                                */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_MAIN.hpp"

namespace STORE
{
  class Card_Project : public Card_MAIN
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Project(eCK eCard);
   ~Card_Project(void) noexcept(false);
    
    StreamDef* pProduceStreamDef(void);

  };

  Card_Project* _pCard_Project(void* aAddr);
  bool _bIsCard_Project(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
