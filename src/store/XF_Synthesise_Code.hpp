/*============================================================================*/
/* XF_Synthesise_Code.hpp -- Call Synthesiser                                           */
/*============================================================================*/
#pragma once

#include "XF_SYNTHESISE.hpp"

#include "Bench.hpp"
#include "CardIter.hpp"

#include "PipelineDef.hpp"

namespace STORE
{
  class XF_Synthesise_Code : public XF_SYNTHESISE
  {
  private:

    CardIter*    m_pCI;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    XF_Synthesise_Code(CardIter* pCI);
   ~XF_Synthesise_Code(void) noexcept(false);

    static void vXF_Make_Code(CardIter* pCI);

    void vSynthesise(void);

  protected:
    Footer* aFooter(void);
  };

  XF_Synthesise_Code* _pXF_Synthesise_Code(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
