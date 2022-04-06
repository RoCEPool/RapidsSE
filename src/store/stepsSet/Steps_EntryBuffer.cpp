/*============================================================================*/
/* Steps_EntBuffer.cpp - entry buffer step group                              */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "IndexDef.hpp"
#include "Index.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_EntBuffer(void)
{
  vAddStep("entryBuffer.dump"_Stz,        &Step::vEntBufferDump);
  vAddStep("entryBuffer.index@"_Stz,      &Step::vEntBufferIndexFetch);
  vAddStep("entryBuffer.nextBuffer"_Stz,  &Step::vEntBufferNextBuffer);
  vAddStep("entryBuffer.partition@"_Stz,  &Step::vEntBufferPartitionFetch);
  vAddStep("entryBuffer.partition!"_Stz,  &Step::vEntBufferPartitionStore);
  vAddStep("entryBuffer::toEntBuffer"_Stz,&Step::vEntBufferToEntBuffer);
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.dump -- dump the index buffer                                  */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferDump(void)
{
  ENTRY();
  
  EntBuffer* pIndexBuffer = _pEntBuffer(m_pDataStack->pPopPtr(__func__));
  
  pIndexBuffer->vShow();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.index@ -- get index from entry buffer                          */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferIndexFetch(void)
{
  ENTRY();
  
  EntBuffer* pIndexBuffer = _pEntBuffer(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushPtr(pIndexBuffer->pIndex());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.nextBuffer -- get next buffer of this index buffer             */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferNextBuffer(void)
{
  ENTRY();
  
  EntBuffer* pIndexBuffer = _pEntBuffer(m_pDataStack->pPopPtr(__func__));
  
  if (pIndexBuffer->pNextEntBuffer()) {
    i8 nBuffer = pIndexBuffer->pNextEntBuffer()->iBufferNo();
    UNUSED(nBuffer);
  }
  
  m_pDataStack->vPushPtr(pIndexBuffer->pNextEntBuffer());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.partition@ -- get partition no of this index buffer            */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferPartitionFetch(void)
{
  ENTRY();
  
  EntBuffer* pIndexBuffer = _pEntBuffer(m_pDataStack->pPopPtr(__func__));

  m_pDataStack->vPushInt(pIndexBuffer->tPtnId().iAsI8());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer.partition! -- set partition no of this index buffer            */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferPartitionStore(void)
{
  ENTRY();
  
  EntBuffer* pIndexBuffer = _pEntBuffer(m_pDataStack->pPopPtr(__func__));

  LglPtn tPtnId;
  tPtnId = m_pDataStack->iPopInt(__func__);

  pIndexBuffer->vPtnId(tPtnId);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* entryBuffer::toEntBuffer -- convert string to entry buffer pointer       */
/*----------------------------------------------------------------------------*/
void Step::vEntBufferToEntBuffer(void)
{
  ENTRY();
  
  s48K sString;
  
  m_pDataStack->vPopStr(sString,__func__);
  
  EntBuffer* pEntBuffer = _pEntBuffer((void*)sString.iToI8());
  
  m_pDataStack->vPushPtr(pEntBuffer);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
