/*============================================================================*/
/* IoCopy.cpp -- I/O Copy                                                     */
/*============================================================================*/

#include "Loader.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
IoCopy* Loader::_pIoCopy(void* aAddr)
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

  m_pInChunk  = new IoChunk;
  m_pOutChunk = new IoChunk;

  m_pByteRate = nullptr;
  
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
  delete m_pByteRate;
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
  
  if (m_pByteRate)
    m_pByteRate->vEndTiming();

  m_pInChunk->vCloseFile();
  m_pOutChunk->vCloseFile();
}

/*----------------------------------------------------------------------------*/
/* copy one chunk from input to output                                        */
/*----------------------------------------------------------------------------*/
bool IoCopy::bCopyChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IOCOPY,_FOOTER_);
  
  i8 nBytesRead = m_pInChunk->nReadBytes(-1);
  if (! nBytesRead)
    return false;
  
  //vTrace("[[chunk]]\n%s\n[[end-chunk]]\n"_Stz,m_pInChunk->aStr());
  
  m_pOutChunk->vCopy(m_pInChunk);
  
  if (m_pByteRate)
    m_pByteRate->vAddBytes(nBytesRead);
  
  m_pOutChunk->vWriteData();

  return true;
}

/*----------------------------------------------------------------------------*/
/* open input file                                                            */
/*----------------------------------------------------------------------------*/
void IoCopy::vOpenRead(s256& sPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IOCOPY,_FOOTER_);
  
  m_pInChunk->vOpenRead(sPath);
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
void IoCopy::vOpenWrite(s256& sPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IOCOPY,_FOOTER_);
  
  m_pOutChunk->vOpenWrite(sPath);
}

/*----------------------------------------------------------------------------*/
/* rewind input                                                                 */
/*----------------------------------------------------------------------------*/
void IoCopy::vRewindInput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IOCOPY,_FOOTER_);
  
  m_pInChunk->vRewindFile();
}

/*----------------------------------------------------------------------------*/
/* write data                                                                 */
/*----------------------------------------------------------------------------*/
void IoCopy::vWithStatistics(Stz* aPrefix)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_IOCOPY,_FOOTER_);

  m_pByteRate = new ByteRate;
  
  m_pByteRate->vSetReporting(aPrefix,C_1GB * 10);
  m_pByteRate->vBeginTiming();
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
