/*============================================================================*/
/* FetchOp.hpp -- Fetch                                                       */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class FetchOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    FetchOp(void);
   ~FetchOp(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  FetchOp* _pFetchOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
