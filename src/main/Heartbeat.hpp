/*============================================================================*/
/* Heartbeat.hpp -- heartbeat message                                         */
/*============================================================================*/
#pragma once

class Heartbeat {
  
public:
  
  i8   m_iHeader;            //   8    8
  i8   m_iSeqNo;             //   8   16
  i8   m_iSincePosixEpoch;   //   8   24
  char m_acBuildGitHash[32]; //  32   56
  i8   m_iValue0;            //   8   64
  i8   m_iValue1;            //   8   72
  i8   m_iValue2;            //   8   80
  char m_acString0[32];      //  32  112
  char m_acString1[32];      //  32  144
  char m_acString2[32];      //  32  176
  i8   m_iChecksum;          //   8  184
  i8   m_iFooter;            //   8  192
  char m_acSpare[304];       // 304  496
                             // ---
                             // 496 bytes
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

