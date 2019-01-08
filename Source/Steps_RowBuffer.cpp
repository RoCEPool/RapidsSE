/*============================================================================*/
/* Steps_RowBuffer.cpp - row buffer step group                                */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RowBuffer(void)
{
  vAddStep("rowBuffer::new"_Stz,    &Step::vRowBufferNew);
  
  vAddStep("rowBuffer.dump"_Stz,   &Step::vRowBufferDump);
}

/*----------------------------------------------------------------------------*/
/* rowbuffer.dump -- dump the row buffer                                      */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferDump(MP_Task mpTask)
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
void Step::vRowBufferNew(MP_Task mpTask)
{
  ENTRY("vRowBufferNew"_Stz);
  
  EntryBuffer* pRowBuffer = new EntryBuffer;
  ASSERT(CN_ENTRYBUFFER == pRowBuffer->eClass());
  
  mpTask->m_pDataStack->vPushObject(pRowBuffer);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
