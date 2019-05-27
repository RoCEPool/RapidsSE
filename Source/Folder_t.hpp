/*============================================================================*/
/* Folder_t.hpp - named folder of items                                       */
/*============================================================================*/
#pragma once

namespace Structures
{  
  template <eCN CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
  class Folder_t : public Object
  {
  //friend class TokenVec;
  
  protected:
  
    // must be last
    Footer m_xFooter;

  public:
  
    void* operator new(size_t);
    void  operator delete(void*);

    Folder_t(void);
    ~Folder_t(void) noexcept(false);
  
    Footer* aFooter(void);
  };
  
  template <eCN CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
  Folder_t<CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>* _pFolder(void* aAddr);
}

////////////////////////////////////////////////////////////////////////////////

using namespace Structures;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
Folder_t<CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>* Structures::_pFolder(void* aAddr)
{
  Folder_t<CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>* pVector =
    reinterpret_cast<Folder_t<CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>*>(aAddr);
  
  RASSERT_TYPE(pVector,CN);
  
  return pVector;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
void* Folder_t<CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
void Folder_t<CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>::operator delete(void* pVectorAddress)
{
  Folder_t<CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>* pVector =
    _pFolder<CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>(pVectorAddress);
  
  HeapMem::vFree(pVector);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
Folder_t<CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>::Folder_t(void)
: Object(CN)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  // compute size of this vector proper (in case it is inherited)
  i8 nBytes = sizeof(Folder_t<CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>);
  
  _checkInitialisedSize_(nBytes);
  
  _postcondition_(_MAGIC_,CN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
Folder_t<CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>::~Folder_t(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM, eCN ITEM_CN, i8 MAX_ITEMS, class SUB>
Footer* Folder_t<CN,ITEM,ITEM_CN,MAX_ITEMS,SUB>::aFooter(void)
{
  _precondition_(_MAGIC_,CN);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
