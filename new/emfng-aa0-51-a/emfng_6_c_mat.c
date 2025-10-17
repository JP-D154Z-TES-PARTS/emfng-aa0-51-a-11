/* emfng-pa000-5000-a-M4 */
/*********************************************************************/
/*  Copyright Toyota Motor Corporation                               */
/*********************************************************************/

/*********************************************************************/
/* オブジェクト名 |  失火検出異常判定部                              */
/* 注意事項       | 【失火OBD有】AND【6気筒】                        */
/*-------------------------------------------------------------------*/
/*     個別変更履歴 （個別変更時は以下に変更日、内容を記入すること） */
/*********************************************************************/

/*-------------------------------------------------------------------*/
/* 適合定数のセクション定義                                          */
/*-------------------------------------------------------------------*/
#pragma ghs section rozdata = ".mat_emfng"

/*-------------------------------------------------------------------*/
/* ヘッダファイルのインクルード                                      */
/*-------------------------------------------------------------------*/
#include <../inc/common.h>
#include <../inc/gllib.h>
#include <engsrc/espc/ejcc.h>           /* JEEFI,JEEGTYPE,JEHIPFI_E,JEMFHOUKI */
                                        /* JEMFOTLV,JEOBDSIMUKE,JEOOBD,u1g_EJCC_NCYL */
                                        /* u1g_EJCC_USAMF,u1g_EJCC_JCUT,u1g_EJCC_ECUT,u1g_EJCC_CCUT,u1g_EJCC_CD34,u1g_EJCC_DUAL,JENGPF_E,u1g_EJCC_V6CYL */
#include <engsrc/inc/elsb.h>

#include <eng/emf/emfng.h>

/*-------------------------------------------------------------------*/
/* コンパイラバージョン定義                                          */
/*-------------------------------------------------------------------*/
#define MK32_ID                         (0x00000040)             /* R9コンパイラを使用 */

#ifdef __GHS_VERSION_NUMBER
 #if ( ( __GHS_VERSION_NUMBER >= 201355 ) \
    && ( __GHS_VERSION_NUMBER < 201400 ) )
 #else
  #error "コンパイルされるコンパイラが異なっています。"
 #endif
#endif

/*-------------------------------------------------------------------*/
/* コンパイルSW未定義チェック                                        */
/*-------------------------------------------------------------------*/
#if !defined JEEFI
#error "JEEFI is undeclared"
#endif

#if !defined JEEGTYPE
#error "JEEGTYPE is undeclared"
#endif

#if !defined JEHIPFI_E
#error "JEHIPFI_E is undeclared"
#endif

#if !defined JEMFHOUKI
#error "JEMFHOUKI is undeclared"
#endif

#if !defined JEMFOTLV
#error "JEMFOTLV is undeclared"
#endif

#if !defined JEOBDSIMUKE
#error "JEOBDSIMUKE is undeclared"
#endif

#if !defined JEOOBD
#error "JEOOBD is undeclared"
#endif

#if !defined u1g_EJCC_NCYL
#error "u1g_EJCC_NCYL is undeclared"
#endif

#if !defined u1g_EJCC_USAMF
#error "u1g_EJCC_USAMF is undeclared"
#endif

#if !defined u1g_EJCC_JCUT
#error "u1g_EJCC_JCUT is undeclared"
#endif

#if !defined u1g_EJCC_ECUT
#error "u1g_EJCC_ECUT is undeclared"
#endif

#if !defined u1g_EJCC_CCUT
#error "u1g_EJCC_CCUT is undeclared"
#endif

#if !defined u1g_EJCC_CD34
#error "u1g_EJCC_CD34 is undeclared"
#endif

#if !defined u1g_EJCC_DUAL
#error "u1g_EJCC_DUAL is undeclared"
#endif

#if !defined JENGPF_E
#error "JENGPF_E is undeclared"
#endif

#if !defined u1g_EJCC_V6CYL
#error "u1g_EJCC_V6CYL is undeclared"
#endif

/*---------------------------------------------------------------------*/
/* 固定定数                                                            */
/*---------------------------------------------------------------------*/
/* $$$固定定数_標準_v4.11 */
/* 固定定数 EMFNG_MFSTOTIは、emfng_l_mat.cファイルに定義しました。 */
/* #if (JETCORE_D == u1g_EJCC_USE) && (JEOBDUDS_D == u1g_EJCC_USE) */   /* 【t-CORE対応有】AND【OBDonUDS対応有】 */
/* 固定定数 EMFNG_CXMF1PASは、emfng_l_mat.cファイルに定義しました。 */
/* 固定定数 EMFNG_CXMFPASは、emfng_l_mat.cファイルに定義しました。 */
/* #endif */                                                            /* JETCORE_D JEOBDUDS_D */
/* $$$固定定数_標準_v4.11 */

