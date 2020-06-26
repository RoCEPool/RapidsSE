/*============================================================================*/
/* SkipRoots.cpp -- roots of the skips list in an entry buffer                */
/*============================================================================*/

#include "Indexing.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Indexing,EntBufSorter,CN_ENTBUFSORTER)
__aNewFunction(EntBufSorter,CN_ENTBUFSORTER)
__vDeleteFunction(EntBufSorter,CN_ENTBUFSORTER)
__aFooterFunction(EntBufSorter)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntBufSorter::EntBufSorter(void)
: Object(CN_ENTBUFSORTER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFSORTER,_FOOTER_);
  
  MemLib::vZero(m_aaEnt_int,sizeof(m_aaEnt_int),CN_ENTBUFSORTER)
    
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFSORTER,_FOOTER_);
  
  _checkInitialised_(CN_ENTBUFSORTER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
EntBufSorter::~EntBufSorter(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sort ent buffer of KE_INT                                                  */
/*----------------------------------------------------------------------------*/
void EntBufSorter::vSort_int(EntBuffer* pEntBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFSORTER,_FOOTER_);
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
