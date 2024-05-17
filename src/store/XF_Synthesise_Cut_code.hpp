/*============================================================================*/
/* XF_Synthesise_Cut_code.hpp -- Cut Code                                     */
/*============================================================================*/
#pragma once

#include "XF_SYNTHESISE.hpp"

#include "Bench.hpp"
#include "CardIter.hpp"

#include "PipelineDef.hpp"

namespace STORE
{
  class XF_Synthesise_Cut_code : public XF_SYNTHESISE
  {
  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    XF_Synthesise_Cut_code(CardIter* pCI);
   ~XF_Synthesise_Cut_code(void) noexcept(false);

    static void vCut(CardIter* pCI);

    void vCut(void);

  protected:
    Footer* aFooter(void);
  };

  XF_Synthesise_Cut_code* _pXF_Synthesise_Cut_code(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
