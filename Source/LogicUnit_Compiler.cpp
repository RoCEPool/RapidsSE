/*============================================================================*/
/* LogicUnit_Compiler.hpp -- Compiler iterface for Logic Unit                 */
/*============================================================================*/

#include "Expressions.h"

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void LogicUnit::vRLU_IXT_DEL_ENT(RLU* aRLU, void* aDev, void* aArg, i8 iArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void LogicUnit::vRLU_IXT_DUMP(RLU* aRLU, void* aDev, void* aArg, i8 iArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iRLU_IXT_ENTRIES(RLU* aRLU, void* aDev, void* aArg, i8 iArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  return 0;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void LogicUnit::vRLU_IXT_INS_ENT(RLU* aRLU, void* aDev, void* aArg, i8 iArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iRLU_IXT_TRV_BEGIN(RLU* aRLU, void* aDev, void* aArg, i8 iArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  return 0;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iRLU_IXT_TRV_NEXT(RLU* aRLU, void* aDev, void* aArg, i8 iArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  return 0;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

#include "RLU_itf.h"

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void vRLU_IXT_DEL_ENT(RLU* aRLU, void* aDev, void* aArg, i8 iArg)
{
  LogicUnit* pLogicUnit = _pLogicUnit(aRLU);

  pLogicUnit->vRLU_IXT_DEL_ENT(aRLU,aDev,aArg,iArg);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void vRLU_IXT_DUMP(RLU* aRLU, void* aDev, void* aArg, i8 iArg)
{
  LogicUnit* pLogicUnit = _pLogicUnit(aRLU);
  
  pLogicUnit->vRLU_IXT_DUMP(aRLU,aDev,aArg,iArg);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
i8 iRLU_IXT_ENTRIES(RLU* aRLU, void* aDev, void* aArg, i8 iArg)
{
  LogicUnit* pLogicUnit = _pLogicUnit(aRLU);
  
  i8 iRes = pLogicUnit->iRLU_IXT_ENTRIES(aRLU,aDev,aArg,iArg);

  return iRes;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void vRLU_IXT_INS_ENT(RLU* aRLU, void* aDev, void* aArg, i8 iArg)
{
  LogicUnit* pLogicUnit = _pLogicUnit(aRLU);

  pLogicUnit->vRLU_IXT_INS_ENT(aRLU,aDev,aArg,iArg);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
i8 iRLU_IXT_TRV_BEGIN(RLU* aRLU, void* aDev, void* aArg, i8 iArg)
{
  LogicUnit* pLogicUnit = _pLogicUnit(aRLU);
  
  i8 iRes = pLogicUnit->iRLU_IXT_TRV_BEGIN(aRLU,aDev,aArg,iArg);

  return iRes;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
i8 iRLU_IXT_TRV_NEXT(RLU* aRLU, void* aDev, void* aArg, i8 iArg)
{
  LogicUnit* pLogicUnit = _pLogicUnit(aRLU);
  
  i8 iRes = pLogicUnit->iRLU_IXT_TRV_NEXT(aRLU,aDev,aArg,iArg);

  return iRes;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
