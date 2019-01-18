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
/* monitor portrait                                                           */
/*----------------------------------------------------------------------------*/
void LockTracker::vMonitorPortrait(s48K& sPortrait)
{
  s48K s;
  i8 nSoFar = 0;
  
  for (i8 i_Lock = 0; i_Lock < LK_ENUMS; i_Lock++) {

    if (*m_ziReadCollisions[i_Lock]) {
      if (nSoFar)
        s.vAppend(' ');
      s.vCatSprint("%s rc%lld"_Stz,
                   aLK[i_Lock],
                   *m_ziReadCollisions[i_Lock]);
      if (*m_ziWriteCollisions[i_Lock])
        s.vCatSprint(" wc%lld"_Stz,
                     *m_ziReadCollisions[i_Lock]);
      nSoFar++;
    }
    else
    if (*m_ziWriteCollisions[i_Lock])  {
      if (nSoFar)
        s.vAppend(' ');
      s.vCatSprint("%s wc%lld"_Stz,
                   aLK[i_Lock],
                   *m_ziWriteCollisions[i_Lock]);
      nSoFar++;
    }
  }
  
  if (nSoFar) {
    sPortrait.vConcat("Locks: "_Stz);
    sPortrait.vConcat(s);
    sPortrait.vAppend('\n');
  }
  
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
