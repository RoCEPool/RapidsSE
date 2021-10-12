/*============================================================================*/
/* vFLAT_Structures.h -- Structures Fragement for Inflate of crwBuffer        */
/*============================================================================*/

  struct S_Dtm {
    union {

      Bcd   m_wBcd;
      Bol   m_wBol;
      Dte   m_wDte;
      Edt   m_wEdt;
      Flt   m_wFlt;
      Int   m_wInt;
      Ivl   m_wIvl;
      Tme   m_wTme;
      Tsp   m_wTsp;

      Stz*  m_szVch;

      Object*  m_pObject;

      CodeCell*  m_aFip;
      i8     m_iFip;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

      i8    m_iWrd;
      d8    m_dWrd;
      char* m_azStr;
      byte  m_yaByte[C_BYTES_PER_WORD];
    };
  };
  struct S_Row
  {
    union {
      u4   m_uMagic; // 4 byte magic signature
      char m_caMagic[4];
    };

    eCN m_eClassNo; // 2 byte class enum

    struct {
      u2 m_bWasReceived:1; // object was received
      u2 m_bPostRecvRun:1; // post receive function run
      u2 m__spareBytes:14; // spare bits
    };

    S_Dtm m_wxaColumnCell[1];
  };

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
