/*============================================================================*/
/* RapidsSE.hpp -- RapidsSE External Api functions                            */
/*============================================================================*/
#pragma once

namespace Interface
{
  class RapidsSE
  {
  public:
  
    static Session* pAllocSession(void);
    static void vFreeSession(Session* pSession);

    static void vDeleteJob(Session* pSession);
    static void vExecuteJob(Session* pSession);
    static eEC  ePrepareJob(Session* pSession,
                            TokenVec* pCmdTknVec,
                            TokenVec* pLitTknVec,
                            TagSet*   pTagSet);
    static void vSubmitJob(Session* pSession);
    static void vWaitJob(Session* pSession);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
