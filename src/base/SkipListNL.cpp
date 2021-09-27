/*============================================================================*/
/* SkipListNL.cpp -- Skip List                                                */
/*============================================================================*/

#include "BASE.h"

// !DIC_AS_SKIPLIST
//#define _VALIDATE_SKIPLIST

/*
 
                                 A Skip List
                                 ===========
 
           H
           E [ ]--------------------->[ ]->[ ]----------->0
           A [ ]------>[ ]------>[ ]->[ ]->[ ]->[ ]->[ ]->0
           D [ ]->[ ]->[ ]->[ ]->[ ]->[ ]->[ ]->[ ]->[ ]->0
              1    2    3    4    5    6    7    8    9
 
 
 */


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SkipListNL* BASE::_pSkipList(void* aAddr)
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
  void* pObject = Heaps::pAllocate(CN_SKIPLIST);
 
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
  
  Heaps::vFree(pSkipList);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SkipListNL::SkipListNL(bool bUniqueKey,
                       CompareFp fpCompare,
                       PrintFp fpPrint,
                       bool bSlabAllocator)
: Object(CN_SKIPLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);
  
  m_pSlabAllocator = bSlabAllocator ? new SlabAllocator : nullptr;

  m_bUniqueKey = bUniqueKey;
  m_fpCompare  = fpCompare;
  m_fpPrint    = fpPrint;

  m_nItems   = 0;
  m_iHeight    = 0;
  m_pHead      = new(C_SKIPLIST_LEVELS,m_pSlabAllocator) SkipNode(nullptr);

  m_pNewly     = nullptr;


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

  if (! m_pSlabAllocator)
    delete m_pHead;
  
  ASSERT(m_pSlabAllocator || ! m_nItems ); // logic check

  if (m_pSlabAllocator)
    delete m_pSlabAllocator;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* compare skip items by their address                                        */
/*----------------------------------------------------------------------------*/
bool SkipListNL::bCompareAddr(void* aInsert, void* aScan, bool& bMatch)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);
    
  if (! aScan)
    return false;
  
  if (aInsert == aScan)
    bMatch = true;
  else
    bMatch = false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // is this inserting lexicography after the scan?
  
  if (aInsert > aScan)
    return true;
  else
    return false;
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
/* find first matching object                                                 */
/*----------------------------------------------------------------------------*/
void* SkipListNL::pFind(void* pObject,
                        bool bTrace,
                        CompareFp fpCompare_alt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);
  
  bool bEqualTo;
  
  SkipNode* pNode = pPosition(pObject,
                              bEqualTo,
                              nullptr,
                              bTrace,
                              fpCompare_alt ? fpCompare_alt : m_fpCompare
                              );
  ASSERT(pNode); // logic check
  
  if ( ! pNode->m_pRight[0] )
    return nullptr;
  else if (bEqualTo)
    return pNode->m_pRight[0]->m_pObject;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find first matching object                                                 */
/*----------------------------------------------------------------------------*/
void* SkipListNL::pFindByKey(
                             std::function<bool(
                                                void* aScan,
                                                bool& bFoundKey
                                                )> bfMoveRightTest,
                             
                             std::function<void(SkipNode* pNode,
                                                int i_Level
                                                )> fPrintNode
                             )
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);
  
  SkipNode* pNode = pLocateKey(bfMoveRightTest);

  if (pNode)
    return pNode->m_pObject;
  
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
bool SkipListNL::bInsert(void* aObjectInsert, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  bool bEqualTo;
  SkipNode* aPriorVec[C_SKIPLIST_LEVELS];
  
  // get a positioned prior vector
  SkipNode* pNode =
    pPosition(aObjectInsert,bEqualTo,aPriorVec,bTrace,m_fpCompare);
  RASSERT(pNode); // logic check
  
  Object* pObjectInsert = (Object*)aObjectInsert;
  //RASSERT( ! pNode->m_pObject || ((Object*)pNode->m_pObject)->eGetClassNo() == pObjectInsert->eGetClassNo() );
  
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
  
  if (bTrace) {
    vTrace("\niNodeLevel %lld\n"_Stz,iNodeLevel);
  }
  
  //RJH vTrace("bInsert:position7\n"_Stz);
  // create new node with inserted object
  m_pNewly = new(iNodeLevel,m_pSlabAllocator) SkipNode(pObjectInsert);
  RASSERT(m_pNewly);
  
  // put new node into the list according the prior vector for random level
  do {
    m_pNewly->m_pRight[iNodeLevel] = aPriorVec[iNodeLevel]->m_pRight[iNodeLevel];
    aPriorVec[iNodeLevel]->m_pRight[iNodeLevel] = m_pNewly;
	} while (--iNodeLevel >= 0);
  
  // one more object in the list
  m_nItems++;
  
  
#ifdef _VALIDATE_SKIPLIST
  vValidate();
#endif
  
  
  // succesful insert
  return true;
}