/*-------------------------------------------------------------------*/
/* 適合定数                                                          */
/*-------------------------------------------------------------------*/
/* $$$適合定数_標準_v4.11 */

#if JEMFOTLV == u1g_EJCC_NOT_USE                                        /* 【触媒OTﾚﾍﾞﾙ限定無】 */
  #if JEMFHOUKI == u1g_EJCC_USAMF                                       /* 【触媒OTﾚﾍﾞﾙ限定無】AND【対米法規】 */
volatile const s2 s2s_emfng_MFDD = s2g_glround((1.5)/(256./128./256.)); /* m=emfng,lsb=256/128/256,ofs=,unit=% :EM悪化ﾚﾍﾞﾙ失火率 */
volatile const u2 u2s_emfng_MFD1 = (u2)(((0.9)/(2./256./256.))+0.5);    /* m=emfng,lsb=2/256/256,ofs=,unit= :EM悪化ﾚﾍﾞﾙ失火検出率 */

  #else                                                                 /* 【触媒OTﾚﾍﾞﾙ限定無】AND【対米法規以外】 */
volatile const s2 s2s_emfng_MFDD = s2g_glround((1.5)/(256./128./256.)); /* m=emfng,lsb=256/128/256,ofs=,unit=% :EM悪化ﾚﾍﾞﾙ失火率 */
volatile const u2 u2s_emfng_MFD1 = (u2)(((0.9)/(2./256./256.))+0.5);    /* m=emfng,lsb=2/256/256,ofs=,unit= :EM悪化ﾚﾍﾞﾙ失火検出率 */
  #endif                                                                /* JEMFHOUKI */
#else                                                                   /* 【触媒OTﾚﾍﾞﾙ限定有】 */
volatile const s2 s2s_emfng_MFDD = s2g_glround((16.6)/(256./128./256.));    /* m=emfng,lsb=256/128/256,ofs=,unit=% :EM悪化ﾚﾍﾞﾙ失火率 */
volatile const u2 u2s_emfng_MFD1 = (u2)(((0.9)/(2./256./256.))+0.5);    /* m=emfng,lsb=2/256/256,ofs=,unit= :EM悪化ﾚﾍﾞﾙ失火検出率 */
#endif                                                                  /* JEMFOTLV */

