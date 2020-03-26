/*============================================================================*/
/* ConsumeXmits.cpp -- Consume xmits counters                                 */
/*============================================================================*/

#include "Pipelines.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ConsumeXmits* Pipelines::_pConsumeXmits(void* aAddr)
{
  ConsumeXmits* pConsumeXmits = reinterpret_cast<ConsumeXmits*>(aAddr);
  
  return pConsumeXmits;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* ConsumeXmits::operator new(size_t)
{
  ConsumeXmits* pConsumeXmits =
  static_cast<ConsumeXmits*>(HeapMem::pAllocate(CN_CONSUMEXMITS));
  
  return pConsumeXmits;
}

/*----------------------------------------------------------------------------*/
/* free back to the heap                                                      */
/*----------------------------------------------------------------------------*/
void ConsumeXmits::operator delete(void* aAddress)
{
  Object* pObject = static_cast<Object*>(aAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ConsumeXmits::ConsumeXmits(i8 iConsumeXmits)
: Focal(CN_CONSUMEXMITS)

{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CONSUMEXMITS,_FOOTER_);

  m_iConsumeXmits  = iConsumeXmits;
  m_iExitMarkers   = 0;
  m_iEosMarkers    = 0;
  m_iPulseMarkers  = 0;
  m_iPulsedMarkers = 0;
  m_iStartMarkers  = 0;
  m_iStreamDefs    = 0;
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_CONSUMEXMITS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ConsumeXmits::~ConsumeXmits(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CONSUMEXMITS,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/* cancel done ?                                                              */
/*----------------------------------------------------------------------------*/
bool ConsumeXmits::bCancelDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CONSUMEXMITS,_FOOTER_);
  
  vTrace("Warning: ConsumeOp ZK_CANCELMARKER received %v; expected %v"_Stz,
         m_iExitMarkers, m_iConsumeXmits);
  
  return (m_iExitMarkers == m_iConsumeXmits ? true : false);
}

/*----------------------------------------------------------------------------*/
/* exit done ?                                                                */
/*----------------------------------------------------------------------------*/
bool ConsumeXmits::bExitDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CONSUMEXMITS,_FOOTER_);
  
  if (m_iExitMarkers > m_iConsumeXmits)
    vTrace("Warning: ConsumeOp ZK_EXITMARKER received %v; expected %v"_Stz,
      m_iExitMarkers, m_iConsumeXmits);
  
  return (m_iExitMarkers == m_iConsumeXmits ? true : false);
}

/*----------------------------------------------------------------------------*/
/* inc cancel                                                                 */
/*----------------------------------------------------------------------------*/
void ConsumeXmits::vIncCancel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CONSUMEXMITS,_FOOTER_);
  
  m_iExitMarkers++;
}

/*----------------------------------------------------------------------------*/
/* inc exit                                                                   */
/*----------------------------------------------------------------------------*/
void ConsumeXmits::vIncExit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CONSUMEXMITS,_FOOTER_);
  
  m_iExitMarkers++;
}

/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
__aFooterFunction(ConsumeXmits);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
