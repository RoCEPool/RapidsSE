/*============================================================================*/
/* ArrayNL_t.hpp -- Array of Objects template                                 */
/*============================================================================*/
#pragma once

namespace Structures
{  
  template <class ITEM, i8 MAX_ITEMS>
  class ArrayNL_t
  {
  private:
    i8    m_nItems;            // number of actual items
    ITEM  m_oaItem[MAX_ITEMS]; // array of items
  
    // must be last
    Footer m_xFooter;

  public:
    ArrayNL_t(void);
   ~ArrayNL_t(void) noexcept(false);
  
    ITEM* pGet(i8 i_Item);
    i8    nItems(void);
    void  vItems(i8 nItems);
    i8    nMaxItems(void);
  
    Footer* aFooter(void);
  };

  template <class ITEM, i8 MAX_ITEMS>
    ArrayNL_t<ITEM, MAX_ITEMS>* _pArrayNL_t(void* aAddr);
}

////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <class ITEM, i8 MAX_ITEMS>
ArrayNL_t<ITEM, MAX_ITEMS>::ArrayNL_t(void)
{
  aFooter()->vAssertFooter();
  
  m_nItems = 0;
  
  _setFooter_();
  
  aFooter()->vAssertFooter();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <class ITEM, i8 MAX_ITEMS>
ArrayNL_t<ITEM, MAX_ITEMS>::~ArrayNL_t(void) noexcept(false)
{
  aFooter()->vAssertFooter();
  
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get a specific item                                                        */
/*----------------------------------------------------------------------------*/
template <class ITEM, i8 MAX_ITEMS>
ITEM* ArrayNL_t<ITEM, MAX_ITEMS>::pGet(i8 i_Item)
{
  aFooter()->vAssertFooter();
  
  ASSERT(i_Item >= 0 && i_Item < m_nItems);
  
  ITEM* pItem = &m_oaItem[i_Item];
  
  return pItem;
}

/*----------------------------------------------------------------------------*/
/* get number of actual items                                                 */
/*----------------------------------------------------------------------------*/
template <class ITEM, i8 MAX_ITEMS>
i8 ArrayNL_t<ITEM, MAX_ITEMS>::nItems(void)
{
  aFooter()->vAssertFooter();
  
  ASSERT(m_nItems >= 0 && m_nItems < MAX_ITEMS);
  
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* set number of actual items                                                 */
/*----------------------------------------------------------------------------*/
template <class ITEM, i8 MAX_ITEMS>
void ArrayNL_t<ITEM, MAX_ITEMS>::vItems(i8 nItems)
{
  aFooter()->vAssertFooter();
  
  ASSERT(nItems >= 0 && nItems < MAX_ITEMS);
  
  m_nItems = nItems;
}

/*----------------------------------------------------------------------------*/
/* get maximum number of items                                                */
/*----------------------------------------------------------------------------*/
template <class ITEM, i8 MAX_ITEMS>
i8 ArrayNL_t<ITEM, MAX_ITEMS>::nMaxItems(void)
{
  aFooter()->vAssertFooter();
  
  return MAX_ITEMS;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <class ITEM, i8 MAX_ITEMS>
Footer* ArrayNL_t<ITEM, MAX_ITEMS>::aFooter(void)
{
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
