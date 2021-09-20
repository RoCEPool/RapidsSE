/*============================================================================*/
/* ListExo_t.hpp -- List of Items (with exo-nodes)                            */
/*============================================================================*/
#pragma once

namespace BASE
{
  template <class ITEM>
  class ListExoNode_t : public Object
  {
  private:
    ITEM* m_pItem; // item
    ListExoNode_t<ITEM>* m_pPrevNode;   // prev node/item
    ListExoNode_t<ITEM>* m_pNextNode;   // next node/item
   
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void operator delete(void* aListExoNode);

    ListExoNode_t<ITEM>(ITEM* pItem);
   ~ListExoNode_t(void) noexcept(false);

    ITEM* pItem(void);
  
    ListExoNode_t<ITEM>* pNextNode(void);
    void vNextNode(ListExoNode_t<ITEM>* pNext);
  
    ListExoNode_t<ITEM>* pPrevNode(void);
    void vPrevNode(ListExoNode_t<ITEM>* pPrev);

  private:
    Footer* aFooter(void);
  };

} // namespace BASE

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void* ListExoNode_t<ITEM>::operator new(size_t)
{
  ListExoNode_t<ITEM>* pListExoNode_t;
  
  pListExoNode_t = (ListExoNode_t<ITEM>*)Heaps::pAllocate(CN_LISTEXONODE_t);
  
  return pListExoNode_t;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListExoNode_t<ITEM>::operator delete(void* aListExoNode_t)
{
  ListExoNode_t<ITEM>* pListExoNode_t = (ListExoNode_t<ITEM>*)aListExoNode_t;

  Heaps::vFree(pListExoNode_t);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ListExoNode_t<ITEM>::ListExoNode_t(ITEM* pItem)
: Object(CN_LISTEXONODE_t)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LISTEXONODE_t,_FOOTER_);

  m_pItem = pItem;
  
  m_pPrevNode = nullptr;
  m_pNextNode = nullptr;
  
  _checkInitialised_(CN_LISTEXONODE_t);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LISTEXONODE_t,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ListExoNode_t<ITEM>::~ListExoNode_t(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LISTEXONODE_t,_FOOTER_);

  ASSERT(! m_pNextNode);
  ASSERT(! m_pPrevNode);
}

/*----------------------------------------------------------------------------*/
/* get item of node                                                           */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* ListExoNode_t<ITEM>::pItem(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LISTEXONODE_t,_FOOTER_);

  return m_pItem;
}

/*----------------------------------------------------------------------------*/
/* get next node                                                              */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ListExoNode_t<ITEM>* ListExoNode_t<ITEM>::pNextNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LISTEXONODE_t,_FOOTER_);

  return m_pNextNode;
}

/*----------------------------------------------------------------------------*/
/* set next node                                                              */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListExoNode_t<ITEM>::vNextNode(ListExoNode_t<ITEM>* pNext)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LISTEXONODE_t,_FOOTER_);

  m_pNextNode = pNext;
}

/*----------------------------------------------------------------------------*/
/* get prev node                                                              */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ListExoNode_t<ITEM>* ListExoNode_t<ITEM>::pPrevNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LISTEXONODE_t,_FOOTER_);

  return m_pPrevNode;
}

/*----------------------------------------------------------------------------*/
/* set prev node                                                              */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListExoNode_t<ITEM>::vPrevNode(ListExoNode_t<ITEM>* pPrev)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LISTEXONODE_t,_FOOTER_);

  m_pPrevNode = pPrev;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <class ITEM>
Footer* ListExoNode_t<ITEM>::aFooter(void)
{
  return &m_xFooter;
}


// § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § §
// § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § §
// § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § §
// § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § §


namespace BASE { template <class ITEM> class ListExoIter_t; }

namespace BASE
{
  template <eCN CLASSNO, class ITEM>
  class ListExo_t : public Object
  {
  protected:
    Lock m_okLock;
    i8   m_nItems;

    ListExoNode_t<ITEM>* m_pHeadNode;
    ListExoNode_t<ITEM>* m_pTailNode;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void* aListExo);

    ListExo_t(void);
    ListExo_t(ListExoIter_t<ITEM>* pIter);
   ~ListExo_t(void) noexcept(false);

