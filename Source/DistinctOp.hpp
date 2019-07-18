/*============================================================================*/
/* DistinctOp.hpp -- Distinct                                                 */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class DistinctOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    DistinctOp(Jobs::Job* pJob, Stz* szOpLabel);
   ~DistinctOp(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  DistinctOp* _pDistinctOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
