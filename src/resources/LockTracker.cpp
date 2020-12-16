/*============================================================================*/
/* LockTracker.cpp -- lock statistics tracker                                 */
/*============================================================================*/

#include "Resources.h"


/*----------------------------------------------------------------------------*/
/* monitor portrait                                                           */
/*----------------------------------------------------------------------------*/
void LockTracker::vMonitorPortrait(s48K& sPortrait)
{
  s48K s;
  i8 nSoFar = 0;
  
  for (i8 i_Lock = 0; i_Lock < LK_ENUMS; i_Lock++) {

    if (*m_ixReadCollisions[i_Lock]) {
      if (nSoFar)
        s.vAppend(' ');
      s.vCatSprint("%s rc%lld"_Stz,
                   aLK[i_Lock],
                   *m_ixReadCollisions[i_Lock]);
      if (*m_ixWriteCollisions[i_Lock])
        s.vCatSprint(" wc%lld"_Stz,
                     *m_ixReadCollisions[i_Lock]);
      nSoFar++;
    }
    else
    if (*m_ixWriteCollisions[i_Lock])  {
      if (nSoFar)
        s.vAppend(' ');
      s.vCatSprint("%s wc%lld"_Stz,
                   aLK[i_Lock],
                   *m_ixWriteCollisions[i_Lock]);
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
  vTrace("  Lock        Lock Name"
         "   ReadGrab  Collision   %%"
         "  WriteGrab  Collision   %%\n"_Stz);
  
  for (i8 i_Lock = 1; i_Lock < LK_ENUMS; i_Lock++) {
    
    vTrace("  %4lld %16s "
           "%10lld %10lld %3lld "
           "%10lld %10lld %3lld\n"_Stz,
           
           i_Lock,
           aLK[i_Lock],
           
           *m_ixReadGrabs[i_Lock],
           *m_ixReadCollisions[i_Lock],
           IntLib::iPercent(*m_ixReadCollisions[i_Lock],*m_ixReadGrabs[i_Lock]),
           
           *m_ixWriteGrabs[i_Lock],
           *m_ixWriteCollisions[i_Lock],
           IntLib::iPercent(*m_ixWriteCollisions[i_Lock],*m_ixWriteGrabs[i_Lock])
           );

  }
}

/*----------------------------------------------------------------------------*/
/* reset steps                                                                */
/*----------------------------------------------------------------------------*/
void LockTracker::vReset(void)
{
  for (i8 i_Lock = 0; i_Lock < LK_ENUMS; i_Lock++) {
    m_ixWorkUnits[i_Lock] = 0;
    m_ixReadGrabs[i_Lock] = 0;
    m_ixReadCollisions[i_Lock] = 0;
    m_ixWriteGrabs[i_Lock] = 0;
    m_ixWriteCollisions[i_Lock] = 0;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
