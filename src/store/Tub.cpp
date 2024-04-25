/*============================================================================*/
/* Tub.cpp -- Chain of Buffers with dangling support                          */
/*============================================================================*/

#include "BASE.h"

#include "Tub.hpp"
#include "IndexDef.hpp"
#include "Key.hpp"
#include "IndexTree.hpp"
#include "Ent.hpp"
#include "Ent_int.h"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Tub,CN_TUB)
__aNewFunction(Tub,CN_TUB)
__vDeleteFunction(Tub,CN_TUB)
__aFooterFunction(Tub)

/*----------------------------------------------------------------------------*/
/*-- Externs -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot a new row                                                            */
/*----------------------------------------------------------------------------*/
extern "C" void vExtern_TubAllotRowPds(void* aDynamic, char** aazStrArea, i8* pnMaxStrAreaLen) {

	Dynamic_T* pDynamicT = (Dynamic_T*)aDynamic;
	Tub* pTub = (Tub*)pDynamicT->m_pTub;

	try {
		RASSERT(CN_TUB == pTub->eGetClassNo());

		RowPix* pRowPix = pTub->pRowPix();

		Pds* pPdsRowBuffer = _pPds(pTub->pWorkingBuffer());

		if (! pPdsRowBuffer || pPdsRowBuffer->nFreeBytes() < C_2KB) {
			pPdsRowBuffer = new Pds(pRowPix);
			pTub->vAddBuffer(pPdsRowBuffer);
		}

		Row* pRowDst = pPdsRowBuffer->pAllotRow();

		if (pRowDst == nullptr) {
			pPdsRowBuffer = new Pds(pRowPix);
			pTub->vAddBuffer(pPdsRowBuffer);
			pRowDst = pPdsRowBuffer->pAllotRow();
			RASSERT(pRowDst);
		}

		*aazStrArea = (char*)pPdsRowBuffer->azDepositString_begin();
		*pnMaxStrAreaLen = pPdsRowBuffer->nFreeBytes();
		pDynamicT->m_xRowInfoPO[0].m_aPdsRowBuffer = pPdsRowBuffer;
		pDynamicT->m_xRowInfoTO[0].m_aRowCellArray = pRowDst->aCellArray();
	}
	catch (LowMemory xLowMemory) {
		pDynamicT->m_iErrorCode = EC_LOW_MEMORY;
		pDynamicT->m_xRowInfoPO[0].m_aPdsRowBuffer = nullptr;
		pDynamicT->m_xRowInfoTO[0].m_aRowCellArray = nullptr;
	}
}

/*----------------------------------------------------------------------------*/
/* allocate row string                                                        */
/*----------------------------------------------------------------------------*/
extern "C" void vExtern_TubRowStrSize(void* pTub, i8 nRowStrSize) {
  Tub::vRowStrSize((Tub*)pTub, nRowStrSize);
}

/*----------------------------------------------------------------------------*/
/* allocate row string                                                        */
/*----------------------------------------------------------------------------*/
extern "C" void vExtern_TubRowPix(void* aTub, i8 nColumns, void* pColumnInfo) {
  Tub* pTub = (Tub*)aTub;
	RowPix* pRowPix = RowPix::pRowPixNew(nColumns, (ColumnInfo*)pColumnInfo);
	pTub->vRowPix(pRowPix);
}

/*----------------------------------------------------------------------------*/
/* allocate row string                                                        */
/*----------------------------------------------------------------------------*/
void vTub_RowStrSize(void* pTub, i8 nRowStrSize) {
  Tub::vRowStrSize((Tub*)pTub, nRowStrSize);
}

