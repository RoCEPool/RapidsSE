/*============================================================================*/
/* IndexTree.cpp -- Index Partition                                            */
/*============================================================================*/

#include "BASE.h"

#include "IndexDef.hpp"

#include "Pay.hpp"
#include "Key.hpp"
#include "Ent.hpp"

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "TailLine.hpp"

#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "RowInflator.hpp"

#include "EntBufSorter.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"

#include "ClangDynamic.h"
#include "Op.hpp"
#include "Stage.hpp"
#include "Stream.hpp"

bool mbCheckPriorVec = false;

extern EnvFlags gxEnvFlags;;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
IndexTree* STORE::_pIndexTree(void* pAddr)
{
  IndexTree* pIndexTree = reinterpret_cast<IndexTree*>(pAddr);
  
  RASSERT_TYPE(pIndexTree,CN_INDEXTREE);
  
  return pIndexTree;
}

void* aNewIndexTree(void* pIndexDef, i8* piKind) {
  return IndexTree::aNewIndexTree((IndexDef*)pIndexDef, (eKE*) piKind);
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* IndexTree::operator new(size_t)
{
  void* pObject = Heaps::pAllocate(CN_INDEXTREE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void IndexTree::operator delete(void* pDataAddress)
{
  IndexTree* pIndexTree = reinterpret_cast<IndexTree*>(pDataAddress);
  
  Heaps::vFree(pIndexTree);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
IndexTree::IndexTree(Index* pIndex, IndexDef* pIndexDef, i8 i_TablePtn)
: Object(CN_INDEXTREE)
, m_okLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  m_pIndex = pIndex;
  m_pIndexDef = pIndexDef ? new IndexDef(pIndexDef) : nullptr;
  m_i_TablePtn = i_TablePtn;
  m_pRootEntBuffer  = nullptr;
  m_pPayStrBuffer = nullptr;
  
  m_nEntryBuffers  = 0;
  
  m_nTreeDeleteCnt = 0;
  m_nTreeInsertCnt = 0;

  m_bTreeDebug = false;

  _checkInitialised_(CN_INDEXTREE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
IndexTree::~IndexTree(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);

  vTruncate();
  delete m_pIndexDef;
  delete m_pPayStrBuffer;
}

/*----------------------------------------------------------------------------*/
/*-- Externs -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
extern "C" void* aExtern_IndexTreeFindInsertGenAsc(void* aDynamic, i8 nPayloadBytes, bool *pbInsertedEntry) {

	// get Dynamic
	Dynamic_T* pDynamicT = (Dynamic_T*)aDynamic;

	// get the IndexTree
	IndexTree* pIndexTree = (IndexTree*)pDynamicT->m_xIndexInfoPL.m_aIndexTree;

	// get the key
	Key* aKey = (Key*)pDynamicT->m_xIndexInfoPL.m_aEntKey;

	// get the ent
	Ent* pEntInt = (Ent*)pDynamicT->m_xIndexInfoPL.m_aEntInt;

	// find and insert entry
	Ent* pEntInserted =  pIndexTree->aFindInsertEntryAscGen(aKey, pEntInt, nPayloadBytes, pbInsertedEntry, &pDynamicT->m_iErrorCode);

	//pDynamicT->m_aiPayload = pEntInserted->aiPayload();

	pDynamicT->m_aiPayload = (i8 *) ((char *) pEntInserted
																	 + sizeof(struct Ent_int)
																	 + ((struct Ent_int *) pEntInserted)->m_iIntElm0
																	 + 8);

//	if (*pbInsertedEntry) {
//		// zero the payload
//
//		for (i8 i = 0; i < nPayloadBytes; i++)
//			pDynamicT->m_aiPayload[i] = 0;
//	}



	return pEntInserted;
}


extern "C" void* aExtern_IndexTreeAllotPayStr(void* pIndexTree, char* sStr) {
  return (void*)IndexTree::aAllotPayStr((IndexTree*)pIndexTree, (Stz*)sStr);
}
extern "C" void* aExtern_IndexTreeBegin(void* pIndexTree, void** ppEntBuffer) {
  return (void*)IndexTree::aIndexBegin((IndexTree*)pIndexTree, (EntBuffer**) ppEntBuffer);
}
extern "C" void* aExtern_IndexTreeFindEntry(void* aIndexTree, void* aKey) {
  return (void*)IndexTree::aFindEntry((IndexTree*) aIndexTree, (Key*) aKey);
}
extern "C" void* aExtern_IndexTreeFindEntryAsc(void* aIndexTree, void* aKey) {
  return (void*)IndexTree::aFindEntryAsc((IndexTree*) aIndexTree, (Key*) aKey);
}
extern "C" void* aExtern_IndexTreeFindEntryAscDsc(void* aIndexTree, void* aKey) {
  return (void*)IndexTree::aFindEntryAscDsc((IndexTree*) aIndexTree, (Key*) aKey);
}
extern "C" void* aExtern_IndexTreeFindEntry_hint(void* aIndexTree, void* aKey, void* aEnt_recent) {
  return (void*)IndexTree::aFindEntry_hint((IndexTree*) aIndexTree, (Key*) aKey, (Ent*) aEnt_recent);
}
extern "C" void* aExtern_IndexTreeInsertEnt(void* aIndexTree, void* aEnt, i8* piErrorCode) {
  return IndexTree::aInsertEnt((IndexTree*)aIndexTree, (Ent*)aEnt, piErrorCode);
}
extern "C" void* aExtern_IndexTreeInsertEntTop(void* aIndexTree, void* aEnt, void** aaEntBottom, i8 iTopCount, i8* piErrorCode) {
  return IndexTree::aInsertEntTop((IndexTree*)aIndexTree, (Ent*)aEnt, (Ent**)aaEntBottom,
                                  iTopCount, piErrorCode);
}
extern "C" void* aExtern_IndexTreeNew(void* aIndexDef, i8* aiKeyKind) {
	IndexTree *pIndexTree = new IndexTree(nullptr, (IndexDef *) aIndexDef, C_UNKNOWN);
	*aiKeyKind = (i8) ((IndexDef *) aIndexDef)->eKeyKind();
	return pIndexTree;
}

extern "C" void vExtern_IndexNew(void* aDynamic, i8 nNumCols, ColumnInfo* pColumnInfo, bool bPayload) {

	Dynamic_T* pDynamicT = (Dynamic_T*)aDynamic;
	EntWrk* pEntWrk = (EntWrk*)pDynamicT->m_xIndexInfoPL.m_aEntWrk;

	IndexDef* pIndexDef = new IndexDef;

	for (i8 i_Col = 0; i_Col < nNumCols; i_Col++) {
		pIndexDef->vAddColumn((Stz*)&pColumnInfo[i_Col].m_cColumnNm[0],
													(TypeId)pColumnInfo[i_Col].m_iColumnTypeId,
													pColumnInfo[i_Col].m_bIsAscending);

		if (pColumnInfo[i_Col].m_bIsAscending == 0)
			pIndexDef->vHasDescending(true);
	}

	pDynamicT->m_xIndexInfoPL.m_aIndexDef = pIndexDef;

  IndexTree* pIndexTree = new IndexTree(nullptr, pIndexDef, C_UNKNOWN);

	EntWrk::vEntKind(pEntWrk, bPayload, pIndexDef->eKeyKind());

	pDynamicT->m_xIndexInfoPL.m_aIndexTree = pIndexTree;
}

extern "C" void vExtern_IndexFind(void* aIndexInfo, char* aTableName, char* aIndexName) {

	LglPtn tLglPtn;
	IndexInfo* pIndexInfo = (IndexInfo*)aIndexInfo;

	Table* pTable = AM::pTableFind((Stz*)aTableName);
	Index* pIndex = pTable->pFindIndex((Stz*)aIndexName);

	pIndexInfo->m_aIndexDef = pIndex->pIndexDef();

	StreamDef* pStreamDef = pTable->pStreamDef();
	Op* pOp = MY::pOp();

	if (pStreamDef->bReplicated()) {

		Stage*  pStage   = pOp->pStage();
		Stream* pStream0 = pStage->pGet(0);
		Op*     pOp0_0   = pStream0->pGet(0);

		tLglPtn = pOp0_0->tLglPtn();
	}
	else {
		tLglPtn = pOp->tLglPtn();
	}

	i8 i_TablePtn = pTable->iPtnNoToTablePtn(tLglPtn);
	TablePtn* pTablePtn = pTable->pTablePtn(i_TablePtn);

	i8 iIndexNo = pIndex->iIndexNo();
	pIndexInfo->m_aIndexTree = pTablePtn->pIndexTree(iIndexNo);
}

/*----------------------------------------------------------------------------*/
/* position at entry greater than equal to given gen key                      */
/*----------------------------------------------------------------------------*/
extern "C" void vExtern_IndexTreePositionGE_gen(void* aIndexInfo) {

	IndexInfo *pIndexInfo = (IndexInfo *) aIndexInfo;
	IndexTree *pIndexTree = (IndexTree *) pIndexInfo->m_aIndexTree;
	Key *aKey = (Key *) pIndexInfo->m_aEntKey;

	EntBuffer *pEntBuffer = nullptr;
	Ent *aEnt = nullptr;

	pEntBuffer = pIndexTree->pPositionH0Buffer_gen(aKey);
	if (!pEntBuffer) {
		pIndexInfo->m_aEntInt = nullptr;
		return;
	}

	aEnt = pEntBuffer->aFindEntryBefore_gen(aKey);
	if (!aEnt) {
		pIndexInfo->m_aEntInt = nullptr;
	  return;
  }

	aEnt = EntBuffer::pMoveToGreaterEqualEntry_gen(aEnt, aKey, &pEntBuffer);

	pIndexInfo->m_aEntBuffer = pEntBuffer;
	pIndexInfo->m_aEntInt = (Ent_int*)aEnt;
}

/*----------------------------------------------------------------------------*/
/* position at entry greater than equal to given int key                      */
/*----------------------------------------------------------------------------*/
extern "C" void vExtern_IndexTreePositionGE_int(void* aIndexInfo) {

	IndexInfo *pIndexInfo = (IndexInfo *) aIndexInfo;
	IndexTree *pIndexTree = (IndexTree *) pIndexInfo->m_aIndexTree;
	Key *aKey = (Key *) pIndexInfo->m_aEntKey;

	EntBuffer *pEntBuffer = nullptr;
	Ent *aEnt = nullptr;

	pEntBuffer = pIndexTree->pPositionH0Buffer_int(aKey);
	if (!pEntBuffer) {
		pIndexInfo->m_aEntInt = nullptr;
		return;
	}

	aEnt = pEntBuffer->aFindEntryBefore_MoveGE_int(aKey, &pEntBuffer);
	if (!aEnt) {
		pIndexInfo->m_aEntInt = nullptr;
		return;
	}

	//aEnt = EntBuffer::pMoveToGreaterEqualEntry_int(aEnt, aKey, &pEntBuffer);

	pIndexInfo->m_aEntBuffer = pEntBuffer;
	pIndexInfo->m_aEntInt = (Ent_int*)aEnt;
}

/*----------------------------------------------------------------------------*/
/* position at entry greater than given general key                           */
/*----------------------------------------------------------------------------*/
extern "C" void vExtern_IndexTreePositionGT_gen(void* aIndexInfo) {

  IndexInfo *pIndexInfo = (IndexInfo *) aIndexInfo;
	IndexTree *pIndexTree = (IndexTree *) pIndexInfo->m_aIndexTree;
	Key *aKey = (Key *) pIndexInfo->m_aEntKey;

	EntBuffer* pEntBuffer = nullptr;
	Ent* aEnt = nullptr;

	pEntBuffer = pIndexTree->pPositionH0Buffer_gen(aKey);
	if ( !pEntBuffer ) {
		pIndexInfo->m_aEntInt = nullptr;
		return;
	}

	aEnt = pEntBuffer->aFindEntryBefore_gen(aKey);
	if ( !aEnt ) {
		pIndexInfo->m_aEntInt = nullptr;
		return;
	}

	aEnt = EntBuffer::pMoveToGreaterThanEntry_gen(aEnt, aKey, &pEntBuffer);

	pIndexInfo->m_aEntBuffer = pEntBuffer;
	pIndexInfo->m_aEntInt = (Ent_int*)aEnt;
}

/*----------------------------------------------------------------------------*/
/* position at entry greater than given integer key                           */
/*----------------------------------------------------------------------------*/
extern "C" void vExtern_IndexTreePositionGT_int(void* aIndexInfo) {

	IndexInfo *pIndexInfo = (IndexInfo *) aIndexInfo;
	IndexTree *pIndexTree = (IndexTree *) pIndexInfo->m_aIndexTree;
	Key *aKey = (Key *) pIndexInfo->m_aEntKey;

	EntBuffer* pEntBuffer = nullptr;
	Ent* aEnt = nullptr;

	pEntBuffer = pIndexTree->pPositionH0Buffer_int(aKey);
	if ( !pEntBuffer ) {
		pIndexInfo->m_aEntInt = nullptr;
		return;
	}

	aEnt = pEntBuffer->aFindEntryBefore_int(aKey);
	if ( !aEnt ) {
		pIndexInfo->m_aEntInt = nullptr;
		return;
	}

	aEnt = EntBuffer::pMoveToGreaterThanEntry_int(aEnt, aKey, &pEntBuffer);

	pIndexInfo->m_aEntBuffer = pEntBuffer;
	pIndexInfo->m_aEntInt = (Ent_int*)aEnt;
}

/*----------------------------------------------------------------------------*/
//* position at entry less than equal to given general key                    */
/*----------------------------------------------------------------------------*/
extern "C" void vExtern_IndexTreePositionLE_gen(void* aIndexInfo) {

	IndexInfo *pIndexInfo = (IndexInfo *) aIndexInfo;
	IndexTree *pIndexTree = (IndexTree *) pIndexInfo->m_aIndexTree;
	Key *aKey = (Key *) pIndexInfo->m_aEntKey;

	EntBuffer* pEntBuffer = nullptr;
	Ent* aEnt = nullptr;

	pEntBuffer = pIndexTree->pPositionH0Buffer_gen(aKey);
	if ( !pEntBuffer ) {
		pIndexInfo->m_aEntUpper = nullptr;
		return;
	}

	aEnt = pEntBuffer->aFindEntryBefore_gen(aKey);
	if ( !aEnt ) {
		pIndexInfo->m_aEntUpper = nullptr;
		return;
	}

	aEnt = EntBuffer::pMoveToNextEntry(aEnt, &pEntBuffer);
	if ( !aEnt ) {
		pIndexInfo->m_aEntUpper = nullptr;
		return;
	}

	aEnt = EntBuffer::pMoveToGreaterThanEntry_gen(aEnt, aKey, &pEntBuffer);

	pIndexInfo->m_aEntUpper = (Ent_int*)aEnt;
}

/*----------------------------------------------------------------------------*/
//* position at entry less than equal to given integer key                    */
/*----------------------------------------------------------------------------*/
extern "C" void vExtern_IndexTreePositionLE_int(void* aIndexInfo) {

	IndexInfo *pIndexInfo = (IndexInfo *) aIndexInfo;
	IndexTree *pIndexTree = (IndexTree *) pIndexInfo->m_aIndexTree;
	Key *aKey = (Key *) pIndexInfo->m_aEntKey;

	EntBuffer* pEntBuffer = nullptr;
	Ent* aEnt = nullptr;

	pEntBuffer = pIndexTree->pPositionH0Buffer_int(aKey);
	if ( !pEntBuffer ) {
		pIndexInfo->m_aEntUpper = nullptr;
		return;
	}

	aEnt = pEntBuffer->aFindEntryBefore_int(aKey);
	if ( !aEnt ) {
		pIndexInfo->m_aEntUpper = nullptr;
		return;
	}

	aEnt = EntBuffer::pMoveToNextEntry(aEnt, &pEntBuffer);
	if ( !aEnt ) {
		pIndexInfo->m_aEntUpper = nullptr;
		return;
	}

	aEnt = EntBuffer::pMoveToGreaterThanEntry_int(aEnt, aKey, &pEntBuffer);

	pIndexInfo->m_aEntUpper = (Ent_int*)aEnt;
}

/*----------------------------------------------------------------------------*/
/* position at entry less than given general key                              */
/*----------------------------------------------------------------------------*/
extern "C" void vExtern_IndexTreePositionLT_gen(void* aIndexInfo) {

	IndexInfo *pIndexInfo = (IndexInfo *) aIndexInfo;
	IndexTree *pIndexTree = (IndexTree *) pIndexInfo->m_aIndexTree;
	Key *aKey = (Key *) pIndexInfo->m_aEntKey;

	EntBuffer* pEntBuffer = nullptr;
	Ent* aEnt = nullptr;

	pEntBuffer = pIndexTree->pPositionH0Buffer_gen(aKey);
	if ( !pEntBuffer ) {
		pIndexInfo->m_aEntUpper = nullptr;
		return;
	}

	aEnt = pEntBuffer->aFindEntryBefore_gen(aKey);
	if ( !aEnt ) {
		pIndexInfo->m_aEntUpper = nullptr;
		return;
	}

	aEnt = EntBuffer::pMoveToNextEntry(aEnt, &pEntBuffer);

	pIndexInfo->m_aEntUpper = (Ent_int*)aEnt;
}

/*----------------------------------------------------------------------------*/
/* position at entry less than given general key                              */
/*----------------------------------------------------------------------------*/
extern "C" void vExtern_IndexTreePositionLT_int(void* aIndexInfo) {

	IndexInfo *pIndexInfo = (IndexInfo *) aIndexInfo;
	IndexTree *pIndexTree = (IndexTree *) pIndexInfo->m_aIndexTree;
	Key *aKey = (Key *) pIndexInfo->m_aEntKey;

	EntBuffer* pEntBuffer = nullptr;
	Ent* aEnt = nullptr;

	pEntBuffer = pIndexTree->pPositionH0Buffer_int(aKey);
	if ( !pEntBuffer ) {
		pIndexInfo->m_aEntUpper = nullptr;
		return;
	}

	aEnt = pEntBuffer->aFindEntryBefore_int(aKey);
	if ( !aEnt ) {
		pIndexInfo->m_aEntUpper = nullptr;
		return;
	}

	aEnt = EntBuffer::pMoveToNextEntry(aEnt, &pEntBuffer);

	pIndexInfo->m_aEntUpper = (Ent_int*)aEnt;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump data                                                                  */
/*----------------------------------------------------------------------------*/
void IndexTree::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  UNUSED(eClass);
  UNUSED(pObject);
  UNUSED(aTab);
}

/*----------------------------------------------------------------------------*/
/* allocate a pay load string                                                  */
/*----------------------------------------------------------------------------*/
char* IndexTree::aAllotPayStr(IndexTree* pIndexTree, Stz* szStz) {

  _static_

  return (char*)pIndexTree->pAllotPayStr(szStz)->szStz();
}

/*----------------------------------------------------------------------------*/
/* begin linear traverse                                                      */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::aIndexBegin(IndexTree* pIndexTree, EntBuffer** ppEntBuffer)
{
  _static_

  // indextree ?
  if ( ! pIndexTree )
    return nullptr;

  //pIndexTree->vDump(false);

  EntBuffer* pEntBuffer = *ppEntBuffer;
  Ent* aEnt = nullptr;

  pEntBuffer = pIndexTree->pFirstH0Buffer();

  if (pEntBuffer)
    aEnt = pEntBuffer->aLeadEntry();

  *ppEntBuffer = pEntBuffer;

  return aEnt;
}

/*----------------------------------------------------------------------------*/
/* insert an entry for given row into this index at level 0                   */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::aInsertEnt(IndexTree* pIndexTree, Ent* pEnt, i8* piErrorCode)
{
  _static_

  try {
    Ent *aInsertedEntry = pIndexTree->aInsertEntry(pEnt);

    //RASSERT(pIndexTree->pRootBuffer()->bVerify());
    //pIndexTree->vDump(false);

    return aInsertedEntry;
  }
  catch (LowMemory xLowMemory) {
    *piErrorCode = EC_LOW_MEMORY;
    return nullptr;
  }
}

/*----------------------------------------------------------------------------*/
/* insert an entry for given row into this index at level 0                   */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::aInsertEntTop(IndexTree* pIndexTree, Ent* pEnt, Ent** ppEntBottom,
                              i8 iTopCount, i8* piErrorCode)
{
  _static_

  try {
    return pIndexTree->pInsertEntTop_(pEnt, ppEntBottom, iTopCount);
  }
  catch (LowMemory xLowMemory) {
    *piErrorCode = EC_LOW_MEMORY;
    return nullptr;
}
}

/*----------------------------------------------------------------------------*/
/* new an indextree                                                           */
/*----------------------------------------------------------------------------*/
IndexTree* IndexTree::aNewIndexTree(IndexDef* pIndexDef, eKE* peKind)
{
  IndexTree* pIndexTree = new IndexTree(nullptr, pIndexDef, C_UNKNOWN);
  *peKind = pIndexDef->eKeyKind();

  return pIndexTree;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot a pay load string                                                    */
/*----------------------------------------------------------------------------*/
PayStr* IndexTree::pAllotPayStr(Stz* szStz)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);

  if (! m_pPayStrBuffer) {
    m_pPayStrBuffer = new PayStrBuffer;
  }

  PayStr* pPayStr = new (m_pPayStrBuffer,szStz) PayStr;

  return pPayStr;
}

/*----------------------------------------------------------------------------*/
/* get the index buffers used                                                 */
/*----------------------------------------------------------------------------*/
i8 IndexTree::nBufferCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  i8 nBufferCount = 0;
  if (m_pRootEntBuffer)
    m_pRootEntBuffer->vBufferCount(nBufferCount);
  
  return nBufferCount;
}

/*----------------------------------------------------------------------------*/
/* compact the index                                                          */
/*----------------------------------------------------------------------------*/
void IndexTree::vCompact(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  TRACE_COMPACT( vTrace("IndexTree::vCompact\n"_Stz); )
  
  mbCheckPriorVec = true;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the number of H1 buffers
  
#ifdef WITH_TRACE_INDEX_COMPACTION
  EntBuffer* pH1First = pFirstH1Buffer();
  i8 nP1Buffers = pH1First ? pH1First->iLengthList() : 0;
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // step thru each H1 buffer in the index
  
  i8 iH1BufferNo = -1;
  UNUSED(iH1BufferNo);

  for (EntBuffer* pH1 = pFirstH1Buffer(); pH1; pH1 = pH1->pNextEntBuffer()) {
    
    iH1BufferNo++;

#ifdef WITH_TRACE_INDEX_COMPACTION
     if (0 == iH1BufferNo % 10)
      vTrace("Compaction Progress: Partition %lld: H1 Buffer %lld of %lld\n"_Stz,
             m_i_TablePtn,iH1BufferNo,nP1Buffers);
#endif
    
    TRACE_COMPACT( vTrace("  Compacting H1 %lld\n"_Stz,pH1->iBufferNo()); )
    //vTrace("  Compacting H1 %lld\n"_Stz,pH1->iBufferNo());
    
    //ASSERT(pH1->bVerifySkipListEntry());
    
    //continue;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the first H0 and the sublist which follows
    
    // get the first H0 of current H1
    EntBuffer* pFirstH0 = pH1->pFirstChildBuffer();
    
    TRACE_COMPACT( pFirstLeftH0->vShowSubList(); )
    
    // get the sublist after the first H0 of current H1
    EntBuffer* pH0SubList = pFirstH0->pUnlinkSubList();
    if (! pH0SubList)
      continue;
    
    i8 iSubListLength = pH0SubList->iLengthSubList();
    UNUSED(iSubListLength);
    
    TRACE_COMPACT( pH0SubList->vShowSubList(); )
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // copy each key in each H0 of sublist leftwards into (new) H0 buffers
    
    // first reinit the H1 buffer
    EntBuffer::vCopyReInitH1(pH1,pFirstH0);
    
    // foreach right H0 in sublist
    EntBuffer* pLeftH0 = pFirstH0;
    while (EntBuffer* pRightH0 = pPopSubList(&pH0SubList)) {
      
      TRACE_COMPACT( if (pSubList) pSubList->vShowSubList(); )
      
      //vTrace("Sublist: %lld\n"_Stz,--iSubListLength);
      
      // copy contents of right H0 leftwards
      EntBuffer::vCopyLeftH0(&pLeftH0,pRightH0);
      
      // delete the right H0
      delete pRightH0;
      
    }
    // build the skip list for dangling left H0
    pLeftH0->vBuildSkipListSorted();
    
    //pH1->bVerifyKeyBufferOrder();
  }
  
  if (KE_INT != m_pIndexDef->eKeyKind())
    return;
  
  EntBuffer* pEntBuffer = pRootBuffer();
  if ( !pEntBuffer )
    return;
  
  EntBufSorter* pEntBufSorter = new EntBufSorter;

  if (pEntBuffer->iBufferHeight())
     pEntBuffer = pEntBuffer->pFirstChildBuffer();

  for (;
       pEntBuffer;
       pEntBuffer = pEntBuffer->pNextEntBuffer()) {
    
    pEntBufSorter->vSort_int(pEntBuffer);

  }
  
  delete pEntBufSorter;

}

/*----------------------------------------------------------------------------*/
/* count the index entries at H0 level                                        */
/*----------------------------------------------------------------------------*/
i8 IndexTree::iCountH0Entries(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  i8 iCount = 0;
  EntBuffer* pEntBuffer = pFirstH0Buffer();

  while (pEntBuffer) {
    iCount += pEntBuffer->nItems();
    pEntBuffer = pEntBuffer->pNextEntBuffer();
  }
  
  return iCount;
}

/*----------------------------------------------------------------------------*/
/* delete an entry from an entryBuffer                                        */
/*----------------------------------------------------------------------------*/
void IndexTree::vDelete(EntBuffer* pEntryBuffer, Ent* pEnt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // purge the entry
  
//  vTrace("x x x x x x x x x x x x x x x x x x x x x x x x x x x x \n"_Stz,pEnt);
//  vTrace("Delete Ent %p\n"_Stz,pEnt);
//  vTrace("nItems %lld\n"_Stz,
//         m_pRootEntBuffer->nItems());
//  m_pRootEntBuffer->vDumpItems();
  
  pEntryBuffer->vPurgeUpwards(pEnt, &m_pRootEntBuffer);
  
//  vTrace("z z z z z z z z z z z z z z z z z z z z z z z z z z z z  \n"_Stz,pEnt);
//  m_pRootEntBuffer->vDumpItems();
//  vTrace("nItems %lld\n"_Stz,
//         m_pRootEntBuffer->nItems());
}

/*----------------------------------------------------------------------------*/
/* dump the index                                                             */
/*----------------------------------------------------------------------------*/
void IndexTree::vDump(bool bSameHeight)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);

  if (bSameHeight) {
    EntBuffer* pEntBuffer = m_pRootEntBuffer;
    
    while (pEntBuffer) {
      pEntBuffer->vShow();
      pEntBuffer = pEntBuffer->pNextEntBuffer();
    }
  }
  else {
    // start with the root
    if (m_pRootEntBuffer)
      m_pRootEntBuffer->vDump(true);
  }
}

