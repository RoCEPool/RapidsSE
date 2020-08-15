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
  
  if (KE_GENERAL == pDevIxt->m_aEnt->eKind()) {
    eKC eCmp = Key::eCompare(aEntKey,aWrkKey);
    return eCmp;
  }
  
  i8 iEntVal = *(i8*)aEntKey;
  i8 iWrkVal = *(i8*)aWrkKey;

  eKC eCmp;
  if (iEntVal < iWrkVal)
    eCmp = KC_BEFORE;
  else
  if (iEntVal < iWrkVal)
    eCmp = KC_SAME;
  else
    eCmp = KC_AFTER;

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
/* push entry                                                                 */
/*----------------------------------------------------------------------------*/
void* aDev_ENT_PUSHUPPER(void* aLU, void* aIXT)
{
  _static_
  
  DevIXT* pDevIxt = _pDevIXT(aIXT);
  
  return pDevIxt->m_aUpperEnt;
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
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  StrLib::iWordFlipBack(pLogicUnit->m_xRluCommon.m_caStrWrk,(char*)szSrc);

  return (Stz*)pLogicUnit->m_xRluCommon.m_caStrWrk;
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
/* compare src.key to alt.key                                                 */
/*----------------------------------------------------------------------------*/
i8 iDev_IXT_CMP_SRCALT(void* aLU)
{
  _static_

  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  
  i8 iRes;
  
  Key* aSrcKey = pLogicUnit->m_pDevSrcIxt->m_aEnt->aKey();
  Key* aAltKey = pLogicUnit->m_pDevAltIxt->m_aEnt->aKey();

  if (KE_GENERAL == pLogicUnit->m_pDevSrcIxt->m_aEnt->eKind()) {
    eKC  eCmp = Key::eCompare(aSrcKey,aAltKey);
  
    if (KC_BEFORE == eCmp)
     iRes = -1;
    else
    if (KC_SAME == eCmp)
     iRes = 0;
    else
     iRes = 1;
    
    return iRes;
  }
  

  i8 iSrcVal = *(i8*)aSrcKey;
  i8 iAltVal = *(i8*)aAltKey;

  if (iSrcVal < iAltVal)
     iRes = -1;
  else
  if (iSrcVal == iAltVal)
   iRes = 0;
  else
   iRes = 1;

  return iRes;
}

/*----------------------------------------------------------------------------*/
/* compare wrk.key to alt.key                                                 */
/*----------------------------------------------------------------------------*/
i8 iDev_IXT_CMP_WRKALT(void* aLU)
{
  _static_

  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  
  i8 iRes;
  
  Key* aWrkKey = pLogicUnit->m_oEntWrk.aKey();
  Key* aAltKey = pLogicUnit->m_pDevAltIxt->m_aEnt->aKey();

if (KE_GENERAL == pLogicUnit->m_pDevAltIxt->m_aEnt->eKind()) {
  
  eKC  eCmp = Key::eCompare(aWrkKey,aAltKey);

   if (KC_BEFORE == eCmp)
     iRes = -1;
   else
   if (KC_SAME == eCmp)
     iRes = 0;
   else
     iRes = 1;
  
    return iRes;
  }

  
  i8 iWrkVal = *(i8*)aWrkKey;
  i8 iAltVal = *(i8*)aAltKey;

  if (iWrkVal < iAltVal)
     iRes = -1;
  else
  if (iWrkVal == iAltVal)
   iRes = 0;
  else
   iRes = 1;

  return iRes;
}

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
/* print ent                                                                  */
/*----------------------------------------------------------------------------*/
void vDev_IXT_ENT_PRINT(void* aLU, void* aIXT)
{
  _static_
  
  DevIXT* pDevIXT = _pDevIXT(aIXT);
  
  SP_Ent spEnt;
  vTrace("Ent : %s\n"_Stz,
         spEnt.sz(pDevIXT->m_aEnt,
                  pDevIXT->m_pIdxTree->pIndexDef()));
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
/*  position alt ixt using src.key                                            */
/*----------------------------------------------------------------------------*/
i8 iDev_IXT_POS_ALTBYSRC(void* aLU)
{
  _static_

  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  i8 iRes;
  
  Key* aSrcKey = pLogicUnit->m_pDevSrcIxt->m_aEnt->aKey();
  
  pLogicUnit->m_pDevAltIxt->m_aEnt =
    pLogicUnit->m_pDevAltIxt->m_pIdxTree->aPositionPriorEntry(aSrcKey,
                                       &pLogicUnit->m_pDevAltIxt->m_pEntBuffer);
  
  if (pLogicUnit->m_pDevAltIxt->m_pEntBuffer) {
    if (pLogicUnit->m_pDevAltIxt->m_aEnt)
      iRes = 1;
    else
      iRes = 0;
  }
  else
    iRes = 0;

  return iRes;
}

/*----------------------------------------------------------------------------*/
/*  position alt ixt using entwrk.key                                         */
/*----------------------------------------------------------------------------*/
i8 iDev_IXT_POS_ALTBYWRK(void* aLU)
{
  _static_

  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  i8 iRes;
  
  Key* aWrkKey = pLogicUnit->m_oEntWrk.aKey();
  
  pLogicUnit->m_pDevAltIxt->m_aEnt =
    pLogicUnit->m_pDevAltIxt->m_pIdxTree->aPositionPriorEntry(aWrkKey,
                                       &pLogicUnit->m_pDevAltIxt->m_pEntBuffer);
  
  if (pLogicUnit->m_pDevAltIxt->m_pEntBuffer) {
    if (pLogicUnit->m_pDevAltIxt->m_aEnt)
      iRes = 1;
    else
      iRes = 0;
  }
  else
    iRes = 0;

  return iRes;
}

/*----------------------------------------------------------------------------*/
/* insert an entry into the indexTree                                         */
/*----------------------------------------------------------------------------*/
i8 iDev_IXT_POS_LBOUND(void* aLU, void* aIXT)
{
  _static_

  DevIXT* pDevIXT = _pDevIXT(aIXT);

  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  pDevIXT->m_pEntBuffer =
    pDevIXT->m_pIdxTree->pPositionH0Buffer(pLogicUnit->m_oEntWrk.aKey());
  
  if (pDevIXT->m_pEntBuffer)
    pDevIXT->m_aEnt =
      pDevIXT->m_pEntBuffer->aFindEntryBefore(pLogicUnit->m_oEntWrk.aKey());
  
  i8 iRes;
  if (pDevIXT->m_aEnt)
    iRes = 1;
  else
    iRes = 0;

  return iRes;
}

/*----------------------------------------------------------------------------*/
/* insert an entry into the indexTree                                         */
/*----------------------------------------------------------------------------*/
i8 iDev_IXT_POS_UBOUND(void* aLU, void* aIXT)
{
  _static_

  i8 iRes;

  DevIXT* pDevIXT = _pDevIXT(aIXT);

  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  
  EntBuffer* pUpperEntBuffer =
    pDevIXT->m_pIdxTree->pPositionH0Buffer(pLogicUnit->m_oEntWrk.aKey());
  
  if (pDevIXT) {
    
    pDevIXT->m_aUpperEnt =
      pUpperEntBuffer->aFindEntryBefore(pLogicUnit->m_oEntWrk.aKey());
    
    if (pDevIXT->m_aUpperEnt) {
      pDevIXT->m_aUpperEnt =
        EntBuffer::pMoveToGreaterThanEntry(pDevIXT->m_aUpperEnt,
                                           pLogicUnit->m_oEntWrk.aKey(),
                                           &pUpperEntBuffer);
      if (pDevIXT->m_aUpperEnt)
        iRes = 1;
      else
        iRes = 0;
    }
    else
      iRes = 0;
  }
  else
    iRes = 0;

  return iRes;
}

/*----------------------------------------------------------------------------*/
/* insert an entry into the indexTree                                         */
/*----------------------------------------------------------------------------*/
i8 iDev_IXT_POS_UBOUND_LT(void* aLU, void* aIXT)
{
  _static_
  
  i8 iRes;
  
  DevIXT* pDevIXT = _pDevIXT(aIXT);
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  
  
  EntBuffer* pUpperEntBuffer =
  pDevIXT->m_pIdxTree->pPositionH0Buffer(pLogicUnit->m_oEntWrk.aKey());
  
  if (pUpperEntBuffer) {
    
    pDevIXT->m_aUpperEnt =
    pUpperEntBuffer->aFindEntryBefore(pLogicUnit->m_oEntWrk.aKey());
    
    if (pDevIXT->m_aUpperEnt) {
      pDevIXT->m_aUpperEnt =
      EntBuffer::pMoveToNextEntry(pDevIXT->m_aUpperEnt,
                                  &pUpperEntBuffer);
      
      if (pDevIXT->m_aUpperEnt)
        iRes = 1;
      else
        iRes = 0;
    }
    else
      iRes = 0;
  }
  else
    iRes = 0;
  
  return iRes;
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
                                       pDevRsb->m_pRowSlab->aStreamPix(),
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
/* create new rsb                                                             */
/*----------------------------------------------------------------------------*/
i8 iDev_RSB_NEW(void* aLU, void* aRSB)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aRSB);
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  bool bNewBuffer;
  
  pDevRsb->m_aRsb =
    pLogicUnit->m_xpPot->aNewDstRsb(pLogicUnit->m_xRluCommon.m_i_Ptn,
                                   &pDevRsb->m_pRowSlab, &bNewBuffer);
  
  return bNewBuffer ? 1 : 0;
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
/* pop and deposit top into dst column                                        */
/*----------------------------------------------------------------------------*/
void vDev_RSB_POPCOLI(void* aLU, void* aRSB, i8 iArg, i8 iTop)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aRSB);
  
  pDevRsb->m_pRowSlab->vPositionOnCell(iArg);
  pDevRsb->m_pRowSlab->vDeposit(iTop);
}

