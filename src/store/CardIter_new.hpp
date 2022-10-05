/*============================================================================*/
/* CardIter_new.hpp --                                                        */
/*============================================================================*/
#pragma once

#include "TransformerTool.hpp"

namespace STORE
{
  class TransformerTool;

  class CardIter_new : public Object
  {
  private:
    Card*  m_pCard;
    i8     m_i_Child;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    CardIter_new(Card* pCard);
   ~CardIter_new(void) noexcept(false);


    Card* pEachChild(void);
    Card* pEachChild(i8& ri_Child);
    Card* pEachChild(eCK eCard);
    Card* pEachChild(eCK eCard, i8& ri_Main);
    Card* pEachChild(i8& ri_Main, eCK& reCard);
  private:

    Footer* aFooter(void);
  };

  CardIter_new* _pCardIter_new(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
