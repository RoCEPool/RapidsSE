/*============================================================================*/
/* FileMap.cpp - map of page file on disk                                     */
/*============================================================================*/

#include "BASE.h"
#include "FileMap.hpp"
#include "CmdLineArgs.hpp"

//extern AttachMap*  gpAttachMap;
extern BufferPool* gpBufferPool;
extern CmdLineArgs  gxCmdLineArgs;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
FileMap* BASE::_pFileMap(void* aAddr)
{
  FileMap* pFileMap = reinterpret_cast<FileMap*>(aAddr);

  RASSERT_TYPE(pFileMap,CN_FILEMAP);

  return pFileMap;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* FileMap::operator new(size_t)
{
  FileMap* pFileMap = (FileMap*)gpBufferPool->pAllocOtherBuffer();

  return pPoolCheck<void*>(pFileMap,__func__);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void FileMap::operator delete(void* pFileMapAddress)
{
  FileMap* pFileMap = reinterpret_cast<FileMap*>(pFileMapAddress);

  Heaps::vFree(pFileMap);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
FileMap::FileMap(void)
    : Object(CN_FILEMAP)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILEMAP);
  m_szFilePath = gxCmdLineArgs.m_xSysArgs.m_sPageFilePath;

    vTrace("Created file map %p: sizeof(FileMap)=%lldB, pathToPageFile: %s\n"_Stz,
           this,
           sizeof(FileMap),
           m_szFilePath);

  vInitialise();

  _checkInitialised_(CN_FILEMAP);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  _postcondition_(_ADDRESS_,_MAGIC_,CN_FILEMAP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
FileMap::~FileMap(void)
{
_precondition_(_ADDRESS_,_MAGIC_,CN_FILEMAP,_FOOTER_);

NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* initialise all buffers on disk                                             */
/*----------------------------------------------------------------------------*/
void FileMap::vInitialise(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILEMAP,_FOOTER_);

  bool bProgress = false, bTouch = false;
  i8 iBufferMod;

  i8 i_Buffer = gpBufferPool->m_nNonPageableBuffers;
  // Pickup initialization of the pageable buffers from, where BufferPool left off.
  // From m_nNonPageableBuffers...m_nBuffers-1

  while (C_NIL != i_Buffer) {

    if (bProgress) {
      iBufferMod = i_Buffer % 1000;
      if (! iBufferMod) {
        fflush(stdout);
      }
    }

    FreeBuffer* pFreeBuffer = gpBufferPool->pToFreeBuffer(i_Buffer);

    pFreeBuffer->vSetMagic();
    pFreeBuffer->vSetClassNo(CN_FREEBUFFER);

    //pFreeBuffer->aFooter()->vSetFooter();
    if (bTouch)
      pFreeBuffer->vZeroData();

    pFreeBuffer->vSetFooter();
    pFreeBuffer->vBufferNo(i_Buffer);
    i_Buffer = gpBufferPool->iBufferNext(i_Buffer);
    pFreeBuffer->vNextFree(i_Buffer);

    gpBufferPool->m_nFreeCount++;
  }

  FreeBuffer* pLastBuffer = gpBufferPool->pToFreeBuffer(gpBufferPool->iBufferLast());

  pLastBuffer->vSetMagic();
  pLastBuffer->vSetClassNo(CN_FREEBUFFER);
  //pLastBuffer->aFooter()->vSetFooter();
  if (bTouch)
    pLastBuffer->vZeroData();

  pLastBuffer->vSetFooter();
  pLastBuffer->vBufferNo(gpBufferPool->iBufferLast());
  pLastBuffer->vNextFree(C_NIL);


  //vDump();
}

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* FileMap::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILEMAP);

  return &m_xFooter;
}