/*============================================================================*/
/* IoCopy.cpp -- I/O Copy                                                     */
/*============================================================================*/

#include "Chunks.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
IoCopy* Chunks::_pIoCopy(void* aAddr)
{
  IoCopy* pIoCopy = reinterpret_cast<IoCopy*>(aAddr);
  
  RASSERT_TYPE(pIoCopy,CN_IOCOPY);
  
  return pIoCopy;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* IoCopy::operator new(size_t)
{
  IoCopy* pIoCopy = (IoCopy*)HeapMem::pAllocate(CN_IOCOPY);
  
  return pPoolCheck<void*>(pIoCopy);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void IoCopy::operator delete(void* pIoCopyAddress)
{
  IoCopy* pIoCopy = static_cast<IoCopy*>(pIoCopyAddress);
  
  HeapMem::vFree(pIoCopy);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
IoCopy::IoCopy(void)
: Object(CN_IOCOPY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IOCOPY,_FOOTER_);

  m_pInChunk  = new IoChunk(AM::pFmt());
  m_pOutChunk = new IoChunk(AM::pFmt());

  
  _checkInitialised_(CN_IOCOPY);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_IOCOPY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
IoCopy::~IoCopy(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IOCOPY,_FOOTER_);
  
  delete m_pInChunk;
  delete m_pOutChunk;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* close both files                                                           */
/*----------------------------------------------------------------------------*/
void IoCopy::vCloseBoth(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IOCOPY,_FOOTER_);
  
  m_pInChunk->vCloseFile();
  m_pOutChunk->vCloseFile();
}

/*----------------------------------------------------------------------------*/
/* copy one chunk from input to output                                        */
/*----------------------------------------------------------------------------*/
i8 IoCopy::nCopyChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IOCOPY,_FOOTER_);
  
  i8 nBytesRead = m_pInChunk->nReadBytes(-1);
  if (! nBytesRead)
    return 0;
  
  //vTrace("[[chunk]]\n%s\n[[end-chunk]]\n"_Stz,m_pInChunk->szStz());
  
  m_pOutChunk->vCopy(m_pInChunk);
  
  m_pOutChunk->vWriteData();

  return nBytesRead;
}

/*----------------------------------------------------------------------------*/
/* open input file                                                            */
/*----------------------------------------------------------------------------*/
bool IoCopy::bOpenRead(s256& sPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IOCOPY,_FOOTER_);
  
  bool bOpened = m_pInChunk->bOpenRead(sPath);
  
  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
bool IoCopy::bOpenWrite(s256& sPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IOCOPY,_FOOTER_);
  
  bool bOpened = m_pOutChunk->bOpenWrite(sPath);
  
  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* rewind input                                                               */
/*----------------------------------------------------------------------------*/
void IoCopy::vRewindInput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IOCOPY,_FOOTER_);
  
  m_pInChunk->vRewindFile();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* IoCopy::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IOCOPY);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
