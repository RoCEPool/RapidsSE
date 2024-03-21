/*============================================================================*/
/* Pot.cpp -- Working Pot of Buffers                                          */
/*============================================================================*/

#include "BASE.h"

#include "LglPtnMap.hpp"
#include "Pot.hpp"



/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Pot,CN_POT)
__aNewFunction(Pot,CN_POT)
__vDeleteFunction(Pot,CN_POT)
__aFooterFunction(Pot)

/*----------------------------------------------------------------------------*/
/*-- Externs -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot a new row                                                            */
/*----------------------------------------------------------------------------*/
extern "C" void* aExtern_PotAllotRowPds(void* pPot, i8 iKeyHash, char** aazStrArea, i8* pnMaxStrAreaLen, i8* piErrorCode) {
  return Pot::aAllotRowPds((Pot*)pPot, iKeyHash, (char**)aazStrArea, pnMaxStrAreaLen, piErrorCode);
}

/*----------------------------------------------------------------------------*/
/* allot a new row                                                            */
/*----------------------------------------------------------------------------*/
extern "C" void vExtern_PotAllotRowPds(void* aDynamicT, byte* aMixIn, i8 iMixInLen, char** aazStrArea, i8* pnMaxStrAreaLen) {

	Dynamic_T* pDynamic = (Dynamic_T*)aDynamicT;
	Pot* pPot = (Pot*)pDynamic->m_pPot;
	i8 iKeyHash = HshLib::iHash(aMixIn,iMixInLen);
	LglPtnMap* pPtnMap = AM::pLglPtnMap();
	LglPtn tLglPtn = pPtnMap->tHashToLglPtnId(iKeyHash);

	try {
		Row* aDstRow;



		RowPix* pRowPix = pPot->m_pRowPix;

		// get pds row buffer for given partition (new a first buffer if ncessary)
		Pds* pPdsRowBuffer = _pPds(pPot->pBuffer(tLglPtn));
		if (! pPdsRowBuffer || pPdsRowBuffer->nFreeBytes() < C_2KB) {
			pPdsRowBuffer = new Pds(pRowPix);
			pPot->vAddBuffer(tLglPtn,pPdsRowBuffer);
		}

		// new a row in pds row buffer (new another pds row buffer if necessary)
		aDstRow = pPdsRowBuffer->pAllotRow();
		if (! aDstRow) {
			pPdsRowBuffer = new Pds(pRowPix);
			pPot->vAddBuffer(tLglPtn,pPdsRowBuffer);
			aDstRow = pPdsRowBuffer->pAllotRow();
			RASSERT(aDstRow);
		}
		*aazStrArea = (char*)pPdsRowBuffer->azDepositString_begin();
		*pnMaxStrAreaLen = pPdsRowBuffer->nFreeBytes();

		pPdsRowBuffer->vLglPtn(tLglPtn);

		pDynamic->m_xRowInfoPO.m_aRowCellArray = aDstRow->aCellArray();
		pDynamic->m_xRowInfoPO.m_aPdsBuffer = pPdsRowBuffer;
	}
	catch (LowMemory xLowMemory) {
		pDynamic->m_iErrorCode = EC_LOW_MEMORY;
	}
}

/*----------------------------------------------------------------------------*/
/* allocate row string                                                        */
/*----------------------------------------------------------------------------*/
extern "C" void vExtern_PotRowStrSize(void* aDynamicT, i8 nRowStrSize) {
  //Pot::vRowStrSize((Pot*)pPot, iKeyHash, nRowStrSize);
	Dynamic_T* pDynamicT = (Dynamic_T*)aDynamicT;
	Pot* pPot = (Pot*)pDynamicT->m_pPot;

	RASSERT(CN_POT == pPot->eGetClassNo());

	//LglPtnMap* pPtnMap = AM::pLglPtnMap();
	//i8 iKeyHsh = HshLib::iHash((byte*)&iMixIn,sizeof(iMixIn));

	Pds* pPdsRowBuffer = _pPds(pDynamicT->m_xRowInfoPO.m_aPdsBuffer);

	pPdsRowBuffer->vDepositString_end(nRowStrSize);
}