    void  vAppend(ITEM* pItem);
    void  vDeleteAll(void);
    ITEM* pHead(void);
    ListExoNode_t<ITEM>* pHeadNode(void);
    void  vInsertAfter(ITEM* pInsertItem, ITEM* pAfterItem);
    void  vInsertAfter(ITEM* pInsertItem, ListExoNode_t<ITEM>* pAfterNode);
    void  vInsertBefore(ITEM* pInsertItem, ITEM* pBeforeItem);
    void  vInsertBefore(ITEM* pInsertItem, ListExoNode_t<ITEM>* pBeforeNode);
    i8    nItems(void);
    ListExoIter_t<ITEM>* pNewIter(void);
    ITEM* pPeek(i8 nThItem);
    ITEM* pPop(void);
    ITEM* pPopTail(void);
    void  vPush(ITEM* pItem);
    void  vPushTail(ITEM* pItem);
    //void  vReadLock(void);
    //void  vReadUnlock(void);
    void  vRemove(ITEM* pItem);
    void  vRemoveAll(void);
    void  vRemoveNode(ListExoNode_t<ITEM>* pNode);
    ITEM* pTail(void);
    ListExoNode_t<ITEM>* pTailNode(void);

    
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
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
void* ListExo_t<CLASSNO,ITEM>::operator new(size_t)
{
  ListExo_t<CLASSNO,ITEM>* pListExo_t;
  
  pListExo_t = (ListExo_t<CLASSNO,ITEM>*)Heaps::pAllocate(CLASSNO);
  
  return pListExo_t;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
void ListExo_t<CLASSNO,ITEM>::operator delete(void* aListExo_t)
{
  ListExo_t<CLASSNO,ITEM>* pListExo_t = (ListExo_t<CLASSNO,ITEM>*)aListExo_t;

  Heaps::vFree(pListExo_t);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
ListExo_t<CLASSNO,ITEM>::ListExo_t(void)
    : Object(CLASSNO)
    , m_okLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
    
  m_nItems = 0;

  m_pHeadNode = nullptr;
  m_pTailNode = nullptr;

  _checkInitialised_(CLASSNO);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor. Soft copy of items from another list.                         */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
ListExo_t<CLASSNO,ITEM>::ListExo_t(ListExoIter_t<ITEM>* pIter)
    : Object(CLASSNO)
    , m_okLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);

  ASSERT(pIter);
  while (pIter->bHasNext()) {
    ITEM* pItem = pIter->pNext();
    vAppend(pItem);
  }

  _checkInitialised_(CLASSNO);

  _postcondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
ListExo_t<CLASSNO,ITEM>::~ListExo_t(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);

  vRemoveAll();
  
  RASSERT(! m_nItems);
}

/*----------------------------------------------------------------------------*/
/* append item to end of list                                                 */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
void ListExo_t<CLASSNO,ITEM>::vAppend(ITEM* pItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
  
  // ?validate item is a sound object

  ListExoNode_t<ITEM>* pAppendingNode = pNewNode(pItem);
  ASSERT(! pAppendingNode->pNextNode());
  ASSERT(! pAppendingNode->pPrevNode());

  m_okLock.vGrabWrite();
  
  vInsertNodeAfter(pAppendingNode,m_pTailNode);
  
  m_okLock.vDropWrite();
  vValidateList();
}

/*----------------------------------------------------------------------------*/
/* delete all items in list                                                   */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
void ListExo_t<CLASSNO,ITEM>::vDeleteAll(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);

  for (ITEM* pItem = pPop(); pItem; pItem = pPop())
    delete pItem;
}

/*----------------------------------------------------------------------------*/
/* get head item of list                                                      */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
ITEM* ListExo_t<CLASSNO,ITEM>::pHead(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
    
  m_okLock.vGrabWrite();
  
  if (! m_pHeadNode) {
    m_okLock.vDropWrite();
    return nullptr;
  }

  ITEM* pItem = m_pHeadNode->pItem();
  
  m_okLock.vDropWrite();

  return pItem;
}

/*----------------------------------------------------------------------------*/
/* get head node                                                              */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
ListExoNode_t<ITEM>* ListExo_t<CLASSNO,ITEM>::pHeadNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);

  return m_pHeadNode;
}

/*----------------------------------------------------------------------------*/
/* insert item after given item                                               */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
void ListExo_t<CLASSNO,ITEM>::vInsertAfter(ITEM* pInsertThisItem,
                                           ITEM* pAfterThisItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);

  ListExoNode_t<ITEM>* pInsertThisNode = pNewNode(pInsertThisItem);
  ASSERT(! pInsertThisNode->pNextNode());
  ASSERT(! pInsertThisNode->pPrevNode());

  m_okLock.vGrabWrite();

  ListExoNode_t<ITEM>* pAfterThisNode = pFindNode(pAfterThisItem);
  
  if (! pAfterThisNode) {
    m_okLock.vDropWrite();
    NEVER_REACH;
    return;
  }
  
  vInsertNodeAfter(pInsertThisNode,pAfterThisNode);
  
  m_okLock.vDropWrite();
  vValidateList();
}

