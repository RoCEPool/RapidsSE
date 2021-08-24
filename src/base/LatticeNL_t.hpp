/*============================================================================*/
/* LatticeNL_t.hpp -- Lattice of Pointers template                            */
/*============================================================================*/
#pragma once

#include "MY.hpp"

extern BufferPool* gpBufferPool;

namespace BASE
{  
  template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
  class LatticeNL_t : public Object
  {  
  private:
    Task* m_pOwningTask; // which task
    
    u8    m_nItems;      // number of items currently in the lattice
    i8    m_i_FirstItem; // subscript of first item cell
    
    u8    m_nFreeCells;  // number of free cells currently in the lattice
    i8    m_i_FirstFree; // subscript of first free cell

    ITEM* m_pItem[MAX_ITEMS];      // array of items (proper objects)
    i8    m_i_NextItem[MAX_ITEMS]; // next item chain
    i8    m_i_NextFree[MAX_ITEMS]; // next free chain
    
    // must be last
    Footer m_xFooter;

  public:
  
    void* operator new(size_t);
    void  operator delete(void*);

    LatticeNL_t(Task* pTask);
   ~LatticeNL_t(void) noexcept(false);

    //ITEM*& operator [](i8 i_Item);

    i8    iAdd(ITEM* pItem);
    ITEM* pGet(i8 i_Item);
    i8    iFirst(void);
    i8    nItems(void);
    i8    iNext(i8 i_After);
    bool  bRemove(ITEM* pItem);
    

    i8    iFind(ITEM* pItem);
    bool  bIsItem(i8 i_Item);
    void  vRemove(i8 i_Item);

    
  private:
  
    void vAddItem(ITEM* pItem, i8 i_Cell);

    void vDumpFrees(void);
    void vDumpItems(void);
    void vDumpNexts(void);

    i8   i_FindItem(ITEM* pItem, i8& i_Prev);

    i8   i_PopFree(void);
    i8   i_PrevItem(ITEM* pItem);
    void vPushFree(i8 i_Free);

    void  vRemoveItem(ITEM* pItem, i8 i_Item, i8 i_Prev);
    
    void  vValidate(Stz* szWhat);

    Footer* aFooter(void);
  };
  
  template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
  LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>* _pLatticeNL(void* aAddr);
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>* BASE::_pLatticeNL(void* aAddr)
{
  LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>* pVector =
    reinterpret_cast<LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>*>(aAddr);
  
  RASSERT_TYPE(pVector,LAT_CN);
  
  return pVector;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
void* LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::operator new(size_t)
{
  void* pObject = Heaps::pAllocate(LAT_CN);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
void LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::operator delete(void* aAddress)
{
  LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>* pLattice =
    _pLatticeNL<LAT_CN,ITEM,MAX_ITEMS>(aAddress);
  
  Heaps::vFree(pLattice);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::LatticeNL_t(Task* pTask)
: Object(LAT_CN)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  
  m_pOwningTask = pTask;
  
  m_nItems      = 0;
  m_i_FirstItem = C_UNKNOWN;
  
  m_nFreeCells  = MAX_ITEMS;
  m_i_FirstFree = 0;

  for (i8 i_Scan = 0; i_Scan < MAX_ITEMS; i_Scan++) {
    m_pItem[i_Scan]      = nullptr;
    m_i_NextItem[i_Scan] = -9;
    m_i_NextFree[i_Scan] = i_Scan + 1;
  }
  m_i_NextFree[MAX_ITEMS - 1] = C_UNKNOWN;

  _setFooter_();
  
  // compute size of this lattice proper (in case it is inherited)
  i8 nBytes = sizeof(LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>);
  
  _checkInitialisedSize_(nBytes);
  
#ifdef WITH_REVISED_LATTICE
  //vValidate("Init"_Stz);
#endif
  
  _postcondition_(_MAGIC_,LAT_CN,_FOOTER_);
}


//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

#ifdef WITH_REVISED_LATTICE

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::~LatticeNL_t(void) noexcept(false)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
    
  //RASSERT(m_pOwningTask == MY::pTask());

}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* add an item                                                                */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
i8 LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::iAdd(ITEM* pItem)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  RASSERT(m_pOwningTask == MY::pTask());

  vValidate("> iAdd"_Stz);

  i8 i_Free = i_PopFree();
  
  vAddItem(pItem,i_Free);
  
  vValidate("< iAdd"_Stz);

  return i_Free;
}

/*----------------------------------------------------------------------------*/
/* get a specific item                                                        */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
ITEM* LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::pGet(i8 i_Item)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  RASSERT(m_pOwningTask == MY::pTask());