/*----------------------------------------------------------------------------*/
/* get number of ent buffers                                                  */
/*----------------------------------------------------------------------------*/
i8 IndexTree::nEntBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
    
  return m_nEntryBuffers;
}

/*----------------------------------------------------------------------------*/
/* dec number of ent buffers                                                  */
/*----------------------------------------------------------------------------*/
void IndexTree::vEntBuffersDec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
    
  m_nEntryBuffers --;
}

/*----------------------------------------------------------------------------*/
/* inc number of ent buffers                                                  */
/*----------------------------------------------------------------------------*/
void IndexTree::vEntBuffersInc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
    
  m_nEntryBuffers ++;
}

/*----------------------------------------------------------------------------*/
/* get the first H1 buffer                                                    */
/*----------------------------------------------------------------------------*/
EntBuffer* IndexTree::pFirstH1Buffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  EntBuffer* pEntBuffer = m_pRootEntBuffer;
  
  if ( ! pEntBuffer )
    return nullptr;
  
  if (pEntBuffer->iBufferHeight() == 0)
    return nullptr;
  
  if (pEntBuffer->iBufferHeight() == 1)
    return pEntBuffer;
  
  while (pEntBuffer) {
    
    Ent* aFirstEntry = pEntBuffer->aLeadEntry();
    pEntBuffer = EntBuffer::pRefdEntBuffer(aFirstEntry);
    
    if (pEntBuffer->iBufferHeight() == 1) {
      return pEntBuffer;
    }
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get the first H0 buffer                                                    */
/*----------------------------------------------------------------------------*/
EntBuffer* IndexTree::pFirstH0Buffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  EntBuffer* pEntBuffer = m_pRootEntBuffer;
  
  if ( ! pEntBuffer )
    return nullptr;
  
  if (pEntBuffer->iBufferHeight() == 0)
    return pEntBuffer;
  
  while (pEntBuffer) {
    
    Ent* aFirstEntry = pEntBuffer->aLeadEntry();
    
    pEntBuffer = EntBuffer::pRefdEntBuffer(aFirstEntry);
    
    if (pEntBuffer->iBufferHeight() == 0) {
      return pEntBuffer;
    }
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get reference given key                                                    */
/*----------------------------------------------------------------------------*/
Rrf IndexTree::xwGetRef(Key* aFindKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  Rrf xwNilRef;

  EntBuffer* pEntBuffer = pPositionH0Buffer(aFindKey);
  if (! pEntBuffer)
    return xwNilRef;

  Ent* aEntPos = pEntBuffer->aFindEntryBefore(aFindKey);
  if (! aEntPos)
    return xwNilRef;

  aEntPos = EntBuffer::pMoveToNextEntry(aEntPos,&pEntBuffer);
  
  //! ALKA added:
  // could be the last entry ...
  if ( ! aEntPos ) {
    return xwNilRef;;
  }
  
  eKC eCompare = Key::eCompare(pIndexDef(),aEntPos->aKey(),aFindKey);
  if (KC_SAME != eCompare)
    return xwNilRef;

  return aEntPos->rxRef();
}

/*----------------------------------------------------------------------------*/
/* get the index                                                              */
/*----------------------------------------------------------------------------*/
Index* IndexTree::pIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  return m_pIndex;
}

/*----------------------------------------------------------------------------*/
/* get the indexDef                                                           */
/*----------------------------------------------------------------------------*/
IndexDef* IndexTree::pIndexDef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  return m_pIndex ? m_pIndex->pIndexDef() : m_pIndexDef;
}

