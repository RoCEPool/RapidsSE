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
  
    static bool bCallLoop(void* aThis);
    bool bZedLoop(void);
  
  private:
    Footer* aFooter(void);
  };

  ZedOp* _pZedOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
