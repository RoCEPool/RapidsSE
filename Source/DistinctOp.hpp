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
    DistinctOp(Stz* szOpLabel, Digest* pDigest, Jobs::Job* pJob);
   ~DistinctOp(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  DistinctOp* _pDistinctOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
