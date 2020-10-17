/*============================================================================*/
/* DEV_interface.cpp -- RLU Dev Interface                                     */
/*============================================================================*/

#include "Expressions.h"

#include "DEV_interface.h"

/*----------------------------------------------------------------------------*/
/* begin rsb traverse                                                         */
/*----------------------------------------------------------------------------*/
void vDev_CRW_BEGIN(void* aLU, void* aIXT)
{
  _static_

  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  DevIXT* pDevIxt = _pDevIXT(aIXT);
  
  pDevIxt->m_i_Row = -1;
  pDevIxt->m_aCrw = nullptr;
  pDevIxt->m_nRows = pDevIxt->m_pCrwBuffer ?
                   pDevIxt->m_pCrwBuffer->nRowCount() : 0;
  
  pLogicUnit->m_xRluCommon.m_nCrwRows = pDevIxt->m_nRows;
  pLogicUnit->m_xRluCommon.m_i_CrwRow = pDevIxt->m_i_Row;
      
  if (pDevIxt->m_pCrwBuffer)
    pLogicUnit->m_xRluCommon.m_aSrcHeadCrw = pDevIxt->m_pCrwBuffer->aGetHead(0);
  else
    pLogicUnit->m_xRluCommon.m_aSrcHeadCrw = nullptr;
}

/*----------------------------------------------------------------------------*/
/* fetch m_aSrcCrw using m_pDevSrcIxt->m_aEnt.ref                             */
/*----------------------------------------------------------------------------*/
void vDev_CRW_DEREF(void* aLU, void* aIXT)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  DevIXT* pDevIxt = _pDevIXT(aIXT);
  
  pDevIxt->m_aCrw  =
    CrwBuffer::pRefdCrw(pDevIxt->m_aEnt->rxRef());
  
  // can we calc it?
  
  i8 iBuffer = ((Ent_int*)pDevIxt->m_aEnt)->Rrf.m_uBuffer;
  i8 iItem   = ((Ent_int*)pDevIxt->m_aEnt)->Rrf.m_uItem;
  
  Crw* aCrw  = (Crw*)
    ( (char*)C_ADR_REGION
             + iBuffer * C_1MB
             + C_CRW_FIRSTHEAD
             + iItem * pLogicUnit->m_xRluCommon.m_nSrcHeadSize );
  
  RASSERT(aCrw == pDevIxt->m_aCrw);
  
  // ... yes we can
}

/*----------------------------------------------------------------------------*/
/*  move to next src rsb                                                      */
/*----------------------------------------------------------------------------*/
i8 iDev_CRW_NEXT(void* aLU, void* aIXT)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  DevIXT* pDevIxt = _pDevIXT(aIXT);

  i8 iRes;
  
  if (++pDevIxt->m_i_Row < pDevIxt->m_nRows) {
    pDevIxt->m_aCrw =
      pDevIxt->m_pCrwBuffer->aCrw(pDevIxt->m_i_Row);
    pLogicUnit->m_xRluCommon.m_aSrcCrw = pDevIxt->m_aCrw;
    iRes = 1;
  }
  else {
    iRes = 0;
  }

  return iRes;
}

/*----------------------------------------------------------------------------*/
/*  move to next src rsb                                                      */
/*----------------------------------------------------------------------------*/
void vDev_CRW_NULL(void* aLU, void* aIXT)
{
  _static_
  
  DevIXT* pDevIxt = _pDevIXT(aIXT);
  pDevIxt->m_pCrwBuffer = nullptr;
}

/*----------------------------------------------------------------------------*/
/*  test if there is a crw src                                                */
/*----------------------------------------------------------------------------*/
i8 iDev_CRW_TEST(void* aLU, void* aIXT)
{
  _static_
  
  DevIXT* pDevIxt = _pDevIXT(aIXT);
  
  i8 iRes;
  
  if (pDevIxt->m_pCrwBuffer)
    iRes = 1;
  else
    iRes = 0;
  
  return iRes;
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
  if (iEntVal == iWrkVal)
    eCmp = KC_SAME;
  else
    eCmp = KC_AFTER;

  return (i8)eCmp;
}

/*----------------------------------------------------------------------------*/
/* move to next m_pEnt                                                        */
/*----------------------------------------------------------------------------*/
Ent_int* aDev_ENT_NEXT(void* aLU, void* aIXT)
{
  _static_
  
  DevIXT* pDevIxt = _pDevIXT(aIXT);
 
  i8 iRes;
  
  pDevIxt->m_aEnt = EntBuffer::pMoveToNextEntry(pDevIxt->m_aEnt,
                                                &pDevIxt->m_pEntBuffer);
  return (Ent_int*)pDevIxt->m_aEnt;
}

