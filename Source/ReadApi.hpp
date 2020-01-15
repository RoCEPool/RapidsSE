/*============================================================================*/
/* ReadApi.hpp -- Read Consistency API                                        */
/*============================================================================*/
#pragma once

namespace CompactBuffers
{
  class ReadApi : public Object
  {
  private:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ReadApi(void);
   ~ReadApi(void) noexcept(false);
  

  private:
    Footer* aFooter(void);
  };
  
  ReadApi* _pReadApi(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
