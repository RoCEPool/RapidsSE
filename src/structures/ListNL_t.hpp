/*============================================================================*/
/* ListNL_t.hpp -- List of Items (non-locking)                                */
/*============================================================================*/
#pragma once

namespace Resources
{
  template <class ITEM> 
  class ListNL_t : public Object,
                   public ListNL_t__<ITEM>
  {
  protected:

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ListNL_t(eCN eClassNo);
  
    Footer* aFooter(void);
  };
}

using namespace Resources;

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void* ListNL_t<ITEM>::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_TASKLIST);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <class ITEM>
void ListNL_t<ITEM>::operator delete(void* aAddress)
{
  ListNL_t<ITEM>* pList = reinterpret_cast<ListNL_t<ITEM>*>(aAddress);
  
  HeapMem::vFree(pList);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <class ITEM>
ListNL_t<ITEM>::ListNL_t(eCN eClassNo)
: Object(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
    
  _checkInitialisedSize_(sizeof(ListNL_t<ITEM>));
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <class ITEM>
Footer* ListNL_t<ITEM>::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
