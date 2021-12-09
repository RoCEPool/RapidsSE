/*============================================================================*/
/* Starter.cpp -- Starter helper                                              */
/*============================================================================*/

#include "BASE.h"

#include "Starter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,Starter,CN_STARTER)
__aNewFunction(Starter,CN_STARTER)
__vDeleteFunction(Starter,CN_STARTER)
__aFooterFunction(Starter)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Starter::Starter(void)
: Object(CN_STARTER)
, m_okLock(this)
{
  _precondition_(_MAGIC_,CN_STARTER,_FOOTER_);
  
  // m_okLock
  m_nStarted = 0;
  // m_nxReady
  
  _checkInitialised_(CN_STARTER);
  
  _postcondition_(_MAGIC_,CN_STARTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Starter::~Starter(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_STARTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dec ready when completed                                                   */
/*----------------------------------------------------------------------------*/
void Starter::vDecReady(void)
{
  _precondition_(_MAGIC_,CN_STARTER,_FOOTER_);
  
  -- m_nxReady;
}

/*----------------------------------------------------------------------------*/
/* drop read lock                                                             */
/*----------------------------------------------------------------------------*/
void Starter::vDropReadLock(void)
{
  _precondition_(_MAGIC_,CN_STARTER,_FOOTER_);
  
  m_okLock.vDropRead();
}

/*----------------------------------------------------------------------------*/
/* drop write lock                                                            */
/*----------------------------------------------------------------------------*/
void Starter::vDropWriteLock(void)
{
  _precondition_(_MAGIC_,CN_STARTER,_FOOTER_);
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* get number ready to start                                                  */
/*----------------------------------------------------------------------------*/
i8 Starter::nGetReady(void)
{
  _precondition_(_MAGIC_,CN_STARTER,_FOOTER_);
  
  return m_nxReady.iVal();
}

/*----------------------------------------------------------------------------*/
/* get number of starters so far                                              */
/*----------------------------------------------------------------------------*/
i8 Starter::nGetStarted(void)
{
  _precondition_(_MAGIC_,CN_STARTER,_FOOTER_);
  
  return m_nStarted;
}

/*----------------------------------------------------------------------------*/
/* grab read lock                                                             */
/*----------------------------------------------------------------------------*/
void Starter::vGrabReadLock(void)
{
  _precondition_(_MAGIC_,CN_STARTER,_FOOTER_);
  
  while (! m_okLock.bGrabRead())
    AM::vNapMsec(10,CN_STARTER);
}

/*----------------------------------------------------------------------------*/
/* grab write lock                                                            */
/*----------------------------------------------------------------------------*/
void Starter::vGrabWriteLock(void)
{
  _precondition_(_MAGIC_,CN_STARTER,_FOOTER_);
  
  m_okLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* inc number that are ready to start                                         */
/*----------------------------------------------------------------------------*/
void Starter::vIncReady(void)
{
  _precondition_(_MAGIC_,CN_STARTER,_FOOTER_);
  
  ++ m_nxReady;
}

/*----------------------------------------------------------------------------*/
/* inc number started so far                                                  */
/*----------------------------------------------------------------------------*/
void Starter::vIncStarted(void)
{
  _precondition_(_MAGIC_,CN_STARTER,_FOOTER_);
  
  m_nStarted ++;
}

/*----------------------------------------------------------------------------*/
/* inc number started so far                                                  */
/*----------------------------------------------------------------------------*/
void Starter::vResetStarted(void)
{
  _precondition_(_MAGIC_,CN_STARTER,_FOOTER_);
  
  m_nStarted = 0;
}

/*----------------------------------------------------------------------------*/
/* status                                                                     */
/*----------------------------------------------------------------------------*/
void Starter::vStatus(void)
{
  _precondition_(_MAGIC_,CN_STARTER,_FOOTER_);

  vTrace("Starter: lock %llx started %lld ready %lld\n"_Stz
         ,m_okLock.uLockWord()
         ,m_nStarted
         ,m_nxReady.iVal()
         );
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
