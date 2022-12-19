/*============================================================================*/
/* Card_Stream.hpp -- Stream                                                  */
/*============================================================================*/
#pragma once

#include "Card_PRED.hpp"
#include "Card_StmColList.hpp"

namespace STORE
{
  class Card_Stream : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Stream(void);
   ~Card_Stream(void) noexcept(false);

    static Card_Stream* pDerive_Of_aggreg(Card_AggElist* p_aggElist);
    static Card_Stream* pDerive_Of_project(Card_ProjElist* p_projElist);
    static Card_Stream* pDerive_Of_table(Table* pTable);
  };

  Card_Stream* _pCard_Stream(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
