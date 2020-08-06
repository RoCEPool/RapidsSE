/*============================================================================*/
/* RLU_interface.cpp -- RLU Rlu Interface                                     */
/*============================================================================*/

#include "Expressions.h"
#include "RLU_interface.h"

/*----------------------------------------------------------------------------*/
/* compare m_pEnt.key to m_oEntWrk.key                                        */
/*----------------------------------------------------------------------------*/
struct RluCommon* aRlu_INIT(void* pLU)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(pLU);
  
  struct RluCommon* aRluCommon = pLogicUnit->aRlu_INIT(pLU);
  
  return aRluCommon;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
