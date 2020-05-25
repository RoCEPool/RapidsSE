/*============================================================================*/
/* Step.hpp -- Step                                                           */
/*============================================================================*/
#pragma once

namespace Steps
{
  class Step : public Forth
  {
  friend class StorageEngine::AttachMap;
  friend class Tasks::Task;
  friend class Word;

  public:
    static void vAddSteps(void);
  
  private:
    static void vAddStep(Stz* aName, StepFp fpStep, bool bImmediate=false);

    static void vAddSteps_AccessPath(void);
    static void vAddSteps_Address(void);
    static void vAddSteps_AggTimer(void);
    static void vAddSteps_Api(void);
    static void vAddSteps_Args(void);
    static void vAddSteps_Article(void);
    static void vAddSteps_Assert(void);
    static void vAddSteps_Astack(void);
    static void vAddSteps_AttachMap(void);
  
    static void vAddSteps_Bp(void);
    static void vAddSteps_Build(void);

    static void vAddSteps_Class(void);
    static void vAddSteps_Column(void);
    static void vAddSteps_Compiler(void);
    static void vAddSteps_Convertor(void);
    static void vAddSteps_CoreWords(void);
    static void vAddSteps_CrwBuffer(void);
    static void vAddSteps_CsvBuffer(void);
    static void vAddSteps_Cursor(void);
  
    static void vAddSteps_DdlLock(void);
    static void vAddSteps_Debug(void);
    static void vAddSteps_Dic(void);
    static void vAddSteps_DicLock(void);
    static void vAddSteps_Dir(void);
    static void vAddSteps_Dot(void);
    static void vAddSteps_Dstack(void);
  
    static void vAddSteps_Encode(void);
    static void vAddSteps_EntBuffer(void);
    static void vAddSteps_Enter(void);
    static void vAddSteps_Env(void);
    static void vAddSteps_Error(void);
    static void vAddSteps_ExtractRow(void);
    static void vAddSteps_Executable(void);

    static void vAddSteps_FetchTest(void);
    static void vAddSteps_File(void);
    static void vAddSteps_Flow(void);
    static void vAddSteps_Forth(void);

    static void vAddSteps_Grid(void);
  
    static void vAddSteps_Handler(void);
  
    static void vAddSteps_Index(void);
    static void vAddSteps_IndexDef(void);
    static void vAddSteps_IndexTree(void);
    static void vAddSteps_InfoSet(void);
    static void vAddSteps_InfoSetTable(void);
    static void vAddSteps_RegIndexTree(void);
    static void vAddSteps_Input(void);
    static void vAddSteps_Int(void);
  
    static void vAddSteps_Job(void);
    static void vAddSteps_JobAnalyzeIndex(void);
    static void vAddSteps_JobClass(void);
    static void vAddSteps_JobGeneric(void);

    static void vAddSteps_JobNodeInfo(void);
    static void vAddSteps_JobOperand(void);
    static void vAddSteps_JobPipeline(void);
    static void vAddSteps_JobRef(void);

    static void vAddSteps_JobWriteBuffer(void);
    static void vAddSteps_JobWriteRecord(void);
  
    static void vAddSteps_Loader(void);
    static void vAddSteps_Locals(void);
    static void vAddSteps_Lock(void);
    static void vAddSteps_LockTracker(void);
    static void vAddSteps_Log(void);
    static void vAddSteps_Logic(void);
  
    static void vAddSteps_Machine(void);
    static void vAddSteps_Magnitude(void);
    static void vAddSteps_Memory(void);
    static void vAddSteps_Metrics(void);

    static void vAddSteps_Node(void);
    static void vAddSteps_Now(void);

    static void vAddSteps_Object(void);
    static void vAddSteps_Olist(void);
    static void vAddSteps_Op(void);
    static void vAddSteps_OpSys(void);

    static void vAddSteps_Persistence(void);
    static void vAddSteps_Pipeline(void);
    static void vAddSteps_Planner(void);
    static void vAddSteps_Plist(void);
    static void vAddSteps_Procedure(void);
    static void vAddSteps_Process(void);
    static void vAddSteps_Push(void);
  
    static void vAddSteps_Queues(void);
  
    static void vAddSteps_Register(void);
    static void vAddSteps_RegAggTimer(void);
    static void vAddSteps_RegCounter(void);
    static void vAddSteps_RegFolder(void);
    static void vAddSteps_RegJob(void);
    static void vAddSteps_RegIndex(void);
    static void vAddSteps_RegNow(void);
    static void vAddSteps_RegOp(void);
    static void vAddSteps_RegOperand(void);
    static void vAddSteps_RegOperandB(void);
    static void vAddSteps_RegOperandC(void);
    static void vAddSteps_RegOperandOoO(void);
    static void vAddSteps_RegPath(void);
    static void vAddSteps_RegPipein(void);
    static void vAddSteps_RegSet(void);
    static void vAddSteps_RegStreamDef(void);
    static void vAddSteps_RegTask(void);
    static void vAddSteps_RegTimer(void);
    static void vAddSteps_RegXmits(void);
    static void vAddSteps_Rstack(void);
    static void vAddSteps_Row(void);
    static void vAddSteps_RowOnce(void);
  
    static void vAddSteps_Session(void);
    static void vAddSteps_Shell(void);
    static void vAddSteps_Socket(void);
    static void vAddSteps_SsnState(void);
    static void vAddSteps_StepTiming(void);
    static void vAddSteps_Str(void);
    static void vAddSteps_StreamSet(void);
    static void vAddSteps_StreamSetTable(void);
    static void vAddSteps_Subset(void);
  
    static void vAddSteps_Table(void);
    static void vAddSteps_TableDef(void);
    static void vAddSteps_TablePartition(void);
    static void vAddSteps_TablePartition2(void);
    static void vAddSteps_Tag(void);
    static void vAddSteps_TagSet(void);
    static void vAddSteps_Task(void);
    static void vAddSteps_Tenants(void);
    static void vAddSteps_Test(void);
    static void vAddSteps_Thread(void);
    static void vAddSteps_Ticking(void);
    static void vAddSteps_Timer(void);
    static void vAddSteps_To(void);
    static void vAddSteps_Trace(void);
    static void vAddSteps_Tx(void);
    static void vAddSteps_TxSet(void);
  
    static void vAddSteps_User(void);
    static void vAddSteps_Utility(void);
 
    static void vAddSteps_Vortex(void);

    static void vAddSteps_Word(void);
    static void vAddSteps_WriteBehind(void);
    
    static void vAddSteps_Zero(void);
  
    /*------------------------------------------------------------------------*/
  
    #include "StepsSet.hpp"
  };
}

#define THROW_ASTACK() \
  Correctness::vThrow(FK_ASTACK,VL_FATAL,\
                      __func__,__FILE__,__LINE__, \
                      "Invalid a-stack"_Stz); \
EndDefine

#define THROW_DSTACK() \
  Correctness::vThrow(FK_DSTACK,VL_FATAL,\
                      __func__,__FILE__,__LINE__, \
                      "Invalid d-stack"_Stz); \
EndDefine

#define THROW_RSTACK() \
  Correctness::vThrow(FK_RSTACK,VL_FATAL,\
                      __func__,__FILE__,__LINE__, \
                      "Invalid r-stack"_Stz); \
EndDefine

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
