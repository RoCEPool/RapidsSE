/*============================================================================*/
/* LoadChunk.hpp -- File Load Buffer                                         */
/*============================================================================*/
#pragma once

namespace Structures
{
  static const i8 C_LOADCHUNK_LENGTH = C_1M; // its 1M not 1MB (i.e. < 1MB)
  
  class LoadChunk : public Object
  {
  private:
    File m_xFile;       // file being loaded
    Stz* m_aEndChunk;   // last byte of where chunk ends
    Stz* m_aNextRecord; // first byte of next record
  
    WspFxd<C_LOADCHUNK_LENGTH> m_xWsp; // workspace
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    LoadChunk(void);
    ~LoadChunk(void);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void  vCloseFile(void);
    i8    iNextRecord(void);
    Stz*  aNextRecord(i8& iRecordBytes);
    void  vOpenFile(s256& sPath);
    void  vReadChunk(i8 iSeek, i8 nBytes);
    
  private:
    void  vReset(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
