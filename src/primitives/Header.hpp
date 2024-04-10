/*============================================================================*/
/* Header.hpp -- Header for objects needing magic & class                     */
/*============================================================================*/
#pragma once

//#include "eEC.h"
#ifdef WITH_THREADSTACKSIZE
#include "TlsKeys.hpp"
extern pthread_key_t giTLS_ThreadStackAddr;
extern AttachMap*    gpAttachMap;
#endif
namespace PRIMITIVES
{
  // magic signature
  static const u4 C_MAGIC = 0x4B696A4D; // spells MjiK
                            //K i j M
  
  //--------------------------------------------------------------------------//
  //------------------------------ _Header -----------------------------------//
  //--------------------------------------------------------------------------//
  
  class _Header
  {
  protected:
    union {
      u4   m_uMagic; // 4 byte magic signature
      char m_caMagic[4];
    };
  
    eCN m_eClassNo; // 2 byte class enum
  
    struct {
      u2 m_bWasReceived:1; // object was received
      u2 m_bPostRecvRun:1; // post receive function run
      u2 m_bPersistentFlag:1; // object is persistent
      u2 m__spareBytes:13; // spare bits
    };

    // ! observe there are no constructors
  
  /*--------------------------------------------------------------------------*/
  /*                           -- Inline Functions --                         */
  /*--------------------------------------------------------------------------*/
  public:

  /*--------------------------------------------------------------------------*/
  /* get class number                                                         */
  /*--------------------------------------------------------------------------*/
  inline eCN eGetClassNo(void)
  {
    return m_eClassNo;
  }
  
  /*--------------------------------------------------------------------------*/
  /* set class number                                                         */
  /*--------------------------------------------------------------------------*/
  inline void vSetClassNo(eCN eClassNo)
  {
    m_eClassNo = eClassNo;
  }
  
  /*--------------------------------------------------------------------------*/
  /* test if given class number                                              */
  /*--------------------------------------------------------------------------*/
  inline bool bIsClassNo(eCN eClassNo)
  {
    if (m_eClassNo == eClassNo)
      return true;
    else
      return false;
  }
  
  /*--------------------------------------------------------------------------*/
  /* test if magic signature is valid                                         */
  /*--------------------------------------------------------------------------*/
  inline bool bIsMagicValid(void)
  {
#ifdef WITH_THREADSTACKSIZE
    if(gpAttachMap) {
      bool bSystemReady = AM::SYSTEM::bReady();
      if (bSystemReady) {
        void *pBaseThreadStackAddr = pthread_getspecific(giTLS_ThreadStackAddr);
        if (C_UNKNOWN != (i8) giTLS_ThreadStackAddr &&
            C_UNKNOWN != (i8) pBaseThreadStackAddr &&
            0 != (i8) pBaseThreadStackAddr) {
          i8 iCurStackAddr = 0;
          char *aAddr0 = reinterpret_cast<char *>(pBaseThreadStackAddr);
          char *aAddr1 = reinterpret_cast<char *>(&iCurStackAddr);
          printf("%s %lld %p %p \n", __func__, (aAddr0 - aAddr1) / (C_1MB), aAddr1, aAddr0);
        }
      }
    }
#endif
    if (C_MAGIC == m_uMagic)
      return true;
    else
      return false;
  }

  /*--------------------------------------------------------------------------*/
  /* set magic signature                                                      */
  /*--------------------------------------------------------------------------*/
  inline void vSetMagic(void)
  {
    m_uMagic = C_MAGIC;
  }
  
  /*--------------------------------------------------------------------------*/
  /* sprint the header into readable form                                     */
  /*--------------------------------------------------------------------------*/
  inline void vSprint(Stz* szHeader, i8 nHeaderSize)
  {
    StrLib::vSprint(szHeader,
                    nHeaderSize,
                    nullptr,
                    "Magic: %c%c%c%c Class %03d %s (%s)"_Stz,
                    m_caMagic[0],
                    m_caMagic[1],
                    m_caMagic[2],
                    m_caMagic[3],
                    m_eClassNo,
                    aCN[m_eClassNo],
                    bIsMagicValid() ? "valid magic" : "not-valid magic"
                    );
  }
  
  /*--------------------------------------------------------------------------*/
  /* has a valid class number ?                                               */
  /*--------------------------------------------------------------------------*/
  inline eEC eValidClassNo(void)
  {
    if (m_eClassNo < CN_ENUMS)
      return EC_NONE;
    
    return EC_OBJECT_CLASSNO_INVALID;
  }

  /*--------------------------------------------------------------------------*/
  /* is this a valid header ?                                                 */
  /*--------------------------------------------------------------------------*/
  inline eEC eValidHeader(void)
  {
    eEC eError;
    
    if ((eError = eValidMagic()))
      return eError;
    
    if ((eError = eValidClassNo()))
      return eError;

    return EC_NONE;
  }

  /*--------------------------------------------------------------------------*/
  /* has a valid magic signature                                              */
  /*--------------------------------------------------------------------------*/
  inline eEC eValidMagic(void)
  {
    if (C_MAGIC == m_uMagic)
      return EC_NONE;
    
    return EC_OBJECT_MAGIC_INVALID;
  }

  /*--------------------------------------------------------------------------*/
  /* get was received flag                                                    */
  /*--------------------------------------------------------------------------*/
  inline bool bGetWasReceived(void)
  {
    return m_bWasReceived;
  }
  
  /*--------------------------------------------------------------------------*/
  /* set was received                                                         */
  /*--------------------------------------------------------------------------*/
  inline void vSetWasReceived(bool bWasReceived)
  {
    m_bWasReceived = bWasReceived;
  }
  
  /*--------------------------------------------------------------------------*/
  /* get was received                                                         */
  /*--------------------------------------------------------------------------*/
  inline bool bGetPostRecvRun(void)
  {
    return m_bPostRecvRun;
  }
  
  /*--------------------------------------------------------------------------*/
  /* set post receive has been run flag                                       */
  /*--------------------------------------------------------------------------*/
  inline void vSetPostRecvRun(bool bPostRecvRun)
  {
  m_bPostRecvRun = bPostRecvRun;
  }

  /*--------------------------------------------------------------------------*/
  /* get persistent flag                                                      */
  /*--------------------------------------------------------------------------*/
  inline bool bGetPersistent(void)
  {
    return m_bPersistentFlag;
  }

  /*--------------------------------------------------------------------------*/
  /* set persistent flag                                                      */
  /*--------------------------------------------------------------------------*/
  inline void vSetPersistent(bool bPersistent)
  {
    m_bPersistentFlag = bPersistent;
  }
  


  }; // class _Header
  

  //--------------------------------------------------------------------------//
  //------------------------------ Header ------------------------------------//
  //--------------------------------------------------------------------------//

  class Header : public _Header
  {
  public:
    Header(void);
    Header(eCN eClass);
//   ~Header(void) noexcept(false);
    ~Header(void);

    void vAssertHeader(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
