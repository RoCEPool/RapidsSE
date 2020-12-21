/*============================================================================*/
/* StackNL_t.hpp -- Stack of Objects template                                 */
/*============================================================================*/
#pragma once

namespace Resources
{  
  template <eCN CLASSNO, class ITEM, i8 MAX_ITEMS>
  class StackNL_t : public Object
  {
  private:
    i8    m_nItems;            // number of current items
    ITEM*  m_paItem[MAX_ITEMS];  // array of items

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    StackNL_t(void);
   ~StackNL_t(void) noexcept(false);
  
    ITEM* pPeek(i8 i_Item);
    ITEM* pPop(void);
    void  vPush(ITEM* pItem);
    i8    nItems(void);
    i8    nMaxItems(void);

    Footer* aFooter(void);
  };

  template <eCN CLASSNO, class ITEM, i8 MAX_ITEMS>
    StackNL_t<CLASSNO, ITEM, MAX_ITEMS>* _pStackNL_t(void* aAddr);
}


/*----------------------------------------------------------------------------*/
/*                         -- Template Implementation --                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 MAX_ITEMS>
void* StackNL_t<CLASSNO, ITEM, MAX_ITEMS>::operator new(size_t)
{
  Object* pObject = (Object*)HeapMem::pAllocate(CLASSNO);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 MAX_ITEMS>
void StackNL_t<CLASSNO, ITEM, MAX_ITEMS>::operator delete(void* aAddress)
{
  HeapMem::vFree((Object*)aAddress);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 MAX_ITEMS>
StackNL_t<CLASSNO, ITEM, MAX_ITEMS>::StackNL_t(void)
: Object(CLASSNO)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
  
  aFooter()->vAssertFooter();
  
  m_nItems = 0;
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_paItem,sizeof(m_paItem),CLASSNO);
#endif
  
  _setFooter_();
  
  aFooter()->vAssertFooter();

  _postcondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 MAX_ITEMS>
StackNL_t<CLASSNO, ITEM, MAX_ITEMS>::~StackNL_t(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
  
  aFooter()->vAssertFooter();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get a specific item                                                        */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 MAX_ITEMS>
ITEM* StackNL_t<CLASSNO, ITEM, MAX_ITEMS>::pPeek(i8 i_Item)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
  aFooter()->vAssertFooter();
  
  ASSERT(i_Item >= 0 && i_Item < m_nItems);

  ITEM* pItem = m_paItem[i_Item];
  
  return pItem;
}

/*----------------------------------------------------------------------------*/
/* get a specific item                                                        */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 MAX_ITEMS>
ITEM* StackNL_t<CLASSNO, ITEM, MAX_ITEMS>::pPop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
  aFooter()->vAssertFooter();
  
  ITEM* pItem = m_paItem[m_nItems - 1];
  
  -- m_nItems;
  
  return pItem;
}

/*----------------------------------------------------------------------------*/
/* get a specific item                                                        */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 MAX_ITEMS>
void StackNL_t<CLASSNO, ITEM, MAX_ITEMS>::vPush(ITEM* pItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
  aFooter()->vAssertFooter();
  
  ASSERT(m_nItems < MAX_ITEMS);

  m_paItem[m_nItems] = pItem;
  
  ++ m_nItems;
  
  return pItem;
}

/*----------------------------------------------------------------------------*/
/* get number of current items                                                */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 MAX_ITEMS>
i8 StackNL_t<CLASSNO, ITEM, MAX_ITEMS>::nItems(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
  aFooter()->vAssertFooter();
  
  ASSERT(m_nItems >= 0 && m_nItems <= MAX_ITEMS);
  
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* get maximum number of items                                                */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 MAX_ITEMS>
i8 StackNL_t<CLASSNO, ITEM, MAX_ITEMS>::nMaxItems(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CLASSNO,_FOOTER_);
  aFooter()->vAssertFooter();
  
  return MAX_ITEMS;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN CLASSNO, class ITEM, i8 MAX_ITEMS>
Footer* StackNL_t<CLASSNO, ITEM, MAX_ITEMS>::aFooter(void)
{
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
