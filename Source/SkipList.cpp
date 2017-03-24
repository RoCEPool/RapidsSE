/*============================================================================*/
/* SkipList.cpp -- Skip List                                                  */
/*============================================================================*/

#include "Structures.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SkipList* Structures::_pSkipList(void* aAddr)
{
  SkipList* pSkipList = reinterpret_cast<SkipList*>(aAddr);
  
  RASSERT( !pSkipList || CN_SKIPLIST == pSkipList->eClass() );
  
  return pSkipList;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* SkipList::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = Heap::pAllocate(CN_SKIPLIST);
 
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* SkipList::operator new(size_t iSize, SkipList* pSkipList)
{
  RASSERT(iSize);
  
  return pSkipList;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void SkipList::operator delete(void* pSkipListAddress)
{
  SkipList* pSkipList = reinterpret_cast<SkipList*>(pSkipListAddress);
  
  Heap::vFree(pSkipList);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SkipList::SkipList(bool bUniqueKey, cmpFun fCompare)
: Object(CN_SKIPLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);
  
  m_pHead      = new(C_SKIPLIST_LEVELS) SkipNode(nullptr);
  m_bUniqueKey = bUniqueKey;
  m_fCompare   = fCompare;
  m_nObjects   = 0;
  m_iHeight    = 0;
  
  for (i8 iLevel = 0; iLevel < C_SKIPLIST_LEVELS; iLevel++)
    m_pHead->m_pRight[iLevel] = nullptr;
  
  _checkInitialised_(CN_SKIPLIST);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SkipList::~SkipList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  ASSERT( ! m_nObjects ); // logic check
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* check integrity of the list                                                */
/*----------------------------------------------------------------------------*/
void SkipList::vCheck(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);
  
  for (SkipNode* pNode = m_pHead->m_pRight[0];
       pNode;
       pNode = pNode->m_pRight[0])
    {
    //    if (n->m_pKey->iElem(0) != n->m_pKey->pObject()->iAttr(0))
    //      vReportTrace("key <> OBJECT error\n");
    //
    //    if (n->m_pKey->iElem(0) > n->m_pNext[0]->m_pKey->iElem(0)) {
    //      vReportTrace("sequence error\n");
    //    }
    }
}

/*----------------------------------------------------------------------------*/
/* list contents                                                              */
/*----------------------------------------------------------------------------*/
void SkipList::vContents(void (*fvDump)(void* pObject))
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);
  
  for (SkipNode* pScan = m_pHead->m_pRight[0];
       pScan;
       pScan = pScan->m_pRight[0]) {
    fvDump(pScan->m_pObject);
  }
}

/*----------------------------------------------------------------------------*/
/* delete all objects                                                         */
/*----------------------------------------------------------------------------*/
bool SkipList::bDeleteAll(void)
{
  //  bool bWasRemoved;
  //
  //  while (true)
  //  {
  //    Entry* pEntry = (Entry*)pPop(&bWasRemoved,2);
  //
  //
  //    if (!pEntry)
  //      break;
  //
  //    if ( ! bWasRemoved )
  //      return false;
  //
  //    //m_pKey->m_pDelObjFun(pObject);
  //    if (m_pKey->m_fDeleteObject)
  //      m_pKey->m_fDeleteObject(pEntry);
  //    else
  //      tDelete(&pEntry);
  //  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get each object                                                            */
/*----------------------------------------------------------------------------*/
void* SkipList::pEach(SkipCursor* pCursor)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);
  
  if ( ! pCursor->m_pCurrentNode )
    pCursor->m_pCurrentNode = pFirst(0);
  else
    pCursor->m_pCurrentNode = pNext(pCursor->m_pCurrentNode, 0);
  
  if (pCursor->m_pCurrentNode)
    return pCursor->m_pCurrentNode->m_pObject;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* test if object is in list                                                  */
/*----------------------------------------------------------------------------*/
bool SkipList::bExists(void* pObject)
{  
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  bool bEqualTo;
  
  SkipNode* pNode = pPosition(pObject,bEqualTo,nullptr);
  ASSERT(pNode); // logic check
  
  if (bEqualTo && pObject == pNode->m_pRight[0]->m_pObject)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* find first matching object                                                 */
/*----------------------------------------------------------------------------*/
void* SkipList::pFind(void* pObject)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);
  
  bool bEqualTo;
  
  SkipNode* pNode = pPosition(pObject,bEqualTo,nullptr);
  ASSERT(pNode); // logic check
  
  if ( ! pNode->m_pRight[0] )
    return nullptr;
  else if (bEqualTo)
    return pNode->m_pRight[0]->m_pObject;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get first node                                                             */
