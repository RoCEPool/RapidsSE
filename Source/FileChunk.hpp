/*============================================================================*/
/* FileChunk.hpp -- File Chunk                                                */
/*============================================================================*/
#pragma once

namespace Loader
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class FileChunk : public DataChunk
  {
  private:
    File m_oFile; // file being used
  
    // must be last
    Footer m_xFooter;
  
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
    
  private:
    Footer* aFooter(void);
  };
  
  FileChunk* _pFileChunk(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