/*----------------------------------------------------------------------------*/
/* position m_pEnt prior using m_oEntWrk.key                                  */
/*----------------------------------------------------------------------------*/
Ent_int* aDev_ENT_POSPRIOR(void* aLU, void* aIXT, Ent_int** aaSkipRoots)
{
  _static_
  
  DevIXT* pDevIxt = _pDevIXT(aIXT);
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  pDevIxt->m_aEnt =
    pDevIxt->m_pIdxTree->aPositionPriorEntry(pLogicUnit->m_oEntWrk.aKey(),
                                             &pDevIxt->m_pEntBuffer);
  
  if (pDevIxt->m_pEntBuffer) {
    pLogicUnit->m_xRluCommon.m_aEntArray =
      (Ent_int*)pDevIxt->m_pEntBuffer->aEntArray();
    
    *aaSkipRoots = (Ent_int*)pDevIxt->m_pEntBuffer->aSkipRoots();
  }
  
  return (Ent_int*)pDevIxt->m_aEnt;
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
  
  i8 iRes = pDevIxt->m_aEnt->iIntElm0();

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
/* set the index name                                                         */
/*----------------------------------------------------------------------------*/
void vDev_IXT_INDEXNM_SET(void* aLU, void* aIXT, Stz* szIndexNm)
{
  _static_
  
  DevIXT* pDevIxt = _pDevIXT(aIXT);
  pDevIxt->m_sIndexNm = szIndexNm;
}

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
Ent_int* aDev_IXT_INS_ENT(void* aLU, void* aIXT)
{
  _static_

  DevIXT* pDevIXT = _pDevIXT(aIXT);

  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  pDevIXT->m_aEnt =
    pDevIXT->m_pIdxTree->aInsertEntry(pLogicUnit->m_oEntWrk.aEnt());
  
  return (Ent_int*)pDevIXT->m_aEnt;
}

/*----------------------------------------------------------------------------*/
/* delete the indexTree from the dev                                          */
/*----------------------------------------------------------------------------*/
void vDev_IXT_INDEXTREE_DELETE(void* aLU, void* aIXT)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  DevIXT* pDevIxt = _pDevIXT(aIXT);
  IndexTree* pIndexTree = pDevIxt->m_pIdxTree;
  pDevIxt->m_pIdxTree = nullptr;
  delete pIndexTree;
}

/*----------------------------------------------------------------------------*/
/* set the indexTree[] in the dev                                             */
/*----------------------------------------------------------------------------*/
void vDev_IXT_INDEXTREE_SET(void* aLU, void* aIXT)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  DevIXT* pDevIxt = _pDevIXT(aIXT);
  pDevIxt->m_pIdxTree = * pLogicUnit->m_xpIndexTree;
  pLogicUnit->m_xpIndexTree = nullptr;
}

