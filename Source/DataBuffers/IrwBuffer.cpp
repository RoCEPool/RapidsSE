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
  
  Row* pRow = aAllotRow(pRowSingle);
  
  if ( ! pRow )
    return false;
  else
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
