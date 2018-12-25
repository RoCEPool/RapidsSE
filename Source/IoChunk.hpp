/*============================================================================*/
/* IoChunk.hpp -- I/O Chunk                                                   */
/*============================================================================*/
#pragma once

namespace Loader
{
  static const i8 C_IO_CHUNK = C_1M; // its 1M not 1MB (i.e. < 1MB)
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class IoChunk : public DataChunk
  {
  private:
    File m_oFile; // file being used
  
    // must be last
    Footer m_xFooter;
  
  public: //$ functions:
    void* operator new(size_t);
    void  operator delete(void*);
    
    IoChunk(void);
    ~IoChunk(void) noexcept(false);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void  vCloseFile(void);
    void  vOpenRead(s256& sPath);
    void  vOpenWrite(s256& sPath);
    i8    nReadBytes(i8 nBytes);
    i8    nWriteBytes(i8 nBytes);
    void  vWriteData(void);
    Stz*  aStr(void);
    
  private:
    Footer* aFooter(void);
  };
  
  IoChunk* _pIoChunk(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