/*----------------------------------------------------------------------------*/
SkipNode* SkipList::pFirst(i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);
  
  if (m_pHead->m_pRight[iLevel])
    return m_pHead->m_pRight[iLevel];
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* insert object into list                                                    */
/*----------------------------------------------------------------------------*/
bool SkipList::bInsert(void* pObject)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  bool bEqualTo;
  SkipNode* pReviseVec[C_SKIPLIST_LEVELS];
  
  SkipNode* pNode = pPosition(pObject,bEqualTo,pReviseVec);
  ASSERT(pNode); // logic check
  
  if (bEqualTo && m_bUniqueKey)
    return false;
  
  i8 iLevel = iRandomLevel();
  if (iLevel > m_iHeight) {
    iLevel = ++m_iHeight;
    pReviseVec[iLevel] = m_pHead;
	}
  
  SkipNode* pNew = new(iLevel) SkipNode(pObject);
 
  do {
    pNew->m_pRight[iLevel] = pReviseVec[iLevel]->m_pRight[iLevel];
    pReviseVec[iLevel]->m_pRight[iLevel] = pNew;
	} while (--iLevel >= 0);
  
  m_nObjects++;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get numbr of objects in list                                               */
/*----------------------------------------------------------------------------*/
i8 SkipList::nObjects(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  return m_nObjects;
}

/*----------------------------------------------------------------------------*/
/* remove object from list                                                    */
/*----------------------------------------------------------------------------*/
bool SkipList::bRemove(void* pObject)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  bool bEqualTo;
  SkipNode* pReviseVec[C_SKIPLIST_LEVELS];
  SkipNode* pRevise;
  
  SkipNode* pNode = pPosition(pObject,bEqualTo,pReviseVec);
  ASSERT(pNode); // logic check
  
  if ( ! bEqualTo ) {
    vTrace("SkipList::bRemove: failed to match"_Str);
    return false;
  }
  
  pNode = pNode->m_pRight[0];
  ASSERT(pObject == pNode->m_pObject); // logic check
  
  i8 iNewHeight = m_iHeight;
  for (i8 l = 0;
       l <= iNewHeight && (pRevise = pReviseVec[l])->m_pRight[l] == pNode;
       l++)
    pRevise->m_pRight[l] = pNode->m_pRight[l];
  
  while ( ! m_pHead->m_pRight[iNewHeight] && iNewHeight > 0)
    iNewHeight--;
  
  m_iHeight = iNewHeight;
  
  // alka
  //tDelete(&pNode);
  
  m_nObjects--;

  return true;
}

