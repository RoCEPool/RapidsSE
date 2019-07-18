/*============================================================================*/
/* MergeOp.hpp -- Merge                                                       */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class MergeOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    MergeOp(Jobs::Job* pJob, Stz* szOpLabel);
   ~MergeOp(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  MergeOp* _pMergeOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
