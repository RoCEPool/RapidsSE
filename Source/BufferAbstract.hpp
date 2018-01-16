/*============================================================================*/
/* BufferAbstract.hpp -- Abstract Buffer                                      */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class BufferAbstract : public Object
  {
  protected:
  
  public:
    BufferAbstract(eCN eClassNo);
   ~BufferAbstract(void);
  };
  
  BufferAbstract* _pBufferAbstract(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
