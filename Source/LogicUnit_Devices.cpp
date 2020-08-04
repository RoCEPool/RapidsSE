/*============================================================================*/
/* LogicUnit_Devices.hpp -- Devices for Logic Unit                            */
/*============================================================================*/

#include "Expressions.h"
#include "DEV_interface.h"

/*----------------------------------------------------------------------------*/
/* compare m_pEnt.key to m_oEntWrk.key                                        */
/*----------------------------------------------------------------------------*/
i8 iDev_ENT_COMPARE(void* pRLU, void* aDev)
{
  DevIXT* pDevIxt = _pDevIXT(aDev);
  
  i8 iRes = pDevIxt->e_ENT_COMPARE(_pLogicUnit(pRLU));
  
  return iRes;
}

/*----------------------------------------------------------------------------*/
/* move to next m_pEnt                                                        */
/*----------------------------------------------------------------------------*/
i8 iDev_ENT_NEXT(void* pRLU, void* aDev)
{
  return 0;
}

/*----------------------------------------------------------------------------*/
/* position m_pEnt prior using m_oEntWrk.key                                  */
/*----------------------------------------------------------------------------*/
void vDev_ENT_POSPRIOR(void* pRLU, void* aDev)
{
  
}

/*----------------------------------------------------------------------------*/
/* push entry                                                                 */
/*----------------------------------------------------------------------------*/
void* aDev_ENT_PUSH(void* pRLU, void* aDev)
{
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* push entry elm 0 int                                                       */
/*----------------------------------------------------------------------------*/
i8 iDev_ENT_PUSHELMI0(void* pRLU, void* aDev)
{
  return 0;
}

/*----------------------------------------------------------------------------*/
/* push entry elm 0 str                                                       */
/*----------------------------------------------------------------------------*/
Stz* szDev_ENT_PUSHELMS0(void* pRLU, void* aDev)
{
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* push entry offset                                                          */
/*----------------------------------------------------------------------------*/
i8 iDev_ENT_PUSHOFS(void* pRLU, void* aDev)
{
  return 0;
}

/*----------------------------------------------------------------------------*/
/* append next element value                                                  */
/*----------------------------------------------------------------------------*/
void vDev_ENTWRK_APPENDELM(void* pRLU, void* aDev)
{
  
}

/*----------------------------------------------------------------------------*/
/* append next element value                                                  */
/*----------------------------------------------------------------------------*/
void vDev_ENTWRK_APPENDELMI(void* pRLU, void* aDev, i8 iTop)
{
  
}

/*----------------------------------------------------------------------------*/
/* append next element value                                                  */
/*----------------------------------------------------------------------------*/
void vDev_ENTWRK_APPENDELMS(void* pRLU, void* aDev, Stz* azTop)
{
  
}

/*----------------------------------------------------------------------------*/
/* delete current entry m_aEnt from indexTree                                 */
/*----------------------------------------------------------------------------*/
void vDev_IXT_DEL_ENT(void* pRLU, void* aDev)
{
  
}

/*----------------------------------------------------------------------------*/
/* dump index tree                                                            */
/*----------------------------------------------------------------------------*/
void vDev_IXT_DUMP(void* pRLU, void* aDev)
{
  
}

/*----------------------------------------------------------------------------*/
/* insert an entry into the indexTree                                         */
/*----------------------------------------------------------------------------*/
void vDev_IXT_INS_ENT(void* pRLU, void* aDev)
{
  
}

/*----------------------------------------------------------------------------*/
/*  push number of entries in tmp index                                       */
/*----------------------------------------------------------------------------*/
i8 iDev_IXT_TMP_ENTRIES(void* pRLU, void* aDev)
{
  return 0;
}

/*----------------------------------------------------------------------------*/
/* begin linear traverse                                                      */
/*----------------------------------------------------------------------------*/
i8 iDev_IXT_TRV_BEGIN(void* pRLU, void* aDev)
{
  return 0;
}

/*----------------------------------------------------------------------------*/
/* traverse to next entry                                                     */
/*----------------------------------------------------------------------------*/
i8 iDev_IXT_TRV_NEXT(void* pRLU, void* aDev)
{
  return 0;
}

/*----------------------------------------------------------------------------*/
/* fetch a column from entry payload                                          */
/*----------------------------------------------------------------------------*/
i8 iDev_PAY_FETCH_COLI(void* pRLU, void* aDev, i8 iArg)
{
  return 0;
}

/*----------------------------------------------------------------------------*/
/* fetch a column from entry payload                                          */
/*----------------------------------------------------------------------------*/
Stz* szDev_PAY_FETCH_COLS(void* pRLU, void* aDev, i8 iArg)
{
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* store a column into entry payload                                          */
/*----------------------------------------------------------------------------*/
void vDev_PAY_STORE_COLI(void* pRLU, void* aDev, i8 iArg, i8 iTop)
{
  
}

/*----------------------------------------------------------------------------*/
/* store a column into entry payload                                          */
/*----------------------------------------------------------------------------*/
void vDev_PAY_STORE_COLS(void* pRLU, void* aDev, i8 iArg, Stz* azTop)
{
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/



// TO BE REMOVED ...

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
