/*============================================================================*/
/* DEV_interface.cpp -- RLU Dev Interface                                     */
/*============================================================================*/

#include "BASE.h"

#include "IndexDef.hpp"
#include "Marker.hpp"

#include "RowBuffer.hpp"
#include "EachRowBuffer.hpp"

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "TailLine.hpp"

#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "RowInflator.hpp"

#include "DEV_interface.h"

#include "SkipRoots.hpp"

#include "EntBuffer.hpp"
#include "IndexTree.hpp"

#include "Crv.hpp"
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "RowOnce.hpp"
#include "IndexStream.hpp"
#include "PtnStream.hpp"
#include "SP_Ent.hpp"

#include "Node_I.hpp"
#include "LogicUnit.hpp"

extern void* C_ADR_REGION;

/*----------------------------------------------------------------------------*/
/* begin row traverse                                                         */
/*----------------------------------------------------------------------------*/
void vDev_IXT_CRWBUF_BEGIN(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  //pop the device
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());

  pDevIxt->m_i_Row = -1;
  pDevIxt->m_aCrw = nullptr;
  pDevIxt->m_nRows = pDevIxt->m_pCrwBuffer ?
                     pDevIxt->m_pCrwBuffer->nRows() : 0;

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* find crw given row no                                                      */
/*----------------------------------------------------------------------------*/
void vDev_IXT_CRWBUF_FINDCRW(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  //pop the device
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());

  //pop the row no
  i8 i_Row = I_PopInt();

  pDevIxt->m_i_Row = i_Row;
  pDevIxt->m_aCrw =
    (Crw*) ( (char*)pDevIxt->m_pCrwBuffer->aGetHead(0)
           + (C_CRW_HEADBYTES * pDevIxt->m_i_Row) );

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* move to next src row                                                       */
/*----------------------------------------------------------------------------*/
void vDev_IXT_CRWBUF_NEXT(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  //pop the device
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());

  if (++pDevIxt->m_i_Row < pDevIxt->m_nRows) {
    pDevIxt->m_aCrw =
        (Crw*) ( (char*)pDevIxt->m_pCrwBuffer->aGetHead(0)
                 + (C_CRW_HEADBYTES * pDevIxt->m_i_Row) );
    I_PushInt(1);
  }
  else {
    I_PushInt(0);
  }

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/*  null the crw buffer                                                       */
/*----------------------------------------------------------------------------*/
void vDev_IXT_CRWBUF_NULL(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  //pop the device
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());

  pDevIxt->m_pCrwBuffer = nullptr;

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/*  get current row cnt                                                       */
/*----------------------------------------------------------------------------*/
void vDev_IXT_CRWBUF_ROWCNT(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  //pop the device
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());

  I_PushInt(pDevIxt->m_nRows);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/*  get current row no                                                        */
/*----------------------------------------------------------------------------*/
void vDev_IXT_CRWBUF_ROWNO(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  //pop the device
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());

  I_PushInt(pDevIxt->m_i_Row);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/*  test if there is a crw src                                                */
/*----------------------------------------------------------------------------*/
void vDev_IXT_CRWBUF_TEST(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  //pop the device
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());

  if (pDevIxt->m_pCrwBuffer)
    I_PushInt(1)
  else
    I_PushInt(0)

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                dev IXT                                     */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compare device m_pEnt.key to entry key                                     */
/*----------------------------------------------------------------------------*/
void vDev_IXT_ENT_COMPARE(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  // pop the entry
  Ent* pEnt = (Ent*)P_PopPtr();

  //pop the device
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());

  Key* aEntKey = pDevIxt->m_aEnt->aKey();
  Key* aKey = pEnt->aKey();
  
  if (KE_GENERAL == pDevIxt->m_aEnt->eKind()) {
    eKC eCmp = Key::eCompare(aEntKey,aKey);
    I_PushInt((i8)eCmp);
  }
  else {
    i8 iEntVal = *(i8 *) aEntKey;
    i8 iWrkVal = *(i8 *) aKey;

    eKC eCmp;
    if (iEntVal < iWrkVal)
      eCmp = KC_BEFORE;
    else if (iEntVal == iWrkVal)
      eCmp = KC_SAME;
    else
      eCmp = KC_AFTER;

    I_PushInt((i8) eCmp);
  }

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* fetch Crw using m_aEnt.ref                                                 */
/*----------------------------------------------------------------------------*/
void vDev_IXT_ENT_DEREF(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  //pop the device
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());

  i8 iBuffer = ((Ent_int*)pDevIxt->m_aEnt)->Rrf.m_uBuffer;
  i8 iItem   = ((Ent_int*)pDevIxt->m_aEnt)->Rrf.m_uItem;

  pDevIxt->m_aCrw  = (Crw*)
      ( (char*)C_ADR_REGION
        + iBuffer * C_1MB
        + C_CRW_FIRSTHEAD
        + iItem * C_CRW_HEADBYTES );  //alka ? C_CRW_HEADBYTES

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* position m_pEnt prior using m_oEntWrk.key                                  */
/*----------------------------------------------------------------------------*/
void vDev_IXT_ENT_FIND(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  Ent_int* aEnt_int;
  tET tNext;
  i8 iTopA;
  i8 iTopB;
  void* aTempSkipRoots = nullptr;
  Ent* pEntArray;

  // pop the wrk ent
  Ent_int* aWrkEnt = (Ent_int*)P_PopPtr();

  //pop the device
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());

  if (pDevIxt->m_pEntBuffer)
    aTempSkipRoots = (Ent_int*)pDevIxt->m_pEntBuffer->aSkipRoots();

  // can we do a linear search
  if (! aTempSkipRoots) {
    *(++aiDstk) = 0;
    goto L_end_FINDI;
  }
  aEnt_int = (Ent_int*)aTempSkipRoots;
  tNext = aEnt_int->m_taNextEntry[0];
//        if (C_ENT_END_TICKET == tNext) {
//          *(++aiDstk) = 0;
//          goto L_end_FINDI;
//        }

  iTopA = aWrkEnt->m_iIntElm0;
  // search for key
  pEntArray = pDevIxt->m_pEntBuffer->aEntArray();

  while (C_ENT_END_TICKET != tNext) {
    //aEnt_int = (struct Ent_int*) &aRluCommon->m_aEntArray[tNext];
    aEnt_int = (struct Ent_int*) &pEntArray[tNext];
    iTopB = aEnt_int->m_iIntElm0;
    //printf("Ticket %lld Ent %p\n",(i8)tNext,aEnt_int);
    if (iTopA == iTopB) {
      //aRluCommon->m_aTmpEnt = aEnt_int;
      pDevIxt->m_aEnt = (Ent*)aEnt_int;
      *(++aiDstk) = 1; // found matching key
      goto L_end_FINDI;
    }
    if (iTopA > iTopB) {
      tNext = aEnt_int->m_taNextEntry[0];
      continue;
    }
    //printf("Did not find %lld\n",aRluCommon->m_aWrkEnt->m_iIntElm0);
    *(++aiDstk) = 0; // did not find matching key
    goto L_end_FINDI;
  }
  // did not find matching key
  *(++aiDstk) = 0;

  L_end_FINDI:

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* move to next m_pEnt                                                        */
/*----------------------------------------------------------------------------*/
void vDev_IXT_ENT_NEXT(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  //pop the device
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());
   
  pDevIxt->m_aEnt = EntBuffer::pMoveToNextEntry(pDevIxt->m_aEnt,
                                                &pDevIxt->m_pEntBuffer);
  P_PushPtr(pDevIxt->m_aEnt);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* position m_pEnt prior using m_oEntWrk.key                                  */
