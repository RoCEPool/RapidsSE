/*============================================================================*/
/* RLU_interface.cpp -- RLU Rlu Interface                                     */
/*============================================================================*/

#include "Expressions.h"
#include "RLU_interface.h"
#include "LIB_interface.h"

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

/*----------------------------------------------------------------------------*/
/* add timer to agg timer                                                     */
/*----------------------------------------------------------------------------*/
void vRlu_TMR_AGG_ADD(void* pLU)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(pLU);

  pLogicUnit->m_oAggTimer.vAdd(&pLogicUnit->m_oTimer);

}

/*----------------------------------------------------------------------------*/
/* push agg nanoseconds                                                       */
/*----------------------------------------------------------------------------*/
i8 iRlu_TMR_AGG_NSECS(void* pLU)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(pLU);
  
  i8 iRes = pLogicUnit->m_oAggTimer.iNanoseconds();
  
  return iRes;
}

/*----------------------------------------------------------------------------*/
/*  reset agg timer                                                           */
/*----------------------------------------------------------------------------*/
void vRlu_TMR_AGG_RESET(void* pLU)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(pLU);

  pLogicUnit->m_oAggTimer.vReset();
}

/*----------------------------------------------------------------------------*/
/* push timer nanoseconds                                                     */
/*----------------------------------------------------------------------------*/
i8 iRlu_TMR_NSECS(void* pLU)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(pLU);

  i8 iRes = pLogicUnit->m_oTimer.iNanoseconds();
  
  return iRes;
}

/*----------------------------------------------------------------------------*/
/* timer start                                                                */
/*----------------------------------------------------------------------------*/
void vRlu_TMR_START(void* pLU)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(pLU);

  pLogicUnit->m_oTimer.vStart();
}

/*----------------------------------------------------------------------------*/
/* timer update                                                               */
/*----------------------------------------------------------------------------*/
void vRlu_TMR_UPDATE(void* pLU)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(pLU);

  pLogicUnit->m_oTimer.vUpdate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
