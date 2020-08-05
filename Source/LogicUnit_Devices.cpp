/*============================================================================*/
/* LogicUnit_Devices.hpp -- Devices for Logic Unit                            */
/*============================================================================*/

#include "Expressions.h"
#include "DEV_interface.h"

/*----------------------------------------------------------------------------*/
/*                                rlu FUN                                     */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compare m_pEnt.key to m_oEntWrk.key                                        */
/*----------------------------------------------------------------------------*/
struct RluCommon* aRlu_INIT(void* pLU)
{
  LogicUnit* pLogicUnit = _pLogicUnit(pLU);
  
  struct RluCommon* aRluCommon = pLogicUnit->aRlu_INIT(pLU);
  
  return aRluCommon;
}


/*----------------------------------------------------------------------------*/
/*                                dev IXT                                     */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compare m_pEnt.key to m_oEntWrk.key                                        */
/*----------------------------------------------------------------------------*/
i8 iDev_ENT_COMPARE(void* pLU, void* aDevIXT)
{
  DevIXT* pDevIxt = _pDevIXT(aDevIXT);
  
  i8 iRes = pDevIxt->e_ENT_COMPARE(_pLogicUnit(pLU));
  
  return iRes;
}

/*----------------------------------------------------------------------------*/
/* move to next m_pEnt                                                        */
/*----------------------------------------------------------------------------*/
i8 iDev_ENT_NEXT(void* pLU, void* aDevIXT)
{
  return 0;
}

/*----------------------------------------------------------------------------*/
/* position m_pEnt prior using m_oEntWrk.key                                  */
/*----------------------------------------------------------------------------*/
void vDev_ENT_POSPRIOR(void* pLU, void* aDevIXT)
{
  
}

