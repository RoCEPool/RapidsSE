/*============================================================================*/
/* GroupBy.cpp -- MockEE Group By                                             */
/*============================================================================*/

#include "MockEE.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
GroupBy* MockEE::_pGroupBy(void* aAddr)
{
  GroupBy* pGroupBy = reinterpret_cast<GroupBy*>(aAddr);
  
  RASSERT_TYPE(pGroupBy,CN_GROUPBY);
 
  return pGroupBy;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* GroupBy::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_GROUPBY);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void GroupBy::operator delete(void* pDataAddress)
{
  AccessPath* pAccessPath = reinterpret_cast<AccessPath*>(pDataAddress);
  
  HeapMem::vFree(pAccessPath);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
GroupBy::GroupBy(void)
: Object(CN_GROUPBY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GROUPBY,_FOOTER_);
  
  m_pAccumVec = new AccumVec;
  
  _checkInitialised_(CN_GROUPBY);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_GROUPBY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
GroupBy::~GroupBy(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GROUPBY,_FOOTER_);
  
  delete m_pAccumVec;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* apply entry or lineitems                                                   */
/*----------------------------------------------------------------------------*/
void GroupBy::vApplyIndex(Entry* pIndexEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GROUPBY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* apply buffer or lineitems                                                  */
/*----------------------------------------------------------------------------*/
void GroupBy::vApplyBuffer(EntryBuffer* pEntryBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GROUPBY,_FOOTER_);
  
  Entry* pEntry = pEntryBuffer->pFirstEntry();
  
    while (pEntry) {
    
    LineItem* pLineItem = (LineItem*)pEntry->aRowData();
    
    //    vTrace("\nEntry: %p LineItem: %p String: %p \n>>%s<<\n"_Stz,
    //           pEntry,pLineItem,pLineItem->aStz(),pLineItem->aStz());
    //    MemLib::vHexDump(pEntry,512);
    //    vTrace("\n"_Stz);
    
    // extract the order key (its the first field)
    Row* aRow = pEntry->aRow();
    char* aCursor = aRow->aColumnDatum(0);
    i8 iOrderKey = *((i8*)aCursor);
    
    // extract the quantity (its after the fifth field)
    aCursor = aRow->aColumnDatum(5);
    i8 iQuantity = *((i8*)aCursor);
    
    // compute which group
    i8 i_Group = iOrderKey % 10;
    Accum* pAccum = m_pAccumVec->pGet(i_Group);
    
    // accumulate
    pAccum->m_iSum += iQuantity;
    if (iQuantity < pAccum->m_iMin)
      pAccum->m_iMin = iQuantity;
    if (iQuantity > pAccum->m_iMax)
      pAccum->m_iMax = iQuantity;
    pAccum->m_iCount++;
     
    //get next entry
    pEntry = pEntryBuffer->pNextEntry(pEntry, 0);
  }
  
}

/*----------------------------------------------------------------------------*/
/* apply entry or lineitems                                                   */
/*----------------------------------------------------------------------------*/
void GroupBy::vApplyEntry(Entry* pLineItemEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GROUPBY,_FOOTER_);
  
  
  LineItem* pLineItem = (LineItem*)pLineItemEntry->aRowData();
  
  //    vTrace("\nEntry: %p LineItem: %p String: %p \n>>%s<<\n"_Stz,
  //           pEntry,pLineItem,pLineItem->aStz(),pLineItem->aStz());
  //    MemLib::vHexDump(pEntry,512);
  //    vTrace("\n"_Stz);
  
  // extract the order key (its the first field)
  Row* aRow = pLineItemEntry->aRow();
  char* aCursor = aRow->aColumnDatum(0);
  i8 iOrderKey = *((i8*)aCursor);
  
  // extract the quantity (its after the fifth field)
  aCursor = aRow->aColumnDatum(5);
  i8 iQuantity = *((i8*)aCursor);
  
  // compute which group
  i8 i_Group = iOrderKey % 10;
  Accum* pAccum = m_pAccumVec->pGet(i_Group);
  
  // accumulate
  pAccum->m_iSum += iQuantity;
  if (iQuantity < pAccum->m_iMin)
    pAccum->m_iMin = iQuantity;
  if (iQuantity > pAccum->m_iMax)
    pAccum->m_iMax = iQuantity;
  pAccum->m_iCount++;
}

