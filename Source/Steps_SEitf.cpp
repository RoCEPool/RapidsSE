/*============================================================================*/
/* Steps_SEitf.cpp - SEitf step group                                         */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// seitf: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_SEitf(void)
{
  vAddStep("seitf.commandProcessor"_Stz, &Task::vSEitfCommandProcessor, false);
  vAddStep("seitf.dicStack"_Stz,         &Task::vSEitfDicStack,         false);
  vAddStep("seitf.display@"_Stz,         &Task::vSEitfDisplayFetch,     false);
  vAddStep("seitf.display!"_Stz,         &Task::vSEitfDisplayStore,     false);
  vAddStep("seitf.dump"_Stz,             &Task::vSEitfDump,             false);
  vAddStep("seitf.isExiting"_Stz,        &Task::vSEitfIsExiting,        false);
  vAddStep("seitf.lastBuffers@"_Stz,     &Task::vSEitfLastBuffersFetch, false);
  vAddStep("seitf.lastBuffers!"_Stz,     &Task::vSEitfLastBuffersStore, false);
  vAddStep("seitf.pid"_Stz,              &Task::vSEitfPid,              false);
  vAddStep("seitf.rowsInvalid!"_Stz,     &Task::vSEitfRowsInvalidStore, false);
  vAddStep("seitf.rowsValid!"_Stz,       &Task::vSEitfRowsValidStore,   false);
  vAddStep("seitf.slot@"_Stz,            &Task::vSEitfSlotFetch,        false);
  vAddStep("seitf.table@"_Stz,           &Task::vSEitfTableFetch,       false);
  vAddStep("seitf.tableRef"_Stz,         &Task::vSEitfTableRef,         false);
  vAddStep("seitf.tableDeref"_Stz,       &Task::vSEitfTableDeref,       false);
  vAddStep("seitf.tx@"_Stz,              &Task::vSEitfTxFetch,          false);
  vAddStep("seitf.tx!"_Stz,              &Task::vSEitfTxStore,          false);
}

