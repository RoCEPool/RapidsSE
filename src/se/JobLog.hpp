/*============================================================================*/
/* JobLog.hpp -- Job log helper                                               */
/*============================================================================*/
#pragma once

namespace Arguments
{
  class JobLog
  {
  public:
    static void vNote(Arguments::Job* pJob, Stz* szFormat, ...);
    static void vNoteState(Arguments::Job* pJob, Stz* szPoint);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
