/*============================================================================*/
/* Card_StreamPtn.hpp -- Stream Partition                                     */
/*============================================================================*/
#pragma once

#include "Card_t.hpp"

namespace STORE
{
  class Card_StreamPtn : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_StreamPtn(void);
   ~Card_StreamPtn(void) noexcept(false);

    void vAddKey(Symbol* p_Y_tblCol);
    bool bContainsKey(Symbol* p_Y_tblCol);
    bool bIsEmpty(void);
    void vResetKeys(void);
  };

  Card_StreamPtn* _pCard_StreamPtn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
