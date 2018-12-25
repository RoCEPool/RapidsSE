/*============================================================================*/
/* IoChunk.hpp -- I/O Chunk                                                   */
/*============================================================================*/
#pragma once

namespace Loader
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class IoChunk : public FileChunk
  {
  private:
  
    // must be last
    Footer m_xFooter;
  
  public: //$ functions:
    void* operator new(size_t);
    void  operator delete(void*);
    
    IoChunk(void);
    ~IoChunk(void) noexcept(false);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
        
  private:
    Footer* aFooter(void);
  };
  
  IoChunk* _pIoChunk(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