/*----------------------------------------------------------------------------*/
/* insert entries for pCrwBuffer into this index at level 0                   */
/*----------------------------------------------------------------------------*/
void IndexTree::vInsertEntries(CrwBuffer* pCrwBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);

  //pEntBuffer->vShow();

  StreamDef* pStreamDef = pCrwBuffer->pStreamDef_dbuff();
  i8* aiaColSet = m_pIndexDef->aiaColSet();

  EntWrk xEntWrk;

  i8* aiDstk;
  HeadLine* aHeadLine;
  
  Rrf xRowRef;
  xRowRef.vBuffer(pCrwBuffer->iBufferNo());

  if (! m_pRootEntBuffer) {
    EntBuffer* pRootBuffer = pNewRootBuffer();
    UNUSED(pRootBuffer);
  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // iterate thru each crw in buffer
  
  i8 nRows = pCrwBuffer->nRows();
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {

    xEntWrk.vReset();
    
    xRowRef.vItem(i_Row);
    xEntWrk.vRef(xRowRef);
    
    if (KE_INT == m_pIndexDef->eKeyKind()) {

      aiDstk = ((i8*)xEntWrk.aKey()) - 1;
      aHeadLine = pCrwBuffer->aGetHead(i_Row);

      RowInflator::vInflate_Key_into_ElementArray(&aiDstk,aHeadLine,aiaColSet,
                               pStreamDef,m_pIndexDef,SI_KEY);
      xEntWrk.aEnt()->vKind(KE_INT);
    }
    else {
      vExtractKey(&xEntWrk,pCrwBuffer,i_Row);
    }
    
    Ent* aInsertedEntry = EntBuffer::aInsertEntTree(xEntWrk.aEnt(),&m_pRootEntBuffer);
    ASSERT(aInsertedEntry);
    UNUSED(aInsertedEntry);
  }
}

