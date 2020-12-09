/*============================================================================*/
/* RapidsSE.hpp -- RapidsSE External Api functions                            */
/*============================================================================*/
#pragma once

namespace Interface
{
  class RapidsSE
  {
  public:
  
    static eEC  eAllocSession(void** aaSession);
    static eEC  eFreeSession(void** aaSession);

    static eEC  eDeleteJob(void* aSession);
    static eEC  eExecuteJob(void* aSession);
    static eEC  ePrepareJob(void* aSession, char* azCommand);
    static eEC  eSubmitJob(void* aSession);
    static eEC  eWaitJob(void* aSession);
  
    static Operand* pPopOperand(void* aSession,
                                eCN* aeClassNo,
                                eOM* aeOpMarker);
    static eEC  eProcessOperand(void* aSession, void** aaOperand);
    static eEC  ePushOperand(void* aSession, void** aaOperand);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