/*----------------------------------------------------------------------------*/
/* allocate row string                                                        */
/*----------------------------------------------------------------------------*/
extern "C" void vExtern_PotRowPix(void* aPot, i8 nColumns, void* pColumnInfo) {
	Pot* pPot = (Pot*)aPot;
	RowPix* pRowPix = RowPix::pRowPixNew(nColumns, (ColumnInfo*)pColumnInfo);
	pPot->vRowPix(pRowPix);
}

/*----------------------------------------------------------------------------*/
/* allocate row string                                                        */
/*----------------------------------------------------------------------------*/
void vPot_RowStrSize(void* pPot, i8 iKeyHash, i8 nRowStrSize) {
  Pot::vRowStrSize((Pot*)pPot, iKeyHash, nRowStrSize);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Pot::Pot(void)
: Object(CN_POT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);
  
  m_pRowPix = nullptr;

  m_iSynthKey = 0;
  m_pPdsOfBuffers = new Pds_t<PotPdsItem>(STORE::iPotPdsItemKeyCompare);
  
  _checkInitialised_(CN_LGLBUFSET);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Pot::~Pot(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);

  delete m_pRowPix;
  delete m_pPdsOfBuffers;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create new destination row (possible new row slab)                         */
/*----------------------------------------------------------------------------*/
void* Pot::aAllotRowPds(Pot* pPot, i8 iKeyHash, char** aazStrArea, i8* pnMaxStrAreaLen, i8* piErrorCode)
{
  _static_

  try {
    Row* aDstRow;

    LglPtnMap* pPtnMap = AM::pLglPtnMap();
    //i8 iKeyHsh = HshLib::iHash((byte*)&iMixIn,sizeof(iMixIn));
    LglPtn tLglPtn = pPtnMap->tHashToLglPtnId(iKeyHash);

    RowPix* pRowPix = pPot->m_pRowPix;

    // get pds row buffer for given partition (new a first buffer if ncessary)
    Pds* pPdsRowBuffer = _pPds(pPot->pBuffer(tLglPtn));
    if (! pPdsRowBuffer || pPdsRowBuffer->nFreeBytes() < C_2KB) {
      pPdsRowBuffer = new Pds(pRowPix);
      pPot->vAddBuffer(tLglPtn,pPdsRowBuffer);
    }

    // new a row in pds row buffer (new another pds row buffer if necessary)
    aDstRow = pPdsRowBuffer->pAllotRow();
    if (! aDstRow) {
      pPdsRowBuffer = new Pds(pRowPix);
      pPot->vAddBuffer(tLglPtn,pPdsRowBuffer);
      aDstRow = pPdsRowBuffer->pAllotRow();
      RASSERT(aDstRow);
    }
    *aazStrArea = (char*)pPdsRowBuffer->azDepositString_begin();
    *pnMaxStrAreaLen = pPdsRowBuffer->nFreeBytes();

    pPdsRowBuffer->vLglPtn(tLglPtn);

    return aDstRow->aCellArray();

  }
  catch (LowMemory xLowMemory) {
    *piErrorCode = EC_LOW_MEMORY;
    return nullptr;
  }
}

/*----------------------------------------------------------------------------*/
/* set row string                                                             */
/*----------------------------------------------------------------------------*/
void Pot::vRowStrSize(Pot* pPot, i8 iKeyHsh, i8 nRowStrSize)
{
  _static_

  RASSERT(CN_POT == pPot->eGetClassNo());

  LglPtnMap* pPtnMap = AM::pLglPtnMap();
  //i8 iKeyHsh = HshLib::iHash((byte*)&iMixIn,sizeof(iMixIn));
  LglPtn tLglPtn = pPtnMap->tHashToLglPtnId(iKeyHsh);

  Pds* pPdsRowBuffer = _pPds(pPot->pBuffer(tLglPtn));

  pPdsRowBuffer->vDepositString_end(nRowStrSize);

}

/*----------------------------------------------------------------------------*/
/* repair the pot                                                             */
/*----------------------------------------------------------------------------*/
void Pot::vRepair(Object* pObject)
{
  _static_
  Pot* pPot = _pPot(pObject);

  delete pPot;
}

/*----------------------------------------------------------------------------*/
/* allocate row string                                                        */
/*----------------------------------------------------------------------------*/
void Pot::vSetRowPix(Pot* pPot, RowPix* pRowPix)
{
  _static_

  RASSERT(CN_POT == pPot->eGetClassNo());

  pPot->vRowPix(pRowPix);

}
/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get current buffer                                                         */
/*----------------------------------------------------------------------------*/
DataBuffer* Pot::pBuffer(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);

  PotPdsItem oSearchKey;
  oSearchKey.iLglPtn = i_Ptn;
  oSearchKey.iSynthKey = C_INT_MAX;

  // Find the last block that is less than LglPtn,Max key
  SkipListBlock* pHead = m_pPdsOfBuffers->pFindLessThan((void*) &oSearchKey);

  if(pHead != nullptr) {
    // If the block is not null, extract the keys and values
    PotPdsItem* pItem = (PotPdsItem*) pHead->aItem();

    // If the LglPtn value matches, we found a buffer part of this LglPtn chain
    // Delete it from the Pds and return the DataBuffer associated with it's bufferId
    if(pItem != nullptr && pItem->iLglPtn == i_Ptn) {
      RASSERT(BufferId::bIsBufferId(pItem->oBufferId));
      return _pDataBuffer(BufferId::aAddressFromId(pItem->oBufferId));
    }
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get any completed/full buffer                                              */
/*----------------------------------------------------------------------------*/
DataBuffer* Pot::pCompleted(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);

  PotPdsItem oSearchKey;
  // We increment the LglPtn number since index 0 is used for the Root Block in the Pds
  oSearchKey.iLglPtn = i_Ptn;
  oSearchKey.iSynthKey = C_INT_MIN;

  // Get the first block that is greater than LglPtn,Min
  SkipListBlock* pHead = m_pPdsOfBuffers->pFindGreaterThan((void*) &oSearchKey);

  if(pHead != nullptr) {
    // If the block is not null, extract the keys and values
    PotPdsItem* pItem = (PotPdsItem*) pHead->aItem();

    // If this block doesn't match our LglPtn key, return nullptr
    if(pItem != nullptr && pItem->iLglPtn != i_Ptn) {
      return nullptr;
    }

    // Now figure out whether this is a completed buffer or not by checking the next buffer
    // in the chain
    SkipListBlock* pNextBlock = m_pPdsOfBuffers->pNextSLBlock(pHead, 0);
    if(pNextBlock != nullptr) {
      // Next block exists, check if the key is the same as this LglPtn
      PotPdsItem* pNextItem = (PotPdsItem*) pNextBlock->aItem();
      RASSERT(pNextItem != nullptr);

      if(pNextItem->iLglPtn == i_Ptn) {
        // Next block exists for this LglPtn chain
        // Which means our current buffer is a completed buffer

        // Extract the buffer id
        BufferId oBufferId = pItem->oBufferId;
        RASSERT(BufferId::bIsBufferId(oBufferId));

        // Remove this block from the pds
        bool bResult = m_pPdsOfBuffers->bDelete((void*)pItem);
        RASSERT(bResult);

        // Get the address from the hashmap
        return _pDataBuffer(BufferId::aAddressFromId(oBufferId));
      }
    }
    // Next block does not exist or does not match the LglPtn, hence our current buffer is incomplete
  }
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get any dangling/last buffer                                               */
/*----------------------------------------------------------------------------*/
DataBuffer* Pot::pDangling(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);

  PotPdsItem oSearchKey;
  oSearchKey.iLglPtn = i_Ptn;
  oSearchKey.iSynthKey = C_INT_MAX;
  // Find the buffer part of this LglPtn at the front of the queue.
  // There may be more than one.
  // Pop the head and return it.
  // Note: This function is slightly different than the implementation in
  // LglBufSet as there can be more than one dangling buffer in the queue

  // Find last buffer less than the Key,MAX (i.e front of queue)
  SkipListBlock* pHead = m_pPdsOfBuffers->pFindLessThan((void*) &oSearchKey);

  if(pHead != nullptr) {
    // Get the current head block of this LglPtn and extract the values
    PotPdsItem* pItem = (PotPdsItem*) pHead->aItem();

    // Delete it from the Pds and return the DataBuffer associated with it's bufferId
    // if the LglPtn matches.
    if(pItem != nullptr && pItem->iLglPtn == i_Ptn) {
      BufferId oBufferId = pItem->oBufferId;
      RASSERT(BufferId::bIsBufferId(oBufferId));
      bool bResult = m_pPdsOfBuffers->bDelete((void*)pItem);
      RASSERT(bResult);
      return _pDataBuffer(BufferId::aAddressFromId(oBufferId));
    }
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* create new destination row (possible new row slab)                         */
/*----------------------------------------------------------------------------*/
Row* Pot::aNewDstRow(i8 i_Ptn, Pds** ppPdsRowBuffer,bool* pbNewBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);

  //ASSERT(0 == i_Ptn);

  LglPtn tLglPtn;

  Row* aDstRow;

  *pbNewBuffer = false;

  // get pds row buffer for given partition (new a first buffer if ncessary)
  Pds* pPdsRowBuffer = _pPds(pBuffer(tLglPtn));
  if (! pPdsRowBuffer || pPdsRowBuffer->nFreeBytes() < C_2KB) {

    pPdsRowBuffer = new Pds(m_pRowPix);
    vAddBuffer(tLglPtn,pPdsRowBuffer);
  }

  // new a row in pds row buffer (new another pds row buffer if necessary)
  aDstRow = pPdsRowBuffer->pAllotRow();
  if (! aDstRow) {
    *pbNewBuffer = true;
    pPdsRowBuffer = new Pds(m_pRowPix);
    vAddBuffer(tLglPtn,pPdsRowBuffer);
    aDstRow = pPdsRowBuffer->pAllotRow();
    ASSERT(aDstRow);
  }

  tLglPtn = i_Ptn;
  pPdsRowBuffer->vLglPtn(tLglPtn);

  * ppPdsRowBuffer = pPdsRowBuffer;

  return aDstRow;
}

/*----------------------------------------------------------------------------*/
/* set stream pix                                                             */
/*----------------------------------------------------------------------------*/
void Pot::vRowPix(RowPix* pRowPix)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);
  
  //if (! m_pRowPix) {
    m_pRowPix = pRowPix;
  //}
}

