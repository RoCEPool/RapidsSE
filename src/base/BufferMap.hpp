/*============================================================================*/
/* BufferMap.hpp -- Buffer (to address) Map                                    */
/*============================================================================*/
#pragma once

namespace BASE
{
  class BufferMap : public Object
  {
  private:
   
    BufferAddress m_xaBufferAddress[1024];
  
    // must be last
    Footer m_xFooter;
    
  public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    BufferMap(void);
   ~BufferMap(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };
  
  BufferMap* _pBufferMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