volatile const s2 s2s_emfng_MFNEL3 = s2g_ELSB_NE(1050.);                /* m=emfng,lsb=12800/64/256,ofs=,unit=rpm :アイドル領域判定値 */
volatile const u2 u2g_emfng_LJMFOT = (u2)(((200.)/(2./u1g_EJCC_NCYL))+0.5); /* m=emfng,lsb=2/u1g_EJCC_NCYL,ofs=,unit=rev :触媒OTﾚﾍﾞﾙ異常判定実行ﾀｲﾐﾝｸﾞ */
volatile const u2 u2s_emfng_LJMFOTI = (u2)(((170.)/(2./u1g_EJCC_NCYL))+0.5);    /* m=emfng,lsb=2/u1g_EJCC_NCYL,ofs=,unit=rev :触媒OTﾚﾍﾞﾙｱｲﾄﾞﾙ時異常判定実行ﾀｲﾐﾝｸﾞ */
volatile const u2 u2s_emfng_LJMFOTF = (u2)(((190.)/(2./u1g_EJCC_NCYL))+0.5);    /* m=emfng,lsb=2/u1g_EJCC_NCYL,ofs=,unit=rev :触媒OTﾚﾍﾞﾙ異常判定実行ﾀｲﾐﾝｸﾞ */
volatile const u1 u1s_emfng_CXMFH = (u1)(((4.)/(1.))+0.5);              /* m=emfng,lsb=1,ofs=,unit=回 :EM悪化ﾚﾍﾞﾙ異常検出回数判定値 */
volatile const u1 u1s_emfng_CXMFL = (u1)(((1.)/(1.))+0.5);              /* m=emfng,lsb=1,ofs=,unit=回 :EM悪化ﾚﾍﾞﾙ異常検出回数判定値 */
volatile const u1 u1s_emfng_CXMF1H = (u1)(((3.)/(1.))+0.5);             /* m=emfng,lsb=1,ofs=,unit=回 :触媒OTﾚﾍﾞﾙ異常検出回数判定値 */
volatile const u1 u1s_emfng_CXMF1L = (u1)(((1.)/(1.))+0.5);             /* m=emfng,lsb=1,ofs=,unit=回 :触媒OTﾚﾍﾞﾙ異常検出回数判定値 */
volatile const u1 u1s_emfng_MFSTOT = (u1)(((70.)/(1.))+0.5);            /* m=emfng,lsb=1,ofs=,unit=回 :気筒別失火回数判定値 */
volatile const u1 u1s_emfng_MFSTEMI = (u1)(((72.)/(1.))+0.5);           /* m=emfng,lsb=1,ofs=,unit=回 :ｱｲﾄﾞﾙ中気筒別失火回数判定値 */
volatile const u2 u2g_emfng_LJMFEM = (u2)(((1000.)/(2./u1g_EJCC_NCYL))+0.5);    /* m=emfng,lsb=2/u1g_EJCC_NCYL,ofs=,unit=rev :EM悪化ﾚﾍﾞﾙ異常判定実行ﾀｲﾐﾝｸﾞ */
volatile const u1 u1s_emfng_CJPMF = (u1)(((3.)/(1.))+0.5);              /* m=emfng,lsb=1,ofs=,unit=回 :ｾﾞﾛｸﾛｽｶｳﾝﾀ判定値 */
volatile const u1 u1g_emfng_CDTMFWA = (u1)(((10.)/(1.))+0.5);           /* m=emfng,lsb=1,ofs=,unit=回 :EM悪化判定用悪路ｶｳﾝﾀ判定値 */
volatile const u1 u1s_emfng_CDTMF2 = (u1)(((3.)/(1.))+0.5);             /* m=emfng,lsb=1,ofs=,unit=回 :触媒OT判定用悪路ｶｳﾝﾀ判定値 */
volatile const u1 u1s_emfng_KLDMFOT = (u1)(((90.)/(1.))+0.5);           /* m=emfng,lsb=1,ofs=,unit=回 :触媒OTﾚﾍﾞﾙ失火回数判定値(100点火/1気筒辺り) */
volatile const u1 u1g_emfng_CATN = (u1)(((2.)/(1.))+0.5);               /* m=emfng,lsb=1,ofs=,unit= :バンク数区別の有無(無:1,有:2) */
volatile const u1 u1s_emfng_KLD1 = (u1)(((0.75)/(2./256.))+0.5);        /* m=emfng,lsb=2/256,ofs=,unit= :対向気筒失火検出率下限 */
volatile const u1 u1s_emfng_KLD2 = (u1)(((1.1)/(2./256.))+0.5);         /* m=emfng,lsb=2/256,ofs=,unit= :対向気筒失火検出率上限 */

#if (JEOBDSIMUKE == u1g_EJCC_JCUT) || (JEOBDSIMUKE == u1g_EJCC_ECUT) || (JEOBDSIMUKE == u1g_EJCC_CCUT)  /* 【JPNｶｯﾄ】OR【Euroｶｯﾄ】OR【CARBｶｯﾄ】 */
volatile const s2 s2g_emfng_MFOTL = s2g_glround((33.3)/(256./128./256.));   /* m=emfng,lsb=256/128/256,ofs=,unit=% :触媒OTﾚﾍﾞﾙ失火判定下限値 */

#elif (JEMFHOUKI == u1g_EJCC_USAMF) || (JEOBDSIMUKE == u1g_EJCC_CD34)   /* (【JPNｶｯﾄ】OR【Euroｶｯﾄ】OR【CARBｶｯﾄ】)以外 AND(【対米法規】OR【高度ﾅJ-OBD】) */
volatile const s2 s2g_emfng_MFOTL = s2g_glround((5.)/(256./128./256.)); /* m=emfng,lsb=256/128/256,ofs=,unit=% :触媒OTﾚﾍﾞﾙ失火判定下限値 */

#else                                                                   /* ((【JPNｶｯﾄ】OR【Euroｶｯﾄ】OR【CARBｶｯﾄ】)OR (【対米法規】OR【高度ﾅJ-OBD】))以外 */
volatile const s2 s2g_emfng_MFOTL = s2g_glround((0.)/(256./128./256.)); /* m=emfng,lsb=256/128/256,ofs=,unit=% :触媒OTﾚﾍﾞﾙ失火判定下限値 */
#endif                                                                  /* JEOBDSIMUKE JEMFHOUKI */

