/*============================================================================*/
/* StreamSet.hpp -- Stream Information Set                                    */
/*============================================================================*/
#pragma once

#include "InfoSetBase.hpp"

namespace STORE
{
  
  class StreamSet : public InfoSetBase
  {
  protected:
    
  public:
    void* operator new(size_t);
    void operator delete(void*);
  
    StreamSet(void);
    StreamSet(StreamDef* pStreamDef);
    ~StreamSet(void) noexcept(false);

  };

  StreamSet* _pStreamSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
