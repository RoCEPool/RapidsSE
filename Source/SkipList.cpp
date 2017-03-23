/*============================================================================*/
/* SkipList.cpp -- Skip List                                                  */
/*============================================================================*/

#include "Structures.h"

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
SkipList::SkipList(bool bIsUniqueKey, compare fCompare)
: Object(CN_SKIPLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);
  
  m_iObjects = 0;
  m_iHeight  = 0;
  m_bIsUniqueKey = bIsUniqueKey;
  m_fCompare = fCompare;
  m_pHead    = new(C_SKIPLIST_LEVELS) SkipNode(nullptr);
  m_pTail    = new((i4)0) SkipNode(nullptr);
  
  for (i4 l = 0; l < C_SKIPLIST_LEVELS; l++)
    m_pHead->m_pForward[l] = m_pTail;
  
  m_pTail->m_pForward[0] = nullptr;
  
  _checkInitialised_(CN_SKIPLIST);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SkipList::~SkipList()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  ASSERT( ! m_iObjects ); // logic check

// alka
//  tDelete(&m_pKey);
//  
//  tDelete(&m_pHead);
//  tDelete(&m_pTail);
}

/*----------------------------------------------------------------------------*/
/* test if object is in list                                                  */
/*----------------------------------------------------------------------------*/
bool SkipList::bExists(void* pObject)
{  
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  bool bEqualTo;
  
  SkipNode* pNode = pPosition(pObject,&bEqualTo);
  ASSERT(pNode); // logic check
  
  if (bEqualTo && pObject == pNode->m_pForward[0]->m_pObject)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* insert object into list                                                    */
/*----------------------------------------------------------------------------*/
bool SkipList::bInsert(void* pObject)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  bool bEqualTo;
  SkipNode* pRevise[C_SKIPLIST_LEVELS];
  
  SkipNode* pNode = pPosition(pObject,&bEqualTo,pRevise);
  ASSERT(pNode); // logic check
  
  if (bEqualTo && m_bIsUniqueKey)
    return false;
  
  i4 l = iLevel();
  if (l > m_iHeight) {
    l = ++m_iHeight;
    pRevise[l] = m_pHead;
	}
  
  SkipNode* pNew = new(l) SkipNode(pObject);
 
  do {
    pNew->m_pForward[l] = pRevise[l]->m_pForward[l];
    pRevise[l]->m_pForward[l] = pNew;
	} while (--l >= 0);
  
  m_iObjects++;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get numbr of objects in list                                               */
/*----------------------------------------------------------------------------*/
i8 SkipList::iObjects()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  return m_iObjects;
}

/*----------------------------------------------------------------------------*/
/* pop top/first object from list                                             */
/*----------------------------------------------------------------------------*/
void* SkipList::pPop(bool* bWasRemoved, i8 iFrom)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  SkipNode* pFirstNode = pFirst(0);
  if (!pFirstNode)
    return nullptr;
  
  void* pObject = pFirstNode->m_pObject;
  if (!pObject)
    return nullptr;
  
  *bWasRemoved = bRemove(pObject);
  //ASSERT(b); // logic check
  if ( ! *bWasRemoved )
    vError("pPop: failed to remove; from %lld"_Str,iFrom);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* remove object from list                                                    */
/*----------------------------------------------------------------------------*/
bool SkipList::bRemove(void* pObject)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  i4 l;
  bool bEqualTo;
  SkipNode* pRevision[C_SKIPLIST_LEVELS];
  SkipNode* pRevise;
  
  SkipNode* pNode = pPosition(pObject,&bEqualTo,pRevision);
  ASSERT(pNode); // logic check
  
  if ( ! bEqualTo ) {
    vTrace("SkipList::bRemove: failed to match"_Str);
    return false;
  }
  
  pNode = pNode->m_pForward[0];
  ASSERT(pObject == pNode->m_pObject); // logic check
  
  i4 iNewHeight = m_iHeight;
  for (l = 0;
       l <= iNewHeight && (pRevise = pRevision[l])->m_pForward[l] == pNode;
       l++)
    pRevise->m_pForward[l] = pNode->m_pForward[l];
  
  while (m_pHead->m_pForward[iNewHeight] == m_pTail && iNewHeight > 0)
    iNewHeight--;
  
  m_iHeight = iNewHeight;
  
  // alka
  //tDelete(&pNode);
  
  m_iObjects--;

  return true;
}