#if JEMFOTLV == u1g_EJCC_NOT_USE                                        /* 【触媒OTﾚﾍﾞﾙ限定無】 */
  #if JEMFHOUKI == u1g_EJCC_USAMF                                       /* 【触媒OTﾚﾍﾞﾙ限定無】AND【対米法規】 */
volatile const u1 u1s_emfng_ST1RTO = (u1)(((1.)/(8./256.))+0.5);        /* m=emfng,lsb=8/256,ofs=,unit=倍 :ｺｰﾙﾄﾞｽﾀｰﾄ時EM悪化ﾚﾍﾞﾙ失火回数判定値算出係数 */
volatile const u1 u1s_emfng_ST2RTO = (u1)(((5.)/(8./256.))+0.5);        /* m=emfng,lsb=8/256,ofs=,unit=倍 :ﾎｯﾄｽﾀｰﾄ時EM悪化ﾚﾍﾞﾙ失火回数判定値算出係数 */

  #else                                                                 /* 【触媒OTﾚﾍﾞﾙ限定無】AND【対米法規以外】 */
volatile const u1 u1s_emfng_ST1RTO = (u1)(((5.)/(8./256.))+0.5);        /* m=emfng,lsb=8/256,ofs=,unit=倍 :ｺｰﾙﾄﾞｽﾀｰﾄ時EM悪化ﾚﾍﾞﾙ失火回数判定値算出係数 */
volatile const u1 u1s_emfng_ST2RTO = (u1)(((5.)/(8./256.))+0.5);        /* m=emfng,lsb=8/256,ofs=,unit=倍 :ﾎｯﾄｽﾀｰﾄ時EM悪化ﾚﾍﾞﾙ失火回数判定値算出係数 */
  #endif                                                                /* JEMFHOUKI */
#else                                                                   /* 【触媒OTﾚﾍﾞﾙ限定有】 */
volatile const u1 u1s_emfng_ST1RTO = (u1)(((2.)/(8./256.))+0.5);        /* m=emfng,lsb=8/256,ofs=,unit=倍 :ｺｰﾙﾄﾞｽﾀｰﾄ時EM悪化ﾚﾍﾞﾙ失火回数判定値算出係数 */
volatile const u1 u1s_emfng_ST2RTO = (u1)(((2.)/(8./256.))+0.5);        /* m=emfng,lsb=8/256,ofs=,unit=倍 :ﾎｯﾄｽﾀｰﾄ時EM悪化ﾚﾍﾞﾙ失火回数判定値算出係数 */
#endif                                                                  /* JEMFOTLV */

volatile const u1 u1s_emfng_KMFK = (u1)(((0.7)/(2./256.))+0.5);         /* m=emfng,lsb=2/256,ofs=,unit=倍 :DTC出力気筒判定係数 */
volatile const u1 u1s_emfng_KCDTMFWA = (u1)(((1.)/(8./256.))+0.5);      /* m=emfng,lsb=8/256,ofs=,unit=倍 :EM悪化判定用悪路ｶｳﾝﾀ減算係数 */
volatile const u1 u1s_emfng_KCDTMFAOTB = (u1)(((1.)/(8./256.))+0.5);    /* m=emfng,lsb=8/256,ofs=,unit=倍 :触媒OT判定用悪路ｶｳﾝﾀ減算係数(ﾊﾞﾝｸ別) */
volatile const u2 u2s_emfng_KLEVEL3 = (u2)(((1.)/(4./256.))+0.5);       /* m=emfng,lsb=4/256,ofs=,unit=倍 :LEVEL3要求時の失火回数反映係数 */

#if (JEEFI == u1g_EJCC_DUAL) && (JEHIPFI_E == u1g_EJCC_USE)             /* 【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】 */
volatile const u1 u1s_emfng_MFDDI = (u1)(((85.)/(1.))+0.5);             /* m=emfng,lsb=1,ofs=,unit=回 :DI異常検出失火回数 */
volatile const u1 u1s_emfng_MFDPFI = (u1)(((85.)/(1.))+0.5);            /* m=emfng,lsb=1,ofs=,unit=回 :PFI異常検出失火回数 */
#endif                                                                  /* JEEFI JEHIPFI_E */

#if JEMFOTLV == u1g_EJCC_NOT_USE                                        /* 【触媒OTﾚﾍﾞﾙ限定無】 */
  #if JEMFHOUKI == u1g_EJCC_USAMF                                       /* 【触媒OTﾚﾍﾞﾙ限定無】AND【対米法規】 */
