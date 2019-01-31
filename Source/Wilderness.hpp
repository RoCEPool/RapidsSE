/*============================================================================*/
/* Wilderness.hpp -- wilderness marker                                        */
/*============================================================================*/
#pragma once

namespace MiniHeaps
{
  class Wilderness : public Item
  {

  public:
    void* operator new(size_t);
    void* operator new(size_t, Wilderness*);
  
    Wilderness(void);
    ~Wilderness(void) noexcept(false);
  
    void vAssertWilderness(void);
    bool bValidWilderness(void);
  };
  
  Wilderness* _aWilderness(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