/*----------------------------------------------------------------------------*/
/* insert entry into this index at level 0 for iTop entries                   */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::pInsertEntTop_(Ent* aEnt, Ent** ppEntBottom, i8 iTopCount)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);

  Ent* pEntBottom = *ppEntBottom;
  EntBuffer* pEntBuffer;

  //vTrace("*** ENTRY : %lld\n"_Stz, aEnt->iIntElm0());

  if (! m_pRootEntBuffer) {
    EntBuffer* pRootBuffer = pNewRootBuffer();
    UNUSED(pRootBuffer);
  }

  if (!*ppEntBottom) {
    Ent* aInsertedEntry = EntBuffer::aInsertEntTree(aEnt,&m_pRootEntBuffer);
    *ppEntBottom = aInsertedEntry;
    return aInsertedEntry;
  }

  if (m_nTreeInsertCnt < iTopCount) {
    Ent *aInsertedEntry = EntBuffer::aInsertEntTree(aEnt, &m_pRootEntBuffer);
    if (pEntBottom->iIntElm0() > aEnt->iIntElm0())
      *ppEntBottom = aInsertedEntry;
    return aInsertedEntry;
  }

  //vDump(false);
  if (pEntBottom->iIntElm0() < aEnt->iIntElm0()) {

    Ent *aInsertedEntry = EntBuffer::aInsertEntTree(aEnt, &m_pRootEntBuffer);

    // delete the first key
    Ent* aFirstEnt = IndexTree::aIndexBegin(this, &pEntBuffer);

    // update the bottom entry
    *ppEntBottom = pEntBuffer->pMoveToNextEntry(aFirstEnt, &pEntBuffer);

    pEntBuffer->vDeleteEntry(&aFirstEnt);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    //vTreeDeleteInc();
    //vDump(false);
    return aInsertedEntry;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* insert a given entry into this index tree starting at leaf / height 0      */
/*----------------------------------------------------------------------------*/
bool IndexTree::bInsertEntry(Ent* aEnt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);

  m_okLock.vGrabWrite();                                           //! GRAB LOCK
  
  if (! m_pRootEntBuffer) {
    EntBuffer* pRootBuffer = pNewRootBuffer();
    UNUSED(pRootBuffer);
  }
  
  Ent* aInsertedEntry = EntBuffer::aInsertEntTree(aEnt,&m_pRootEntBuffer);
  ASSERT(aInsertedEntry);
  UNUSED(aInsertedEntry);
  
  //m_pRootEntBuffer->bVerify();
  
  m_okLock.vDropWrite();                                           //! DROP LOCK
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* insert an entry for given row into this index at level 0                   */
/*----------------------------------------------------------------------------*/
bool IndexTree::bInsertEntry(CrwBuffer* pCrwBuffer, i8 i_Row, Rrf& xRowRef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  m_okLock.vGrabWrite();
  
  EntWrk xEntWrk;
  vExtractKey(&xEntWrk,pCrwBuffer,i_Row);
  xEntWrk.vRef(xRowRef);
  
  if (! m_pRootEntBuffer) {
    EntBuffer* pRootBuffer = pNewRootBuffer();
    UNUSED(pRootBuffer);
  }
  
  Ent* aInsertedEntry = EntBuffer::aInsertEntTree(xEntWrk.aEnt(),&m_pRootEntBuffer);
  ASSERT(aInsertedEntry);
  UNUSED(aInsertedEntry);

  //m_pRootEntBuffer->bVerify();
  
  m_okLock.vDropWrite();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* insert an entry for given row into this index at level 0                   */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::aInsertPayloadEntry(EntWrk& xEntWrk, i8 nPayloadBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  m_okLock.vGrabWrite();
  
  xEntWrk.vPayload(true);
  xEntWrk.vPayloadBytes(nPayloadBytes);
  
  byte* ayPayLoad = xEntWrk.ayPayload();
  RndLib::vBytes(ayPayLoad,nPayloadBytes);

  if (! m_pRootEntBuffer) {
    EntBuffer* pRootBuffer = pNewRootBuffer();
    UNUSED(pRootBuffer);
  }
  
  Ent* aInsertedEntry = EntBuffer::aInsertEntTree(xEntWrk.aEnt(),&m_pRootEntBuffer);
  ASSERT(aInsertedEntry);
  UNUSED(aInsertedEntry);

  //m_pRootEntBuffer->bVerify();
  
  m_okLock.vDropWrite();
  
  return aInsertedEntry;
}

/*----------------------------------------------------------------------------*/
/* insert an entry for given row into this index at level 0                   */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::aInsertEntry(Ent* pEnt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  m_okLock.vGrabWrite();
  
  if (! m_pRootEntBuffer) {
    EntBuffer* pRootBuffer = pNewRootBuffer();
    UNUSED(pRootBuffer);
  }
  
  Ent* aInsertedEntry = EntBuffer::aInsertEntTree(pEnt,&m_pRootEntBuffer);
  ASSERT(aInsertedEntry);
  
  //m_pRootEntBuffer->bVerify();
  
  m_okLock.vDropWrite();

  //vDump(false);

  return aInsertedEntry;
}

/*----------------------------------------------------------------------------*/
/* get the key kind                                                           */
/*----------------------------------------------------------------------------*/
eKE IndexTree::eKeyKind(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  return m_pIndexDef->eKeyKind();
}
                            
/*----------------------------------------------------------------------------*/
/* build skip lists for the index                                             */
/*----------------------------------------------------------------------------*/
void IndexTree::vLazyBuild(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  m_pRootEntBuffer->vLazyBuild();
}

/*----------------------------------------------------------------------------*/
/* pop entry buffer from sub list                                             */
/*----------------------------------------------------------------------------*/
EntBuffer* IndexTree::pPopSubList(EntBuffer** ppSubList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);

  EntBuffer* pTopBuffer = *ppSubList;
  
  if (! pTopBuffer)
    return nullptr;
  
  *ppSubList = (*ppSubList)->pNextEntBuffer();
  
  pTopBuffer->vNextEntBuffer(nullptr);
  pTopBuffer->vPrevEntBuffer(nullptr);
  
  if (*ppSubList)
    (*ppSubList)->vPrevEntBuffer(nullptr);
  
  return pTopBuffer;
}

/*----------------------------------------------------------------------------*/
/* position to the first H0 buffer which has given key                        */
/*----------------------------------------------------------------------------*/
EntBuffer* IndexTree::pPositionH0Buffer(Key* aFindKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  if (m_pRootEntBuffer)
    return m_pRootEntBuffer->pFindLeafBuffer(aFindKey);
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* position to the first H0 buffer which has given key                        */
/*----------------------------------------------------------------------------*/
EntBuffer* IndexTree::pPositionH0Buffer_gen(Key* aFindKey)
{
	_precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);

	if (m_pRootEntBuffer)
		return m_pRootEntBuffer->pFindLeafBuffer_gen(aFindKey);
	else
		return nullptr;
}

/*----------------------------------------------------------------------------*/
/* position to the first H0 buffer which has given key                        */
/*----------------------------------------------------------------------------*/
EntBuffer* IndexTree::pPositionH0Buffer_int(Key* aFindKey)
{
	_precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);

	if (m_pRootEntBuffer)
		return m_pRootEntBuffer->pFindLeafBuffer_int(aFindKey);
	else
		return nullptr;
}

