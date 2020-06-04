/*============================================================================*/
/* RsbBuffer.hpp -- Row Slab Buffer                                           */
/*============================================================================*/

#include "DataBuffers.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(DataBuffers,RsbBuffer,CN_RSBBUFFER)
__aNewFunction(RsbBuffer,CN_RSBBUFFER)
__vDeleteFunction(RsbBuffer,CN_RSBBUFFER)
__aFooterFunction(RsbBuffer)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RsbBuffer::RsbBuffer(void)
: DataBuffer(CN_RSBBUFFER,false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RSBBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_RSBBUFFER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_RSBBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RsbBuffer::~RsbBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RSBBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get row slab                                                               */
/*----------------------------------------------------------------------------*/
RowSlab* RsbBuffer::pRowSlab(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RSBBUFFER,_FOOTER_);
  
  return & m_oRowSlab;
}

//*============================================================================*/
/* end                                                                        */
/*============================================================================*/
