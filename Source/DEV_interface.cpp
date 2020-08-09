/*============================================================================*/
/* DEV_interface.cpp -- RLU Dev Interface                                     */
/*============================================================================*/

#include "Expressions.h"

#include "DEV_interface.h"


/*----------------------------------------------------------------------------*/
/* reset column in streamPix of dst                                           */
/*----------------------------------------------------------------------------*/
void vDev_COL_NOT_PROJECT(void* aLU, i8 iArg)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  pLogicUnit->m_oStreamPix.vNotProject(iArg);
}


/*----------------------------------------------------------------------------*/
/* begin rsb traverse                                                         */
/*----------------------------------------------------------------------------*/
void vDev_CRW_BEGIN(void* aLU, void* aIXT)
{
  _static_

  DevIXT* pDevIxt = _pDevIXT(aIXT);
  
  pDevIxt->m_i_Row = -1;
  pDevIxt->m_aCrw = nullptr;
  pDevIxt->m_nRows = pDevIxt->m_pCrwBuffer ?
                   pDevIxt->m_pCrwBuffer->nRowCount() : 0;
}

/*----------------------------------------------------------------------------*/
/* fetch m_aSrcCrw using m_pDevSrcIxt->m_aEnt.ref                             */
/*----------------------------------------------------------------------------*/
void vDev_CRW_FETCH(void* aLU, void* aIXT)
{
  _static_
  
  DevIXT* pDevIxt = _pDevIXT(aIXT);
  
  pDevIxt->m_aCrw  =
    CrwBuffer::pRefdCrw(pDevIxt->m_aEnt->rxRef());

}

/*----------------------------------------------------------------------------*/
/*  move to next src rsb                                                      */
/*----------------------------------------------------------------------------*/
i8 iDev_CRW_NEXT(void* aLU, void* aIXT)
{
  _static_
  
  DevIXT* pDevIxt = _pDevIXT(aIXT);

  i8 iRes;
  
  if (++pDevIxt->m_i_Row < pDevIxt->m_nRows) {
    pDevIxt->m_aCrw =
      pDevIxt->m_pCrwBuffer->aCrw(pDevIxt->m_i_Row);
    iRes = 1;
  }
  else {
    iRes = 0;
  }

  return iRes;
}

