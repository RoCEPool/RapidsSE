/*============================================================================*/
/* PdshUtils.cpp -- Utility structures and functions for Pds                  */
/*============================================================================*/
#include "BASE.h"
#include "PdsUtils.hpp"
#include "LglBufSet.hpp"
#include "Pot.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compare two i8 items                                                       */
/*----------------------------------------------------------------------------*/
i8 PdsUtils::iIntCompare(void* aItem_l, void* aItem_r)
{
  _static_
  i8 iKey_l = *(i8*) aItem_l;
  i8 iKey_r = *(i8*) aItem_r;

  // do the node keys match?
  if (iKey_l == iKey_r)
    return 0;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (iKey_l >= iKey_r)
    return 1;
  else
    return -1;
}

/*----------------------------------------------------------------------------*/
/* compare two nodes                                          */
/*----------------------------------------------------------------------------*/
i8 PdsUtils::iLglBufSetKeyCompare(void* aItem_l, void* aItem_r)
{
  _static_

  LglBufSetItem* pKey_l = (LglBufSetItem*) aItem_l;
  LglBufSetItem* pKey_r = (LglBufSetItem*) aItem_r;

  // do the node names match?
  if (pKey_l->iLglPtn == pKey_r->iLglPtn) {
    if (pKey_l->iSynthKey == pKey_r->iSynthKey) {
      return 0;
    }
    else {
      if (pKey_l->iSynthKey >= pKey_r->iSynthKey)
        return 1;
      else
        return -1;
    }
  } else {
    if (pKey_l->iLglPtn >= pKey_r->iLglPtn)
      return 1;
    else
      return -1;
  }
}

/*----------------------------------------------------------------------------*/
/* compare two nodes                                                          */
/*----------------------------------------------------------------------------*/
i8 PdsUtils::iPotPdsItemKeyCompare(void* aItem_l, void* aItem_r)
{
  _static_

  PotPdsItem* pKey_l = (PotPdsItem*) aItem_l;
  PotPdsItem* pKey_r = (PotPdsItem*) aItem_r;

  // do the node names match?
  if (pKey_l->iLglPtn == pKey_r->iLglPtn) {
    if (pKey_l->iSynthKey == pKey_r->iSynthKey) {
      return 0;
    }
    else {
      if (pKey_l->iSynthKey >= pKey_r->iSynthKey)
        return 1;
      else
        return -1;
    }
  } else {
    if (pKey_l->iLglPtn >= pKey_r->iLglPtn)
      return 1;
    else
      return -1;
  }
}

/*----------------------------------------------------------------------------*/
/* compare two nodes                                                          */
/*----------------------------------------------------------------------------*/
i8 PdsUtils::iHashMapEntryKeyCompare(void* aItem_l, void* aItem_r)
{
  _static_

  HashMapEntry* pKey_l = (HashMapEntry*) aItem_l;

  // do the node keys match?
  if (pKey_l->m_pKeyObject == aItem_r)
    return 0;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (pKey_l->m_pKeyObject >= aItem_r)
    return 1;
  else
    return -1;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
