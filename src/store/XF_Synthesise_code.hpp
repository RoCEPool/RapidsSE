/*============================================================================*/
/* XF_Synthesise_code.hpp -- Call Synthesiser                                           */
/*============================================================================*/
#pragma once

#include "XF_SYNTHESISE.hpp"

#include "Bench.hpp"
#include "CardIter.hpp"

#include "PipelineDef.hpp"

namespace STORE
{
  class XF_Synthesise_code : public XF_SYNTHESISE
  {
  private:

    CardIter*    m_pCI;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    XF_Synthesise_code(CardIter* pCI);
   ~XF_Synthesise_code(void) noexcept(false);

    static void vSynthesise(CardIter* pCI);

    void vSynthesise(void);

  protected:
    Footer* aFooter(void);
  };

  XF_Synthesise_code* _pXF_Synthesise_code(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
