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
  
  m_oDdlMasterLock.vResetStats();
  m_oDicMasterLock.vResetStats();
  m_oTccMasterLock.vResetStats();

  for (i8 i_Lock = 0; i_Lock < CN_ENUMS; i_Lock++) {

    m_ixReadGrabs[i_Lock].vResetAtomic();
    m_ixReadCollisions[i_Lock].vResetAtomic();
    
    m_ixWriteGrabs[i_Lock].vResetAtomic();
    m_ixWriteCollisions[i_Lock].vResetAtomic();
    
  }
}

/*----------------------------------------------------------------------------*/
/* show locks                                                                 */
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
  
  vTrace("Lock        Lock Name "
         "|  ReadGrab   Collide   %% "
         "| WriteGrab   Collide   %%\n"_Stz);
  /*
  vTrace("%4lld %16s "
         "|%10lld %9lld %3lld "
         "|%10lld %9lld %3lld\n"_Stz
         
         ,(i8)-1
         ,aCN[CN_DDLLOCK]
         
         ,m_oDdlMasterLock.iReadGrabs()
         ,m_oDdlMasterLock.iReadCollisions()
         ,IntLib::iPercent(m_oDdlMasterLock.iReadCollisions(),
                           m_oDdlMasterLock.iReadGrabs())
         
         ,m_oDdlMasterLock.iWriteGrabs()
         ,m_oDdlMasterLock.iWriteCollisions()
         ,IntLib::iPercent(m_oDdlMasterLock.iWriteCollisions(),
                           m_oDdlMasterLock.iWriteGrabs())
         );

  vTrace("%4lld %16s "
         "|%10lld %9lld %3lld "
         "|%10lld %9lld %3lld\n"_Stz
         
         ,(i8)-1
         ,aCN[CN_DICLOCK]
         
         ,m_oDicMasterLock.iReadGrabs()
         ,m_oDicMasterLock.iReadCollisions()
         ,IntLib::iPercent(m_oDicMasterLock.iReadCollisions(),
                           m_oDicMasterLock.iReadGrabs())
         
         ,m_oDicMasterLock.iWriteGrabs()
         ,m_oDicMasterLock.iWriteCollisions()
         ,IntLib::iPercent(m_oDicMasterLock.iWriteCollisions(),
                           m_oDicMasterLock.iWriteGrabs())
         );

  vTrace("%4lld %16s "
         "|%10lld %9lld %3lld "
         "|%10lld %9lld %3lld\n"_Stz
         
         ,(i8)-1
         ,aCN[CN_TCCLOCK]
         
         ,m_oTccMasterLock.iReadGrabs()
         ,m_oTccMasterLock.iReadCollisions()
         ,IntLib::iPercent(m_oTccMasterLock.iReadCollisions(),
                           m_oTccMasterLock.iReadGrabs())
         
         ,m_oTccMasterLock.iWriteGrabs()
         ,m_oTccMasterLock.iWriteCollisions()
         ,IntLib::iPercent(m_oTccMasterLock.iWriteCollisions(),
                           m_oTccMasterLock.iWriteGrabs())
         );

  vTrace("----\n"_Stz);
  */
  
  for (i8 i_Lock = 1; i_Lock < CN_ENUMS; i_Lock++) {
    if (! *m_ixReadGrabs[i_Lock] && ! *m_ixWriteGrabs[i_Lock])
      continue;
    
    vTrace("%4lld %16s "
           "|%10lld %9lld %3lld "
           "|%10lld %9lld %3lld\n"_Stz
           
           ,i_Lock
           ,aCN[i_Lock]
           
           ,*m_ixReadGrabs[i_Lock]
           ,*m_ixReadCollisions[i_Lock]
           ,IntLib::iPercent(*m_ixReadCollisions[i_Lock],
                             *m_ixReadGrabs[i_Lock])
           
           ,*m_ixWriteGrabs[i_Lock]
           ,*m_ixWriteCollisions[i_Lock]
           ,IntLib::iPercent(*m_ixWriteCollisions[i_Lock],
                             *m_ixWriteGrabs[i_Lock])
           );
  }
  
#endif
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
