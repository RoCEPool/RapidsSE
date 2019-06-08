/*============================================================================*/
/* KeyWrk.hpp -- Key Working Ent                                            */
/*============================================================================*/
#pragma once

namespace Data
{
  class EntBuffer;
  class Key;
  class Row;
  
  class KeyWrk : public EntWrk
  {
  private:
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, KeyWrk*);
    
    KeyWrk(void);
    ~KeyWrk(void) noexcept(false);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
