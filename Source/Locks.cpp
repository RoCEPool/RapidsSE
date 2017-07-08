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
  xFile.vPrint("Lock, LockNm, ReadGrab, ReadCollision, WriteGrab, WriteCollision\n"_Str);
  for (i8 i_Lock = 0; i_Lock < LK_ENUMS; i_Lock++)
    xFile.vPrint("%lld, %s, %lld, %lld, %lld, %lld\n"_Str,
                 i_Lock,
                 aLK[i_Lock],
                 *m_zReadGrabs[i_Lock],
                 *m_zReadCollisions[i_Lock],
                 *m_zWriteGrabs[i_Lock],
                 *m_zWriteCollisions[i_Lock]
                 );
                 
}

/*----------------------------------------------------------------------------*/
/* reset steps                                                                */
/*----------------------------------------------------------------------------*/
void Locks::vReset(void)
{
  for (i8 i_Lock = 0; i_Lock < LK_ENUMS; i_Lock++) {
    m_zReadGrabs[i_Lock] = 0;
    m_zReadCollisions[i_Lock] = 0;
    m_zWriteGrabs[i_Lock] = 0;
    m_zWriteCollisions[i_Lock] = 0;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
