/*============================================================================*/
/* IngestChunk.hpp -- Ingest Chunk                                            */
/*============================================================================*/
#pragma once

namespace Chunks
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class IngestChunk : public DataChunk,
                      public ListNode_t<IngestChunk>
  {
  public:
    Formatters* m_pFormatters; // using these formatters

    i8 m_iChunkNo;

    Stz* m_aNextRecord;        // first byte of next record
    i8   m_nRecordBytes;       // size of record

    // must be last
    Footer m_xFooter;
  
  public: //$ functions:
    void* operator new(size_t);
    void  operator delete(void*);
    
    IngestChunk(Formatters* pFormatters);
    ~IngestChunk(void) noexcept(false);
  
    bool  bGetRecord(Stz** aaRecord, i8& nRecordBytes);

    Stz*  aStz(void);
 
  protected:
    bool  bFindRecordEnd(char** aaRecordEnd);

  private:
    Footer* aFooter(void);
  };
  
  IngestChunk* _pIngestChunk(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
