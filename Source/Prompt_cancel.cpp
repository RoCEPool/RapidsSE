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

  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  
  ASSERT(pInpTknVec->pGet(0)->bSame("cancel"_Stz));
  
  i8 iJobId = pInpTknVec->pGet(1)->iToI8();
  
  // find the pipeline job
  Job* pJob_Pipeline = Scheduler::pJob_FindParticipating(iJobId);
  
  if ( ! pJob_Pipeline ) {
    vTrace("job %lld not found\n"_Stz, iJobId);
    Scheduler::vJob_DumpParticipating();
    return false;
  }
  else
    vTrace("Cancelling job %lld\n"_Stz, iJobId);
  
  // get the pipeline from the job
  Pipeline* pPipeline = pJob_Pipeline->m_pPipeline;
  
  // cancel the pipeline
  if ( ! pPipeline->bCancel(nullptr) ) {
    vTrace("Cancel rejected. Job %lld almost done.\n"_Stz, iJobId);
    return false;
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
