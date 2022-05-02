/*============================================================================*/
/* Step.hpp -- Step                                                           */
/*============================================================================*/
#pragma once

#include "RowOnce.hpp"
#include "IndexStream.hpp"

// declare a function pointer to any member function of Step
typedef void (Step::*fpSTEPFN)(void);

namespace STORE
{
  class Step : public Task
  {
  friend class STORE::AttachMap;
  friend class Word;

  public:
    static void vAddSteps(void);
  
  private:
    static void vAddStep(Stz* aName, fpSTEPFN fpStepFn, bool bImmediate=false);

    static void vAddSteps_AccessPath(void);
    static void vAddSteps_AdaptorWL(void);
    static void vAddSteps_Address(void);
    static void vAddSteps_AggTimer(void);
    static void vAddSteps_AM(void);
    static void vAddSteps_AnswerWL(void);
    static void vAddSteps_Api(void);
    static void vAddSteps_SysArgs(void);
    static void vAddSteps_Arrange(void);
    static void vAddSteps_Assert(void);
    static void vAddSteps_Astack(void);
    static void vAddSteps_AttachMap(void);
  
    static void vAddSteps_Bp(void);
    static void vAddSteps_BktMap(void);
    static void vAddSteps_BktMapGet(void);
    static void vAddSteps_BktMapSend(void);
    static void vAddSteps_BktMapRecv(void);
    static void vAddSteps_Build(void);
    static void vAddSteps_BucketGet(void);
    static void vAddSteps_BucketRecv(void);
    static void vAddSteps_BucketSend(void);

    static void vAddSteps_Class(void);
    static void vAddSteps_Column(void);
    static void vAddSteps_Combined(void);
    static void vAddSteps_CoreWords(void);
    static void vAddSteps_CrwBuffer(void);
    static void vAddSteps_CrwToRow(void);
    static void vAddSteps_CsvBuffer(void);
    static void vAddSteps_Cursor(void);

    static void vAddSteps_DataBuffer(void);
    static void vAddSteps_DdlLock(void);
    static void vAddSteps_Debug(void);
    static void vAddSteps_Deck(void);
    static void vAddSteps_DeckOfOp(void);
    static void vAddSteps_Dic(void);
    static void vAddSteps_DicLock(void);
    static void vAddSteps_Dir(void);
    static void vAddSteps_Discern(void);
    static void vAddSteps_Dot(void);
    static void vAddSteps_Dstack(void);
    static void vAddSteps_Dynamo(void);

    static void vAddSteps_Encode(void);
    static void vAddSteps_EntBuffer(void);
    static void vAddSteps_Env(void);
    static void vAddSteps_ErrorCode(void);
    static void vAddSteps_ErrorMarker(void);
    static void vAddSteps_EvMgr(void);
    static void vAddSteps_Executable(void);

    static void vAddSteps_FetchTest(void);
    static void vAddSteps_File(void);
    static void vAddSteps_Fun(void);
    static void vAddSteps_Flush(void);

    static void vAddSteps_Grid(void);
  
    static void vAddSteps_Handler(void);
  
    static void vAddSteps_Index(void);
    static void vAddSteps_IndexDef(void);
    static void vAddSteps_IndexStream(void);
    static void vAddSteps_InfoSet(void);
    static void vAddSteps_InfoSetRow(void);
    static void vAddSteps_InfoSetTable(void);
    static void vAddSteps_Inject(void);
    static void vAddSteps_Input(void);
    static void vAddSteps_Int(void);
    static void vAddSteps_IntStack(void);

    static void vAddSteps_Job(void);
    static void vAddSteps_JobAnalyzeIndex(void);
    static void vAddSteps_JobGeneric(void);
    static void vAddSteps_JobMetrics(void);
    static void vAddSteps_JobPipeline(void);
    static void vAddSteps_ListedJob(void);
    static void vAddSteps_JobReport(void);
    static void vAddSteps_Jobs(void);

    static void vAddSteps_List(void);
    static void vAddSteps_Locals(void);
    static void vAddSteps_Lock(void);
    static void vAddSteps_Locks(void);
    static void vAddSteps_Logic(void);
    static void vAddSteps_LogicUnit(void);

    static void vAddSteps_Machine(void);
    static void vAddSteps_Magnitude(void);
    static void vAddSteps_Marker();
    static void vAddSteps_MetaGet(void);
    static void vAddSteps_MetaTable(void);
    static void vAddSteps_Memory(void);
    static void vAddSteps_Monitor(void);
 
    static void vAddSteps_NamedBuffer(void);
    static void vAddSteps_Network(void);
    static void vAddSteps_Node(void);
    static void vAddSteps_Node_I(void);
    static void vAddSteps_Now(void);

    static void vAddSteps_Object(void);
    static void vAddSteps_Op(void);
    static void vAddSteps_Operand(void);
    static void vAddSteps_OperandOoO(void);
    static void vAddSteps_OpSys(void);
    static void vAddSteps_Optimiser(void);

