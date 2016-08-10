/*============================================================================*/
/* FreeChunk.hpp - free chunk in heap                                         */
/*============================================================================*/

#pragma once

#include "MemoryManagement.h"

namespace MemoryManagement
{
  class FreeChunk : public Object
  {
  private:
    FreeChunk* m_pNextFree;  // next free chunk in chain
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    FreeChunk(void);
    ~FreeChunk(void);
    void vInit(eCN eClass);
    
    FreeChunk* pNextFree(void);
    void       vNextFree(FreeChunk* pNextFree);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