/*----------------------------------------------------------------------------*/
/* insert item after given node                                               */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
void ListExo_t<CLASSNO,ITEM>::vInsertAfter(ITEM* pInsertThisItem,
                                           ListExoNode_t<ITEM>* pAfterThisNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);

  ListExoNode_t<ITEM>* pInsertThisNode = pNewNode(pInsertThisItem);
  ASSERT(! pInsertThisNode->pNextNode());
  ASSERT(! pInsertThisNode->pPrevNode());

  m_okLock.vGrabWrite();
  
  vInsertNodeAfter(pInsertThisNode,pAfterThisNode);

  m_okLock.vDropWrite();
  vValidateList();
}

/*----------------------------------------------------------------------------*/
/* insert item before given item                                              */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
void ListExo_t<CLASSNO,ITEM>::vInsertBefore(ITEM* pInsertThisItem,
                                            ITEM* pBeforeThisItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
  
  ListExoNode_t<ITEM>* pInsertThisNode = pNewNode(pInsertThisItem);
  ASSERT(! pInsertThisNode->pNextNode());
  ASSERT(! pInsertThisNode->pPrevNode());

  m_okLock.vGrabWrite();

  ListExoNode_t<ITEM>* pBeforeThisNode = pFindNode(pBeforeThisItem);
  
  if (! pBeforeThisNode) {
    m_okLock.vDropWrite();
    NEVER_REACH;
    return;
  }
  
  vInsertNodeBefore(pInsertThisNode,pBeforeThisNode);
  
  m_okLock.vDropWrite();
  vValidateList();
}

/*----------------------------------------------------------------------------*/
/* insert item before given node                                              */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
void ListExo_t<CLASSNO,ITEM>::vInsertBefore(ITEM* pInsertThisItem,
                                           ListExoNode_t<ITEM>* pBeforeThisNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);

  ListExoNode_t<ITEM>* pInsertThisNode = pNewNode(pInsertThisItem);
  ASSERT(! pInsertThisNode->pNextNode());
  ASSERT(! pInsertThisNode->pPrevNode());

  m_okLock.vGrabWrite();

  vInsertNodeBefore(pInsertThisNode,pBeforeThisNode);

  m_okLock.vDropWrite();
  vValidateList();
}

/*----------------------------------------------------------------------------*/
/* get number of items in list                                                */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
i8 ListExo_t<CLASSNO,ITEM>::nItems(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
  
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* peek at nth item of list                                                   */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
ListExoIter_t<ITEM>* ListExo_t<CLASSNO,ITEM>::pNewIter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
  
  ListExoIter_t<ITEM>* pIter
    = new ListExoIter_t<ITEM>((ListExo_t<CN_NONE,ITEM>*)this);
  
  return pIter;
}

/*----------------------------------------------------------------------------*/
/* peek at nth item of list                                                   */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
ITEM* ListExo_t<CLASSNO,ITEM>::pPeek(i8 nThItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
  
  ListExoNode_t<ITEM>* pScanNode = nullptr;
  ITEM* pScanItem = nullptr;

  m_okLock.vGrabWrite();
  
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
  
  m_okLock.vDropWrite();
  // no change, no validate
  
  return pScanItem;
}

/*----------------------------------------------------------------------------*/
/* pop/remove item from head of list                                          */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
ITEM* ListExo_t<CLASSNO,ITEM>::pPop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
     
  m_okLock.vGrabWrite();
  
  if (! m_pHeadNode) {
    ASSERT(! m_nItems);
    m_okLock.vDropWrite();
    return nullptr;
  }

  ListExoNode_t<ITEM>* pWasHeadNode = m_pHeadNode;
  ITEM* pWasHeadItem = pWasHeadNode->pItem();
  
  vCutNode(pWasHeadNode);

  m_okLock.vDropWrite();
  vValidateList();

  //delete pWasHeadNode;

  return pWasHeadItem;
}

/*----------------------------------------------------------------------------*/
/* pop/remove item from tail of list                                          */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
ITEM* ListExo_t<CLASSNO,ITEM>::pPopTail(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
     
  m_okLock.vGrabWrite();
  
  if (! m_pTailNode) {
    ASSERT(! m_nItems);
    m_okLock.vDropWrite();
    return nullptr;
  }

  ListExoNode_t<ITEM>* pWasTailNode = m_pTailNode;
  ITEM* pWasTailItem = pWasTailNode->pItem();
  
  vCutNode(pWasTailNode);

  m_okLock.vDropWrite();
  vValidateList();

  //delete pWasTailNode;
  
  return pWasTailItem;
}

