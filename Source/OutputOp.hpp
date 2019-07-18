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
  
    OutputOp(void);
   ~OutputOp(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  OutputOp* _pOutputOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
