/*============================================================================*/
/* Bucket.cpp -- Bucket                                                       */
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

#include "CrvBuffer.hpp"

#include "TablePtn.hpp"
#include "Table.hpp"
#include "CrwTablePtn.hpp"
#include "RowTablePtn.hpp"
#include "RowBuffer.hpp"

#include "Bucket.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Bucket,CN_BUCKET)
__aNewFunction(Bucket,CN_BUCKET)
__vDeleteFunction(Bucket,CN_BUCKET)
__aFooterFunction(Bucket)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Bucket::Bucket(Table* pTable, LglPtn tLglPtn, eCN eKind)
: Object(CN_BUCKET)
//, m_okLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKET,_FOOTER_);

  m_pTable = pTable;
  m_tLglPtn = tLglPtn;
  m_eKind = eKind;

  switch (m_eKind) {
    case CN_ROWTABLEPTN:
      m_pRowTablePtn = new RowTablePtn(m_pTable,m_tLglPtn,this);
      break;
    case CN_CRWTABLEPTN:
      m_pCrwTablePtn = new CrwTablePtn(m_pTable,m_tLglPtn,this);
      break;
    default: NEVER_REACH;
  }

  _checkInitialised_(CN_BUCKET);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_BUCKET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Bucket::~Bucket(void) noexcept(false)
{
_precondition_(_ADDRESS_,_MAGIC_,CN_BUCKET,_FOOTER_);

  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      delete m_pCrwTablePtn;
      break;
    case CN_ROWTABLEPTN:
      delete m_pRowTablePtn;
      break;
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair the bucket                                                          */
/*----------------------------------------------------------------------------*/
void Bucket::vRepair(Object* pObject)
{
  _static_

  Bucket* pBucket = _pBucket(pObject);

  // delete the bucket
  delete pBucket;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/




/*----------------------------------------------------------------------------*/
/* get the partition number (in grid) of this table partition                 */
/*----------------------------------------------------------------------------*/
LglPtn Bucket::tLglPtn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKET,_FOOTER_);

  return m_tLglPtn;
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* Bucket::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKET,_FOOTER_);

  return m_pTable;
}


/*----------------------------------------------------------------------------*/
/*                                CRW/ROW                                     */
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* append a buffer into the TablePtn                                          */
/*----------------------------------------------------------------------------*/
void Bucket::vAppendBuffer(DataBuffer* pDataBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKET,_FOOTER_);

  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      m_pCrwTablePtn->vAppendBuffer(_pCrwBuffer(pDataBuffer));
      break;
    case CN_ROWTABLEPTN:
      m_pRowTablePtn->vAppendBuffer(_pRowBuffer(pDataBuffer));
      break;
    default: NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* get the number of buffers in this partition                                */
/*----------------------------------------------------------------------------*/
i8 Bucket::nBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKET,_FOOTER_);

  i8 nBufferCount;

  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      nBufferCount = m_pCrwTablePtn->nBuffers();
      break;
    case CN_ROWTABLEPTN:
      nBufferCount = m_pRowTablePtn->nBuffers();
      break;
    default:
      NEVER_REACH;
  }

  return nBufferCount;
}

/*----------------------------------------------------------------------------*/
/* dump the table                                                             */
/*----------------------------------------------------------------------------*/
void Bucket::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKET,_FOOTER_);

  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      m_pCrwTablePtn->vDump();
      break;
    case CN_ROWTABLEPTN:
      m_pRowTablePtn->vDump();
      break;
    default:
      NEVER_REACH;
  }
}





/*----------------------------------------------------------------------------*/
/* insert entries into given index for all rows in this partition             */
/*----------------------------------------------------------------------------*/
bool Bucket::bPopulateIndexTree(IndexTree *pIndexTree)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKET,_FOOTER_);


  bool b;

  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      b = m_pCrwTablePtn->bPopulateIndexTree(pIndexTree);
      break;
    case CN_ROWTABLEPTN:
      NEVER_REACH;
      break;
    default:
      NEVER_REACH;
  }

  return b;
}

/*----------------------------------------------------------------------------*/
/* get the root crw buffer                                                    */
/*----------------------------------------------------------------------------*/
CrwBuffer* Bucket::pRootCrwBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKET,_FOOTER_);

  CrwBuffer* pRootBuffer;

  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      pRootBuffer = m_pCrwTablePtn->pRootCrwBuffer();
      break;
    case CN_ROWTABLEPTN:
      NEVER_REACH
      break;
    default: NEVER_REACH;
  }

  return pRootBuffer;
}

