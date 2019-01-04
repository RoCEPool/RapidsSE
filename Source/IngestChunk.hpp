/*============================================================================*/
/* IngestChunk.hpp -- Ingest Chunk                                            */
/*============================================================================*/
#pragma once

namespace Chunks
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class IngestChunk : public RecordExtractor_NEW,
                      public ListNode_t<IngestChunk>
  {
  public:
    i8 m_iChunkNo;

    // must be last
    Footer m_xFooter;
  
  public: //$ functions:
    void* operator new(size_t);
    void  operator delete(void*);
    
    IngestChunk(Formatters* pFormatters);
    ~IngestChunk(void) noexcept(false);
  
    Stz*  aStz(void);
 
  private:
    Footer* aFooter(void);
  };
  
  IngestChunk* _pIngestChunk(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
