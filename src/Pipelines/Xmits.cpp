/*============================================================================*/
/* Xmits.cpp -- Consume xmits counters                                        */
/*============================================================================*/

#include "Pipelines.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Xmits* Pipelines::_pXmits(void* aAddr)
{
  Xmits* pXmits = reinterpret_cast<Xmits*>(aAddr);
  
  return pXmits;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* Xmits::operator new(size_t)
{
  Xmits* pXmits =
  static_cast<Xmits*>(HeapMem::pAllocate(CN_XMITS));
  
  return pXmits;
}

/*----------------------------------------------------------------------------*/
/* free back to the heap                                                      */
/*----------------------------------------------------------------------------*/
void Xmits::operator delete(void* aAddress)
{
  Object* pObject = static_cast<Object*>(aAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Xmits::Xmits(i8 iXmits)
: Focal(CN_XMITS)

{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITS,_FOOTER_);

  m_iXmits  = iXmits;
  
  m_iCancelMarkers  = 0;
  m_iDataPulses     = 0;
  m_iDestroyMarkers = 0;
  m_iExitMarkers    = 0;
  m_iEosMarkers     = 0;
  m_iPulsedMarkers  = 0;
  m_iStartMarkers   = 0;
  m_iStreamDefs     = 0;
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Xmits::~Xmits(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITS,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/* cancel done ?                                                              */
/*----------------------------------------------------------------------------*/
bool Xmits::bCancelDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITS,_FOOTER_);
  
  vTrace("Warning: ConsumeOp received ZK_CANCELMARKER %lld; expected %lld\n"_Stz,
         m_iCancelMarkers, m_iXmits);
  
  if (m_iCancelMarkers == m_iXmits) {
    //m_iCancelMarkers = 0;
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* data pulse done ?                                                          */
/*----------------------------------------------------------------------------*/
bool Xmits::bDataPulseDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITS,_FOOTER_);
  
  if (m_iDataPulses > m_iXmits)
    vTrace("Warning: ConsumeOp ZK_DATAPULSE received %lld; expected %lld\n"_Stz,
           m_iDataPulses, m_iXmits);
  
  //if (m_iDataPulses == m_iXmits) {
  if (m_iDataPulses == 0) {
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* destroy done ?                                                             */
/*----------------------------------------------------------------------------*/
bool Xmits::bDestroyDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITS,_FOOTER_);
  
  if (m_iDestroyMarkers > m_iXmits)
  vTrace("Warning: ConsumeOp received ZK_DESTROYMARKER %lld; expected %lld\n"_Stz,
         m_iDestroyMarkers, m_iXmits);
  
  if (m_iDestroyMarkers == m_iXmits) {
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* eos done ?                                                                 */
/*----------------------------------------------------------------------------*/
bool Xmits::bEosDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITS,_FOOTER_);
  
  if (m_iEosMarkers > m_iXmits)
    vTrace("Warning: ConsumeOp received ZK_EOSMARKER %lld; expected %lld\n"_Stz,
           m_iEosMarkers, m_iXmits);
  
  if (m_iEosMarkers == m_iXmits) {
    //m_iEosMarkers = 0;
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* error done ?                                                               */
/*----------------------------------------------------------------------------*/
bool Xmits::bErrorDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITS,_FOOTER_);
  
  if (m_iEosMarkers > m_iXmits)
    vTrace("Warning: ConsumeOp received ZK_ERRORMARKER %lld; expected %lld\n"_Stz,
           m_iEosMarkers, m_iXmits);
  
  if (m_iEosMarkers == m_iXmits) {
    //m_iEosMarkers = 0;
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* exit done ?                                                                */
/*----------------------------------------------------------------------------*/
bool Xmits::bExitDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITS,_FOOTER_);
  
  if (m_iExitMarkers > m_iXmits)
    vTrace("Warning: ConsumeOp received ZK_EXITMARKER %lld; expected %lld\n"_Stz,
      m_iExitMarkers, m_iXmits);
  
  if (m_iExitMarkers == m_iXmits) {
    //m_iExitMarkers = 0;
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* pulsed done ?                                                              */
/*----------------------------------------------------------------------------*/
bool Xmits::bPulsedDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITS,_FOOTER_);
  
  if (m_iPulsedMarkers > m_iXmits)
    vTrace("Warning: ConsumeOp ZK_PULSEDMARKER received %lld; expected %lld\n"_Stz,
           m_iPulsedMarkers, m_iXmits);
  
  if (m_iPulsedMarkers == m_iXmits) {
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* start done ?                                                               */
/*----------------------------------------------------------------------------*/
bool Xmits::bStartDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITS,_FOOTER_);
  
  if (m_iStartMarkers > m_iXmits)
    vTrace("Warning: ConsumeOp ZK_STARTMARKER received %lld; expected %lld\n"_Stz,
           m_iStartMarkers, m_iXmits);
  
  if (m_iStartMarkers == 0) {
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* streamDef done ?                                                           */
/*----------------------------------------------------------------------------*/
bool Xmits::bStreamDefDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITS,_FOOTER_);
  
  if (m_iStreamDefs > m_iXmits)
    vTrace("Warning: ConsumeOp ZK_STREAMDEF received %lld; expected %lld\n"_Stz,
           m_iStreamDefs, m_iXmits);
  
  if (m_iStreamDefs == 0) {
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* inc cancel                                                                 */
/*----------------------------------------------------------------------------*/
void Xmits::vIncCancel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITS,_FOOTER_);
  
  m_iCancelMarkers++;
}

/*----------------------------------------------------------------------------*/
/* inc data pulse                                                             */
/*----------------------------------------------------------------------------*/
void Xmits::vIncDataPulse(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITS,_FOOTER_);
  
  m_iDataPulses++;
}

/*----------------------------------------------------------------------------*/
/* inc destroy                                                                 */
/*----------------------------------------------------------------------------*/
void Xmits::vIncDestroy(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITS,_FOOTER_);
  
  m_iDestroyMarkers++;
}

/*----------------------------------------------------------------------------*/
/* inc eos                                                                    */
/*----------------------------------------------------------------------------*/
void Xmits::vIncEos(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITS,_FOOTER_);
  
  m_iEosMarkers++;
}

/*----------------------------------------------------------------------------*/
/* inc error                                                                  */
/*----------------------------------------------------------------------------*/
void Xmits::vIncError(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITS,_FOOTER_);
  
  m_iEosMarkers++;
}

/*----------------------------------------------------------------------------*/
/* inc exit                                                                   */
/*----------------------------------------------------------------------------*/
void Xmits::vIncExit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITS,_FOOTER_);
  
  m_iExitMarkers++;
}

/*----------------------------------------------------------------------------*/
/* inc pulsed                                                                 */
/*----------------------------------------------------------------------------*/
void Xmits::vIncPulsed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITS,_FOOTER_);
  
  m_iPulsedMarkers++;
}

/*----------------------------------------------------------------------------*/
/* inc start                                                                  */
/*----------------------------------------------------------------------------*/
void Xmits::vIncStart(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITS,_FOOTER_);
  
  m_iStartMarkers++;
}

/*----------------------------------------------------------------------------*/
/* inc streamDef                                                              */
/*----------------------------------------------------------------------------*/
void Xmits::vIncStreamDef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XMITS,_FOOTER_);
  
  m_iStreamDefs++;
}

/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
__aFooterFunction(Xmits);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