/*----------------------------------------------------------------------------*/
/* position to the first H0 buffer which has given key                        */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::aPositionPriorEntry(Key* aFindKey, EntBuffer** ppEntBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  EntBuffer* pEntBuffer;
  
  // root buffer ?
  if ( ! m_pRootEntBuffer )
    return nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  - - - - - - -
  // position at H0 buffer
  pEntBuffer = m_pRootEntBuffer->pFindLeafBuffer(aFindKey);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  - - - - - -
  // position at prior entry
  Ent* aEnt_prior = pEntBuffer->aFindEntryBefore(aFindKey);
  
  // entry ?
  if ( ! aEnt_prior )
    return nullptr;
  
  // return the prior entry and the buffer
  *ppEntBuffer = pEntBuffer;
  return aEnt_prior;

}

/*----------------------------------------------------------------------------*/
/* position to the first H0 buffer which has given key                        */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::aPositionPriorEntryAsc(Key* aFindKey, EntBuffer** ppEntBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);

  EntBuffer* pEntBuffer;

  // root buffer ?
  if ( ! m_pRootEntBuffer )
    return nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  - - - - - - -
  // position at H0 buffer
  pEntBuffer = m_pRootEntBuffer->pFindLeafBuffer(aFindKey);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  - - - - - -
  // position at prior entry
  Ent* aEnt_prior = pEntBuffer->aFindEntryBefore(aFindKey);

  // entry ?
  if ( ! aEnt_prior )
    return nullptr;

  // return the prior entry and the buffer
  *ppEntBuffer = pEntBuffer;
  return aEnt_prior;

}