  ASSERT(i_Item >= 0 && i_Item < MAX_ITEMS);

  ITEM* pItem = m_pItem[i_Item];
  
  return pItem;
}

/*----------------------------------------------------------------------------*/
/* get subscript of first item                                                */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
i8 LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::iFirst(void)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  RASSERT(m_pOwningTask == MY::pTask());

  return m_i_FirstItem;
}

/*----------------------------------------------------------------------------*/
/* get number of actual items                                                 */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
i8 LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::nItems(void)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  //RASSERT(m_pOwningTask == MY::pTask());

  ASSERT(m_nItems >= 0 && m_nItems <= MAX_ITEMS);
  
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* get next item after given i_Item                                           */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
i8 LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::iNext(i8 i_After)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  RASSERT(m_pOwningTask == MY::pTask());

  ASSERT(i_After >= 0 && i_After <= MAX_ITEMS);

  i8 i_Next = m_i_NextItem[i_After];
  
  return i_Next;
}

/*----------------------------------------------------------------------------*/
/* remove the given item                                                      */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
bool LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::bRemove(ITEM* pItem)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  
  vValidate("> bRemove"_Stz);

  i8 i_Prev;
  
  i8 i_Item = i_FindItem(pItem,i_Prev);
  
  RASSERT(pItem == pGet(i_Item));
  
  vRemoveItem(pItem,i_Item,i_Prev);
  
  RASSERT(! pGet(i_Item));

  vPushFree(i_Item);

  vValidate("< bRemove"_Stz);

  return true;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

#else

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::~LatticeNL_t(void) noexcept(false)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
    
  for (u8 i_Item = (i8)0; (i8)i_Item < (i8)m_nItems; i_Item++) {
    ITEM* pItem = m_pItem[i_Item];
    delete pItem;
  }
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add an item                                                                */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
i8 LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::iAdd(ITEM* pItem)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  
  //RASSERT((! MY::pTask()) || m_pOwningTask == MY::pTask());
  RASSERT(m_pOwningTask == MY::pTask());

  ASSERT(m_i_FirstFree >= 0 && m_i_FirstFree < MAX_ITEMS);
  
  // get item number
  i8 i_Item = m_i_FirstFree;

  if (C_UNKNOWN == i_Item)
    return C_UNKNOWN;
  
  m_i_FirstFree = m_i_NextFree[i_Item];
  if (! (m_i_FirstFree >= 0 && m_i_FirstFree < MAX_ITEMS))
    vDebugPoint();
  ASSERT(m_i_FirstFree >= 0 && m_i_FirstFree < MAX_ITEMS);
  
  m_pItem[i_Item] = pItem;
  
  m_nItems++;
  
  return i_Item;
}

/*----------------------------------------------------------------------------*/
/* add an item                                                                */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
i8 LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::iFind(ITEM* pItem)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  
  //RASSERT((! MY::pTask()) || m_pOwningTask == MY::pTask());
  RASSERT(m_pOwningTask == MY::pTask());

  for (i8 i_Item = 0; i_Item < MAX_ITEMS; i_Item++)
    if (pItem == m_pItem[i_Item])
      return i_Item;
  
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* get subscript of first item                                                */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
i8 LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::iFirst(void)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  
  //RASSERT((! MY::pTask()) || m_pOwningTask == MY::pTask());
  RASSERT(m_pOwningTask == MY::pTask());

  for (i8 i_Item = 0; i_Item < MAX_ITEMS; i_Item++)
    if (bIsItem(i_Item))
      return i_Item;
  
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* get a specific item                                                        */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
ITEM* LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::pGet(i8 i_Item)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  
  //RASSERT((! MY::pTask()) || m_pOwningTask == MY::pTask());
  RASSERT(m_pOwningTask == MY::pTask());

  ASSERT(i_Item >= 0 && i_Item < MAX_ITEMS);
  
  ITEM* pItem = m_pItem[i_Item];
  
  return pItem;
}

/*----------------------------------------------------------------------------*/
/* is elememt an item (or free)                                               */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
bool LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::bIsItem(i8 i_Item)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  
  //RASSERT((! MY::pTask()) || m_pOwningTask == MY::pTask());
  RASSERT(m_pOwningTask == MY::pTask());

  if (gpBufferPool->bValidAddress(m_pItem[i_Item]))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* get number of actual items                                                 */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
