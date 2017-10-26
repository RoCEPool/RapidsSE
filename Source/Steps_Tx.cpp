/*============================================================================*/
/* Steps_Tx.cpp - tx step grou                                                */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern Heap*           gpMediumHeap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;
extern TxCollection*   gpTxCollection;
extern TxSet*          gpTxSet;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// tx: /////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Tx(void)
{
  vAddPrimitive("tx::begin"_Stz,    &vTxBegin,        false);
  vAddPrimitive("tx.commit"_Stz,    &vTxCommit,       false);
  vAddPrimitive("tx.dump"_Stz,      &vTxDump,         false);
  vAddPrimitive("tx::dumpAll"_Stz,  &vTxDumpAll,      false);
  vAddPrimitive("tx.rollback"_Stz,  &vTxRollback,     false);
}

/*----------------------------------------------------------------------------*/
/* tx::begin -- begin a tx                                                    */
/*----------------------------------------------------------------------------*/
void Task::vTxBegin(MP_Task mpTask)
{
  ENTRY("vTxBegin"_Stz);
  
  Tx* pTx = gpTxSet->pAllocLocalTx();
  
  mpTask->m_pDataStack->vPushObject(pTx);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx.commit -- commit a tx                                                   */
/*----------------------------------------------------------------------------*/
void Task::vTxCommit(MP_Task mpTask)
{
  ENTRY("vTxCommit"_Stz);
  
  Object* pPtrTx;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTx) )
    THROW_INVALID_DSTACK("tx.commit"_Stz);
  Tx* pTx = _pTx(pPtrTx);
  
  pTx->vPhase(TP_COMMITTED);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx.dump -- dump a tx                                                       */
/*----------------------------------------------------------------------------*/
void Task::vTxDump(MP_Task mpTask)
{
ENTRY("vTxDump"_Stz);
  
  Object* pPtrTx;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTx) )
    THROW_INVALID_DSTACK("tx.dump"_Stz);
  Tx* pTx = _pTx(pPtrTx);
  
  pTx->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx::dumpAll -- dump all tx's                                               */
/*----------------------------------------------------------------------------*/
void Task::vTxDumpAll(MP_Task mpTask)
{
  ENTRY("vTxDumpAll"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tx.rollback -- rollback a tx                                               */
/*----------------------------------------------------------------------------*/
void Task::vTxRollback(MP_Task mpTask)
{
  ENTRY("vTxRollback"_Stz);
  
  Object* pPtrTx;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTx) )
    THROW_INVALID_DSTACK("tx.rollback"_Stz);
  Tx* pTx = _pTx(pPtrTx);
  
  pTx->vPhase(TP_ROLLEDBACK);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
