/*============================================================================*/
/* HashOp.hpp -- Hash                                                         */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class HashOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    HashOp(Jobs::Job* pJob, Stz* szOpLabel);
   ~HashOp(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  HashOp* _pHashOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
