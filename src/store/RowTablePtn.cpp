/*============================================================================*/
/* RowTablePtn.cpp -- Row Table Partition                                     */
/*============================================================================*/

#include "BASE.h"

#include "RowCoder.hpp"
#include "HeadLine.hpp"

#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"

#include "RowBuffer.hpp"

#include "CrvBuffer.hpp"

#include "TablePtn.hpp"

#include "RowTablePtn.hpp"
#include "Table.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,RowTablePtn,CN_ROWTABLEPTN)
__aNewFunction(RowTablePtn,CN_ROWTABLEPTN)
__vDeleteFunction(RowTablePtn,CN_ROWTABLEPTN)
__aFooterFunction(RowTablePtn)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowTablePtn::RowTablePtn(Table* pTable, LglPtn tLglPtn)
    : Object(CN_ROWTABLEPTN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWTABLEPTN,_FOOTER_);

  m_pTable    = pTable;
  m_tLglPtn    = tLglPtn;

  m_pRootBuffer    = nullptr;
  m_pCurrentBuffer = nullptr;
  m_oRootBufferId.m_pvAsAddress = nullptr;

  _checkInitialised_(CN_ROWTABLEPTN);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWTABLEPTN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowTablePtn::~RowTablePtn(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWTABLEPTN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append a row buffer into the RowTablePtn                                   */
/*----------------------------------------------------------------------------*/
void RowTablePtn::vAppendBuffer(RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWTABLEPTN,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // if buffer has 0 entries (only head) then should be last buffer
  if (pRowBuffer->nRows() == 0) {
    delete pRowBuffer;
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark rowBuffer as permanent
  pRowBuffer->vIsPermanent(true);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the table in the buffer
  pRowBuffer->vTable_dbuff(m_pTable);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the root buffer
  if (m_pRootBuffer == nullptr) {
    ASSERT( ! m_pCurrentBuffer )
    m_oRootBufferId = pRowBuffer->oGetBufferId();
    m_pRootBuffer = pRowBuffer;
  }
  else {
#ifdef WITH_PAGING
    pRowBuffer->vNextLinkedBufferId(m_pCurrentBuffer->oNextLinkedBufferId());
    pRowBuffer->vPrevLinkedBufferId(m_pCurrentBuffer->oGetBufferId());
    m_pCurrentBuffer->vNextLinkedBufferId(pRowBuffer->oGetBufferId());
#else
//    pRowBuffer->vNextLinkedBufferId(m_pCurrentBuffer->oNextLinkedBufferId());
//    pRowBuffer->vPrevLinkedBufferId(m_pCurrentBuffer->oGetBufferId());
//    m_pCurrentBuffer->vNextLinkedBufferId(pRowBuffer->oGetBufferId());

    pRowBuffer->vNextLinkedBuffer(m_pCurrentBuffer->pNextLinkedBuffer());
    pRowBuffer->vPrevLinkedBuffer(m_pCurrentBuffer);
    m_pCurrentBuffer->vNextLinkedBuffer(pRowBuffer);
#endif
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // we are done with the current buffer, allow it to be paged out by dropping
  // the lock

  if(m_pCurrentBuffer)
    AM::pHashMap()->vDropKeyLock(m_pCurrentBuffer->oGetBufferId().m_pvAsAddress);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the current buffer
  m_pCurrentBuffer = pRowBuffer;
}

/*----------------------------------------------------------------------------*/
/* get the number of buffers in this partition                                */
/*----------------------------------------------------------------------------*/
i8 RowTablePtn::nBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWTABLEPTN,_FOOTER_);

  i8 nBufferCount = 0;

  if (m_pRootBuffer)
    nBufferCount = m_pRootBuffer->nLinkedBufferCount();

  return nBufferCount;
}

/*----------------------------------------------------------------------------*/
/* reset the current buffer ptr                                               */
/*----------------------------------------------------------------------------*/
void RowTablePtn::vCleanup(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWTABLEPTN,_FOOTER_);

#ifdef WITH_PAGING
  m_pCurrentBuffer = nullptr;
#endif
}

/*----------------------------------------------------------------------------*/
/* drop all paging locks                                                      */
/*----------------------------------------------------------------------------*/
void RowTablePtn::vDropAllPagingLocks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWTABLEPTN,_FOOTER_);

  if(m_pCurrentBuffer)
    AM::pHashMap()->vDropKeyLock(m_pCurrentBuffer->oGetBufferId().m_pvAsAddress);
}

/*----------------------------------------------------------------------------*/
/* dump the table                                                             */
/*----------------------------------------------------------------------------*/
void RowTablePtn::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWTABLEPTN,_FOOTER_);

  RowBuffer* pRowBuffer = m_pRootBuffer;

  while (pRowBuffer) {
    pRowBuffer->vShow(m_pTable->pStreamDef());
    pRowBuffer = pRowBuffer->pNextRowBuffer();
  }

}

/*----------------------------------------------------------------------------*/
/* get the partition number (in grid) of this table partition                 */
/*----------------------------------------------------------------------------*/
LglPtn RowTablePtn::tLglPtn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWTABLEPTN,_FOOTER_);

  return m_tLglPtn;
}

/*----------------------------------------------------------------------------*/
/* get the root row buffer                                                    */
/*----------------------------------------------------------------------------*/
RowBuffer* RowTablePtn::pRootRowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWTABLEPTN,_FOOTER_);

  return m_pRootBuffer;
}

/*----------------------------------------------------------------------------*/
/* get the root row buffer                                                    */
/*----------------------------------------------------------------------------*/
BufferId RowTablePtn::oRootRowBufferId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWTABLEPTN,_FOOTER_);

  return m_oRootBufferId;
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* RowTablePtn::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWTABLEPTN,_FOOTER_);

  return m_pTable;
}


/*----------------------------------------------------------------------------*/
/* truncate the table data                                                    */
/*----------------------------------------------------------------------------*/
void RowTablePtn::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWTABLEPTN,_FOOTER_);

  if (m_pRootBuffer)
    m_pRootBuffer->vTruncate();

  m_pRootBuffer    = nullptr;
  m_pCurrentBuffer = nullptr;
}

/*----------------------------------------------------------------------------*/
/* verify the table data of this partition                                    */
/*----------------------------------------------------------------------------*/
bool RowTablePtn::bVerifyTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWTABLEPTN,_FOOTER_);

  RowBuffer* pRowBuffer = m_pRootBuffer;
  bool bVerify = true;

  while (pRowBuffer) {
    bVerify = pRowBuffer->bVerify();
    if (! bVerify )
      break;
    pRowBuffer = pRowBuffer->pNextRowBuffer();
  }

  return bVerify;
}

/*----------------------------------------------------------------------------*/
/* visualize the table data of this partition                                 */
/*----------------------------------------------------------------------------*/
void RowTablePtn::vVisualizeTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWTABLEPTN,_FOOTER_);

  RowBuffer* pRowBuffer = m_pRootBuffer;

  while (pRowBuffer) {
    // print the decile of this buffer
    vTrace("%lld"_Stz, pRowBuffer->iDecile());
    pRowBuffer = pRowBuffer->pNextRowBuffer();
  }
  vTrace("\n"_Stz);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
