/*============================================================================*/
/* DistinctOp.hpp -- Distinct                                                 */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class DistinctOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    DistinctOp(Stz* szOpLabel, Pipeline* pPipeline);
   ~DistinctOp(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  DistinctOp* _pDistinctOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