/*----------------------------------------------------------------------------*/
/* apply partial group by results                                             */
/*----------------------------------------------------------------------------*/
void GroupBy::vApplyPartial(GroupBy* pPartialGroupBy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GROUPBY,_FOOTER_);
  
  // print all the accumulators
  for (i8 i_Group = 0; i_Group < m_pAccumVec->nItems(); i_Group++) {
    Accum* pAccum = m_pAccumVec->pGet(i_Group);
    Accum* pPartialAccum = pPartialGroupBy->m_pAccumVec->pGet(i_Group);
    pAccum->m_iCount += pPartialAccum->m_iCount;
    pAccum->m_iSum += pPartialAccum->m_iSum;
    pAccum->m_iMin = pAccum->m_iMin < pPartialAccum->m_iMin ?
      pAccum->m_iMin : pPartialAccum->m_iMin;
    pAccum->m_iMax = pAccum->m_iMax > pPartialAccum->m_iMax ?
      pAccum->m_iMax : pPartialAccum->m_iMax;
  }
}

/*----------------------------------------------------------------------------*/
/* end group by accumulation, print the accumulators                          */
/*----------------------------------------------------------------------------*/
void GroupBy::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GROUPBY,_FOOTER_);
  
  // print all the accumulators
  for (i8 i_Group = 0; i_Group < m_pAccumVec->nItems(); i_Group++) {
    Accum* pAccum = m_pAccumVec->pGet(i_Group);
    vTrace("Group: %lld Count: %lld Sum: %lld Min: %lld Max: %lld\n"_Stz,
           pAccum->m_iGroup,
           pAccum->m_iCount,
           pAccum->m_iSum,
           pAccum->m_iMin,
           pAccum->m_iMax
           );
  }
}

/*----------------------------------------------------------------------------*/
/* start group by accumulation, create accumulators                           */
/*----------------------------------------------------------------------------*/
void GroupBy::vStart(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GROUPBY,_FOOTER_);
  
  // create an accum for each group
  for (i8 i_Group = 0; i_Group < 10; i_Group++) {
    Accum* pAccum = new Accum;
    pAccum->m_iGroup = i_Group;
    m_pAccumVec->vAdd(pAccum);
  }
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

_aFooter_(GroupBy);

/*----------------------------------------------------------------------------*/
/* convert index entry to line item entry                                     */
/*----------------------------------------------------------------------------*/
Entry* GroupBy::pToLineItemEntry(Index* pIndex, Entry* pEntry)
{  
  // get the table from the index
  Table* pTable = pIndex->pTable();
  
  // get the hiddenindex from the table
  Index* pHiddenIndex = pTable->pHiddenIndex();
  
  // get the Id from the entry
  Id xId = pEntry->xId();
  
  // find the buffer from the buffer pool given the buffer no
  bn nBufferNo = xId.iBuffer();
  EntryBuffer* pEntryBuffer = reinterpret_cast<EntryBuffer*>
  (gpBufferPool->pToBuffer(nBufferNo));
  
  // form the hidden index key using the row id from the entry
  KeyWrk xKeyWrk;
  GenVar xValWrk;
  xValWrk = xId.iRow();
  
  // hiddenIndex should have only one column
  ASSERT(pHiddenIndex->pIdxColVec()->nItems() == 1);
  
  // get the 0th column of the hidden index
  Column* pColumn = pHiddenIndex->pIdxColVec()->pGet(0);
  ASSERT(pColumn->iColumnNo() == -1);
  
  // add the rowId to the key
  xKeyWrk.vAddDatum(xValWrk, pColumn);
  
  // position in the entry buffer at the row id key
  Entry* pLineItemEntry = pEntryBuffer->pPositionQuery(xKeyWrk.aKey(), 1, JC_AFTER);
  
  return pLineItemEntry;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