/*----------------------------------------------------------------------------*/
/* convert crw src to rsb                                           NOT RIGHT */
/*----------------------------------------------------------------------------*/
void vDev_CRW_TORSB(void* aLU, void* aIXT)
{
  _static_
  
  DevIXT* pDevIxt = _pDevIXT(aIXT);

  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  RowInflator::vInflateColumns(pLogicUnit->m_pDevDstRsb->m_pRowSlab,
                               pDevIxt->m_aCrw);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                dev IXT                                     */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compare m_pEnt.key to m_oEntWrk.key                                        */
/*----------------------------------------------------------------------------*/
i8 iDev_ENT_COMPARE(void* aLU, void* aIXT)
{
  _static_
  
  DevIXT* pDevIxt = _pDevIXT(aIXT);
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  
  Key* aEntKey = pDevIxt->m_aEnt->aKey();
  Key* aWrkKey = pLogicUnit->pEntWrk()->aKey();
  
  eKC eCmp = Key::eCompare(aEntKey,aWrkKey);

  return (i8)eCmp;
}

/*----------------------------------------------------------------------------*/
/* move to next m_pEnt                                                        */
/*----------------------------------------------------------------------------*/
i8 iDev_ENT_NEXT(void* aLU, void* aIXT)
{
  _static_
  
  DevIXT* pDevIxt = _pDevIXT(aIXT);
 
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
void vDev_ENT_POSPRIOR(void* aLU, void* aIXT)
{
  _static_
  
  DevIXT* pDevIxt = _pDevIXT(aIXT);
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  pDevIxt->m_aEnt =
    pDevIxt->m_pIdxTree->aPositionPriorEntry(pLogicUnit->m_oEntWrk.aKey(),
                                             &pDevIxt->m_pEntBuffer);
}

/*----------------------------------------------------------------------------*/
/* print both m_pEnt.key and m_oEntWrk.key                                    */
/*----------------------------------------------------------------------------*/
void vDev_ENT_PRINTKEYS(void* aLU, void* aIXT)
{
  _static_
  
  DevIXT* pDevIxt = _pDevIXT(aIXT);
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  
  SP_Ent spEnt;
  vTrace("\n\nEntWrk : %s\n"_Stz,
         spEnt.sz(pLogicUnit->m_oEntWrk.aEnt(),
                  pDevIxt->m_pIdxTree->pIndexDef()));
  
  vTrace("\n\nEnt : %s\n"_Stz,
         spEnt.sz(pDevIxt->m_aEnt,
                  pDevIxt->m_pIdxTree->pIndexDef()));
}

/*----------------------------------------------------------------------------*/
/* push entry                                                                 */
/*----------------------------------------------------------------------------*/
void* aDev_ENT_PUSH(void* aLU, void* aIXT)
{
  _static_
  
  DevIXT* pDevIxt = _pDevIXT(aIXT);
  
  return pDevIxt->m_aEnt;
}

/*----------------------------------------------------------------------------*/
/* push entry elm 0 int                                                       */
/*----------------------------------------------------------------------------*/
i8 iDev_ENT_PUSHELMI0(void* aLU, void* aIXT)
{
  _static_
  
  DevIXT* pDevIxt = _pDevIXT(aIXT);
  
  i8 iRes = pDevIxt->m_aEnt->aKey()->iGetIntElm();
  
  return iRes;
}

/*----------------------------------------------------------------------------*/
/* push entry elm 0 str                                                       */
/*----------------------------------------------------------------------------*/
Stz* szDev_ENT_PUSHELMS0(void* aLU, void* aIXT)
{
  _static_
  
  DevIXT* pDevIxt = _pDevIXT(aIXT);
  
  Stz* szSrc = pDevIxt->m_aEnt->aKey()->szGetStrElm();

  return szSrc;
}

/*----------------------------------------------------------------------------*/
/* push entry offset                                                          */
/*----------------------------------------------------------------------------*/
i8 iDev_ENT_PUSHOFS(void* aLU, void* aIXT)
{
  _static_
  
  DevIXT* pDevIxt = _pDevIXT(aIXT);
  
  i8 iRes = pDevIxt->m_aEnt->iBuffer();
  
  return iRes;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* delete current entry m_aEnt from indexTree                                 */
/*----------------------------------------------------------------------------*/
void vDev_IXT_DEL_ENT(void* aLU, void* aIXT)
{
  _static_
  
  DevIXT* pDevIXT = _pDevIXT(aIXT);

  pDevIXT->m_pIdxTree->vDelete(pDevIXT->m_pEntBuffer,pDevIXT->m_aEnt);
}

/*----------------------------------------------------------------------------*/
/* dump index tree                                                            */
/*----------------------------------------------------------------------------*/
void vDev_IXT_DUMP(void* aLU, void* aIXT)
{
  _static_
  
  DevIXT* pDevIXT = _pDevIXT(aIXT);
  
  pDevIXT->m_pIdxTree->vDump(false);
}

/*----------------------------------------------------------------------------*/
/* insert an entry into the indexTree                                         */
/*----------------------------------------------------------------------------*/
void vDev_IXT_INS_ENT(void* aLU, void* aIXT)
{
  _static_

  DevIXT* pDevIXT = _pDevIXT(aIXT);

  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  pDevIXT->m_aEnt =
    pDevIXT->m_pIdxTree->aInsertEntry(pLogicUnit->m_oEntWrk.aEnt());

}

/*----------------------------------------------------------------------------*/
/*  push number of entries in tmp index                                       */
/*----------------------------------------------------------------------------*/
i8 iDev_IXT_TMP_ENTRIES(void* aLU, void* aIXT)
{
  _static_
  
  DevIXT* pDevIXT = _pDevIXT(aIXT);

  i8 iRes = pDevIXT->m_pIdxTree->iCountH0Entries();

  return iRes;
}

/*----------------------------------------------------------------------------*/
/* begin linear traverse                                                      */
/*----------------------------------------------------------------------------*/
i8 iDev_IXT_TRV_BEGIN(void* aLU, void* aIXT)
{
  _static_

  DevIXT* pDevIXT = _pDevIXT(aIXT);

  i8 iRes;
  
  pDevIXT->m_pEntBuffer = pDevIXT->m_pIdxTree->pFirstH0Buffer();
  if (pDevIXT->m_pEntBuffer) {
    pDevIXT->m_aEnt = pDevIXT->m_pEntBuffer->aLeadEntry();
    if (pDevIXT->m_aEnt)
      iRes = 1;
    else
      iRes = 0;
  }
  else
    iRes = 0;

  return iRes;
}

/*----------------------------------------------------------------------------*/
/* traverse to next entry                                                     */
/*----------------------------------------------------------------------------*/
i8 iDev_IXT_TRV_NEXT(void* aLU, void* aIXT)
{
  _static_
  
  DevIXT* pDevIXT = _pDevIXT(aIXT);

  i8 iRes;
  
  pDevIXT->m_aEnt =
     EntBuffer::pMoveToNextEntry(pDevIXT->m_aEnt,&pDevIXT->m_pEntBuffer);
  if (pDevIXT->m_aEnt)
    iRes = 1;
  else
    iRes = 0;

  return iRes;
}

/*----------------------------------------------------------------------------*/
/* fetch a column from entry payload                                          */
/*----------------------------------------------------------------------------*/
i8 iDev_PAY_FETCH_COLI(void* aLU, void* aIXT, i8 iArg)
{
  _static_
  
  DevIXT* pDevIXT = _pDevIXT(aIXT);

  i8* aPayload = pDevIXT->m_aEnt->aiPayCell(iArg);
  
  i8 iRes = *aPayload;
  
  return iRes;
}

/*----------------------------------------------------------------------------*/
/* fetch a column from entry payload                                          */
/*----------------------------------------------------------------------------*/
Stz* szDev_PAY_FETCH_COLS(void* aLU, void* aIXT, i8 iArg)
{
  _static_
  
  DevIXT* pDevIXT = _pDevIXT(aIXT);

  i8* aPayload = pDevIXT->m_aEnt->aiPayCell(iArg);
  
  Stz* szSrc = (Stz*)aPayload;
  i8 nSize = StrLib::nSize((char*)szSrc);
  ASSERT(nSize < 8);

  return szSrc;
}

/*----------------------------------------------------------------------------*/
/* store a column into entry payload                                          */
/*----------------------------------------------------------------------------*/
void vDev_PAY_STORE_COLI(void* aLU, void* aIXT, i8 iArg, i8 iTop)
{
  _static_
  
  DevIXT* pDevIXT = _pDevIXT(aIXT);

  i8* aPayload = pDevIXT->m_aEnt->aiPayCell(iArg);
  *aPayload = iTop;
}

/*----------------------------------------------------------------------------*/
/* store a column into entry payload                                          */
/*----------------------------------------------------------------------------*/
Stz* szDev_PAY_STORE_COLS(void* aLU, void* aIXT, i8 iArg)
{
  _static_
  
  DevIXT* pDevIXT = _pDevIXT(aIXT);

  i8* aPayload = pDevIXT->m_aEnt->aiPayCell(iArg);

  return (Stz*)aPayload;
}


/*----------------------------------------------------------------------------*/
/*                                dev RSB                                     */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append next element value                                                  */
/*----------------------------------------------------------------------------*/
void vDev_ENTWRK_APPENDELM(void* aLU, void* aRSB, i8 iArg)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aRSB);
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  pLogicUnit->m_oEntWrk.vAppendElement(pDevRsb->m_aRsb,
                                       &pDevRsb->m_pRowSlab->m_oStreamPix,
                                       iArg);
}

/*----------------------------------------------------------------------------*/
/* append next element value                                                  */
/*----------------------------------------------------------------------------*/
void vDev_ENTWRK_APPENDELMI(void* aLU, void* aRSB, i8 iTop)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aRSB);

  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  pLogicUnit->m_oEntWrk.vAppendElementInt(pDevRsb->m_aRsb,iTop);
}

