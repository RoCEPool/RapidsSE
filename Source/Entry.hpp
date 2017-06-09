/*============================================================================*/
/* Entry.hpp -- index entry (skip list variant)                               */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class IndexBuffer;
  
  class Entry : public BufferItem
  {
  private:
    // ..data..        // right offsets & key
    // ..footer..
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, Entry*);
    
    ~Entry(void);
      
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
