/*============================================================================*/
/* LockTracker.cpp -- lock statistics tracker                                 */
/*============================================================================*/

#include "Measurement.h"
#include "Pool.h"

/*----------------------------------------------------------------------------*/
/* list steps                                                                 */
/*----------------------------------------------------------------------------*/
void LockTracker::vList(File& oFile)
{
  oFile.vPrint("Lock, LockNm, WorkUnits, ReadGrab, ReadCollision, WriteGrab, WriteCollision\n"_Stz);
  for (i8 i_Lock = 0; i_Lock < LK_ENUMS; i_Lock++)
    oFile.vPrint("%lld, %s, %lld, %lld, %lld, %lld, %lld\n"_Stz,
                 i_Lock,
                 aLK[i_Lock],
                 *m_ziWorkUnits[i_Lock],
                 *m_ziReadGrabs[i_Lock],
                 *m_ziReadCollisions[i_Lock],
                 *m_ziWriteGrabs[i_Lock],
                 *m_ziWriteCollisions[i_Lock]
                 );
                 
}

/*----------------------------------------------------------------------------*/
/* locks report                                                               */
/*----------------------------------------------------------------------------*/
void LockTracker::vReport(void)
{
  vTrace("  Lock       Lock Name   ReadGrab  Collision  WriteGrab  Collision\n"_Stz);
  
  for (i8 i_Lock = 0; i_Lock < LK_ENUMS; i_Lock++) {
    
    vTrace("  %4lld %15s %10lld %10lld %10lld %10lld\n"_Stz,
           
           i_Lock,
           aLK[i_Lock],
           
           *m_ziReadGrabs[i_Lock],
           *m_ziReadCollisions[i_Lock],
           
           *m_ziWriteGrabs[i_Lock],
           *m_ziWriteCollisions[i_Lock]
           );

  }
}

/*----------------------------------------------------------------------------*/
/* reset steps                                                                */
/*----------------------------------------------------------------------------*/
void LockTracker::vReset(void)
{
  for (i8 i_Lock = 0; i_Lock < LK_ENUMS; i_Lock++) {
    m_ziWorkUnits[i_Lock] = 0;
    m_ziReadGrabs[i_Lock] = 0;
    m_ziReadCollisions[i_Lock] = 0;
    m_ziWriteGrabs[i_Lock] = 0;
    m_ziWriteCollisions[i_Lock] = 0;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
