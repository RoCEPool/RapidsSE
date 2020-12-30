/*============================================================================*/
/* RowUte.hpp -- Row Ute helper                                               */
/*============================================================================*/
#pragma once

namespace STORE
{
  class RowUte
  {
  public:
   
    static bool bAddSingleToSampleSet(RowBuffer* pSampleSet,
                                      RowSingle* pRowSingle,
                                      StreamDef* pStreamDef);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