/*----------------------------------------------------------------------------*/
/* append next element value                                                  */
/*----------------------------------------------------------------------------*/
void vDev_ENTWRK_APPENDELMS(void* aLU, void* aRSB, Stz* szTop)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aRSB);

  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  pLogicUnit->m_oEntWrk.vAppendElementStz(pDevRsb->m_aRsb,szTop);
}

/*----------------------------------------------------------------------------*/
/* create payload                                                             */
/*----------------------------------------------------------------------------*/
void vDev_ENTWRK_CREATEPAY(void* aLU, void* aRSB, i8 iArg)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aRSB);

  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  pLogicUnit->m_oEntWrk.vPayload(true);
  pLogicUnit->m_oEntWrk.vPayloadBytes(iArg);
}

/*----------------------------------------------------------------------------*/
/* pop offset                                                                 */
/*----------------------------------------------------------------------------*/
void vDev_ENTWRK_POPOFS(void* aLU, void* aRSB, i8 iTop)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aRSB);

  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  pLogicUnit->m_oEntWrk.vBuffer(iTop);
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void vDev_ENTWRK_PRINT(void* aLU, void* aRSB)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aRSB);

  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  Ent* aEnt = pLogicUnit->m_oEntWrk.aEnt();
  IndexDef* pIndexDef = pLogicUnit->m_xpIndexTree->pIndexDef();
  
  SP_Ent spEnt;

  vTrace("EntWrk : %s\n"_Stz,spEnt.sz(aEnt,pIndexDef));
}

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void vDev_ENTWRK_RESET(void* aLU, void* aRSB)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aRSB);

  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  pLogicUnit->m_oEntWrk.vReset();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* print src rsb                                                              */