volatile const s2 s2s_emfng_THW = s2g_ELSB_THW(95.);                    /* m=emfng,lsb=160/256,ofs=,unit=℃ :ﾎｯﾄｽﾀｰﾄ判定水温 */
volatile const s2 s2s_emfng_THA = s2g_ELSB_THW(75.);                    /* m=emfng,lsb=160/256,ofs=,unit=℃ :ﾎｯﾄｽﾀｰﾄ判定吸気温 */

  #else                                                                 /* 【触媒OTﾚﾍﾞﾙ限定無】AND【対米法規以外】 */
volatile const s2 s2s_emfng_THW = s2g_ELSB_THW(90.);                    /* m=emfng,lsb=160/256,ofs=,unit=℃ :ﾎｯﾄｽﾀｰﾄ判定水温 */
volatile const s2 s2s_emfng_THA = s2g_ELSB_THW(50.);                    /* m=emfng,lsb=160/256,ofs=,unit=℃ :ﾎｯﾄｽﾀｰﾄ判定吸気温 */
  #endif                                                                /* JEMFHOUKI */
#else                                                                   /* 【触媒OTﾚﾍﾞﾙ限定有】 */
volatile const s2 s2s_emfng_THW = s2g_S2MIN;                            /* m=emfng,lsb=160/256,ofs=,unit=℃ :ﾎｯﾄｽﾀｰﾄ判定水温 */
volatile const s2 s2s_emfng_THA = s2g_S2MIN;                            /* m=emfng,lsb=160/256,ofs=,unit=℃ :ﾎｯﾄｽﾀｰﾄ判定吸気温 */
#endif                                                                  /* JEMFOTLV */

/* emfng_THW、emfng_THA は法規要件のため、変更する時はOBDグループまで連絡すること。 */
/* CARB認可の値は emfng_THW≧95℃、emfng_THA≧65℃ */
/* 【燃料OBD有】の場合はemfng_THW、emfng_THAをefkgd_THW、efkgd_THAとそれぞれ同じ値とすること。 */

#if JEMFHOUKI == u1g_EJCC_USAMF                                         /* 【対米法規】 */
volatile const s2 s2s_emfng_THWCSTL = s2g_ELSB_THW(-40.);               /* m=emfng,lsb=160/256,ofs=,unit=℃ :ｺｰﾙﾄﾞｽﾀｰﾄ判定水温 */
volatile const s2 s2s_emfng_THWCSTH = s2g_ELSB_THW(-40.);               /* m=emfng,lsb=160/256,ofs=,unit=℃ :ｺｰﾙﾄﾞｽﾀｰﾄ判定水温 */
volatile const s2 s2s_emfng_THACST = s2g_ELSB_THW(-40.);                /* m=emfng,lsb=160/256,ofs=,unit=℃ :ｺｰﾙﾄﾞｽﾀｰﾄ判定吸気温 */
volatile const u1 u1s_emfng_CJEMREV = (u1)(((1.)/(1.))+0.5);            /* m=emfng,lsb=1,ofs=,unit=回 :EM悪化ﾚﾍﾞﾙ失火回数嵩上げ1000rev回数 */

#else                                                                   /* 【対米法規以外】 */
volatile const s2 s2s_emfng_THWCSTL = s2g_ELSB_THW(10.);                /* m=emfng,lsb=160/256,ofs=,unit=℃ :ｺｰﾙﾄﾞｽﾀｰﾄ判定水温 */
volatile const s2 s2s_emfng_THWCSTH = s2g_ELSB_THW(60.);                /* m=emfng,lsb=160/256,ofs=,unit=℃ :ｺｰﾙﾄﾞｽﾀｰﾄ判定水温 */
volatile const s2 s2s_emfng_THACST = s2g_ELSB_THW(10.);                 /* m=emfng,lsb=160/256,ofs=,unit=℃ :ｺｰﾙﾄﾞｽﾀｰﾄ判定吸気温 */
volatile const u1 u1s_emfng_CJEMREV = (u1)(((2.)/(1.))+0.5);            /* m=emfng,lsb=1,ofs=,unit=回 :EM悪化ﾚﾍﾞﾙ失火回数嵩上げ1000rev回数 */
#endif                                                                  /* JEMFHOUKI */

