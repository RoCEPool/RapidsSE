/*============================================================================*/
/* DEV_interface.cpp -- RLU Dev Interface                                     */
/*============================================================================*/

#include "BASE.h"

#include "IndexDef.hpp"
#include "Marker.hpp"

#include "RowBuffer.hpp"

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
#include "ReadApi.hpp"
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
                     pDevIxt->m_pCrwBuffer->nRowCount() : 0;

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

  //pDevIxt->m_aCrw  =
  //  CrwBuffer::pRefdCrw(pDevIxt->m_aEnt->rxRef());

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
  Ent_int* aWrkEnt = (Ent_int*)P_PopPtr();

  //pop the device
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());

  Ent *pEnt = (Ent*)aWrkEnt;
  pDevIxt->m_aEnt =
    pDevIxt->m_pIdxTree->aPositionPriorEntry(pEnt->aKey(),
                                             &pDevIxt->m_pEntBuffer);
  
//  if (pDevIxt->m_pEntBuffer) {
//    pLogicUnit->m_xRluCommon.m_aEntArray =
//      (Ent_int*)pDevIxt->m_pEntBuffer->aEntArray();
//
//    //*aaSkipRoots = (Ent_int*)pDevIxt->m_pEntBuffer->aSkipRoots();
//  }
  
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

  P_PushPtr(pDevIXT->m_aEnt);
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

  IndexTree* pIndexTree = pDevIxt->m_pIdxTree;
  pDevIxt->m_pIdxTree = nullptr;
  delete pIndexTree;

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
void vDev_IXT_POS_LBOUND(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  // pop the device
  DevIXT* pDevIXT = _pDevIXT(P_PopPtr());

  // pop the wrkent
  EntWrk* pEntWrk = (EntWrk*)(P_PopPtr());
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
void vDev_IXT_POS_UBOUND(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  // pop the device
  DevIXT* pDevIXT = _pDevIXT(P_PopPtr());

  // pop the wrkent
  EntWrk* pEntWrk = (EntWrk*)(P_PopPtr());
  Ent *pEnt = (Ent*)pEntWrk->aEnt();
  
  EntBuffer* pUpperEntBuffer =
    pDevIXT->m_pIdxTree->pPositionH0Buffer(pEnt->aKey());
  
  if (pDevIXT) {
    
    pDevIXT->m_aUpperEnt =
      pUpperEntBuffer->aFindEntryBefore(pEnt->aKey());
    
    if (pDevIXT->m_aUpperEnt) {
      pDevIXT->m_aUpperEnt =
        EntBuffer::pMoveToGreaterThanEntry(pDevIXT->m_aUpperEnt,
                                           pEnt->aKey(),
                                           &pUpperEntBuffer);
      P_PushPtr(pDevIXT->m_aUpperEnt);
    }
    else
      P_PushPtr( nullptr);
  }
  else
    P_PushPtr( nullptr);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* insert an entry into the indexTree                                         */
/*----------------------------------------------------------------------------*/
void vDev_IXT_POS_UBOUND_LT(i8** aaiDstk)
{
  i8 *aiDstk = *aaiDstk;

  // pop the device
  DevIXT* pDevIXT = _pDevIXT(P_PopPtr());

  // pop the wrkent
  EntWrk* pEntWrk = (EntWrk*)(P_PopPtr());
  Ent *pEnt = (Ent*)pEntWrk->aEnt();
  
  EntBuffer* pUpperEntBuffer =
  pDevIXT->m_pIdxTree->pPositionH0Buffer(pEnt->aKey());
  
  if (pUpperEntBuffer) {
    
    pDevIXT->m_aUpperEnt =
    pUpperEntBuffer->aFindEntryBefore(pEnt->aKey());
    
    if (pDevIXT->m_aUpperEnt) {
      pDevIXT->m_aUpperEnt =
      EntBuffer::pMoveToNextEntry(pDevIXT->m_aUpperEnt,
                                  &pUpperEntBuffer);
      
      P_PushPtr(pDevIXT->m_aUpperEnt);
    }
    else
      P_PushPtr( nullptr);
  }
  else
      P_PushPtr( nullptr);
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

  pDevIXT->m_aEnt =
     EntBuffer::pMoveToNextEntry(pDevIXT->m_aEnt,&pDevIXT->m_pEntBuffer);

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
  IndexTree *pIndexTree = _pIndexTree(P_PopPtr());

  Ent *pEnt = pEntWrk->aEnt();
  IndexDef* pIndexDef = pIndexTree->pIndexDef();
  SP_Ent spEnt;

  vTrace("EntWrk : %s\n"_Stz,spEnt.sz(pEnt,pIndexDef));

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
/* set int only element                                                       */
/*----------------------------------------------------------------------------*/
void vDev_ENTWRK_SETINTELM(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;
  EntWrk *pEntWrk = (EntWrk*)P_PopPtr();
  i8 iTop = I_PopInt();

  //pLogicUnit->m_oEntWrk.vAppendElementInt(pDevRow->m_aRow,iTop);
  pEntWrk->aEnt()->vKind(KE_INT);
  *(i8*)pEntWrk->aEnt()->aKey() = iTop;

  *aaiDstk = aiDstk;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin src row traverse                                                     */
/*----------------------------------------------------------------------------*/
void vDev_ROW_BEGIN(i8** aaiDstk)
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
/* create new row                                                             */
/*----------------------------------------------------------------------------*/
void vDev_ROW_NEW(i8** aaiDstk)
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

  pDevRow->m_aRow =pPot->aNewDstRow(i_Ptn, &pDevRow->m_pRowBuffer, &bNewBuffer);

  I_PushInt(bNewBuffer);

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* move to next src row                                                       */
/*----------------------------------------------------------------------------*/
void vDev_ROW_NEXT(i8** aaiDstk)
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
void vDev_ROW_NULL(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  // pop the row dev
  DevROW* pDevRow = _pDevROW(P_PopPtr());
  pDevRow->m_pRowBuffer = nullptr;

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

  // pop the column
  i8 iColumn = *(aiDstk--);

  // pop the row dev
  DevROW* pDevRow = _pDevROW(P_PopPtr());

  // get the str address
  Stz* szStr = (Stz*)S_StrAdr();

  pDevRow->m_pRowBuffer->vPositionOnCell(iColumn);
  
  char* azDst = pDevRow->m_pRowBuffer->azDepositString_begin();
  
  StrLib::vCopy(azDst,(char*)szStr);
  i8 nSize = StrLib::nSize(azDst);

  pDevRow->m_pRowBuffer->vDepositString_end(nSize);
  S_StrDrp();

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* print src row                                                              */
/*----------------------------------------------------------------------------*/
void vDev_ROW_PRINT(i8** aaiDstk)
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
void vDev_ROW_RESET(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;
  DevROW* pDevRow = _pDevROW(P_PopPtr());
  
  pDevRow->m_pRowBuffer->vReset();

  *aaiDstk = aiDstk;
}

/*----------------------------------------------------------------------------*/
/* set row row                                                                 */
/*----------------------------------------------------------------------------*/
void vDev_ROW_SET(i8** aaiDstk)
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
/* set src row by using offset on stack                                       */
/*----------------------------------------------------------------------------*/
void vDev_ROW_SETBYOFS(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  // pop the offset
  i8 iOffset = *(aiDstk--);

  // pop the device
  DevROW* pDevRow = _pDevROW(P_PopPtr());
  
  pDevRow->m_aRow = pDevRow->m_pRowBuffer->aRowByOfs(iOffset);
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
void vDev_ROW_TESTROW(i8** aaiDstk)
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
void vDev_ROW_TESTBUFFER(i8** aaiDstk)
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
void vDev_ROW_ZERO(i8** aaiDstk)
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
/* push column value from m_aSrcCrw                                           */
/*----------------------------------------------------------------------------*/
void vDev_IXT_COL_PUSHSET_ADR(i8** aaiDstk, i8* iaColSet)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  char* acStrArea_null = nullptr;

  // pop the ixt device
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());

  // pop the row device
  DevROW* pDevRow = _pDevROW(P_PopPtr());

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
/* push column value from m_aSrcCrw                                           */
/*----------------------------------------------------------------------------*/
void vDev_IXT_COL_PUSHSET_PST(i8** aaiDstk, i8* iaColSet)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  char* acStrArea_null = nullptr;

  // pop the ixt device
  DevIXT* pDevIxt = _pDevIXT(P_PopPtr());

  // pop the row device
  DevROW* pDevRow = _pDevROW(P_PopPtr());

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
/* push column value from m_aSrcCrw to dst row                                */
/*----------------------------------------------------------------------------*/
void vDev_IXT_COL_PUSHSET_TOROW(i8** aaiDstk, i8* iaColSet)
{
  _static_

  i8 *aiDstk = *aaiDstk;

  char* acStrArea_null = nullptr;

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
/* push column value from m_aSrcCrw                                           */
/*----------------------------------------------------------------------------*/
void vDev_ALT_COL_PUSHSET(void* aLU, i8** aaiDstk, i8 ieStrInflateRule, i8* iaColSet)
{
  _static_

  char* acStrArea_null = nullptr;

  LogicUnit* pLogicUnit = _pLogicUnit(aLU);
  DevIXT* pDevIxt = _pDevIXT(pLogicUnit->m_pDevAltIxt);

  RowInflator::vInflate_ColumnSet_into_CellArray
  (
    aaiDstk,
    acStrArea_null,
    (eSI)ieStrInflateRule,
    (Crw*)pDevIxt->m_aCrw,
    iaColSet,
    pLogicUnit->m_pDevAltRow->m_pStreamDef
  );

}

/*----------------------------------------------------------------------------*/
/* push column value from m_aSrcCrw                                           */
/*----------------------------------------------------------------------------*/
void vDev_ROW_COL_PUSHSET(void* aLU, i8** aaiDstk, void* aROWDEVICE, i8* iaColSet)
{
  _static_
  
  UNUSED(aLU);

  DevROW* pDevRow = _pDevROW(aROWDEVICE);

  i8* aiDstk_lcl = *aaiDstk;
  i8* ayDst = ++aiDstk_lcl;

  for (i8 i_ColSet = 0; iaColSet[i_ColSet] != -1; i_ColSet++) {
    Column* pColumn = pDevRow->m_pStreamDef->pGetColumn(iaColSet[i_ColSet]);
    switch (pColumn->eColumnType()) {
      case CT_INTEGER:
      case CT_DATE:
      case CT_TIMESTAMP:
      case CT_DECIMAL:
        *ayDst = pDevRow->m_aRow->iColVal_int(iaColSet[i_ColSet]);
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
}

/*----------------------------------------------------------------------------*/
/* push column value from m_aSrcCrw                                           */
/*----------------------------------------------------------------------------*/
HACK(LogicUnit,"Alka : vInflate_Key_into_ElementArray is not right");

void vDev_SRC_COL_PUSHSET(void* aLU, i8** aaiDstk, i8 ieStrInflateRule, i8* iaColSet)
{
  _static_
  
  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  DevIXT* pDevIxt = _pDevIXT(pLogicUnit->m_pDevSrcIxt);

  //HeadLine *aHeadLine = (HeadLine *) pLogicUnit->m_xRluCommon.m_aSrcCrw;
  HeadLine *aHeadLine = (HeadLine *) pDevIxt->m_aCrw;

  StreamDef* pStreamDef = pLogicUnit->m_pDevSrcRow->m_pStreamDef;
  
  RowInflator::vInflate_Key_into_ElementArray
        (aaiDstk,aHeadLine,iaColSet,
         pStreamDef,nullptr,ieStrInflateRule);
  return;
  
  //--- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - ---
  
  u1* auMetaCode;
  byte* ayDataByte;
  
  i8 nCols = pStreamDef->nColumns();
  
  //vPrepare_v2(auMetaCode,ayDataByte,nCols,aHeadLine);
  auMetaCode = aHeadLine->auMetaArea();
  
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
  u1 uMetaCode;
  u1 uHigh;
  u1 uLow;
  
  u2 u_Tail_rel;
  
  char* azDst;
  char** aacCellValue;
  
  Edt* aEdt;
  Dte* aDte;
  
  OverLine* aOverLine;
  TailLine* aTailLine;
  
  i8 nRoundedStrBytes;
  
  for (i8 i_Col = 0; /*none*/; i_Col++) {
    
    uMetaCode = *auMetaCode++;
    uHigh = uMetaCode >> 4;
    uLow  = uMetaCode & 0x0F;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // ignore column
    
    if (i_Col != i_ColToRetrieve) {
      
    L_Overflowed:
      
      if (uHigh <=  MC_STRING_x1) // numeric & date
        ayDataByte += uLow;
      else
      if (uHigh >=  MC_NEGATIVE)  // small value
      {;}
      else
      if (uHigh == MC_STRING_x1)  // string x 1
        ayDataByte += STRx1(uLow);
      else
      if (uHigh == MC_STRING_x2)  // string x 2
        ayDataByte += STRx2(uLow);
      else
      if (uHigh == MC_STRING_x4)  // string x 4
        ayDataByte += STRx4(uLow);
      else
      if (uHigh == MC_STRING_x8)  // string x 8
        ayDataByte += STRx8(uLow);
      else
      if (uHigh == MC_STRING_TAIL)  // string tail is 2B
        ayDataByte += 2;
      else
      if (uHigh == MC_OVERFLOW) { // overflow
        
        //OverLine* aOverLine = aHeadLine->aGetOver();
        
        // calculation to get overflow
        nSizeUnits = aHeadLine->w8_1B(C_HEADLINE_OFS_WORDS);
        if (nSizeUnits >= 128) nSizeUnits -= 128;
        nHeadBytes = nSizeUnits * C_BYTES_PER_WORD;
        i_Over_rel = aHeadLine->w8_2B(nHeadBytes - C_2B);
        
        aOverLine = RowNub::aGetOver_rel(aHeadLine,i_Over_rel);
        //OverLine* aOverLine = (OverLine*)aGetTail_rel(aHeadLine,i_Over_rel);
        
        uMetaCode = *((u1*)aOverLine + C_U_1B);
        METACODE_SPLIT(uMetaCode,uHigh,uLow);
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
    
    //RASSERT(SI_CELLARRAY_ADR == eStrInflateRule || SI_CELLARRAY_PST == eStrInflateRule);
    
    
    /////////////////////////////////////////////////////////////////////////////
    
  L_HighSwitch:
    
    aiCellValue = (i8*)ayDstByte;
    
    switch (uHigh) {
        
      case MC_NUMERIC: // numeric values 0..8B
        
        switch (uLow) {
            
          case 0x0: // MC_NUMERIC 0B
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
            
          case 0x1: // MC_NUMERIC 1B
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
            
          case 0x2: // MC_NUMERIC 2B
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
            
          case 0x3: // MC_NUMERIC 3B
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
            
          case 0x4: // MC_NUMERIC 4B
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
            
          case 0x5: // MC_NUMERIC 5B
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
            
          case 0x6: // MC_NUMERIC 6B
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
            
          case 0x7: // MC_NUMERIC 7B
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
            
          case 0x8: // MC_NUMERIC 8B
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
            
          case 0x9: // MC_NUMERIC 9B
            RASSERT(0);
            break;
            
          case 0xA: // MC_NUMERIC 10B
            RASSERT(0);
            break;
            
          case 0xB: // MC_NUMERIC 11B
            RASSERT(0);
            break;
            
          case 0xC: // MC_NUMERIC 12B
            RASSERT(0);
            break;
            
          case 0xD: // MC_NUMERIC 13B
            RASSERT(0);
            break;
            
          case 0xE: // MC_NUMERIC 14B
            RASSERT(0);
            break;
            
          case 0xF: // MC_NUMERIC 15B
            RASSERT(0);
            break;
            
        }
        break;
        
      case MC_DATE_E0: // date era-0 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
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
        
      case MC_DATE_E1: // date era-1 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
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
        
      case MC_DATE_E2: // date era-2 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
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
        
      case MC_STRING_x1: // short string 0..15B (no null) - -- -- -- -- -- -- --
        nStrSize = STRx1(uLow);
        
        if (SI_CELLARRAY_ADR == (eSI)ieStrInflateRule) {
          aacCellValue = (char**)ayDstByte;
          *aacCellValue = (char*)ayDataByte;
          ayDstByte += 8;
          
          ayDataByte += nStrSize;
        }
        else
        if (SI_STRAREA_ADR == (eSI)ieStrInflateRule) {
          memcpy(acStrArea,ayDataByte,(size_t)nStrSize);
          ayDataByte += nStrSize;
          
          aacCellValue = (char**)ayDstByte;
          *aacCellValue = acStrArea;
          ayDstByte += 8;
          acStrArea += nStrSize;
        }
        else
        if (SI_CELLARRAY_PST == (eSI)ieStrInflateRule) {
          memcpy(ayDstByte,ayDataByte,(size_t)nStrSize);
          ayDataByte += nStrSize;
          
          //vTrace("SI_CELLARRAY_PST : %p %s\n"_Stz,rayDstByte,rayDstByte);
          
          nRoundedStrBytes = nWordRoundedStrBytes(nStrSize);
          ayDstByte += nRoundedStrBytes;
          aiCellValue = (i8*)ayDstByte;
          *aiCellValue = nStrSize;
          ayDstByte += 8;
        }
        else {
          RASSERT(0);
        }
      
        break;
        
      case MC_NULL: // null - -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
        *(i8*)ayDstByte = C_DATUM_NULL;
        ayDstByte += C_BYTES_PER_WORD;
        break;
        
      case MC_SPARE0: // spare
        RASSERT(0);
        break;
        
      case MC_OVERFLOW: // overflow 2B
        
        // calculation to get overflow
        //aOverLine = aHeadLine->aGetOver();
        nSizeUnits = aHeadLine->w8_1B(C_HEADLINE_OFS_WORDS);
        if (nSizeUnits >= 128) nSizeUnits -= 128;
        nHeadBytes = nSizeUnits * C_BYTES_PER_WORD;
        i_Over_rel = aHeadLine->w8_2B(nHeadBytes - C_2B);
        
        aOverLine = RowNub::aGetOver_rel(aHeadLine,i_Over_rel);
        
        METACODE_SPLIT( (* aAdd<u1*>(aOverLine,C_U_1B) ), uHigh, uLow);
        ayDataByte = aAdd<byte*>(aOverLine,C_U_1B + C_M_1B);
        goto L_HighSwitch;
        
        break;
        
        
        // end where uLow is n-bytes
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // toBytes(uLow) depends on uHigh
        
      case MC_STRING_x2: // string x 2 -- -- -- -- -- -- -- -- -- -- -- -- -- --
        nStrSize = STRx2(uLow);
        
        if (SI_CELLARRAY_ADR == (eSI)ieStrInflateRule) {
          aacCellValue = (char**)ayDstByte;
          *aacCellValue = (char*)ayDataByte;
          ayDstByte += 8;
          
          ayDataByte += nStrSize;
        }
        else
        if (SI_STRAREA_ADR == (eSI)ieStrInflateRule) {
          
          memcpy(acStrArea,ayDataByte,(size_t)nStrSize);
          ayDataByte += nStrSize;
          
          aacCellValue = (char**)ayDstByte;
          *aacCellValue = acStrArea;
          ayDstByte += 8;
          acStrArea += nStrSize;
        }
        else
        if (SI_CELLARRAY_PST == (eSI)ieStrInflateRule) {
          azDst = (char*)ayDstByte;
          memcpy(azDst,ayDataByte,(size_t)nStrSize);
          ayDataByte += nStrSize;
          
          //vTrace("SI_CELLARRAY_PST : %p %s\n"_Stz,rayDstByte,rayDstByte);
          
          nRoundedStrBytes = nWordRoundedStrBytes(nStrSize);
          ayDstByte += nRoundedStrBytes;
          aiCellValue = (i8*)ayDstByte;
          *aiCellValue = nStrSize;
          ayDstByte += 8;
        }
        else {
          RASSERT(0);
        }
        
        break;
        
      case MC_STRING_x4: // string x 4 -- -- -- -- -- -- -- -- -- -- -- -- -- --
        nStrSize = STRx4(uLow);
        
        if (SI_CELLARRAY_ADR == (eSI)ieStrInflateRule) {
          aacCellValue = (char**)ayDataByte;
          *aacCellValue = (char*)ayDataByte;
          ayDstByte += 8;
          
          ayDataByte += nStrSize;
        }
        else
        if (SI_STRAREA_ADR == (eSI)ieStrInflateRule) {
          
          memcpy(acStrArea,ayDataByte,(size_t)nStrSize);
          ayDataByte += nStrSize;
          
          aacCellValue = (char**)ayDstByte;
          *aacCellValue = acStrArea;
          ayDstByte += 8;
          acStrArea += nStrSize;
        }
        else
        if (SI_CELLARRAY_PST == (eSI)ieStrInflateRule) {
          memcpy(ayDstByte,ayDataByte,(size_t)nStrSize);
          ayDataByte += nStrSize;
          
          //vTrace("SI_CELLARRAY_PST : %p %s\n"_Stz,rayDstByte,rayDstByte);
          
          nRoundedStrBytes = nWordRoundedStrBytes(nStrSize);
          ayDstByte += nRoundedStrBytes;
          aiCellValue = (i8*)ayDstByte;
          *aiCellValue = nStrSize;
          ayDstByte += 8;
        }
        else {
          RASSERT(0);
        }
        
        break;
        
      case MC_STRING_x8: // string x 8 -- -- -- -- -- -- -- -- -- -- -- -- -- --
        nStrSize = STRx8(uLow);
        
        if (SI_CELLARRAY_ADR == (eSI)ieStrInflateRule) {
          aacCellValue = (char**)ayDstByte;
          *aacCellValue = (char*)ayDataByte;
          ayDstByte += 8;
          
          ayDataByte += nStrSize;
        }
        else
        if (SI_STRAREA_ADR == (eSI)ieStrInflateRule) {
          
          memcpy(acStrArea,ayDataByte,(size_t)nStrSize);
          ayDataByte += nStrSize;
          
          aacCellValue = (char**)ayDstByte;
          *aacCellValue = acStrArea;
          ayDstByte += 8;
          acStrArea += nStrSize;
        }
        else
        if (SI_CELLARRAY_PST == (eSI)ieStrInflateRule) {
          memcpy(ayDstByte,ayDataByte,(size_t)nStrSize);
          ayDataByte += nStrSize;
          
          //vTrace("SI_CELLARRAY_PST : %p %s\n"_Stz,rayDstByte,rayDstByte);
          
          nRoundedStrBytes = nWordRoundedStrBytes(nStrSize);
          ayDstByte += nRoundedStrBytes;
          aiCellValue = (i8*)ayDstByte;
          *aiCellValue = nStrSize;
          ayDstByte += 8;
        }
        else {
          RASSERT(0);
        }
        
        break;
        
      case MC_STRING_TAIL: // string in tail -- -- -- -- -- -- -- -- -- -- -- --
        u_Tail_rel = *(u2*)ayDataByte;
        aTailLine = aHeadLine->aGetTailRel(u_Tail_rel);
        
        nStrSize = aTailLine->w8_1B(+ C_U_1B);
        if (SI_STRAREA_ADR == (eSI)ieStrInflateRule) {
          
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
        
      case MC_BLOB_0: // blob type 0 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
        RASSERT(0);
        break;
        
      case MC_BLOB_1: // blob type 1 - -- -- -- -- -- -- -- -- -- -- -- -- -- --
        RASSERT(0);
        break;
        
      case MC_NEGATIVE: // -ve digit -15..-0 -- -- -- -- -- -- -- -- -- -- -- --
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
        
      case MC_POSITIVE: // +ve digit 0..15 - -- -- -- -- -- -- -- -- -- -- -- --
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
/* print stream pix                                                           */
/*----------------------------------------------------------------------------*/
void vDev_RLU_PIX_STM_PRINT(i8** aaiDstk)
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
/* add a column to indexDefRlu[]                                              */
/*----------------------------------------------------------------------------*/
void vDev_RLU_INDEXDEF_ADDCOLUMN(i8** aaiDstk)
{
  _static_

  i8 *aiDstk = *aaiDstk;
  i8 nSize;

  // get the indexDef
  IndexDef* pIndexDef = _pIndexDef(P_PopPtr());

  // get the name of the column
  Stz* szColumnName = (Stz*)S_StrAdr();

  // get the type of the column
  eCT eColType = (eCT)I_PopInt();

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
}

/*----------------------------------------------------------------------------*/
/* new indexDef[]                                                             */
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

  LogicUnit* pLogicUnit = _pLogicUnit(aLU);

  PtnStream* pPtnStream = *pLogicUnit->m_xpPtnStream;
  pLogicUnit->m_xpOperandA = pPtnStream->pRead();
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
