/*===========================================================================*/
/* Skip.cpp -- a skip list                                                   */
/*===========================================================================*/

#include "DataStructures.h"

//---------------------------------------------------------------------------//
// Skip: constructor                                                         //
//---------------------------------------------------------------------------//
Skip::Skip(bool bIsUniqueKey, compare fCompare)
{
  m_iObjects = 0;
  m_iHeight  = 0;
  m_bIsUniqueKey = bIsUniqueKey;
  m_fCompare = fCompare;
  m_pHead    = new(MAX_LEVELS) SkipNode(nullptr);
  m_pTail    = new(0) SkipNode(nullptr);
  
  for (i4 l = 0; l < MAX_LEVELS; l++)
    m_pHead->m_pForward[l] = m_pTail;
  
  m_pTail->m_pForward[0] = nullptr;
}

//---------------------------------------------------------------------------//
// ~Skip: destructor                                                         //
//---------------------------------------------------------------------------//
Skip::~Skip()
{
  ASSERT( ! m_iObjects ); // logic check

// alka
//  tDelete(&m_pKey);
//  
//  tDelete(&m_pHead);
//  tDelete(&m_pTail);
}

//---------------------------------------------------------------------------//
// bExists: test if given object is in list                                  //
//---------------------------------------------------------------------------//
bool Skip::bExists(void* pObject)
{  
  bool bEqualTo;
  
  SkipNode* pNode = pPosition(pObject,&bEqualTo);
  ASSERT(pNode); // logic check
  
  if (bEqualTo && pObject == pNode->m_pForward[0]->m_pObject)
    return true;
  else
    return false;
}

//---------------------------------------------------------------------------//
// bInsert: insert given object                                              //
//---------------------------------------------------------------------------//
bool Skip::bInsert(void* pObject)
{
  bool bEqualTo;
  SkipNode* pRevise[MAX_LEVELS];
  
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

//---------------------------------------------------------------------------//
// iObjects: get numbr of objects in list                                    //
//---------------------------------------------------------------------------//
i8 Skip::iObjects()
{
  return m_iObjects;
}

//---------------------------------------------------------------------------//
// pPop: pop top/first object from list                                      //
//---------------------------------------------------------------------------//
void* Skip::pPop(bool* bWasRemoved, i8 iFrom)
{
  SkipNode* pFirstNode = pFirst();
  if (!pFirstNode)
    return nullptr;
  
  void* pObject = pFirstNode->m_pObject;
  if (!pObject)
    return nullptr;
  
  *bWasRemoved = bRemove(pObject);
  //ASSERT(b); // logic check
  if ( ! *bWasRemoved )
    vError("pPop: failed to remove; from %lld",iFrom);
  
  return pObject;
}

//---------------------------------------------------------------------------//
// bRemove: remove given object (do not run finalize function on object)     //
//---------------------------------------------------------------------------//
bool Skip::bRemove(void* pObject)
{
  i4 l;
  bool bEqualTo;
  SkipNode* pRevision[MAX_LEVELS];
  SkipNode* pRevise;
  
  SkipNode* pNode = pPosition(pObject,&bEqualTo,pRevision);
  ASSERT(pNode); // logic check
  
  if ( ! bEqualTo ) {
    vTrace("Skip::bRemove: failed to match");
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

//---------------------------------------------------------------------------//
// find first object matching search                                         //
//---------------------------------------------------------------------------//
void* Skip::pFindFirst(void* pObject)
{
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

//---------------------------------------------------------------------------//
// pFirst: get first object matching search                                  //
//---------------------------------------------------------------------------//
//void* Skip::pFirst(void* pObject)
//{
//  bool bEqualTo;
//  
//  Node* pNode = pPosition(pObject,&bEqualTo);
//  ASSERT(pNode); // logic check
//  
//  if (pNode->m_pForward[0] == m_pTail)
//    return nullptr;
//  else
//    return pNode->m_pForward[0]->m_pObject;
//}
//
//---------------------------------------------------------------------------//
// pNext: get next object matching search                                    //
//---------------------------------------------------------------------------//
//void* Skip::pNext(void* pObject)
//{
//  return nullptr;
//}

//---------------------------------------------------------------------------//
// vDump: dump the list                                                      //
//---------------------------------------------------------------------------//
void Skip::vDump(void (*fvDump)(void* pObject))
{
  for (SkipNode* pScan = m_pHead->m_pForward[0];
       pScan && pScan != m_pTail;
       pScan = pScan->m_pForward[0]) {
    fvDump(pScan->m_pObject);
  }
}

//---------------------------------------------------------------------------//
// vCheck: check integrity of the list                                       //
//---------------------------------------------------------------------------//
void Skip::vCheck()
{
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

//---------------------------------------------------------------------------//
// pEach: get each object                                                    //
//---------------------------------------------------------------------------//
void* Skip::pEach(SkipCursor* pCursor)
{
  if ( ! pCursor->m_pCurrentNode )
    pCursor->m_pCurrentNode = pFirst();
  else
    pCursor->m_pCurrentNode = pNext(pCursor->m_pCurrentNode);
  
  if (pCursor->m_pCurrentNode)
    return pCursor->m_pCurrentNode->m_pObject;
  else
    return nullptr;
}

//---------------------------------------------------------------------------//
// pFirst: get first node                                                    //
//---------------------------------------------------------------------------//
SkipNode* Skip::pFirst()
{
  if (m_pHead->m_pForward[0] != m_pTail)
    return m_pHead->m_pForward[0];
  
  return nullptr;
}

//---------------------------------------------------------------------------//
// pNext: get next node                                                      //
//---------------------------------------------------------------------------//
SkipNode* Skip::pNext(SkipNode* pPrev)
{
  if (pPrev->m_pForward[0] != m_pTail)
    return pPrev->m_pForward[0];
  
  return nullptr;
}

//---------------------------------------------------------------------------//
// pHead: get head bookend                                                   //
//---------------------------------------------------------------------------//
SkipNode* Skip::pHead()
{
  return m_pHead;
}

//---------------------------------------------------------------------------//
// pTail: get tail bookend                                                   //
//---------------------------------------------------------------------------//
SkipNode* Skip::pTail()
{
  return m_pTail;
}

//---------------------------------------------------------------------------//
// bDeleteAll: delete all objects                                            //
//---------------------------------------------------------------------------//
bool Skip::bDeleteAll(void)
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

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------//
// iLevel: choose a random level for new node                                //
//---------------------------------------------------------------------------//
i4 Skip::iLevel()
{
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
  
  return (level > MAX_LEVELS - 1) ? MAX_LEVELS - 1: level;
}

//---------------------------------------------------------------------------//
// pPosition: position search on node just prior to lower bound              //
//---------------------------------------------------------------------------//
SkipNode* Skip::pPosition(void* pObject, bool* bEqualTo)
{
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

//---------------------------------------------------------------------------//
// pPosition: position search on node just prior to lower bound              //
//---------------------------------------------------------------------------//
SkipNode* Skip::pPosition(void* pObject, bool* bEqualTo, SkipNode** pRevise)
{
  SkipNode* pNext = nullptr;
  
  *bEqualTo = false;
  
  i4 l = m_iHeight;
  SkipNode* pScan = m_pHead;
  //Node* pPrev = nullptr;
  do
  {
    while (pNext = pScan->m_pForward[l],
           pNext != m_pTail
           && m_fCompare(pObject,pNext->m_pObject,bEqualTo))
    {
      //pPrev = pScan;
      pScan = pNext;
    }
  
    pRevise[l] = pScan;
    
  } while(--l >= 0);
  
  return pScan;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/