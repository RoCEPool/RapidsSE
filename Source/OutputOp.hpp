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
  
    OutputOp(Stz* szOpLabel, Jobs::Job* pJob);
   ~OutputOp(void) noexcept(false);
  
    static eOR eCallLoop(void* aThis);
    eOR eOutputLoop(void);

  private:
    Footer* aFooter(void);
  };

  OutputOp* _pOutputOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
