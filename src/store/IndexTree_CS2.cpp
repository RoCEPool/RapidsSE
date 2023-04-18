/*============================================================================*/
/* IndexTree_CS2.cpp -- Index Tree                                            */
/*============================================================================*/

#include "BASE.h"

#include "IndexDef.hpp"

#include "Pay.hpp"
#include "Key.hpp"
#include "Ent.hpp"

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "TailLine.hpp"

#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "RowInflator.hpp"

#include "EntBufSorter.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"



/*----------------------------------------------------------------------------*/
/* find an entry in the indexTree                                         CS2 */
/*----------------------------------------------------------------------------*/
Ent* IndexTree::aFindEntry_CS2(IndexTree* pIndexTree, Key* aKey)
{
  EntBuffer* pEntBuffer = nullptr;
  eKC eCmp;

  //pIndexTree->vDump(false);

  ///////////////////////////////////////////////////////////////////////////

  IndexDef* pIndexDef = pIndexTree->pIndexDef();

  ///////////////////////////////////////////////////////////////////////////

  Ent* aEnt = pIndexTree->aPositionPriorEntry(aKey, &pEntBuffer);
  if ( !aEnt )
    return nullptr;

  ///////////////////////////////////////////////////////////////////////////

//  aEnt = EntBuffer::pMoveToNextEntry(aEnt, &pEntBuffer);
//  if ( !aEnt )
//    return nullptr;

  /*-----------------------------------------------------------------------*/

  // move to next H0 entry (across buffers if necessary)

  if (! (aEnt = pEntBuffer->aNextEntry(aEnt,0)) ) {

    // move to the next buffer
    if (! (pEntBuffer = pEntBuffer->pNextEntBuffer()) )
      return nullptr;

    // move to first entry in that buffer
    if (! (aEnt = pEntBuffer->aNextEntry(pEntBuffer->aSkipRoots(),0)) )
      return nullptr;
  }

  ///////////////////////////////////////////////////////////////////////////

  Key* aEntKey = aEnt->aKey();

  if (KE_GENERAL == aEnt->eKind()) {
    eCmp = Key::eCompare(pIndexDef, aEntKey,aKey);
  }
  else {
    i8 iEntVal = *(i8 *) aEntKey;
    i8 iWrkVal = *(i8 *) aKey;

    if (iEntVal < iWrkVal)
      eCmp = KC_BEFORE;
    else if (iEntVal == iWrkVal)
      eCmp = KC_SAME;
    else
      eCmp = KC_AFTER;
  }

  if (eCmp == KC_SAME)
    return aEnt;
  else
    return nullptr;
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
