/*============================================================================*/
/* ReduceOp.hpp -- Reduce                                                     */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class ReduceOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;
  
  public:
    ReduceOp(Jobs::Job* pJob, Stz* szOpLabel);
   ~ReduceOp(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  ReduceOp* _pReduceOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
