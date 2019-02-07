/*============================================================================*/
/* Decider.cpp -- ack decider                                                 */
/*============================================================================*/

#include "Tasks.h"

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Decider::operator new(size_t, Decider* pDecider)
{
  return pDecider;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Decider::operator delete(void* )
{
  // do nothing
  // must be explicitly free'd by caller
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Decider::Decider(void)
: Object(CN_DECIDER)
{
  m_nAcksOriginally = 0;
  m_nAcksRemaining  = 0;
  
  _checkInitialised_(CN_DECIDER);
  
  _postcondition_(_MAGIC_,CN_DECIDER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Decider::~Decider(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_DECIDER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* check initialization                                                       */
/*----------------------------------------------------------------------------*/
void Decider::vCheckInit(void)
{
  _precondition_(_MAGIC_,CN_DECIDER,_FOOTER_);
  
  ASSERT(0 == m_nAcksRemaining); // logic check
}

/*----------------------------------------------------------------------------*/
/* atomically dec acks remaining and test if caller is decider                */
/*----------------------------------------------------------------------------*/
bool Decider::bDecAndDecide(void)
{
  _precondition_(_MAGIC_,CN_DECIDER,_FOOTER_);

  if (m_nAcksRemaining < 1)
    vTrace("m_nAcksRemaining is %lld\n"_Stz, m_nAcksRemaining);
  
  RASSERT(m_nAcksRemaining > 0);
  i8 iNewValueWas = Atomic::uAtomicDec(&m_nAcksRemaining);
  RASSERT(iNewValueWas >= 0);
  
  bool bIsDecider = iNewValueWas ? false : true;
  RASSERT( (! bIsDecider) || (bIsDecider && 0 == m_nAcksRemaining) );
  
  return bIsDecider;
}

/*----------------------------------------------------------------------------*/
/* has decider been set?                                                      */
/*----------------------------------------------------------------------------*/
bool Decider::bIsSet(void)
{
  _precondition_(_MAGIC_,CN_DECIDER,_FOOTER_);
  
  return m_nAcksRemaining ? true : false;
}

/*----------------------------------------------------------------------------*/
/* get original number of acks **FOR PRINING ONLY**                           */
/*----------------------------------------------------------------------------*/
i8 Decider::nOriginalAcks(void)
{
  _precondition_(_MAGIC_,CN_DECIDER,_FOOTER_);
  
  return m_nAcksRemaining;
}

/*----------------------------------------------------------------------------*/
/* get current number of acks remaining **FOR PRINING ONLY**                  */
/*----------------------------------------------------------------------------*/
i8 Decider::iPrintOnlyValue(void)
{
  _precondition_(_MAGIC_,CN_DECIDER,_FOOTER_);
  
  return m_nAcksRemaining;
}

/*----------------------------------------------------------------------------*/
/* set the number of acks remaining                                           */
/*----------------------------------------------------------------------------*/
void Decider::vSetRemaining(i8 nAcksRemaining)
{
  _precondition_(_MAGIC_,CN_DECIDER,_FOOTER_);
  
  RASSERT(0 == m_nAcksRemaining);
  RASSERT(nAcksRemaining > 0);
  
  //vTrace("Setting m_nAcksRemaining to %lld\n"_Stz, nAcksRemaining);
  m_nAcksOriginally = nAcksRemaining;
  m_nAcksRemaining  = nAcksRemaining;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Decider::aFooter(void)
{
  _precondition_(_MAGIC_,CN_DECIDER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