i8 LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::nItems(void)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  
  //RASSERT((! MY::pTask()) || m_pOwningTask == MY::pTask());
  //RASSERT(m_pOwningTask == MY::pTask());

  ASSERT(m_nItems >= 0 && m_nItems <= MAX_ITEMS);
  
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* get next i_item                                                            */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
i8 LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::iNext(i8 i_Item)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  
  //RASSERT((! MY::pTask()) || m_pOwningTask == MY::pTask());
  RASSERT(m_pOwningTask == MY::pTask());

  i8 i_Next = i_Item + 1;
  
  for (;;) {
  
    if (i_Next >= MAX_ITEMS)
      return C_UNKNOWN;
  
    if (bIsItem(i_Next))
      return i_Next;
    
    i_Next++;
  }
}

/*----------------------------------------------------------------------------*/
/* remove the given item                                                      */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
bool LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::bRemove(ITEM* pItem)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  
  //RASSERT((! MY::pTask()) || m_pOwningTask == MY::pTask());
  RASSERT(m_pOwningTask == MY::pTask());

  i8 i_Item = iFind(pItem);
  if (C_UNKNOWN == i_Item)
    return false;
  
  vRemove(i_Item);
  return true;
}

/*----------------------------------------------------------------------------*/
/* remove the given item                                                      */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
void LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::vRemove(i8 i_Item)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  
  //RASSERT((! MY::pTask()) || m_pOwningTask == MY::pTask());
  RASSERT(m_pOwningTask == MY::pTask());

  ASSERT(i_Item >= 0 && i_Item < MAX_ITEMS);
  ASSERT(m_i_FirstFree >= 0 && m_i_FirstFree < MAX_ITEMS);

  m_i_NextFree[i_Item] = m_i_FirstFree;
  m_i_FirstFree = i_Item;
  ASSERT(m_i_FirstFree >= 0 && m_i_FirstFree < MAX_ITEMS);

  m_nItems--;
}

#endif

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\


/*----------------------------------------------------------------------------*/
/* [] operator                                                                */
/*----------------------------------------------------------------------------*/
//template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
//ITEM*& LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::operator [](i8 i_Item)
//{
//  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
//
//
//  return m_pItem[i_Item];
//}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add an item                                                                */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
void LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::vAddItem(ITEM* pItem, i8 i_Cell)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  
  // cell should be free
  RASSERT(! m_pItem[i_Cell]);
  
  i8 i_Item = i_Cell;
  
  m_pItem[i_Item] = pItem;
  m_i_NextItem[i_Item] = m_i_FirstItem;
  m_i_FirstItem = i_Item;
  
  m_nItems++;
  
  //vDumpItems();
  //vDumpNexts();
}

/*----------------------------------------------------------------------------*/
/* dump frees                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
void LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::vDumpFrees(void)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  
  printf("Frees #%02lld @%02lld : ",m_nFreeCells,m_i_FirstFree);
  for (i8 i_Scan = 0; i_Scan < MAX_ITEMS; i_Scan++)
    printf("%02lld ",m_i_NextFree[i_Scan]);
  printf("\n");
}

/*----------------------------------------------------------------------------*/
/* dump items                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
void LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::vDumpItems(void)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
    
  printf("Items #%02lld @%02lld : ",m_nItems,m_i_FirstItem);
  
  for (i8 i_Scan = 0; i_Scan < MAX_ITEMS; i_Scan++) {
    ITEM* pItem = m_pItem[i_Scan];
    if (pItem)
      if (gpBufferPool->bValidAddress(pItem))
        printf("^p ",m_i_NextFree[i_Scan]);
      else
        printf("^k ",m_i_NextFree[i_Scan]);
    else
      printf("-- ",m_i_NextFree[i_Scan]);
  }
  printf("\n");
}

/*----------------------------------------------------------------------------*/
/* dump nexts                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
void LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::vDumpNexts(void)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  
  printf("Nexts #%02lld @%02lld : ",m_nItems,m_i_FirstItem);
  for (i8 i_Scan = 0; i_Scan < MAX_ITEMS; i_Scan++)
    printf("%02lld ",m_i_NextItem[i_Scan]);
  printf("\n");
}



/*----------------------------------------------------------------------------*/
/*find given item                                                             */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
i8 LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::i_FindItem(ITEM* pItem, i8& i_Prev)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  
  i8 i_Item = iFirst();
  i_Prev = C_UNKNOWN;
  
  while (C_UNKNOWN != i_Item) {
    
    if (pGet(i_Item) == pItem)
      break;
    
    i_Prev = i_Item;
    i_Item = iNext(i_Item);
  }
  
  return i_Item;
}

