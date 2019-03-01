/*============================================================================*/
/* Step.hpp -- Step                                                           */
/*============================================================================*/
#pragma once

namespace Steps
{
  // tracing masks
  static const i8 C_TRACE_NONE     = 0x0;
  static const i8 C_TRACE_STEPS    = 0x1;
  static const i8 C_TRACE_ROUTINES = 0x2;

  class Step
  {
  friend class Tasks::Task;
  friend class Forth::Word;
  
  public:
    static void vAddSteps(void);
  
  private:
    static void vAddStep(Stz* aName, StepFp fpStep, bool bImmediate=false);

    static void vAddSteps_AccessPath(void);
    static void vAddSteps_ActivityMap(void);
    static void vAddSteps_Address(void);
    static void vAddSteps_ApiState(void);
    static void vAddSteps_Args(void);
    static void vAddSteps_Assert(void);
    static void vAddSteps_Astack(void);
    static void vAddSteps_AttachMap(void);
  
    static void vAddSteps_Bp(void);
  
    static void vAddSteps_Cast(void);
    static void vAddSteps_Class(void);
    static void vAddSteps_Column(void);
    static void vAddSteps_Comment(void);
    static void vAddSteps_CoreWords(void);
    static void vAddSteps_Cpu(void);
    static void vAddSteps_Cursor(void);
  
    static void vAddSteps_DdlLock(void);
    static void vAddSteps_Debug(void);
    static void vAddSteps_Dic(void);
    static void vAddSteps_DicLock(void);
    static void vAddSteps_DicPath(void);
    static void vAddSteps_DicStack(void);
    static void vAddSteps_Dir(void);
    static void vAddSteps_Docol(void);
    static void vAddSteps_Dot(void);
    static void vAddSteps_Dstack(void);
  
    static void vAddSteps_Encode(void);
    static void vAddSteps_EntryBuffer(void);
    static void vAddSteps_Enter(void);
    static void vAddSteps_Env(void);
    static void vAddSteps_Error(void);
    static void vAddSteps_Exit(void);
  
    static void vAddSteps_FetchTest(void);
    static void vAddSteps_File(void);
    static void vAddSteps_Flow(void);
  
    static void vAddSteps_Grid(void);
  
    static void vAddSteps_Handler(void);
  
    static void vAddSteps_Index(void);
    static void vAddSteps_Input(void);
    static void vAddSteps_Int(void);
  
    static void vAddSteps_Job(void);
    static void vAddSteps_JobAnalyzeIndex(void);
    static void vAddSteps_JobCat(void);
    static void vAddSteps_JobClass(void);
    static void vAddSteps_JobCreateIndex(void);
    static void vAddSteps_JobExport(void);
    static void vAddSteps_JobFetchRows(void);
    static void vAddSteps_JobGenData(void);
    static void vAddSteps_JobGeneric(void);
    static void vAddSteps_JobIngest(void);
    static void vAddSteps_JobInsertRows(void);
    static void vAddSteps_JobLoadTable(void);
    static void vAddSteps_JobMonitor(void);
    static void vAddSteps_JobNetHog(void);
    static void vAddSteps_JobNodeInfo(void);
    static void vAddSteps_JobPublish(void);
    static void vAddSteps_JobPurge(void);
    static void vAddSteps_JobReceiver(void);
    static void vAddSteps_JobRepeater(void);
    static void vAddSteps_JobReport(void);
    static void vAddSteps_JobSelectTable(void);
    static void vAddSteps_JobSink(void);
    static void vAddSteps_JobSubExporter(void);
    static void vAddSteps_JobSubIngester(void);
    static void vAddSteps_JobSubLoader(void);
    static void vAddSteps_JobSubPublish(void);
    static void vAddSteps_JobSubPurger(void);
    static void vAddSteps_JobSubWalker(void);
    static void vAddSteps_JobTruncateTable(void);
    static void vAddSteps_JobWriteBuffer(void);
    static void vAddSteps_JobWriteRecord(void);
    static void vAddSteps_JobWriteRow(void);
  
    static void vAddSteps_Locals(void);
    static void vAddSteps_Lock(void);
    static void vAddSteps_LockTracker(void);
    static void vAddSteps_Logic(void);
  
    static void vAddSteps_Magnitude(void);
    static void vAddSteps_Memory(void);
  
    static void vAddSteps_Node(void);
  
    static void vAddSteps_Object(void);
  
    static void vAddSteps_Persistence(void);
    static void vAddSteps_Planner(void);
    static void vAddSteps_Process(void);
    static void vAddSteps_Push(void);
  
    static void vAddSteps_Queues(void);
  
    static void vAddSteps_Row(void);
    static void vAddSteps_Rstack(void);
  
    static void vAddSteps_Session(void);
    static void vAddSteps_Shell(void);
    static void vAddSteps_Slots(void);
    static void vAddSteps_Socket(void);
    static void vAddSteps_StepTiming(void);
    static void vAddSteps_Str(void);
  
    static void vAddSteps_Table(void);
    static void vAddSteps_TablePartition(void);
    static void vAddSteps_Task(void);
    static void vAddSteps_Test(void);
    static void vAddSteps_Thread(void);
    static void vAddSteps_Timer(void);
    static void vAddSteps_To(void);
    static void vAddSteps_Trace(void);
    static void vAddSteps_Tx(void);
    static void vAddSteps_TxSet(void);
  
    static void vAddSteps_Utility(void);
  
    static void vAddSteps_Word(void);
  
    static void vAddSteps_Zero(void);
  
    /*------------------------------------------------------------------------*/
  
    #include "StepsSet.hpp"
  };
}

#include "StepMacros.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
