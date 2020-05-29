/*============================================================================*/
/* RefBuffer.cpp -- Reference Buffer                                          */
/*============================================================================*/

#include "Rows.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RefBuffer* Rows::_pRefBuffer(void* aAddress)
{
  RefBuffer* pRefBuffer = reinterpret_cast<RefBuffer*>(aAddress);
  
  return pRefBuffer;
}

/*----------------------------------------------------------------------------*/
/* allocate a new ref buffer from heap                                        */
/*----------------------------------------------------------------------------*/
void* RefBuffer::operator new(size_t)
{
  RefBuffer* pRefBuffer =
    static_cast<RefBuffer*>(HeapMem::pAllocate(CN_REFBUFFER));
  
  return pRefBuffer;
}

/*----------------------------------------------------------------------------*/
/* free a ref buffer back to the heap                                         */
/*----------------------------------------------------------------------------*/
void RefBuffer::operator delete(void* aAddress)
{
  RefBuffer* pRefBuffer = static_cast<RefBuffer*>(aAddress);
  
  HeapMem::vFree(pRefBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RefBuffer::RefBuffer(i8 iPartition)
: DataBuffer(CN_REFBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REFBUFFER,_FOOTER_);
  
  m_bIsLast = false;
  m_iPartitionNo = iPartition;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_REFBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_REFBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RefBuffer::~RefBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REFBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump reference buffer                                                      */
/*----------------------------------------------------------------------------*/
void RefBuffer::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RefBuffer* pRefBuffer = _pRefBuffer(pObject);
  
  //pRefBuffer->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot a ref set in this buffer (may return nulltpr)                        */
/*----------------------------------------------------------------------------*/
RefSet* RefBuffer::aAllotRefSet(i8 nRowRefs)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REFBUFFER,_FOOTER_);
  
  // allot using bytes as we don't know the size of the item
  byte* ayRefSet = ayAllotBytes(RefSet::nBytesReq(nRowRefs));
  
  if (! ayRefSet)
    return nullptr;
  
  vIncItems();
  
  RefSet* aRefSet = new(ayRefSet) RefSet(nRowRefs);
  
  return aRefSet;
}

/*----------------------------------------------------------------------------*/
/* return first ref set in this buffer                                        */
/*----------------------------------------------------------------------------*/
RefSet* RefBuffer::aFirstRefSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REFBUFFER,_FOOTER_);
  
  RefSet* aRefSet = aFirstItem();
  
  return aRefSet;
}

/*----------------------------------------------------------------------------*/
/* set buffer is last                                                         */
/*----------------------------------------------------------------------------*/
void RefBuffer::vIsLast(bool bIsLast)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REFBUFFER,_FOOTER_);
  
  m_bIsLast = bIsLast;
}

/*----------------------------------------------------------------------------*/
/* get if buffer is last                                                      */
/*----------------------------------------------------------------------------*/
bool RefBuffer::bIsLast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REFBUFFER,_FOOTER_);
  
  return m_bIsLast;
}

/*----------------------------------------------------------------------------*/
/* return next ref set in this buffer                                         */
/*----------------------------------------------------------------------------*/
RefSet* RefBuffer::aNextRefSet(RefSet* aPriorRefSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REFBUFFER,_FOOTER_);

  RefSet* aRefSet = aAdjacentItem(aPriorRefSet);
  
  if (aRefSet && CN_REFSET == aRefSet->eClass())
    return aRefSet;

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* show contents of buffer                                                    */
/*----------------------------------------------------------------------------*/
void RefBuffer::vShow(ColumnVec* pLeftColVec, ColumnVec* pRightColVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REFBUFFER,_FOOTER_);
  
