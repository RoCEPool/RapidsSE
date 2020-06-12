/*============================================================================*/
/* LrwBuffer.cpp -- Lrw Buffer                                                */
/*============================================================================*/

#include "CompactBuffers.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(CompactBuffers,LrwBuffer,CN_LRWBUFFER)
__aNewFunction(LrwBuffer,CN_LRWBUFFER)
__vDeleteFunction(LrwBuffer,CN_LRWBUFFER)
__aFooterFunction(LrwBuffer)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LrwBuffer::LrwBuffer(void)
: DataBuffer(CN_LRWBUFFER, false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LRWBUFFER,_FOOTER_);
    
  m_nItems = 0;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LRWBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_LRWBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LrwBuffer::~LrwBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LRWBUFFER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot lrw                                                                  */
/*----------------------------------------------------------------------------*/
Lrw* LrwBuffer::pAllotLrw(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LRWBUFFER,_FOOTER_);
  
  Lrw* pLrw = _pLrw(m_xWsp.ayAllot(nBytes));
  
  if (! pLrw)
    return nullptr;
  
  m_nItems++;
  return pLrw;
}

/*----------------------------------------------------------------------------*/
/* each lrw                                                                   */
/*----------------------------------------------------------------------------*/
Lrw* LrwBuffer::pEach(Lrw* pLrwPrev)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LRWBUFFER,_FOOTER_);
  
  Lrw* pLrw;
  
  if (! pLrwPrev) {
    if (! m_nItems)
      return nullptr;
  
    pLrw = _pLrw(m_xWsp.ayArea());
    return pLrw;
  }
  
  i8 nPrevBytes = pLrwPrev->nSize();
  
  pLrw = pAdd<Lrw*>(pLrwPrev,nPrevBytes);
  
  if (pLrw >= _pLrw(m_xWsp.ayLatter()))
    return nullptr;
  
  return pLrw;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
