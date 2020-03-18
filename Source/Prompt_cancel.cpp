/*============================================================================*/
/* Prompt_cancel.cpp -- cancel a pipeline job                                 */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_cancel (
  PF_CANCEL,            // prompt function
  "Prompt_cancel"_Stz, // name of the prompt function
  "comment"_Stz,       // comment about the function
  "cancel"_Stz,        // the prompt command
  nullptr,             // the second word
  bPrompt_cancel,      // prompt function
  true,                // enabled

  true,              // visible
  "cancel"_Stz,      // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_cancel(SePrompt* pPrompt)
{

  TokenVec* pTokenVec = pPrompt->pTokenVec();
  
  ASSERT((*pTokenVec)[0]->bSame("cancel"_Stz));
  
  i8 iJobId = pTokenVec->pGet(1)->iToI8();
  
  // find the pipeline job
  Job* pJob_Pipeline = Scheduler::pFindParticipatingJob(iJobId);
  
  if ( ! pJob_Pipeline )
    vTrace("job %lld not found\n"_Stz, iJobId);
  else
    vTrace("Cancelling job %lld\n"_Stz, iJobId);
  
  // get the first stage of the pipeline
  Stage* pStage = pJob_Pipeline->m_pPipeline->pGet(0);
  
  // send cancelMarker to the startOps of the first stage of the pipeline
  i8 iThisNode = AM::pGrid()->iThisNode();
  pStage->vCancelOperators(iThisNode);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
