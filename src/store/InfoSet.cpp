/*============================================================================*/
/* InfoSet.cpp -- Information Set                                             */
/*============================================================================*/

#include "BASE.h"

#include "RowBuffer.hpp"

#include "InfoPix.hpp"
#include "InfoSet.hpp"

#include "OpSpec.hpp"
#include "PipelineSpec.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
InfoSet* STORE::_pInfoSet(void* aAddr)
{
  InfoSet* pInfoSet = reinterpret_cast<InfoSet*>(aAddr);
  
  return pInfoSet;
}

/*----------------------------------------------------------------------------*/
/* allocate from heap                                                         */
/*----------------------------------------------------------------------------*/
void* InfoSet::operator new(size_t)
{
  InfoSet* pInfoSet =
    static_cast<InfoSet*>(HeapMem::pAllocate(CN_INFOSET));
  
  return pInfoSet;
}

/*----------------------------------------------------------------------------*/
/* free back to the heap                                                      */
/*----------------------------------------------------------------------------*/
void InfoSet::operator delete(void* aAddress)
{
  Object* pObject = static_cast<Object*>(aAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                           received over socket */
/*----------------------------------------------------------------------------*/
InfoSet::InfoSet(void)
: InfoSetBase(CN_INFOSET)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
  m_eOpInfoSet = OI_NONE;
  _checkInitialised_(CN_INFOSET);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
InfoSet::InfoSet(StreamDef* pStreamDef)
: InfoSetBase(CN_INFOSET, pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
  m_eOpInfoSet = OI_NONE;
  
  _checkInitialised_(CN_INFOSET);
  
  vFormInfoPix(pStreamDef);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
InfoSet::~InfoSet(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
}

/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* formulate info pix from stream def                                         */
/*----------------------------------------------------------------------------*/
void InfoSet::vFormInfoPix(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
  i8 nColumns = pStreamDef->nColumns();
  
  i8 nInitialSize = sizeof(InfoPix) + ((nColumns - 1) * sizeof(ColPix));
  
  InfoPix* pInfoPix = new ((InfoPix*)ayAllotAux0(nInitialSize)) InfoPix;
  
  ColPix* aColPix = pInfoPix->aColPix();
    
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
    Column* pColumn = pStreamDef->pGetColumn(i_Col);

    Stz* szColNmSrc = pColumn->szColNm();
    
    i8 nColNmSize = szColNmSrc->nSize();
    
    Stz* szColNmDst = (Stz*)ayExtendAux0(nColNmSize);
    
    szColNmDst->vCopy(szColNmSrc);
  }
  
  
}

/*----------------------------------------------------------------------------*/
/* get the infopix for this infoset ( aux 0 )                                 */
/*----------------------------------------------------------------------------*/
InfoPix* InfoSet::pInfoPix(void) {
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
  InfoPix* pInfoPix = _pInfoPix(ayAux0());
  
  return pInfoPix;
}

/*----------------------------------------------------------------------------*/
/* set operator infoset                                                       */
/*----------------------------------------------------------------------------*/
void InfoSet::vOpInfoSet(eOI eOpInfoSet) {
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
  m_eOpInfoSet = eOpInfoSet;
}

/*----------------------------------------------------------------------------*/
/* get operator infoset                                                       */
/*----------------------------------------------------------------------------*/
eOI InfoSet::eOpInfoSet(void) {
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_INFOSET);
  
  return m_eOpInfoSet;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
