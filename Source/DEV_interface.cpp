/*============================================================================*/
/* DEV_interface.cpp -- RLU Dev Interface                                     */
/*============================================================================*/

#include "Expressions.h"

#include "DEV_interface.h"

/*----------------------------------------------------------------------------*/
/*                                dev IXT                                     */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compare m_pEnt.key to m_oEntWrk.key                                        */
/*----------------------------------------------------------------------------*/
i8 iDev_ENT_COMPARE(void* pLU, void* aDevIXT)
{
  _static_
  
  DevIXT* pDevIxt = _pDevIXT(aDevIXT);
  
  i8 iRes = pDevIxt->e_ENT_COMPARE(_pLogicUnit(pLU));
  
  return iRes;
}

/*----------------------------------------------------------------------------*/
/* move to next m_pEnt                                                        */
/*----------------------------------------------------------------------------*/
i8 iDev_ENT_NEXT(void* pLU, void* aDevIXT)
{
  _static_
  
  DevIXT* pDevIxt = _pDevIXT(aDevIXT);
 
  i8 iRes;
  
  pDevIxt->m_aEnt = EntBuffer::pMoveToNextEntry(pDevIxt->m_aEnt,
                                                &pDevIxt->m_pEntBuffer);
  if (pDevIxt->m_aEnt)
    iRes = 1;
  else
    iRes = 0;

  return iRes;
}

/*----------------------------------------------------------------------------*/
/* position m_pEnt prior using m_oEntWrk.key                                  */
/*----------------------------------------------------------------------------*/
void vDev_ENT_POSPRIOR(void* pLU, void* aDevIXT)
{
  _static_
  
  DevIXT* pDevIxt = _pDevIXT(aDevIXT);
  
  LogicUnit* pLogicUnit = _pLogicUnit(pLU);

  pDevIxt->m_aEnt =
    pDevIxt->m_pIdxTree->aPositionPriorEntry(pLogicUnit->m_oEntWrk.aKey(),
                                             &pDevIxt->m_pEntBuffer);
}

/*----------------------------------------------------------------------------*/
/* push entry                                                                 */
/*----------------------------------------------------------------------------*/
void* aDev_ENT_PUSH(void* pLU, void* aDevIXT)
{
  _static_
  
  DevIXT* pDevIxt = _pDevIXT(aDevIXT);
  
  return pDevIxt->m_aEnt;
}

/*----------------------------------------------------------------------------*/
/* push entry elm 0 int                                                       */
/*----------------------------------------------------------------------------*/
i8 iDev_ENT_PUSHELMI0(void* pLU, void* aDevIXT)
{
  _static_
  
  DevIXT* pDevIxt = _pDevIXT(aDevIXT);
  
  i8 iRes = pDevIxt->m_aEnt->aKey()->iGetIntElm();
  
  return iRes;
}

/*----------------------------------------------------------------------------*/
/* push entry elm 0 str                                                       */
/*----------------------------------------------------------------------------*/
Stz* szDev_ENT_PUSHELMS0(void* pLU, void* aDevIXT)
{
  _static_
  
  DevIXT* pDevIxt = _pDevIXT(aDevIXT);
  
  Stz* szSrc = pDevIxt->m_aEnt->aKey()->szGetStrElm();

  return szSrc;
}

/*----------------------------------------------------------------------------*/
/* push entry offset                                                          */
/*----------------------------------------------------------------------------*/
i8 iDev_ENT_PUSHOFS(void* pLU, void* aDevIXT)
{
  _static_
  
  DevIXT* pDevIxt = _pDevIXT(aDevIXT);
  
  i8 iRes = pDevIxt->m_aEnt->iBuffer();
  
  return iRes;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* delete current entry m_aEnt from indexTree                                 */
/*----------------------------------------------------------------------------*/
void vDev_IXT_DEL_ENT(void* pLU, void* aDevIXT)
{
  _static_
  
  DevIXT* pDevIXT = _pDevIXT(aDevIXT);

  pDevIXT->m_pIdxTree->vDelete(pDevIXT->m_pEntBuffer,pDevIXT->m_aEnt);
}

/*----------------------------------------------------------------------------*/
/* dump index tree                                                            */
/*----------------------------------------------------------------------------*/
void vDev_IXT_DUMP(void* pLU, void* aDevIXT)
{
  _static_
  
  DevIXT* pDevIXT = _pDevIXT(aDevIXT);

  pDevIXT->v_IXT_DUMP((LogicUnit*)pLU);
}

/*----------------------------------------------------------------------------*/
/* insert an entry into the indexTree                                         */
/*----------------------------------------------------------------------------*/
void vDev_IXT_INS_ENT(void* pLU, void* aDevIXT)
{
  _static_
  

}

/*----------------------------------------------------------------------------*/
/*  push number of entries in tmp index                                       */
/*----------------------------------------------------------------------------*/
i8 iDev_IXT_TMP_ENTRIES(void* pLU, void* aDevIXT)
{
  _static_
  
  return 0;
}

/*----------------------------------------------------------------------------*/
/* begin linear traverse                                                      */
/*----------------------------------------------------------------------------*/
i8 iDev_IXT_TRV_BEGIN(void* pLU, void* aDevIXT)
{
  _static_
  
  return 0;
}

/*----------------------------------------------------------------------------*/
/* traverse to next entry                                                     */
/*----------------------------------------------------------------------------*/
i8 iDev_IXT_TRV_NEXT(void* pLU, void* aDevIXT)
{
  _static_
  
  return 0;
}

/*----------------------------------------------------------------------------*/
/* fetch a column from entry payload                                          */
/*----------------------------------------------------------------------------*/
i8 iDev_PAY_FETCH_COLI(void* pLU, void* aDevIXT, i8 iArg)
{
  _static_
  
  return 0;
}

/*----------------------------------------------------------------------------*/
/* fetch a column from entry payload                                          */
/*----------------------------------------------------------------------------*/
Stz* szDev_PAY_FETCH_COLS(void* pLU, void* aDevIXT, i8 iArg)
{
  _static_
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* store a column into entry payload                                          */
/*----------------------------------------------------------------------------*/
void vDev_PAY_STORE_COLI(void* pLU, void* aDevIXT, i8 iArg, i8 iTop)
{
  _static_
  

}

/*----------------------------------------------------------------------------*/
/* store a column into entry payload                                          */
/*----------------------------------------------------------------------------*/
void vDev_PAY_STORE_COLS(void* pLU, void* aDevIXT, i8 iArg, Stz* azTop)
{
  _static_
  

}


/*----------------------------------------------------------------------------*/
/*                                dev RSB                                     */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append next element value                                                  */
/*----------------------------------------------------------------------------*/
void vDev_ENTWRK_APPENDELM(void* pLU, void* aDevRSB)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aDevRSB);
  
  LogicUnit* pLogicUnit = _pLogicUnit(pLU);

}

