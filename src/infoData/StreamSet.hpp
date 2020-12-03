/*============================================================================*/
/* StreamSet.hpp -- Stream Information Set                                    */
/*============================================================================*/
#pragma once

namespace InfoData
{
  
  class StreamSet : public InfoSetBase
  {
  protected:
    
  public:
    void* operator new(size_t);
    void operator delete(void*);
  
    StreamSet(Socket*);
    StreamSet(StreamDef* pStreamDef);
    ~StreamSet(void) noexcept(false);

  };

  StreamSet* _pStreamSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
