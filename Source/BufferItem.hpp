/*============================================================================*/
/* BufferItem.hpp -- abstract buffer item                                     */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class BufferItem : public Object
  {
  private:

  public:
    BufferItem(eCN eClassNo);
    ~BufferItem(void);
  
  };
  
  BufferItem* _pBufferItem(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
