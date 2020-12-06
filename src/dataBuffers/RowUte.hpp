/*============================================================================*/
/* RowUte.hpp -- Row Ute helper                                               */
/*============================================================================*/
#pragma once

namespace DataBuffers
{
  class RowUte
  {
  public:
   
    static bool bAddSingleToSlab(RowSlab* pRowSlab,
                                 RowSingle* pRowSingle,
                                 StreamDef* pStreamDef);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
