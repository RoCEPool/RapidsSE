/*============================================================================*/
/* Folder_t.hpp - folder of named items                                       */
/*============================================================================*/
#pragma once

namespace Structures
{  
  template <eCN CN, class ITEM, class PARENT>
  class Folder_t : public Object
  {
  protected:
  
    s32     m_sName;   // name of this folder
    PARENT* m_pParent; // parent object / folder
  
    ListNL_t<ITEM>* m_pList; // list of named objects
  
    // must be last
    Footer m_xFooter;

  public:
  
    void* operator new(size_t);
    void  operator delete(void*);

    Folder_t(void);
    ~Folder_t(void) noexcept(false);
  
    Footer* aFooter(void);
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  template <eCN CN, class ITEM, class PARENT>
  Folder_t<CN,ITEM,PARENT>* _pFolder(void* aAddr);
}

////////////////////////////////////////////////////////////////////////////////

using namespace Structures;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM, class PARENT>
Folder_t<CN,ITEM,PARENT>* Structures::_pFolder(void* aAddr)
{
  Folder_t<CN,ITEM,PARENT>* pVector =
    reinterpret_cast<Folder_t<CN,ITEM,PARENT>*>(aAddr);
  
  RASSERT_TYPE(pVector,CN);
  
  return pVector;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM, class PARENT>
void* Folder_t<CN,ITEM,PARENT>::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM, class PARENT>
void Folder_t<CN,ITEM,PARENT>::operator delete(void* pVectorAddress)
{
  Folder_t<CN,ITEM,PARENT>* pVector =
    _pFolder<CN,ITEM,PARENT>(pVectorAddress);
  
  HeapMem::vFree(pVector);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM, class PARENT>
Folder_t<CN,ITEM,PARENT>::Folder_t(void)
: Object(CN)
{
  _precondition_(_MAGIC_,CN,_FOOTER_);
  
  // compute size of this vector proper (in case it is inherited)
  i8 nBytes = sizeof(Folder_t<CN,ITEM,PARENT>);
  
  _checkInitialisedSize_(nBytes);
  
  _postcondition_(_MAGIC_,CN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <eCN CN, class ITEM, class PARENT>
Folder_t<CN,ITEM,PARENT>::~Folder_t(void) noexcept(false)
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
template <eCN CN, class ITEM, class PARENT>
Footer* Folder_t<CN,ITEM,PARENT>::aFooter(void)
{
  _precondition_(_MAGIC_,CN);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
