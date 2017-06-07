/*============================================================================*/
/* BufferItem.hpp -- abstract buffer item                                     */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class BufferItem : public Object
  {
  protected:
      i8  m_iBytes;  // number of data bytes (incl null teminator)

  public:
    BufferItem(eCN eClassNo);
    ~BufferItem(void);
  
  };
  
  BufferItem* _pBufferItem(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
