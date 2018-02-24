/*============================================================================*/
/* Wilderness.hpp -- wilderness marker                                        */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Wilderness : public Item
  {

  public:
    void* operator new(size_t);
    void* operator new(size_t, Wilderness*);
  
    Wilderness(void);
    ~Wilderness(void);
  
    void vAssertWilderness(void);
  };
  
  Wilderness* _pWilderness(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