/*----------------------------------------------------------------------------*/
/* insert object into list                                                    */
/*----------------------------------------------------------------------------*/
bool SkipListNL::bInsertByKey(void* aObjectInsert,
                              
                              std::function<bool(void* aScan,
                                                 bool& bFound
                                                )> bfMoveRightTest,
                              
                              std::function<void(SkipNode* pNode,
                                                 int i_Level
                                                 )> fPrintNode
                              )
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  bool bEqualTo;
  SkipNode* aPriorVec[C_SKIPLIST_LEVELS];
  
  // get a positioned prior vector
  SkipNode* pNode = pPositionPrior(bfMoveRightTest,aPriorVec,bEqualTo);
  RASSERT(pNode); // logic check
  
  Object* pObjectInsert = (Object*)aObjectInsert;
  //RASSERT( ! pNode->m_pObject || ((Object*)pNode->m_pObject)->eGetClassNo() == pObjectInsert->eGetClassNo() );
  
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
  m_pNewly = new(iNodeLevel,m_pSlabAllocator) SkipNode(pObjectInsert);
  RASSERT(m_pNewly);
  
  // put new node into the list according the prior vector for random level
  do {
    m_pNewly->m_pRight[iNodeLevel] = aPriorVec[iNodeLevel]->m_pRight[iNodeLevel];
    aPriorVec[iNodeLevel]->m_pRight[iNodeLevel] = m_pNewly;
  } while (--iNodeLevel >= 0);
  
  // one more object in the list
  m_nItems++;
  
  
#ifdef _VALIDATE_SKIPLIST
  vValidate();
#endif
  
  
  // succesful insert
  return true;
}