volatile const s2 s2s_emfng_KCDMFWL = s2g_glround((0.5)/(2./64./256.)); /* m=emfng,lsb=2/64/256,ofs=,unit= :ﾃｽﾄ結果出力用 対向気筒失火ｶｳﾝﾀ加算判定値 */
volatile const u2 u2g_emfng_ECDMFAEMTV_MI = u2g_U2MIN;                  /* m=emfng,lsb=1,ofs=,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(全気筒)初期値 */
volatile const u2 u2g_emfng_ECDMFAEMMXTL_MI = u2g_U2MIN;                /* m=emfng,lsb=1,ofs=,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ上限値(全気筒)初期値 */
volatile const u2 u2g_emfng_ECDMFAEMMNTL_MI = u2g_U2MIN;                /* m=emfng,lsb=1,ofs=,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ下限値(全気筒)初期値 */
volatile const u2 u2g_emfng_ECDMFEMTV_MI = u2g_U2MIN;                   /* m=emfng,lsb=1,ofs=,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(気筒別)初期値 */
volatile const u2 u2g_emfng_ECDMFEMMXTL_MI = u2g_U2MIN;                 /* m=emfng,lsb=1,ofs=,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ上限値(気筒別)初期値 */
volatile const u2 u2g_emfng_ECDMFEMMNTL_MI = u2g_U2MIN;                 /* m=emfng,lsb=1,ofs=,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ下限値(気筒別)初期値 */
volatile const u2 u2g_emfng_ECDMFAAVTV_MI = u2g_U2MIN;                  /* m=emfng,lsb=1,ofs=,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ(全気筒)初期値 */
volatile const u2 u2g_emfng_ECDMFAAVMXTL_MI = u2g_U2MIN;                /* m=emfng,lsb=1,ofs=,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ上限値(全気筒)初期値 */
volatile const u2 u2g_emfng_ECDMFAAVMNTL_MI = u2g_U2MIN;                /* m=emfng,lsb=1,ofs=,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ下限値(全気筒)初期値 */
volatile const u2 u2g_emfng_ECDMFAVTV_MI = u2g_U2MIN;                   /* m=emfng,lsb=1,ofs=,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ(気筒別)初期値 */
volatile const u2 u2g_emfng_ECDMFAVMXTL_MI = u2g_U2MIN;                 /* m=emfng,lsb=1,ofs=,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ上限値(気筒別)初期値 */
volatile const u2 u2g_emfng_ECDMFAVMNTL_MI = u2g_U2MIN;                 /* m=emfng,lsb=1,ofs=,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ下限値(気筒別)初期値 */
volatile const u1 u1g_emfng_EXRCDMFI_MI = (u1)(OFF);                    /* m=emfng,lsb=,ofs=,unit= :EWMA失火ｶｳﾝﾀ計算履歴フラグ初期値 */