/*----------------------------------------------------------------------------*/
void vDev_RSB_PRINT(void* aLU, void* aRSB)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aRSB);
  
  pDevRsb->m_pRowSlab->vDumpRow(-1,pDevRsb->m_aRsb);
}

/*----------------------------------------------------------------------------*/
/* begin src rsb traverse                                                     */
/*----------------------------------------------------------------------------*/
void vDev_RSB_BEGIN(void* aLU, void* aRSB)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aRSB);
  
  pDevRsb->m_i_Row = -1;
  pDevRsb->m_aRsb  = nullptr;
  pDevRsb->m_nRows = pDevRsb->m_pRowSlab ? pDevRsb->m_pRowSlab->nRows() : 0;
}

/*----------------------------------------------------------------------------*/
/* move to next src rsb                                                       */
/*----------------------------------------------------------------------------*/
i8 iDev_RSB_NEXT(void* aLU, void* aRSB)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aRSB);
  
  i8 iRes;
  
  if (++pDevRsb->m_i_Row  < pDevRsb->m_nRows) {
    pDevRsb->m_aRsb = pDevRsb->m_pRowSlab->aRsbAdr(pDevRsb->m_i_Row );
    iRes = 1;
  }
  else {
    iRes = 0;
  }

  return iRes;
}

/*----------------------------------------------------------------------------*/
/* push offset of source rsb                                                  */
/*----------------------------------------------------------------------------*/
i8 iDev_RSB_PUSHOFS(void* aLU, void* aRSB)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aRSB);
  
  i8 iRes = pDevRsb->m_pRowSlab->iOfsOfRsb(pDevRsb->m_aRsb);

  return iRes;
}