/*----------------------------------------------------------------------------*/
/* get numbr of objects in list                                               */
/*----------------------------------------------------------------------------*/
i8 SkipListNL::nItems(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* get most recently new'd skip node                                          */
/*----------------------------------------------------------------------------*/
SkipNode* SkipListNL::pNewly(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);
  
  return m_pNewly;
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
    
    m_nItems--;
  }
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* print contents of the skiplist                                             */
/*----------------------------------------------------------------------------*/
void SkipListNL::vPrintList(
                            std::function<void(SkipNode* pNode,
                                              int i_Level
                                              )> fPrintNode
                            )
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);
  
  vTrace("----------------------------\n"_Stz);

  vTrace("HEIGHT %lld HEAD 0x%llx\n"_Stz
         ,m_iHeight
         ,m_pHead);

  for (i8 i_Level = C_SKIPLIST_LEVELS - 1; i_Level >= 0; i_Level--) {
    if (m_pHead->m_pRight[i_Level])
      vTrace("Level %lld: Node %p\n"_Stz
             ,i_Level
             ,m_pHead->m_pRight[i_Level]);
  }
  
  for (i8 i_Level = m_iHeight; i_Level >= 0; i_Level--) {

    vTrace("LEVEL %lld\n"_Stz, i_Level);
    
    for (SkipNode* pNode = pFirst(i_Level);
         pNode;
         pNode = pNext(pNode,i_Level))  {
      fPrintNode(pNode,i_Level);
    }
  }

  vTrace("----------------------------\n"_Stz);
  vTrace("\n"_Stz);
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
  
  ASSERT(nCount == m_nItems);
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
  
  SkipNode* pNodePrev =
    pPosition(pObjectRemove,bEqualTo,aPriorVec,false,m_fpCompare);
  RASSERT(pNodePrev); // logic check
  
  if ( ! bEqualTo ) {
    vTrace("SkipListNL::bRemove: failed to match\n"_Stz);
    vSkipListDump(this);
    SkipNode* pNodePrev =
      pPosition(pObjectRemove,bEqualTo,aPriorVec,false,m_fpCompare);
    UNUSED(pNodePrev);
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
  m_nItems--;

  // delete the node
  delete pNodeRemove;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* remove object from list                                                    */
/*----------------------------------------------------------------------------*/
bool SkipListNL::bRemoveByKey(void* pObjectRemove,
                              
                              std::function<bool(void* aRightObject,
                                                 bool& bFound
                                                 )> bfMoveRightTest,
                              
                              std::function<void(SkipNode* pNode,
                                                 int i_Level
                                                 )> fPrintNode
                              )
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  // Remove iff UNIQUE KEY
  RASSERT(m_bUniqueKey);
  
  bool bEqualTo;
  SkipNode* aPriorVec[C_SKIPLIST_LEVELS];
  SkipNode* aPrior;
  
  SkipNode* pNodePrev = pPositionPrior(bfMoveRightTest,aPriorVec,bEqualTo);
  RASSERT(pNodePrev); // logic check
  
  if ( ! bEqualTo ) {
    vTrace("SkipListNL::bRemove: failed to match\n"_Stz);
    vSkipListDump(this);
    SkipNode* pNodePrev =
      pPosition(pObjectRemove,bEqualTo,aPriorVec,false,m_fpCompare);
    UNUSED(pNodePrev);
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
  m_nItems--;

  // delete the node
  delete pNodeRemove;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* validate the skiplist                                                      */
/*----------------------------------------------------------------------------*/
void SkipListNL::vValidate(std::function<bool(void* aRightObject,
                                              bool& bFoundKey
                                             )> bfMoveRightTest,
                           
                           std::function<void(SkipNode* pNode,
                                              int i_Level
                                              )> fPrintNode
                           )
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);
  
  i8 nItems = 0;
  for (SkipNode* pNode = pFirst(0); pNode; pNode = pNext(pNode,0)) {
    pNode->vResetFlags();
    nItems++;
  }
  RASSERT(nItems == m_nItems);

  for (SkipNode* pNode = pFirst(0); pNode; pNode = pNext(pNode,0))
    RASSERT(pNode->m_iHeight >= 0);

  for (SkipNode* pNode = pFirst(1); pNode; pNode = pNext(pNode,1))
    RASSERT(pNode->m_iHeight >= 1);
  
  for (SkipNode* pNode = pFirst(2); pNode; pNode = pNext(pNode,2))
    RASSERT(pNode->m_iHeight == 2);


//  for (SkipNode* pNode = pFirst(0); pNode; pNode = pNext(pNode,0)) {
//    if (!pFind(pNode->m_pObject)) {
//      vTrace("Cannot find : "_Stz);
//        m_fpPrint(pNode->m_pObject);
//        vTrace("\n"_Stz);
//      NEVER_REACH;
//    }
//  }
           
}

/*----------------------------------------------------------------------------*/
/*-- private: ----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* position object to be inserted                                             */
/*----------------------------------------------------------------------------*/

#define moveScanToRight(pLeft,iLevel) pLeft = pLeft->m_pRight[iLevel];
#define goDownLevel(iLevel) iLevel--;

