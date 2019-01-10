/*============================================================================*/
/* LoadChunk.hpp -- Load Chunk                                                */
/*============================================================================*/
#pragma once

namespace Chunks
{  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class LoadChunk : public FileChunk
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    LoadChunk(Formatters* pFormatters);
    ~LoadChunk(void) noexcept(false);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
        
  private:
    Footer* aFooter(void);
  };
  
  LoadChunk* _pLoadChunk(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
