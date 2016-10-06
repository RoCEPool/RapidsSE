/*============================================================================*/
/* SEitfList.cpp - SEitf List                                                 */
/*============================================================================*/

#include "Interface.h"

extern Heap*        gpSmallHeap;
extern SEitfList*   gpSEitfList;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SEitfList* Interface::_pSEitfList(void* pAddr)
{
  SEitfList* pSEitfList = reinterpret_cast<SEitfList*>(pAddr);
  
  return pSEitfList;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* SEitfList::operator new(size_t iSize, SEitfList* pSEitfList)
{
  RASSERT(iSize);
  return pSEitfList;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void SEitfList::operator delete(void* pBufferAddress)
{
  RASSERT(pBufferAddress);
  vFatal("fatal: SEitfList class cannot be deleted");
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SEitfList::SEitfList(void)
: Object(CN_SEITFLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITFLIST,_FOOTER_);
  
  ASSERT( ! gpSEitfList );
  
  gpSEitfList = this;
  
  _checkInitialised_(CN_SEITFLIST);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SEITFLIST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SEitfList::~SEitfList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITFLIST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* allocate a seitf                                                           */
/*----------------------------------------------------------------------------*/
SEitf* SEitfList::pAllocSEitf(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITFLIST,_FOOTER_);

  SEitf* pSEitf = _pSEitf(Heap::pAllocate(CN_SEITF));
  
  vAppend(pSEitf);
  
  return pSEitf;
}

/*----------------------------------------------------------------------------*/
/* free a seitf                                                               */
/*----------------------------------------------------------------------------*/
void SEitfList::vFreeSEitf(SEitf* pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITFLIST,_FOOTER_);
  
  vRemove(pSEitf);
  
  Heap::vFree(pSEitf);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SEitfList::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITFLIST);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/