    static void vAddSteps_PagingMetrics(void);
    static void vAddSteps_PageOut(void);
    static void vAddSteps_Parcel(void);
    static void vAddSteps_Persistence(void);
    static void vAddSteps_Pipeline(void);
    static void vAddSteps_PipelineOfOp(void);
    static void vAddSteps_PipeSpec(void);
    static void vAddSteps_PathIdentifier(void);
    static void vAddSteps_PlanOperand(void);
    static void vAddSteps_Pot(void);
    static void vAddSteps_PotRlu(void);
    static void vAddSteps_Procedure(void);
    static void vAddSteps_Process(void);
    static void vAddSteps_Profile(void);
    static void vAddSteps_PtnDef(void);
    static void vAddSteps_PtnMap(void);
    static void vAddSteps_PtnStream(void);
    static void vAddSteps_Push(void);
  
    static void vAddSteps_RecvdObject(void);
    static void vAddSteps_RecvWL(void);
    static void vAddSteps_RefTimer(void);
    static void vAddSteps_RegCounter(void);
    static void vAddSteps_RegFolder(void);
    static void vAddSteps_RegNow(void);
    static void vAddSteps_RegPath(void);
    static void vAddSteps_Pipein(void);
    static void vAddSteps_RegSends(void);
    static void vAddSteps_ResponseOp(void);
    static void vAddSteps_RowOnce(void);
    static void vAddSteps_RowBuffer(void);
    static void vAddSteps_Rstack(void);

    static void vAddSteps_Schedule(void);
    static void vAddSteps_SeekOffsets(void);
    static void vAddSteps_Sen(void);
    static void vAddSteps_SenCompiler(void);
    static void vAddSteps_SendNode(void);
    static void vAddSteps_SendWL(void);
    static void vAddSteps_Shell(void);
    static void vAddSteps_Socket(void);
    static void vAddSteps_SqlCompiler(void);
    static void vAddSteps_Stage(void);
    static void vAddSteps_StageOfOp(void);
    static void vAddSteps_Stats(void);
    static void vAddSteps_Stencil(void);
    static void vAddSteps_Steps(void);
    static void vAddSteps_Str(void);
    static void vAddSteps_Stream(void);
    static void vAddSteps_StreamDefLft(void);
    static void vAddSteps_StreamDefOut(void);
    static void vAddSteps_StreamDefRht(void);
    static void vAddSteps_StreamOfOp(void);
    static void vAddSteps_StreamSet(void);
    static void vAddSteps_StreamSetTable(void);
    static void vAddSteps_StringA(void);
    static void vAddSteps_SubCall(void);
    static void vAddSteps_SubReturn(void);
    static void vAddSteps_Subset(void);
    static void vAddSteps_Supervisor(void);
    static void vAddSteps_SysMetrics(void);
    static void vAddSteps_SysReport(void);
    static void vAddSteps_System(void);

    static void vAddSteps_Table(void);
    static void vAddSteps_TablePtn(void);
    static void vAddSteps_Tag(void);
    static void vAddSteps_TagSet(void);
    static void vAddSteps_Task(void);
    static void vAddSteps_Tenants(void);
    static void vAddSteps_Test(void);
    static void vAddSteps_Text(void);
    static void vAddSteps_Thread(void);
    static void vAddSteps_Ticking(void);
    static void vAddSteps_Timer(void);
    static void vAddSteps_TinyC(void);
    static void vAddSteps_To(void);
    static void vAddSteps_TokenVec(void);
    static void vAddSteps_ToKeyHshRow(void);
    static void vAddSteps_ToLglPtnCrw(void);
    static void vAddSteps_ToLglPtnNst(void);
    static void vAddSteps_ToLglPtnRow(void);
    static void vAddSteps_Trace(void);
    static void vAddSteps_Tub(void);
    static void vAddSteps_Tx(void);
    static void vAddSteps_TxSet(void);
  
    static void vAddSteps_User(void);
 
    static void vAddSteps_Vertex(void);
    static void vAddSteps_VertexOfOp(void);

    static void vAddSteps_With(void);
    static void vAddSteps_Word(void);

    static void vAddSteps_Zero(void);
  
    /*------------------------------------------------------------------------*/
  
    #include "StepsSet.hpp"

    void vBktMapPipeOut(void);
  };
}

#define THROW_ASTACK() \
  BASE::vThrow(FK_ASTACK,\
                      __func__,__FILE__,__LINE__, \
                      "Invalid a-stack"_Stz); \
EndDefine

#define THROW_DSTACK() \
  BASE::vThrow(FK_DSTACK,\
                      __func__,__FILE__,__LINE__, \
                      "Invalid d-stack"_Stz); \
EndDefine

#define THROW_RSTACK() \
  BASE::vThrow(FK_RSTACK,\
                      __func__,__FILE__,__LINE__, \
                      "Invalid r-stack"_Stz); \
EndDefine

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
