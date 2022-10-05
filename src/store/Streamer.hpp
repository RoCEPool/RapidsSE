/*============================================================================*/
/* Streamer.hpp --                                                           */
/*============================================================================*/
#pragma once

#include "TransformerTool.hpp"

namespace STORE
{
  class Streamer : public TransformerTool
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Streamer(Bench* pBench);
  
   ~Streamer(void) noexcept(false);

    void vAppend_stream_to_scan(void);

    void vCalc_streamOrdinalRanges(void);

  private:



    Footer* aFooter(void);
  };

  Streamer* _pStreamer(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
