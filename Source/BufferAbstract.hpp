/*============================================================================*/
/* BufferAbstract.hpp -- Abstract Buffer                                      */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class BufferAbstract : public Object
  {
  protected:
  i8    m_nEntries;    // number of entries in this buffer

  WspFxd<C_SIZE_LARGE_OBJECT - C_1KB*4> m_xWsp;
  
  public:
    BufferAbstract(eCN eClassNo);
   ~BufferAbstract(void);
  
    Entry* pAllotEntryBytes(i8 nBytes);
    i8     nFreeBytes(void);
  
  };
  
  BufferAbstract* _pBufferAbstract(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