/*----------------------------------------------------------------------------*/
/* create a new root buffer                                                   */
/*----------------------------------------------------------------------------*/
EntBuffer* IndexTree::pNewRootBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  ASSERT(! m_pRootEntBuffer);
  
  m_pRootEntBuffer = new EntBuffer(this);

  LglPtn tLglPtn;
  tLglPtn = m_i_TablePtn;
  m_pRootEntBuffer->vLglPtn(tLglPtn);

  return m_pRootEntBuffer;
}

/*----------------------------------------------------------------------------*/
/* get root buffer                                                            */
/*----------------------------------------------------------------------------*/
EntBuffer* IndexTree::pRootBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  return m_pRootEntBuffer;
}

/*----------------------------------------------------------------------------*/
/* set root buffer                                                            */
/*----------------------------------------------------------------------------*/
void IndexTree::vRootBuffer(EntBuffer* pRootBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  m_pRootEntBuffer = pRootBuffer;
}

/*----------------------------------------------------------------------------*/
/* truncate the index                                                         */
/*----------------------------------------------------------------------------*/
void IndexTree::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  m_okLock.vGrabWrite();
  
  if (m_pRootEntBuffer)
    m_pRootEntBuffer->vTruncate();
  
  m_pRootEntBuffer    = nullptr;
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* verify                                                                     */
/*----------------------------------------------------------------------------*/
bool IndexTree::bVerify(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  if (m_pRootEntBuffer)
    return m_pRootEntBuffer->bVerify();

  return true;
}

