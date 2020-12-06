/*============================================================================*/
/* RowUte.hpp -- Row Ute helper                                               */
/*============================================================================*/
#pragma once

namespace DataBuffers
{
  class RowUte
  {
  public:
   
    static bool bAddSingleToSampleSet(RowSlab* pSampleSet,
                                 RowSingle* pRowSingle,
                                 StreamDef* pStreamDef);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
