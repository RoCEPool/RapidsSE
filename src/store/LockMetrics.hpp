/*============================================================================*/
/* LockMetrics.hpp -- Lock Expressions                                            */
/*============================================================================*/
#pragma once

namespace STORE
{
  class LockMetrics
  {
  public:
    static Row* pSample(StreamDef* pStreamDef, InfoSet* pInfoSet);
    static Row* pSamplePds(StreamDef* pStreamDef, Pds* pPdsRowBuffer);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