/*----------------------------------------------------------------------------*/
/* verify index from root                                                     */
/*----------------------------------------------------------------------------*/
bool IndexTree::bVerifyIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  if (m_pRootEntBuffer)
    return m_pRootEntBuffer->bVerify();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a key from row into EntWrk                                         */
/*----------------------------------------------------------------------------*/
void IndexTree::vExtractKey(EntWrk* pEntWrk, CrwBuffer* pCrwBuffer, i8 i_Row)
{
  // create a row
  HV_RowSingle hvRowSingle(pCrwBuffer->pStreamDef_dbuff());
  
  IndexDef* pIndexDef = m_pIndex->pIndexDef();
  
  i8 nIdxCols = pIndexDef->nColumns();
  
  for (i8 i_IdxCol = 0; i_IdxCol < nIdxCols; i_IdxCol++) {
    
    // get the column from the index column vector
    Column* pColumn = pIndexDef->pGetColumn(i_IdxCol);
    
    // mark column to be retrieved
    i8 iColumn = pColumn->iColumnNo();
    hvRowSingle->m_abGetCol[iColumn] = true;
  }
  
  // get the inflated compact row
  pCrwBuffer->vGetRow_single(hvRowSingle, i_Row);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (KE_INT == eKeyKind()) {
    
    Column* pCol = pIndexDef->pGetColumn(0);
    i8 iVal = hvRowSingle->iColVal(pCol->iColumnNo());
    pEntWrk->aEnt()->vKind(KE_INT);
    pEntWrk->aEnt()->vIntElm0(iVal);
    
    return;
  }
  
  for (i8 i_Column = 0; i_Column < nIdxCols; i_Column++) {
    
    Column* pColumn = pIndexDef->pGetColumn(i_Column);
    
    // add the column value to the key
    pEntWrk->vAppendElement(hvRowSingle, pColumn);
  }
  
}