/*----------------------------------------------------------------------------*/
/* pop and deposit top into dst column                                        */
/*----------------------------------------------------------------------------*/
void vDev_RSB_POPCOLS(void* aLU, void* aRSB, i8 iArg, Stz* szTop)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aRSB);
  
  pDevRsb->m_pRowSlab->vPositionOnCell(iArg);
  
  char* azDst = pDevRsb->m_pRowSlab->azDepositString_begin();
  
  StrLib::vCopy(azDst,(char*)szTop);
  i8 nSize = StrLib::nSize(azDst);

  pDevRsb->m_pRowSlab->vDepositString_end(nSize);
}

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
/* push src column                                                            */
/*----------------------------------------------------------------------------*/
i8 iDev_RSB_PUSHCOLI(void* aLU, void* aRSB, i8 iArg)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aRSB);
    
  i8 iRes = pDevRsb->m_aRsb->iVal(iArg);

  return iRes;
}

/*----------------------------------------------------------------------------*/
/* push src column                                                            */
/*----------------------------------------------------------------------------*/
Stz* szDev_RSB_PUSHCOLS(void* aLU, void* aRSB, i8 iArg)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aRSB);
  
  Stz* szSrc = pDevRsb->m_aRsb->szVal(iArg);

  return szSrc;
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
/* reset rsb                                                                  */
/*----------------------------------------------------------------------------*/
void vDev_RSB_RESET(void* aLU, void* aRSB)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aRSB);
  
  pDevRsb->m_pRowSlab->vReset();
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
/* push offset of source rsb                                                  */
/*----------------------------------------------------------------------------*/
i8 iDev_RSB_TEST(void* aLU, void* aRSB)
{
  _static_
    
  DevRSB* pDevRsb = _pDevRSB(aRSB);

  i8 iRes;

  if (pDevRsb->m_aRsb)
    iRes = 1;
  else
    iRes = 0;

  return iRes;
}

