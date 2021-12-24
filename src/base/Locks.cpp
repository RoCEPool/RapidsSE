/*============================================================================*/
/* Locks.cpp -- lock manager                                                  */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Locks::Locks(void)
: Object(CN_LOCKS)
{
}

/*----------------------------------------------------------------------------*/
/* reset lock stats                                                           */
/*----------------------------------------------------------------------------*/
void Locks::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCKS,_FOOTER_);
  
  for (i8 i_Lock = 0; i_Lock < CN_ENUMS; i_Lock++) {
        
    m_ixReadGrabs[i_Lock]       = 0;
    m_ixReadCollisions[i_Lock]  = 0;
    
    m_ixWriteGrabs[i_Lock]      = 0;
    m_ixWriteCollisions[i_Lock] = 0;
  }
}

/*----------------------------------------------------------------------------*/
/* locks report                                                               */
/*----------------------------------------------------------------------------*/
void Locks::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCKS,_FOOTER_);
  
#ifndef WITH_LOCK_TRACKING
  
  vTrace("WITH_LOCK_TRACKING not defined\n"_Stz);
  return;
  
#else
  
  if (AM::SYSARGS::bLockTracking())
    vTrace("Lock Tracking ENABLED\n"_Stz);
  else
    vTrace("Lock Tracking DISABLED\n"_Stz);
  
  vTrace("  Lock        Lock Name"
         "   ReadGrab  Collision   %%"
         "  WriteGrab  Collision   %%\n"_Stz);
  
  for (i8 i_Lock = 1; i_Lock < CN_ENUMS; i_Lock++) {
    
    vTrace("  %4lld %16s "
           "%10lld %10lld %3lld "
           "%10lld %10lld %3lld\n"_Stz,
           
           i_Lock,
           aCN[i_Lock],
           
           *m_ixReadGrabs[i_Lock],
           *m_ixReadCollisions[i_Lock],
           IntLib::iPercent(*m_ixReadCollisions[i_Lock],*m_ixReadGrabs[i_Lock]),
           
           *m_ixWriteGrabs[i_Lock],
           *m_ixWriteCollisions[i_Lock],
           IntLib::iPercent(*m_ixWriteCollisions[i_Lock],*m_ixWriteGrabs[i_Lock])
           );

  }
  
#endif
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
