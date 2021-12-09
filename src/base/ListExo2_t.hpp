/*============================================================================*/
/* ListExo2_t.hpp -- List of Items (with exo-nodes)                            */
/*============================================================================*/
#pragma once

namespace BASE { template <class ITEM> class ListExoIter2_t; }

namespace BASE
{
  template <class ITEM>
  class ListExo2_t
  {
  protected:
    i8   m_nItems;

    ListExoNode_t<ITEM>* m_pHeadNode;
    ListExoNode_t<ITEM>* m_pTailNode;
    
  public:
    ListExo2_t(void);
    ListExo2_t(ListExoIter2_t<ITEM>* pIter);
   ~ListExo2_t(void) noexcept(false);

    void  vAppend(ITEM* pItem);
    void  vDeleteAll(void);
    i8    iFind(ITEM* pItem);
    ITEM* pGet(i8 nThItem);
    ITEM* pHead(void);
    ListExoNode_t<ITEM>* pHeadNode(void);
    void  vInsertAfter(ITEM* pInsertItem, ITEM* pAfterItem);
    void  vInsertAfter(ITEM* pInsertItem, ListExoNode_t<ITEM>* pAfterNode);
    void  vInsertBefore(ITEM* pInsertItem, ITEM* pBeforeItem);
    void  vInsertBefore(ITEM* pInsertItem, ListExoNode_t<ITEM>* pBeforeNode);
    i8    nItems(void);
    ListExoIter2_t<ITEM>* pNewIter(void);
    ITEM* pPeek(i8 nThItem);
    ITEM* pPop(void);
    ITEM* pPopTail(void);
    void  vPush(ITEM* pItem);
    void  vPushTail(ITEM* pItem);
    void  vRemove(ITEM* pItem);
    void  vRemoveAll(void);
    void  vRemoveNode(ListExoNode_t<ITEM>* pNode);
    ITEM* pTail(void);
    ListExoNode_t<ITEM>* pTailNode(void);
    void  vWalk(std::function<void(ITEM* pItem)> f_vClosure);
    
  private:
    void vCutNode(ListExoNode_t<ITEM>* pCutThisNode);
    
    ListExoNode_t<ITEM>* pFindNode(ITEM* pItem);

    void vInsertNodeAfter(ListExoNode_t<ITEM>* pInsertThisNode,
                          ListExoNode_t<ITEM>* pAfterThisNode);
  
    void vInsertNodeBefore(ListExoNode_t<ITEM>* pInsertThisNode,
                           ListExoNode_t<ITEM>* pBeforeThisNode);
  
    ListExoNode_t<ITEM>* pNewNode(ITEM* pItem);
    
    void vValidateList(void);
  
    Footer* aFooter(void);
  };

} // namespace BASE


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ListExo2_t<ITEM>::ListExo2_t(void)
{
  m_nItems = 0;

  m_pHeadNode = nullptr;
  m_pTailNode = nullptr;

}

/*----------------------------------------------------------------------------*/
/* constructor. Soft copy of items from another list.                         */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ListExo2_t<ITEM>::ListExo2_t(ListExoIter2_t<ITEM>* pIter)

