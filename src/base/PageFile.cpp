
/*============================================================================*/
/* PageFile.cpp -- Instance of a page file                                    */
/*============================================================================*/

#include "BASE.h"
extern AttachMap* gpAttachMap;
/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
PageFile* BASE::_pPageFile(void* aAddr)
{
  PageFile* pPageFile = reinterpret_cast<PageFile*>(aAddr);

  RASSERT_TYPE(pPageFile,CN_PAGEFILE);

  return pPageFile;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* PageFile::operator new(size_t)
{
  PageFile* pPageFile = (PageFile*)Heaps::pAllocate(CN_PAGEFILE);

  return pPoolCheck<void*>(pPageFile,__func__);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void PageFile::operator delete(void* pPageFileAddress)
{
  PageFile* pPageFile = static_cast<PageFile*>(pPageFileAddress);

  Heaps::vFree(pPageFile);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PageFile::PageFile(Stz* sPath)
    : Object(CN_PAGEFILE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PAGEFILE,_FOOTER_);

  m_szPFilePath = sPath;
  m_pFile = new File(this);

  vInitialise();
  _checkInitialised_(CN_PAGEFILE);
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PAGEFILE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PageFile::~PageFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PAGEFILE,_FOOTER_);
  m_pFile->vClose();
  delete m_pFile;
}

/*----------------------------------------------------------------------------*/
/* dump PageFile                                                              */
/*----------------------------------------------------------------------------*/
void PageFile::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  UNUSED(eClass);
  UNUSED(pObject);
  UNUSED(aTab);

  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* write pool buffers to disk                                                 */
/*----------------------------------------------------------------------------*/
void PageFile::vInitialise()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PAGEFILE,_FOOTER_);

  //---------------------------------------------------------------------------
  // Sanity check page file path

  //---------------------------------------------------------------------------
  // Open to read and write
  if (0 == m_pFile->bOpenWrite(m_szPFilePath)) {
    vTrace("Error opening page file\n"_Stz);
  }

  //---------------------------------------------------------------------------
  // Check if header and footer are written to file
    // If not, initialize file

  //---------------------------------------------------------------------------
  // Save total file size

  //---------------------------------------------------------------------------
  // Check which areas are written to?

}

/*----------------------------------------------------------------------------*/
/* write data buffer to disk                                                 */
/*----------------------------------------------------------------------------*/
void PageFile::vWriteDataBufferToPFile(RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  BufferId oBufferId = pRowBuffer->oGetBufferId();
  i8 iSeek = oBufferId.m_uBuffOffset;
  m_pFile->vSeek(iSeek);

  i8 iStart  = TickingDevice::iGetTick();

  bool bResult = m_pFile->bWriteBytes((void*)pRowBuffer,C_SIZE_POOL_BUFFER);
  ASSERT(bResult);

  i8 iUpdate = TickingDevice::iGetTick();
  i8 iTicks = (iUpdate - iStart);
  i8 iTicksPerSec = TickingDevice::iTicksPerSec();
  i8 iMilli = iTicks * C_1K / iTicksPerSec;

  vTrace("Writing data buffer %p took %lld ms\n"_Stz, pRowBuffer, iMilli);
}

/*----------------------------------------------------------------------------*/
/* get address of last usable byte in buffer                                  */
/*----------------------------------------------------------------------------*/
//Stz* PoolBuffer::aLastUsableByte(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
//
//  // last byte in buffer
//  Stz* aLastByte = reinterpret_cast<Stz*>(this) + C_SIZE_POOL_BUFFER - 1;
//
//  // last byte before footer
//  Stz* aLastUsableByte = aLastByte - sizeof(C_FOOTER);
//
//  return aLastUsableByte;
//}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* PageFile::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PAGEFILE);

  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/