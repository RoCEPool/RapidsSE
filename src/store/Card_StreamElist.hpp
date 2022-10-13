/*============================================================================*/
/* Card_StreamElist.hpp -- List of Stream Expressions                         */
/*============================================================================*/
#pragma once

#include "Card_ELIST.hpp"

namespace STORE
{
  class Card_StreamElist : public Card_ELIST
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_StreamElist(void);
    Card_StreamElist(StreamDef* pStreamDef);
   ~Card_StreamElist(void) noexcept(false);
    
    bool bSimpleDef(void);
    void vStreamElistPrint(Ribbon* pRibbon, i8 iDepth, Card* pParent);

  private:
    void vPopulate(StreamDef* pStreamDef);
  };

  Card_StreamElist* _pCard_StreamElist(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
