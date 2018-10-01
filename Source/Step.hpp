/*============================================================================*/
/* Step.hpp -- Step                                                           */
/*============================================================================*/
#pragma once

namespace Tasks
{
  class Step
  {
  friend class Task;
  
    void vAddPrimitives_AccessPath(void);
    void vAddPrimitives_Accum(void);
    void vAddPrimitives_Address(void);
    void vAddPrimitives_Args(void);
    void vAddPrimitives_Assert(void);
    void vAddPrimitives_Astack(void);
    void vAddPrimitives_AttachMap(void);
  
    void vAddPrimitives_Bp(void);
  
    void vAddPrimitives_Cast(void);
    void vAddPrimitives_Class(void);
    void vAddPrimitives_Column(void);
    void vAddPrimitives_CommandProcessor(void);
    void vAddPrimitives_Comment(void);
    void vAddPrimitives_Common(void);
    void vAddPrimitives_Cpu(void);
    void vAddPrimitives_Cursor(void);
  
    void vAddPrimitives_DdlLock(void);
    void vAddPrimitives_Debug(void);
    void vAddPrimitives_Dic(void);
    void vAddPrimitives_DicLock(void);
    void vAddPrimitives_DicPath(void);
    void vAddPrimitives_DicStack(void);
    void vAddPrimitives_Dir(void);
    void vAddPrimitives_Docol(void);
    void vAddPrimitives_Dot(void);
    void vAddPrimitives_Dstack(void);
  
    void vAddPrimitives_Encode(void);
    void vAddPrimitives_EntryBuffer(void);
    void vAddPrimitives_Enter(void);
    void vAddPrimitives_Env(void);
    void vAddPrimitives_Error(void);
    void vAddPrimitives_Exit(void);
  
    void vAddPrimitives_FetchTest(void);
    void vAddPrimitives_File(void);
    void vAddPrimitives_Flow(void);
  
    void vAddPrimitives_Grid(void);
    void vAddPrimitives_GroupBy(void);
  
    void vAddPrimitives_Index(void);
    void vAddPrimitives_Input(void);
    void vAddPrimitives_Int(void);
  
    void vAddPrimitives_Job();
    void vAddPrimitives_JobAnalyzeIndex();
    void vAddPrimitives_JobClass();
    void vAddPrimitives_JobCreateIndex();
    void vAddPrimitives_JobFetchRows();
    void vAddPrimitives_JobGeneric();
    void vAddPrimitives_JobInsertRow();
    void vAddPrimitives_JobLoadTable();
    void vAddPrimitives_JobReceiver();
    void vAddPrimitives_JobStartCluster();
    void vAddPrimitives_JobTruncateTable();
  
    void vAddPrimitives_Loader(void);
    void vAddPrimitives_LoaderSub(void);
    void vAddPrimitives_Locals(void);
    void vAddPrimitives_Lock(void);
    void vAddPrimitives_Locks(void);
    void vAddPrimitives_Logic(void);
  
    void vAddPrimitives_Magnitude(void);
    void vAddPrimitives_Memory(void);
  
    void vAddPrimitives_Node(void);
  
    void vAddPrimitives_Object(void);
    void vAddPrimitives_OpFrame(void);
  
    void vAddPrimitives_PartitionScheduler(void);
    void vAddPrimitives_Persistence(void);
    void vAddPrimitives_Plan(void);
    void vAddPrimitives_PlanList(void);
    void vAddPrimitives_Planner(void);
    void vAddPrimitives_Process(void);
    void vAddPrimitives_Prompt(void);
    void vAddPrimitives_Push(void);
  
    void vAddPrimitives_Queue(void);
    void vAddPrimitives_Queues(void);
  
    void vAddPrimitives_RbCursor(void);
    void vAddPrimitives_Row(void);
    void vAddPrimitives_RowBuffer(void);
    void vAddPrimitives_RowCursor(void);
    void vAddPrimitives_Rstack(void);
  
    void vAddPrimitives_Scheduler(void);
    void vAddPrimitives_SEitf(void);
    void vAddPrimitives_SelCursor(void);
    void vAddPrimitives_Semaphore(void);
    void vAddPrimitives_ServerScheduler(void);
    void vAddPrimitives_Shell(void);
    void vAddPrimitives_Slots(void);
    void vAddPrimitives_Socket(void);
    void vAddPrimitives_Steps(void);
    void vAddPrimitives_Str(void);
  
    void vAddPrimitives_Table(void);
    void vAddPrimitives_TablePartition(void);
    void vAddPrimitives_Task(void);
    void vAddPrimitives_TaskCursor(void);
    void vAddPrimitives_Thread(void);
    void vAddPrimitives_Timer(void);
    void vAddPrimitives_To(void);
    void vAddPrimitives_Trace(void);
    void vAddPrimitives_Tx(void);
    void vAddPrimitives_TxSet(void);
  
    void vAddPrimitives_Vector(void);
  
    void vAddPrimitives_Utility(void);
  
    void vAddPrimitives_Word(void);
    void vAddPrimitives_WorkingBuffer(void);
  
    void vAddPrimitives_Zero(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