/*----------------------------------------------------------------------------*/
void vDev_IXT_ENT_POSPRIOR(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;
  Ent* pEntArray;

  // pop the wrk ent
  Ent* aWrkEnt = (Ent*)P_PopPtr();

  //pop the device
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());

  Ent *pEnt = (Ent*)aWrkEnt;
  pDevIxt->m_aEnt =
    pDevIxt->m_pIdxTree->aPositionPriorEntry(pEnt->aKey(),
                                             &pDevIxt->m_pEntBuffer);
  
  P_PushPtr(pDevIxt->m_aEnt);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* print entry key of device and other entry working key                      */
/*----------------------------------------------------------------------------*/
void vDev_IXT_ENT_PRINTKEYS(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;
  Ent* pEntArray;

  // pop the entry
  Ent* pEnt = (Ent*)P_PopPtr();

  //pop the device
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());

  SP_Ent spEnt;
  vTrace("\n\nEntWrk : %s\n"_Stz,
         spEnt.sz(pEnt,
                  pDevIxt->m_pIdxTree->pIndexDef()));
  
  vTrace("\n\nDevEnt : %s\n"_Stz,
         spEnt.sz(pDevIxt->m_aEnt,
                  pDevIxt->m_pIdxTree->pIndexDef()));
}

/*----------------------------------------------------------------------------*/
/* push entry                                                                 */
/*----------------------------------------------------------------------------*/
void vDev_IXT_ENT_PUSH(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  //pop the device
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());
  
  P_PushPtr(pDevIxt->m_aEnt);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* push entry elm 0 int                                                       */
/*----------------------------------------------------------------------------*/
void vDev_IXT_ENT_PUSHELMI0(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  //pop the device
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());
  
  i8 iRes = pDevIxt->m_aEnt->iIntElm0();

  I_PushInt(iRes);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* push entry elm 0 str                                                       */
/*----------------------------------------------------------------------------*/
void vDev_IXT_ENT_PUSHELMS0(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;
  char caStrWrk[128];
  i8 nSize;
  char* azDst;

  //pop the device
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());
  
  Stz* szSrc = pDevIxt->m_aEnt->aKey()->szGetStrElm();

  StrLib::iWordFlipBack(caStrWrk,(char*)szSrc);

  S_PushStr(caStrWrk);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* push entry                                                                 */
/*----------------------------------------------------------------------------*/
void vDev_IXT_ENT_PUSHUPPER(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  //pop the device
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());

  P_PushPtr(pDevIxt->m_aUpperEnt);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set the index name                                                         */
/*----------------------------------------------------------------------------*/
void vDev_IXT_INDEXNM_SET(i8** aaiDstk)
{
  _static_

  // ALKA: Double string copy
  // come back to this !!!

  i8 *aiDstk = *aaiDstk;
  i8 nSize;
  char* azSrc;
  char azDst[256];

  // pop index name into the device
  S_PopStr(azDst);

  // pop device
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());
  StrLib::vCopy(pDevIxt->m_sIndexNm.azChar(), azDst);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* compare keys of two devices                                                */
