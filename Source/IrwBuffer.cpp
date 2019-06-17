/*============================================================================*/
/* IrwBuffer.hpp -- Inflated Row Buffer                                       */
/*============================================================================*/

#include "PushDown.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
IrwBuffer* Transform::_pIrwBuffer(void* aAddress)
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
: DataBuffer(CN_IRWBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IRWBUFFER,_FOOTER_);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_IRWBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_IRWBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
IrwBuffer::~IrwBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IRWBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump reference buffer                                                      */
/*----------------------------------------------------------------------------*/
void IrwBuffer::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  IrwBuffer* pIrwBuffer = _pIrwBuffer(pObject);
  
  pIrwBuffer->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot a ref set in this buffer (may return nulltpr)                        */
/*----------------------------------------------------------------------------*/
Row* IrwBuffer::aAllotRow(ColumnVec* pColVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IRWBUFFER,_FOOTER_);
  
  // allot using bytes as we don't know the size of the item
  byte* ayRow = ayAllotBytes(sizeof(Row));
  
  if (! ayRow)
    return nullptr;
  
  vIncItems();
  
  Row* aRow = new(ayRow) Row(pColVec);
  
  return aRow;
}

/*----------------------------------------------------------------------------*/
/* return first ref set in this buffer                                        */
/*----------------------------------------------------------------------------*/
Row* IrwBuffer::aFirstRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IRWBUFFER,_FOOTER_);
  
  Row* aRow = aFirstItem();
  
  return aRow;
}

/*----------------------------------------------------------------------------*/
/* return next ref set in this buffer                                         */
/*----------------------------------------------------------------------------*/
Row* IrwBuffer::aNextRow(Row* aPriorRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IRWBUFFER,_FOOTER_);

  Row* aRow = aAdjacentItem(aPriorRow);
  
  if (aRow && CN_REFSET == aRow->eClass())
    return aRow;

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* show contents of buffer                                                    */
/*----------------------------------------------------------------------------*/
void IrwBuffer::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IRWBUFFER,_FOOTER_);
  
  i8 nRows = nItems();
  
  vTrace("IrwBuffer: %p (buffer %lld): rows: %lld\n"_Stz,
         this,m_iBufferNo,nRows);
  
  vTrace("  prevBuffer %p nextBuffer %p\n"_Stz,
         m_pPrevBuffer,m_pNextBuffer);
  
  vTrace("  partition: %lld \n"_Stz, m_iPartitionNo);
  
  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
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
