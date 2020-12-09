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
    static eEC  ePrepareJob(void* aSession, char* azCommand);
    static void vSubmitJob(void* aSession);
    static void vWaitJob(void* aSession);
  
    static Operand* pPopOperand(void* aSession,
                                eCN* aeClassNo,
                                eOM* aeOpMarker);
    static void vPushOperand(void* aSession, void** aaOperand);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
