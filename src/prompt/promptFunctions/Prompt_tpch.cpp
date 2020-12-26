/*============================================================================*/
/* Prompt_tpch.cpp -- show tpch                                               */
/*============================================================================*/

#include "StorageEngine.h"

PromptFun __Prompt_tpch (
  PF_TPCH,           // prompt function
  "Prompt_tpch"_Stz, // name of the prompt function
  "comment"_Stz,     // comment about the function
  "tpch"_Stz,        // the prompt command
  nullptr,           // the second word
  bPrompt_tpch,      // prompt function
  true,              // enabled

  true,              // visible
  "tpch"_Stz,        // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_tpch(SePrompt* pPrompt)
{
  printf("C_1_BYTE  %llu\n",C_1_BYTE);
  printf("C_2_BYTES %llu\n",C_2_BYTES);
  printf("C_3_BYTES %llu\n",C_3_BYTES);
  printf("C_4_BYTES %llu\n",C_4_BYTES);
  printf("C_5_BYTES %llu\n",C_5_BYTES);
  printf("C_6_BYTES %llu\n",C_6_BYTES);
  printf("C_7_BYTES %llu\n",C_7_BYTES);
  printf("C_8_BYTES %llu\n",C_8_BYTES);
  
  s32 sMagSF;
  
  s32 sMagCustomer;
  s32 sMagLineItem;
  s32 sMagOrders;
  s32 sMagPart;
  s32 sMagPartSupp;
  s32 sMagSupplier;

  s32 sByteMagCustomer;
  s32 sByteMagLineItem;
  s32 sByteMagOrders;
  s32 sByteMagPart;
  s32 sByteMagPartSupp;
  s32 sByteMagSupplier;

  vTrace("        Customer    LineItem    Orders      Part        PartSupp     Supplier\n"_Stz);
  
  for (i8 iSF = 1; iSF <= C_1M; iSF *= 10) {
    MagLib::vUnitMag(iSF,sMagSF);
    vTrace("SF%-4s: "_Stz,sMagSF.azChar());
    
    MagLib::vUnitMag(C_TPCH_CUSTOMER * iSF,sMagCustomer);
    MagLib::vUnitMag(C_TPCH_LINEITEM * iSF,sMagLineItem);
    MagLib::vUnitMag(C_TPCH_ORDERS   * iSF,sMagOrders);
    MagLib::vUnitMag(C_TPCH_PART     * iSF,sMagPart);
    MagLib::vUnitMag(C_TPCH_PARTSUPP * iSF,sMagPartSupp);
    MagLib::vUnitMag(C_TPCH_SUPPLIER * iSF,sMagSupplier);

    MagLib::vByteMag(IntLib::iByteSpace(C_TPCH_CUSTOMER * iSF),sByteMagCustomer);
    MagLib::vByteMag(IntLib::iByteSpace(C_TPCH_LINEITEM * iSF),sByteMagLineItem);
    MagLib::vByteMag(IntLib::iByteSpace(C_TPCH_ORDERS   * iSF),sByteMagOrders);
    MagLib::vByteMag(IntLib::iByteSpace(C_TPCH_PART     * iSF),sByteMagPart);
    MagLib::vByteMag(IntLib::iByteSpace(C_TPCH_PARTSUPP * iSF),sByteMagPartSupp);
    MagLib::vByteMag(IntLib::iByteSpace(C_TPCH_SUPPLIER * iSF),sByteMagSupplier);

    vTrace("%4s(%5s) %4s(%5s) %4s(%5s) %4s(%5s) %4s(%5s) %4s(%5s)\n"_Stz,
           sMagCustomer.azChar(),sByteMagCustomer.azChar(),
           sMagLineItem.azChar(),sByteMagLineItem.azChar(),
           sMagOrders.azChar(),  sByteMagOrders.azChar(),
           sMagPart.azChar(),    sByteMagPart.azChar(),
           sMagPartSupp.azChar(),sByteMagPartSupp.azChar(),
           sMagSupplier.azChar(),sByteMagSupplier.azChar());
  }
  
//  s32 sMag;
//  MagLib::vByteMag(IntLib::iByteSpace(C_1_BYTE), 3,sMag); vTrace("1B: %4s %lld\n"_Stz,sMag.azChar());
//  MagLib::vByteMag(IntLib::iByteSpace(C_2_BYTES),3,sMag); vTrace("2B: %4s\n"_Stz,sMag.azChar());
//  MagLib::vByteMag(IntLib::iByteSpace(C_3_BYTES),3,sMag); vTrace("3B: %4s\n"_Stz,sMag.azChar());
//  MagLib::vByteMag(IntLib::iByteSpace(C_4_BYTES),3,sMag); vTrace("4B: %4s\n"_Stz,sMag.azChar());
//  MagLib::vByteMag(IntLib::iByteSpace(C_5_BYTES),3,sMag); vTrace("5B: %4s\n"_Stz,sMag.azChar());
//  MagLib::vByteMag(IntLib::iByteSpace(C_6_BYTES),3,sMag); vTrace("6B: %4s\n"_Stz,sMag.azChar());
//  MagLib::vByteMag(IntLib::iByteSpace(C_7_BYTES),3,sMag); vTrace("7B: %4s\n"_Stz,sMag.azChar());
//  MagLib::vByteMag(IntLib::iByteSpace(C_8_BYTES),3,sMag); vTrace("8B: %4s\n"_Stz,sMag.azChar());

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
