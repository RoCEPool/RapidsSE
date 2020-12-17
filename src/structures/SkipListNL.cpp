/*============================================================================*/
/* SkipListNL.cpp -- Skip List                                                  */
/*============================================================================*/

#include "Resources.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SkipListNL* Resources::_pSkipList(void* aAddr)
{
  SkipListNL* pSkipList = reinterpret_cast<SkipListNL*>(aAddr);
  
  RASSERT_TYPE(pSkipList,CN_SKIPLIST);
  
  return pSkipList;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* SkipListNL::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_SKIPLIST);
 
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* SkipListNL::operator new(size_t, SkipListNL* pSkipList)
{
  return pSkipList;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void SkipListNL::operator delete(void* pSkipListAddress)
{
  SkipListNL* pSkipList = reinterpret_cast<SkipListNL*>(pSkipListAddress);
  
  HeapMem::vFree(pSkipList);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SkipListNL::SkipListNL(bool bUniqueKey, CompareFp fpCompare, bool bSlabAllocator)
: Object(CN_SKIPLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);
  
  m_pSlabAllocator = bSlabAllocator ? new SlabAllocator : nullptr;
  
  m_pHead      = new(C_SKIPLIST_LEVELS,m_pSlabAllocator) SkipNode(nullptr);
  m_bUniqueKey = bUniqueKey;
  m_fpCompare  = fpCompare;
  m_nObjects   = 0;
  m_iHeight    = 0;
  
  
  for (i8 i_Level = 0; i_Level < C_SKIPLIST_LEVELS; i_Level++)
    m_pHead->m_pRight[i_Level] = nullptr;
  
  _checkInitialised_(CN_SKIPLIST);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SkipListNL::~SkipListNL(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  //ASSERT( ! m_nObjects ); // logic check

  if (m_pSlabAllocator)
    delete m_pSlabAllocator;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* check integrity of the list                                                */
/*----------------------------------------------------------------------------*/
void SkipListNL::vCheck(void)
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
void SkipListNL::vContents(void (*fvDump)(void* pObject))
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
bool SkipListNL::bDeleteAll(void)
{
  //  bool bWasRemoved;
  //
  //  while (true)
  //  {
  //    Ent* aItem = (Ent*)pPop(&bWasRemoved,2);
  //
  //
  //    if (!aItem)
  //      break;
  //
  //    if ( ! bWasRemoved )
  //      return false;
  //
  //    //m_pKey->m_pDelObjFun(pObject);
  //    if (m_pKey->m_fDeleteObject)
  //      m_pKey->m_fDeleteObject(aItem);
  //    else
  //      tDelete(&aItem);
  //  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* test if object is in list                                                  */
/*----------------------------------------------------------------------------*/
bool SkipListNL::bExists(void* pObject)
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
void* SkipListNL::pFind(void* pObject)
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
SkipNode* SkipListNL::pFirst(i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);
  
  if (m_pHead->m_pRight[iLevel])
    return m_pHead->m_pRight[iLevel];
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* insert object into list                                                    */
/*----------------------------------------------------------------------------*/
bool SkipListNL::bInsert(void* aObjectInsert)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  bool bEqualTo;
  SkipNode* aPriorVec[C_SKIPLIST_LEVELS];
  
  // get a positioned prior vector
  SkipNode* pNode = pPosition(aObjectInsert,bEqualTo,aPriorVec);
  RASSERT(pNode); // logic check
  
  Object* pObjectInsert = (Object*)aObjectInsert;
  //RASSERT( ! pNode->m_pObject || ((Object*)pNode->m_pObject)->eClass() == pObjectInsert->eClass() );
  
  // do not insert duplicates into a list of unique keys
  if (bEqualTo && m_bUniqueKey) {
    return false;
  }
  
  // choose a random level for the new node
  i8 iNodeLevel = iRandomLevel();
  if (iNodeLevel > m_iHeight) {
    iNodeLevel = ++m_iHeight;
    aPriorVec[iNodeLevel] = m_pHead;
	}
  
  //RJH vTrace("bInsert:position7\n"_Stz);
  // create new node with inserted object
  SkipNode* pNodeInsert = new(iNodeLevel,m_pSlabAllocator) SkipNode(pObjectInsert);
  RASSERT(pNodeInsert);
  
  // put new node into the list according the prior vector for random level
  do {
    pNodeInsert->m_pRight[iNodeLevel] = aPriorVec[iNodeLevel]->m_pRight[iNodeLevel];
    aPriorVec[iNodeLevel]->m_pRight[iNodeLevel] = pNodeInsert;
	} while (--iNodeLevel >= 0);
  
  // one more object in the list
  m_nObjects++;
  
  // succesful insert
  return true;
}

/*----------------------------------------------------------------------------*/
/* get numbr of objects in list                                               */
/*----------------------------------------------------------------------------*/
i8 SkipListNL::nObjects(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  return m_nObjects;
}

/*----------------------------------------------------------------------------*/
/* pop first object from list                                                 */
/*----------------------------------------------------------------------------*/
void* SkipListNL::pPop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  SkipNode* pPopNode = nullptr;
  void* pObject = nullptr;
  
  if ( m_pHead->m_pRight[0]) {
    pPopNode = m_pHead->m_pRight[0];
    
    for (i8 i_Level = 0; i_Level <= m_iHeight; i_Level++) {
      if (m_pHead->m_pRight[i_Level] == pPopNode)
        m_pHead->m_pRight[i_Level] = pPopNode->m_pRight[i_Level];
    }
    pObject = pPopNode->m_pObject;
    delete pPopNode;
    
    m_nObjects--;
  }
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* recount number of objects in list                                          */
/*----------------------------------------------------------------------------*/
i8 SkipListNL::nRecount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);
  
  i8 nCount = 0;
  
  for (SkipNode* pNode = m_pHead->m_pRight[0];
       pNode;
       pNode = pNode->m_pRight[0])
    nCount++;
  
  ASSERT(nCount == m_nObjects);
  return nCount;
}

/*----------------------------------------------------------------------------*/
/* remove object from list                                                    */
/*----------------------------------------------------------------------------*/
bool SkipListNL::bRemove(void* pObjectRemove)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  // Remove iff UNIQUE KEY
  RASSERT(m_bUniqueKey);
  
  bool bEqualTo;
  SkipNode* aPriorVec[C_SKIPLIST_LEVELS];
  SkipNode* aPrior;
  
  SkipNode* pNodePrev = pPosition(pObjectRemove,bEqualTo,aPriorVec);
  RASSERT(pNodePrev); // logic check
  
  if ( ! bEqualTo ) {
    vTrace("SkipListNL::bRemove: failed to match\n"_Stz);
    vSkipListDump(this);
    SkipNode* pNodePrev = pPosition(pObjectRemove,bEqualTo,aPriorVec);
    return false;
  }
  
  // get node to be removed
  SkipNode* pNodeRemove = pNodePrev->m_pRight[0];
  RASSERT(pObjectRemove == pNodeRemove->m_pObject); // logic check
  
  // cut out the node being removed
  for (i8 i_Level = 0;
       i_Level <= m_iHeight &&
         (aPrior = aPriorVec[i_Level])->m_pRight[i_Level] == pNodeRemove;
       i_Level++)
    aPrior->m_pRight[i_Level] = pNodeRemove->m_pRight[i_Level];
  
  // correct the height if we have removed the last tallest node
  while ( ! m_pHead->m_pRight[m_iHeight] && m_iHeight > 0 )
    m_iHeight--;
  
  // one less object in the list
  m_nObjects--;

  // delete the node
  delete pNodeRemove;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get next node                                                              */
/*----------------------------------------------------------------------------*/
SkipNode* SkipListNL::pNext(SkipNode* pPrev, i8 iLevel)
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
           E [ ]--------------------->[ ]->[ ]----------->0
           A [ ]------>[ ]------>[ ]->[ ]->[ ]->[ ]->[ ]->0
           D [ ]->[ ]->[ ]->[ ]->[ ]->[ ]->[ ]->[ ]->[ ]->0
              1    2    3    4    5    6    7    8    9
 
 ******************************************************************************/

/*----------------------------------------------------------------------------*/
/* position object to be inserted                                             */
/*----------------------------------------------------------------------------*/

#define goRightNode(iLevel,pScan) pScan = pScan->m_pRight[iLevel];
#define goDownLevel(iLevel) iLevel--;
SkipNode* SkipListNL::pPosition(void* pObjectInsert, bool& bEqualTo, SkipNode** aPriorVec)
{
  //RJH vTrace("Adding %p head:%p\n"_Stz,pObjectInsert, m_pHead);
  bEqualTo = false;
  // start from head at known highest level
  SkipNode* pCurrentNode = m_pHead;
  i8 iCurrentLevel = m_iHeight;
  
  do {
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // inner loop: go right a number of nodes until we find
    //   ObjectInsert < ObjectNext at current level
    
    while (SkipNode* pNextNode = pCurrentNode->m_pRight[iCurrentLevel]) {
      
      //RJH vTrace("pNextNode:%p n"_Stz,pNextNode);
      //RJH vTrace("pNextNode:%s n"_Stz,aCN[pNextNode->eClass()]);
      //RJH vTrace("Object:%p\n"_Stz,pNextNode->m_pObject);
      
      // compare the inserting object with next object
      bool bGreaterEqual = m_fpCompare(pObjectInsert, pNextNode->m_pObject, bEqualTo);
      
      if (bGreaterEqual) {
        // pObjectInsert is >= pObjectNext
        //RJH vTrace("  %p after %p (level %lld)\n"_Stz,
        //RJH        pObjectInsert,pNextNode->m_pObject,iCurrentLevel);
        
        // go right one node
        goRightNode(iCurrentLevel,pCurrentNode);
      }
      else {
        // pObjectInsert is < pObjectNext or pObjectNext is null
        //RJH vTrace("  %p before %p (level %lld)\n"_Stz,
        //RJH        pObjectInsert,pNextNode->m_pObject,iCurrentLevel);
        
        break;
      }
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // outer loop: now having gone right as far as we go down one single level
    
    // first record current node as a revision point
    if (aPriorVec)
      aPriorVec[iCurrentLevel] = pCurrentNode;
    
    //RJH vTrace("going down 1 level\n"_Stz);
    
    // now go down one level
    goDownLevel(iCurrentLevel);
    
    // and repeat till we have a complete revision vector
  } while (iCurrentLevel >= 0);
  
  return pCurrentNode;
}

/*----------------------------------------------------------------------------*/
/* choose a random level for new node                                         */
/*----------------------------------------------------------------------------*/
i8 SkipListNL::iRandomLevel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  i8 iRandomBits = RndLib::iInteger(256*256-1);
  i8 _BitsInRandom = 16;
  i8 iRandomsLeft = _BitsInRandom/2;
  
  i8 level = 0;
  i8 b;
  
  do {
    b = iRandomBits & 3;
    if (!b) level++;
    iRandomBits>>=2;
    if (--iRandomsLeft == 0) {
      iRandomBits = (int)RndLib::iInteger(256*256-1);
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
Footer* SkipListNL::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST);
  
  return &m_xFooter;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

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

void Resources::vSkipListDump(SkipListNL* pSkipList)
{
  vTrace("----------------------------\n"_Stz);
  
  vTrace("HEIGHT %lld HEAD 0x%llx\n"_Stz,pSkipList->m_iHeight,pSkipList->m_pHead);
  for (i8 i_Level = 0; i_Level < C_SKIPLIST_LEVELS; i_Level++)
    if (pSkipList->m_pHead->m_pRight[i_Level])
      vTrace("Level %lld: Node %p : %p (L%lld)\n"_Stz,i_Level, pSkipList->m_pHead->m_pRight[i_Level]);
           
  i8 iLevel = pSkipList->m_iHeight;
  do {
    vTrace("LEVEL %lld\n"_Stz, iLevel);
    
    SkipNode* pNode = pSkipList->pFirst(iLevel);
    while (pNode) {
      i8 i = (i8)pNode->m_pObject;
      vTrace("Node %p : %p (L%lld)\n"_Stz,pNode,(i8)pNode->m_pObject, pNode->m_nRight);
      pNode = pSkipList->pNext(pNode, iLevel);
      
    }
  } while (--iLevel >= 0);
  vTrace("============================\n"_Stz);
  vTrace("\n"_Stz);
}

void Resources::vSkipListTest(void)
{
  vTrace("SkipListNL Tests\n"_Stz);
  vTrace("\n"_Stz);

  SkipListNL* pSkipList = new SkipListNL(C_UNQIUE_KEY,bI8Compare);
  
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
  
  pSkipList->bRemove((void*)4); vSkipListDump(pSkipList);
  pSkipList->bRemove((void*)5); vSkipListDump(pSkipList);
  pSkipList->bRemove((void*)6); vSkipListDump(pSkipList);
  
  pSkipList->bInsert((void*)6); vSkipListDump(pSkipList);
  pSkipList->bInsert((void*)4); vSkipListDump(pSkipList);
  pSkipList->bInsert((void*)5); vSkipListDump(pSkipList);
  
  vSkipListDump(pSkipList);
  
  return;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
