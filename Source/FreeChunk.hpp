/*============================================================================*/
/* FreeChunk.hpp - free chunk in heap                                         */
/*============================================================================*/
#pragma once

namespace Pool
{
  class FreeChunk : public Object
  {
  private:
    FreeChunk* m_pNextFree; // next free chunk in chain
    eCN        m_eWasClass; // what was the class
    
    // must be last
    //Footer m_xFooter;
    
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
    
    //Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
