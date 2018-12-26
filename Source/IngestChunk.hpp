/*============================================================================*/
/* IngestChunk.hpp -- Ingest Chunk Buffer                                     */
/*============================================================================*/
#pragma once

namespace Chunks
{
  static const i8 C_INGEST_CHUNK = C_1M; // its 1M not 1MB (i.e. < 1MB)
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class IngestChunk : public RecordExtractor
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    IngestChunk(Formatters* pFormatters);
    ~IngestChunk(void) noexcept(false);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void  vCloseFile(void);
    bool  bGetRecord(Stz** aaRecord, i8& nRecordBytes);
    i8    iNextRecord(void);
    bool  bOpenFile(s256& sPath);
     bool  bReadChunk(void);
    void  vReadChunk(i8 iSeek, i8 nBytes);

  private:
    void  vCopyDown(void);
    bool  bFindRecordEnd(char** aRecordEnd);
    void  vReset(void);
  
    Footer* aFooter(void);
  };
  
  IngestChunk* _pIngestChunk(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