/*----------------------------------------------------------------------------*/
/* allocate row string                                                        */
/*----------------------------------------------------------------------------*/
void vTub_RowPix(void* pTub, void* pRowPix) {
  Tub::vSetRowPix((Tub*)pTub, (RowPix*)pRowPix);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Tub::Tub(void)
: Object(CN_TUB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);

  m_pBufferAddrQueue = new PdsBufferQueue();

  m_pRowPix      = nullptr;
  
  _checkInitialised_(CN_TUB);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Tub::~Tub(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);

  delete m_pRowPix;

  delete m_pBufferAddrQueue;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair tubby                                                               */
/*----------------------------------------------------------------------------*/
void Tub::vRepair(Object* pObject)
{
  _static_
  
  UNUSED(pObject);
}

/*----------------------------------------------------------------------------*/
/* allot a new row in a pds                                                   */
/*----------------------------------------------------------------------------*/
void* Tub::aAllotRowPds(Tub* pTub, char** aazStrArea, i8* pnMaxStrAreaLen, i8* piErrorCode)
{
  _static_

  try {
    RASSERT(CN_TUB == pTub->eGetClassNo());

    RowPix* pRowPix = pTub->pRowPix();

    Pds* pPdsRowBuffer = _pPds(pTub->pWorkingBuffer());

    if (! pPdsRowBuffer || pPdsRowBuffer->nFreeBytes() < C_2KB) {
      pPdsRowBuffer = new Pds(pRowPix);
      pTub->vAddBuffer(pPdsRowBuffer);
    }

    Row* pRowDst = pPdsRowBuffer->pAllotRow();

    if (pRowDst == nullptr) {
      pPdsRowBuffer = new Pds(pRowPix);
      pTub->vAddBuffer(pPdsRowBuffer);
      pRowDst = pPdsRowBuffer->pAllotRow();
      RASSERT(pRowDst);
    }

    *aazStrArea = (char*)pPdsRowBuffer->azDepositString_begin();
    *pnMaxStrAreaLen = pPdsRowBuffer->nFreeBytes();
    return pRowDst->aCellArray();
  }
  catch (LowMemory xLowMemory) {
    *piErrorCode = EC_LOW_MEMORY;
    return nullptr;
  }
}

/*----------------------------------------------------------------------------*/
/* allot a new row in a pds                                                   */
/*----------------------------------------------------------------------------*/
Row* Tub::pAllotRow(Tub* pTub, char** aazStrArea, i8* pnMaxStrAreaLen, i8* piErrorCode)
{
	_static_

	try {
		RASSERT(CN_TUB == pTub->eGetClassNo());

		RowPix* pRowPix = pTub->pRowPix();

		Pds* pPdsRowBuffer = _pPds(pTub->pWorkingBuffer());

		if (! pPdsRowBuffer || pPdsRowBuffer->nFreeBytes() < C_2KB) {
			pPdsRowBuffer = new Pds(pRowPix);
			pTub->vAddBuffer(pPdsRowBuffer);
		}

		Row* pRowDst = pPdsRowBuffer->pAllotRow();

		if (pRowDst == nullptr) {
			pPdsRowBuffer = new Pds(pRowPix);
			pTub->vAddBuffer(pPdsRowBuffer);
			pRowDst = pPdsRowBuffer->pAllotRow();
			RASSERT(pRowDst);
		}

		*aazStrArea = (char*)pPdsRowBuffer->azDepositString_begin();
		*pnMaxStrAreaLen = pPdsRowBuffer->nFreeBytes();
		return pRowDst;
	}
	catch (LowMemory xLowMemory) {
		*piErrorCode = EC_LOW_MEMORY;
		return nullptr;
	}
}

/*----------------------------------------------------------------------------*/
/* allocate row string                                                        */
/*----------------------------------------------------------------------------*/
void Tub::vRowStrSize(Tub* pTub, i8 nRowStrSize)
{
  _static_
  
  RASSERT(CN_TUB == pTub->eGetClassNo());

  Pds* pPdsRowBuffer = _pPds(pTub->pWorkingBuffer());

  pPdsRowBuffer->vDepositString_end(nRowStrSize);

}

/*----------------------------------------------------------------------------*/
/* allocate row string                                                        */
/*----------------------------------------------------------------------------*/
void Tub::vSetRowPix(Tub* pTub, RowPix* pRowPix)
{
  _static_

  RASSERT(CN_TUB == pTub->eGetClassNo());

  pTub->vRowPix(pRowPix);

}

/*----------------------------------------------------------------------------*/
/* convert Pds into one or more csv tub buffers                               */
/*----------------------------------------------------------------------------*/
void Tub::vToCsv(Tub* pTub, Pds* pPds)
{
  _static_
  HV_Record hvRecord(pPds->pRowPix());

  CsvBuffer* pCsvBuffer = _pCsvBuffer(pTub->pWorkingBuffer());
  if (! pCsvBuffer) {
    pCsvBuffer = new CsvBuffer;
    pTub->vAddBuffer(pCsvBuffer);
  }

  i8 nSize = 0;
  Stz* szRecord = nullptr;

  PdsCursor* pCursor = new PdsCursor(pPds);
  while( ! pCursor->bIsCurrNull()) {
    Row* aRow = pCursor->pCurrRow();
    RASSERT(aRow);

    Stz* szRow = hvRecord->sz(aRow,true);
    nSize = szRow->nSize();

    if (pCsvBuffer->nFreeBytes() < nSize + 1) {
      pCsvBuffer = new CsvBuffer;
      pTub->vAddBuffer(pCsvBuffer);
    }

    szRecord = pCsvBuffer->szAllotRecord(nSize - 1);
    StrLib::vCopy(szRecord->azChar(), nSize, szRow->azChar());
    pCursor->vNext();
  }

  pCursor->m_pPds = nullptr;
  delete pCursor;

  if (szRecord)
    ((char*)szRecord)[nSize-1] = '\0';

  //vTrace("End: %s\n"_Stz,pCsvBuffer->szRecords());
  //vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add given buffer as first into buffer chain                                */
/*----------------------------------------------------------------------------*/
void Tub::vAddBuffer(DataBuffer* pDataBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);

  m_pBufferAddrQueue->vAddBuffer(pDataBuffer);

  return;
}

/*----------------------------------------------------------------------------*/
/* return a non-first/full buffer (if any)                                    */
/*----------------------------------------------------------------------------*/
DataBuffer* Tub::pCompletedBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);

  return m_pBufferAddrQueue->pCompletedBuffer();
}

/*----------------------------------------------------------------------------*/
/* return any first/final dangling buffer                                     */
/*----------------------------------------------------------------------------*/
DataBuffer* Tub::pDanglingBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);

  return m_pBufferAddrQueue->pDanglingBuffer();
}

/*----------------------------------------------------------------------------*/
/* is this tub empty?                                                         */
/*----------------------------------------------------------------------------*/
bool Tub::bIsEmpty(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);
  
  return m_pBufferAddrQueue->bIsEmpty();
}

/*----------------------------------------------------------------------------*/
/* get row pix                                                                */
/*----------------------------------------------------------------------------*/
RowPix* Tub::pRowPix(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);
    
  return m_pRowPix;
}

/*----------------------------------------------------------------------------*/
/* set row pix                                                                */
/*----------------------------------------------------------------------------*/
void Tub::vRowPix(RowPix* pRowPix)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);

  m_pRowPix = pRowPix;
}

/*----------------------------------------------------------------------------*/
/* return working buffer (tail)                                               */
/*----------------------------------------------------------------------------*/
DataBuffer* Tub::pWorkingBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);

  return m_pBufferAddrQueue->pDataBuffer();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