/*----------------------------------------------------------------------------*/
/* seitf.commandProcessor -- get SEitf command processor                      */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCommandProcessor(MP_Task mpTask)
{
  ENTRY("vSEitfDicStack"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.dicStack"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  mpTask->m_pDataStack->vPushObject(pSEitf->m_pCommandProcessor);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.dicStack -- get SEitf dic stack                                      */
/*----------------------------------------------------------------------------*/
void Task::vSEitfDicStack(MP_Task mpTask)
{
  ENTRY("vSEitfDicStack"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.dicStack"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  mpTask->m_pDataStack->vPushObject(pSEitf->pPathStack());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.display@ -- get display from the SE interface                        */
/*----------------------------------------------------------------------------*/
void Task::vSEitfDisplayFetch(MP_Task mpTask)
{
  ENTRY("vSEitfDisplayFetch"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.display@"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  mpTask->m_pDataStack->vPushInt(pSEitf->m_bDisplay ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.display! -- set display in the SE interface                          */
/*----------------------------------------------------------------------------*/
void Task::vSEitfDisplayStore(MP_Task mpTask)
{
  ENTRY("vSEitfDisplayStore"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.display!"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  i8 iDisplay;
  if ( ! mpTask->m_pDataStack->bPop(iDisplay) )
    THROW_INVALID_DSTACK("seitf.displayOff"_Stz);
  
  pSEitf->m_bDisplay = iDisplay == 1 ? true : false;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.dump -- dump of SE interface                                         */
/*----------------------------------------------------------------------------*/
void Task::vSEitfDump(MP_Task mpTask)
{
ENTRY("vSEitfDump"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.dump"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  pSEitf->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.isExiting -- is seitf exiting                                        */
/*----------------------------------------------------------------------------*/
void Task::vSEitfIsExiting(MP_Task mpTask)
{
  ENTRY("vSEitfIsExiting"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.isExiting"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  mpTask->m_pDataStack->vPushInt(pSEitf->m_bSeitfExiting);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.lastBuffers@ -- fetch the number of last buffers for a fetch         */
/*----------------------------------------------------------------------------*/
void Task::vSEitfLastBuffersFetch(MP_Task mpTask)
{
  ENTRY("vSEitfLastBuffersFetch"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
  THROW_INVALID_DSTACK("seitf.indexBuffer@"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  i8 nLastBuffers = pSEitf->nLastBuffers();
  
  mpTask->m_pDataStack->vPushInt(nLastBuffers);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.lastBuffers! -- store the number of last buffers for a fetch         */
/*----------------------------------------------------------------------------*/
void Task::vSEitfLastBuffersStore(MP_Task mpTask)
{
  ENTRY("vSEitfLastBuffersStore"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.lastBuffers!"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  i8 nLastBuffers;
  if ( ! mpTask->m_pDataStack->bPop(nLastBuffers) )
    THROW_INVALID_DSTACK("seitf.lastBuffers!"_Stz);
  
  pSEitf->vLastBuffers(nLastBuffers);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.pid -- get pid of SE interface                                       */
/*----------------------------------------------------------------------------*/
void Task::vSEitfPid(MP_Task mpTask)
{
  ENTRY("vSEitfPid"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.pid"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  mpTask->m_pDataStack->vPushInt(pSEitf->m_iPid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.rowsInvalid! -- store rows invalid in SE interface                   */
/*----------------------------------------------------------------------------*/
void Task::vSEitfRowsInvalidStore(MP_Task mpTask)
{
  ENTRY("vSEitfRowsInvalid"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.rowsInvalid!"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  i8 iRowsInvalid;
  if ( ! mpTask->m_pDataStack->bPop(iRowsInvalid) )
    THROW_INVALID_DSTACK("seitf.rowsInvalid!"_Stz);
  
  pSEitf->m_iRowsInvalid = iRowsInvalid;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.rowsValid! -- store rows valid in SE interface                       */
/*----------------------------------------------------------------------------*/
void Task::vSEitfRowsValidStore(MP_Task mpTask)
{
  ENTRY("vSEitfRowsVvalid"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.rowsValid!"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  i8 iRowsValid;
  if ( ! mpTask->m_pDataStack->bPop(iRowsValid) )
    THROW_INVALID_DSTACK("seitf.rowsInvalid!"_Stz);
  
  pSEitf->m_iRowsValid = iRowsValid;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.slot@ -- fetch slot of SE interface                                  */
/*----------------------------------------------------------------------------*/
void Task::vSEitfSlotFetch(MP_Task mpTask)
{
  ENTRY("vSEitfSlotFetch"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.slot@"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  mpTask->m_pDataStack->vPushInt(pSEitf->iSlot());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.table@ -- fetch table from SEitf                                     */
/*----------------------------------------------------------------------------*/
void Task::vSEitfTableFetch(MP_Task mpTask)
{
  ENTRY("vSEitfTableFetch"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.rsp!"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  mpTask->m_pDataStack->vPushObject(pSEitf->pCurrentTable());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.tableDeref -- store table in SEitf and inc reference                 */
/*----------------------------------------------------------------------------*/
void Task::vSEitfTableDeref(MP_Task mpTask)
{
  ENTRY("vSEitfRspStore"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.tableDeref"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Table* pTable = pSEitf->pCurrentTable();
  ASSERT(CN_TABLE == pTable->eClass());
  
  pTable->vRefDec();

  pSEitf->vCurrentTable(nullptr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.tableRef -- store table in SEitf and inc reference                   */
/*----------------------------------------------------------------------------*/
void Task::vSEitfTableRef(MP_Task mpTask)
{
  ENTRY("vSEitfTableRef"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.tableRef"_Stz);
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrTable;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("seitf.tableRef"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pTable->vRefInc();
  
  pSEitf->vCurrentTable(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.tx@ -- fetch tx of SE interface                                      */
/*----------------------------------------------------------------------------*/
void Task::vSEitfTxFetch(MP_Task mpTask)
{
  ENTRY("vSEitfTxFetch"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.tx@"_Stz);
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  
  Tx* pTx = pSEitf->pTx();
  
  mpTask->m_pDataStack->vPushObject(pTx);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.tx! -- store tx of SE interface                                      */
/*----------------------------------------------------------------------------*/
void Task::vSEitfTxStore(MP_Task mpTask)
{
  ENTRY("vSEitfTxStore"_Stz);
  
  Object* pPtrSEitf;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSEitf) )
    THROW_INVALID_DSTACK("seitf.tx!"_Stz);
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  
  Object* pPtrTx;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTx) )
    THROW_INVALID_DSTACK("seitf.tx!"_Stz);
  Tx* pTx = _pTx(pPtrTx);
  
  pSEitf->vTx(pTx);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
