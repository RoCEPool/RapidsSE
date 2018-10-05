/*============================================================================*/
/* LockTrack.cpp -- lock statistics tracker                                   */
/*============================================================================*/

#include "System.h"
#include "Pool.h"

/*----------------------------------------------------------------------------*/
/* list steps                                                                 */
/*----------------------------------------------------------------------------*/
void Locks::vList(File& xFile)
{
  xFile.vPrint("Lock, LockNm, WorkUnits, ReadGrab, ReadCollision, WriteGrab, WriteCollision\n"_Stz);
  for (i8 i_Lock = 0; i_Lock < LK_ENUMS; i_Lock++)
    xFile.vPrint("%lld, %s, %lld, %lld, %lld, %lld, %lld\n"_Stz,
                 i_Lock,
                 aLK[i_Lock],
                 *m_zWorkUnits[i_Lock],
                 *m_zReadGrabs[i_Lock],
                 *m_zReadCollisions[i_Lock],
                 *m_zWriteGrabs[i_Lock],
                 *m_zWriteCollisions[i_Lock]
                 );
                 
}

/*----------------------------------------------------------------------------*/
/* locks report                                                               */
/*----------------------------------------------------------------------------*/
void Locks::vReport(void)
{
  vTrace("  Lock       Lock Name   ReadGrab  Collision  WriteGrab  Collision\n"_Stz);
  
  for (i8 i_Lock = 0; i_Lock < LK_ENUMS; i_Lock++) {
    
    vTrace("  %4lld %15s %10lld %10lld %10lld %10lld\n"_Stz,
           
           i_Lock,
           aLK[i_Lock],
           
           *m_zReadGrabs[i_Lock],
           *m_zReadCollisions[i_Lock],
           
           *m_zWriteGrabs[i_Lock],
           *m_zWriteCollisions[i_Lock]
           );

  }
}

/*----------------------------------------------------------------------------*/
/* reset steps                                                                */
/*----------------------------------------------------------------------------*/
void Locks::vReset(void)
{
  for (i8 i_Lock = 0; i_Lock < LK_ENUMS; i_Lock++) {
    m_zWorkUnits[i_Lock] = 0;
    m_zReadGrabs[i_Lock] = 0;
    m_zReadCollisions[i_Lock] = 0;
    m_zWriteGrabs[i_Lock] = 0;
    m_zWriteCollisions[i_Lock] = 0;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