/*----------------------------------------------------------------------------*/
/*  position alt ixt using src.key                                            */
/*----------------------------------------------------------------------------*/
Ent_int* aDev_IXT_POS_ALTBYSRC(void* aLU)
{
  _static_

  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  
  Key* aSrcKey = pLogicUnit->m_pDevSrcIxt->m_aEnt->aKey();
  
  pLogicUnit->m_pDevAltIxt->m_aEnt =
    pLogicUnit->m_pDevAltIxt->m_pIdxTree->aPositionPriorEntry(aSrcKey,
                                       &pLogicUnit->m_pDevAltIxt->m_pEntBuffer);
  
  if (pLogicUnit->m_pDevAltIxt->m_pEntBuffer) {
    
    pLogicUnit->m_xRluCommon.m_aEntArray =
      (Ent_int*)pLogicUnit->m_pDevAltIxt->m_pEntBuffer->aEntArray();
    
    if (pLogicUnit->m_pDevAltIxt->m_aEnt)
      return (Ent_int*)pLogicUnit->m_pDevAltIxt->m_aEnt;
    else
      return nullptr;
  }
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/*  position alt ixt using entwrk.key                                         */
/*----------------------------------------------------------------------------*/
Ent_int* aDev_IXT_POS_ALTBYWRK(void* aLU)
{
  _static_

  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  Key* aWrkKey = pLogicUnit->m_oEntWrk.aKey();
  
  pLogicUnit->m_pDevAltIxt->m_aEnt =
    pLogicUnit->m_pDevAltIxt->m_pIdxTree->aPositionPriorEntry(aWrkKey,
                                       &pLogicUnit->m_pDevAltIxt->m_pEntBuffer);
  
  if (pLogicUnit->m_pDevAltIxt->m_pEntBuffer) {
    
    pLogicUnit->m_xRluCommon.m_aEntArray =
      (Ent_int*)pLogicUnit->m_pDevAltIxt->m_pEntBuffer->aEntArray();

    if (pLogicUnit->m_pDevAltIxt->m_aEnt)
      return (Ent_int*)pLogicUnit->m_pDevAltIxt->m_aEnt;
    else
      return nullptr;
  }
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* insert an entry into the indexTree                                         */
/*----------------------------------------------------------------------------*/
Ent_int* aDev_IXT_POS_LBOUND(void* aLU, void* aIXT)
{
  _static_

  DevIXT* pDevIXT = _pDevIXT(aIXT);

  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  pDevIXT->m_aEnt = nullptr;
  
  pDevIXT->m_pEntBuffer =
    pDevIXT->m_pIdxTree->pPositionH0Buffer(pLogicUnit->m_oEntWrk.aKey());
  
  if (pDevIXT->m_pEntBuffer) {
    pDevIXT->m_aEnt =
      pDevIXT->m_pEntBuffer->aFindEntryBefore(pLogicUnit->m_oEntWrk.aKey());
  }
  
  if (pDevIXT->m_aEnt) {
    
    pLogicUnit->m_xRluCommon.m_aEntArray =
      (Ent_int*)pDevIXT->m_pEntBuffer->aEntArray();

    return (Ent_int*)pDevIXT->m_aEnt;
  }
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* insert an entry into the indexTree                                         */
/*----------------------------------------------------------------------------*/
Ent_int* aDev_IXT_POS_UBOUND(void* aLU, void* aIXT)
{
  _static_

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
      if (pDevIXT->m_aUpperEnt) {
        
        pLogicUnit->m_xRluCommon.m_aEntArray =
          (Ent_int*)pUpperEntBuffer->aEntArray();

        return (Ent_int*)pDevIXT->m_aUpperEnt;
      }
      else
        return nullptr;
    }
    else
      return nullptr;
  }
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* insert an entry into the indexTree                                         */
/*----------------------------------------------------------------------------*/
Ent_int* aDev_IXT_POS_UBOUND_LT(void* aLU, void* aIXT)
{
  _static_
    
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
      
      if (pDevIXT->m_aUpperEnt) {
        
        pLogicUnit->m_xRluCommon.m_aEntArray =
          (Ent_int*)pUpperEntBuffer->aEntArray();
        
        return (Ent_int*)pDevIXT->m_aUpperEnt;
      }
      else
        return nullptr;
    }
    else
      return nullptr;
  }
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* set m_aEnt                                                                 */
/*----------------------------------------------------------------------------*/
void vDev_IXT_SET_ENT(void* aLU, void* aIXT, void* aEnt)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  DevIXT* pDevIXT = _pDevIXT(aIXT);
  
  pDevIXT->m_aEnt = (Ent*)aEnt;
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
Ent_int* aDev_IXT_TRV_BEGIN(void* aLU, void* aIXT)
{
  _static_

  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  DevIXT* pDevIXT = _pDevIXT(aIXT);
  
  pDevIXT->m_pEntBuffer = pDevIXT->m_pIdxTree->pFirstH0Buffer();
  
  if (pDevIXT->m_pEntBuffer) {
    pDevIXT->m_aEnt = pDevIXT->m_pEntBuffer->aLeadEntry();
    if (pDevIXT->m_aEnt) {
      pLogicUnit->m_xRluCommon.m_aEntArray =
        (Ent_int*)pDevIXT->m_pEntBuffer->aEntArray();
      return (Ent_int*)pDevIXT->m_aEnt;
    }
    else
      return nullptr;
  }
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* traverse to next entry                                                     */
/*----------------------------------------------------------------------------*/
Ent_int* aDev_IXT_TRV_NEXT(void* aLU, void* aIXT)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  DevIXT* pDevIXT = _pDevIXT(aIXT);

  
  pDevIXT->m_aEnt =
     EntBuffer::pMoveToNextEntry(pDevIXT->m_aEnt,&pDevIXT->m_pEntBuffer);
  
  if (pDevIXT->m_pEntBuffer)
    pLogicUnit->m_xRluCommon.m_aEntArray =
      (Ent_int*)pDevIXT->m_pEntBuffer->aEntArray();
  else
    pLogicUnit->m_xRluCommon.m_aEntArray = nullptr;

  return (Ent_int*)pDevIXT->m_aEnt;
}

/*----------------------------------------------------------------------------*/
/* get address of payload                                                     */
/*----------------------------------------------------------------------------*/
i8* aDev_PAY_ADDRESS(void* aLU, void* aIXT)
{
  _static_
  
  DevIXT* pDevIXT = _pDevIXT(aIXT);

  i8* aPayload = pDevIXT->m_aEnt->aiPayCell(0);
    
  return aPayload;
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
/* set int only element                                                       */
/*----------------------------------------------------------------------------*/
void vDev_ENTWRK_SETINTELM(void* aLU, void* aRSB, i8 iTop)
{
  _static_
  
  //DevRSB* pDevRsb = _pDevRSB(aRSB);

  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  //pLogicUnit->m_oEntWrk.vAppendElementInt(pDevRsb->m_aRsb,iTop);
  pLogicUnit->m_oEntWrk.aEnt()->vKind(KE_INT);
  *(i8*)pLogicUnit->m_oEntWrk.aKey() = iTop;
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
/*  set rsb to null                                                           */
/*----------------------------------------------------------------------------*/
void vDev_RSB_NULL(void* aLU, void* aRSB)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aRSB);
  pDevRsb->m_pRowSlab = nullptr;
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
/* set the index name                                                         */
/*----------------------------------------------------------------------------*/
void vDev_RSB_TABLENM_SET(void* aLU, void* aRSB, Stz* szTableNm)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aRSB);
  pDevRsb->m_sTableNm = szTableNm;
}

/*----------------------------------------------------------------------------*/
/* push offset of source rsb                                                  */
/*----------------------------------------------------------------------------*/
i8 iDev_RSB_TESTROW(void* aLU, void* aRSB)
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
/* push offset of source rsb                                                  */
/*----------------------------------------------------------------------------*/
i8 iDev_RSB_TEST(void* aLU, void* aRSB)
{
  _static_
  
  DevRSB* pDevRsb = _pDevRSB(aRSB);
  
  i8 iRes;
  
  if (pDevRsb->m_pRowSlab)
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
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* push column value from m_aSrcCrw                                           */
/*----------------------------------------------------------------------------*/
void vDev_ALT_COL_PUSHSET(void* aLU, i8** aaiDstk, i8 ieStrInflate, i8* iaColSet)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
      
  RowInflator::vInflateSet(aaiDstk,
                                //pLogicUnit->m_pDevAltIxt->m_aCrw,
                                (Crw*)pLogicUnit->m_xRluCommon.m_aAltCrw,
                                pLogicUnit->m_pDevAltRsb->m_pStreamDef,
                                (eSI)ieStrInflate,iaColSet);
}

/*----------------------------------------------------------------------------*/
/* push column value from m_aSrcCrw                                           */
/*----------------------------------------------------------------------------*/
void vDev_RSB_COL_PUSHSET(void* aLU, i8** aaiDstk, i8 ieStrInflate, i8* iaColSet)
{
  _static_
  RASSERT(0); // check with ALka
  
  LogicUnit* pLogicUnit = (LogicUnit*)aLU; //_pLogicUnit(aLU);
    
  RowInflator::vInflateSet(aaiDstk,
                                pLogicUnit->m_pDevSrcIxt->m_aCrw,
                                pLogicUnit->m_pDevSrcRsb->m_pStreamDef,
                                (eSI)ieStrInflate,iaColSet);
}

