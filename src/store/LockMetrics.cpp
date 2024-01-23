/*============================================================================*/
/* LockMetrics.cpp -- Lock Expressions                                            */
/*============================================================================*/

#include "BASE.h"

#include "Grid.hpp"
#include "LockMetrics.hpp"

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sample current lock Expressions                                            */
/*----------------------------------------------------------------------------*/
Row* LockMetrics::pSamplePds(Pds* pPdsRowBuffer)
{
  _static_

  Locks* pLckTrk = AM::pLocks();

  //pLckTrk->vReport();

  Grid* pGrid = AM::pGrid();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Row* pRow = nullptr;

  for (i8 i_Lock = 1; i_Lock < CN_ENUMS; i_Lock++) {

    pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,

                                 pGrid->iThisNodeId(),                    // node no
                                 0,                                       // take no

                                 i_Lock,                                  // lock no
                                 aCN[i_Lock],                             // lock name

                                 *pLckTrk->m_ixReadGrabs[i_Lock],         // read grab
                                 *pLckTrk->m_ixReadCollisions[i_Lock],    // read collision
                                 IntLib::iPercent(                        // read percent
                                     *pLckTrk->m_ixReadCollisions[i_Lock],
                                     *pLckTrk->m_ixReadGrabs[i_Lock]),

                                 *pLckTrk->m_ixWriteGrabs[i_Lock],        // write grab
                                 *pLckTrk->m_ixWriteCollisions[i_Lock],   // write collision
                                 IntLib::iPercent(                        // write percent
                                     *pLckTrk->m_ixWriteCollisions[i_Lock],
                                     *pLckTrk->m_ixWriteGrabs[i_Lock]),

                                 C_VARGS_END);
    ASSERT(pRow);
  }
  return pRow;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