/*----------------------------------------------------------------------------*/
/* pop next i_Free from free chain                                            */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
i8 LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::i_PopFree(void)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  
  if (C_UNKNOWN == m_i_FirstFree)
    return C_UNKNOWN;
  
  i8 i_Free = m_i_FirstFree;
  RASSERT(! m_pItem[i_Free]);
  m_i_FirstFree = m_i_NextFree[m_i_FirstFree];
  
  m_nFreeCells--;
  
  m_i_NextFree[i_Free] = -9;
  
  //vDumpFrees();

  return i_Free;
}

/*----------------------------------------------------------------------------*/
/* find the previous cell for given item                                      */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
i8 LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::i_PrevItem(ITEM* pItem)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  
  i8 i_Prev = C_UNKNOWN;
  
  i8 i_Scan = m_i_FirstItem;
  while (C_UNKNOWN != i_Scan) {
    
    if (pItem == m_pItem[i_Scan])
      break;
    
    i_Prev = i_Scan;
    i_Scan = m_i_NextItem[i_Scan];
  }
  
  RASSERT(C_UNKNOWN != i_Scan);
  
  return i_Prev;
}

/*----------------------------------------------------------------------------*/
/* push i_Free onto free chain                                                */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
void LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::vPushFree(i8 i_Free)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  
  m_i_NextFree[i_Free] = m_i_FirstFree;
  m_i_FirstFree = i_Free;
  
  m_nFreeCells++;

  //vDumpFrees();
}

/*----------------------------------------------------------------------------*/
/* remove the given item                                                      */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
void LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::vRemoveItem(ITEM* pItem,
                                                     i8 i_Item,
                                                     i8 i_Prev)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);

  // remove item
  RASSERT(pItem == m_pItem[i_Item]);
  m_pItem[i_Item] = nullptr;
  
  
  if (C_UNKNOWN == i_Prev) {
    // i_Item is first in item chain
    RASSERT(i_Item == m_i_FirstItem);
    m_i_FirstItem = m_i_NextItem[i_Item];
  }
  else {
    // prev is set to items next
    m_i_NextItem[i_Prev] = m_i_NextItem[m_i_NextItem[i_Item]];
  }
  
  m_i_NextItem[i_Item] = -9;
  
  m_nItems--;
  
  //vDumpItems();
  //vDumpNexts();
}

/*----------------------------------------------------------------------------*/
/* validate the lattice                                                       */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
void LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::vValidate(Stz* szWhere)
{
  _precondition_(_MAGIC_,LAT_CN,_FOOTER_);
  
//  printf("%-9s::\n",szWhere);
//
//  vDumpFrees();
//  vDumpItems();
//  vDumpNexts();
  
//  printf("\n");

  RASSERT(m_nItems >= 0 && m_nItems <= MAX_ITEMS);
  
  i8 nItems = 0;
  for (i8 i_Item = m_i_FirstItem;
       C_UNKNOWN != i_Item;
       i_Item = m_i_NextItem[i_Item]) {
    
    RASSERT(i_Item >= 0 && i_Item < MAX_ITEMS);
    
    ITEM* pItem = m_pItem[i_Item];
    //RASSERT(gpBufferPool->bValidAddress(pItem));
    
    nItems++;
    RASSERT(nItems <= MAX_ITEMS);
  }
  RASSERT(nItems == m_nItems);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  RASSERT(m_nFreeCells >= 0 && m_nFreeCells <= MAX_ITEMS);
  
  i8 nFrees = 0;
  for (i8 i_Free = m_i_FirstFree;
       C_UNKNOWN != i_Free;
       i_Free = m_i_NextFree[i_Free]) {
    
    RASSERT(i_Free >= 0 && i_Free < MAX_ITEMS);
    
    ITEM* pFree = m_pItem[i_Free];
    RASSERT(! pFree);
    
    nFrees++;
    RASSERT(nFrees <= MAX_ITEMS);
  }
  RASSERT(nFrees == m_nFreeCells);

}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN LAT_CN, class ITEM, i8 MAX_ITEMS>
Footer* LatticeNL_t<LAT_CN,ITEM,MAX_ITEMS>::aFooter(void)
{
  _precondition_(_MAGIC_,LAT_CN);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