/*----------------------------------------------------------------------------*/
/* push column value from m_aSrcCrw                                           */
/*----------------------------------------------------------------------------*/
void vDev_SRC_COL_PUSHSET(void* aLU, i8** aaiDstk, i8 ieStrInflate, i8* iaColSet)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  HeadLine*  aHeadLine  = (HeadLine*)pLogicUnit->m_xRluCommon.m_aSrcCrw;
  StreamDef* pStreamDef = pLogicUnit->m_pDevSrcRsb->m_pStreamDef;
  
  RowInflator::vInflateSet(aaiDstk,aHeadLine,iaColSet,ieStrInflate,
                           pStreamDef,nullptr);
  return;

  // --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - ---

  u1* auMetaClue;
  byte* ayDataByte;
  
  i8 nCols = pStreamDef->nTotalColumns();
  
  //vPrepare_v2(auMetaClue,ayDataByte,nCols,aHeadLine);
  auMetaClue = aHeadLine->auMetaArea();
  
  i8 i_BaseOffsetData = C_W_1B   // (ControlDate) 1 byte for the headline size (in words)
  + C_T_8B // (ControlDate) 8 bytes for the perspective
  + nCols; // metaSize
  
  ayDataByte = aAdd<byte*>(aHeadLine,i_BaseOffsetData);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form the set of columns to be retrieved
  
  //  i8 nWhichCols = 0;
  //  i8 iWhichCol[C_MAX_COLUMNS];
  //
  //  i8 i_ColToRetrieve;
  //  for (;;) {
  //
  //    i_ColToRetrieve = *((*aaiDstk)--);
  //
  //    if (-1 == i_ColToRetrieve)
  //      break;
  //
  //    // must be in ascending order
  //    ASSERT(0 == nWhichCols ||
  //           i_ColToRetrieve > iWhichCol[nWhichCols-1]);
  //
  //    iWhichCol[nWhichCols++] = i_ColToRetrieve;
  //  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //
  
  char* acStrArea = nullptr;
  
  i8* aiDstk_lcl = *aaiDstk;
  
  byte* ayDstByte = (byte*)++aiDstk_lcl;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each column
  
  i8 i_WhichCol = 0;
  i8 i_ColToRetrieve = iaColSet[i_WhichCol];
  
  i8 nSizeUnits;
  i8 nHeadBytes;
  i8 i_Over_rel;
  i8 nStrSize;
  
  i8* aiCellValue;
  u1 uMetaClue;
  u1 uHigh;
  u1 uLow;
  
  u2 u_Tail_rel;
  
  char* azDst;
  char** aacCellValue;
  
  Edt* aEdt;
  Dte* aDte;
  
  OverLine* aOverLine;
  TailLine* aTailLine;
  
  for (i8 i_Col = 0; /*none*/; i_Col++) {
    
    uMetaClue = *auMetaClue++;
    uHigh = uMetaClue >> 4;
    uLow  = uMetaClue & 0x0F;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // ignore column
    
    if (i_Col != i_ColToRetrieve) {
      //vSkip_v2(uHigh,uLow,ayDataByte,aHeadLine);
      
    L_Overflowed:
      
      if (uHigh <=  MN_STRING_x1) // numeric & date
        ayDataByte += uLow;
      else
        if (uHigh >=  MN_NEGATIVE)  // small value
        {;}
        else
          if (uHigh == MN_STRING_x1)  // string x 1
            ayDataByte += STRx1(uLow);
          else
            if (uHigh == MN_STRING_x2)  // string x 2
              ayDataByte += STRx2(uLow);
            else
              if (uHigh == MN_STRING_x4)  // string x 4
                ayDataByte += STRx4(uLow);
              else
                if (uHigh == MN_STRING_x8)  // string x 8
                  ayDataByte += STRx8(uLow);
                else
                  if (uHigh == MN_OVERFLOW) { // overflow
                    
                    //OverLine* aOverLine = aHeadLine->aGetOver();
                    
                    // calculation to get overflow
                    nSizeUnits = aHeadLine->w8_1B(C_HEADLINE_OFS_WORDS);
                    if (nSizeUnits >= 128) nSizeUnits -= 128;
                    nHeadBytes = nSizeUnits * C_BYTES_PER_WORD;
                    i_Over_rel = aHeadLine->w8_2B(nHeadBytes - C_2B);
                    
                    aOverLine = RowNub::aGetOver_rel(aHeadLine,i_Over_rel);
                    //OverLine* aOverLine = (OverLine*)aGetTail_rel(aHeadLine,i_Over_rel);
                    
                    uMetaClue = *((u1*)aOverLine + C_U_1B);
                    METACLUE_SPLIT(uMetaClue,uHigh,uLow);
                    ayDataByte = (byte*)aOverLine + C_U_1B + C_M_1B;
                    goto L_Overflowed;
                  }
                  else {
                    RASSERT(0);
                  }
      
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // inflate column
    
    //vTrace("Inflate %lld\n"_Stz,i_Col);
    
    //RASSERT(SI_ADDRESS == eStrInflate || SI_DEST_PST == eStrInflate);
    //vInflate_v2(ayDstByte,ayDataByte,aHeadLine,uHigh,uLow,eStrInflate,acStrArea);
    
    
    /////////////////////////////////////////////////////////////////////////////
    
  L_HighSwitch:
    
    aiCellValue = (i8*)ayDstByte;
    
    switch (uHigh) {
        
      case MN_NUMERIC: // numeric values 0..8B
        
        switch (uLow) {
            
          case 0x0: // MN_NUMERIC 0B
            *aiCellValue = 0;
            ayDstByte += C_8B;
            //            *ayDstByte++ = 0; // 0
            //            *ayDstByte++ = 0; // 1
            //            *ayDstByte++ = 0; // 2
            //            *ayDstByte++ = 0; // 3
            //            *ayDstByte++ = 0; // 4
            //            *ayDstByte++ = 0; // 5
            //            *ayDstByte++ = 0; // 6
            //            *ayDstByte++ = 0; // 7
            break;
            
          case 0x1: // MN_NUMERIC 1B
            *aiCellValue = 0;
            *ayDstByte++ = *ayDataByte++; // 0
            ayDstByte += C_7B;
            //            *ayDstByte++ = 0; // 1
            //            *ayDstByte++ = 0; // 2
            //            *ayDstByte++ = 0; // 3
            //            *ayDstByte++ = 0; // 4
            //            *ayDstByte++ = 0; // 5
            //            *ayDstByte++ = 0; // 6
            //            *ayDstByte++ = 0; // 7
            break;
            
          case 0x2: // MN_NUMERIC 2B
            *aiCellValue = *(u2*)(ayDataByte);
            ayDataByte += C_2B;
            ayDstByte += C_8B;
            //            *ayDstByte++ = *ayDataByte++; // 0
            //            *ayDstByte++ = *ayDataByte++; // 1
            //            *ayDstByte++ = 0; // 2
            //            *ayDstByte++ = 0; // 3
            //            *ayDstByte++ = 0; // 4
            //            *ayDstByte++ = 0; // 5
            //            *ayDstByte++ = 0; // 6
            //            *ayDstByte++ = 0; // 7
            break;
            
          case 0x3: // MN_NUMERIC 3B
            *aiCellValue = 0;
            *ayDstByte++ = *ayDataByte++; // 0
            *ayDstByte++ = *ayDataByte++; // 1
            *ayDstByte++ = *ayDataByte++; // 2
            ayDstByte += C_5B;
            //            *ayDstByte++ = 0; // 3
            //            *ayDstByte++ = 0; // 4
            //            *ayDstByte++ = 0; // 5
            //            *ayDstByte++ = 0; // 6
            //            *ayDstByte++ = 0; // 7
            break;
            
          case 0x4: // MN_NUMERIC 4B
            *aiCellValue = *(u4*)(ayDataByte);
            ayDataByte += C_4B;
            ayDstByte += C_8B;
            //            *ayDstByte++ = *ayDataByte++; // 0
            //            *ayDstByte++ = *ayDataByte++; // 1
            //            *ayDstByte++ = *ayDataByte++; // 2
            //            *ayDstByte++ = *ayDataByte++; // 3
            //            *ayDstByte++ = 0; // 4
            //            *ayDstByte++ = 0; // 5
            //            *ayDstByte++ = 0; // 6
            //            *ayDstByte++ = 0; // 7
            break;
            
          case 0x5: // MN_NUMERIC 5B
            *aiCellValue = 0;
            *ayDstByte++ = *ayDataByte++; // 0
            *ayDstByte++ = *ayDataByte++; // 1
            *ayDstByte++ = *ayDataByte++; // 2
            *ayDstByte++ = *ayDataByte++; // 3
            *ayDstByte++ = *ayDataByte++; // 4
            ayDstByte += C_3B;
            //            *ayDstByte++ = 0; // 5
            //            *ayDstByte++ = 0; // 6
            //            *ayDstByte++ = 0; // 7
            break;
            
          case 0x6: // MN_NUMERIC 6B
            *aiCellValue = 0;
            *ayDstByte++ = *ayDataByte++; // 0
            *ayDstByte++ = *ayDataByte++; // 1
            *ayDstByte++ = *ayDataByte++; // 2
            *ayDstByte++ = *ayDataByte++; // 3
            *ayDstByte++ = *ayDataByte++; // 4
            *ayDstByte++ = *ayDataByte++; // 5
            ayDstByte += C_2B;
            //            *ayDstByte++ = 0; // 6
            //            *ayDstByte++ = 0; // 7
            break;
            
          case 0x7: // MN_NUMERIC 7B
            //*aiCellValue = 0;
            *ayDstByte++ = *ayDataByte++; // 0
            *ayDstByte++ = *ayDataByte++; // 1
            *ayDstByte++ = *ayDataByte++; // 2
            *ayDstByte++ = *ayDataByte++; // 3
            *ayDstByte++ = *ayDataByte++; // 4
            *ayDstByte++ = *ayDataByte++; // 5
            *ayDstByte++ = *ayDataByte++; // 6
            *ayDstByte++ = 0; // 7
            break;
            
          case 0x8: // MN_NUMERIC 8B
            *aiCellValue = *(u8*)(ayDataByte);
            //            *ayDstByte++ = *ayDataByte++; // 0
            //            *ayDstByte++ = *ayDataByte++; // 1
            //            *ayDstByte++ = *ayDataByte++; // 2
            //            *ayDstByte++ = *ayDataByte++; // 3
            //            *ayDstByte++ = *ayDataByte++; // 4
            //            *ayDstByte++ = *ayDataByte++; // 5
            //            *ayDstByte++ = *ayDataByte++; // 6
            //            *ayDstByte++ = *ayDataByte++; // 7
            ayDstByte += C_8B;
            ayDataByte += C_8B;
            break;
            
          case 0x9: // MN_NUMERIC 9B
            RASSERT(0);
            break;
            
          case 0xA: // MN_NUMERIC 10B
            RASSERT(0);
            break;
            
          case 0xB: // MN_NUMERIC 11B
            RASSERT(0);
            break;
            
          case 0xC: // MN_NUMERIC 12B
            RASSERT(0);
            break;
            
          case 0xD: // MN_NUMERIC 13B
            RASSERT(0);
            break;
            
          case 0xE: // MN_NUMERIC 14B
            RASSERT(0);
            break;
            
          case 0xF: // MN_NUMERIC 15B
            RASSERT(0);
            break;
            
        }
        break;
        
      case MN_DATE_E0: // date era-0 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
        *aiCellValue = 0;
        *ayDstByte++ = *ayDataByte++; // 0
        *ayDstByte++ = *ayDataByte++; // 1
        ayDstByte += 6;
        //        *ayDstByte++ = 0; // 2
        //        *ayDstByte++ = 0; // 3
        //        *ayDstByte++ = 0; // 4
        //        *ayDstByte++ = 0; // 5
        //        *ayDstByte++ = 0; // 6
        //        *ayDstByte++ = 0; // 7
        
        //vDteFromEdt(aiCellValue,C_ERA_0);
        aEdt = (Edt*)aiCellValue;
        aDte = (Dte*)aiCellValue;
        
        aDte->m_uYearDelta = aEdt->m_uYearInEra + C_ERA_0;
        aDte->m_uMonth     = aEdt->m_uMonth;
        aDte->m_uDay       = aEdt->m_uDay;
        break;
        
      case MN_DATE_E1: // date era-1 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
        *aiCellValue = 0;
        *ayDstByte++ = *ayDataByte++; // 0
        *ayDstByte++ = *ayDataByte++; // 1
        ayDstByte += 6;
        //        *ayDstByte++ = 0; // 2
        //        *ayDstByte++ = 0; // 3
        //        *ayDstByte++ = 0; // 4
        //        *ayDstByte++ = 0; // 5
        //        *ayDstByte++ = 0; // 6
        //        *ayDstByte++ = 0; // 7
        
        //vDteFromEdt(aiCellValue,C_ERA_1);
        aEdt = (Edt*)aiCellValue;
        aDte = (Dte*)aiCellValue;
        
        aDte->m_uYearDelta = aEdt->m_uYearInEra + C_ERA_1;
        aDte->m_uMonth     = aEdt->m_uMonth;
        aDte->m_uDay       = aEdt->m_uDay;
        break;
        
      case MN_DATE_E2: // date era-2 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
        *aiCellValue = 0;
        *ayDstByte++ = *ayDataByte++; // 0
        *ayDstByte++ = *ayDataByte++; // 1
        ayDstByte += 6;
        //        *ayDstByte++ = 0; // 2
        //        *ayDstByte++ = 0; // 3
        //        *ayDstByte++ = 0; // 4
        //        *ayDstByte++ = 0; // 5
        //        *ayDstByte++ = 0; // 6
        //        *ayDstByte++ = 0; // 7
        
        //vDteFromEdt(aiCellValue,C_ERA_2);
        aEdt = (Edt*)aiCellValue;
        aDte = (Dte*)aiCellValue;
        
        aDte->m_uYearDelta = aEdt->m_uYearInEra + C_ERA_2;
        aDte->m_uMonth     = aEdt->m_uMonth;
        aDte->m_uDay       = aEdt->m_uDay;
        break;
        
      case MN_STRING_x1: // short string 0..15B (no null) - -- -- -- -- -- -- --
        nStrSize = STRx1(uLow);
        
        if (SI_ADDRESS == (eSI)ieStrInflate) {
          aacCellValue = (char**)ayDstByte;
          *aacCellValue = (char*)ayDataByte;
          ayDstByte += 8;
          
          ayDataByte += nStrSize;
        }
        else
          if (SI_AREA_ADR == (eSI)ieStrInflate) {
            memcpy(acStrArea,ayDataByte,(size_t)nStrSize);
            ayDataByte += nStrSize;
            
            aacCellValue = (char**)ayDstByte;
            *aacCellValue = acStrArea;
            ayDstByte += 8;
            acStrArea += nStrSize;
          }
          else
            if (SI_DEST_PST == (eSI)ieStrInflate) {
              memcpy(ayDstByte,ayDataByte,(size_t)nStrSize);
              ayDataByte += nStrSize;
              
              //vTrace("SI_DEST_PST : %p %s\n"_Stz,rayDstByte,rayDstByte);
              
              ayDstByte += (((nStrSize) / 8) * 8) + 16;
              aiCellValue = (i8*)ayDstByte;
              *aiCellValue = nStrSize - 1;
              ayDstByte += 8;
            }
            else {
              RASSERT(0);
            }
        
        break;
        
      case MN_NULL: // null - -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
        *(i8*)ayDstByte = C_DATUM_NULL;
        ayDstByte += C_BYTES_PER_WORD;
        break;
        
      case MN_SPARE0: // spare
        RASSERT(0);
        break;
        
      case MN_OVERFLOW: // overflow 2B
        
        // calculation to get overflow
        //aOverLine = aHeadLine->aGetOver();
        nSizeUnits = aHeadLine->w8_1B(C_HEADLINE_OFS_WORDS);
        if (nSizeUnits >= 128) nSizeUnits -= 128;
        nHeadBytes = nSizeUnits * C_BYTES_PER_WORD;
        i_Over_rel = aHeadLine->w8_2B(nHeadBytes - C_2B);
        
        aOverLine = RowNub::aGetOver_rel(aHeadLine,i_Over_rel);
        
        METACLUE_SPLIT( (* aAdd<u1*>(aOverLine,C_U_1B) ), uHigh, uLow);
        ayDataByte = aAdd<byte*>(aOverLine,C_U_1B + C_M_1B);
        goto L_HighSwitch;
        
        break;
        
        
        // end where uLow is n-bytes
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // toBytes(uLow) depends on uHigh
        
      case MN_STRING_x2: // string x 2 -- -- -- -- -- -- -- -- -- -- -- -- -- --
        nStrSize = STRx2(uLow);
        
        if (SI_ADDRESS == (eSI)ieStrInflate) {
          aacCellValue = (char**)ayDstByte;
          *aacCellValue = (char*)ayDataByte;
          ayDstByte += 8;
          
          ayDataByte += nStrSize;
        }
        else
          if (SI_AREA_ADR == (eSI)ieStrInflate) {
            
            memcpy(acStrArea,ayDataByte,(size_t)nStrSize);
            ayDataByte += nStrSize;
            
            aacCellValue = (char**)ayDstByte;
            *aacCellValue = acStrArea;
            ayDstByte += 8;
            acStrArea += nStrSize;
          }
          else
            if (SI_DEST_PST == (eSI)ieStrInflate) {
              azDst = (char*)ayDstByte;
              memcpy(azDst,ayDataByte,(size_t)nStrSize);
              ayDataByte += nStrSize;
              
              //vTrace("SI_DEST_PST : %p %s\n"_Stz,rayDstByte,rayDstByte);
              
              ayDstByte += (((nStrSize) / 8) * 8) + 8;
              aiCellValue = (i8*)ayDstByte;
              *aiCellValue = nStrSize - 1;
              ayDstByte += 8;
            }
            else {
              RASSERT(0);
            }
        
        break;
        
      case MN_STRING_x4: // string x 4 -- -- -- -- -- -- -- -- -- -- -- -- -- --
        nStrSize = STRx4(uLow);
        
        if (SI_ADDRESS == (eSI)ieStrInflate) {
          aacCellValue = (char**)ayDataByte;
          *aacCellValue = (char*)ayDataByte;
          ayDstByte += 8;
          
          ayDataByte += nStrSize;
        }
        else
          if (SI_AREA_ADR == (eSI)ieStrInflate) {
            
            memcpy(acStrArea,ayDataByte,(size_t)nStrSize);
            ayDataByte += nStrSize;
            
            aacCellValue = (char**)ayDstByte;
            *aacCellValue = acStrArea;
            ayDstByte += 8;
            acStrArea += nStrSize;
          }
          else
            if (SI_DEST_PST == (eSI)ieStrInflate) {
              memcpy(ayDstByte,ayDataByte,(size_t)nStrSize);
              ayDataByte += nStrSize;
              
              //vTrace("SI_DEST_PST : %p %s\n"_Stz,rayDstByte,rayDstByte);
              
              ayDstByte += (((nStrSize) / 8) * 8) + 8;
              aiCellValue = (i8*)ayDstByte;
              *aiCellValue = nStrSize - 1;
              ayDstByte += 8;
            }
            else {
              RASSERT(0);
            }
        
        break;
        
      case MN_STRING_x8: // string x 8 -- -- -- -- -- -- -- -- -- -- -- -- -- --
        nStrSize = STRx8(uLow);
        
        if (SI_ADDRESS == (eSI)ieStrInflate) {
          aacCellValue = (char**)ayDstByte;
          *aacCellValue = (char*)ayDataByte;
          ayDstByte += 8;
          
          ayDataByte += nStrSize;
        }
        else
          if (SI_AREA_ADR == (eSI)ieStrInflate) {
            
            memcpy(acStrArea,ayDataByte,(size_t)nStrSize);
            ayDataByte += nStrSize;
            
            aacCellValue = (char**)ayDstByte;
            *aacCellValue = acStrArea;
            ayDstByte += 8;
            acStrArea += nStrSize;
          }
          else
            if (SI_DEST_PST == (eSI)ieStrInflate) {
              memcpy(ayDstByte,ayDataByte,(size_t)nStrSize);
              ayDataByte += nStrSize;
              
              //vTrace("SI_DEST_PST : %p %s\n"_Stz,rayDstByte,rayDstByte);
              
              ayDstByte += (((nStrSize) / 8) * 8) + 8;
              aiCellValue = (i8*)ayDstByte;
              *aiCellValue = nStrSize - 1;
              ayDstByte += 8;
            }
            else {
              RASSERT(0);
            }
        
        break;
        
      case MN_STRING_TAIL: // string in tail -- -- -- -- -- -- -- -- -- -- -- --
        u_Tail_rel = *(u2*)ayDataByte;
        aTailLine = aHeadLine->aGetTailRel(u_Tail_rel);
        
        nStrSize = aTailLine->w8_1B(+ C_U_1B);
        if (SI_AREA_ADR == (eSI)ieStrInflate) {
          
          memcpy(acStrArea,( (char*)aTailLine + C_U_1B + C_L_1B ),(size_t)nStrSize);
          
          aacCellValue = (char**)ayDstByte;
          *aacCellValue = acStrArea;
          ayDstByte += 8;
          ayDataByte += 2;
          acStrArea += nStrSize;
        }
        else {
          RASSERT(0);
        }
        
        break;
        
      case MN_BLOB_0: // blob type 0 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
        RASSERT(0);
        break;
        
      case MN_BLOB_1: // blob type 1 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
        RASSERT(0);
        break;
        
      case MN_NEGATIVE: // -ve digit -15..-0 -- -- -- -- -- -- -- -- -- -- -- --
        aiCellValue = (i8*)ayDstByte;
        
        *ayDstByte++ = (i8)uLow; // 0
        *ayDstByte++ = 0; // 1
        *ayDstByte++ = 0; // 2
        *ayDstByte++ = 0; // 3
        *ayDstByte++ = 0; // 4
        *ayDstByte++ = 0; // 5
        *ayDstByte++ = 0; // 6
        *ayDstByte++ = 0; // 7
        
        *aiCellValue = - *aiCellValue;
        
        break;
        
      case MN_POSITIVE: // +ve digit 0..15 - -- -- -- -- -- -- -- -- -- -- -- --
        *ayDstByte++ = (i8)uLow; // 0
        *ayDstByte++ = 0; // 1
        *ayDstByte++ = 0; // 2
        *ayDstByte++ = 0; // 3
        *ayDstByte++ = 0; // 4
        *ayDstByte++ = 0; // 5
        *ayDstByte++ = 0; // 6
        *ayDstByte++ = 0; // 7
        break;
        
      default:
        NEVER_REACH;
    }
    ///////////////////////////////////////////////////////////////////////////////
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // move to next column to retrieve
    
    //    if (++i_WhichCol >= nWhichCols)
    //      break;
    
    i_ColToRetrieve = iaColSet[++i_WhichCol];
    if (-1 == i_ColToRetrieve)
      break;
  }
  
  // end for each column
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // update callers aiDstk
  
  aiDstk_lcl = (i8*)ayDstByte;
  
  *aaiDstk = --aiDstk_lcl;
}

