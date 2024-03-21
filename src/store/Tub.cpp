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
		pDynamicT->m_xRowInfoPO.m_aPdsBuffer = pPdsRowBuffer;
		pDynamicT->m_xRowInfoTO.m_aRowCellArray = pRowDst->aCellArray();
	}
	catch (LowMemory xLowMemory) {
		pDynamicT->m_iErrorCode = EC_LOW_MEMORY;
		pDynamicT->m_xRowInfoPO.m_aPdsBuffer = nullptr;
		pDynamicT->m_xRowInfoTO.m_aRowCellArray = nullptr;
	}
}

/*----------------------------------------------------------------------------*/
/* create a tub                                                               */
/*----------------------------------------------------------------------------*/
extern "C" void vExtern_TubCreate(void* pDynamic, i8 nCols, void* pColumnInfo) {
	return Tub::vTubCreate((Dynamic_T*)pDynamic, nCols, (ColumnInfo*)pColumnInfo);
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

  m_pBufferIdQueue = new PdsBufferIdQueue();

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

  delete m_pBufferIdQueue;
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
/* allot a new row in a pds                                                   */
/*----------------------------------------------------------------------------*/
void Tub::vTubCreate(Dynamic_T* pDynamicT, i8 nCols, ColumnInfo* pColumnInfo)
{
	_static_

	Tub* pTub = (Tub*)pDynamicT->m_pTub;
	Row* pRow;
	RowPix* pRowPix = RowPix::pRowPixNew(nCols, pColumnInfo);
	Tub::vSetRowPix(pTub, pRowPix);

	//struct SR_TO* pSR_TO;
	//struct SR_PL* pSR_PL;

	// - - - - - - - - - - - - - -
	// payload traversal
	//pDynamicT->m_xIndexInfoPL.m_aEntInt = (struct Ent_int*)aExtern_IndexTreeBegin(
	//				pDynamicT->m_xIndexInfoPL.m_aIndexTree, &pDynamicT->m_xIndexInfoPL.m_aEntBuffer);
	pDynamicT->m_xIndexInfoPL.m_aEntInt = (Ent_int*)IndexTree::aIndexBegin((IndexTree*)pDynamicT->m_xIndexInfoPL.m_aIndexTree,
																																			(EntBuffer**) &pDynamicT->m_xIndexInfoPL.m_aEntBuffer);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// traverse the index

	char * azStringDyn;
	i8 nMaxStrAreaLen;

	while (pDynamicT->m_xIndexInfoPL.m_aEntInt) {

		pDynamicT->m_bProcessRow = true;

		pDynamicT->m_xRowInfoPT.m_acStrArea = pDynamicT->m_xRowInfoPT.m_acStrAreaSav;
		pDynamicT->m_aiPayload = (i8 *) ((char *) pDynamicT->m_xIndexInfoPL.m_aEntInt
																	 + sizeof(struct Ent_int)
																	 + ((struct Ent_int*)pDynamicT->m_xIndexInfoPL.m_aEntInt)->m_iIntElm0
																	 + 8);

	  i8 iElemSize = 0;

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		// allocate a row from the tub/pot
		//aDynamicT->m_xRowInfoTO.m_aRowCellArray = (byte*)aExtern_TubAllotRowPds((void*)aDynamicT->m_pTub,&azStringDyn,&nMaxStrAreaLen,&aDynamicT->m_iErrorCode);
		pRow = Tub::pAllotRow(pTub,&azStringDyn,&nMaxStrAreaLen, &pDynamicT->m_iErrorCode);

		Pds* pPdsRowBuffer = _pPds(pTub->pWorkingBuffer());

		azStringDyn = (char*)pPdsRowBuffer->azDepositString_begin();
		nMaxStrAreaLen = pPdsRowBuffer->nFreeBytes();

		// check for error
		if (pDynamicT->m_iErrorCode != (i8) EC_NONE) {
			return;
		}

	  //pSR_TO = (struct SR_TO *) pDynamicT->m_xRowInfoTO.m_aRowCellArray;

	  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	  // column 0

	  char* szSrcStz = ((char*)(&(((struct Ent_int*)pDynamicT->m_xIndexInfoPL.m_aEntInt)->m_iIntElm0)) + 8 + iElemSize);

		if (*(i8*)szSrcStz == C_DATUM_NULL) {
			//pSR_TO->C0_C0_COL0.m_iAsI8 = C_DATUM_NULL;
			pRow->vColVal_int(0,C_DATUM_NULL);
			iElemSize += 8;
		}
		else {
			char* szDstStz = azStringDyn;

			//i8 iDstBufLen = pDynamicT->m_xRowInfoTO.m_nStringAreaSize;

			i8 nSrcSize = StrLib::iWordFlipBack(szDstStz, nMaxStrAreaLen, szSrcStz);

			// save the string offset
			//m8* amCellAddress = (m8*) &pSR_TO->C0_C0_COL0.m_iAsI8;
			//i8 iOffset = - nDifference(azStringDyn, amCellAddress);
			//pSR_TO->C0_C0_COL0.m_iAsI8 = iOffset;
			pRow->vColVal_ofs(0,(Stz*)azStringDyn);

			// Store the len of string copied
			i8 iDstLen = strlen(szDstStz) + 1;

			// update the tub string size
			pDynamicT->m_xRowInfoTO.m_nStringAreaSize -= iDstLen;
			pDynamicT->m_xRowInfoTO.m_acStrArea += iDstLen;

			// Add SrcSize to ElemSize since payload keys are on 8 byte boundries
			// SrcSize is rounded up to an 8 byte value
			iElemSize += nSrcSize;

			// Below function uses nSrcSize to update Pds
			// So replace nSrcSize with iDstLen
			nSrcSize = iDstLen;

			// Take string size bytes from Head area of Pds
			//vExtern_TubRowStrSize(aDynamicT->m_pTub,nSrcSize);
			Tub::vRowStrSize(pTub, nSrcSize);

			// update string address to point to the next string area in the pds
			azStringDyn = azStringDyn + nSrcSize;
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// column 1

	szSrcStz = ((char*)(&(((struct Ent_int*)pDynamicT->m_xIndexInfoPL.m_aEntInt)->m_iIntElm0)) + 8 + iElemSize);

	if (*(i8*)szSrcStz == C_DATUM_NULL) {
	  //pSR_TO->C1_C1_COL1.m_iAsI8 = C_DATUM_NULL;
		pRow->vColVal_int(1,C_DATUM_NULL);
	  iElemSize += 8;
	}
	else {
		char* szDstStz = azStringDyn;

		//i8 iDstBufLen = pDynamicT->m_xRowInfoTO.m_nStringAreaSize;

		i8 nSrcSize = StrLib::iWordFlipBack(szDstStz, nMaxStrAreaLen, szSrcStz);

		// save the string offset
		//m8* amCellAddress = (m8*) &pSR_TO->C1_C1_COL1.m_iAsI8;
		//i8 iOffset = - nDifference(azStringDyn, amCellAddress);
		//pSR_TO->C1_C1_COL1.m_iAsI8 = iOffset;
		pRow->vColVal_ofs(1,(Stz*)azStringDyn);

		// Store the len of string copied
		i8 iDstLen = strlen(szDstStz) + 1;

		// update the tub string size
		pDynamicT->m_xRowInfoTO.m_nStringAreaSize -= iDstLen;
		pDynamicT->m_xRowInfoTO.m_acStrArea += iDstLen;

		// Add SrcSize to ElemSize since payload keys are on 8 byte boundries
		// SrcSize is rounded up to an 8 byte value
		iElemSize += nSrcSize;

		// Below function uses nSrcSize to update Pds
		// So replace nSrcSize with iDstLen
		nSrcSize = iDstLen;

		// Take string size bytes from Head area of Pds
		Tub::vRowStrSize(pTub, nSrcSize);

		// update string address to point to the next string area in the pds
		azStringDyn = azStringDyn + nSrcSize;
	}
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// column 2

	//pSR_TO->C2_C2_COL2.m_iAsI8 = pDynamicT->m_aiPayload[0];
	pRow->vColVal_int(2,pDynamicT->m_aiPayload[0]);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// column 3

	//pSR_TO->C3_C3_COL3.m_iAsI8 = aDynamicT->m_aiPayload[1];
	pRow->vColVal_int(3,pDynamicT->m_aiPayload[1]);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// column 4

	//pSR_TO->C4_C4_COL4.m_iAsI8 = aDynamicT->m_aiPayload[2];
	pRow->vColVal_int(4,pDynamicT->m_aiPayload[2]);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// column 5

	//pSR_TO->C5_C5_COL5.m_iAsI8 = aDynamicT->m_aiPayload[3];
	pRow->vColVal_int(5,pDynamicT->m_aiPayload[3]);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// column 6

	//pSR_TO->C6_C6_COL6.m_iAsI8 = aDynamicT->m_aiPayload[4];
	pRow->vColVal_int(6,pDynamicT->m_aiPayload[4]);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// column 7

	//pSR_TO->C7_C7_COL7.m_iAsI8 = aDynamicT->m_aiPayload[5];
	pRow->vColVal_int(7,pDynamicT->m_aiPayload[5]);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// column 8

	//pSR_TO->C8_C8_COL8.m_iAsI8 = aDynamicT->m_aiPayload[6];
	pRow->vColVal_int(8,pDynamicT->m_aiPayload[6]);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// column 9

	//pSR_TO->C9_C9_COL9.m_iAsI8 = aDynamicT->m_aiPayload[7];
	pRow->vColVal_int(9,pDynamicT->m_aiPayload[7]);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// column 10

	//pSR_TO->C10_C10_COL10.m_iAsI8 = aDynamicT->m_aiPayload[8];
	pRow->vColVal_int(10,pDynamicT->m_aiPayload[8]);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// move to next entry
	//aDynamicT->m_xIndexInfoPL.m_aEntInt = (struct Ent_int*)aExtern_EntBufferNextEntry(
	//				&aDynamicT->m_xIndexInfoPL.m_aEntBuffer, aDynamicT->m_xIndexInfoPL.m_aEntInt);
	pDynamicT->m_xIndexInfoPL.m_aEntInt = (struct Ent_int*)EntBuffer::aGetNextEntry(
					(EntBuffer **) &pDynamicT->m_xIndexInfoPL.m_aEntBuffer,
					(Ent *) pDynamicT->m_xIndexInfoPL.m_aEntInt);
	}

	return;
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

  m_pBufferIdQueue->vAddBuffer(pDataBuffer);

  return;
}

/*----------------------------------------------------------------------------*/
/* return a non-first/full buffer (if any)                                    */
/*----------------------------------------------------------------------------*/
DataBuffer* Tub::pCompletedBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);

  return m_pBufferIdQueue->pCompletedBuffer();
}

/*----------------------------------------------------------------------------*/
/* return any first/final dangling buffer                                     */
/*----------------------------------------------------------------------------*/
DataBuffer* Tub::pDanglingBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);

  return m_pBufferIdQueue->pDanglingBuffer();
}

/*----------------------------------------------------------------------------*/
/* is this tub empty?                                                         */
/*----------------------------------------------------------------------------*/
bool Tub::bIsEmpty(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TUB,_FOOTER_);
  
  return m_pBufferIdQueue->bIsEmpty();
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

  return m_pBufferIdQueue->pDataBuffer();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
