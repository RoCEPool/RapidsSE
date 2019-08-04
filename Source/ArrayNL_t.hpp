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
    i8    m_nItems;           // number of actual items
    ITEM  m_xItem[MAX_ITEMS]; // array of items (not proper objects)
  
    // must be last
    Footer m_xFooter;

  public:
    ArrayNL_t(void);
    ~ArrayNL_t(void) noexcept(false);
  
    ITEM& operator [](i8 iItemNo);
  
    i8    nItems(void);
    void  vItems(i8 nItems);
    ITEM  xGet(i8 iItem);
    i8    nMaxItems(void);
  
    Footer* aFooter(void);
  };
  
  template <class ITEM, i8 MAX_ITEMS>
  ArrayNL_t<ITEM, MAX_ITEMS>* _pArrayNL_t(void* aAddr);
}

using namespace Structures;


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
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
  
  for (i8 i_Item = 0; i_Item < MAX_ITEMS; i_Item++)
    m_xItem[i_Item] = (ITEM)0;
  
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
  
//  for (i8 i_Item = 0; i_Item < m_nItems; i_Item++) {
//    ITEM xItem = m_xItem[i_Item];
//    delete xItem;
//  }
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* [] operator                                                                */
/*----------------------------------------------------------------------------*/
template <class ITEM, i8 MAX_ITEMS>
ITEM& ArrayNL_t<ITEM, MAX_ITEMS>::operator [](i8 iItemNo)
{
  aFooter()->vAssertFooter();
  
  ASSERT(iItemNo >= 0 and iItemNo < MAX_ITEMS);
  
  return m_xItem[iItemNo];
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
/* get a specific item                                                        */
/*----------------------------------------------------------------------------*/
template <class ITEM, i8 MAX_ITEMS>
ITEM ArrayNL_t<ITEM, MAX_ITEMS>::xGet(i8 iItem)
{
  aFooter()->vAssertFooter();
  
  ASSERT(iItem >= 0 && iItem < m_nItems);
  
  ITEM xItem = m_xItem[iItem];
  
  return xItem;
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