/*----------------------------------------------------------------------------*/
/* get the root buffer                                                        */
/*----------------------------------------------------------------------------*/
DataBuffer* Bucket::pRootBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKET,_FOOTER_);

  DataBuffer* pRootBuffer;

  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      pRootBuffer = m_pCrwTablePtn->pRootCrwBuffer();
      break;
    case CN_ROWTABLEPTN:
      pRootBuffer = m_pRowTablePtn->pRootRowBuffer();
      break;
    default: NEVER_REACH;
  }

  return pRootBuffer;
}

/*----------------------------------------------------------------------------*/
/* get the root row buffer                                                    */
/*----------------------------------------------------------------------------*/
RowBuffer* Bucket::pRootRowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKET,_FOOTER_);

  RowBuffer* pRootBuffer;

  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      NEVER_REACH
      break;
    case CN_ROWTABLEPTN:
      pRootBuffer = m_pRowTablePtn->pRootRowBuffer();
      break;
    default: NEVER_REACH;
  }

  return pRootBuffer;
}



/*----------------------------------------------------------------------------*/
/* truncate the table data                                                    */
/*----------------------------------------------------------------------------*/
void Bucket::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKET,_FOOTER_);

  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      m_pCrwTablePtn->vTruncate();
      break;
    case CN_ROWTABLEPTN:
      m_pRowTablePtn->vTruncate();
      break;
    default: NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* truncate the index data for given index                                    */
/*----------------------------------------------------------------------------*/
void Bucket::vTruncateIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKET,_FOOTER_);
  NEVER_REACH;
  /*
  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      m_pCrwTablePtn->vTruncateIndex(pIndex);
      break;
    case CN_ROWTABLEPTN:
      NEVER_REACH;
      break;
    default: NEVER_REACH;
  }*/
}

/*----------------------------------------------------------------------------*/
/* verify the table data of this partition                                    */
/*----------------------------------------------------------------------------*/
bool Bucket::bVerifyTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKET,_FOOTER_);

  bool bVerify;

  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      bVerify = m_pCrwTablePtn->bVerifyTable();
      break;
    case CN_ROWTABLEPTN:
      bVerify = m_pRowTablePtn->bVerifyTable();
      break;
    default:
      NEVER_REACH;
  }

  return bVerify;
}

/*----------------------------------------------------------------------------*/
/* analyze table                                                              */
/*----------------------------------------------------------------------------*/
void Bucket::vAnalyze(i8& nBuffers, i8& nRows)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKET,_FOOTER_);

  CrwBuffer* pCrwBuffer = pRootCrwBuffer();

  if (pCrwBuffer)
    pCrwBuffer->vAnalyze(nBuffers, nRows);

}

/*----------------------------------------------------------------------------*/
/* visualize the table data of this partition                                 */
/*----------------------------------------------------------------------------*/
void Bucket::vVisualizeTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUCKET,_FOOTER_);

  switch (m_eKind) {
    case CN_CRWTABLEPTN:
      m_pCrwTablePtn->vVisualizeTable();
      break;
    case CN_ROWTABLEPTN:
      m_pRowTablePtn->vVisualizeTable();

      break;
    default: NEVER_REACH;
  }
}


/*----------------------------------------------------------------------------*/
/* dump the table data of this partition                                      */
/*----------------------------------------------------------------------------*/
Stz* Bucket::szDumpContents(i8 i_Bucket)
{
  i8 nBuffers = m_pCrwTablePtn->nBuffers();
  s4K sDumpBuffer = ""_Stz;
  if (nBuffers > 0) {
    sDumpBuffer.vCatSprint("  PtnId:%lld   Bkt:%lld   nBuffers:%lld\n"_Stz, m_tLglPtn.iAsI8(), i_Bucket, nBuffers);
    for (i8 i_Buffer = 0; i_Buffer < nBuffers; i_Buffer++) {
      i8 nRows = m_pCrwTablePtn->pRootCrwBuffer()->nRows();
      sDumpBuffer.vCatSprint("     Buffer:%lld   PtnId:%lld  Bkt:%lld   nRows:%lld\n"_Stz, i_Buffer, m_tLglPtn.iAsI8(), i_Bucket, nRows);
    }
  }
  return sDumpBuffer;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

