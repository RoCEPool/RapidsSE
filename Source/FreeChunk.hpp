/*============================================================================*/
/* FreeChunk.hpp - free chunk in heap                                         */
/*============================================================================*/
#pragma once

namespace Pool
{
  class FreeChunk : public Header
  {
  private:
    FreeChunk* m_pNextFree; // next free chunk in chain
    eCN        m_eWasClass; // what was the class
  
  public:
    void* operator new(size_t, FreeChunk* pFreeChunk);
    void  operator delete(void*);

    FreeChunk(void);
    ~FreeChunk(void) noexcept(false);
    void vInit(eCN eWasClass);
    eCN eWasClass(void);
    void vWasClass(eCN eWasClass);

    FreeChunk* pNextFree(void);
    void       vNextFree(FreeChunk* pNextFree);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
