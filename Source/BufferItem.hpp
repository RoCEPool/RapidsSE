/*============================================================================*/
/* BufferItem.hpp -- abstract buffer item                                     */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class BufferItem : public Object
  {
  protected:
    i8  m_iBytes;   // number of data bytes
    Id  m_xId;      // id
    i8  m_iOffsets; // no of offsets for this entry (0,1 or 2)

  public:
    BufferItem(eCN eClassNo);
    ~BufferItem(void);
  
  };
  
  BufferItem* _pBufferItem(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
