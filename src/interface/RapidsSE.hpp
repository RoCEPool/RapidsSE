/*============================================================================*/
/* RapidsSE.hpp -- RapidsSE External Api functions                            */
/*============================================================================*/
#pragma once

namespace Interface
{
  class RapidsSE
  {
  public:
  
    static void* aAllocSession(void);
    static void vFreeSession(void* aSession);

    static void vDeleteJob(void* aSession);
    static void vExecuteJob(void* aSession);
    static eEC  ePrepareJob(void* aSession,
                            TokenVec* pCmdTknVec,
                            TokenVec* pLitTknVec,
                            TagSet*   pTagSet);
    static void vSubmitJob(void* aSession);
    static void vWaitJob(void* aSession);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
