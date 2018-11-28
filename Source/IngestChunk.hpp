/*============================================================================*/
/* IngestChunk.hpp -- Ingest Chunk Buffer                                     */
/*============================================================================*/
#pragma once

namespace Loader
{
  static const i8 C_INGEST_CHUNK = C_1M; // its 1M not 1MB (i.e. < 1MB)
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class IngestChunk : public Object
  {
  private:
    File m_xFile;              // file/pipe being ingested
    Formatters* m_pFormatters; // using these formatters
  
    Stz* m_aEndChunk;    // last byte of where chunk ends
    Stz* m_aNextRecord;  // first byte of next record
    i8   m_nRecordBytes; // size of record
    bool m_bHaveRecord;  // record in work space?
  
    WspFxd<C_INGEST_CHUNK> m_xWsp; // workspace
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    IngestChunk(Formatters* pFormatters);
    ~IngestChunk(void) noexcept(false);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void  vCloseFile(void);
    i8    iNextRecord(void);
    Stz*  aNextRecord(i8& nRecordBytes);
    bool  bOpenFile(s256& sPath);
    void  vPushBack(void);
    void  vReadChunk(void);
    void  vReadChunk(i8 iSeek, i8 nBytes);
    Stz*  aReadRecord(i8& nRecordBytes);

  private:
    void  vAdvanceRecordEnd(char** aSrc);
    void  vReset(void);
  
    Footer* aFooter(void);
  };
  
  IngestChunk* _pIngestChunk(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
