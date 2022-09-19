/*============================================================================*/
/* Booler.hpp -- transform <calc> into <calcBool>                             */
/*============================================================================*/
#pragma once

#include "TransformerTool.hpp"

namespace STORE
{
  class Booler : public TransformerTool
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Booler(Bench* pBench);
   ~Booler(void) noexcept(false);

    void vBool(void);

  private:

    void vBool(Card* pCard_Filter);

    Card* pTo_CalcBool(Card* pCard_Calc);

    Footer* aFooter(void);
  };

  Booler* _pBooler(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
