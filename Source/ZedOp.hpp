/*============================================================================*/
/* ZedOp.hpp -- Zed                                                           */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class ZedOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    ZedOp(Stz* szOpLabel, Jobs::Job* pJob);
   ~ZedOp(void) noexcept(false);
  
    static Op* pNew(Job_Dynamic* pJob, TokenVec* pTknVec, i8& i_Tkn_);
    static eOR eCall(void* aThis);
  
    eOR eZedLoop(void);
  
  private:
    Footer* aFooter(void);
  };

  ZedOp* _pZedOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
