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
		static Row* pSample(StreamDef* pStreamDef, RowBuffer* pRowBuffer);
    static Row* pSamplePds(Pds* pPdsRowBuffer);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
