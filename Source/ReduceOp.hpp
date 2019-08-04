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
    ReduceOp(Stz* szOpLabel, Digest* pDigest);
   ~ReduceOp(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  ReduceOp* _pReduceOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
