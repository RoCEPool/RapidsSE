/*============================================================================*/
/* IngestChunk.hpp -- Ingest Chunk                                            */
/*============================================================================*/
#pragma once

namespace Chunks
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class IngestChunk : public DataChunk
  {
  protected:
  
    // must be last
    Footer m_xFooter;
  
  public: //$ functions:
    void* operator new(size_t);
    void  operator delete(void*);
    
    IngestChunk(void);
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
