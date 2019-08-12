/*============================================================================*/
/* ApiLog.hpp -- Api log helper                                               */
/*============================================================================*/

#include "Interface.h"
#include "AM.hpp"

extern Log* gpLog;

/*----------------------------------------------------------------------------*/
/* add a log entry for api entry                                              */
/*----------------------------------------------------------------------------*/
void ApiLog::vNoteEntry(i8 eApiCall, i8 iJobId)
{
#ifdef WITH_SHARED_LOG
  if ( ! gpLog )
    return;
  
  if ( ! AM::pMarkApi()->bEnum(eApiCall) &&
       ! AM::pMarkApi()->bAll() )
    return;
  
  Stz* zApiName = aAC[eApiCall];
  
  bool bForeign = Job::bIsForeign(iJobId);
  gpLog->iEntryJob(iJobId,bForeign,">Api %s"_Stz,zApiName);
#endif
}

/*----------------------------------------------------------------------------*/
/* add a log entry for api error                                              */
/*----------------------------------------------------------------------------*/
void ApiLog::vNoteError(i8 eApiCall, i8 iJobId, i8 iError)
{
#ifdef WITH_SHARED_LOG
  if ( ! gpLog )
    return;
  
  if ( ! AM::pMarkApi()->bEnum(eApiCall) &&
      ! AM::pMarkApi()->bAll() )
    return;

  Stz* zApiName = aAC[eApiCall];
  
  bool bForeign = Job::bIsForeign(iJobId);
  gpLog->iEntryJob(iJobId,bForeign,"!Api %s Error %lld"_Stz,zApiName,iError);
#endif
}

/*----------------------------------------------------------------------------*/
/* add a log entry for api exit                                               */
/*----------------------------------------------------------------------------*/
void ApiLog::vNoteExit(i8 eApiCall, i8 iJobId)
{
#ifdef WITH_SHARED_LOG
  if ( ! gpLog )
    return;
  
  if ( ! AM::pMarkApi()->bEnum(eApiCall) &&
      ! AM::pMarkApi()->bAll() )
    return;

  Stz* zApiName = aAC[eApiCall];
  
  bool bForeign = Job::bIsForeign(iJobId);
  gpLog->iEntryJob(iJobId,bForeign,"<Api %s"_Stz,zApiName);
#endif
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