//  i8 nRefSets = nItems();
//
//  vTrace("RefBuffer: %p (buffer %lld): rows: %lld\n"_Stz,
//         this,m_iBufferNo,nRefSets);
//
//  vTrace("  prevBuffer %p nextBuffer %p\n"_Stz,
//         m_pPrevBuffer,m_pNextBuffer);
//
//  vTrace("  partition: %lld \n"_Stz, m_iPartitionNo);
//
//  HV_Rec hvRecLeft(pLeftColVec);
//  HV_Row hRowLeft(pLeftColVec);
//
//  HV_Rec hvRecRight(pRightColVec);
//  HV_Row hRowRight(pRightColVec);
//
//  RefSet* aRefSet = aFirstRefSet();
//
//  i8 nRows = nItems();
//  while (nRows--) {
//
//    // get the left dataRow
//    Crw* aLeftCrw = (Crw*)(aRefSet->iRowRef(0));
//
//    // get the right dataRow
//    Crw* aRightCrw = (Crw*)(aRefSet->iRowRef(1));
//
//
//    vTrace("RowRefs %p %p\n"_Stz, aLeftCrw, aRightCrw);
//
//    // inflate the left row
//    if (aLeftCrw) {
//      RowInflator::vInflateRow(hRowLeft, aLeftCrw);
//    } else {
//      // get no of columns
//      i8 nColumns = pLeftColVec->nItems();
//
//      // for each column
//      for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
//        hRowLeft->m_waColVal[i_Col].m_iI8 = C_DATUM_NULL;
//      }
//    }
//    hvRecLeft->vAppend(hRowLeft);
//
//
//
//    if (aRightCrw) {
//      // inflate the right row
//      RowInflator::vInflateRow(hRowRight, aRightCrw);
//    }
//    else {
//      // get no of columns
//      i8 nColumns = pRightColVec->nItems();
//
//      // for each column
//      for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
//        hRowRight->m_waColVal[i_Col].m_iI8 = C_DATUM_NULL;
//      }
//    }
//    hvRecRight->vAppend(hRowRight);
//
//    vTrace("%p: %p %s %s\n"_Stz, aRefSet, aRefSet->iRowRef(0),
//           hvRecLeft->szRecStr(), hvRecRight->szRecStr());
//
//    aRefSet = aNextRefSet(aRefSet);
//  }
//
//  vTrace("--------------------------------------------------"
//         "--------------------------------------------------\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
SrtBuffer* RefBuffer::pToSrtBuffer(i8 iKeyCol, bool bKeyIsLeft,
                                   ColumnVec* pLeftColVec, ColumnVec* pRightColVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REFBUFFER,_FOOTER_);
  
//  i8 nRowRefs = 2;
//
//  SrtBuffer* pSrtBuffer = new SrtBuffer(nRowRefs, m_iPartitionNo);
//
//  SrtSet* pSrtSet;
//
//  CrwBuffer* pLeftCrwBuffer;
//  CrwBuffer* pRightCrwBuffer;
//
//  Crw* aLeftCrw;
//  Crw* aRightCrw;
//
//  RefSet* aRefSet = aFirstRefSet();
//  while (aRefSet) {
//
//    pSrtSet = pSrtBuffer->aAllotSrtSet();
//    if ( ! pSrtSet ) {
//
//      // create a new SrtBuffer
//      SrtBuffer* pNewSrtBuffer = new SrtBuffer(nRowRefs, m_iPartitionNo);
//      pNewSrtBuffer->vNextBuffer(pSrtBuffer);
//      pSrtBuffer = pNewSrtBuffer;
//      pSrtSet = pSrtBuffer->aAllotSrtSet();
//
//    }
//    RASSERT(pSrtSet);
//
//    // get the left data row
//    aLeftCrw = (Crw*)aRefSet->iRowRef(0);
//
//    // set the rowData in the srtSet
//    pSrtSet->vRowRef(0, (i8)aLeftCrw);
//
//    // get the right dataRow
//    aRightCrw = (Crw*)aRefSet->iRowRef(1);
//
//    // set the rowData in the srtSet
//    pSrtSet->vRowRef(1, (i8)aRightCrw);
//
//    if ( bKeyIsLeft) {
//      HV_Row hRow(pLeftColVec);
//
//      // get the key from the left buffer row
//      if (aLeftCrw) {
//        RowInflator::vInflateColumn(hRow, aLeftCrw, iKeyCol);
//
//        // set the key in the srtSet
//        pSrtSet->vKey(hRow->m_waColVal[iKeyCol].m_iI8);
//      }
//      else
//        pSrtSet->vKey(C_DATUM_NULL);
//    }
//    else {
//      HV_Row hRow(pRightColVec);
//
//      // get the key from the left buffer row
//      if (aRightCrw) {
//        RowInflator::vInflateColumn(hRow, aRightCrw, iKeyCol);
//
//        // set the key in the srtSet
//        pSrtSet->vKey(hRow->m_waColVal[iKeyCol].m_iI8);
//      }
//      else
//        pSrtSet->vKey(C_DATUM_NULL);
//    }
//
//    aRefSet = aNextRefSet(aRefSet);
//  }
//
//  return pSrtBuffer;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* RefBuffer::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REFBUFFER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
