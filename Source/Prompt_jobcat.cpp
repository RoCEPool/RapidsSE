/*============================================================================*/
/* Prompt_jobcat.cpp -- print job catalog                                     */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_jobcat (
  "Prompt_jobcat"_Stz, // name of the prompt function
  "comment"_Stz,       // comment about the functopm
  "jobcat"_Stz,        // the prompt command
  nullptr,             // the second word
  bPrompt_jobcat,      // prompt post-process function
  true                 // enabled
);

extern JobCatalog    gxJobCatalog;
extern PromptCatalog gxPromptCatalog;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_jobcat(SePrompt* pPrompt)
{
  vTrace("%20s %10s %20s : %10s %20s %20s %20s %20s %20s\n"_Stz,

         "Job Name"_Stz,
         "Kind"_Stz,
         "Comment"_Stz,

         "Resources"_Stz,
         "Req Seed"_Stz,
         "Rcv Seed"_Stz,
         "Exe Seed"_Stz,
         "End Seed"_Stz,
         "Can Seed"_Stz
  );
  
  s32 sBar10 = "----------"_Stz;
  s32 sBar20 = "--------------------"_Stz;
  
  vTrace("%20s %10s %20s : %10s %20s %20s %20s %20s %20s\n"_Stz,
         sBar20.aStr(),sBar10.aStr(),sBar20.aStr(),sBar10.aStr(),
         sBar20.aStr(),sBar20.aStr(),sBar20.aStr(),sBar20.aStr(),sBar20.aStr());
  
  for (i8 i_Job = 0; i_Job < gxJobCatalog.m_nClasses; i_Job++) {
    
    if ( ! gxJobCatalog.m_xJobClass[i_Job].m_bResources )
      continue;
    
    vTrace("%20s %10s %20s : %10s %20s %20s %20s %20s %20s\n"_Stz,
           
           gxJobCatalog.m_xJobClass[i_Job].m_aName,
           aJK[gxJobCatalog.m_xJobClass[i_Job].m_eKind],
           gxJobCatalog.m_xJobClass[i_Job].m_aComment,

           gxJobCatalog.m_xJobClass[i_Job].m_bResources
             ? "_chk/rel"_Stz : "-"_Stz,

           gxJobCatalog.m_xJobClass[i_Job].m_aSeedReq
             ? gxJobCatalog.m_xJobClass[i_Job].m_aSeedReq : "-"_Stz,
           
           gxJobCatalog.m_xJobClass[i_Job].m_aSeedRcv
             ? gxJobCatalog.m_xJobClass[i_Job].m_aSeedRcv : "-"_Stz,

           gxJobCatalog.m_xJobClass[i_Job].m_aSeedExe
             ? gxJobCatalog.m_xJobClass[i_Job].m_aSeedExe : "-"_Stz,

           gxJobCatalog.m_xJobClass[i_Job].m_aSeedEnd
             ? gxJobCatalog.m_xJobClass[i_Job].m_aSeedEnd : "-"_Stz,

           gxJobCatalog.m_xJobClass[i_Job].m_aSeedCan
             ? gxJobCatalog.m_xJobClass[i_Job].m_aSeedCan : "-"_Stz

    );
  }

  vTrace("\n"_Stz);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
