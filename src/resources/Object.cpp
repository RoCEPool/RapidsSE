/*============================================================================*/
/* Object.cpp -- Proper Object in system                                      */
/*============================================================================*/

#include "Resources.h"

extern SizeHeap  gxaSizeHeap[];

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Object::Object(eCN eClassNo)
: Header(eClassNo)
{
  
  _precondition_(_MAGIC_,eClassNo);
}

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Stz* Object::szClassNm(void)
{
  eCN eClass = this->eClass();

  Stz* szName = aCN[eClass];
  
  return szName;
}

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Object::aCalcFooterAddress(void)
{
  eCN eClass = this->eClass();

  Footer* aFooter =
    aAdd<Footer*>(this,gxaSizeHeap[eClass].m_nClassSize-sizeof(Footer));

  if (aFooter->bValidFooter())
    return aFooter;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* inspect object                                                             */
/*----------------------------------------------------------------------------*/
void Object::vInspectObject(void)
{
  vTrace("Inspecting %p %-12s:\n"_Stz,this,szClassNm());
  
  vMemoryDump();
}

/*----------------------------------------------------------------------------*/
/* memory dump                                                                */
/*----------------------------------------------------------------------------*/
void Object::vMemoryDump(void)
{
  eCN eClass = this->eClass();
  i8 nSize = nSizeOfObject();
  
  MemLib::vDumpWords(this,nSize);
}

/*----------------------------------------------------------------------------*/
/* get size of object                                                         */
/*----------------------------------------------------------------------------*/
i8 Object::nSizeOfObject(void)
{
  eCN eClass = this->eClass();

  i8 nSize = gxaSizeHeap[eClass].m_nClassSize;
  
  return nSize;
}

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
eEC Object::eValidObject(void)
{
  eEC eError;
  
  if ((eError = this->eValidHeader()))
    return eError;
  
  eCN eClass = this->eClass();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Footer* aFooter =
    aAdd<Footer*>(this,gxaSizeHeap[eClass].m_nClassSize-sizeof(Footer));

  if ((eError = aFooter->eValidFooter()))
    return eError;

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* get when allocated                                                         */
/*----------------------------------------------------------------------------*/
i8 Object::iWhenAllocated(void)
{
  Footer* aFooter = aCalcFooterAddress();
  
  i8 iWhen = aFooter ? aFooter->iWhenAllocated() : -1;
  
  return iWhen;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Object::aFooter(void)
{
  return nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
