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
/* dump the rlu Dstk                                                          */
/*----------------------------------------------------------------------------*/
void vRlu_DUMP(void* pLU, i8* aiDstk)
{
  LogicUnit* pLogicUnit = _pLogicUnit(pLU);
  
  pLogicUnit->vRlu_DUMP(pLU,aiDstk);
}

/*----------------------------------------------------------------------------*/
/* begin EO timer                                                             */
/*----------------------------------------------------------------------------*/
void vRlu_BEGINTMR(void* aLU)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  pLogicUnit->m_pLogicTimer->vBeginTmr();
}

/*----------------------------------------------------------------------------*/
/* begin EO timer                                                             */
/*----------------------------------------------------------------------------*/
void vRlu_ENDTMR(void* aLU, i8 iCode)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  pLogicUnit->m_pLogicTimer->vEndTmr((eEO)iCode);
}

/*----------------------------------------------------------------------------*/
/* diarise enter                                                              */
/*----------------------------------------------------------------------------*/
void vRlu_ENTER(void* aLU)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  
  pLogicUnit->vEnter();
}

/*----------------------------------------------------------------------------*/
/* diarise exit                                                               */
/*----------------------------------------------------------------------------*/
void vRlu_EXIT(void* aLU)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  pLogicUnit->vExit();
}

/*----------------------------------------------------------------------------*/
/* set logicUnit handler                                                      */
/*----------------------------------------------------------------------------*/
void vRlu_HANDLER(void* aLU, Stz* aHandler)
{
  _static_

  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  DicStack* pDicStack = AM::pTaskDicStack();
  Word* pWordHandler = pDicStack->pFindWord(aHandler);

  pLogicUnit->m_xpHandler = pWordHandler;
}

/*----------------------------------------------------------------------------*/
/* add query fragment EO timer                                                */
/*----------------------------------------------------------------------------*/
void vRlu_QUERYFRAG(void* aLU, i8 iQueryFrag)
{
  _static_
  
  if (AM::bRLUTimer()) {
    LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  
    pLogicUnit->m_pLogicTimer->vQueryFrag(iQueryFrag);
  }
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
/* get nsecs since                                                            */
/*----------------------------------------------------------------------------*/
i8 iRlu_TMR_SINCE(void* pLU)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(pLU);

  pLogicUnit->m_oSince.vUpdate();
  
  i8 iRes = pLogicUnit->m_oSince.iNanoseconds();

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