/*----------------------------------------------------------------------------*/
void vDev_IXT_CMP_KEYS(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  // pop the second Ent
  Ent* pEntSecond = (Ent*)(P_PopPtr());

  // pop the first Ent
  Ent* pEntFirst = (Ent*)(P_PopPtr());

  // get the entries from the devices
  Key* aFirstKey = pEntFirst->aKey();
  Key* aSecondKey = pEntSecond->aKey();

  if (KE_GENERAL == pEntFirst->eKind()) {
    eKC  eCmp = Key::eCompare(aFirstKey,aSecondKey);
  
    if (KC_BEFORE == eCmp)
     I_PushInt(-1)
    else
    if (KC_SAME == eCmp)
      I_PushInt(0)
    else
      I_PushInt(1)
  }
  else {

    i8 iRes = ((Ent_int *) (pEntFirst))->m_iIntElm0 ==
              ((Ent_int *) (pEntSecond))->m_iIntElm0;

    I_PushInt(iRes);
  }
  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* delete current entry m_aEnt from indexTree                                 */
/*----------------------------------------------------------------------------*/
void vDev_IXT_IDX_DEL(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  DevIXT* pDevIXT = _pDevIXT(P_PopPtr());

  pDevIXT->m_pIdxTree->vDelete(pDevIXT->m_pEntBuffer,pDevIXT->m_aEnt);
  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* dump index tree                                                            */
/*----------------------------------------------------------------------------*/
void vDev_IXT_IDX_DUMP(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;
  DevIXT* pDevIXT = _pDevIXT(P_PopPtr());

  pDevIXT->m_pIdxTree->vDump(false);
  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* print ent                                                                  */
/*----------------------------------------------------------------------------*/
void vDev_IXT_ENT_PRINT(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;
  DevIXT* pDevIXT = _pDevIXT(P_PopPtr());
  
  SP_Ent spEnt;
  vTrace("Ent : %s\n"_Stz,
         spEnt.sz(pDevIXT->m_aEnt,
                  pDevIXT->m_pIdxTree->pIndexDef()));
  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* insert an entry into the indexTree                                         */
/*----------------------------------------------------------------------------*/
void vDev_IXT_IDX_INS(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  // pop entry
  Ent *pEnt = (Ent*)P_PopPtr();

  DevIXT* pDevIXT = _pDevIXT(P_PopPtr());

  pDevIXT->m_aEnt =
      pDevIXT->m_pIdxTree->aInsertEntry(pEnt);

  //P_PushPtr(pDevIXT->m_aEnt);
  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* delete the indexTree from the dev                                          */
/*----------------------------------------------------------------------------*/
void vDev_IXT_INDEXTREE_DELETE(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());

  //IndexTree* pIndexTree = pDevIxt->m_pIdxTree;
  //pDevIxt->m_pIdxTree = nullptr;
  //delete pIndexTree;

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* set the indexTree[] in the dev                                             */
/*----------------------------------------------------------------------------*/
void vDev_IXT_INDEXTREE_SET(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  // pop the device
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());

  // pop the indexTree
  IndexTree* pIndexTree = _pIndexTree(P_PopPtr());

  // set the indexTree in the device
  pDevIxt->m_pIdxTree = pIndexTree;

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/*  position alt ixt using src.key                                            */
/*----------------------------------------------------------------------------*/
void vDev_IXT_POS_BYKEY(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  // pop the entry
  Ent* aEnt = (Ent*)P_PopPtr();
  Key* aKey = aEnt->aKey();

  // pop the device
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());

  pDevIxt->m_aEnt =
      pDevIxt->m_pIdxTree->aPositionPriorEntry(aKey,
                                       &pDevIxt->m_pEntBuffer);
  
  if (pDevIxt->m_pEntBuffer) {
    P_PushPtr(pDevIxt->m_aEnt);
  }
  else {
    P_PushPtr(nullptr);
  }

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* insert an entry into the indexTree                                         */
/*----------------------------------------------------------------------------*/
void vDev_IXT_POS_LBOUND_GE(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  // pop the wrkent
  EntWrk* pEntWrk = (EntWrk*)(P_PopPtr());

  // pop the device
  DevIXT* pDevIXT = _pDevIXT(P_PopPtr());

  Ent *pEnt = (Ent*)pEntWrk->aEnt();

  pDevIXT->m_aEnt = nullptr;
  
  pDevIXT->m_pEntBuffer =
    pDevIXT->m_pIdxTree->pPositionH0Buffer(pEnt->aKey());
  
  if (pDevIXT->m_pEntBuffer) {
    pDevIXT->m_aEnt =
        pDevIXT->m_pEntBuffer->aFindEntryBefore(pEnt->aKey());

    if (pDevIXT->m_aEnt) {
      pDevIXT->m_aEnt =
          EntBuffer::pMoveToGreaterEqualEntry(pDevIXT->m_aEnt,
                                              pEnt->aKey(),
                                              &pDevIXT->m_pEntBuffer);
    }
  }

  P_PushPtr(pDevIXT->m_aEnt);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* insert an entry into the indexTree                                         */
/*----------------------------------------------------------------------------*/
void vDev_IXT_POS_LBOUND_GE_OLD(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  // pop the wrkent
  EntWrk* pEntWrk = (EntWrk*)(P_PopPtr());

  // pop the device
  DevIXT* pDevIXT = _pDevIXT(P_PopPtr());

  Ent *pEnt = (Ent*)pEntWrk->aEnt();

  pDevIXT->m_aEnt = nullptr;

  pDevIXT->m_pEntBuffer =
      pDevIXT->m_pIdxTree->pPositionH0Buffer(pEnt->aKey());

  if (pDevIXT->m_pEntBuffer) {
    pDevIXT->m_aEnt =
        pDevIXT->m_pEntBuffer->aFindEntryBefore(pEnt->aKey());
  }

  P_PushPtr(pDevIXT->m_aEnt);

  *aaiDstk = aiDstk;
}


/*----------------------------------------------------------------------------*/
/* insert an entry into the indexTree                                         */
/*----------------------------------------------------------------------------*/
void vDev_IXT_POS_LBOUND_GT(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  // pop the wrkent
  EntWrk* pEntWrk = (EntWrk*)(P_PopPtr());

  // pop the device
  DevIXT* pDevIXT = _pDevIXT(P_PopPtr());

  Ent *pEnt = (Ent*)pEntWrk->aEnt();

  pDevIXT->m_aEnt = nullptr;

  pDevIXT->m_pEntBuffer =
      pDevIXT->m_pIdxTree->pPositionH0Buffer(pEnt->aKey());

  if (pDevIXT->m_pEntBuffer) {
    pDevIXT->m_aEnt =
        pDevIXT->m_pEntBuffer->aFindEntryBefore(pEnt->aKey());

    if (pDevIXT->m_aEnt) {
      pDevIXT->m_aEnt =
          EntBuffer::pMoveToGreaterThanEntry(pDevIXT->m_aEnt,
                                             pEnt->aKey(),
                                             &pDevIXT->m_pEntBuffer);
    }
  }
  P_PushPtr(pDevIXT->m_aEnt);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* insert an entry into the indexTree                                         */
/*----------------------------------------------------------------------------*/
void vDev_IXT_POS_UBOUND_LE(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  // pop the wrkent
  EntWrk* pEntWrk = (EntWrk*)(P_PopPtr());

  // pop the device
  DevIXT* pDevIXT = _pDevIXT(P_PopPtr());

  Ent *pEnt = (Ent*)pEntWrk->aEnt();

  pDevIXT->m_aUpperEnt = nullptr;

  EntBuffer* pUpperEntBuffer =
    pDevIXT->m_pIdxTree->pPositionH0Buffer(pEnt->aKey());
  
  if (pUpperEntBuffer) {

    pDevIXT->m_aUpperEnt =
        pUpperEntBuffer->aFindEntryBefore(pEnt->aKey());

    if (pDevIXT->m_aUpperEnt) {

      pDevIXT->m_aUpperEnt =
          EntBuffer::pMoveToNextEntry(pDevIXT->m_aUpperEnt,
                                      &pUpperEntBuffer);

      if (pDevIXT->m_aUpperEnt) {
        pDevIXT->m_aUpperEnt =
            EntBuffer::pMoveToGreaterThanEntry(pDevIXT->m_aUpperEnt,
                                               pEnt->aKey(),
                                               &pUpperEntBuffer);

      }
    }
  }

  P_PushPtr(pDevIXT->m_aUpperEnt);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* insert an entry into the indexTree                                         */
/*----------------------------------------------------------------------------*/
void vDev_IXT_POS_UBOUND_LT(i8** aaiDstk)
{
  i8 *aiDstk = *aaiDstk;

  // pop the wrkent
  EntWrk* pEntWrk = (EntWrk*)(P_PopPtr());

  // pop the device
  DevIXT* pDevIXT = _pDevIXT(P_PopPtr());

  Ent *pEnt = (Ent*)pEntWrk->aEnt();

  pDevIXT->m_aUpperEnt = nullptr;

  EntBuffer* pUpperEntBuffer =
      pDevIXT->m_pIdxTree->pPositionH0Buffer(pEnt->aKey());

  if (pUpperEntBuffer) {

    pDevIXT->m_aUpperEnt =
        pDevIXT->m_pEntBuffer->aFindEntryBefore(pEnt->aKey());

    if (pDevIXT->m_aUpperEnt) {

      pDevIXT->m_aUpperEnt =
          EntBuffer::pMoveToNextEntry(pDevIXT->m_aUpperEnt,
                                      &pUpperEntBuffer);
    }
  }

  P_PushPtr(pDevIXT->m_aUpperEnt);
  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* insert an entry into the indexTree                                         */
/*----------------------------------------------------------------------------*/
void vDev_IXT_POS_UBOUND_LT_OLD(i8** aaiDstk)
{
  i8 *aiDstk = *aaiDstk;

  // pop the wrkent
  EntWrk* pEntWrk = (EntWrk*)(P_PopPtr());

  // pop the device
  DevIXT* pDevIXT = _pDevIXT(P_PopPtr());

  Ent *pEnt = (Ent*)pEntWrk->aEnt();

  pDevIXT->m_aUpperEnt = nullptr;

  EntBuffer* pUpperEntBuffer =
      pDevIXT->m_pIdxTree->pPositionH0Buffer(pEnt->aKey());

  if (pUpperEntBuffer) {

    pDevIXT->m_aUpperEnt =
        pUpperEntBuffer->aFindEntryBefore(pEnt->aKey());

    if (pDevIXT->m_aUpperEnt) {
      pDevIXT->m_aUpperEnt =
          EntBuffer::pMoveToNextEntry(pDevIXT->m_aUpperEnt,
                                      &pUpperEntBuffer);
    }
  }

  P_PushPtr(pDevIXT->m_aUpperEnt);
  *aaiDstk = aiDstk;
}


/*----------------------------------------------------------------------------*/
/* push entry of devixt                                                       */
/*----------------------------------------------------------------------------*/
void vDev_IXT_PUSH_ENT(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  // pop the device
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());

  P_PushPtr(pDevIxt->m_aEnt);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/*  push number of entries in dev index                                       */
/*----------------------------------------------------------------------------*/
void vDev_IXT_IDX_ENTRIES(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  DevIXT* pDevIXT = _pDevIXT(P_PopPtr());

  i8 iRes = pDevIXT->m_pIdxTree->iCountH0Entries();
  I_PushInt(iRes);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* begin linear traverse                                                      */
/*----------------------------------------------------------------------------*/
void vDev_IXT_IDX_BEGIN(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;
  DevIXT* pDevIXT = _pDevIXT(P_PopPtr());

  pDevIXT->m_pEntBuffer = pDevIXT->m_pIdxTree->pFirstH0Buffer();

  if (pDevIXT->m_pEntBuffer) {
    pDevIXT->m_aEnt = pDevIXT->m_pEntBuffer->aLeadEntry();
    if (pDevIXT->m_aEnt) {
      P_PushPtr(pDevIXT->m_aEnt);
    }
    else
      P_PushPtr(nullptr);
  }
  else
    P_PushPtr(nullptr);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* traverse to next entry                                                     */
/*----------------------------------------------------------------------------*/
void vDev_IXT_IDX_NEXT(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;
  DevIXT* pDevIXT = _pDevIXT(P_PopPtr());

  if (pDevIXT->m_pEntBuffer) {
    pDevIXT->m_aEnt =
        EntBuffer::pMoveToNextEntry(pDevIXT->m_aEnt, &pDevIXT->m_pEntBuffer);
  }
  else {
    pDevIXT->m_aEnt = nullptr;
  }

  P_PushPtr(pDevIXT->m_aEnt);
  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* get address of payload                                                     */
/*----------------------------------------------------------------------------*/
void vDev_PAY_ADDRESS(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;
  DevIXT* pDevIXT = _pDevIXT(P_PopPtr());

  i8* aiPayload;

  // aiPayload = pDevIXT->m_aEnt->aiPayCell(0);

  Ent_int* aEnt = (Ent_int*)pDevIXT->m_aEnt;

  if (aEnt->Rrf.m_uKind) {
    aiPayload = (i8 *) ((char *) aEnt
                        + sizeof(struct Ent_int)
                        + 8
    );
  } else {
    aiPayload = (i8 *) ((char *) aEnt
                        + sizeof(struct Ent_int)
                        + aEnt->m_iIntElm0
                        + 8
    );
  }

  P_PushPtr(aiPayload);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/*                                dev ROW                                     */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append next element value                                                  */
/*----------------------------------------------------------------------------*/
void vDev_ROW_ENTWRK_APPENDELMI(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  DevROW* pDevRow = _pDevROW(P_PopPtr());
  EntWrk *pEntWrk = (EntWrk*)P_PopPtr();
  i8 iTop = I_PopInt();

  pEntWrk->vAppendElementInt(pDevRow->m_aRow,iTop);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* append next element value                                                  */
/*----------------------------------------------------------------------------*/
void vDev_ROW_ENTWRK_APPENDELMS(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;
  Stz* szTop;
  i8 nSize;

  DevROW* pDevRow = _pDevROW(P_PopPtr());
  EntWrk *pEntWrk = (EntWrk*)P_PopPtr();
  szTop = (Stz*)S_StrAdr();

  pEntWrk->vAppendElementStz(pDevRow->m_aRow,szTop);
  S_StrDrp();

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* create payload                                                             */
/*----------------------------------------------------------------------------*/
void vDev_ROW_ENTWRK_CREATEPAY(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  i8 iPayLoadBytes = I_PopInt();
  DevROW* pDevRow = _pDevROW(P_PopPtr());
  EntWrk *pEntWrk = (EntWrk*)P_PopPtr();

  pEntWrk->vPayload(true);
  pEntWrk->vPayloadBytes(iPayLoadBytes);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void vDev_ENTWRK_PRINT(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  EntWrk *pEntWrk = (EntWrk*)P_PopPtr();
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());
  IndexTree* pIndexTree = _pIndexTree(pDevIxt->m_pIdxTree);

  Ent *pEnt = pEntWrk->aEnt();
  IndexDef* pIndexDef = pIndexTree->pIndexDef();
  SP_Ent spEnt;
  //pEnt->aKey()->vShow(pIndexTree->pIndexDef());

  vTrace("EntWrk : %s\n"_Stz,spEnt.sz(pEnt,pIndexDef));

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* push element                                                               */
/*----------------------------------------------------------------------------*/
void vDev_ENTWRK_PUSHELMI(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  i8 iElement = I_PopInt();

  EntWrk *pEntWrk = (EntWrk*)P_PopPtr();
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());
  IndexTree* pIndexTree = _pIndexTree(pDevIxt->m_pIdxTree);

  if (pEntWrk->aEnt()->eKind() == KE_INT) {
    Key* aKey = pEntWrk->aEnt()->aKey();

    i8 iKeyValue = *(i8*)aKey;
    I_PushInt(iKeyValue);
  }
  else {
    Key* aKey = pEntWrk->aEnt()->aKey();

    i8 iKeyValue = aKey->iGetIntElm(pIndexTree->pIndexDef(), iElement);
    I_PushInt(iKeyValue);
  }


  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* push element                                                               */
/*----------------------------------------------------------------------------*/
void vDev_ENTWRK_PUSHELMS(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;
  char caLiteral[128];

  i8 iElement = I_PopInt();

  EntWrk *pEntWrk = (EntWrk*)P_PopPtr();
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());
  IndexTree* pIndexTree = _pIndexTree(pDevIxt->m_pIdxTree);

  Key* aKey = pEntWrk->aEnt()->aKey();
  aKey->vShow(pIndexTree->pIndexDef());

  i8 nSize = aKey->iGetStrElm(pIndexTree->pIndexDef(), iElement, &caLiteral[0]);

  Stz* sKey = (Stz*)&caLiteral;
  char* azDst;
  S_PushStr(sKey);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void vDev_ENTWRK_RESET(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;
  EntWrk *pEntWrk = (EntWrk*)P_PopPtr();

  pEntWrk->aEnt()->vReset();

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* set entry kind                                                             */
/*----------------------------------------------------------------------------*/
void vDev_ENTWRK_SETENTKND(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;
  EntWrk *pEntWrk = (EntWrk*)P_PopPtr();

  //pop the device
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());
  IndexTree* pIndexTree = _pIndexTree(pDevIxt->m_pIdxTree);
  pEntWrk->aEnt()->vKind(pIndexTree->eKeyKind());

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* set int only element                                                       */
/*----------------------------------------------------------------------------*/
void vDev_ENTWRK_SETINTELM_OLD(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;
  EntWrk *pEntWrk = (EntWrk*)P_PopPtr();
  i8 iTop = I_PopInt();

  pEntWrk->aEnt()->vKind(KE_INT);
  *(i8*)pEntWrk->aEnt()->aKey() = iTop;

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* set int only element                                                       */
/*----------------------------------------------------------------------------*/
void vDev_ENTWRK_SETINTELM(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;
  EntWrk *pEntWrk = (EntWrk*)P_PopPtr();
  i8 iTop = I_PopInt();

  if (pEntWrk->aEnt()->eKind() == KE_INT) {
    *(i8*)pEntWrk->aEnt()->aKey() = iTop;
  }
  else {
    pEntWrk->vAppendElementInt(nullptr,iTop);
  }

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* set int only element                                                       */
/*----------------------------------------------------------------------------*/
void vDev_ENTWRK_SETSTZELM(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;
  EntWrk *pEntWrk = (EntWrk*)P_PopPtr();

  char* azSrc;
  i8 nSize;

  if (*aiDstk == C_DATUM_NULL)    {
    pEntWrk->vAppendNull();
  }
  else {
    azSrc =  S_StrAdr();
    RASSERT(pEntWrk->aEnt()->eKind() == KE_GENERAL);
    pEntWrk->vAppendElementStz(nullptr, (Stz *) azSrc);
  }
  S_StrDrp();

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin src row traverse                                                     */
/*----------------------------------------------------------------------------*/
void vDev_ROW_ROWBUF_BEGIN(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  // pop the row dev
  DevROW* pDevRow = _pDevROW(P_PopPtr());
  
  pDevRow->m_i_Row = -1;
  pDevRow->m_aRow  = nullptr;
  pDevRow->m_nRows = pDevRow->m_pRowBuffer ? pDevRow->m_pRowBuffer->nRows() : 0;

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* find row given row no                                                      */
/*----------------------------------------------------------------------------*/
void vDev_ROW_ROWBUF_FINDROW(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  // pop the row dev
  DevROW* pDevRow = _pDevROW(P_PopPtr());

  //pop the row no
  i8 i_Row = I_PopInt();

  pDevRow->m_i_Row = i_Row;
  pDevRow->m_aRow = pDevRow->m_pRowBuffer->pRowAddress(pDevRow->m_i_Row );

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* create new row                                                             */
/*----------------------------------------------------------------------------*/
void vDev_ROW_ROW_NEW(i8** aaiDstk)
{
  _static_

  bool bNewBuffer;
  i8 *aiDstk = *aaiDstk;

  // pop the partition no
  i8 i_Ptn = I_PopInt();

  // pop the pot
  Pot* pPot = _pPot(P_PopPtr());

  // pop the row dev
  DevROW* pDevRow = _pDevROW(P_PopPtr());

  pDevRow->m_aRow = pPot->aNewDstRow(i_Ptn, &pDevRow->m_pRowBuffer, &bNewBuffer);

  I_PushInt(bNewBuffer);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* move to next src row                                                       */
/*----------------------------------------------------------------------------*/
void vDev_ROW_ROWBUF_NEXT(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  // pop the row dev
  DevROW* pDevRow = _pDevROW(P_PopPtr());
  
  if (++pDevRow->m_i_Row  < pDevRow->m_nRows) {
    pDevRow->m_aRow = pDevRow->m_pRowBuffer->pRowAddress(pDevRow->m_i_Row );
    I_PushInt(1);
  }
  else {
    I_PushInt(0);
  }

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/*  set buffer to null                                                        */
/*----------------------------------------------------------------------------*/
void vDev_ROW_ROWBUF_NULL(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  // pop the row dev
  DevROW* pDevRow = _pDevROW(P_PopPtr());
  pDevRow->m_pRowBuffer = nullptr;

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/*  get current row cnt                                                       */
/*----------------------------------------------------------------------------*/
void vDev_ROW_ROWBUF_ROWCNT(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  //pop the device
  DevROW* pDevRow = _pDevROW(P_PopPtr());

  I_PushInt(pDevRow->m_nRows);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/*  get current row no                                                        */
/*----------------------------------------------------------------------------*/
void vDev_ROW_ROWBUF_ROWNO(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  //pop the device
  DevROW* pDevRow = _pDevROW(P_PopPtr());

  I_PushInt(pDevRow->m_i_Row);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* pop and deposit top into dst column                                        */
/*----------------------------------------------------------------------------*/
void vDev_ROW_POPCOLI(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  // pop the column
  i8 iColumn = *(aiDstk--);

  // pop the row dev
  DevROW* pDevRow = _pDevROW(P_PopPtr());

  // pop the value
  i8 iValue = *(aiDstk--);

  // position on cell
  pDevRow->m_pRowBuffer->vPositionOnCell(iColumn);

  // deposit the value in the cell
  pDevRow->m_pRowBuffer->vDepositColVal(iValue);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* pop and deposit top into dst column                                        */
/*----------------------------------------------------------------------------*/
void vDev_ROW_POPCOLS(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;
  i8 nSize;

  // pop the column
  i8 iColumn = *(aiDstk--);

  // pop the row dev
  DevROW* pDevRow = _pDevROW(P_PopPtr());

  pDevRow->m_pRowBuffer->vPositionOnCell(iColumn);

  // check for nulls
  if (*(aiDstk) == C_DATUM_NULL) {
    pDevRow->m_pRowBuffer->vDepositColVal(C_DATUM_NULL);
  }
  else {

    // get the str address
    Stz* szStr = (Stz*)S_StrAdr();

    char *azDst = pDevRow->m_pRowBuffer->azDepositString_begin();

    StrLib::vCopy(azDst, (char *) szStr);
    nSize = StrLib::nSize(azDst);

    pDevRow->m_pRowBuffer->vDepositString_end(nSize);
  }
  S_StrDrp();

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* print src row                                                              */
/*----------------------------------------------------------------------------*/
void vDev_ROW_ROW_PRINT(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;
  DevROW* pDevRow = _pDevROW(P_PopPtr());
  
  pDevRow->m_pRowBuffer->vDumpRow(-1,pDevRow->m_aRow);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* push src column                                                            */
/*----------------------------------------------------------------------------*/
void vDev_ROW_PUSHCOLI(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  // pop the column
  i8 iColumn = *(aiDstk--);

  // pop the device
  DevROW* pDevRow = _pDevROW(P_PopPtr());
    
  i8 iRes = pDevRow->m_aRow->iColVal_int(iColumn);

  I_PushInt(iRes);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* push src column                                                            */
/*----------------------------------------------------------------------------*/
Stz* szDev_ROW_PUSHCOLS(void* aLU, void* aROWDEVICE, i8 iArg)
{
  _static_

  UNUSED(aLU);

  DevROW* pDevRow = _pDevROW(aROWDEVICE);
  
  Stz* szSrc = pDevRow->m_aRow->szColVal_ofs(iArg);

  return szSrc;
}

/*----------------------------------------------------------------------------*/
/* push src column                                                            */
/*----------------------------------------------------------------------------*/
void vDev_ROW_PUSHCOLS(i8** aaiDstk)
{
  _static_

  i8 nSize;
  char* azDst;

  i8 *aiDstk = *aaiDstk;

  // pop the column
  i8 iColumn = *(aiDstk--);

  // pop the device
  DevROW* pDevRow = _pDevROW(P_PopPtr());

  Stz* szSrc = pDevRow->m_aRow->szColVal_ofs(iColumn);

  S_PushStr(szSrc);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* push offset of source row                                                  */
/*----------------------------------------------------------------------------*/
void vDev_ROW_PUSHOFS(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;
  DevROW* pDevRow = _pDevROW(P_PopPtr());
  
  i8 iRes = pDevRow->m_pRowBuffer->iOffsetInWsp(pDevRow->m_aRow);

  I_PushInt(iRes);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* reset row                                                                  */
/*----------------------------------------------------------------------------*/
void vDev_ROW_ROWBUF_RESET(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;
  DevROW* pDevRow = _pDevROW(P_PopPtr());
  
  pDevRow->m_pRowBuffer->vReset();

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* set row row                                                                */
/*----------------------------------------------------------------------------*/
void vDev_ROW_ROW_SET(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  // pop the device
  DevROW* pDevRow = _pDevROW(P_PopPtr());

  // pop the row no
  i8 iRow = I_PopInt();

  pDevRow->m_aRow = pDevRow->m_pRowBuffer->aSetCurRow(iRow);
  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* set the index name                                                         */
/*----------------------------------------------------------------------------*/
void vDev_ROW_TABLENM_SET(i8** aaiDstk)
{
  _static_

  // ALKA: Double string copy
  // come back to this !!!

  i8 *aiDstk = *aaiDstk;
  i8 nSize;
  char* azSrc;
  char azDst[256];

  // pop index name into the device
  S_PopStr(azDst);

  // pop row device
  DevROW* pDevRow = _pDevROW(P_PopPtr());
  StrLib::vCopy(pDevRow->m_sTableNm.azChar(), azDst);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* test for row                                                               */
/*----------------------------------------------------------------------------*/
void vDev_ROW_ROW_TEST(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  // pop row device
  DevROW* pDevRow = _pDevROW(P_PopPtr());

  i8 iRes;

  if (pDevRow->m_aRow)
    I_PushInt(1)
  else
    I_PushInt(0)

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* test for buffer                                                            */
/*----------------------------------------------------------------------------*/
void vDev_ROW_ROWBUF_TEST(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  // pop row device
  DevROW* pDevRow = _pDevROW(P_PopPtr());
  
  if (pDevRow->m_pRowBuffer)
    I_PushInt(1)
  else
    I_PushInt(1)

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* zero row                                                                   */
/*----------------------------------------------------------------------------*/
void vDev_ROW_ROW_ZERO(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  // pop row device
  DevROW* pDevRow = _pDevROW(P_PopPtr());
  
  pDevRow->m_pRowBuffer->vZeroRow(pDevRow->m_aRow);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* drop tablePtn read lock                                                    */
/*----------------------------------------------------------------------------*/
void vDev_TABLEPTN_DROPREAD(void* aLU)
{
  _static_

  LogicUnit *pLogicUnit = _pLogicUnit(aLU);

  TablePtn *pTablePtn = *pLogicUnit->m_xpTablePtn;
  pTablePtn->vDropRead();
}

/*----------------------------------------------------------------------------*/
/* grab tablePtn read lock                                                    */
/*----------------------------------------------------------------------------*/
void vDev_TABLEPTN_GRABREAD(void* aLU)
{
  _static_

  LogicUnit *pLogicUnit = _pLogicUnit(aLU);

  TablePtn *pTablePtn = *pLogicUnit->m_xpTablePtn;
  pTablePtn->bGrabRead();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* push column value from dev                                                 */
/*----------------------------------------------------------------------------*/
void vDev_IXT_COL_PUSHSET_ADR(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  char* acStrArea_null = nullptr;

  // pop the ixt device
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());

  // pop the row device
  DevROW* pDevRow = _pDevROW(P_PopPtr());

  // pop the column set
  i8* iaColSet = (i8*)P_PopPtr();

  *aaiDstk = aiDstk;
  RowInflator::vInflate_ColumnSet_into_CellArray
      (
          aaiDstk,
          acStrArea_null,
          SI_CELLARRAY_ADR,
          (Crw*)pDevIxt->m_aCrw,
          iaColSet,
          pDevRow->m_pStreamDef
      );
}

/*----------------------------------------------------------------------------*/
/* // inflate dev columns direct into locals                                  */
/*----------------------------------------------------------------------------*/
void vDev_IXT_COLS_TO_LOCALS(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  char* acStrArea_null = nullptr;

  // pop the ixt device
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());

  // pop the row device
  DevROW* pDevRow = _pDevROW(P_PopPtr());

  // pop the column set
  i8* iaColSet = (i8*)P_PopPtr();

  // pop the locals address
  i8** aaiLocals = (i8**)P_PopPtr();

  RowInflator::vInflate_ColumnSet_into_CellArray
      (
          aaiLocals,
          acStrArea_null,
          SI_CELLARRAY_ADR,
          (Crw*)pDevIxt->m_aCrw,
          iaColSet,
          pDevRow->m_pStreamDef
      );

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* push column value from m_aSrcCrw                                           */
/*----------------------------------------------------------------------------*/
void vDev_IXT_COL_PUSHSET_PST(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  char* acStrArea_null = nullptr;

  // pop the ixt device
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());

  // pop the row device
  DevROW* pDevRow = _pDevROW(P_PopPtr());

  // pop the column set
  i8* iaColSet = (i8*)P_PopPtr();

  *aaiDstk = aiDstk;
  RowInflator::vInflate_ColumnSet_into_CellArray
      (
          aaiDstk,
          acStrArea_null,
          SI_CELLARRAY_PST,
          (Crw*)pDevIxt->m_aCrw,
          iaColSet,
          pDevRow->m_pStreamDef
      );
}

/*----------------------------------------------------------------------------*/
/* inflate dev columns to dev row                                             */
/*----------------------------------------------------------------------------*/
void vDev_IXT_COLS_TO_ROW(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  // pop the ixt device
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());

  // pop the row device
  DevROW* pDevRow = _pDevROW(P_PopPtr());

  //HeadLine *aHeadLine = (HeadLine *) pLogicUnit->m_xRluCommon.m_aSrcCrw;
  HeadLine *aHeadLine = (HeadLine *) pDevIxt->m_aCrw;

  RowInflator::vInflate_AllColumns_into_RowBuffer(pDevRow->m_pRowBuffer,aHeadLine);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* inflate dev columns to dev row                                             */
/*----------------------------------------------------------------------------*/
void vDev_IXT_COLS_TO_ROW_OFS(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  // pop the dst row start col
  i8 iAtDstCol = I_PopInt();

  // pop the ixt device
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());

  // pop the row device
  DevROW* pDevRow = _pDevROW(P_PopPtr());

  // pop the dst row device
  DevROW* pDevRowDst = _pDevROW(P_PopPtr());

  //HeadLine *aHeadLine = (HeadLine *) pLogicUnit->m_xRluCommon.m_aSrcCrw;
  HeadLine *aHeadLine = (HeadLine *) pDevIxt->m_aCrw;

  RowInflator::vInflate_AllColumns_into_RowBufferAt(pDevRowDst->m_pRowBuffer,
                                                    iAtDstCol,
                                                    aHeadLine,
                                                    pDevRow->m_pStreamDef->nColumns());

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* row buffer columns of row dev into locals                                  */
/*----------------------------------------------------------------------------*/
void vDev_ROW_COLS_TO_LOCALS(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  // pop the row device
  DevROW* pDevRow = _pDevROW(P_PopPtr());

  // pop the column set
  i8* iaColSet = (i8*)P_PopPtr();

  // pop the locals address
  i8** aaiLocals = (i8**)P_PopPtr();

  i8* aiDst_lcl = *aaiLocals;
  i8* ayDst = ++aiDst_lcl;
  d8 dValue;

  for (i8 i_ColSet = 0; iaColSet[i_ColSet] != -1; i_ColSet++) {
    Column* pColumn = pDevRow->m_pStreamDef->pGetColumn(iaColSet[i_ColSet]);
    switch (pColumn->eColumnType()) {
      case CT_BOOLEAN:
      case CT_INTEGER:
      case CT_DATE:
      case CT_TIME:
      case CT_TIMESTAMP:
      case CT_DECIMAL:
        *ayDst = pDevRow->m_aRow->iColVal_int(iaColSet[i_ColSet]);
        ayDst++;
        break;
      case CT_FLOAT:
        dValue = pDevRow->m_aRow->dColVal_flt(iaColSet[i_ColSet]);
        *ayDst = *(i8*)&dValue;
        ayDst++;
        break;
      case CT_CHAR:
      case CT_VARCHAR:
        *ayDst = (i8)pDevRow->m_aRow->szColVal_ofs(iaColSet[i_ColSet]);
        ayDst++;
        break;
      default:
        ASSERT(0);
    }
  }

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* row buffer columns of row dev into row                                     */
/*----------------------------------------------------------------------------*/
void vDev_ROW_COLS_TO_ROW(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  // pop the src row device
  DevROW* pDevRow = _pDevROW(P_PopPtr());

  // pop the dst row device
  DevROW* pDevRowDst = _pDevROW(P_PopPtr());

  Stz* szStr;
  char* azDst;
  i8 nSize;

  for (i8 i_Col = 0; i_Col < pDevRow->m_pStreamDef->nColumns(); i_Col++) {
    Column* pColumn = pDevRow->m_pStreamDef->pGetColumn(i_Col);
    switch (pColumn->eColumnType()) {
      case CT_BOOLEAN:
      case CT_INTEGER:
      case CT_DATE:
      case CT_TIME:
      case CT_TIMESTAMP:
      case CT_DECIMAL:
        pDevRowDst->m_aRow->vColVal_int(i_Col, pDevRow->m_aRow->iColVal_int(i_Col));
        break;
      case CT_CHAR:
      case CT_VARCHAR:
        // get the str address
        szStr = pDevRow->m_aRow->szColVal_ofs(i_Col);

        pDevRowDst->m_pRowBuffer->vPositionOnCell(i_Col);

        azDst = pDevRowDst->m_pRowBuffer->azDepositString_begin();

        StrLib::vCopy(azDst,(char*)szStr);
        nSize = StrLib::nSize(azDst);

        pDevRowDst->m_pRowBuffer->vDepositString_end(nSize);
        break;

      default:
        ASSERT(0);
    }
  }

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* row buffer columns of row dev into row                                     */
/*----------------------------------------------------------------------------*/
void vDev_ROW_COLS_TO_ROW_OFS(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  // pop the dst row start col
  i8 iDstCol = I_PopInt();

  // pop the src row device
  DevROW* pDevRow = _pDevROW(P_PopPtr());

  // pop the dst row device
  DevROW* pDevRowDst = _pDevROW(P_PopPtr());

  Stz* szStr;
  char* azDst;
  i8 nSize;

  for (i8 i_Col = 0; i_Col < pDevRow->m_pStreamDef->nColumns(); i_Col++) {
    Column* pColumn = pDevRow->m_pStreamDef->pGetColumn(i_Col);
    switch (pColumn->eColumnType()) {
      case CT_BOOLEAN:
      case CT_INTEGER:
      case CT_DATE:
      case CT_TIME:
      case CT_TIMESTAMP:
      case CT_DECIMAL:
        pDevRowDst->m_aRow->vColVal_int(iDstCol, pDevRow->m_aRow->iColVal_int(i_Col));
        break;
      case CT_CHAR:
      case CT_VARCHAR:
        // get the str address
        szStr = pDevRow->m_aRow->szColVal_ofs(i_Col);

        pDevRowDst->m_pRowBuffer->vPositionOnCell(iDstCol);

        azDst = pDevRowDst->m_pRowBuffer->azDepositString_begin();

        StrLib::vCopy(azDst,(char*)szStr);
        nSize = StrLib::nSize(azDst);

        pDevRowDst->m_pRowBuffer->vDepositString_end(nSize);
        break;

      default:
        ASSERT(0);
    }
    iDstCol++;
  }

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* push column value from row of row device                                   */
/*----------------------------------------------------------------------------*/
void vDev_ROW_COLS_PUSHSET_PST(i8** aaiDstk) {
  _static_

  i8 *aiDstk = *aaiDstk;

  char *acStrArea_null = nullptr;

  // pop the row device
  DevROW *pDevRow = _pDevROW(P_PopPtr());

  // pop the column set
  i8 *iaColSet = (i8 *) P_PopPtr();

  i8* ayDst = ++aiDstk;
  d8 dValue;

  for (i8 i_ColSet = 0; iaColSet[i_ColSet] != -1; i_ColSet++) {
    Column* pColumn = pDevRow->m_pStreamDef->pGetColumn(iaColSet[i_ColSet]);
    switch (pColumn->eColumnType()) {
      case CT_BOOLEAN:
      case CT_INTEGER:
      case CT_DATE:
      case CT_TIME:
      case CT_TIMESTAMP:
      case CT_DECIMAL:
        *ayDst = pDevRow->m_aRow->iColVal_int(iaColSet[i_ColSet]);
        ayDst++;
        break;
      case CT_FLOAT:
        dValue = pDevRow->m_aRow->dColVal_flt(iaColSet[i_ColSet]);
        *ayDst = *(i8*)&dValue;
        ayDst++;
        break;
      case CT_CHAR:
      case CT_VARCHAR:
        *ayDst = (i8)pDevRow->m_aRow->szColVal_ofs(iaColSet[i_ColSet]);
        ayDst++;
        break;
      default:
        ASSERT(0);
    }
  }

  *aaiDstk = --ayDst;
}

/*----------------------------------------------------------------------------*/
/* row buffer columns of row dev null values into row                         */
/*----------------------------------------------------------------------------*/
void vDev_ROW_NULS_TO_ROW_OFS(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  // pop the dst row start col
  i8 iDstCol = I_PopInt();

  // pop the src row device
  DevROW* pDevRow = _pDevROW(P_PopPtr());

  // pop the dst row device
  DevROW* pDevRowDst = _pDevROW(P_PopPtr());

  for (i8 i_Col = 0; i_Col < pDevRow->m_pStreamDef->nColumns(); i_Col++) {
    pDevRowDst->m_aRow->vSetColNull(iDstCol++);
  }

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* print row pix                                                              */
/*----------------------------------------------------------------------------*/
void vDev_RLU_ROWPIX_PRINT(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;
  RowPix* pRowPix = _pRowPix(P_PopPtr());

  pRowPix->vPrint();
}

/*----------------------------------------------------------------------------*/
/* use stream def out for this pot                                            */
/*----------------------------------------------------------------------------*/
void vDev_POT_USE_STREAMDEFOUT(void* aLU, i8 iAddPtnHsh)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  
  pLogicUnit->m_xpStreamDefOut->vToRowPix((RowPix*)pLogicUnit->m_xRluCommon.m_pRowPix,
                                               (bool)iAddPtnHsh);

  Pot* pPot = _pPot(pLogicUnit->m_xRluCommon.m_pPot);

  pPot->vRowPix((RowPix*)pLogicUnit->m_xRluCommon.m_pRowPix);
  
}

/*----------------------------------------------------------------------------*/
/* move StreamDefLft[] to  streamDefout[]                                     */
/*----------------------------------------------------------------------------*/
void vDev_REG_STREAMDEF_STREAMDEFOUT(void* aLU)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  
  pLogicUnit->m_xpStreamDefOut = * pLogicUnit->m_xpStreamDefLft;
}

/*----------------------------------------------------------------------------*/
/* new an EOSMARKER in OperandA                                               */
/*----------------------------------------------------------------------------*/
void vDev_REG_EOSMARKER_NEW(void* aLU)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  
  pLogicUnit->m_xpMarker = new Marker(OM_EOSMARKER);
}

/*----------------------------------------------------------------------------*/
/* add a column to indexDefRlu[]                                              */
/*----------------------------------------------------------------------------*/
void vDev_RLU_INDEXDEF_ADDCOLUMN(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;
  i8 nSize;

  // get the indexDef
  IndexDef* pIndexDef = _pIndexDef(P_PopPtr());

  // get the type of the column
  eCT eColType = (eCT)I_PopInt();

  // get the name of the column
  Stz* szColumnName = (Stz*)S_StrAdr();

  pIndexDef->vAddColumn(szColumnName,eColType);

  S_StrDrp();

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* delete indexDefRlu[]                                                       */
/*----------------------------------------------------------------------------*/
void vDev_RLU_INDEXDEF_DELETE(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;
  
  IndexDef* pIndexDef = _pIndexDef(P_PopPtr());
  
  delete pIndexDef;

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* new IndexDef[]                                                             */
/*----------------------------------------------------------------------------*/
void vDev_RLU_INDEXDEF_NEW(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;
  i8 nSize;

  // get the name of the indexDef
  Stz* szName = (Stz*)S_StrAdr();

  IndexDef* pIndexDef = new IndexDef;
  pIndexDef->vIndexNm(szName);

  S_StrDrp();

  P_PushPtr(pIndexDef);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* new indexTreeRlu[]                                                         */
/*----------------------------------------------------------------------------*/
void vDev_RLU_INDEXTREE_NEW(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  IndexDef* pIndexDef = _pIndexDef(P_PopPtr());
  IndexTree* pIndexTree = new IndexTree(nullptr, pIndexDef, C_UNKNOWN);
  P_PushPtr(pIndexTree);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* close ptnStream[]                                                          */
/*----------------------------------------------------------------------------*/
void vDev_REG_PTNSTREAM_CLOSE(void* aLU) {
  _static_

  LogicUnit *pLogicUnit = _pLogicUnit(aLU);

  PtnStream *pPtnStream = *pLogicUnit->m_xpPtnStream;
  pPtnStream->vClose();
}

/*----------------------------------------------------------------------------*/
/* delete ptnStream[]                                                         */
/*----------------------------------------------------------------------------*/
void vDev_REG_PTNSTREAM_DELETE(void* aLU)
{
  _static_

  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  PtnStream* pPtnStream = *pLogicUnit->m_xpPtnStream;
  delete pPtnStream;
  pLogicUnit->m_xpPtnStream = nullptr;
}

/*----------------------------------------------------------------------------*/
/* new ptnStream[]                                                            */
/*----------------------------------------------------------------------------*/
void vDev_REG_PTNSTREAM_NEW(void* aLU)
{
  _static_

  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  pLogicUnit->m_xpPtnStream = new PtnStream(*pLogicUnit->m_xpTablePtn);
}

/*----------------------------------------------------------------------------*/
/* open ptnStream[]                                                           */
/*----------------------------------------------------------------------------*/
void vDev_REG_PTNSTREAM_OPEN(void* aLU)
{
  _static_

  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  PtnStream* pPtnStream = *pLogicUnit->m_xpPtnStream;
  bool bOpened = pPtnStream->bOpen();
  UNUSED(bOpened);
}

/*----------------------------------------------------------------------------*/
/* read ptnStream[] into Operand[]                                            */
/*----------------------------------------------------------------------------*/
void vDev_REG_PTNSTREAM_READ(void* aLU)
{
  _static_

  // Alka: 31 August 2021
  // Not used.
  // Adding RASSERT as we no longer have OperandA
  // So if we revoke this we need to use another register
  RASSERT(0);
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  PtnStream* pPtnStream = *pLogicUnit->m_xpPtnStream;
  //pLogicUnit->m_xpOperandA = pPtnStream->pRead();
}

/*----------------------------------------------------------------------------*/
/* new a streamDefout[]                                                       */
/*----------------------------------------------------------------------------*/
void vDev_REG_STREAMDEFOUT_NEW(void* aLU)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  
  pLogicUnit->m_xpStreamDefOut = new StreamDef;
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