/*----------------------------------------------------------------------------*/
/* zero rsb                                                                   */
/*----------------------------------------------------------------------------*/
void vDev_RSB_ZERO(void* aLU, void* aRSB)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aRSB);
  
  pDevRsb->m_pRowSlab->vZeroRsb(pDevRsb->m_aRsb);
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
void vDev_RSB_COL_PUSHI(void* aLU, void* aRSB, i8 iArg, i8** aaiDstk)
{
  _static_
  
  LogicUnit* pLogicUnit = (LogicUnit*)aLU; //_pLogicUnit(aLU);
  
//  *(++*aaiDstk) = -1;
//  *(++*aaiDstk) = iArg; // col to inflate
  
  RowInflator::vInflateColI_stk(aaiDstk,
                   pLogicUnit->m_pDevSrcIxt->m_aCrw,
                   pLogicUnit->m_pDevSrcRsb->m_pStreamDef);
  
//  RowInflator::vInflateColI(aamDstk,
//                            pLogicUnit->m_pDevSrcIxt->m_aCrw,
//                            iArg,
//                            pLogicUnit->m_pDevSrcRsb->m_pStreamDef);
}

/*----------------------------------------------------------------------------*/
/* push column value from m_aSrcCrw                                 NOT RIGHT */
/*----------------------------------------------------------------------------*/
void vDev_RSB_COL_PUSHSTR(void* aLU, void* aRSB, i8 iArg, i8** aaiDstk)
{
  _static_
  
  vDev_RSB_COL_PUSHI(aLU,aRSB,iArg,aaiDstk);
  
  /*
  DevRSB* pDevRsb = _pDevRSB(aRSB);
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  HV_Row hRow(pDevRsb->m_pStreamDef,C_ALL);
  RowInflator::vInflateColumn(hRow,pLogicUnit->m_pDevSrcIxt->m_aCrw,iArg);
  
  Stz* szStr = (Stz*)hRow->m_waColVal[iArg].m_iWrd;

  i8 nSize = StrLib::nSize((char*)szStr);
  char* azDst = (char*)(++*aaiDstk);
  StrLib::vCopy(azDst,(char*)szStr);
  *aaiDstk += (nSize / 8) + 1;
  *(++*aaiDstk) = nSize;
  */
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
/* push column value from m_aSrcCrw                                 NOT RIGHT */
/*----------------------------------------------------------------------------*/
void vDev_ALT_COL_PUSHI(void* aLU, void* aRSB, i8 iArg, i8** aaiDstk)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
      
//  *(++*aaiDstk) = -1;
//  *(++*aaiDstk) = iArg; // col to inflate

  RowInflator::vInflateColI_stk(aaiDstk,
                   pLogicUnit->m_pDevAltIxt->m_aCrw,
                   pLogicUnit->m_pDevAltRsb->m_pStreamDef);
}

/*----------------------------------------------------------------------------*/
/* push column value from m_aSrcCrw                                 NOT RIGHT */
/*----------------------------------------------------------------------------*/
void vDev_SRC_COL_PUSHI(void* aLU, i8 iArg, i8** aaiDstk)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  
  //  *(++*aaiDstk) = -1;
  //  *(++*aaiDstk) = iArg; // col to inflate
  
  RowInflator::vInflateColI_stk(aaiDstk,
                                pLogicUnit->m_pDevSrcIxt->m_aCrw,
                                *pLogicUnit->m_xpStreamDef);
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
