/*============================================================================*/
/* MetricsHelper.hpp -- Lock Expressions                                            */
/*============================================================================*/
#pragma once

namespace STORE
{
  class MetricsHelper
  {
  public:
    static void vWriteCsv(InfoSet* pInfoSet, StreamDef* pStreamDef, File* pFile);
		static void vWriteCsv(RowBuffer* pIRowBuffer, StreamDef* pStreamDef, File* pFile);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