/*----------------------------------------------------------------------------*/
/* push item onto top of list                                                 */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
void ListExo_t<CLASSNO,ITEM>::vPush(ITEM* pItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
  
  ListExoNode_t<ITEM>* pPushingNode = pNewNode(pItem);
  ASSERT(! pPushingNode->pNextNode());
  ASSERT(! pPushingNode->pPrevNode());

  m_okLock.vGrabWrite();
  
  vInsertNodeBefore(pPushingNode,m_pHeadNode);

  m_okLock.vDropWrite();
  vValidateList();
}

/*----------------------------------------------------------------------------*/
/* push item onto end of list                                                 */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
void ListExo_t<CLASSNO,ITEM>::vPushTail(ITEM* pItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
  
  ListExoNode_t<ITEM>* pPushingNode = pNewNode(pItem);
  ASSERT(! pPushingNode->pNextNode());
  ASSERT(! pPushingNode->pPrevNode());

  m_okLock.vGrabWrite();
  
  vInsertNodeAfter(pPushingNode,m_pTailNode);
  
  m_okLock.vDropWrite();
  vValidateList();
}

/*----------------------------------------------------------------------------*/
/* remove item from list                                                      */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
void ListExo_t<CLASSNO,ITEM>::vRemove(ITEM* pItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
  
  m_okLock.vGrabWrite();
  
  ListExoNode_t<ITEM>* pRemovingNode = pFindNode(pItem);
  
  if (! pRemovingNode) {
    m_okLock.vDropWrite();
    vValidateList();
    return;
  }
  
  vCutNode(pRemovingNode);
  
  m_okLock.vDropWrite();
  vValidateList();

  //delete pRemovingNode;
}

/*----------------------------------------------------------------------------*/
/* remove all items in list                                                   */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
void ListExo_t<CLASSNO,ITEM>::vRemoveAll(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
  
  for (ITEM* pItem = pPop(); pItem; pItem = pPop())
    {;}
}

/*----------------------------------------------------------------------------*/
/* remove a node from list                                                    */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
void ListExo_t<CLASSNO,ITEM>::vRemoveNode(ListExoNode_t<ITEM>* pRemovingNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);

  m_okLock.vGrabWrite();

  vCutNode(pRemovingNode);

  m_okLock.vDropWrite();
  vValidateList();

  //delete pRemovingNode;
}

/*----------------------------------------------------------------------------*/
/* get tail item of list                                                      */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
ITEM* ListExo_t<CLASSNO,ITEM>::pTail(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
  
  m_okLock.vGrabWrite();
  
  if (! m_pTailNode) {
    m_okLock.vDropWrite();
    return nullptr;
  }

  ITEM* pItem = m_pTailNode->pItem();
  
  m_okLock.vDropWrite();

  return pItem;
}

/*----------------------------------------------------------------------------*/
/* get tail node                                                              */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
ListExoNode_t<ITEM>* ListExo_t<CLASSNO,ITEM>::pTailNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);

  return m_pTailNode;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* cut exo-node from list, dec m_ntems (exo-node is not deletes)              */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