/*----------------------------------------------------------------------------*/
/* set the indexTree[] in the dev                                             */
/*----------------------------------------------------------------------------*/
void vDev_INDEXTREE_DEV_SET(void* aLU, i8 i_Dev)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  //pLogicUnit->m_aaDev[i_Dev] = (void*)(* pLogicUnit->m_xpIndexTree);
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

/*----------------------------------------------------------------------------*/
/* use stream def out for this pot                                            */
/*----------------------------------------------------------------------------*/
void vDev_POT_USE_STREAMDEFOUT(void* aLU, i8 iAddPtnHsh)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  
  pLogicUnit->m_xpStreamDefOut->vToStreamPix(&pLogicUnit->m_oStreamPix,
                                               (bool)iAddPtnHsh);
  
  pLogicUnit->m_xpPot->vStreamPix(&pLogicUnit->m_oStreamPix);
  
}

/*----------------------------------------------------------------------------*/
/* move StreamDef[] to  streamDefout[]                                        */
/*----------------------------------------------------------------------------*/
void vDev_REG_STREAMDEF_STREAMDEFOUT(void* aLU)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  
  pLogicUnit->m_xpStreamDefOut = pLogicUnit->m_xpStreamDef;
}

/*----------------------------------------------------------------------------*/
/* new an EOSMARKER in OperandA                                               */
/*----------------------------------------------------------------------------*/
void vDev_REG_EOSMARKER_NEW(void* aLU)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  
  pLogicUnit->m_xpOperandA = new Marker(OM_EOSMARKER);
}

