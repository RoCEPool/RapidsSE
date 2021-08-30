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
  
  m_iCancellingMarkers = 0;
  m_iCleaningMarkers   = 0;
  m_iDataPulses        = 0;
  m_iDestroyMarkers    = 0;
  m_iEndingMarkers     = 0;
  m_iErroringMarkers   = 0;
  m_iExitingMarkers    = 0;
  m_iEosMarkers        = 0;
  m_iEndMarkers        = 0;
  m_iLeavingMarkers    = 0;
  m_iStartMarkers      = 0;
  m_iStreamDefRhts     = 0;
  m_iStreamDefs        = 0;
  
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
/* cancelling done ?                                                          */
/*----------------------------------------------------------------------------*/
bool Sends::bCancellingDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);

  if (m_iCancellingMarkers > m_iSends)
    vTrace("Warning: ConsumeOp received ZK_M_CANCELLING %lld; expected %lld\n"_Stz,
         m_iCancellingMarkers, m_iSends);
  
  if (m_iCancellingMarkers == m_iSends) {
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* cleaning done ?                                                            */
/*----------------------------------------------------------------------------*/
bool Sends::bCleaningDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);

  if (m_iCleaningMarkers > m_iSends)
    vTrace("Warning: ConsumeOp received ZK_M_CLEANING %lld; expected %lld\n"_Stz,
           m_iCleaningMarkers, m_iSends);

  if (m_iCleaningMarkers == m_iSends) {
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

  if (m_iDataPulses == 0) {
    return true;
  }
  else {
    //if (m_iDataPulses == m_iSends)
    //  m_iDataPulses = 0;
    return false;
  }
}

/*----------------------------------------------------------------------------*/
/* destroy done ?                                                             */
/*----------------------------------------------------------------------------*/
bool Sends::bDestroyDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);
  
  if (m_iDestroyMarkers > m_iSends)
  vTrace("Warning: ConsumeOp received ZK_M_DESTROY %lld; expected %lld\n"_Stz,
         m_iDestroyMarkers, m_iSends);
  
  if (m_iDestroyMarkers == m_iSends) {
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* end done ?                                                                 */
/*----------------------------------------------------------------------------*/
bool Sends::bEndDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);

  if (m_iEndMarkers > m_iSends)
    vTrace("Warning: ConsumeOp ZK_M_END received %lld; expected %lld\n"_Stz,
           m_iEndMarkers, m_iSends);

  if (m_iEndMarkers == m_iSends) {
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* ending done ?                                                              */
/*----------------------------------------------------------------------------*/
bool Sends::bEndingDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);

  if (m_iEndingMarkers > m_iSends)
    vTrace("Warning: ConsumeOp received ZK_M_ENDING %lld; expected %lld\n"_Stz,
           m_iEndingMarkers, m_iSends);

  if (m_iEndingMarkers == m_iSends) {
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
    vTrace("Warning: ConsumeOp received ZK_M_EOS %lld; expected %lld\n"_Stz,
           m_iEosMarkers, m_iSends);
  
  if (m_iEosMarkers == m_iSends) {
    //m_iEosMarkers = 0;
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* ending done ?                                                              */
/*----------------------------------------------------------------------------*/
bool Sends::bErroringDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);

  if (m_iErroringMarkers > m_iSends)
    vTrace("Warning: ConsumeOp received ZK_M_ERRORING %lld; expected %lld\n"_Stz,
           m_iErroringMarkers, m_iSends);

  if (m_iErroringMarkers == m_iSends) {
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* exiting done ?                                                             */
/*----------------------------------------------------------------------------*/
bool Sends::bExitingDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);
  
  if (m_iExitingMarkers > m_iSends)
    vTrace("Warning: ConsumeOp ZK_M_EXITING ZK_M_EXIT %lld; expected %lld\n"_Stz,
           m_iExitingMarkers, m_iSends);
  
  if (m_iExitingMarkers == m_iSends) {
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* leaving done ?                                                             */
/*----------------------------------------------------------------------------*/
bool Sends::bLeavingDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);

  if (m_iLeavingMarkers > m_iSends)
    vTrace("Warning: ConsumeOp received ZK_M_LEAVING %lld; expected %lld\n"_Stz,
           m_iLeavingMarkers, m_iSends);

  if (m_iLeavingMarkers == m_iSends) {
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
    vTrace("Warning: ConsumeOp ZK_M_START received %lld; expected %lld\n"_Stz,
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
    vTrace("Warning: ConsumeOp ZK_D_STREAMDEF received %lld; expected %lld\n"_Stz,
           m_iStreamDefs, m_iSends);
  
  if (m_iStreamDefs == 0) {
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* streamDefRht done ?                                                        */
/*----------------------------------------------------------------------------*/
bool Sends::bStreamDefRhtDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);

  if (m_iStreamDefRhts > m_iSends)
    vTrace("Warning: ConsumeOp ZK_D_STREAMDEFRHT received %lld; expected %lld\n"_Stz,
           m_iStreamDefRhts, m_iSends);

  if (m_iStreamDefRhts == 0) {
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* inc cancelling                                                             */
/*----------------------------------------------------------------------------*/
void Sends::vIncCancelling(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);
  
  m_iCancellingMarkers++;
}

/*----------------------------------------------------------------------------*/
/* inc cleaning                                                               */
/*----------------------------------------------------------------------------*/
void Sends::vIncCleaning(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);

  m_iCleaningMarkers++;
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
/* inc ending                                                                 */
/*----------------------------------------------------------------------------*/
void Sends::vIncEnding(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);

  m_iEndingMarkers++;
}

/*----------------------------------------------------------------------------*/
/* inc end                                                                    */
/*----------------------------------------------------------------------------*/
void Sends::vIncEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);

  m_iEndMarkers++;
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
/* inc erroring                                                               */
/*----------------------------------------------------------------------------*/
void Sends::vIncErroring(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);

  m_iErroringMarkers++;
}

/*----------------------------------------------------------------------------*/
/* inc exiting                                                                */
/*----------------------------------------------------------------------------*/
void Sends::vIncExiting(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);

  m_iExitingMarkers++;
}

/*----------------------------------------------------------------------------*/
/* inc leaving                                                                */
/*----------------------------------------------------------------------------*/
void Sends::vIncLeaving(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);

  m_iLeavingMarkers++;
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
/* inc streamDefRht                                                           */
/*----------------------------------------------------------------------------*/
void Sends::vIncStreamDefRht(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SENDS,_FOOTER_);

  m_iStreamDefRhts++;
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
