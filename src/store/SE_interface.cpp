/*============================================================================*/
/* SE_interface.cpp -- SE Interface                                           */
/*============================================================================*/

#include "BASE.h"

#include "CrwBuffer.hpp"

void vSE_CrwBuffer(void* aCrwBuffer, char** ppFirstRow, i8* pnRowSize, i8* pnRows)
{
  _static_

  CrwBuffer* pCrwBuffer = _pCrwBuffer(aCrwBuffer);

  *ppFirstRow = (char *)pCrwBuffer->aCrw(0);
  *pnRowSize = pCrwBuffer->nRowSize();
  *pnRows    = pCrwBuffer->nRows();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