/*----------------------------------------------------------------------------*/
/* set src rsb by using offset on stack                                       */
/*----------------------------------------------------------------------------*/
void vDev_RSB_SETBYOFS(void* aLU, void* aRSB, i8 iTop)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aRSB);
  
  pDevRsb->m_aRsb = pDevRsb->m_pRowSlab->aRsbByOfs(iTop);
}

/*----------------------------------------------------------------------------*/
/* push src column                                                            */
/*----------------------------------------------------------------------------*/
i8 iDev_RSB_PUSHCOLI(void* aLU, void* aRSB, i8 iArg)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aRSB);
  
  i8 iRes =
    pDevRsb->m_aRsb->iVal_ntv(pDevRsb->m_aRsb->i_ColToCell_ntv(iArg));

  return iRes;
}

/*----------------------------------------------------------------------------*/
/* push src column                                                            */
/*----------------------------------------------------------------------------*/
Stz* szDev_RSB_PUSHCOLS(void* aLU, void* aRSB, i8 iArg)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aRSB);
  
  Stz* szSrc = pDevRsb->m_aRsb->szVal_ntv(pDevRsb->m_aRsb->i_ColToCell_ntv(iArg));

  return szSrc;
}

/*----------------------------------------------------------------------------*/
/* push column value from m_aSrcCrw                                 NOT RIGHT */
/*----------------------------------------------------------------------------*/
i8 iDev_RSB_COL_PUSH(void* aLU, void* aRSB, i8 iArg)
{
  _static_
  
  //DevRSB* pDevRsb = _pDevRSB(aRSB); NOT RIGHT
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  HV_Row hRow(pLogicUnit->m_pDevSrcRsb->m_pStreamDef,C_ALL);
  RowInflator::vInflateColumn(hRow,pLogicUnit->m_pDevSrcIxt->m_aCrw,iArg);
  
  i8 iRes = hRow->m_waColVal[iArg].m_iWrd;

  return iRes;
}

/*----------------------------------------------------------------------------*/
/* push column value from m_aSrcCrw                                 NOT RIGHT */
/*----------------------------------------------------------------------------*/
void vDev_RSB_COL_PUSHSTR(void* aLU, void* aRSB, i8 iArg, i8*& raiDstk)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aRSB);
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  HV_Row hRow(pDevRsb->m_pStreamDef,C_ALL);
  RowInflator::vInflateColumn(hRow,pLogicUnit->m_pDevSrcIxt->m_aCrw,iArg);
  
  Stz* szStr = (Stz*)hRow->m_waColVal[iArg].m_iWrd;

  i8 nSize = StrLib::nSize((char*)szStr);
  char* azDst = (char*)(++raiDstk);
  StrLib::vCopy(azDst,(char*)szStr);
  raiDstk += (nSize / 8) + 1;
  *(++raiDstk) = nSize;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* push column value from m_aAltCrw                                 NOT RIGHT */
/*----------------------------------------------------------------------------*/
i8 iDev_ALT_COL_PUSH(void* aLU, void* aRSB, i8 iArg)
{
  _static_
  
  //DevRSB* pDevRsb = _pDevRSB(aRSB); NOT RIGHT
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  HV_Row hRow(pLogicUnit->m_pDevAltRsb->m_pStreamDef,C_ALL);
  RowInflator::vInflateColumn(hRow,pLogicUnit->m_pDevAltIxt->m_aCrw,iArg);
  
  i8 iRes = hRow->m_waColVal[iArg].m_iWrd;

  return iRes;
}


/*----------------------------------------------------------------------------*/
/* print stream pix                                                           */
/*----------------------------------------------------------------------------*/
void vDev_PIX_STM_PRINT(void* aLU)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  
  pLogicUnit->m_oStreamPix.vPrint();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
