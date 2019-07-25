/*============================================================================*/
/* ApiLog.hpp -- Api log helper                                               */
/*============================================================================*/
#pragma once

namespace Interface
{
  class ApiLog
  {
  public:
    static void vNote(i8 eApiCall, i8 iJobId, Stz* aFormat, ...);
    static void vNoteEntry(i8 eApiCall, i8 iJobId=-1);
    static void vNoteError(i8 eApiCall, i8 iError, i8 iJobId=-1);
    static void vNoteExit(i8 eApiCall, i8 iJobId=-1);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