#if JEOOBD == u1g_EJCC_USE                                              /* 【市場調査有】 */
volatile const u1 u1s_emfng_MFLOW = (u1)(((0.15)/(2./256.))+0.5);       /* m=emfng,lsb=2/256,ofs=,unit= :失火回数小判定係数 */
volatile const u1 u1s_emfng_MFMID = (u1)(((0.3)/(2./256.))+0.5);        /* m=emfng,lsb=2/256,ofs=,unit= :失火回数中判定係数 */
volatile const u1 u1s_emfng_MFHI = (u1)(((0.5)/(2./256.))+0.5);         /* m=emfng,lsb=2/256,ofs=,unit= :失火回数大判定係数 */
volatile const u2 u2g_emfng_EOCMFNL_MI = u2g_U2MIN;                     /* m=emfng,lsb=1,ofs=,unit=回 :失火回数(小)の頻度 初期値 */
volatile const u2 u2g_emfng_EOCMFNM_MI = u2g_U2MIN;                     /* m=emfng,lsb=1,ofs=,unit=回 :失火回数(中)の頻度 初期値 */
volatile const u2 u2g_emfng_EOCMFNH_MI = u2g_U2MIN;                     /* m=emfng,lsb=1,ofs=,unit=回 :失火回数(大)の頻度 初期値 */
volatile const u2 u2g_emfng_EOCMFMLTL_MI = u2g_U2MIN;                   /* m=emfng,lsb=1,ofs=,unit=回 :対向気筒失火回数(小)の頻度 初期値 */
volatile const u2 u2g_emfng_EOCMFMLTH_MI = u2g_U2MIN;                   /* m=emfng,lsb=1,ofs=,unit=回 :対向気筒失火回数(大)の頻度 初期値 */
volatile const u1 u1g_emfng_EONEMFMLT_MI = u1g_U1MIN;                   /* m=emfng,lsb=200/8,ofs=,unit=rpm :対向気筒失火時のｴﾝｼﾞﾝ回転なまし値 初期値 */
volatile const u1 u1g_emfng_EOCDMFAE2MX_MI = u1g_U1MIN;                 /* m=emfng,lsb=1,ofs=,unit=回 :1000rev中失火ｶｳﾝﾀの最大値 初期値 */
volatile const u1 u1g_emfng_EOCJREVEM_MI = u1g_U1MIN;                   /* m=emfng,lsb=1,ofs=,unit=回 :最大失火ｶｳﾝﾀ更新時の1000rev回数 初期値 */
volatile const s1 s1g_emfng_EOTHWEM_MI = s1g_glround((0.)/(160./128.)); /* m=emfng,lsb=160/128,ofs=,unit=℃ :最大失火ｶｳﾝﾀ更新時の水温 初期値 */
volatile const s1 s1g_emfng_EOTHAEM_MI = s1g_glround((0.)/(160./128.)); /* m=emfng,lsb=160/128,ofs=,unit=℃ :最大失火ｶｳﾝﾀ更新時の吸気温 初期値 */
volatile const u1 u1g_emfng_EOCDMFEM2MX_MI = u1g_U1MIN;                 /* m=emfng,lsb=1,ofs=,unit=回 :最大失火ｶｳﾝﾀ更新時の気筒別失火ｶｳﾝﾀ 初期値 */
volatile const u1 u1g_emfng_EOSPDEMAV_MI = u1g_U1MIN;                   /* m=emfng,lsb=256/256,ofs=,unit=km/h :失火ｶｳﾝﾄ時の平均車速 初期値 */
volatile const u1 u1g_emfng_EONEEMAV_MI = u1g_U1MIN;                    /* m=emfng,lsb=12800/256,ofs=,unit=rpm :失火ｶｳﾝﾄ時の平均ｴﾝｼﾞﾝ回転数 初期値 */
volatile const u1 u1g_emfng_EOKLSMEMAV_MI = u1g_U1MIN;                  /* m=emfng,lsb=320/256,ofs=,unit=% :失火ｶｳﾝﾄ時の平均負荷 初期値 */
volatile const u1 u1g_emfng_EOCJMFNEEMI_MI = u1g_U1MIN;                 /* m=emfng,lsb=1,ofs=,unit=回 :失火ｶｳﾝﾄ時のｱｲﾄﾞﾙﾓｰﾄﾞ回数 初期値 */
volatile const u1 u1g_emfng_EOCJMFNEEML_MI = u1g_U1MIN;                 /* m=emfng,lsb=1,ofs=,unit=回 :失火ｶｳﾝﾄ時のﾛｰﾓｰﾄﾞ回数 初期値 */
volatile const u1 u1g_emfng_EOCJMFNEEMH_MI = u1g_U1MIN;                 /* m=emfng,lsb=1,ofs=,unit=回 :失火ｶｳﾝﾄ時のﾊｲﾓｰﾄﾞ回数 初期値 */
volatile const u1 u1g_emfng_EOCDMFAOTMX_MI = u1g_U1MIN;                 /* m=emfng,lsb=1,ofs=,unit=回 :200rev中最大ｶｳﾝﾀ最大値  初期値 */
volatile const u1 u1g_emfng_EOCJREVOT_MI = u1g_U1MIN;                   /* m=emfng,lsb=1,ofs=,unit=回 :最大失火ｶｳﾝﾀ更新時の1000rev回数 初期値 */
volatile const s1 s1g_emfng_EOTHWOT_MI = s1g_glround((0.)/(160./128.)); /* m=emfng,lsb=160/128,ofs=,unit=℃ :最大失火ｶｳﾝﾀ更新時の水温 初期値 */
volatile const s1 s1g_emfng_EOTHAOT_MI = s1g_glround((0.)/(160./128.)); /* m=emfng,lsb=160/128,ofs=,unit=℃ :最大失火ｶｳﾝﾀ更新時の吸気温 初期値 */
#endif                                                                  /* JEOOBD */

/* $$$適合定数_標準_v4.11 */