/*----------------------------------------------------------------------------*/
/* get next node                                                              */
/*----------------------------------------------------------------------------*/
SkipNode* SkipList::pNext(SkipNode* pPrev, i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  if (pPrev->m_pRight[iLevel])
    return pPrev->m_pRight[iLevel];
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- private: ----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/******************************************************************************
 
                                 A Skip List
                                 ===========
 
           H
           E [ ]--------------------->[ ]->[ ]---------------->0
           A [ ]------>[ ]------>[ ]->[ ]->[ ]->[ ]->[ ]->[ ]->0
           D [ ]->[ ]->[ ]->[ ]->[ ]->[ ]->[ ]->[ ]->[ ]->[ ]->0
           1    2    3    4    5    6    7    8    9
 
 ******************************************************************************/

/*----------------------------------------------------------------------------*/
/* position object to be inserted                                             */
/*----------------------------------------------------------------------------*/

#define goRightNode(iLevel,pScan) pScan = pScan->m_pRight[iLevel];
#define goDownLevel(iLevel) iLevel--;
SkipNode* SkipList::pPosition(void* pObjectInsert, bool& bEqualTo, SkipNode** pReviseVec)
{
  vTrace("Adding %lld\n"_Str,(i8)pObjectInsert);
  
  // start from head at known highest level
  SkipNode* pCurrentNode = m_pHead;
  i8 iCurrentLevel = m_iHeight;
  
  do {
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // inner loop: go right a number of nodes until we find
    //   ObjectInsert < ObjectNext at current level
    
    while (SkipNode* pNextNode = pCurrentNode->m_pRight[iCurrentLevel]) {
      
      // compare the inserting object with next object
      bool bGreaterEqual = m_fCompare(pObjectInsert, pNextNode->m_pObject, bEqualTo);
      
      if (bGreaterEqual) {
        // pObjectInsert is >= pObjectNext
        vTrace("  %lld after %lld (level %lld)\n"_Str,
               (i8)pObjectInsert,(i8)pNextNode->m_pObject,iCurrentLevel);
        
        // go right one node
        goRightNode(iCurrentLevel,pCurrentNode);
      }
      else {
        // pObjectInsert is < pObjectNext or pObjectNext is null
        vTrace("  %lld before %lld (level %lld)\n"_Str,
               (i8)pObjectInsert,(i8)pNextNode->m_pObject,iCurrentLevel);
        
        break;
      }
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // outer loop: now having gone right as far as we go down one single level
    
    // first record current node as a revision point
    if (pReviseVec)
      pReviseVec[iCurrentLevel] = pCurrentNode;
    
    // now go down one level
    goDownLevel(iCurrentLevel);
    
    // and repeat till we have a complete revision vector
  } while (iCurrentLevel >= 0);
  
  return pCurrentNode;
}

/*----------------------------------------------------------------------------*/
/* choose a random level for new node                                         */
/*----------------------------------------------------------------------------*/
i8 SkipList::iRandomLevel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  i8 iRandomBits = Random::iRandom(256*256-1);
  i8 _BitsInRandom = 16;
  i8 iRandomsLeft = _BitsInRandom/2;
  
  i8 level = 0;
  i8 b;
  
  do {
    b = iRandomBits & 3;
    if (!b) level++;
    iRandomBits>>=2;
    if (--iRandomsLeft == 0) {
      iRandomBits = (int)Random::iRandom(256*256-1);
      iRandomsLeft = _BitsInRandom/2;
    }
  } while (!b);
  
  return (level > C_SKIPLIST_LEVELS - 1) ? C_SKIPLIST_LEVELS - 1: level;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SkipList::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST);
  
  return &m_xFooter;
}


/*----------------------------------------------------------------------------*/
/*-- Test --------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
bool bI8Compare(void* pObjectInsert, void* pObjectScan, bool& bMatch)
{
  i8 iInsertEntry = (i8)pObjectInsert;
  i8 iScanEntry   = (i8)pObjectScan;
  
  if ( ! pObjectScan )
    return false;
  
  if (iInsertEntry == iScanEntry)
    bMatch = true;
  else
    bMatch = false;
  
  if (iInsertEntry > iScanEntry)
    return true;
  else
    return false;
}

void Structures::vSkipListDump(SkipList* pSkipList)
{
  vTrace("----------------------------\n"_Str);

  vTrace("HEAD 0x%llx\n"_Str, pSkipList->m_pHead);
  i8 iLevel = pSkipList->m_iHeight;
  do {
    vTrace("LEVEL %lld\n"_Str, iLevel);
    
    SkipNode* pNode = pSkipList->pFirst(iLevel);
    while (pNode) {
      i8 i = (i8)pNode->m_pObject;
      vTrace("Node %p : %lld (L%lld)\n"_Str,pNode,(i8)pNode->m_pObject, pNode->m_nRight);
      pNode = pSkipList->pNext(pNode, iLevel);
    }
  } while (--iLevel >= 0);
  vTrace("============================\n"_Str);
  vTrace("\n"_Str);
}

void Structures::vSkipListTest(void)
{
  vTrace("SkipList Tests\n"_Str);
  vTrace("\n"_Str);

  SkipList* pSkipList = new SkipList(true,bI8Compare);
  
  pSkipList->bInsert((void*)3); vSkipListDump(pSkipList);
  pSkipList->bInsert((void*)1); vSkipListDump(pSkipList);
  pSkipList->bInsert((void*)8); vSkipListDump(pSkipList);
  pSkipList->bInsert((void*)2); vSkipListDump(pSkipList);
  pSkipList->bInsert((void*)4); vSkipListDump(pSkipList);
  pSkipList->bInsert((void*)7); vSkipListDump(pSkipList);
  pSkipList->bInsert((void*)5); vSkipListDump(pSkipList);
  pSkipList->bInsert((void*)6); vSkipListDump(pSkipList);
  pSkipList->bInsert((void*)9); vSkipListDump(pSkipList);
  
  ASSERT(4 == (i8)pSkipList->pFind((void*)4));
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
