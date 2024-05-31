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
extern "C" void vExtern_PotAllotRowPds(void* aDynamicT, byte* aMixIn, i8 iMixInLen, char** aazStrArea, i8* pnMaxStrAreaLen) {

	Dynamic_T* pDynamic = (Dynamic_T*)aDynamicT;
	Pot* pPot = (Pot*)pDynamic->m_pPot;

	// calculate the key hash
	i8 iKeyHash = HshLib::iHash(aMixIn,iMixInLen);

	// get the lgtptn
	LglPtnMap* pPtnMap = AM::pLglPtnMap();
	LglPtn tLglPtn = pPtnMap->tHashToLglPtnId(iKeyHash);

	try {
		Row* aDstRow;

		// get pds row buffer for given partition (new a first buffer if ncessary)
		Pds* pPdsRowBuffer = _pPds(pPot->pBuffer(tLglPtn));
		if (! pPdsRowBuffer || pPdsRowBuffer->nFreeBytes() < C_2KB) {
			pPdsRowBuffer = new Pds((StreamDef*)pDynamic->m_aStreamDefOut);
			pPot->vAddBuffer(tLglPtn,pPdsRowBuffer);
		}

		// new a row in pds row buffer (new another pds row buffer if necessary)
		aDstRow = pPdsRowBuffer->pAllotRow();
		if (! aDstRow) {
			pPdsRowBuffer = new Pds((StreamDef*)pDynamic->m_aStreamDefOut);
			pPot->vAddBuffer(tLglPtn,pPdsRowBuffer);
			aDstRow = pPdsRowBuffer->pAllotRow();
			RASSERT(aDstRow);
		}
		*aazStrArea = (char*)pPdsRowBuffer->azDepositString_begin();
		*pnMaxStrAreaLen = pPdsRowBuffer->nFreeBytes();

		pPdsRowBuffer->vLglPtn(tLglPtn);

		pDynamic->m_xRowInfoPO[0].m_aRowCellArray = aDstRow->aCellArray();
		pDynamic->m_xRowInfoPO[0].m_aPdsRowBuffer = pPdsRowBuffer;
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

	Pds* pPdsRowBuffer = _pPds(pDynamicT->m_xRowInfoPO[0].m_aPdsRowBuffer);

	pPdsRowBuffer->vDepositString_end(nRowStrSize);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Pot::Pot(void)
: Object(CN_POT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);

  m_iSynthKey = 0;
  m_pPdsOfBufferAddrs = new Pds_t<PotPdsItem>(PdsUtils::iPotPdsItemKeyCompare);
  
  _checkInitialised_(CN_LGLBUFSET);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Pot::~Pot(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);

  delete m_pPdsOfBufferAddrs;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
  SkipListBlock* pHead = m_pPdsOfBufferAddrs->pFindLessThan((void*) &oSearchKey);

  if(pHead != nullptr) {
    // If the block is not null, extract the keys and values
    PotPdsItem* pItem = (PotPdsItem*) pHead->aItem();

    // If the LglPtn value matches, we found a buffer part of this LglPtn chain
    // Delete it from the Pds and return the DataBuffer associated with it's bufferId
    if(pItem != nullptr && pItem->iLglPtn == i_Ptn) {
      return pItem->pDataBuffer;
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
  SkipListBlock* pHead = m_pPdsOfBufferAddrs->pFindGreaterThan((void*) &oSearchKey);

  if(pHead != nullptr) {
    // If the block is not null, extract the keys and values
    PotPdsItem* pItem = (PotPdsItem*) pHead->aItem();

    // If this block doesn't match our LglPtn key, return nullptr
    if(pItem != nullptr && pItem->iLglPtn != i_Ptn) {
      return nullptr;
    }

    // Now figure out whether this is a completed buffer or not by checking the next buffer
    // in the chain
    SkipListBlock* pNextBlock = m_pPdsOfBufferAddrs->pNextSLBlock(pHead, 0);
    if(pNextBlock != nullptr) {
      // Next block exists, check if the key is the same as this LglPtn
      PotPdsItem* pNextItem = (PotPdsItem*) pNextBlock->aItem();
      ASSERT(pNextItem != nullptr);

      if(pNextItem->iLglPtn == i_Ptn) {
        // Next block exists for this LglPtn chain
        // Which means our current buffer is a completed buffer

        DataBuffer* pDataBuffer = pItem->pDataBuffer;

        // Remove this block from the pds
        bool bResult = m_pPdsOfBufferAddrs->bDelete((void*)pItem);
        UNUSED(bResult);
        ASSERT(bResult);

        return pDataBuffer;
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
  SkipListBlock* pHead = m_pPdsOfBufferAddrs->pFindLessThan((void*) &oSearchKey);

  if(pHead != nullptr) {
    // Get the current head block of this LglPtn and extract the values
    PotPdsItem* pItem = (PotPdsItem*) pHead->aItem();

    // Delete it from the Pds and return the DataBuffer associated with it's bufferId
    // if the LglPtn matches.
    if(pItem != nullptr && pItem->iLglPtn == i_Ptn) {
      DataBuffer* pDataBuffer = pItem->pDataBuffer;
      bool bResult = m_pPdsOfBufferAddrs->bDelete((void*)pItem);
      UNUSED(bResult);
      ASSERT(bResult);
      return pDataBuffer;
    }
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* create new destination row (possible new row slab)                         */
/*----------------------------------------------------------------------------*/
Row* Pot::aNewDstRow(StreamDef* pStreamDef, i8 i_Ptn, Pds** ppPdsRowBuffer,bool* pbNewBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);

  //ASSERT(0 == i_Ptn);

  LglPtn tLglPtn;

  Row* aDstRow;

  *pbNewBuffer = false;

  // get pds row buffer for given partition (new a first buffer if ncessary)
  Pds* pPdsRowBuffer = _pPds(pBuffer(tLglPtn));
  if (! pPdsRowBuffer || pPdsRowBuffer->nFreeBytes() < C_2KB) {

    pPdsRowBuffer = new Pds(pStreamDef);
    vAddBuffer(tLglPtn,pPdsRowBuffer);
  }

  // new a row in pds row buffer (new another pds row buffer if necessary)
  aDstRow = pPdsRowBuffer->pAllotRow();
  if (! aDstRow) {
    *pbNewBuffer = true;
    pPdsRowBuffer = new Pds(pStreamDef);
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
/* get estimate of size                                                       */
/*----------------------------------------------------------------------------*/
i8 Pot::nBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POT,_FOOTER_);
  
  return m_pPdsOfBufferAddrs->iNumEntries(0);
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
  oItem.pDataBuffer = pAddBuffer;
  m_iSynthKey++;

  SkipListBlock* pResultBlock = m_pPdsOfBufferAddrs->pInsert((void*) &oItem);
  RASSERT(pResultBlock != nullptr);

  *(PotPdsItem*)(pResultBlock->aItem()) = oItem;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
