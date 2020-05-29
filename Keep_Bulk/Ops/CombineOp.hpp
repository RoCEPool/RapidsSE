/*============================================================================*/
/* CombineOp.hpp -- Combine                                                   */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class CombineOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    CombineOp(Stz* szOpLabel, Pipeline* pPipeline);
   ~CombineOp(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  CombineOp* _pCombineOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
