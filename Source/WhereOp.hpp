/*============================================================================*/
/* WhereOp.hpp -- Where                                                       */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class WhereOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    WhereOp(Jobs::Job* pJob);
   ~WhereOp(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  WhereOp* _pWhereOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