/*-------------------------------------------------------------------*/
/* 適合マップ                                                        */
/*-------------------------------------------------------------------*/
/* $$$適合マップ_標準_v4.11 */
/*---- emfng_emfrtot_map -------------------------------------------*/
#define X(x) (s2g_ELSB_KL(x))                                           /* m=emfng,lsb=320/64/256,ofs=,unit=% :eklset_eklsm */
#define Y(y) (s2g_ELSB_NE(y))                                           /* m=emfng,lsb=12800/64/256,ofs=,unit=rpm :ene_ene */
#define Z(z) (s2g_glround((z)/(256./128./256.)))                        /* m=emfng,lsb=256/128/256,ofs=,unit=% : */
volatile const s2 s2g_emfng_emfrtot_map[] =
{
    6,7,0,
              X(15.),   X(30.),   X(40.),   X(55.),   X(70.),   X(85.),
    Y(800.),  Z(50.),   Z(50.),   Z(47.06), Z(44.44), Z(32.),   Z(25.81),
    Y(1600.), Z(40.),   Z(32.),   Z(29.63), Z(28.57), Z(25.),   Z(24.24),
    Y(2400.), Z(36.36), Z(30.77), Z(26.67), Z(22.86), Z(18.18), Z(17.02),
    Y(3200.), Z(32.),   Z(21.05), Z(18.18), Z(14.04), Z(10.),   Z(11.94),
    Y(4000.), Z(24.24), Z(17.02), Z(14.04), Z(14.04), Z(11.94), Z(11.94),
    Y(4800.), Z(21.05), Z(15.09), Z(12.9),  Z(12.9),  Z(14.04), Z(16.),
    Y(5600.), Z(18.18), Z(11.94), Z(11.94), Z(14.04), Z(18.18), Z(18.18)
};
#undef X
#undef Y
#undef Z

#if JENGPF_E != u1g_EJCC_NOT_USE                                        /* 【GPF有】 */
/*---- emfng_emfrtotgpf_map ----------------------------------------*/
#define X(x) (s2g_ELSB_KL(x))                                           /* m=emfng,lsb=320/64/256,ofs=,unit=% :eklset_eklsm */
#define Y(y) (s2g_ELSB_NE(y))                                           /* m=emfng,lsb=12800/64/256,ofs=,unit=rpm :ene_ene */
#define Z(z) (s2g_glround((z)/(256./128./256.)))                        /* m=emfng,lsb=256/128/256,ofs=,unit=% : */
volatile const s2 s2g_emfng_emfrtotgpf_map[] =                          /* 【GPF有】 */
{
    6,7,0,
              X(15.),   X(30.),   X(40.),   X(55.),   X(70.),   X(85.),
    Y(800.),  Z(50.),   Z(50.),   Z(47.06), Z(44.44), Z(32.),   Z(25.81),
    Y(1600.), Z(40.),   Z(32.),   Z(29.63), Z(28.57), Z(25.),   Z(24.24),
    Y(2400.), Z(36.36), Z(30.77), Z(26.67), Z(22.86), Z(18.18), Z(17.02),
    Y(3200.), Z(32.),   Z(21.05), Z(18.18), Z(14.04), Z(10.),   Z(11.94),
    Y(4000.), Z(24.24), Z(17.02), Z(14.04), Z(14.04), Z(11.94), Z(11.94),
    Y(4800.), Z(21.05), Z(15.09), Z(12.9),  Z(12.9),  Z(14.04), Z(16.),
    Y(5600.), Z(18.18), Z(11.94), Z(11.94), Z(14.04), Z(18.18), Z(18.18)
};
#undef X
#undef Y
#undef Z

#endif                                                                  /* JENGPF_E */

#if JEEGTYPE == u1g_EJCC_V6CYL                                          /* 【V6】 */
/*---- emfng_mfcylbnk_tbl ------------------------------------------*/
#define X(x) ((u1)(((x)/(1.))+0.5))                                     /* m=emfng,lsb=1,ofs=,unit= : */
volatile const u1 u1s_emfng_mfcylbnk_tbl[] =                            /* 【V6】 */
{
    /* t_mfcyl    0      1      2      3      4      5     */
    /* t_bnk */   X(0.), X(1.), X(0.), X(1.), X(0.), X(1.)
};
#undef X

/*---- emfng_mfwcyl_tbl --------------------------------------------*/
#define X(x) ((u1)(x))                                                  /* m=emfng,lsb=,ofs=,unit= : */
volatile const u1 u1g_emfng_mfwcyl_tbl[] =                              /* 【V6】 */
{
    /* t_cylw     0            1            2           */
    /* t_bnk */   (u1)(0x24U), (u1)(0x09U), (u1)(0x12U)
};
#undef X

#endif                                                                  /* JEEGTYPE */

/* $$$適合マップ_標準_v4.11 */

/**** End of File ****************************************************/
