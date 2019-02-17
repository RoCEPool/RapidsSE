/*============================================================================*/
/* Tapper.cpp -- forward ack decider                                          */
/*============================================================================*/

#include "Tasks.h"

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Tapper::operator new(size_t, Tapper* pTapper)
{
  return pTapper;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Tapper::operator delete(void* )
{
  // do nothing
  // must be explicitly free'd by caller
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Tapper::Tapper(void)
: Object(CN_TAPPER)
{
  m_nTapsOriginally = 0;
  m_nTapsRemaining  = 0;
  m_bTwiddling      = false;
  
  _checkInitialised_(CN_TAPPER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TAPPER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Tapper::~Tapper(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TAPPER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* check initialization                                                       */
/*----------------------------------------------------------------------------*/
void Tapper::vCheckInit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TAPPER,_FOOTER_);
  
  ASSERT(0 == m_nTapsRemaining); // logic check
}

/*----------------------------------------------------------------------------*/
/* atomically dec acks remaining and test if caller is decider                */
/*----------------------------------------------------------------------------*/
bool Tapper::bDecAndDecide(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TAPPER,_FOOTER_);

  if (m_nTapsRemaining < 1)
    vTrace("m_nTapsRemaining is %lld\n"_Stz, m_nTapsRemaining);
  
  RASSERT(m_nTapsRemaining > 0);
  i8 iNewValueWas = Atomic::uAtomicDec(&m_nTapsRemaining);
  RASSERT(iNewValueWas >= 0);
  
  bool bIsTapper = iNewValueWas ? false : true;
  RASSERT( (! bIsTapper) || (bIsTapper && 0 == m_nTapsRemaining) );
  
  return bIsTapper;
}

/*----------------------------------------------------------------------------*/
/* has decider been set?                                                      */
/*----------------------------------------------------------------------------*/
bool Tapper::bIsSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TAPPER,_FOOTER_);
  
  return m_nTapsRemaining ? true : false;
}

/*----------------------------------------------------------------------------*/
/* get original number of taps **FOR PRINING ONLY**                           */
/*----------------------------------------------------------------------------*/
i8 Tapper::nOriginalTaps(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TAPPER,_FOOTER_);
  
  return m_nTapsRemaining;
}

/*----------------------------------------------------------------------------*/
/* get current number of taps remaining **FOR PRINING ONLY**                  */
/*----------------------------------------------------------------------------*/
i8 Tapper::iPrintOnlyValue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TAPPER,_FOOTER_);
  
  return m_nTapsRemaining;
}

/*----------------------------------------------------------------------------*/
/* set the number of acks remaining                                           */
/*----------------------------------------------------------------------------*/
void Tapper::vSetRemaining(i8 nTapsRemaining)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TAPPER,_FOOTER_);
  
  RASSERT(0 == m_nTapsRemaining);
  RASSERT(nTapsRemaining > 0);
  
  //vTrace("Setting m_nTapsRemaining to %lld\n"_Stz, nTapsRemaining);
  m_nTapsOriginally = nTapsRemaining;
  m_nTapsRemaining  = nTapsRemaining;
}

/*----------------------------------------------------------------------------*/
/* get twiddling                                                              */
/*----------------------------------------------------------------------------*/
bool Tapper::bTwiddling(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TAPPER,_FOOTER_);
  
  return m_bTwiddling;
}

/*----------------------------------------------------------------------------*/
/* set twiddling                                                              */
/*----------------------------------------------------------------------------*/
void Tapper::vTwiddling(bool bTwiddling)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TAPPER,_FOOTER_);
  
  m_bTwiddling = bTwiddling;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Tapper::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TAPPER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
