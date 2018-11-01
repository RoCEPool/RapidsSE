/*============================================================================*/
/* Steps_RowBuffer.cpp - row buffer step group                                */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// rowbuffer: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RowBuffer(void)
{
  vAddStep("rowBuffer::new"_Stz,    &Task::vRowBufferNew);
  
  vAddStep("rowBuffer.dump"_Stz,   &Task::vRowBufferDump);
}

/*----------------------------------------------------------------------------*/
/* rowbuffer.dump -- dump the row buffer                                      */
/*----------------------------------------------------------------------------*/
void Task::vRowBufferDump(MP_Task mpTask)
{
  ENTRY("vRowBufferDump"_Stz);
  
  Object* pPtrRowBuffer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRowBuffer) )
    THROW_INVALID_DSTACK("rowbuffer.dump"_Stz);
  EntryBuffer* pRowBuffer = (EntryBuffer*)_pEntryBuffer(pPtrRowBuffer);

  Entry* pRow = pRowBuffer->pFirstEntry();
  
  while (pRow) {
    pRow->vContentDump();
    pRow = pRowBuffer->pNextEntry(pRow, 0);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowbuffer::new -- new a row buffer                                          */
/*----------------------------------------------------------------------------*/
void Task::vRowBufferNew(MP_Task mpTask)
{
  ENTRY("vRowBufferNew"_Stz);
  
  EntryBuffer* pRowBuffer = new EntryBuffer;
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());
  
  mpTask->m_pDataStack->vPushObject(pRowBuffer);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
