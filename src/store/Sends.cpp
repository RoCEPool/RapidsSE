/*============================================================================*/
/* Sends.cpp -- Consume Sends counters                                        */
/*============================================================================*/

#include "BASE.h"

#include "Sends.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Sends* STORE::_pSends(void* aAddr)
{
  Sends* pSends = reinterpret_cast<Sends*>(aAddr);
  
  return pSends;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* Sends::operator new(size_t)
{
  Sends* pSends =
  static_cast<Sends*>(Heaps::pAllocate(CN_SENDS));
  
  return pSends;
}

/*----------------------------------------------------------------------------*/
/* free back to the heap                                                      */
/*----------------------------------------------------------------------------*/
void Sends::operator delete(void* aAddress)
{
  Object* pObject = static_cast<Object*>(aAddress);
  
  Heaps::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Sends::Sends(i8 iSends)
: Object(CN_SENDS)

{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);

  m_iSends  = iSends;
  
  m_iCancelMarkers  = 0;
  m_iDataPulses     = 0;
  m_iDestroyMarkers = 0;
  m_iExitMarkers    = 0;
  m_iEosMarkers     = 0;
  m_iPulsedMarkers  = 0;
  m_iStartMarkers   = 0;
  m_iStreamDefs     = 0;
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Sends::~Sends(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/* cancel done ?                                                              */
/*----------------------------------------------------------------------------*/
bool Sends::bCancelDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);
  
  vTrace("Warning: ConsumeOp received ZK_CANCELMARKER %lld; expected %lld\n"_Stz,
         m_iCancelMarkers, m_iSends);
  
  if (m_iCancelMarkers == m_iSends) {
    //m_iCancelMarkers = 0;
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* cleaup done ?                                                              */
/*----------------------------------------------------------------------------*/
bool Sends::bCleanupDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);

  vTrace("Warning: ConsumeOp received ZK_CLEANUPMARKER %lld; expected %lld\n"_Stz,
         m_iCleanupMarkers, m_iSends);

  if (m_iCleanupMarkers == m_iSends) {
    //m_iCleanupMarkers = 0;
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* data pulse done ?                                                          */
/*----------------------------------------------------------------------------*/
bool Sends::bDataPulseDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);
  
  if (m_iDataPulses > m_iSends)
    vTrace("Warning: ConsumeOp ZK_DATAPULSE received %lld; expected %lld\n"_Stz,
           m_iDataPulses, m_iSends);
  
  //if (m_iDataPulses == m_iSends) {
  if (m_iDataPulses == 0) {
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* destroy done ?                                                             */
/*----------------------------------------------------------------------------*/
bool Sends::bDestroyDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);
  
  if (m_iDestroyMarkers > m_iSends)
  vTrace("Warning: ConsumeOp received ZK_DESTROYMARKER %lld; expected %lld\n"_Stz,
         m_iDestroyMarkers, m_iSends);
  
  if (m_iDestroyMarkers == m_iSends) {
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* eos done ?                                                                 */
/*----------------------------------------------------------------------------*/
bool Sends::bEosDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);
  
  if (m_iEosMarkers > m_iSends)
    vTrace("Warning: ConsumeOp received ZK_EOSMARKER %lld; expected %lld\n"_Stz,
           m_iEosMarkers, m_iSends);
  
  if (m_iEosMarkers == m_iSends) {
    //m_iEosMarkers = 0;
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* error done ?                                                               */
/*----------------------------------------------------------------------------*/
bool Sends::bErrorDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);
  
  if (m_iEosMarkers > m_iSends)
    vTrace("Warning: ConsumeOp received ZK_ERRORMARKER %lld; expected %lld\n"_Stz,
           m_iEosMarkers, m_iSends);
  
  if (m_iEosMarkers == m_iSends) {
    //m_iEosMarkers = 0;
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* exit done ?                                                                */
/*----------------------------------------------------------------------------*/
bool Sends::bExitDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);
  
  if (m_iExitMarkers > m_iSends)
    vTrace("Warning: ConsumeOp received ZK_EXITMARKER %lld; expected %lld\n"_Stz,
      m_iExitMarkers, m_iSends);
  
  if (m_iExitMarkers == m_iSends) {
    //m_iExitMarkers = 0;
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* pulsed done ?                                                              */
/*----------------------------------------------------------------------------*/
bool Sends::bPulsedDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);
  
  if (m_iPulsedMarkers > m_iSends)
    vTrace("Warning: ConsumeOp ZK_PULSEDMARKER received %lld; expected %lld\n"_Stz,
           m_iPulsedMarkers, m_iSends);
  
  if (m_iPulsedMarkers == m_iSends) {
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* start done ?                                                               */
/*----------------------------------------------------------------------------*/
bool Sends::bStartDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);
  
  if (m_iStartMarkers > m_iSends)
    vTrace("Warning: ConsumeOp ZK_STARTMARKER received %lld; expected %lld\n"_Stz,
           m_iStartMarkers, m_iSends);
  
  if (m_iStartMarkers == 0) {
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* streamDef done ?                                                           */
/*----------------------------------------------------------------------------*/
bool Sends::bStreamDefDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);
  
  if (m_iStreamDefs > m_iSends)
    vTrace("Warning: ConsumeOp ZK_STREAMDEF received %lld; expected %lld\n"_Stz,
           m_iStreamDefs, m_iSends);
  
  if (m_iStreamDefs == 0) {
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* inc cancel                                                                 */
/*----------------------------------------------------------------------------*/
void Sends::vIncCancel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);
  
  m_iCancelMarkers++;
}

/*----------------------------------------------------------------------------*/
/* inc cleanup                                                                */
/*----------------------------------------------------------------------------*/
void Sends::vIncCleanup(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);

  m_iCleanupMarkers++;
}

/*----------------------------------------------------------------------------*/
/* inc data pulse                                                             */
/*----------------------------------------------------------------------------*/
void Sends::vIncDataPulse(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);
  
  m_iDataPulses++;
}

/*----------------------------------------------------------------------------*/
/* inc destroy                                                                 */
/*----------------------------------------------------------------------------*/
void Sends::vIncDestroy(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);
  
  m_iDestroyMarkers++;
}

/*----------------------------------------------------------------------------*/
/* inc eos                                                                    */
/*----------------------------------------------------------------------------*/
void Sends::vIncEos(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);
  
  m_iEosMarkers++;
}

/*----------------------------------------------------------------------------*/
/* inc error                                                                  */
/*----------------------------------------------------------------------------*/
void Sends::vIncError(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);
  
  m_iEosMarkers++;
}

/*----------------------------------------------------------------------------*/
/* inc exit                                                                   */
/*----------------------------------------------------------------------------*/
void Sends::vIncExit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);
  
  m_iExitMarkers++;
}

/*----------------------------------------------------------------------------*/
/* inc pulsed                                                                 */
/*----------------------------------------------------------------------------*/
void Sends::vIncPulsed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);
  
  m_iPulsedMarkers++;
}

/*----------------------------------------------------------------------------*/
/* inc start                                                                  */
/*----------------------------------------------------------------------------*/
void Sends::vIncStart(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);
  
  m_iStartMarkers++;
}

/*----------------------------------------------------------------------------*/
/* inc streamDef                                                              */
/*----------------------------------------------------------------------------*/
void Sends::vIncStreamDef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);
  
  m_iStreamDefs++;
}

/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
__aFooterFunction(Sends);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
