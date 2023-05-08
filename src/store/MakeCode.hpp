/*============================================================================*/
/* MakeCode.hpp -- Call Synthesiser                                           */
/*============================================================================*/
#pragma once

#include "Bench.hpp"
#include "CardIter.hpp"


namespace STORE
{
  class MakeCode : public Object
  {
  private:

    CardIter* m_pCI;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    MakeCode(CardIter* pCI);
   ~MakeCode(void) noexcept(false);

    void vSynthesise(void);

  protected:
    Footer* aFooter(void);
  };

  MakeCode* _pMakeCode(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
