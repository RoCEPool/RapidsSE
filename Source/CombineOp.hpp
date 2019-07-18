/*============================================================================*/
/* CombineOp.hpp -- Combine                                                   */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class CombineOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    CombineOp(Jobs::Job* pJob);
   ~CombineOp(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  CombineOp* _pCombineOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