{
  ASSERT(pIter);
  while (pIter->bHasNext()) {
    ITEM* pItem = pIter->pNext();
    vAppend(pItem);
  }
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ListExo2_t<ITEM>::~ListExo2_t(void) noexcept(false)
{
  vRemoveAll();
  
  RASSERT(! m_nItems);
}

/*----------------------------------------------------------------------------*/
/* append item to end of list                                                 */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListExo2_t<ITEM>::vAppend(ITEM* pItem)
{
  // ?validate item is a sound object

  ListExoNode_t<ITEM>* pAppendingNode = pNewNode(pItem);
  ASSERT(! pAppendingNode->pNextNode());
  ASSERT(! pAppendingNode->pPrevNode());

  vInsertNodeAfter(pAppendingNode,m_pTailNode);
  
  vValidateList();
}

/*----------------------------------------------------------------------------*/
/* delete all items in list                                                   */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListExo2_t<ITEM>::vDeleteAll(void)
{
  for (ITEM* pItem = pPop(); pItem; pItem = pPop())
    delete pItem;
}

/*----------------------------------------------------------------------------*/
/* find i_Item of item                                                        */
/*----------------------------------------------------------------------------*/
template <class ITEM>
i8 ListExo2_t<ITEM>::iFind(ITEM* pItem)
{
  ListExoNode_t<ITEM>* pScanNode = nullptr;
  
  i8 i_Item = C_UNKNOWN;
  
  i8 i_Scan = 0;
  for (pScanNode = m_pHeadNode;
       pScanNode;
       pScanNode = pScanNode->pNextNode()) {
    
    if (pItem == pScanNode->pItem()) {
      i_Item = i_Scan;
      break;
    }
    
    i_Scan++;
  }
  
  // no change, no validate
  
  return i_Item;
}

/*----------------------------------------------------------------------------*/
/* get nth item of list                                                       */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* ListExo2_t<ITEM>::pGet(i8 nThItem)
{
  ListExoNode_t<ITEM>* pScanNode = nullptr;
  ITEM* pScanItem = nullptr;
  
  i8 i_Scan = 0;
  for (pScanNode = m_pHeadNode;
       pScanNode;
       pScanNode = pScanNode->pNextNode()) {
    
    if (i_Scan == nThItem)
      break;
    
    i_Scan++;
  }

  ASSERT((i_Scan == nThItem && pScanNode) || (i_Scan > nThItem && !pScanNode));
  
  if (pScanNode)
    pScanItem = pScanNode->pItem();
  
  // no change, no validate
  
  return pScanItem;
}

/*----------------------------------------------------------------------------*/
/* get head item of list                                                      */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* ListExo2_t<ITEM>::pHead(void)
{
  
  if (! m_pHeadNode) {
    return nullptr;
  }

  ITEM* pItem = m_pHeadNode->pItem();
  
  return pItem;
}

/*----------------------------------------------------------------------------*/
/* get head node                                                              */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ListExoNode_t<ITEM>* ListExo2_t<ITEM>::pHeadNode(void)
{
  return m_pHeadNode;
}

/*----------------------------------------------------------------------------*/
/* insert item after given item                                               */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListExo2_t<ITEM>::vInsertAfter(ITEM* pInsertThisItem,
                                           ITEM* pAfterThisItem)
{
  ListExoNode_t<ITEM>* pInsertThisNode = pNewNode(pInsertThisItem);
  ASSERT(! pInsertThisNode->pNextNode());
  ASSERT(! pInsertThisNode->pPrevNode());

  ListExoNode_t<ITEM>* pAfterThisNode = pFindNode(pAfterThisItem);
  
  if (! pAfterThisNode) {
    NEVER_REACH;
    return;
  }
  
  vInsertNodeAfter(pInsertThisNode,pAfterThisNode);
  
  vValidateList();
}

/*----------------------------------------------------------------------------*/
/* insert item after given node                                               */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListExo2_t<ITEM>::vInsertAfter(ITEM* pInsertThisItem,
                                           ListExoNode_t<ITEM>* pAfterThisNode)
{
  ListExoNode_t<ITEM>* pInsertThisNode = pNewNode(pInsertThisItem);
  ASSERT(! pInsertThisNode->pNextNode());
  ASSERT(! pInsertThisNode->pPrevNode());
  
  vInsertNodeAfter(pInsertThisNode,pAfterThisNode);

  vValidateList();
}

/*----------------------------------------------------------------------------*/
/* insert item before given item                                              */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListExo2_t<ITEM>::vInsertBefore(ITEM* pInsertThisItem,
                                            ITEM* pBeforeThisItem)
{
  ListExoNode_t<ITEM>* pInsertThisNode = pNewNode(pInsertThisItem);
  ASSERT(! pInsertThisNode->pNextNode());
  ASSERT(! pInsertThisNode->pPrevNode());

  ListExoNode_t<ITEM>* pBeforeThisNode = pFindNode(pBeforeThisItem);
  
  if (! pBeforeThisNode) {
    NEVER_REACH;
    return;
  }
  
  vInsertNodeBefore(pInsertThisNode,pBeforeThisNode);
  
  vValidateList();
}

/*----------------------------------------------------------------------------*/
/* insert item before given node                                              */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListExo2_t<ITEM>::vInsertBefore(ITEM* pInsertThisItem,
                                           ListExoNode_t<ITEM>* pBeforeThisNode)
{
  ListExoNode_t<ITEM>* pInsertThisNode = pNewNode(pInsertThisItem);
  ASSERT(! pInsertThisNode->pNextNode());
  ASSERT(! pInsertThisNode->pPrevNode());

  vInsertNodeBefore(pInsertThisNode,pBeforeThisNode);

  vValidateList();
}

/*----------------------------------------------------------------------------*/
/* get number of items in list                                                */
/*----------------------------------------------------------------------------*/
template <class ITEM>
i8 ListExo2_t<ITEM>::nItems(void)
{
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* peek at nth item of list                                                   */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ListExoIter2_t<ITEM>* ListExo2_t<ITEM>::pNewIter(void)
{
  ListExoIter2_t<ITEM>* pIter
    = new ListExoIter2_t<ITEM>((ListExo2_t<ITEM>*)this);
  
  return pIter;
}

/*----------------------------------------------------------------------------*/
/* peek at nth item of list                                                   */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* ListExo2_t<ITEM>::pPeek(i8 nThItem)
{
  ListExoNode_t<ITEM>* pScanNode = nullptr;
  ITEM* pScanItem = nullptr;
  
  i8 i_Scan = 0;
  for (pScanNode = m_pHeadNode;
       pScanNode;
       pScanNode = pScanNode->pNextNode()) {
    
    if (i_Scan == nThItem)
      break;
    
    i_Scan++;
  }

  ASSERT((i_Scan == nThItem && pScanNode) || (i_Scan > nThItem && !pScanNode));
  
  if (pScanNode)
    pScanItem = pScanNode->pItem();
  
  // no change, no validate
  
  return pScanItem;
}

/*----------------------------------------------------------------------------*/
/* pop/remove item from head of list                                          */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* ListExo2_t<ITEM>::pPop(void)
{
  if (! m_pHeadNode) {
    ASSERT(! m_nItems);
    return nullptr;
  }

  ListExoNode_t<ITEM>* pWasHeadNode = m_pHeadNode;
  ITEM* pWasHeadItem = pWasHeadNode->pItem();
  
  vCutNode(pWasHeadNode);

  vValidateList();

  //delete pWasHeadNode;

  return pWasHeadItem;
}

/*----------------------------------------------------------------------------*/
/* pop/remove item from tail of list                                          */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* ListExo2_t<ITEM>::pPopTail(void)
{
  if (! m_pTailNode) {
    ASSERT(! m_nItems);
    return nullptr;
  }

  ListExoNode_t<ITEM>* pWasTailNode = m_pTailNode;
  ITEM* pWasTailItem = pWasTailNode->pItem();
  
  vCutNode(pWasTailNode);

  vValidateList();

  //delete pWasTailNode;
  
  return pWasTailItem;
}

/*----------------------------------------------------------------------------*/
/* push item onto top of list                                                 */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListExo2_t<ITEM>::vPush(ITEM* pItem)
{
  ListExoNode_t<ITEM>* pPushingNode = pNewNode(pItem);
  ASSERT(! pPushingNode->pNextNode());
  ASSERT(! pPushingNode->pPrevNode());
  
  vInsertNodeBefore(pPushingNode,m_pHeadNode);

  vValidateList();
}

/*----------------------------------------------------------------------------*/
/* push item onto end of list                                                 */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListExo2_t<ITEM>::vPushTail(ITEM* pItem)
{
  ListExoNode_t<ITEM>* pPushingNode = pNewNode(pItem);
  ASSERT(! pPushingNode->pNextNode());
  ASSERT(! pPushingNode->pPrevNode());
  
  vInsertNodeAfter(pPushingNode,m_pTailNode);
  

  vValidateList();
}

/*----------------------------------------------------------------------------*/
/* remove item from list                                                      */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListExo2_t<ITEM>::vRemove(ITEM* pItem)
{
  ListExoNode_t<ITEM>* pRemovingNode = pFindNode(pItem);
  
  if (! pRemovingNode) {
    vValidateList();
    return;
  }
  
  vCutNode(pRemovingNode);
  
  vValidateList();

  //delete pRemovingNode;
}

/*----------------------------------------------------------------------------*/
/* remove all items in list                                                   */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListExo2_t<ITEM>::vRemoveAll(void)
{
  for (ITEM* pItem = pPop(); pItem; pItem = pPop())
    {;}
}

/*----------------------------------------------------------------------------*/
/* remove a node from list                                                    */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListExo2_t<ITEM>::vRemoveNode(ListExoNode_t<ITEM>* pRemovingNode)
{

  vCutNode(pRemovingNode);

  vValidateList();

  //delete pRemovingNode;
}

/*----------------------------------------------------------------------------*/
/* get tail item of list                                                      */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* ListExo2_t<ITEM>::pTail(void)
{
  if (! m_pTailNode) {
    return nullptr;
  }

  ITEM* pItem = m_pTailNode->pItem();
  
  return pItem;
}

/*----------------------------------------------------------------------------*/
/* get tail node                                                              */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ListExoNode_t<ITEM>* ListExo2_t<ITEM>::pTailNode(void)
{
  return m_pTailNode;
}

/*----------------------------------------------------------------------------*/
/* walk                                                                       */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListExo2_t<ITEM>::vWalk(std::function<void(ITEM* pItem)> f_vClosure)
{
  ListExoIter2_t<ITEM>* pIter = pNewIter();
  
  while (pIter->bHasNext()) {
    ITEM* pItem = pIter->pNext();
    
    f_vClosure(pItem);
  }
  
  delete pIter;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* cut exo-node from list, dec m_ntems (exo-node is not deletes)              */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListExo2_t<ITEM>::vCutNode(ListExoNode_t<ITEM>* pCutThisNode)
{
  if (! pCutThisNode) {
    return;
  }
  
  ListExoNode_t<ITEM>* pNextOfCut = pCutThisNode->pNextNode();
  ListExoNode_t<ITEM>* pPrevOfCut = pCutThisNode->pPrevNode();

  if (pCutThisNode == m_pHeadNode && pCutThisNode == m_pTailNode) {
    // cut only node
    
    ASSERT(1 == m_nItems);
    m_pHeadNode = nullptr;
    m_pTailNode = nullptr;
  }
  else
  if (pCutThisNode == m_pHeadNode) {
    // cut head node
    
    ASSERT(m_nItems > 1);
    pNextOfCut->vPrevNode(nullptr);
    m_pHeadNode = pNextOfCut;
  }
  else
  if (pCutThisNode == m_pTailNode) {
    // cut tail node

    ASSERT(m_nItems > 1);
    pPrevOfCut->vNextNode(nullptr);
    m_pTailNode = pPrevOfCut;
  }
  else {
    // cut body node
    
    ASSERT(m_nItems > 1);
    pPrevOfCut->vNextNode(pNextOfCut);
    pNextOfCut->vPrevNode(pPrevOfCut);
  }
  
  pCutThisNode->vNextNode(nullptr);
  pCutThisNode->vPrevNode(nullptr);
  
  delete pCutThisNode;
  m_nItems--;
  ASSERT(m_nItems >= 0);
}

/*----------------------------------------------------------------------------*/
/* find node given item                                                       */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ListExoNode_t<ITEM>* ListExo2_t<ITEM>::pFindNode(ITEM* pItem)
{
  ListExoNode_t<ITEM>* pScanNode;

  for (pScanNode = m_pHeadNode;
       pScanNode;
       pScanNode = pScanNode->pNextNode()) {
    
    if (pItem == pScanNode->pItem()) {
      ASSERT(m_nItems >= 1);
      return pScanNode;
    }
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* insert node after given node                                               */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListExo2_t<ITEM>::
  vInsertNodeAfter(ListExoNode_t<ITEM>* pInsertThisNode,
                   ListExoNode_t<ITEM>* pAfterThisNode)
{
  if (pAfterThisNode) {
    
    ASSERT(m_nItems);

    if (pAfterThisNode->pNextNode())
      pAfterThisNode->pNextNode()->vPrevNode(pInsertThisNode); // (1)
    
    pInsertThisNode->vNextNode(pAfterThisNode->pNextNode()); // (2)
    
    pInsertThisNode->vPrevNode(pAfterThisNode); // (3)
    
    pAfterThisNode->vNextNode(pInsertThisNode); // (4)
  }
  
  if (! m_pHeadNode) {
    ASSERT(! m_nItems);
    m_pHeadNode = pInsertThisNode;
  }
  
  if (! m_pTailNode || m_pTailNode == pAfterThisNode)
    m_pTailNode = pInsertThisNode;

  m_nItems++;
  ASSERT(m_nItems >= 1);
}

/*----------------------------------------------------------------------------*/
/* insert node before given node                                              */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListExo2_t<ITEM>::
  vInsertNodeBefore(ListExoNode_t<ITEM>* pInsertThisNode,
                    ListExoNode_t<ITEM>* pBeforeThisNode)
{
  if (pBeforeThisNode) {
    
    ASSERT(m_nItems);
    
    if (pBeforeThisNode->pPrevNode())
      pBeforeThisNode->pPrevNode()->vNextNode(pInsertThisNode); // (1)
    
    pInsertThisNode->vPrevNode(pBeforeThisNode->pPrevNode()); // (2)
    
    pInsertThisNode->vNextNode(pBeforeThisNode); // (3)
    
    pBeforeThisNode->vPrevNode(pInsertThisNode); // (4)
  }
  
  
  if (! m_pHeadNode || m_pHeadNode == pBeforeThisNode)
    m_pHeadNode = pInsertThisNode;
  
  if (! m_pTailNode) {
    ASSERT(! m_nItems);
    m_pTailNode = pInsertThisNode;
  }
  
  m_nItems++;
  ASSERT(m_nItems >= 1);
}

/*----------------------------------------------------------------------------*/
/* create a node for item                                                     */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ListExoNode_t<ITEM>* ListExo2_t<ITEM>::pNewNode(ITEM* pItem)
{
  ListExoNode_t<ITEM>* pListExoNode = new ListExoNode_t<ITEM>(pItem);
  
  return pListExoNode;
}

/*----------------------------------------------------------------------------*/
/* validate list                                                              */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListExo2_t<ITEM>::vValidateList(void)
{
  i8 nItemsCheck;
  ListExoNode_t<ITEM>* pScanNode;

  nItemsCheck = 0;
  for (pScanNode = m_pHeadNode;
       pScanNode;
       pScanNode = pScanNode->pNextNode()) {
    
    if (nItemsCheck > m_nItems)
      break;
    
    //RASSERT(CLASSNO == pScanNode->pItem()->eGetClassNo());
    nItemsCheck++;
  }

  RASSERT(nItemsCheck == m_nItems);
  
  nItemsCheck = 0;
  for (pScanNode = m_pTailNode;
       pScanNode;
       pScanNode = pScanNode->pPrevNode()) {
    
    if (nItemsCheck > m_nItems)
      break;
    
    //RASSERT(CLASSNO == pScanNode->pItem()->eGetClassNo());
    nItemsCheck++;
  }
  
  RASSERT(nItemsCheck == m_nItems);

}


// § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § §
// § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § §
// § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § §
// § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § §

namespace BASE
{
template <class ITEM>
class ListExoIter2_t : public Object
{
private:
   
  ListExo2_t<ITEM>* m_pListExot;

  b1                       m_bFwdDir; // true if the iter last moved towards the tail
  b1                       m_bCanRemove; // true if it is safe to call vRemove()
  ListExoNode_t<ITEM>*     m_pNextCursor;
  ListExoNode_t<ITEM>*     m_pPrevCursor;
  ListExoNode_t<ITEM>*     m_pLastCursor;  // The cursor of the last item returned

  // Note: The iterator always points between items, but remembers the last item
  // returned (via m_bFwdDir). This allows for consistent operation with forward
  // traversals, backward traversals and removals. If the iterator simply points
  // at the last item returned then we will get incorrect results for certain
  // arbitrary traversals.
  // e.g. consider the following sequence:
  //    pIter->pNext()
  //    pIter->pNext()
  //    pIter->vRemove()
  //    pIter->pPrev()
  // We need the iterator to point between objects to do this properly.

  // must be last
  Footer m_xFooter;

public:
  void* operator new(size_t);
  void operator delete(void* aListExoNode);

  ListExoIter2_t<ITEM>(ListExo2_t<ITEM>* pListExo);
 ~ListExoIter2_t(void) noexcept(false);
  
  void  vAfterLast(void);
  void  vBeforeFirst(void);
  ITEM* pFirst(void);
  bool  bIsAfterLast(void);
  bool  bIsBeforeFirst(void);
  bool  bHasNext(void);
  bool  bHasPrev(void);
  ITEM* pNext(void);
  ITEM* pPeekNext(void);
  bool  bPositionBefore(ITEM* pItem);
  ITEM* pPrev(void);
  void  vRemove(void);
  void  vInsertAfter(ITEM* pItem);
  void  vInsertBefore(ITEM* pItem);

private:
  Footer* aFooter(void);
};

} // namespace BASE


/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void* ListExoIter2_t<ITEM>::operator new(size_t)
{
  ListExoIter2_t<ITEM>* pListExoIter2_t;
  
  pListExoIter2_t =
    (ListExoIter2_t<ITEM>*)Heaps::pAllocate(CN_LISTEXOITER_t);
  
  return pListExoIter2_t;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListExoIter2_t<ITEM>::operator delete(void* aListExoIter2_t)
{
  ListExoIter2_t<ITEM>* pListExoIter2_t;

  pListExoIter2_t = (ListExoIter2_t<ITEM>*)aListExoIter2_t;

  Heaps::vFree(pListExoIter2_t);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ListExoIter2_t<ITEM>::ListExoIter2_t(ListExo2_t<ITEM>* pListExo)
: Object(CN_LISTEXOITER_t)
{
  _precondition_(_ADDRESS_,CN_LISTEXOITER_t,_FOOTER_);

  m_pListExot = pListExo;

  vBeforeFirst();

  _checkInitialised_(CN_LISTEXOITER_t);
  
  _postcondition_(_ADDRESS_,CN_LISTEXOITER_t,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ListExoIter2_t<ITEM>::~ListExoIter2_t(void) noexcept(false)
{
  _precondition_(_ADDRESS_,CN_LISTEXOITER_t,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* position before first item                                                 */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListExoIter2_t<ITEM>::vBeforeFirst(void)
{
  _precondition_(_ADDRESS_,CN_LISTEXOITER_t,_FOOTER_);

  m_bFwdDir = true;
  m_bCanRemove = false;
  m_pNextCursor = m_pListExot->pHeadNode();
  m_pPrevCursor = nullptr;
  m_pLastCursor = nullptr;
}

/*----------------------------------------------------------------------------*/
/* position after last item for a backwards traversal                         */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListExoIter2_t<ITEM>::vAfterLast(void)
{
  _precondition_(_ADDRESS_,CN_LISTEXOITER_t,_FOOTER_);

  m_bFwdDir = false;
  m_bCanRemove = false;
  m_pNextCursor = nullptr;
  m_pPrevCursor = m_pListExot->pTailNode();
  m_pLastCursor = nullptr;
}

/*----------------------------------------------------------------------------*/
/* get first item of list                                                     */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* ListExoIter2_t<ITEM>::pFirst(void)
{
  _precondition_(_ADDRESS_,CN_LISTEXOITER_t,_FOOTER_);

  vBeforeFirst();
  return pNext();
}

/*----------------------------------------------------------------------------*/
/* return true if the iterator is after the last element in the list          */
/*----------------------------------------------------------------------------*/
template <class ITEM>
bool ListExoIter2_t<ITEM>::bIsAfterLast(void)
{
  _precondition_(_ADDRESS_,CN_LISTEXOITER_t,_FOOTER_);

  return !m_pNextCursor;
}

/*----------------------------------------------------------------------------*/
/* return true if the iterator is before the first element in the list        */
/*----------------------------------------------------------------------------*/
template <class ITEM>
bool ListExoIter2_t<ITEM>::bIsBeforeFirst(void)
{
  _precondition_(_ADDRESS_,CN_LISTEXOITER_t,_FOOTER_);

  return !m_pPrevCursor;
}

/*----------------------------------------------------------------------------*/
/* Return true if there is another node after this one                        */
/*----------------------------------------------------------------------------*/
template <class ITEM>
bool ListExoIter2_t<ITEM>::bHasNext(void)
{
  _precondition_(_ADDRESS_,CN_LISTEXOITER_t,_FOOTER_);

  return m_pNextCursor;
}

/*----------------------------------------------------------------------------*/
/* Return true if there is another node before this one                       */
/*----------------------------------------------------------------------------*/
template <class ITEM>
bool ListExoIter2_t<ITEM>::bHasPrev(void)
{
  _precondition_(_ADDRESS_,CN_LISTEXOITER_t,_FOOTER_);

  return m_pPrevCursor;
}

/*----------------------------------------------------------------------------*/
/* get next item in list moving towards the tail                              */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* ListExoIter2_t<ITEM>::pNext(void)
{
  _precondition_(_ADDRESS_,CN_LISTEXOITER_t,_FOOTER_);
  
  ITEM* pItem;

  m_bFwdDir = true;

  if (bIsAfterLast()) {
    m_bCanRemove = false;
    m_pLastCursor = nullptr;
    return nullptr;
  }

  ASSERT(m_pNextCursor);
  m_pLastCursor = m_pNextCursor;
  pItem = m_pNextCursor->pItem();

  m_pPrevCursor = m_pNextCursor;
  m_pNextCursor = m_pNextCursor->pNextNode();

  m_bCanRemove = true;

  return pItem;
}

/*----------------------------------------------------------------------------*/
/* peek at next item in list                                                  */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* ListExoIter2_t<ITEM>::pPeekNext(void)
{
  _precondition_(_ADDRESS_,CN_LISTEXOITER_t,_FOOTER_);
  
  if (! bHasNext())
    return nullptr;
  
  ITEM* pItem = pNext();
  
  IGNORE_RESULT(pPrev());
  
  return pItem;
}

/*----------------------------------------------------------------------------*/
/* position cursor before given item                                          */
/*----------------------------------------------------------------------------*/
template <class ITEM>
bool ListExoIter2_t<ITEM>::bPositionBefore(ITEM* pItem)
{
  ASSERT(pItem);
  
  vBeforeFirst();
  
  while (bHasNext()) {
    ITEM* pTestItem = pNext();

    if (pTestItem == pItem) {
      IGNORE_RETURN(pPrev());
      return true;
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* get previous item in list moving towards the head                          */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* ListExoIter2_t<ITEM>::pPrev(void)
{
  _precondition_(_ADDRESS_,CN_LISTEXOITER_t,_FOOTER_);

  ITEM* pItem;

  m_bFwdDir = false;

  if (bIsBeforeFirst()) {
    m_bCanRemove = false;
    m_pLastCursor = nullptr;
    return nullptr;
  }

  ASSERT(m_pPrevCursor);
  m_pLastCursor = m_pPrevCursor;
  pItem = m_pPrevCursor->pItem();

  m_pNextCursor = m_pPrevCursor;
  m_pPrevCursor = m_pPrevCursor->pPrevNode();

  m_bCanRemove = true;

  return pItem;
}

/*----------------------------------------------------------------------------*/
/* Remove the last item that was returned (either by pNext() or pPrev().      */
/* The item in the list is not freed, only removed from the list.             */
/* Note that we don't expect to have multiple iterators of the list if it is  */
/* being modified, nor do we expect the list to be directly modified either.  */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListExoIter2_t<ITEM>::vRemove(void)
{
  ASSERT(m_bCanRemove);
  ASSERT(m_pLastCursor);
  ASSERT((!bIsBeforeFirst() && m_bFwdDir) || (!bIsAfterLast() && !m_bFwdDir));

  if (m_pPrevCursor == m_pLastCursor) {
    m_pPrevCursor = m_pPrevCursor->pPrevNode();
  }

  if (m_pNextCursor == m_pLastCursor) {
    m_pNextCursor = m_pNextCursor->pNextNode();
  }

  m_pListExot->vRemoveNode(m_pLastCursor);

  // Can't call vRemove() twice. Need to move the iterator.
  m_bCanRemove = false;
  m_pLastCursor = nullptr;
}

/*----------------------------------------------------------------------------*/
/* Inserts an object after the last object that was retrieved                 */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListExoIter2_t<ITEM>::vInsertAfter(ITEM* pItem)
{
  ASSERT(pItem);

  if (m_pLastCursor) {
    m_pListExot->vInsertAfter(pItem, m_pLastCursor);

    if (m_pPrevCursor == m_pLastCursor) {
      m_pNextCursor = m_pPrevCursor->pNextNode();
    }

    // No need to compare if m_pNextCursor == m_pLastCursor because
    // if we are inserting after it then we don't need to update m_pNextCursor

  } else {
    // m_pLastCursor must have already been removed with pIter->vRemove().
    // Cannot insertAfter() an item that has already been removed.
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Inserts an object before the last object that was retrieved                */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListExoIter2_t<ITEM>::vInsertBefore(ITEM* pItem)
{
  ASSERT(pItem);

  if (m_pLastCursor) {
    m_pListExot->vInsertBefore(pItem, m_pLastCursor);

    if (m_pNextCursor == m_pLastCursor) {
      m_pPrevCursor = m_pNextCursor->pPrevNode();
    }

    // No need to compare if m_pPrevCursor == m_pLastCursor because
    // if we are inserting before it then we don't need to update m_pPrevCursor

  } else {
    // m_pLastCursor must have already been removed with pIter->vRemove().
    // Cannot insertAfter() an item that has already been removed.
    NEVER_REACH
  }
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <class ITEM>
Footer* ListExoIter2_t<ITEM>::aFooter(void)
{
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
