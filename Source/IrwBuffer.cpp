/*============================================================================*/
/* IrwBuffer.hpp -- Inflated Row Buffer                                       */
/*============================================================================*/

#include "DataBuffers.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
IrwBuffer* DataBuffers::_pIrwBuffer(void* aAddress)
{
  IrwBuffer* pIrwBuffer = reinterpret_cast<IrwBuffer*>(aAddress);
  
  return pIrwBuffer;
}

/*----------------------------------------------------------------------------*/
/* allocate a new ref buffer from heap                                        */
/*----------------------------------------------------------------------------*/
void* IrwBuffer::operator new(size_t)
{
  IrwBuffer* pIrwBuffer =
    static_cast<IrwBuffer*>(HeapMem::pAllocate(CN_IRWBUFFER));
  
  return pIrwBuffer;
}

/*----------------------------------------------------------------------------*/
/* free a ref buffer back to the heap                                         */
/*----------------------------------------------------------------------------*/
void IrwBuffer::operator delete(void* aAddress)
{
  IrwBuffer* pIrwBuffer = static_cast<IrwBuffer*>(aAddress);
  
  HeapMem::vFree(pIrwBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
IrwBuffer::IrwBuffer(void)
: RowBuffer(CN_IRWBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IRWBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_IRWBUFFER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_IRWBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
IrwBuffer::~IrwBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IRWBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* copy row into buffer from single row                                       */
/*----------------------------------------------------------------------------*/
bool IrwBuffer::bCopyRow(RowSingle* pRowSingle)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IRWBUFFER,_FOOTER_);
  
  StreamDef* pStreamDef = pRowSingle->m_pStreamDef;
  
  i8 nHeadSize = Row::nSizeBytes(pStreamDef);
  i8 nTailSize = pRowSingle->nSizeVch();
  
  if (nFreeBytes() < nHeadSize + nTailSize)
    return false;
  
  Row* pRow = aAllotRow(nHeadSize);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy single row into this buffer
  
  for (i8 i_Col = 0; i_Col < pStreamDef->nColumns(C_ALL); i_Col++) {
    
    Column* pCol = pStreamDef->pGetColumn(i_Col);
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // vch
    
    if (DM_VCH == pCol->eDatum()) {
      
      if (pRowSingle->m_waColVal[i_Col].bIsNull()) {
        pRow->m_waColVal[i_Col].vSetNull();
        continue;
      }
      
      char* acSrcString = (char*)pRowSingle->m_waColVal[i_Col].m_szVch;
      i8 nSrcLength = StrLib::iSize(acSrcString);
      
      char* acDstString = (char*)ayAllotTail(nSrcLength);
      i8 nCopyBytes = StrLib::nCopy(acDstString, acSrcString);
      ASSERT(nCopyBytes + 1 == nSrcLength);
      
      pRow->m_waColVal[i_Col].m_azStr = acDstString;
      
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // all others
    
    pRow->m_waColVal[i_Col].m_iWrd = pRowSingle->m_waColVal[i_Col].m_iWrd;
  }
  
#ifdef WITH_ENCODING_TRACE
  SP_Row spRow;
  vTrace("pSampleRow:: %s\n"_Stz,spRow.sz(pRowSingle));
#endif

  //pRow->vDump(pStreamDef);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* IrwBuffer::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IRWBUFFER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
