/*============================================================================*/
/* ArrayNL_t.hpp -- Array of Objects template                                 */
/*============================================================================*/
#pragma once

namespace Resources
{  
  template <eCN ARY_CN, class ITEM, i8 MAX_ITEMS>
  class ArrayNL_t : public Object
  {
  private:
    i8    m_nItems;            // number of actual items
    ITEM  m_oItem[MAX_ITEMS];  // array of items
  
    // must be last
    Footer m_xFooter;

  public:
    ArrayNL_t(void);
   ~ArrayNL_t(void) noexcept(false);
  
    void  vDecItems(void);
    ITEM* pGet(i8 i_Item, bool bDisableBounds=false);
    ITEM  xGet(i8 i_Item);
    void  vIncItems(void);
    ITEM* aItemArea(void);
    i8    nItems(void);
    void  vItems(i8 nItems);
    i8    nMaxItems(void);
    void  vSet(i8 i_Item, ITEM xItem);

    Footer* aFooter(void);
  };

  template <eCN ARY_CN, class ITEM, i8 MAX_ITEMS>
    ArrayNL_t<ARY_CN, ITEM, MAX_ITEMS>* _pArrayNL_t(void* aAddr);
}

////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eCN ARY_CN, class ITEM, i8 MAX_ITEMS>
ArrayNL_t<ARY_CN, ITEM, MAX_ITEMS>::ArrayNL_t(void)
: Object(ARY_CN)
{
  aFooter()->vAssertFooter();
  
  m_nItems = 0;
  
  _setFooter_();
  
  aFooter()->vAssertFooter();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN ARY_CN, class ITEM, i8 MAX_ITEMS>
ArrayNL_t<ARY_CN, ITEM, MAX_ITEMS>::~ArrayNL_t(void) noexcept(false)
{
  aFooter()->vAssertFooter();
  
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* decrement the number of items                                              */
/*----------------------------------------------------------------------------*/
template <eCN ARY_CN, class ITEM, i8 MAX_ITEMS>
void ArrayNL_t<ARY_CN, ITEM, MAX_ITEMS>::vDecItems(void)
{
  aFooter()->vAssertFooter();

  m_nItems--;
}

/*----------------------------------------------------------------------------*/
/* get a specific item                                                        */
/*----------------------------------------------------------------------------*/
template <eCN ARY_CN, class ITEM, i8 MAX_ITEMS>
ITEM* ArrayNL_t<ARY_CN, ITEM, MAX_ITEMS>::pGet(i8 i_Item, bool bDisableBounds)
{
  aFooter()->vAssertFooter();
  
  ASSERT(bDisableBounds || (i_Item >= 0 && i_Item < m_nItems));

  ITEM* pItem = &m_oItem[i_Item];
  
  return pItem;
}

/*----------------------------------------------------------------------------*/
/* get a specific item                                                        */
/*----------------------------------------------------------------------------*/
template <eCN ARY_CN, class ITEM, i8 MAX_ITEMS>
ITEM ArrayNL_t<ARY_CN, ITEM, MAX_ITEMS>::xGet(i8 i_Item)
{
  aFooter()->vAssertFooter();
  
  ITEM xItem = m_oItem[i_Item];
  
  return xItem;
}

/*----------------------------------------------------------------------------*/
/* increment the number of items                                              */
/*----------------------------------------------------------------------------*/
template <eCN ARY_CN, class ITEM, i8 MAX_ITEMS>
void ArrayNL_t<ARY_CN, ITEM, MAX_ITEMS>::vIncItems(void)
{
  aFooter()->vAssertFooter();

  m_nItems++;
}

/*----------------------------------------------------------------------------*/
/* get a specific item                                                        */
/*----------------------------------------------------------------------------*/
template <eCN ARY_CN, class ITEM, i8 MAX_ITEMS>
ITEM* ArrayNL_t<ARY_CN, ITEM, MAX_ITEMS>::aItemArea(void)
{
  aFooter()->vAssertFooter();
  
  ITEM* pItem = m_oItem;
  
  return pItem;
}

/*----------------------------------------------------------------------------*/
/* get number of actual items                                                 */
/*----------------------------------------------------------------------------*/
template <eCN ARY_CN, class ITEM, i8 MAX_ITEMS>
i8 ArrayNL_t<ARY_CN, ITEM, MAX_ITEMS>::nItems(void)
{
  aFooter()->vAssertFooter();
  
  ASSERT(m_nItems >= 0 && m_nItems <= MAX_ITEMS);
  
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* set number of actual items                                                 */
/*----------------------------------------------------------------------------*/
template <eCN ARY_CN, class ITEM, i8 MAX_ITEMS>
void ArrayNL_t<ARY_CN, ITEM, MAX_ITEMS>::vItems(i8 nItems)
{
  aFooter()->vAssertFooter();
  
  ASSERT(nItems >= 0 && nItems <= MAX_ITEMS);
  
  m_nItems = nItems;
}

/*----------------------------------------------------------------------------*/
/* get maximum number of items                                                */
/*----------------------------------------------------------------------------*/
template <eCN ARY_CN, class ITEM, i8 MAX_ITEMS>
i8 ArrayNL_t<ARY_CN, ITEM, MAX_ITEMS>::nMaxItems(void)
{
  aFooter()->vAssertFooter();
  
  return MAX_ITEMS;
}

/*----------------------------------------------------------------------------*/
/* set a specific item                                                        */
/*----------------------------------------------------------------------------*/
template <eCN ARY_CN, class ITEM, i8 MAX_ITEMS>
void ArrayNL_t<ARY_CN, ITEM, MAX_ITEMS>::vSet(i8 i_Item, ITEM xItem)
{
  aFooter()->vAssertFooter();
  
  m_oItem[i_Item] = xItem;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN ARY_CN, class ITEM, i8 MAX_ITEMS>
Footer* ArrayNL_t<ARY_CN, ITEM, MAX_ITEMS>::aFooter(void)
{
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