/*----------------------------------------------------------------------------*/
/* push entry                                                                 */
/*----------------------------------------------------------------------------*/
void* aDev_ENT_PUSH(void* pLU, void* aDevIXT)
{
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* push entry elm 0 int                                                       */
/*----------------------------------------------------------------------------*/
i8 iDev_ENT_PUSHELMI0(void* pLU, void* aDevIXT)
{
  return 0;
}

/*----------------------------------------------------------------------------*/
/* push entry elm 0 str                                                       */
/*----------------------------------------------------------------------------*/
Stz* szDev_ENT_PUSHELMS0(void* pLU, void* aDevIXT)
{
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* push entry offset                                                          */
/*----------------------------------------------------------------------------*/
i8 iDev_ENT_PUSHOFS(void* pLU, void* aDevIXT)
{
  return 0;
}

/*----------------------------------------------------------------------------*/
/* delete current entry m_aEnt from indexTree                                 */
/*----------------------------------------------------------------------------*/
void vDev_IXT_DEL_ENT(void* pLU, void* aDevIXT)
{
  
}

/*----------------------------------------------------------------------------*/
/* dump index tree                                                            */
/*----------------------------------------------------------------------------*/
void vDev_IXT_DUMP(void* pLU, void* aDevIXT)
{
  
}

/*----------------------------------------------------------------------------*/
/* insert an entry into the indexTree                                         */
/*----------------------------------------------------------------------------*/
void vDev_IXT_INS_ENT(void* pLU, void* aDevIXT)
{
  
}

/*----------------------------------------------------------------------------*/
/*  push number of entries in tmp index                                       */
/*----------------------------------------------------------------------------*/
i8 iDev_IXT_TMP_ENTRIES(void* pLU, void* aDevIXT)
{
  return 0;
}

/*----------------------------------------------------------------------------*/
/* begin linear traverse                                                      */
/*----------------------------------------------------------------------------*/
i8 iDev_IXT_TRV_BEGIN(void* pLU, void* aDevIXT)
{
  return 0;
}

/*----------------------------------------------------------------------------*/
/* traverse to next entry                                                     */
/*----------------------------------------------------------------------------*/
i8 iDev_IXT_TRV_NEXT(void* pLU, void* aDevIXT)
{
  return 0;
}

/*----------------------------------------------------------------------------*/
/* fetch a column from entry payload                                          */
/*----------------------------------------------------------------------------*/
i8 iDev_PAY_FETCH_COLI(void* pLU, void* aDevIXT, i8 iArg)
{
  return 0;
}

/*----------------------------------------------------------------------------*/
/* fetch a column from entry payload                                          */
/*----------------------------------------------------------------------------*/
Stz* szDev_PAY_FETCH_COLS(void* pLU, void* aDevIXT, i8 iArg)
{
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* store a column into entry payload                                          */
/*----------------------------------------------------------------------------*/
void vDev_PAY_STORE_COLI(void* pLU, void* aDevIXT, i8 iArg, i8 iTop)
{
  
}

/*----------------------------------------------------------------------------*/
/* store a column into entry payload                                          */
/*----------------------------------------------------------------------------*/
void vDev_PAY_STORE_COLS(void* pLU, void* aDevIXT, i8 iArg, Stz* azTop)
{
  
}


/*----------------------------------------------------------------------------*/
/*                                dev RSB                                     */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append next element value                                                  */
/*----------------------------------------------------------------------------*/
void vDev_ENTWRK_APPENDELM(void* pLU, void* aDevRSB)
{
  
}

/*----------------------------------------------------------------------------*/
/* append next element value                                                  */
/*----------------------------------------------------------------------------*/
void vDev_ENTWRK_APPENDELMI(void* pLU, void* aDevRSB, i8 iTop)
{
  
}

/*----------------------------------------------------------------------------*/
/* append next element value                                                  */
/*----------------------------------------------------------------------------*/
void vDev_ENTWRK_APPENDELMS(void* pLU, void* aDevRSB, Stz* azTop)
{
  
}



/*----------------------------------------------------------------------------*/
/* print src rsb                                                              */
/*----------------------------------------------------------------------------*/
void vDev_RSB_PRINT(void* pLU, void* pDevRSB)
{
  
}

/*----------------------------------------------------------------------------*/
/* begin src rsb traverse                                                     */
/*----------------------------------------------------------------------------*/
void vDev_RSB_BEGIN(void* pLU, void* pDevRSB)
{
  
}

/*----------------------------------------------------------------------------*/
/* move to next src rsb                                                       */
/*----------------------------------------------------------------------------*/
i8 iDev_RSB_NEXT(void* pLU, void* pDevRSB)
{
  return 0;
}

/*----------------------------------------------------------------------------*/
/* push offset of source rsb                                                  */
/*----------------------------------------------------------------------------*/
i8 iDev_RSB_PUSHOFS(void* pLU, void* pDevRSB)
{
  return 0;
}

/*----------------------------------------------------------------------------*/
/* set src rsb by using offset on stack                                       */
/*----------------------------------------------------------------------------*/
void vDev_RSB_SETBYOFS(void* pLU, void* pDevRSB, i8 iTop)
{
  
}

/*----------------------------------------------------------------------------*/
/* push src column                                                            */
/*----------------------------------------------------------------------------*/
i8 iDev_RSB_PUSHCOLI(void* pLU, void* pDevRSB, i8 iArg)
{
  return 0;
}

/*----------------------------------------------------------------------------*/
/* push src column                                                            */
/*----------------------------------------------------------------------------*/
Stz* szDev_RSB_PUSHCOLS(void* pLU, void* pDevRSB, i8 iArg)
{
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* push column value from m_aSrcCrw                                 NOT RIGHT */
/*----------------------------------------------------------------------------*/
void vDev_RSB_COL_PUSH(void* pLU, void* pDevRSB, i8 iArg)
{
  
}

/*----------------------------------------------------------------------------*/
/* push column value from m_aSrcCrw                                 NOT RIGHT */
/*----------------------------------------------------------------------------*/
Stz* szDev_RSB_COL_PUSHSTR(void* pLU, void* pDevRSB, i8 iArg)
{
  return nullptr;
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
