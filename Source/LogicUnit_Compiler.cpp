/*============================================================================*/
/* LogicUnit_Compiler.hpp -- Compiler iterface for Logic Unit                 */
/*============================================================================*/

#include "Expressions.h"

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iDEV_MAP_DEL_ENT(void* aRLU, void* aDev,
                               void** aaCrs,void* aArg, i8 iArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  return C_IGNORED;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iDEV_MAP_DUMP(void* aRLU, void* aDev,
                            void** aaCrs, void* aArg, i8 iArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  IndexTree* pIndexTree = _pIndexTree(aDev);
  
  pIndexTree->vDump(false);

  return C_IGNORED;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iDEV_MAP_ENTRIES(void* aRLU, void* aDev,
                               void** aaCrs, void* aArg, i8 iArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  return 0;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iDEV_MAP_INS_ENT(void* aRLU, void* aDev,
                               void** aaCrs, void* aArg, i8 iArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  return C_IGNORED;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iDEV_MAP_TRV_BEGIN(void* aRLU, void* aDev,
                                 void** aaCrs, void* aArg, i8 iArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  return 0;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
i8 LogicUnit::iDEV_MAP_TRV_NEXT(void* aRLU, void* aDev,
                                void** aaCrs, void* aArg, i8 iArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  return 0;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

#include "DEV_interface.h"
#include "DEV_enum.h"

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
i8 iDEV_MAP(void* aRLU, void* aDev, i8 eDO, void** aaCrs, void* aArg, i8 iArg)
{
  switch (eDO) {
    case DEV_MAP_DEL_ENT:   return iDEV_MAP_DEL_ENT  (aRLU,aDev,aaCrs,aArg,iArg);
    case DEV_MAP_DUMP:      return iDEV_MAP_DUMP     (aRLU,aDev,aaCrs,aArg,iArg);
    case DEV_MAP_ENTRIES:   return iDEV_MAP_ENTRIES  (aRLU,aDev,aaCrs,aArg,iArg);
    case DEV_MAP_INS_ENT:   return iDEV_MAP_INS_ENT  (aRLU,aDev,aaCrs,aArg,iArg);
    case DEV_MAP_TRV_BEGIN: return iDEV_MAP_TRV_BEGIN(aRLU,aDev,aaCrs,aArg,iArg);
    case DEV_MAP_TRV_NEXT:  return iDEV_MAP_TRV_NEXT (aRLU,aDev,aaCrs,aArg,iArg);
    default:
    NEVER_REACH;
  }
  return 0;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
i8 iDEV_MAP_DEL_ENT(void* aRLU, void* aDev,
                    void** aaCrs, void* aArg, i8 iArg)
{
  LogicUnit* pLogicUnit = _pLogicUnit(aRLU);

  i8 iRes = pLogicUnit->iDEV_MAP_DEL_ENT(aRLU,aDev,aaCrs,aArg,iArg);
  
  return iRes;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
i8 iDEV_MAP_DUMP(void* aRLU, void* aDev,
                 void** aaCrs, void* aArg, i8 iArg)
{
  LogicUnit* pLogicUnit = _pLogicUnit(aRLU);
  
  i8 iRes = pLogicUnit->iDEV_MAP_DUMP(aRLU,aDev,aaCrs,aArg,iArg);
  
  return iRes;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
i8 iDEV_MAP_ENTRIES(void* aRLU, void* aDev,
                    void** aaCrs, void* aArg, i8 iArg)
{
  LogicUnit* pLogicUnit = _pLogicUnit(aRLU);
  
  i8 iRes = pLogicUnit->iDEV_MAP_ENTRIES(aRLU,aDev,aaCrs,aArg,iArg);

  return iRes;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
i8 iDEV_MAP_INS_ENT(void* aRLU, void* aDev,
                    void** aaCrs, void* aArg, i8 iArg)
{
  LogicUnit* pLogicUnit = _pLogicUnit(aRLU);

  i8 iRes = pLogicUnit->iDEV_MAP_INS_ENT(aRLU,aDev,aaCrs,aArg,iArg);

  return iRes;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
i8 iDEV_MAP_TRV_BEGIN(void* aRLU, void* aDev,
                      void** aaCrs, void* aArg, i8 iArg)
{
  LogicUnit* pLogicUnit = _pLogicUnit(aRLU);
  
  i8 iRes = pLogicUnit->iDEV_MAP_TRV_BEGIN(aRLU,aDev,aaCrs,aArg,iArg);

  return iRes;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
i8 iDEV_MAP_TRV_NEXT(void* aRLU, void* aDev,
                     void** aaCrs, void* aArg, i8 iArg)
{
  LogicUnit* pLogicUnit = _pLogicUnit(aRLU);
  
  i8 iRes = pLogicUnit->iDEV_MAP_TRV_NEXT(aRLU,aDev,aaCrs,aArg,iArg);

  return iRes;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
