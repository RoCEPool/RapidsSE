/*============================================================================*/
/* Step.cpp -- Step                                                           */
/*============================================================================*/

#include "BASE.h"

#include "Step.hpp"
#include "TagConstants.h"

fpSTEPFN gfpDoColon = &STORE::Step::vSenDoColon;

/*----------------------------------------------------------------------------*/
/* add primitive word into main dictionary                                    */
/*----------------------------------------------------------------------------*/
void Step::vAddStep(Stz* aName, fpSTEPFN fpStepFn, bool bImmediate)
{
  //vTrace("%s Size = %lld\n"_Stz, aName, aName->nSize());
  RASSERT(aName->nSize() <= 32);
  
  Dic* pDicRoot = AM::DIC::pRoot();
  Word* pNewWord = new(pDicRoot) Word(aName);
  
  //m_pSenCompiler->vCurrentWord(pNewWord);
  
  pNewWord->vWord(WK_INTRINSIC);
  pNewWord->vEntryStep(fpStepFn);
  pNewWord->vImmediate(bImmediate);
}

/*----------------------------------------------------------------------------*/
/* add step primitives                                                        */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps(void)
{
  //Dic* pDicRoot = AM::DIC::pRoot();
  if (AM::TRACE::bVerbose(C_VB_BOOT))
    vTrace("[[mainThread]] Adding primitives...\n"_Stz);
  
  //vAddStep("word"_Stz,            &Step::vWord);
  //AM::vRootWord(pDicRoot->pFindWord("word"_Stz));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Step::vAddSteps_AccessPath();
  Step::vAddSteps_AdaptorWL();
  Step::vAddSteps_Address();
  Step::vAddSteps_AggTimer();
  Step::vAddSteps_AM();
  Step::vAddSteps_AnswerWL();
  Step::vAddSteps_Api();
  Step::vAddSteps_SysArgs();
  Step::vAddSteps_Arrange();
  Step::vAddSteps_Assert();
  Step::vAddSteps_Astack();
  Step::vAddSteps_AttachMap();
  
  Step::vAddSteps_Bp();
  Step::vAddSteps_BktMap();
  Step::vAddSteps_BktMapGet();
  Step::vAddSteps_BktMapSend();
  Step::vAddSteps_BktMapRecv();
  Step::vAddSteps_Build();
  Step::vAddSteps_BucketGet();
  Step::vAddSteps_BucketRecv();

  Step::vAddSteps_Class();
  Step::vAddSteps_Column();
  Step::vAddSteps_Combined();
  Step::vAddSteps_CoreWords();
  Step::vAddSteps_CrwBuffer();
  Step::vAddSteps_CrwToRow();
  Step::vAddSteps_CsvBuffer();
  Step::vAddSteps_Cursor();

  Step::vAddSteps_DataBuffer();
  Step::vAddSteps_DdlLock();
  Step::vAddSteps_Debug();
  Step::vAddSteps_Deck();
  Step::vAddSteps_DeckOfOp();
  Step::vAddSteps_Dic();
  Step::vAddSteps_DicLock();
  Step::vAddSteps_Dir();
  Step::vAddSteps_Discern();
  Step::vAddSteps_Dot();
  Step::vAddSteps_Dstack();
  Step::vAddSteps_Dynamo();

  Step::vAddSteps_Encode();
  Step::vAddSteps_EntBuffer();
  Step::vAddSteps_Env();
  Step::vAddSteps_ErrorCode();
  Step::vAddSteps_ErrorMarker();
  Step::vAddSteps_Event();
  Step::vAddSteps_Executable();

  Step::vAddSteps_File();
  Step::vAddSteps_Flush();
  Step::vAddSteps_Fun();

  Step::vAddSteps_Grid();
  
  Step::vAddSteps_Handler();
  
  Step::vAddSteps_Index();
  Step::vAddSteps_IndexDef();
  Step::vAddSteps_IndexStream();
  Step::vAddSteps_InfoSet();
  Step::vAddSteps_InfoSetRow();
  Step::vAddSteps_InfoSetTable();
  Step::vAddSteps_Inject();
  Step::vAddSteps_Input();
  Step::vAddSteps_Int();
  Step::vAddSteps_IntStack();
  
  Step::vAddSteps_Job();
  Step::vAddSteps_JobAnalyzeIndex();
  Step::vAddSteps_JobGeneric();
  Step::vAddSteps_JobMetrics();
  Step::vAddSteps_ListedJob();
  Step::vAddSteps_JobReport();
  Step::vAddSteps_Jobs();

  Step::vAddSteps_List();
  Step::vAddSteps_Locals();
  Step::vAddSteps_Lock();
  Step::vAddSteps_Locks();
  Step::vAddSteps_Logic();
  Step::vAddSteps_LogicUnit();

  Step::vAddSteps_Machine();
  Step::vAddSteps_Magnitude();
  Step::vAddSteps_Marker();
  Step::vAddSteps_MetaGet();
  Step::vAddSteps_MetaTable();
  Step::vAddSteps_Memory();
  Step::vAddSteps_Monitor();

  Step::vAddSteps_NamedBuffer();
  Step::vAddSteps_Network();
  Step::vAddSteps_Node();
  Step::vAddSteps_Node_I();
  Step::vAddSteps_Now();

  Step::vAddSteps_Object();
  Step::vAddSteps_Op();
  Step::vAddSteps_Operand();
  Step::vAddSteps_OperandOoO();
  Step::vAddSteps_OpSys();
  Step::vAddSteps_Optimiser();

  Step::vAddSteps_PagingMetrics();
  Step::vAddSteps_Parcel();
  Step::vAddSteps_Persistence();
  Step::vAddSteps_Pipein();
  Step::vAddSteps_Pipeline();
  Step::vAddSteps_PipelineOfOp();
  Step::vAddSteps_PipeSpec();
  Step::vAddSteps_PathIdentifier();
  Step::vAddSteps_PlanOperand();
  Step::vAddSteps_Pot();
  Step::vAddSteps_PotRlu();
  Step::vAddSteps_Procedure();
  Step::vAddSteps_Process();
  Step::vAddSteps_Profile();
  Step::vAddSteps_PtnDef();
  Step::vAddSteps_PtnMap();
  Step::vAddSteps_PtnStream();
  Step::vAddSteps_Push();

  Step::vAddSteps_RecvdObject();
  Step::vAddSteps_RecvWL();
  Step::vAddSteps_RefTimer();
  Step::vAddSteps_RegAggTimer();
  Step::vAddSteps_RegCounter();
  Step::vAddSteps_RegFolder();
  Step::vAddSteps_RegNow();
  Step::vAddSteps_RegPath();
  Step::vAddSteps_RegSends();
  Step::vAddSteps_RowOnce();
  Step::vAddSteps_RowBuffer();
  Step::vAddSteps_Rstack();
  
  Step::vAddSteps_Schedule();
  Step::vAddSteps_SeekOffsets();
  Step::vAddSteps_Sen();
  Step::vAddSteps_SenCompiler();
  Step::vAddSteps_SendNode();
  Step::vAddSteps_SendWL();
  Step::vAddSteps_Session();
  Step::vAddSteps_Shell();
  Step::vAddSteps_Socket();
  Step::vAddSteps_SqlCompiler();
  Step::vAddSteps_SsnState();
  Step::vAddSteps_Stage();
  Step::vAddSteps_StageOfOp();
  Step::vAddSteps_Stats();
  Step::vAddSteps_Stencil();
  Step::vAddSteps_Steps();
  Step::vAddSteps_Str();
  Step::vAddSteps_Stream();
  Step::vAddSteps_StringA();
  Step::vAddSteps_StreamDefLft();
  Step::vAddSteps_StreamDefOut();
  Step::vAddSteps_StreamDefRht();
  Step::vAddSteps_StreamOfOp();
  Step::vAddSteps_StreamSet();
  Step::vAddSteps_StreamSetTable();
  Step::vAddSteps_SubCall();
  Step::vAddSteps_SubReturn();
  Step::vAddSteps_Subset();
  Step::vAddSteps_Supervisor();
  Step::vAddSteps_SysMetrics();
  Step::vAddSteps_SysReport();
  Step::vAddSteps_System();
  

  Step::vAddSteps_Table();
  Step::vAddSteps_TablePtn();
  Step::vAddSteps_Tag();
  Step::vAddSteps_TagSet();
  Step::vAddSteps_Task();
  Step::vAddSteps_Tenants();
  Step::vAddSteps_Test();
  Step::vAddSteps_Text();
  Step::vAddSteps_Thread();
  Step::vAddSteps_Ticking();
  Step::vAddSteps_Timer();
  Step::vAddSteps_TinyC();
  Step::vAddSteps_To();
  Step::vAddSteps_TokenVec();
  Step::vAddSteps_ToKeyHshRow();
  Step::vAddSteps_ToLglPtnCrw();
  Step::vAddSteps_ToLglPtnNst();
  Step::vAddSteps_ToLglPtnRow();
  Step::vAddSteps_Trace();
  Step::vAddSteps_Tub();
  Step::vAddSteps_Tx();
  Step::vAddSteps_TxSet();
  
  Step::vAddSteps_User();
    
  Step::vAddSteps_Vertex();
  Step::vAddSteps_VertexOfOp();

  Step::vAddSteps_With();
  Step::vAddSteps_Word();
  
  Step::vAddSteps_Zero();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