/*----------------------------------------------------------------------------*/
/* add a column to indexDef[]                                                 */
/*----------------------------------------------------------------------------*/
void vDev_REG_INDEXDEF_ADDCOLUMN(void* aLU, Stz* sColumnNm, i8 iColumnType)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  
  eCT eColType = (eCT)iColumnType;
  
  IndexDef* pIndexDef = * pLogicUnit->m_xpIndexDef;
  
  pIndexDef->vAddColumn(sColumnNm,eColType);
}

/*----------------------------------------------------------------------------*/
/* delete indexDef[]                                                          */
/*----------------------------------------------------------------------------*/
void vDev_REG_INDEXDEF_DELETE(void* aLU)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  
  IndexDef* pIndexDef = * pLogicUnit->m_xpIndexDef;
  
  pLogicUnit->m_xpIndexDef = nullptr;
  
  delete pIndexDef;
}

/*----------------------------------------------------------------------------*/
/* new indexDef[]                                                             */
/*----------------------------------------------------------------------------*/
void vDev_REG_INDEXDEF_NEW(void* aLU, Stz* sName)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  
  pLogicUnit->m_xpIndexDef = new IndexDef;
  pLogicUnit->m_xpIndexDef->vIndexNm(sName);
}

/*----------------------------------------------------------------------------*/
/* new indexTree[]                                                            */
/*----------------------------------------------------------------------------*/
void vDev_REG_INDEXTREE_NEW(void* aLU)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  
  pLogicUnit->m_xpIndexTree = new IndexTree(nullptr, *pLogicUnit->m_xpIndexDef, C_UNKNOWN);
}

/*----------------------------------------------------------------------------*/
/* new a streamDefout[]                                                       */
/*----------------------------------------------------------------------------*/
void vDev_REG_STREAMDEFOUT_NEW(void* aLU)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  
  pLogicUnit->m_xpStreamDefOut = new StreamDef;
  (* pLogicUnit->m_xpStreamDefOut)->vAddDefSet();
}

/*----------------------------------------------------------------------------*/
/* add a column to streamDefOut[]                                             */
/*----------------------------------------------------------------------------*/
void vDev_REG_STREAMDEFOUT_ADDCOLUMN(void* aLU, Stz* sColumnNm, i8 iColumnType,
                                     i8 iColumnLength, i8 iColumnScale)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  
  StreamDef* pStreamDefOut = * pLogicUnit->m_xpStreamDefOut;
  
  pStreamDefOut->vAddColumn(sColumnNm,(eCT)iColumnType,iColumnLength,iColumnScale);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
