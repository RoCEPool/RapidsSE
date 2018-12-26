/*============================================================================*/
/* RecordLoader.hpp -- Record Loader                                          */
/*============================================================================*/
#pragma once

namespace Chunks
{
  static const i8 C_LOAD_CHUNK = C_1M; // its 1M not 1MB (i.e. < 1MB)
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class RecordLoader : public Object
  {
  private:
    Formatters* m_pFormatters; // using these formatters
  
    File m_oFile;        // file being loaded
  
    Stz* m_aEndChunk;    // last byte of where chunk ends
  
    Stz* m_aNextRecord;  // first byte of next record
    i8   m_nRecordBytes; // size of record
  
    WspFxd<C_LOAD_CHUNK> m_xWsp; // workspace
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    RecordLoader(Formatters* pFormatters);
    ~RecordLoader(void) noexcept(false);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void  vCloseFile(void);
    i8    iNextRecord(void);
    bool  bGetRecord(Stz** aaRecord, i8& iRecordBytes);
    void  vOpenFile(s256& sPath);
    void  vReadChunk(i8 iSeek, i8 nBytes);
    
  private:
    void  vAdvanceRecordEnd(char** aSrc, Formatters* pFormatters);
    void  vReset(void);
  
    Footer* aFooter(void);
  };
  
  RecordLoader* _pRecordLoader(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
