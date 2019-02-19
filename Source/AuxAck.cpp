/*============================================================================*/
/* AuxAck.cpp -- Auxilary Ack                                                 */
/*============================================================================*/

#include "Tasks.h"

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* AuxAck::operator new(size_t, AuxAck* pAuxAck)
{
  return pAuxAck;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void AuxAck::operator delete(void* )
{
  // do nothing
  // must be explicitly free'd by caller
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
AuxAck::AuxAck(void)
: Object(CN_AUXACK)
{
  m_nAcksOriginally = 0;
  m_nAcksRemaining  = 0;
  m_bTwiddling      = false;
  
  _checkInitialised_(CN_AUXACK);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_AUXACK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
AuxAck::~AuxAck(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_AUXACK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* check initialization                                                       */
/*----------------------------------------------------------------------------*/
void AuxAck::vCheckInit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_AUXACK,_FOOTER_);
  
  ASSERT(0 == m_nAcksRemaining); // logic check
}

/*----------------------------------------------------------------------------*/
/* atomically dec acks remaining and test if caller is decider                */
/*----------------------------------------------------------------------------*/
bool AuxAck::bDecAndDecide(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_AUXACK,_FOOTER_);

  if (m_nAcksRemaining < 1)
    vTrace("m_nAcksRemaining is %lld\n"_Stz, m_nAcksRemaining);
  
  RASSERT(m_nAcksRemaining > 0);
  i8 iNewValueWas = Atomic::uAtomicDec(&m_nAcksRemaining);
  RASSERT(iNewValueWas >= 0);
  
  bool bIsTapper = iNewValueWas ? false : true;
  RASSERT( (! bIsTapper) || (bIsTapper && 0 == m_nAcksRemaining) );
  
  return bIsTapper;
}

/*----------------------------------------------------------------------------*/
/* has decider been set?                                                      */
/*----------------------------------------------------------------------------*/
bool AuxAck::bIsSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_AUXACK,_FOOTER_);
  
  return m_nAcksRemaining ? true : false;
}

/*----------------------------------------------------------------------------*/
/* get original number of taps **FOR PRINING ONLY**                           */
/*----------------------------------------------------------------------------*/
i8 AuxAck::nOriginalAcks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_AUXACK,_FOOTER_);
  
  return m_nAcksRemaining;
}

/*----------------------------------------------------------------------------*/
/* get current number of taps remaining **FOR PRINING ONLY**                  */
/*----------------------------------------------------------------------------*/
i8 AuxAck::iPrintOnlyValue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_AUXACK,_FOOTER_);
  
  return m_nAcksRemaining;
}

/*----------------------------------------------------------------------------*/
/* set the number of acks remaining                                           */
/*----------------------------------------------------------------------------*/
void AuxAck::vSetRemaining(i8 nAcksRemaining)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_AUXACK,_FOOTER_);
  
  RASSERT(0 == m_nAcksRemaining);
  RASSERT(nAcksRemaining > 0);
  
  //vTrace("Setting m_nAcksRemaining to %lld\n"_Stz, nAcksRemaining);
  m_nAcksOriginally = nAcksRemaining;
  m_nAcksRemaining  = nAcksRemaining;
}

/*----------------------------------------------------------------------------*/
/* get twiddling                                                              */
/*----------------------------------------------------------------------------*/
bool AuxAck::bTwiddling(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_AUXACK,_FOOTER_);
  
  return m_bTwiddling;
}

/*----------------------------------------------------------------------------*/
/* set twiddling                                                              */
/*----------------------------------------------------------------------------*/
void AuxAck::vTwiddling(bool bTwiddling)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_AUXACK,_FOOTER_);
  
  m_bTwiddling = bTwiddling;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* AuxAck::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_AUXACK);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