/*----------------------------------------------------------------------------*/
/* get estimate of size                                                       */
/*----------------------------------------------------------------------------*/
i8 Pot::nBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);
  
  return m_pPdsOfBuffers->iNumEntries(0);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add row as first into buffer chain for partition                           */
/*----------------------------------------------------------------------------*/
void Pot::vAddBuffer(i8 i_Ptn, DataBuffer* pAddBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);

  LglPtn tLglPtn;
  tLglPtn = i_Ptn;
  pAddBuffer->vLglPtn(tLglPtn);

  // Append a buffer to this pds
  // Use a synthetic key to ensure the buffers are added in ascending order
  PotPdsItem oItem;
  oItem.iLglPtn = i_Ptn;
  oItem.iSynthKey = m_iSynthKey;
  oItem.oBufferId = pAddBuffer->oGetBufferId();
  m_iSynthKey++;
  RASSERT(BufferId::bIsBufferId(oItem.oBufferId));

  SkipListBlock* pResultBlock = m_pPdsOfBuffers->pInsert((void*) &oItem);
  RASSERT(pResultBlock != nullptr);

  *(PotPdsItem*)(pResultBlock->aItem()) = oItem;
}

/*----------------------------------------------------------------------------*/
/* compare two nodes                                                          */
/*----------------------------------------------------------------------------*/
i8 STORE::iPotPdsItemKeyCompare(void* aItem_l, void* aItem_r)
{
  _static_

  PotPdsItem* pKey_l = (PotPdsItem*) aItem_l;
  PotPdsItem* pKey_r = (PotPdsItem*) aItem_r;

  // do the node names match?
  if (pKey_l->iLglPtn == pKey_r->iLglPtn) {
    if (pKey_l->iSynthKey == pKey_r->iSynthKey) {
      return 0;
    }
    else {
      if (pKey_l->iSynthKey >= pKey_r->iSynthKey)
        return 1;
      else
        return -1;
    }
  } else {
    if (pKey_l->iLglPtn >= pKey_r->iLglPtn)
      return 1;
    else
      return -1;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