void ListExo_t<CLASSNO,ITEM>::vCutNode(ListExoNode_t<ITEM>* pCutThisNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
  
  m_okLock.vAssertWrite();
  
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
template <eCN CLASSNO, class ITEM>
ListExoNode_t<ITEM>* ListExo_t<CLASSNO,ITEM>::pFindNode(ITEM* pItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
  
  m_okLock.vAssertWrite();
  
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
template <eCN CLASSNO, class ITEM>
void ListExo_t<CLASSNO,ITEM>::
  vInsertNodeAfter(ListExoNode_t<ITEM>* pInsertThisNode,
                   ListExoNode_t<ITEM>* pAfterThisNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);

  m_okLock.vAssertWrite();

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
template <eCN CLASSNO, class ITEM>
void ListExo_t<CLASSNO,ITEM>::
  vInsertNodeBefore(ListExoNode_t<ITEM>* pInsertThisNode,
                    ListExoNode_t<ITEM>* pBeforeThisNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
  
  m_okLock.vAssertWrite();

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
template <eCN CLASSNO, class ITEM>
ListExoNode_t<ITEM>* ListExo_t<CLASSNO,ITEM>::pNewNode(ITEM* pItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);

  ListExoNode_t<ITEM>* pListExoNode = new ListExoNode_t<ITEM>(pItem);
  
  return pListExoNode;
}

/*----------------------------------------------------------------------------*/
/* validate list                                                              */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
void ListExo_t<CLASSNO,ITEM>::vValidateList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
  
  i8 nItemsCheck;
  ListExoNode_t<ITEM>* pScanNode;

  m_okLock.vGrabWrite();

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

  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM>
Footer* ListExo_t<CLASSNO,ITEM>::aFooter(void)
{
  return &m_xFooter;
}


// § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § §
// § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § §
// § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § §
// § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § § §

namespace BASE
{
template <class ITEM>
class ListExoIter_t : public Object
{
private:
   
  ListExo_t<CN_NONE,ITEM>* m_pListExot;

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

  ListExoIter_t<ITEM>(ListExo_t<CN_NONE,ITEM>* pListExo);
 ~ListExoIter_t(void) noexcept(false);
  
  void  vAfterLast(void);
  void  vBeforeFirst(void);
  ITEM* pFirst(void);
  bool  bIsAfterLast(void);
  bool  bIsBeforeFirst(void);
  bool  bHasNext(void);
  bool  bHasPrev(void);
  ITEM* pNext(void);
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
void* ListExoIter_t<ITEM>::operator new(size_t)
{
  ListExoIter_t<ITEM>* pListExoIter_t;
  
  pListExoIter_t =
    (ListExoIter_t<ITEM>*)Heaps::pAllocate(CN_LISTEXOITER_t);
  
  return pListExoIter_t;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListExoIter_t<ITEM>::operator delete(void* aListExoIter_t)
{
  ListExoIter_t<ITEM>* pListExoIter_t;

  pListExoIter_t = (ListExoIter_t<ITEM>*)aListExoIter_t;

  Heaps::vFree(pListExoIter_t);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ListExoIter_t<ITEM>::ListExoIter_t(ListExo_t<CN_NONE,ITEM>* pListExo)
: Object(CN_LISTEXOITER_t)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LISTEXOITER_t,_FOOTER_);

  m_pListExot = pListExo;

  vBeforeFirst();

  _checkInitialised_(CN_LISTEXOITER_t);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LISTEXOITER_t,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ListExoIter_t<ITEM>::~ListExoIter_t(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LISTEXOITER_t,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* position before first item                                                 */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListExoIter_t<ITEM>::vBeforeFirst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LISTEXOITER_t,_FOOTER_);

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
void ListExoIter_t<ITEM>::vAfterLast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LISTEXOITER_t,_FOOTER_);

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
ITEM* ListExoIter_t<ITEM>::pFirst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LISTEXOITER_t,_FOOTER_);

  vBeforeFirst();
  return pNext();
}

/*----------------------------------------------------------------------------*/
/* return true if the iterator is after the last element in the list          */
/*----------------------------------------------------------------------------*/
template <class ITEM>
bool ListExoIter_t<ITEM>::bIsAfterLast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LISTEXOITER_t,_FOOTER_);

  return !m_pNextCursor;
}

/*----------------------------------------------------------------------------*/
/* return true if the iterator is before the first element in the list        */
/*----------------------------------------------------------------------------*/
template <class ITEM>
bool ListExoIter_t<ITEM>::bIsBeforeFirst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LISTEXOITER_t,_FOOTER_);

  return !m_pPrevCursor;
}

/*----------------------------------------------------------------------------*/
/* Return true if there is another node after this one                        */
/*----------------------------------------------------------------------------*/
template <class ITEM>
bool ListExoIter_t<ITEM>::bHasNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LISTEXOITER_t,_FOOTER_);

  return m_pNextCursor;
}

/*----------------------------------------------------------------------------*/
/* Return true if there is another node before this one                       */
/*----------------------------------------------------------------------------*/
template <class ITEM>
bool ListExoIter_t<ITEM>::bHasPrev(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LISTEXOITER_t,_FOOTER_);

  return m_pPrevCursor;
}

/*----------------------------------------------------------------------------*/
/* get next item in list moving towards the tail                              */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* ListExoIter_t<ITEM>::pNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LISTEXOITER_t,_FOOTER_);
  
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
/* get previous item in list moving towards the head                          */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ITEM* ListExoIter_t<ITEM>::pPrev(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LISTEXOITER_t,_FOOTER_);

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
void ListExoIter_t<ITEM>::vRemove(void)
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
void ListExoIter_t<ITEM>::vInsertAfter(ITEM* pItem)
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
void ListExoIter_t<ITEM>::vInsertBefore(ITEM* pItem)
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
Footer* ListExoIter_t<ITEM>::aFooter(void)
{
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