SkipNode* SkipListNL::pPosition(void* pSearchForObject,
                                bool& bEqualTo,
                                SkipNode** aPriorVec,
                                bool bTrace,
                                CompareFp fpCompare)
{
  bEqualTo = false;

  SkipNode* pPriorNode = m_pHead;
  i8 iLevel = m_iHeight;
  
  do {
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // inner loop: go right a number of nodes until we find
    //               ObjectInsert < ObjectNext at current level
    
    bool bPosObjIsGT;
    
    while (SkipNode* pRightNode = pPriorNode->m_pRight[iLevel]) {
      
      
      // compare the object being searched for
      if (fpCompare) {
        
        bPosObjIsGT = fpCompare(pSearchForObject,
                                pRightNode->m_pObject,
                                bEqualTo);
      }
      else {
        bPosObjIsGT = bCompareAddr(pSearchForObject,
                                   pRightNode->m_pObject,
                                   bEqualTo);
      }
      
      if (bTrace) {
        vTrace("Pos ^%lld: "_Stz,iLevel);
          m_fpPrint(pSearchForObject);
          vTrace(" v "_Stz);
          m_fpPrint(pRightNode->m_pObject);
          vTrace(" : %s\n"_Stz,bPosObjIsGT ? "GT moveRight" : "LE goDown" );
      }
      
      if (bPosObjIsGT) {
        moveScanToRight(pPriorNode,iLevel);
      }
      else {
        break;
      }
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // outer loop: now having gone right as far as we go down one single level
    
    // first record current node as a revision point
    if (aPriorVec)
      aPriorVec[iLevel] = pPriorNode;
       
    // now go down one level on scan node
    goDownLevel(iLevel);
    
    // and repeat till we have a complete revision vector
  } while (iLevel >= 0);
  
  return pPriorNode;
}

/*----------------------------------------------------------------------------*/
/* locate object using lambda                                                 */
/*----------------------------------------------------------------------------*/
SkipNode* SkipListNL::pLocateKey
           (
            std::function<bool(void* aRightObject,
                               bool& bFoundKey
                               )> bfMoveRightTest,
            
            std::function<void(SkipNode* pNode,
                               int i_Level
                               )> fPrintNode
            )
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);
  
  SkipNode* pScanNode = m_pHead;
  
  bool bFoundKey = false;

  for (i8 iLevel = m_iHeight; iLevel >= 0; iLevel--) {
              
    while (SkipNode* pRightNode = pScanNode->m_pRight[iLevel]) {
      
      // inner loop:
      //   go right a number of nodes until we find
      //     ScanObject < RightObject at current level
      
      void* pRightObject = pRightNode->m_pObject;
      RASSERT(pRightObject);
      
      bool bMoveRightAgain = bfMoveRightTest(pRightObject,bFoundKey);
            
      if (bFoundKey)
        return pRightNode;
      
      if (! bMoveRightAgain)
        break;
      
      pScanNode = pScanNode->m_pRight[iLevel];
      
    } // while able to go right as far as possible
        
  } // for each level downwards
  
  return nullptr;
}


/*----------------------------------------------------------------------------*/
/* position prior vector for insert/remove using lambda                       */
/*----------------------------------------------------------------------------*/
SkipNode* SkipListNL::pPositionPrior
           (
            std::function<bool(void* aRightObject,
                               bool& bFoundKey
                               )> bfMoveRightTest,
            
            SkipNode** paPriorVec,
            
            bool& bFoundInList,
            
            std::function<void(SkipNode* pNode,
                               int i_Level
                               )> fPrintNode
            )
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPLIST,_FOOTER_);

  SkipNode* pPriorNode = m_pHead;
  
  bFoundInList = false;

  for (i8 iLevel = m_iHeight; iLevel >= 0; iLevel--) {
              
    while (SkipNode* pRightNode = pPriorNode->m_pRight[iLevel]) {
      
      // inner loop:
      //   move right along the nodes at this level
      //     until we find
      //       ScanObject < RightObject at current level
      
      void* pRightObject = pRightNode->m_pObject;
      RASSERT(pRightObject);
      
      bool bMoveRightAgain = bfMoveRightTest(pRightObject,bFoundInList);
            
      if (! bMoveRightAgain)
        break;
            
      pPriorNode = pPriorNode->m_pRight[iLevel];
      
    } // while able to go right as far as possible
        
    paPriorVec[iLevel] = pPriorNode;

  } // for each level downwards

  // return prior node as at level 0
  return pPriorNode;
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


// /////////////////////////////////////////////////////////////////////////////
// /////////////////////////////////////////////////////////////////////////////
// /////////////////////////////////////////////////////////////////////////////

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

void BASE::vSkipListDump(SkipListNL* pSkipList)
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
      UNUSED(i);
      vTrace("Node %p : %p (L%lld)\n"_Stz
             ,pNode,(i8)pNode->m_pObject, pNode->m_iHeight);
      pNode = pSkipList->pNext(pNode, iLevel);
    }
  } while (--iLevel >= 0);
  vTrace("============================\n"_Stz);
  vTrace("\n"_Stz);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
