/*============================================================================*/
/* Step.cpp -- Step                                                           */
/*============================================================================*/

#include "Steps.h"

StepFp gfpEntryStep = &Steps::Step::vEnterRoutine;

/*----------------------------------------------------------------------------*/
/* add primitive word into main dictionary                                    */
/*----------------------------------------------------------------------------*/
void Step::vAddStep(Stz* aName, StepFp fpStep, bool bImmediate)
{
  //vTrace("%s Size = %lld\n"_Stz, aName, aName->nSize());
  RASSERT(aName->nSize() <= 32);
  
  Dic* pRootDic = AM::pRootDic();
  Word* pNewWord = new(pRootDic) Word(aName);
  
  //m_pCompiler->vCurrentWord(pNewWord);
  
  pNewWord->vWord(WK_INTRINSIC);
  pNewWord->vEntryStep(fpStep);
  pNewWord->vImmediate(bImmediate);
}

/*----------------------------------------------------------------------------*/
/* add step primitives                                                        */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps(void)
{
  //Dic* pRootDic = AM::pRootDic();
  if (AM::bVerbose())
    vTrace("[[mainThread]] Adding primitives...\n"_Stz);
  
  //vAddStep("word"_Stz,            &Step::vWord);
  //AM::vRootWord(pRootDic->pFindWord("word"_Stz));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Step::vAddSteps_AccessPath();
  Step::vAddSteps_AggTimer();
  Step::vAddSteps_Address();
  Step::vAddSteps_Api();
  Step::vAddSteps_Args();
  Step::vAddSteps_Arrange();
  Step::vAddSteps_Article();
  Step::vAddSteps_Assert();
  Step::vAddSteps_Astack();
  Step::vAddSteps_AttachMap();
  
  Step::vAddSteps_Bp();
  Step::vAddSteps_Build();

  Step::vAddSteps_Class();
  Step::vAddSteps_Column();
  Step::vAddSteps_Compiler();
  Step::vAddSteps_CoreWords();
  Step::vAddSteps_CrwBuffer();
  Step::vAddSteps_CsvBuffer();
  Step::vAddSteps_Cursor();
  
  Step::vAddSteps_DdlLock();
  Step::vAddSteps_Debug();
  Step::vAddSteps_Dic();
  Step::vAddSteps_DicLock();
  Step::vAddSteps_Dir();
  Step::vAddSteps_Dot();
  Step::vAddSteps_Dstack();
  
  Step::vAddSteps_TagSet();
  Step::vAddSteps_Encode();
  Step::vAddSteps_Enter();
  Step::vAddSteps_EntBuffer();
  Step::vAddSteps_Env();
  Step::vAddSteps_Error();
  Step::vAddSteps_Executable();
  
  Step::vAddSteps_File();
  Step::vAddSteps_Flow();
  Step::vAddSteps_Forth();

  Step::vAddSteps_Grid();
  
  Step::vAddSteps_Handler();
  
  Step::vAddSteps_Index();
  Step::vAddSteps_IndexDef();
  Step::vAddSteps_IndexTree();
  Step::vAddSteps_InfoSet();
  Step::vAddSteps_InfoSetTable();
  Step::vAddSteps_Input();
  Step::vAddSteps_Int();
  
  Step::vAddSteps_Job();
  Step::vAddSteps_JobAnalyzeIndex();
  Step::vAddSteps_JobGeneric();
  Step::vAddSteps_JobPipeline();
  Step::vAddSteps_JobRef();
  
  Step::vAddSteps_Loader();
  Step::vAddSteps_Locals();
  Step::vAddSteps_Lock();
  Step::vAddSteps_LockTracker();
  Step::vAddSteps_Log();
  Step::vAddSteps_Logic();
  
  Step::vAddSteps_Machine();
  Step::vAddSteps_Magnitude();
  Step::vAddSteps_Memory();
  Step::vAddSteps_Metrics();

  Step::vAddSteps_Node();
  Step::vAddSteps_Now();

  Step::vAddSteps_Object();
  Step::vAddSteps_Olist();
  Step::vAddSteps_Op();
  Step::vAddSteps_OpSys();

  Step::vAddSteps_Persistence();
  Step::vAddSteps_Pipeline();
  Step::vAddSteps_Planner();
  Step::vAddSteps_Plist();
  Step::vAddSteps_Procedure();
  Step::vAddSteps_Process();
  Step::vAddSteps_Profile();
  Step::vAddSteps_PtnStream();
  Step::vAddSteps_Push();
  
  Step::vAddSteps_Queues();
  
  Step::vAddSteps_Register();
  Step::vAddSteps_RegAggTimer();
  Step::vAddSteps_RegCounter();
  Step::vAddSteps_RegFolder();
  Step::vAddSteps_RegIndex();
  Step::vAddSteps_RegIndexTree();
  Step::vAddSteps_RegJob();
  Step::vAddSteps_RegNow();
  Step::vAddSteps_RegOp();
  Step::vAddSteps_RegOperand();
  Step::vAddSteps_RegOperandB();
  Step::vAddSteps_RegOperandC();
  Step::vAddSteps_RegOperandOoO();
  Step::vAddSteps_RegPath();
  Step::vAddSteps_RegPipein();
  Step::vAddSteps_RegSet();
  Step::vAddSteps_RegStreamDef();
  Step::vAddSteps_RegTask();
  Step::vAddSteps_RegTimer();
  Step::vAddSteps_RegXmits();
  Step::vAddSteps_Row();
  Step::vAddSteps_RowOnce();
  Step::vAddSteps_RsbBuffer();
  Step::vAddSteps_Rstack();
  
  Step::vAddSteps_SeekOffsets();
  Step::vAddSteps_Session();
  Step::vAddSteps_Shell();
  Step::vAddSteps_Socket();
  Step::vAddSteps_SsnState();
  Step::vAddSteps_StepTiming();
  Step::vAddSteps_Str();
  Step::vAddSteps_StreamSet();
  Step::vAddSteps_StreamSetTable();
  Step::vAddSteps_Subset();
  
  Step::vAddSteps_Table();
  Step::vAddSteps_TablePartition();
  Step::vAddSteps_TablePartition2();
  Step::vAddSteps_Tag();
  Step::vAddSteps_Task();
  Step::vAddSteps_Tenants();
  Step::vAddSteps_Test();
  Step::vAddSteps_Thread();
  Step::vAddSteps_Ticking();
  Step::vAddSteps_Timer();
  Step::vAddSteps_To();
  Step::vAddSteps_ToHshCrw();
  Step::vAddSteps_ToNstRsb();
  Step::vAddSteps_ToPtnRsb();
  Step::vAddSteps_ToRsb();
  Step::vAddSteps_Trace();
  Step::vAddSteps_Tx();
  Step::vAddSteps_TxSet();
  
  Step::vAddSteps_User();
  Step::vAddSteps_Utility();
  
  Step::vAddSteps_Vortex();
  
  Step::vAddSteps_Word();
  Step::vAddSteps_WriteHash();

  Step::vAddSteps_XmitOperand();
  
  Step::vAddSteps_Zero();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
