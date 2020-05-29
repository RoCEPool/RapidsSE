/*============================================================================*/
/* ReduceOp.hpp -- Reduce                                                     */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class ReduceOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;
  
  public:
    ReduceOp(Stz* szOpLabel, Pipeline* pPipeline);
   ~ReduceOp(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  ReduceOp* _pReduceOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
