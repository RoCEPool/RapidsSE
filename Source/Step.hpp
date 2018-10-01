/*============================================================================*/
/* Step.hpp -- Step                                                           */
/*============================================================================*/
#pragma once

namespace Tasks
{
  class Step
  {
  friend class Task;
 
    static void vAddSteps(void);

    static void vAddPrimitives_AccessPath(void);
    static void vAddPrimitives_Accum(void);
    static void vAddPrimitives_Address(void);
    static void vAddPrimitives_Args(void);
    static void vAddPrimitives_Assert(void);
    static void vAddPrimitives_Astack(void);
    static void vAddPrimitives_AttachMap(void);
  
    static void vAddPrimitives_Bp(void);
  
    static void vAddPrimitives_Cast(void);
    static void vAddPrimitives_Class(void);
    static void vAddPrimitives_Column(void);
    static void vAddPrimitives_CommandProcessor(void);
    static void vAddPrimitives_Comment(void);
    static void vAddPrimitives_Common(void);
    static void vAddPrimitives_Cpu(void);
    static void vAddPrimitives_Cursor(void);
  
    static void vAddPrimitives_DdlLock(void);
    static void vAddPrimitives_Debug(void);
    static void vAddPrimitives_Dic(void);
    static void vAddPrimitives_DicLock(void);
    static void vAddPrimitives_DicPath(void);
    static void vAddPrimitives_DicStack(void);
    static void vAddPrimitives_Dir(void);
    static void vAddPrimitives_Docol(void);
    static void vAddPrimitives_Dot(void);
    static void vAddPrimitives_Dstack(void);
  
    static void vAddPrimitives_Encode(void);
    static void vAddPrimitives_EntryBuffer(void);
    static void vAddPrimitives_Enter(void);
    static void vAddPrimitives_Env(void);
    static void vAddPrimitives_Error(void);
    static void vAddPrimitives_Exit(void);
  
    static void vAddPrimitives_FetchTest(void);
    static void vAddPrimitives_File(void);
    static void vAddPrimitives_Flow(void);
  
    static void vAddPrimitives_Grid(void);
    static void vAddPrimitives_GroupBy(void);
  
    static void vAddPrimitives_Index(void);
    static void vAddPrimitives_Input(void);
    static void vAddPrimitives_Int(void);
  
    static void vAddPrimitives_Job();
    static void vAddPrimitives_JobAnalyzeIndex();
    static void vAddPrimitives_JobClass();
    static void vAddPrimitives_JobCreateIndex();
    static void vAddPrimitives_JobFetchRows();
    static void vAddPrimitives_JobGeneric();
    static void vAddPrimitives_JobInsertRow();
    static void vAddPrimitives_JobLoadTable();
    static void vAddPrimitives_JobReceiver();
    static void vAddPrimitives_JobStartCluster();
    static void vAddPrimitives_JobTruncateTable();
  
    static void vAddPrimitives_Loader(void);
    static void vAddPrimitives_LoaderSub(void);
    static void vAddPrimitives_Locals(void);
    static void vAddPrimitives_Lock(void);
    static void vAddPrimitives_Locks(void);
    static void vAddPrimitives_Logic(void);
  
    static void vAddPrimitives_Magnitude(void);
    static void vAddPrimitives_Memory(void);
  
    static void vAddPrimitives_Node(void);
  
    static void vAddPrimitives_Object(void);
    static void vAddPrimitives_OpFrame(void);
  
    static void vAddPrimitives_PartitionScheduler(void);
    static void vAddPrimitives_Persistence(void);
    static void vAddPrimitives_Plan(void);
    static void vAddPrimitives_PlanList(void);
    static void vAddPrimitives_Planner(void);
    static void vAddPrimitives_Process(void);
    static void vAddPrimitives_Prompt(void);
    static void vAddPrimitives_Push(void);
  
    static void vAddPrimitives_Queue(void);
    static void vAddPrimitives_Queues(void);
  
    static void vAddPrimitives_RbCursor(void);
    static void vAddPrimitives_Row(void);
    static void vAddPrimitives_RowBuffer(void);
    static void vAddPrimitives_RowCursor(void);
    static void vAddPrimitives_Rstack(void);
  
    static void vAddPrimitives_Scheduler(void);
    static void vAddPrimitives_SEitf(void);
    static void vAddPrimitives_SelCursor(void);
    static void vAddPrimitives_Semaphore(void);
    static void vAddPrimitives_ServerScheduler(void);
    static void vAddPrimitives_Shell(void);
    static void vAddPrimitives_Slots(void);
    static void vAddPrimitives_Socket(void);
    static void vAddPrimitives_Steps(void);
    static void vAddPrimitives_Str(void);
  
    static void vAddPrimitives_Table(void);
    static void vAddPrimitives_TablePartition(void);
    static void vAddPrimitives_Task(void);
    static void vAddPrimitives_TaskCursor(void);
    static void vAddPrimitives_Thread(void);
    static void vAddPrimitives_Timer(void);
    static void vAddPrimitives_To(void);
    static void vAddPrimitives_Trace(void);
    static void vAddPrimitives_Tx(void);
    static void vAddPrimitives_TxSet(void);
  
    static void vAddPrimitives_Vector(void);
  
    static void vAddPrimitives_Utility(void);
  
    static void vAddPrimitives_Word(void);
    static void vAddPrimitives_WorkingBuffer(void);
  
    static void vAddPrimitives_Zero(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