/*----------------------------------------------------------------------------*/
/* append next element value                                                  */
/*----------------------------------------------------------------------------*/
void vDev_ENTWRK_APPENDELMI(void* pLU, void* aDevRSB, i8 iTop)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aDevRSB);

  LogicUnit* pLogicUnit = _pLogicUnit(pLU);

}

/*----------------------------------------------------------------------------*/
/* append next element value                                                  */
/*----------------------------------------------------------------------------*/
void vDev_ENTWRK_APPENDELMS(void* pLU, void* aDevRSB, Stz* azTop)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aDevRSB);

  LogicUnit* pLogicUnit = _pLogicUnit(pLU);

}

/*----------------------------------------------------------------------------*/
/* create payload                                                             */
/*----------------------------------------------------------------------------*/
void vDev_ENTWRK_CREATEPAY(void* pLU, void* aDevRSB, i8 iArg)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aDevRSB);

  LogicUnit* pLogicUnit = _pLogicUnit(pLU);

}

/*----------------------------------------------------------------------------*/
/* pop offset                                                                 */
/*----------------------------------------------------------------------------*/
void vDev_ENTWRK_POPOFS(void* pLU, void* aDevRSB, i8 iTop)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aDevRSB);

  LogicUnit* pLogicUnit = _pLogicUnit(pLU);

}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void vDev_ENTWRK_PRINT(void* pLU, void* aDevRSB)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aDevRSB);

  LogicUnit* pLogicUnit = _pLogicUnit(pLU);

}

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void vDev_ENTWRK_RESET(void* pLU, void* aDevRSB)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aDevRSB);

  LogicUnit* pLogicUnit = _pLogicUnit(pLU);

}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* print src rsb                                                              */
/*----------------------------------------------------------------------------*/
void vDev_RSB_PRINT(void* pLU, void* pDevRSB)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(pDevRSB);
}

/*----------------------------------------------------------------------------*/
/* begin src rsb traverse                                                     */
/*----------------------------------------------------------------------------*/
void vDev_RSB_BEGIN(void* pLU, void* pDevRSB)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(pDevRSB);
}

/*----------------------------------------------------------------------------*/
/* move to next src rsb                                                       */
/*----------------------------------------------------------------------------*/
i8 iDev_RSB_NEXT(void* pLU, void* pDevRSB)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(pDevRSB);
  
  return 0;
}

/*----------------------------------------------------------------------------*/
/* push offset of source rsb                                                  */
/*----------------------------------------------------------------------------*/
i8 iDev_RSB_PUSHOFS(void* pLU, void* pDevRSB)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(pDevRSB);
  
  return 0;
}

/*----------------------------------------------------------------------------*/
/* set src rsb by using offset on stack                                       */
/*----------------------------------------------------------------------------*/
void vDev_RSB_SETBYOFS(void* pLU, void* pDevRSB, i8 iTop)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(pDevRSB);
  
}

/*----------------------------------------------------------------------------*/
/* push src column                                                            */
/*----------------------------------------------------------------------------*/
i8 iDev_RSB_PUSHCOLI(void* pLU, void* pDevRSB, i8 iArg)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(pDevRSB);
  
  return 0;
}

/*----------------------------------------------------------------------------*/
/* push src column                                                            */
/*----------------------------------------------------------------------------*/
Stz* szDev_RSB_PUSHCOLS(void* pLU, void* pDevRSB, i8 iArg)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(pDevRSB);
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* push column value from m_aSrcCrw                                 NOT RIGHT */
/*----------------------------------------------------------------------------*/
void vDev_RSB_COL_PUSH(void* pLU, void* pDevRSB, i8 iArg)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(pDevRSB);
  

}

/*----------------------------------------------------------------------------*/
/* push column value from m_aSrcCrw                                 NOT RIGHT */
/*----------------------------------------------------------------------------*/
Stz* szDev_RSB_COL_PUSHSTR(void* pLU, void* pDevRSB, i8 iArg)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(pDevRSB);
  
  return nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
