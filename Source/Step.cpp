/*============================================================================*/
/* Step.cpp -- Step                                                           */
/*============================================================================*/

#include "Steps.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitive word into main dictionary                                    */
/*----------------------------------------------------------------------------*/
void Step::vAddStep(Stz* aName, StepFp fpStep, bool bImmediate)
{
  RASSERT(aName->nSize() <= 32);
  
  Word* pNewWord = new(&gpAttachMap->m_oRootDic) Word(aName);
  
  //m_pCompiler->m_pCurrentWord = pNewWord;
  
  pNewWord->vWordType(WT_PRIMITIVE);
  pNewWord->vEntryStep(fpStep);
  pNewWord->vImmediate(bImmediate);
}

/*----------------------------------------------------------------------------*/
/* add step primitives                                                        */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps(void)
{
  if (gpAttachMap->m_bVerbose)
    vTrace("[[mainThread]] Adding primitives...\n"_Stz);
  
  vAddStep("word"_Stz,            &Step::vWord);
  gpAttachMap->m_pWord = gpAttachMap->m_oRootDic.pFindWord("word"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Step::vAddSteps_AccessPath();
  Step::vAddSteps_ActivityMap();
  Step::vAddSteps_Address();
  Step::vAddSteps_ApiState();
  Step::vAddSteps_Args();
  Step::vAddSteps_Assert();
  Step::vAddSteps_Astack();
  Step::vAddSteps_AttachMap();
  
  Step::vAddSteps_Bp();
  
  Step::vAddSteps_Cast();
  Step::vAddSteps_Class();
  Step::vAddSteps_Column();
  Step::vAddSteps_Comment();
  Step::vAddSteps_CoreWords();
  Step::vAddSteps_Cpu();
  Step::vAddSteps_Cursor();
  
  Step::vAddSteps_DdlLock();
  Step::vAddSteps_Debug();
  Step::vAddSteps_Dic();
  Step::vAddSteps_DicLock();
  Step::vAddSteps_DicPath();
  Step::vAddSteps_DicStack();
  Step::vAddSteps_Dir();
  Step::vAddSteps_Docol();
  Step::vAddSteps_Dot();
  Step::vAddSteps_Dstack();
  
  Step::vAddSteps_Encode();
  Step::vAddSteps_Enter();
  Step::vAddSteps_EntryBuffer();
  Step::vAddSteps_Env();
  Step::vAddSteps_Error();
  Step::vAddSteps_Exit();
  
  Step::vAddSteps_File();
  Step::vAddSteps_Flow();
  
  Step::vAddSteps_Grid();
  
  Step::vAddSteps_Handler();
  
  Step::vAddSteps_Index();
  Step::vAddSteps_Input();
  Step::vAddSteps_Int();
  
  Step::vAddSteps_Job();
  Step::vAddSteps_JobAnalyzeIndex();
  Step::vAddSteps_JobCat();
  Step::vAddSteps_JobClass();
  Step::vAddSteps_JobCreateIndex();
  Step::vAddSteps_JobExport();
  Step::vAddSteps_JobFetchRows();
  Step::vAddSteps_JobGenData();
  Step::vAddSteps_JobGeneric();
  Step::vAddSteps_JobIngest();
  Step::vAddSteps_JobInsertRows();
  Step::vAddSteps_JobKidExport();
  Step::vAddSteps_JobLoadTable();
  Step::vAddSteps_JobMonitor();
  Step::vAddSteps_JobNetHog();
  Step::vAddSteps_JobNodeInfo();
  Step::vAddSteps_JobPublish();
  Step::vAddSteps_JobPurge();
  Step::vAddSteps_JobReceiver();
  Step::vAddSteps_JobRepeater();
  Step::vAddSteps_JobReport();
  Step::vAddSteps_JobSelectTable();
  Step::vAddSteps_JobSink();
  Step::vAddSteps_JobSubExporter();
  Step::vAddSteps_JobSubIngester();
  Step::vAddSteps_JobSubLoader();
  Step::vAddSteps_JobSubPublish();
  Step::vAddSteps_JobSubPurger();
  Step::vAddSteps_JobTruncateTable();
  Step::vAddSteps_JobWriteBuffer();
  Step::vAddSteps_JobWriteRecord();
  Step::vAddSteps_JobWriteRow();
  
  Step::vAddSteps_Locals();
  Step::vAddSteps_Lock();
  Step::vAddSteps_LockTracker();
  Step::vAddSteps_Logic();
  
  Step::vAddSteps_Magnitude();
  Step::vAddSteps_Memory();
  
  Step::vAddSteps_Node();
  
  Step::vAddSteps_Object();
  
  Step::vAddSteps_Persistence();
  Step::vAddSteps_Planner();
  Step::vAddSteps_Process();
  Step::vAddSteps_Push();
  
  Step::vAddSteps_Queues();
  
  Step::vAddSteps_Row();
  Step::vAddSteps_Rstack();
  
  Step::vAddSteps_Session();
  Step::vAddSteps_Shell();
  Step::vAddSteps_Slots();
  Step::vAddSteps_Socket();
  Step::vAddSteps_StepTiming();
  Step::vAddSteps_Str();
  
  Step::vAddSteps_Table();
  Step::vAddSteps_TablePartition();
  Step::vAddSteps_Task();
  Step::vAddSteps_Test();
  Step::vAddSteps_Thread();
  Step::vAddSteps_Timer();
  Step::vAddSteps_To();
  Step::vAddSteps_Trace();
  Step::vAddSteps_Tx();
  Step::vAddSteps_TxSet();
  
  Step::vAddSteps_Utility();
  
  Step::vAddSteps_Word();
  
  Step::vAddSteps_Zero();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
