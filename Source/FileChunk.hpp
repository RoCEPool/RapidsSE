/*============================================================================*/
/* FileChunk.hpp -- File Chunk                                                */
/*============================================================================*/
#pragma once

namespace Chunks
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class FileChunk : public RecordExtractor
  {
  protected:
    File m_oFile; // file being used
  
  public: //$ functions:
    void* operator new(size_t);
    void  operator delete(void*);
    
    FileChunk(eCN eClassNo, Formatters* pFormatters);
    ~FileChunk(void) noexcept(false);
  
    void  vCloseFile(void);
    bool  bOpenRead(s256& sPath);
    bool  bOpenWrite(s256& sPath);
    i8    nReadBytes(i8 nBytes);
    void  vReadSpecificChunk(i8 iSeek, i8 nBytes);
    void  vRewindFile(void);
    i8    nWriteBytes(i8 nBytes);
    void  vWriteData(void);
  };
  
  FileChunk* _pFileChunk(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
