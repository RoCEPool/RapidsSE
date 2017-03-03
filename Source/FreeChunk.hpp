/*============================================================================*/
/* FreeChunk.hpp - free chunk in heap                                         */
/*============================================================================*/
#pragma once

namespace Pool
{
  class FreeChunk : public Object
  {
  private:
    FreeChunk* m_pNextFree;  // next free chunk in chain
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t, FreeChunk* pFreeChunk);
    void  operator delete(void*);

    FreeChunk(void);
    ~FreeChunk(void);
    void vInit(eCN eClass);
    
    FreeChunk* pNextFree(void);
    void       vNextFree(FreeChunk* pNextFree);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