/*----------------------------------------------------------------------------*/
/* get tree debug mode?                                                       */
/*----------------------------------------------------------------------------*/
bool IndexTree::bTreeDebug(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  return m_bTreeDebug;
}

/*----------------------------------------------------------------------------*/
/* set tree debug mode?                                                       */
/*----------------------------------------------------------------------------*/
void IndexTree::vTreeDebug(bool bDebug)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  m_bTreeDebug = bDebug;
}

/*----------------------------------------------------------------------------*/
/* get running delete count                                                   */
/*----------------------------------------------------------------------------*/
i8 IndexTree::nTreeDeleteCnt(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  return m_nTreeDeleteCnt;
}

/*----------------------------------------------------------------------------*/
/* inc running delete count                                                   */
/*----------------------------------------------------------------------------*/
void IndexTree::vTreeDeleteInc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  m_nTreeDeleteCnt++;
}

/*----------------------------------------------------------------------------*/
/* get running insert count                                                   */
/*----------------------------------------------------------------------------*/
i8 IndexTree::nTreeInsertCnt(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  return m_nTreeInsertCnt;
}

/*----------------------------------------------------------------------------*/
/* inc running insert count                                                   */
/*----------------------------------------------------------------------------*/
void IndexTree::vTreeInsertInc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  m_nTreeInsertCnt++;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* visualize the index                                                        */
/*----------------------------------------------------------------------------*/
void IndexTree::vDumpLevel(EntBuffer* pEntBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  // get the first entry
  Ent* aEnt = pEntBuffer->aLeadEntry();
  
  while (aEnt) {
    
    // get the index buffer the first enetry poinst to
    EntBuffer* pEntryEntBuffer = EntBuffer::pRefdEntBuffer(aEnt);
    
    // print the decile of the buffer
    vTrace("%lld"_Stz, pEntryEntBuffer->iDecile());
    
    // insert spaces for no of entries in buffer
    if (pEntryEntBuffer->iBufferHeight() > 0) {
      
      // exclude the head entry
      i8 iEntries = pEntryEntBuffer->nItems() - 1;
      
      while (iEntries--)
        vTrace(" "_Stz);
    }
    
    // move to the next entry
    aEnt = pEntBuffer->aNextEntry(aEnt, 0);
  }
}

/*----------------------------------------------------------------------------*/
/* visualize the index                                                        */
/*----------------------------------------------------------------------------*/
void IndexTree::vVisualize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXTREE,_FOOTER_);
  
  // start with the root buffer
  EntBuffer* pEntBuffer = m_pRootEntBuffer;
  
  if ( ! pEntBuffer)
    return;

  // print the decile of the root buffer
  vTrace("%lld\n"_Stz, pEntBuffer->iDecile());
  
  i8 iBufferHeight = pEntBuffer->iBufferHeight();
  
  // if there is only one buffer i.e root is at level 0, we are done
  if (iBufferHeight == 0) {
    vTrace("\n"_Stz);
    return;
  }
  
  // iterate through the buffers
  while (pEntBuffer) {
    
    // dump this buffer
    vDumpLevel(pEntBuffer);

    // if we are at level 1, iterate thru next buffers
    if (pEntBuffer->iBufferHeight() == 1) {
      pEntBuffer = (EntBuffer*)pEntBuffer->pNextEntBuffer();
    }
    else {
      vTrace("\n"_Stz);
      
      // get the first entry of the buffer
      Ent* aEnt = pEntBuffer->aLeadEntry();
      
      // get the index buffer this entry points to
      pEntBuffer = EntBuffer::pRefdEntBuffer(aEnt);
    }
  }
  
  vTrace("\n"_Stz);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(IndexTree);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/