/*----------------------------------------------------------------------------*/
/* find first matching object                                                 */
/*----------------------------------------------------------------------------*/
void* SkipList::pFindFirst(void* pObject)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  bool bEqualTo;
  
  SkipNode* pNode = pPosition(pObject,&bEqualTo);
  ASSERT(pNode); // logic check
  
  if (pNode->m_pForward[0] == m_pTail)
    return nullptr;
  else if (bEqualTo)
    return pNode->m_pForward[0]->m_pObject;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dump the list                                                              */
/*----------------------------------------------------------------------------*/
void SkipList::vItemDump(void (*fvDump)(void* pObject))
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  for (SkipNode* pScan = m_pHead->m_pForward[0];
       pScan && pScan != m_pTail;
       pScan = pScan->m_pForward[0]) {
    fvDump(pScan->m_pObject);
  }
}

/*----------------------------------------------------------------------------*/
/* check integrity of the list                                                */
/*----------------------------------------------------------------------------*/
void SkipList::vCheck()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  for (SkipNode* pNode = m_pHead->m_pForward[0];
       pNode && pNode != m_pTail;
       pNode = pNode->m_pForward[0])
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
/* get first node                                                             */
/*----------------------------------------------------------------------------*/
SkipNode* SkipList::pFirst(i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  if (m_pHead->m_pForward[iLevel] != m_pTail)
    return m_pHead->m_pForward[iLevel];
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get next node                                                              */
/*----------------------------------------------------------------------------*/
SkipNode* SkipList::pNext(SkipNode* pPrev, i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  if (pPrev->m_pForward[iLevel] != m_pTail)
    return pPrev->m_pForward[iLevel];
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get head bookend                                                           */
/*----------------------------------------------------------------------------*/
SkipNode* SkipList::pHead()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  return m_pHead;
}

/*----------------------------------------------------------------------------*/
/* get tail bookend                                                           */
/*----------------------------------------------------------------------------*/
SkipNode* SkipList::pTail()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  return m_pTail;
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
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* choose a random level for new node                                         */
/*----------------------------------------------------------------------------*/
i4 SkipList::iLevel()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  i4 iRandomBits = Random::iRandom(256*256-1);
  i4 _BitsInRandom = 16;
  i4 iRandomsLeft = _BitsInRandom/2;
  
  i4 level = 0;
  i4 b;
  
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
/* position search on node just prior to lower bound                          */
/*----------------------------------------------------------------------------*/
SkipNode* SkipList::pPosition(void* pObject, bool* bEqualTo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  SkipNode* pNext = nullptr;
  
  i4 l = m_iHeight;
  SkipNode* pScan = m_pHead;
  //Node* pPrev = nullptr;
  do
  {
    while (pNext = pScan->m_pForward[l],
           pNext != m_pTail
           && m_fCompare(pObject,pNext->m_pObject,bEqualTo)) {
      //pPrev = pScan;
      pScan = pNext;
    }
    
  } while(--l >= 0);
  
  return pScan;
}

/*----------------------------------------------------------------------------*/
/* position search on node just prior to lower bound                          */
/*----------------------------------------------------------------------------*/
SkipNode* SkipList::pPosition(void* pObjectInsert, bool* bEqualTo, SkipNode** pRevise)
/*
{
  SkipNode* pScan = m_pHead;
  SkipNode* pPrev;
  
  // start with the highest level
  i8 iLevel = m_iHeight;
  pPrev = pScan;
  goRightLevel(pScan);
  
  do {
    
    void* pObjectScan = pScan->m_pObject;

    if (!pObjectScan) {
      // record cursor before going down a level
      pRevise[iLevel] = pPrev;
      pScan = pPrev;
      // no right pointer at iLevel
      goDownLevel(iLevel);
      continue;
    }
    
    // compare node to be inserted with ScanNode
    bool bGreater = m_fCompare(pObjectInsert, pObjectScan, bEqualTo);
    
    if (bGreater) {
      // pObjectInsert is >= pObjectScan
      pPrev = pScan;
      goRightLevel(pScan);
      continue;
    }
    
    // record cursor before going down a level
    pRevise[iLevel] = pPrev;
    pScan = pPrev;
    goDownLevel(iLevel);
    
  } while (iLevel >= 0);
  
  
  return pScan;

}
*/

/*
 {
 _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);
 
 SkipNode* pNext = nullptr;
 
 *bEqualTo = false;
 
 i4 l = m_iHeight;
 SkipNode* pScan = m_pHead;
 //Node* pPrev = nullptr;
 do
 {
   while (pNext = pScan->m_pForward[l],
   pNext != m_pTail
   && m_fCompare(pObjectInsert,pNext->m_pObject,bEqualTo))
   {
   //pPrev = pScan;
   pScan = pNext;
 }
 
 pRevise[l] = pScan;
 
 } while(--l >= 0);
 
 return pScan;
 }
 
 */

#define goRightLevel(pScan) pScan = pScan->m_pForward[iLevel];
#define goDownLevel(iLevel) iLevel--;

{
  SkipNode* pNext = m_pHead;
  SkipNode* pScan;
  
  // start with the highest level
  i8 iLevel = m_iHeight;
  pScan = pNext;
  goRightLevel(pNext);
  
  do {
    
    void* pObjectNext = pNext->m_pObject;
    
    if (!pObjectNext) {
      // record cursor before going down a level
      pRevise[iLevel] = pScan;
      
      // move backward
      pNext = pScan;
      
      // no right pointer at iLevel
      goDownLevel(iLevel);
      continue;
    }
    
    // compare node to be inserted with NextNode
    bool bGreater = m_fCompare(pObjectInsert, pObjectNext, bEqualTo);
    
    if (bGreater) {
      // pObjectInsert is >= pObjectNext
      
      // save
      pScan = pNext;
      
      goRightLevel(pNext);
    }
    else {
      // pObjectInsert is < pObjectNext
      
      // record cursor before going down a level
      pRevise[iLevel] = pScan;
      
      // move backward
      pNext = pScan;
      
      goDownLevel(iLevel);
    }
    
  } while (iLevel >= 0);
  
  
  return pScan;
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
bool bI8Compare(void* pObjectInsert, void* pObjectScan, bool* bMatch)
{
  i8 iInsertEntry = (i8)pObjectInsert;
  i8 iScanEntry   = (i8)pObjectScan;
  
  if (iInsertEntry == iScanEntry)
    *bMatch = true;
  
  if (iInsertEntry > iScanEntry)
    return true;
  else
    return false;
}

void Structures::vSkipListTest(void)
{
  SkipList* pSkipList = new SkipList(true,bI8Compare);
  
  
  pSkipList->bInsert((void*)3);
  pSkipList->bInsert((void*)1);

  pSkipList->bInsert((void*)8);

  pSkipList->bInsert((void*)2);
  
  pSkipList->bInsert((void*)4);
  
  pSkipList->bInsert((void*)7);
  
  pSkipList->bInsert((void*)5);

  pSkipList->bInsert((void*)6);
  pSkipList->bInsert((void*)9);

  
  i8 iLevel = pSkipList->m_iHeight;
  do {
    vTrace("LEVEL %lld\n"_Str, iLevel);
    
    SkipNode* pNode = pSkipList->pFirst(iLevel);
    while (pNode) {
      i8 i = (i8)pNode->m_pObject;
      vTrace("Node %p : %lld (L%lld)\n"_Str,pNode,(i8)pNode->m_pObject, pNode->m_nForward);
      pNode = pSkipList->pNext(pNode, iLevel);
    }
  } while (--iLevel >= 0);
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
