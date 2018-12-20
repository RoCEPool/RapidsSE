/*============================================================================*/
/* IoChunk.hpp -- I/O Chunk                                                   */
/*============================================================================*/
#pragma once

namespace Loader
{
  static const i8 C_IO_CHUNK = C_1M; // its 1M not 1MB (i.e. < 1MB)
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class IoChunk : public Object
  {
  private:
    File m_xFile;        // file being used
  
    i8   m_nBytesRead;   // bytes read
    Stz* m_aEndChunk;    // last byte of where chunk ends
  
    WspFxd<C_IO_CHUNK> m_xWsp; // workspace
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    IoChunk(void);
    ~IoChunk(void) noexcept(false);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void  vCloseFile(void);
    void  vOpenFile(s256& sPath);
    bool  bReadChunk(i8 nBytes);
    
  private:
  
    Footer* aFooter(void);
  };
  
  IoChunk* _pIoChunk(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
