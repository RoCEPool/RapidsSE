/*============================================================================*/
/* OutputOp.hpp -- Output                                                     */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class OutputOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    OutputOp(Jobs::Job* pJob, Stz* szOpLabel);
   ~OutputOp(void) noexcept(false);
  
    static bool bCall(void* aThis);
    bool bOutput(void);

  private:
    Footer* aFooter(void);
  };

  OutputOp* _pOutputOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
