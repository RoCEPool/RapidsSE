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

#include "Bucket.hpp"

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
RowTablePtn::RowTablePtn(Table* pTable, LglPtn tLglPtn, Bucket* pBucket)
: Object(CN_ROWTABLEPTN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWTABLEPTN,_FOOTER_);

  m_pTable    = pTable;
  m_pBucket   = pBucket;
  m_tLglPtn    = tLglPtn;

  m_pRootBuffer    = nullptr;
  m_pCurrentBuffer = nullptr;

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
  // mark crwBuffer as permanent
  pRowBuffer->vIsPermanent(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the table in the buffer
  pRowBuffer->vTable(m_pTable);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the root buffer
  if (m_pRootBuffer == nullptr) {
    ASSERT( ! m_pCurrentBuffer )
    m_pRootBuffer = pRowBuffer;
  }
  else {
    pRowBuffer->vNextLinkedBuffer(m_pCurrentBuffer->pNextLinkedBuffer());
    pRowBuffer->vPrevLinkedBuffer(m_pCurrentBuffer);
    m_pCurrentBuffer->vNextLinkedBuffer(pRowBuffer);
  }
  
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
