/*============================================================================*/
/* BufferItem.hpp -- abstract buffer item                                     */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class EntWsp;
  
  class BufferItem : public Object
  {
  protected:
    i8  m_iBytes;   // number of data bytes
    Id  m_xId;      // id
    i8  m_iOffsets; // no of offsets for this entry (0,1 or 2)

  public:
    BufferItem(eCN eClassNo);
    ~BufferItem(void);
  
    static i8 nBytesRequiredEntry(EntWsp& xEntWsp);
    static i8 nBytesRequiredEntry(i8 nOffsets, i8 nKeyBytes);
  
    static i8 nBytesRequiredRow(i8 iStrLength);
  };
  
  BufferItem* _pBufferItem(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
