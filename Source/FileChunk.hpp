/*============================================================================*/
/* FileChunk.hpp -- File Chunk                                                */
/*============================================================================*/
#pragma once

namespace Chunks
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class FileChunk : public DataChunk
  {
  protected:
    File m_oFile; // file being used
  
  public: //$ functions:
    void* operator new(size_t);
    void  operator delete(void*);
    
    FileChunk(eCN eClassNo);
    ~FileChunk(void) noexcept(false);
  
    void  vCloseFile(void);
    void  vOpenRead(s256& sPath);
    void  vOpenWrite(s256& sPath);
    i8    nReadBytes(i8 nBytes);
    void  vRewindFile(void);
    i8    nWriteBytes(i8 nBytes);
    void  vWriteData(void);
    Stz*  aStr(void);
  };
  
  FileChunk* _pFileChunk(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
