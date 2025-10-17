/* emfng-pa000-5000-a-M4 */
/*********************************************************************/
/*  Copyright Toyota Motor Corporation                               */
/*********************************************************************/

/*********************************************************************/
/* オブジェクト名 |  失火検出異常判定部                              */
/* 注意事項       | 【失火OBD有】AND(【4気筒】OR【6気筒】)           */
/*-------------------------------------------------------------------*/
/*     個別変更履歴 （個別変更時は以下に変更日、内容を記入すること） */
/*********************************************************************/

/*-------------------------------------------------------------------*/
/* ヘッダファイルのインクルード                                      */
/*-------------------------------------------------------------------*/
#include <../inc/common.h>
#include <../inc/gllib.h>
#include <engsrc/espc/ejcc.h>           /* JEEFI,JEEGTYPE,JEHIPFI_E,JEMAT_OBD2,JEMFFC */
                                        /* JEMFHOUKI,JEMFMETHOD_D,u1g_EJCC_TIME_D,u1g_EJCC_OMEGA_D */
                                        /* JENCYL,JEOOBD,u1g_EJCC_NCYL,JEMFDTC_D */
                                        /* JETCORE_D,JEOBDUDS_D,JERLOK,JENGPF_E */
                                        /* u1g_ejcc_NCYL,u1g_EJCC_DUAL,u1g_EJCC_6CYL,u1g_EJCC_4CYL,u1g_EJCC_USAMF,u1g_EJCC_NOT_USAMF,u1g_EJCC_V6CYL,u1g_EJCC_L6CYL */
#include <engsrc/inc/elsb.h>
#include <engsrc/inc/elib.h>

#include <obd/wfh/wsentivchg.h>         /* u1g_wsentivchg_state,u1g_WSENTIVCHG_IUE,u1g_WSENTIVCHG_LVL2 */
#include <obd/wfh/wdiaginfclr.h>        /* big_wdiaginfclr_xreqclr */
#if JEOBDUDS_D == u1g_EJCC_USE          /*【OBDonUDS対応有】*/
#include <obd/wfh/wpsajdg.h>            /* big_wpsajdg_xpsa */
#endif
#if JEMAT_OBD2 == u1g_EJCC_NOT_USE      /*【OBD2適合無】*/
#include <obd/wbp/wobd2msk.h>           /* big_wobd2msk_xmfKwp */
#endif
#if JEMFDTC_D == u1g_EJCC_USE           /*【失火OBD DTC有】*/
#include <obd/wfd/wmf_fdi.h>            /* big_wmf_fdi_xem_lstpnd,big_wmf_fdi_xot_lstpnd,big_wmf_fdi_xot_warning */
                                        /* big_wmf_fdi_xem_warning,vdg_wmf_fdi_ot_fald_u1(),vdg_wmf_fdi_em_fald_u1(),vdg_wmf_fdi_ot_excmf() */
                                        /* vdg_wmf_fdi_em_excmf(),vdg_wmf_fdi_mil_blink_u1(),vdg_wmf_fdi_ot_imdf(),vdg_wmf_fdi_ot_imdnf() */
                                        /* vdg_wmf_fdi_pas_u2u1(),u1g_WMF_FDI_OUTOF_FTPCYCLE,u1g_WMF_FDI_DURING_FTPCYCLE */
                                        /* u1g_WMF_FDI_WITHIN_FIRST1000RPM,u1g_WMF_FDI_AFTER_FIRST1000RPM,u2g_WMF_FDI_ID_OT */
                                        /* big_wmf_fdi_xot_pnd,big_wmf_fdi_xem_pnd,vdg_wmf_fdi_ot_excmf_u2u1(),vdg_wmf_fdi_em_excmf_u2u1(),u2g_WMF_FDI_ID_EM */
#endif
#if ( JETCORE_D == u1g_EJCC_USE ) && ( JEOBDUDS_D == u1g_EJCC_NOT_USE )  /*【t-CORE対応有】AND【OBDonUDS対応無】*/
#include <obd/wspc/wtcif.h>             /* u1g_wtcif_dem_setdtr_u2s4s4s4u1(),u2g_WTCIF_DEM_DTR_ECDMFAEM,u1g_WTCIF_DEM_DTR_CTL_NORMAL */
                                        /* u2g_WTCIF_DEM_DTR_ECDMFEM1,u2g_WTCIF_DEM_DTR_ECDMFEM2,u2g_WTCIF_DEM_DTR_ECDMFEM3 */
                                        /* u2g_WTCIF_DEM_DTR_ECDMFEM4,u2g_WTCIF_DEM_DTR_ECDMFEM5,u2g_WTCIF_DEM_DTR_ECDMFEM6 */
                                        /* u2g_WTCIF_DEM_DTR_ECDMFAV1,u2g_WTCIF_DEM_DTR_ECDMFAV2,u2g_WTCIF_DEM_DTR_ECDMFAV3 */
                                        /* u2g_WTCIF_DEM_DTR_ECDMFAV4,u2g_WTCIF_DEM_DTR_ECDMFAV5,u2g_WTCIF_DEM_DTR_ECDMFAV6,u2g_WTCIF_DEM_DTR_ECDMFAAV */
#endif
#if JENGPF_E != u1g_EJCC_NOT_USE        /*【GPF有】*/
#include <engsrc/efunc/efuncin/egpfrgcnd/egpfrgcnd.h>              /* u1g_egpfrgcnd_exgpfrgsrv */
#include <engsrc/efunc/eemi/egpf/egpfhtum/egpfhtum.h>              /* u1g_egpfhtum_exdth,u1g_egpfhtum_exeff */
 #if JERLOK == u1g_EJCC_USE             /*【ﾘｯﾁﾘｰﾝ運転可】*/
#include <engsrc/efunc/eemi/egpf/egpfrgum.h>                       /* u1g_egpfrgum_exrgact */
 #endif
#endif
#include <engsrc/eserv/emem/ememctr.h>              /* s4g_ememctr_write(),s4g_ememctr_read() */
                                                    /* u2g_EMFNG_EONEMFMLT_U1_ID,u2g_EMFNG_EXRCDMF_U1_ID */
                                                    /* u2g_EMFNG_EOCDMFAE2MX_U1_ID,u2g_EMFNG_EOCJREVEM_U1_ID */
                                                    /* u2g_EMFNG_EOTHWEM_S1_ID,u2g_EMFNG_EOTHAEM_S1_ID */
                                                    /* u2g_EMFNG_EOCDMFEM2MX1_U1_ID,u2g_EMFNG_EOCDMFEM2MX2_U1_ID */
                                                    /* u2g_EMFNG_EOCDMFEM2MX3_U1_ID,u2g_EMFNG_EOCDMFEM2MX4_U1_ID */
                                                    /* u2g_EMFNG_EOCDMFEM2MX5_U1_ID,u2g_EMFNG_EOCDMFEM2MX6_U1_ID */
                                                    /* u2g_EMFNG_EOSPDEMAV_U1_ID,u2g_EMFNG_EONEEMAV_U1_ID */
                                                    /* u2g_EMFNG_EOKLSMEMAV_U1_ID,u2g_EMFNG_EOCJMFNEEMI_U1_ID */
                                                    /* u2g_EMFNG_EOCJMFNEEML_U1_ID,u2g_EMFNG_EOCJMFNEEMH_U1_ID */
                                                    /* u2g_EMFNG_EOCDMFAOTMX_U1_ID,u2g_EMFNG_EOCJREVOT_U1_ID */
                                                    /* u2g_EMFNG_EOTHWOT_S1_ID,u2g_EMFNG_EOTHAOT_S1_ID */
                                                    /* u2g_EMFNG_EOCMFNL_U2_ID,u2g_EMFNG_EOCMFNM_U2_ID */
                                                    /* u2g_EMFNG_EOCMFNH_U2_ID,u2g_EMFNG_EOCMFMLTL_U2_ID */
                                                    /* u2g_EMFNG_EOCMFMLTH_U2_ID,u2g_EMFNG_ECDMFAEMMXTL_U2_ID */
                                                    /* u2g_EMFNG_ECDMFEMMXTL1_U2_ID,u2g_EMFNG_ECDMFEMMXTL2_U2_ID */
                                                    /* u2g_EMFNG_ECDMFEMMXTL3_U2_ID,u2g_EMFNG_ECDMFEMMXTL4_U2_ID */
                                                    /* u2g_EMFNG_ECDMFEMMXTL5_U2_ID,u2g_EMFNG_ECDMFEMMXTL6_U2_ID */
                                                    /* u2g_EMFNG_ECDMFAAVMXTL_U2_ID,u2g_EMFNG_ECDMFAVMXTL1_U2_ID */
                                                    /* u2g_EMFNG_ECDMFAVMXTL2_U2_ID,u2g_EMFNG_ECDMFAVMXTL3_U2_ID */
                                                    /* u2g_EMFNG_ECDMFAVMXTL4_U2_ID,u2g_EMFNG_ECDMFAVMXTL5_U2_ID */
                                                    /* u2g_EMFNG_ECDMFAVMXTL6_U2_ID,u2g_EMFNG_ECDMFAEMTV_U2_ID */
                                                    /* u2g_EMFNG_ECDMFAEMMNTL_U2_ID,u2g_EMFNG_ECDMFAAVTV_U2_ID */
                                                    /* u2g_EMFNG_ECDMFAAVMNTL_U2_ID,u2g_EMFNG_ECDMFEMTV1_U2_ID */
                                                    /* u2g_EMFNG_ECDMFEMTV2_U2_ID,u2g_EMFNG_ECDMFEMTV3_U2_ID */
                                                    /* u2g_EMFNG_ECDMFEMTV4_U2_ID,u2g_EMFNG_ECDMFEMTV5_U2_ID */
                                                    /* u2g_EMFNG_ECDMFEMTV6_U2_ID,u2g_EMFNG_ECDMFEMMNTL1_U2_ID */
                                                    /* u2g_EMFNG_ECDMFEMMNTL2_U2_ID,u2g_EMFNG_ECDMFEMMNTL3_U2_ID */
                                                    /* u2g_EMFNG_ECDMFEMMNTL4_U2_ID,u2g_EMFNG_ECDMFEMMNTL5_U2_ID */
                                                    /* u2g_EMFNG_ECDMFEMMNTL6_U2_ID,u2g_EMFNG_ECDMFAVTV1_U2_ID */
                                                    /* u2g_EMFNG_ECDMFAVTV2_U2_ID,u2g_EMFNG_ECDMFAVTV3_U2_ID */
                                                    /* u2g_EMFNG_ECDMFAVTV4_U2_ID,u2g_EMFNG_ECDMFAVTV5_U2_ID */
                                                    /* u2g_EMFNG_ECDMFAVTV6_U2_ID,u2g_EMFNG_ECDMFAVMNTL1_U2_ID */
                                                    /* u2g_EMFNG_ECDMFAVMNTL2_U2_ID,u2g_EMFNG_ECDMFAVMNTL3_U2_ID */
                                                    /* u2g_EMFNG_ECDMFAVMNTL4_U2_ID,u2g_EMFNG_ECDMFAVMNTL5_U2_ID */
                                                    /* u2g_EMFNG_ECDMFAVMNTL6_U2_ID */
#if ( JEEFI == u1g_EJCC_DUAL ) && ( JEHIPFI_E == u1g_EJCC_USE ) /*【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/
#include <engsrc/eactmedi/einj/eminj.h>             /* s2g_eminj_ekpfi */
#endif
#include <engsrc/estate/esin/ene.h>                 /* s2g_ene_ene */
#include <engsrc/estate/esin/espd.h>                /* s2g_espd_espd */
#include <engsrc/estate/esin/etha.h>                /* s2g_etha_etha */
#include <engsrc/estate/estepin/exst.h>             /* u1g_exst_exastnrm */
#include <engsrc/etranfwd/etemp/ethw.h>             /* s2g_ethw_ethw */
#include <engsrc/espc/eengspc.h>                    /* u1g_eengspc_ewcnvt_tbl[] */
#include <eng/ekl/eklset.h>                         /* s2g_eklset_eklsm */

#if ( JEOOBD == u1g_EJCC_USE ) && ( JENCYL == u1g_EJCC_6CYL )   /*【市場調査有】AND【6気筒】*/
#include <eng/eobd/eobdsm.h>            /* s2g_eobdsm_enejsm */
#endif

#include <eng/emf/emfcc.h>              /* s2g_emfcc_edltmaxlh,s2g_emfcc_edltmaxhh */
                                        /* s2g_emfcc_ewmaxl,s2g_emfcc_ewmaxh */
#include <eng/emf/emfccrnk.h>           /* u1g_EMFCCRNK_DLTMFL_TIMMING,s2g_emfccrnk_edltmflxh[] */
                                        /* u1g_EMFCCRNK_DDTCRX_TIMMING,s2g_emfccrnk_eddtcrxh[] */
                                        /* s2g_emfccrnk_ewmfl[],s2g_emfccrnk_ewmfh[],s2g_emfccrnk_ewmfhp[] */
#include <eng/emf/emfcnt.h>             /* u1g_EMFCNT_NOT,big_emfcnt_exptnup,s2g_emfcnt_MFSPD */
                                        /* u1g_EMFCNT_LOW,u1g_EMFCNT_HIGH,u1g_EMFCNT_OPPS */
                                        /* u1g_EMFCNT_MLT,u1g_EMFCNT_CYL() */
#include <eng/emf/emfinftp.h>           /* big_emfinftp_exinftp */
#include <eng/emf/emfne.h>              /* u1g_emfne_ejmfne,u1g_EMFNE_REV_IDLE1,u1g_EMFNE_REV_IDLE2 */
                                        /* u1g_EMFNE_REV_LOW,u1g_EMFNE_REV_HIGH */
#include <eng/emf/emfok.h>              /* s2g_emfok_NGDI,s2g_emfok_NGPFI,emfok_roughcnt() */
                                        /* emfok_mfneld_set(),emfok_mfareacnt_ann_u1u1() */
                                        /* emfok_mfneld_ann(),emfok_mfcmodclr() */

#if JEMFFC == u1g_EJCC_USE              /*【触媒OT失火異常時FC有】*/
#include <eng/emf/emffc.h>              /* u1g_emffc_exfcbi */
#endif

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

#if !defined JEMAT_OBD2
#error "JEMAT_OBD2 is undeclared"
#endif

#if !defined JEMFFC
#error "JEMFFC is undeclared"
#endif

#if !defined JEMFHOUKI
#error "JEMFHOUKI is undeclared"
#endif

#if !defined JEMFMETHOD_D
#error "JEMFMETHOD_D is undeclared"
#endif

#if !defined u1g_EJCC_TIME_D
#error "u1g_EJCC_TIME_D is undeclared"
#endif

#if !defined u1g_EJCC_OMEGA_D
#error "u1g_EJCC_OMEGA_D is undeclared"
#endif

#if !defined JENCYL
#error "JENCYL is undeclared"
#endif

#if !defined JEOOBD
#error "JEOOBD is undeclared"
#endif

#if !defined u1g_EJCC_NCYL
#error "u1g_EJCC_NCYL is undeclared"
#endif

#if !defined JEMFDTC_D
#error "JEMFDTC_D is undeclared"
#endif

#if !defined JETCORE_D
#error "JETCORE_D is undeclared"
#endif

#if !defined JEOBDUDS_D
#error "JEOBDUDS_D is undeclared"
#endif

#if !defined JERLOK
#error "JERLOK is undeclared"
#endif

#if !defined JENGPF_E
#error "JENGPF_E is undeclared"
#endif

#if !defined u1g_EJCC_DUAL
#error "u1g_EJCC_DUAL is undeclared"
#endif

#if !defined u1g_EJCC_6CYL
#error "u1g_EJCC_6CYL is undeclared"
#endif

#if !defined u1g_EJCC_4CYL
#error "u1g_EJCC_4CYL is undeclared"
#endif

#if !defined u1g_EJCC_USAMF
#error "u1g_EJCC_USAMF is undeclared"
#endif

#if !defined u1g_EJCC_NOT_USAMF
#error "u1g_EJCC_NOT_USAMF is undeclared"
#endif

#if !defined u1g_EJCC_V6CYL
#error "u1g_EJCC_V6CYL is undeclared"
#endif

#if !defined u1g_EJCC_L6CYL
#error "u1g_EJCC_L6CYL is undeclared"
#endif

/*-------------------------------------------------------------------*/
/* 適合値                                                            */
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
/* 型定義                                                            */
/*-------------------------------------------------------------------*/

/*---------------------------------------------------------------------*/
/* macro定義                                                           */
/*---------------------------------------------------------------------*/
/* $$$マクロ定数_標準_v4.11 */
#define u1s_EMFNG_MFSTOTI ((u1)(((173.)/(1.))+0.5))                     /* m=emfng,lsb=1,ofs=,unit=回 :ｱｲﾄﾞﾙ中全気筒失火回数判定値上限 */

#if (JETCORE_D == u1g_EJCC_USE) && (JEOBDUDS_D == u1g_EJCC_USE)         /* 【t-CORE対応有】AND【OBDonUDS対応有】 */
#define u1s_EMFNG_CXMF1PAS ((u1)(((0.)/(1.))+0.5))                      /* m=emfng,lsb=1,ofs=,unit=回 :レディネス用 触媒OT失火回数判定値 */
#define u1s_EMFNG_CXMFPAS ((u1)(((0.)/(1.))+0.5))                       /* m=emfng,lsb=1,ofs=,unit=回 :レディネス用 EM悪化失火回数判定値 */
#endif                                                                  /* JETCORE_D JEOBDUDS_D */

/* $$$マクロ定数_標準_v4.11 */

#define u1s_EMFNG_NCYLW      ((u1)(u1g_ejcc_NCYL >> 1))       /* 気筒群数 */

/*-------------------------------------------------------------------*/
/* 変数定義                                                          */
/*-------------------------------------------------------------------*/
/* グローバル変数 */
/* emf内にのみ公開 */
u2 u2g_emfng_ekldmfae;       /* m=emfng,lsb=1,unit=回 :EM悪化ﾚﾍﾞﾙ失火判定値 */
u1 u1g_emfng_ecdmfaotbnk[2]; /* m=emfng,lsb=1,unit=回 :ﾊﾞﾝｸ別触媒OTﾚﾍﾞﾙ失火ｶｳﾝﾀ */
                             /* 1バンクでも2バンク分の領域を確保する */
#if JEMFFC == u1g_EJCC_USE   /*【触媒OT失火異常時FC有】*/
u1 u1g_emfng_exdmffc;        /* m=emfng,lsb=1 :FCｵﾌﾟｼｮﾝ判定用触媒OTﾚﾍﾞﾙ異常通知ﾌﾗｸﾞ */
#endif
u1 u1g_emfng_ecdmfaot;       /* m=emfng,lsb=1,unit=回 :触媒OTﾚﾍﾞﾙ全気筒失火ｶｳﾝﾀ */
u2 u2g_emfng_ecdmfaoti;      /* m=emfng,lsb=1,unit=回 :ｱｲﾄﾞﾙ中全気筒失火ｶｳﾝﾀ */
u2 u2g_emfng_ecdmfae2;       /* m=emfng,lsb=1,unit=回 :EM悪化ﾚﾍﾞﾙ全気筒失火ｶｳﾝﾀ */
u1 u1g_emfng_ecdmfaem;       /* m=emfng,lsb=1,unit=回 :EM悪化ﾚﾍﾞﾙ全気筒失火ｶｳﾝﾀ */
u2 u2g_emfng_ecjmfot;        /* m=emfng,lsb=1/3【6気筒】1/2【4気筒】,unit=rev :触媒OTﾚﾍﾞﾙ失火検出許可ｶｳﾝﾀ */
u2 u2g_emfng_ecjmfoti;       /* m=emfng,lsb=1/3【6気筒】1/2【4気筒】,unit=rev :触媒OTﾚﾍﾞﾙｱｲﾄﾞﾙ時失火検出許可ｶｳﾝﾀ */
u2 u2g_emfng_ecjmfem;        /* m=emfng,lsb=1/3【6気筒】1/2【4気筒】,unit=rev :EM悪化ﾚﾍﾞﾙ失火検出許可ｶｳﾝﾀ */
u2 u2g_emfng_ekldmfot;       /* m=emfng,lsb=1,unit=回 :触媒OTﾚﾍﾞﾙ失火回数判定値 */
u2 u2g_emfng_ekldmfoth_mcr;  /* m=emfng,lsb=1,unit=回 :触媒OTﾚﾍﾞﾙ失火回数判定値(計算値) */
u2 u2g_emfng_eldmfaem_mcr;   /* m=emfng,lsb=1,unit=回 :EM悪化ﾚﾍﾞﾙ失火回数判定値(計算値) */

stflag8 stg_emfng_flag1;     /* m=emfng :グローバルフラグ用構造体(16msl,65msl,drvclchgタイミング) */
#if ( JEEFI == u1g_EJCC_DUAL ) && ( JEHIPFI_E == u1g_EJCC_USE )   /*【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/
stflag8 stg_emfng_flag2;     /* m=emfng :グローバルフラグ用構造体(65msl,drvclchgタイミング) */
#endif
#if JETCORE_D == u1g_EJCC_USE                           /*【t-CORE対応有】*/
stflag8 stg_emfng_flag3;     /* m=emfng :グローバルフラグ用構造体(65msl,drvclchgタイミング) */
#endif
u1 u1g_emfng_ecdmfem[u1g_EJCC_NCYL];    /* m=emfng,lsb=1,unit=回 :EM悪化ﾚﾍﾞﾙ気筒別失火ｶｳﾝﾀ */
u2 u2g_emfng_ecdmfem2[u1g_EJCC_NCYL];   /* m=emfng,lsb=1,unit=回 :EM悪化ﾚﾍﾞﾙ気筒別失火ｶｳﾝﾀ */
u1 u1g_emfng_ecdmfemi[u1g_EJCC_NCYL];   /* m=emfng,lsb=1,unit=回 :ｱｲﾄﾞﾙ中気筒別失火ｶｳﾝﾀ */
u1 u1g_emfng_ecdtmf2;                   /* m=emfng,lsb=1,unit=回 :触媒OTﾚﾍﾞﾙ判定用悪路ｶｳﾝﾀ */
u2 u2g_emfng_ecmfot;                    /* m=emfng,lsb=1,unit=回 :複数気筒失火ｶｳﾝﾀ */
u1 u1g_emfng_ecdmfw[u1g_EJCC_NCYL/2];   /* m=emfng,lsb=1,unit=回 :対向気筒失火ｶｳﾝﾀ */

/* スタティック変数 */
static u1 u1s_emfng_ecjpmf;    /* lsb=1,unit=回 :ｾﾞﾛｸﾛｽｶｳﾝﾀ */
static u2 u2s_emfng_ecdtmfwa;  /* lsb=1,unit=回 :EM悪化ﾚﾍﾞﾙ判定用悪路ｶｳﾝﾀ */
static u1 u1s_emfng_ecxmf1;    /* lsb=1,unit=回 :触媒OTﾚﾍﾞﾙ異常検出回数 */
static u1 u1s_emfng_ecxmf;     /* lsb=1,unit=回 :EM悪化ﾚﾍﾞﾙ異常検出回数 */
static u1 u1s_emfng_ecjemrev;  /* lsb=1,unit=回 :始動後EM悪化ﾚﾍﾞﾙ判定回数 */
#if JEOOBD == u1g_EJCC_USE     /*【市場調査有】*/
static s4 s4s_emfng_espdaem;   /* lsb=256/256,unit=km/h :1000rev間の車速累積値 */
static s4 s4s_emfng_eneaem;    /* lsb=12800/64/256,unit=rpm :1000rev間のｴﾝｼﾞﾝ回転数累積値 */
static s4 s4s_emfng_eklsmaem;  /* lsb=320/64/256,unit=% :1000rev間のｴﾝｼﾞﾝ負荷累積値 */
static u1 u1s_emfng_ecjmfneemi; /* lsb=1,unit=回 :1000rev間のｱｲﾄﾞﾙﾓｰﾄﾞ回数 */
static u1 u1s_emfng_ecjmfneeml; /* lsb=1,unit=回 :1000rev間のﾛｰﾓｰﾄﾞ回数 */
static u1 u1s_emfng_ecjmfneemh; /* lsb=1,unit=回 :1000rev間のﾊｲﾓｰﾄﾞ回数 */
#endif
#if ( JEEFI == u1g_EJCC_DUAL ) && ( JEHIPFI_E == u1g_EJCC_USE )   /*【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/
static u2 u2s_emfng_ecjmfdi;    /* lsb=1/3【6気筒】1/2【4気筒】,unit=rev :DI失火検出許可ｶｳﾝﾀ */
static u2 u2s_emfng_ecjmfpfi;   /* lsb=1/3【6気筒】1/2【4気筒】,unit=rev :PFI失火検出許可ｶｳﾝﾀ */
static u1 u1s_emfng_ecdmfdi[u1g_EJCC_NCYL];      /* lsb=1,unit=回 :気筒別DI失火ｶｳﾝﾀ */
static u1 u1s_emfng_ecdmfpfi[u1g_EJCC_NCYL];     /* lsb=1,unit=回 :気筒別PFI失火ｶｳﾝﾀ */
static u1 u1s_emfng_exdmfdim[u1g_EJCC_NCYL];     /* lsb=1 :気筒別DI異常成立ﾌﾗｸﾞ */
static u1 u1s_emfng_exdmfpfim[u1g_EJCC_NCYL];    /* lsb=1 :気筒別PFI異常成立ﾌﾗｸﾞ */
#endif
static u1 u1s_emfng_ekldmfw1_mcr; /* lsb=1,unit=回 :対向気筒失火回数判定下限値(計算値) */
#if JENCYL == u1g_EJCC_6CYL       /*【6気筒】*/
static u1 u1s_emfng_ekldmfw2_mcr; /* lsb=1,unit=回 :対向気筒失火回数判定上限値(計算値) */
#endif
static u2 u2s_emfng_eldmfst1_mcr; /* lsb=1,unit=回 :ｺｰﾙﾄﾞｽﾀｰﾄ時EM悪化ﾚﾍﾞﾙ失火回数判定値(計算値) */
static u2 u2s_emfng_eldmfst2_mcr; /* lsb=1,unit=回 :ﾎｯﾄｽﾀｰﾄ時EM悪化ﾚﾍﾞﾙ失火回数判定値(計算値) */
static u2 u2s_emfng_emfk_mcr;     /* lsb=1,unit=回 :DTC出力気筒判定値(計算値) */
#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
static u2 u2s_emfng_ecmfnl_mcr;   /* lsb=1,unit=回 :失火回数小頻度の判定値(計算値) */
static u2 u2s_emfng_ecmfnm_mcr;   /* lsb=1,unit=回 :失火回数中頻度の判定値(計算値) */
static u2 u2s_emfng_ecmfnh_mcr;   /* lsb=1,unit=回 :失火回数大頻度の判定値(計算値) */
#endif

static stflag8 sts_emfng_flagi1;   /* クランク角同期中(ne30m)タイミング操作フラグ */
#define bis_emfng_exjmf          (sts_emfng_flagi1.b0)  /* :失火の仮判定ﾌﾗｸﾞ(TDC) */
#define bis_emfng_exjtmf         (sts_emfng_flagi1.b1)  /* :失火の仮判定禁止ﾌﾗｸﾞ(TDC) */
#define bis_emfng_exjtmf2        (sts_emfng_flagi1.b2)  /* :失火の本判定許可ﾌﾗｸﾞ(TDC) */
#define bis_emfng_exmfkpl        (sts_emfng_flagi1.b3)  /* :低回転状態継続ﾌﾗｸﾞ(TDC) */
#define bis_emfng_exmfkpm        (sts_emfng_flagi1.b4)  /* :中回転状態継続ﾌﾗｸﾞ(TDC) */
#if ( JEEFI == u1g_EJCC_DUAL ) && ( JEHIPFI_E == u1g_EJCC_USE )   /*【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/
#define bis_emfng_exjmfdi        (sts_emfng_flagi1.b5)  /* :ecjmfdiｶｳﾝﾄｱｯﾌﾟ条件成立記憶ﾌﾗｸﾞ(TDC) */
#define bis_emfng_exjmfpfi       (sts_emfng_flagi1.b6)  /* :ecjmfpfiｶｳﾝﾄｱｯﾌﾟ条件成立記憶ﾌﾗｸﾞ(TDC) */
#endif

static stflag8 sts_emfng_flagi2;   /* 65msl,drvclchgタイミング操作フラグ */
#define bis_emfng_exottd         (sts_emfng_flagi2.b0)  /* :触媒OTﾚﾍﾞﾙ異常検出仮記憶ﾌﾗｸﾞ(65msl,drvclchg) */
#define bis_emfng_excdmfwclr     (sts_emfng_flagi2.b1)  /* :ecdmfw[]ｸﾘｱ要求ﾌﾗｸﾞ(65msl) */
#define bis_emfng_excdmfaot      (sts_emfng_flagi2.b2)  /* :触媒OT失火ｶｳﾝﾀによる触媒OT判定成立ﾌﾗｸﾞ(65msl) */
#define bis_emfng_excdmfem2clr   (sts_emfng_flagi2.b3)  /* :ecdmfem2[]ｸﾘｱ要求ﾌﾗｸﾞ(65msl) */
#define bis_emfng_excdmfae2clr   (sts_emfng_flagi2.b4)  /* :ecdmfae2ｸﾘｱ要求ﾌﾗｸﾞ(65msl) */
#define bis_emfng_excntmf        (sts_emfng_flagi2.b5)  /* :ﾄﾘｯﾌﾟ継続ﾌﾗｸﾞ(65msl,drvclchg) */
#define bis_emfng_exdmf1         (sts_emfng_flagi2.b6)  /* :触媒OTﾚﾍﾞﾙ現在異常記憶ﾌﾗｸﾞ(65msl,drvclchg) */

static stflag8 sts_emfng_flagi3;   /* 65mslタイミング操作フラグ */
#define bis_emfng_exdmfot        (sts_emfng_flagi3.b0)  /* :触媒OTﾚﾍﾞﾙ異常判定ﾌﾗｸﾞ(65msl) */
#define bis_emfng_exumfot        (sts_emfng_flagi3.b1)  /* :触媒OTﾚﾍﾞﾙ正常判定保留ﾌﾗｸﾞ(65msl) */
#define bis_emfng_exhmfoton      (sts_emfng_flagi3.b2)  /* :触媒OTによるMIL点滅要求ﾌﾗｸﾞ(65msl) */
#define bis_emfng_exhmfotoff     (sts_emfng_flagi3.b3)  /* :触媒OTによるMIL点滅ｷｬﾝｾﾙ要求ﾌﾗｸﾞ(65msl) */
#define bis_emfng_exdmfem        (sts_emfng_flagi3.b4)  /* :EM悪化ﾚﾍﾞﾙ異常検出ﾌﾗｸﾞ(65msl) */
#define bis_emfng_exumfem        (sts_emfng_flagi3.b5)  /* :EM悪化ﾚﾍﾞﾙ正常判定保留ﾌﾗｸﾞ(65msl) */

#if ( JEEFI == u1g_EJCC_DUAL ) && ( JEHIPFI_E == u1g_EJCC_USE )   /*【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/
static stflag8 sts_emfng_flagi4;   /* 65mslタイミング操作フラグ */
#define bis_emfng_exdmfdi        (sts_emfng_flagi4.b0)  /* :DI異常ﾌﾗｸﾞ(65msl) */
#define bis_emfng_exumfdi        (sts_emfng_flagi4.b1)  /* :DI異常による正常判定保留ﾌﾗｸﾞ(65msl) */
#define bis_emfng_exdmfpfi       (sts_emfng_flagi4.b2)  /* :PFI異常ﾌﾗｸﾞ(65msl) */
#define bis_emfng_exumfpfi       (sts_emfng_flagi4.b3)  /* :PFI異常による正常判定保留ﾌﾗｸﾞ(65msl) */
#define bis_emfng_excdmfdiclr    (sts_emfng_flagi4.b4)  /* :exdmfdim[]ｸﾘｱ用ﾌﾗｸﾞ(65msl) */
#define bis_emfng_excdmfpficlr   (sts_emfng_flagi4.b5)  /* :exdmfpfim[]ｸﾘｱ用ﾌﾗｸﾞ(65msl) */
#endif

#if JETCORE_D == u1g_EJCC_USE                           /*【t-CORE対応有】*/
static stflag8 sts_emfng_flagi5;   /* 65mslタイミング操作フラグ */
#define bis_emfng_exoutftp       (sts_emfng_flagi5.b0)  /* :OT失火モード領域外判定ﾌﾗｸﾞ(65msl) */
#define bis_emfng_exemfst        (sts_emfng_flagi5.b1)  /* :EM悪化レベル初回1000rev判定ﾌﾗｸﾞ(65msl) */
#endif

static s2 s2s_emfng_ecdmfaotb;  /* lsb=1,unit=回 :ecdmfaot-ecdtmf2のラッチ(MODE06用) */
static s4 s4s_emfng_emfrtotsum; /* lsb=256/128/256,unit=% :積算失火判定値 */
static u2 u2s_emfng_ecmfotcnt;  /* lsb=1,unit=回 :平均算出用積算回数 */
#if JEMFDTC_D == u1g_EJCC_USE  /*【失火OBD DTC有】*/
static u1 u1s_emfng_xem_lstpnd_Lch;     /* lsb=1 :前ﾄﾞﾗｲﾋﾞﾝｸﾞｻｲｸﾙ仮異常状態(EM)(他部品ﾗｯﾁ用) */
static u1 u1s_emfng_xot_lstpnd_Lch;     /* lsb=1 :前ﾄﾞﾗｲﾋﾞﾝｸﾞｻｲｸﾙ仮異常状態(OT)(他部品ﾗｯﾁ用) */
static u1 u1s_emfng_xot_warning_Lch;    /* lsb=1 :OT失火MIL状態(他部品ﾗｯﾁ用) */
static u1 u1s_emfng_xem_warning_Lch;    /* lsb=1 :EM失火MIL状態(他部品ﾗｯﾁ用) */
#endif
static u1 u1s_emfng_xreqclr_Lch;        /* lsb=1 :ﾀﾞｲｱｸﾞ情報ｸﾘｱ要求ﾌﾗｸﾞ(他部品ﾗｯﾁ用) */
static u1 u1s_emfng_state_Lch;          /* lsb=1 :検出感度状態(他部品ﾗｯﾁ用) */
#if ( JEMFDTC_D == u1g_EJCC_USE ) && ( JETCORE_D == u1g_EJCC_USE ) && ( JEOBDUDS_D == u1g_EJCC_USE )    /*【失火OBD DTC有】AND【t-CORE対応有】AND【OBDonUDS対応有】*/
static u1 u1s_emfng_pas;                /* EM悪化ﾚﾍﾞﾙ判定ﾀｲﾐﾝｸﾞ成立ﾌﾗｸﾞ(65msl) */
#endif

/*-------------------------------------------------------------------*/
/* const data定義                                                    */
/*-------------------------------------------------------------------*/
#pragma ghs startdata
/* バックアップRAM-ID */
static volatile const u2 u2s_ECDMFEMTV_U2_ID[u1g_EJCC_NCYL] =  /* :バックアップRAM-ID */
{
     u2g_EMFNG_ECDMFEMTV1_U2_ID
    ,u2g_EMFNG_ECDMFEMTV2_U2_ID
    ,u2g_EMFNG_ECDMFEMTV3_U2_ID
    ,u2g_EMFNG_ECDMFEMTV4_U2_ID
#if JENCYL == u1g_EJCC_6CYL          /*【6気筒】*/
    ,u2g_EMFNG_ECDMFEMTV5_U2_ID
    ,u2g_EMFNG_ECDMFEMTV6_U2_ID
#endif
};
static volatile const u2 u2s_ECDMFEMMNTL_U2_ID[u1g_EJCC_NCYL] =  /* :バックアップRAM-ID */
{
     u2g_EMFNG_ECDMFEMMNTL1_U2_ID
    ,u2g_EMFNG_ECDMFEMMNTL2_U2_ID
    ,u2g_EMFNG_ECDMFEMMNTL3_U2_ID
    ,u2g_EMFNG_ECDMFEMMNTL4_U2_ID
#if JENCYL == u1g_EJCC_6CYL          /*【6気筒】*/
    ,u2g_EMFNG_ECDMFEMMNTL5_U2_ID
    ,u2g_EMFNG_ECDMFEMMNTL6_U2_ID
#endif
};
static volatile const u2 u2s_ECDMFEMMXTL_U2_ID[u1g_EJCC_NCYL] =  /* :バックアップRAM-ID */
{
     u2g_EMFNG_ECDMFEMMXTL1_U2_ID
    ,u2g_EMFNG_ECDMFEMMXTL2_U2_ID
    ,u2g_EMFNG_ECDMFEMMXTL3_U2_ID
    ,u2g_EMFNG_ECDMFEMMXTL4_U2_ID
#if JENCYL == u1g_EJCC_6CYL          /*【6気筒】*/
    ,u2g_EMFNG_ECDMFEMMXTL5_U2_ID
    ,u2g_EMFNG_ECDMFEMMXTL6_U2_ID
#endif
};
static volatile const u2 u2s_ECDMFAVTV_U2_ID[u1g_EJCC_NCYL] =  /* :バックアップRAM-ID */
{
     u2g_EMFNG_ECDMFAVTV1_U2_ID
    ,u2g_EMFNG_ECDMFAVTV2_U2_ID
    ,u2g_EMFNG_ECDMFAVTV3_U2_ID
    ,u2g_EMFNG_ECDMFAVTV4_U2_ID
#if JENCYL == u1g_EJCC_6CYL          /*【6気筒】*/
    ,u2g_EMFNG_ECDMFAVTV5_U2_ID
    ,u2g_EMFNG_ECDMFAVTV6_U2_ID
#endif
};
static volatile const u2 u2s_ECDMFAVMNTL_U2_ID[u1g_EJCC_NCYL] =  /* :バックアップRAM-ID */
{
     u2g_EMFNG_ECDMFAVMNTL1_U2_ID
    ,u2g_EMFNG_ECDMFAVMNTL2_U2_ID
    ,u2g_EMFNG_ECDMFAVMNTL3_U2_ID
    ,u2g_EMFNG_ECDMFAVMNTL4_U2_ID
#if JENCYL == u1g_EJCC_6CYL          /*【6気筒】*/
    ,u2g_EMFNG_ECDMFAVMNTL5_U2_ID
    ,u2g_EMFNG_ECDMFAVMNTL6_U2_ID
#endif
};
static volatile const u2 u2s_ECDMFAVMXTL_U2_ID[u1g_EJCC_NCYL] =  /* :バックアップRAM-ID */
{
     u2g_EMFNG_ECDMFAVMXTL1_U2_ID
    ,u2g_EMFNG_ECDMFAVMXTL2_U2_ID
    ,u2g_EMFNG_ECDMFAVMXTL3_U2_ID
    ,u2g_EMFNG_ECDMFAVMXTL4_U2_ID
#if JENCYL == u1g_EJCC_6CYL          /*【6気筒】*/
    ,u2g_EMFNG_ECDMFAVMXTL5_U2_ID
    ,u2g_EMFNG_ECDMFAVMXTL6_U2_ID
#endif
};

#if JEOOBD == u1g_EJCC_USE           /*【市場調査有】*/
static volatile const u2 u2s_EOCDMFEM2MX_U1_ID[u1g_EJCC_NCYL] =  /* :バックアップRAM-ID */
{
     u2g_EMFNG_EOCDMFEM2MX1_U1_ID
    ,u2g_EMFNG_EOCDMFEM2MX2_U1_ID
    ,u2g_EMFNG_EOCDMFEM2MX3_U1_ID
    ,u2g_EMFNG_EOCDMFEM2MX4_U1_ID
 #if JENCYL == u1g_EJCC_6CYL         /*【6気筒】*/
    ,u2g_EMFNG_EOCDMFEM2MX5_U1_ID
    ,u2g_EMFNG_EOCDMFEM2MX6_U1_ID
 #endif
};
#endif

#if ( JETCORE_D == u1g_EJCC_USE ) && ( JEOBDUDS_D == u1g_EJCC_NOT_USE )  /*【t-CORE対応有】AND【OBDonUDS対応無】*/
static volatile const u2 u2s_emfng_ecdmfav_tbl[u1g_EJCC_NCYL] =
{
     u2g_WTCIF_DEM_DTR_ECDMFAV1
    ,u2g_WTCIF_DEM_DTR_ECDMFAV2
    ,u2g_WTCIF_DEM_DTR_ECDMFAV3
    ,u2g_WTCIF_DEM_DTR_ECDMFAV4
 #if JENCYL == u1g_EJCC_6CYL         /*【6気筒】*/
    ,u2g_WTCIF_DEM_DTR_ECDMFAV5
    ,u2g_WTCIF_DEM_DTR_ECDMFAV6
 #endif
};
static volatile const u2 u2s_emfng_ecdmfem_tbl[u1g_EJCC_NCYL] =
{
     u2g_WTCIF_DEM_DTR_ECDMFEM1
    ,u2g_WTCIF_DEM_DTR_ECDMFEM2
    ,u2g_WTCIF_DEM_DTR_ECDMFEM3
    ,u2g_WTCIF_DEM_DTR_ECDMFEM4
 #if JENCYL == u1g_EJCC_6CYL         /*【6気筒】*/
    ,u2g_WTCIF_DEM_DTR_ECDMFEM5
    ,u2g_WTCIF_DEM_DTR_ECDMFEM6
 #endif
};
#endif
#pragma ghs enddata

/*-------------------------------------------------------------------*/
/* 関数のprototype宣言                                               */
/*-------------------------------------------------------------------*/
#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
static void emfng_eocmfmlt_pt( const u1 ptt_ecdmfw[] ); /* :市場調査RAM更新処理 */
static void emfng_eocmfn_u2( u2 u2t_cdmfae );     /* :市場調査RAM更新処理 */
#endif
static u1 u1s_emfng_wxreqclr( void );             /* :市場調査RAM消去処理 */
static void emfng_mcr_cal( void );                /* :計算値変数の算出 */
static void emfng_xottd_cal( void );              /* :触媒OTﾚﾍﾞﾙ異常検出仮記憶ﾌﾗｸﾞの操作 */
static void emfng_cdmfi_reset( void );            /* :失火回数判定ｶｳﾝﾀ(ｱｲﾄﾞﾙ用)のｸﾘｱ処理 */
static void emfng_mf_cnt_u1u1( u1 u1t_mfkind, u1 u1t_mfcyl );             /* :失火カウンタの算出 */
static void emfng_ot_detect_ptptptpt( u1 *ptt_xumfot, u1 *ptt_xdmfot, u1 *ptt_xhmfoton, u1 *ptt_xhmfotoff );  /* :触媒OTﾚﾍﾞﾙ異常判定 */
static void emfng_em_detect_u2ptpt( u2 u2t_cdmfae2, u1 *ptt_xdmfem, u1 *ptt_xumfem );  /* :EM悪化レベル異常判定 */
#if ( JEEFI == u1g_EJCC_DUAL ) && ( JEHIPFI_E == u1g_EJCC_USE )   /*【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/
static void emfng_di_detect_ptpt( u1 *ptt_xdmfdi, u1 *ptt_xumfdi );     /* :DI異常検出 */
static void emfng_pfi_detect_ptpt( u1 *ptt_xdmfpfi, u1 *ptt_xumfpfi );  /* :PFI異常検出 */
#endif
#if JEMFDTC_D == u1g_EJCC_USE    /*【失火OBD DTC有】*/
static void emfng_wxmf_out( void );               /* :電技への通知処理 */
#endif
static void emfng_emode06_cal( void );            /* :mode$06対応失火ｶｳﾝﾀの操作 */
static u1 u1s_emfng_exrcdmf( void );              /* :EWMA失火ｶｳﾝﾀ計算履歴ﾌﾗｸﾞ取得処理 */
#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
static void emfng_espdaemclr( void );             /* :市場調査RAMｸﾘｱ処理 */
#endif
static void emfng_roughjdg_u1u1( u1 u1t_xmfne, u1 u1t_mfkind );  /* 悪路判定関連処理 */

/*-------------------------------------------------------------------*/
/* 関数                                                              */
/*-------------------------------------------------------------------*/
/*********************************************************************/
/*  関数名        | emfng_pwon( )                                    */
/*  処理内容      | 初期化処理                                       */
/*  制御タイミング| emfcnt_pwon()よりコール                          */
/*  引数          | なし                                             */
/*  戻り値        | なし                                             */
/*  注意事項      |                                                  */
/*********************************************************************/
void
emfng_pwon( void )
{
    /********************/
    /* 計算値変数の算出 */
    /********************/
    emfng_mcr_cal();
}

/*********************************************************************/
/*  関数名        | emfng_mfptn_ann_u1u1u1( )                        */
/*  処理内容      | 検出許可ｶｳﾝﾀの操作                               */
/*  制御タイミング| TDC毎(emfcnt_tdcm()よりコール)                   */
/*  引数          | u1t_mfkind     :lsb=1 失火種別                   */
/*                | u1t_mfcyl      :lsb=1 失火気筒                   */
/*                | u1t_xmfne      :lsb=1 失火本判定                 */
/*  戻り値        | なし                                             */
/*  注意事項      | 前提条件成立時に実施(emfcnt)                     */
/*********************************************************************/
void
emfng_mfptn_ann_u1u1u1( u1 u1t_mfkind, u1 u1t_mfcyl, u1 u1t_xmfne )
{
    s4 s4t_ecjmfot;         /* lsb=1/3【6気筒】1/2【4気筒】,unit=rev :触媒OTﾚﾍﾞﾙ失火検出許可ｶｳﾝﾀ */
    s4 s4t_ecjmfoti;        /* lsb=1/3【6気筒】1/2【4気筒】,unit=rev :触媒OTﾚﾍﾞﾙｱｲﾄﾞﾙ時失火検出許可ｶｳﾝﾀ */
    s4 s4t_ecjmfem;         /* lsb=1/3【6気筒】1/2【4気筒】,unit=rev :EM悪化ﾚﾍﾞﾙ失火検出許可ｶｳﾝﾀ */
    s4 s4t_ene;             /* lsb=12800/64/256,unit=rpm :ｴﾝｼﾞﾝ回転数 */
    s4 s4t_ejmfne;          /* lsb=1 :回転領域判定値 */
    s4 s4t_emfrtot;         /* lsb=256/128/256,unit=% :積算失火判定値補正係数 */
    s4 s4t_emfrtotsum;      /* lsb=256/128/256,unit=% :積算失火判定値 */
    u2 u2t_ecmfotcnt;       /* lsb=1,unit=回 :平均算出用積算回数 */
#if ( JEEFI == u1g_EJCC_DUAL ) && ( JEHIPFI_E == u1g_EJCC_USE )   /*【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/
    s2 s2t_ekpfi;           /* lsb=(1*4)/128/256,unit=倍 :ﾎﾟｰﾄ噴射量算出係数 */
    u2 u2t_ecjmfdi;         /* lsb=1/3【6気筒】1/2【4気筒】,unit=rev :DI失火検出許可ｶｳﾝﾀ */
    u2 u2t_ecjmfpfi;        /* lsb=1/3【6気筒】1/2【4気筒】,unit=rev :PFI失火検出許可ｶｳﾝﾀ */
#endif
    s2 s2t_MFNEL3_c;        /* lsb=12800/64/256,unit=rpm :アイドル領域判定値 */
    s2 s2t_eklsm;           /* lsb=320/64/256,unit=% :空気量負荷率（絶対質量） */

    /* ワークデータ初期化 */
    s4t_ene      = (s4)s2g_ene_ene;
    s4t_ejmfne   = (s4)u1g_emfne_ejmfne;
#if ( JEEFI == u1g_EJCC_DUAL ) && ( JEHIPFI_E == u1g_EJCC_USE )   /*【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/
    s2t_ekpfi    = s2g_eminj_ekpfi;
#endif
    s2t_MFNEL3_c = s2s_emfng_MFNEL3;
    s2t_eklsm = s2g_eklset_eklsm;

    /***************************************/
    /* 触媒OTﾚﾍﾞﾙ失火検出許可ｶｳﾝﾀのｲﾝｸﾘﾒﾝﾄ */
    /***************************************/
    s4t_ecjmfot = (s4)u2g_emfng_ecjmfot;
    if ( s4t_ecjmfot < (s4)u2g_U2MAX )
    {
        s4t_ecjmfot++;
    }
    /* RAMデータに格納 */
    u2g_emfng_ecjmfot = (u2)s4t_ecjmfot;              /* lsb=1/3【6気筒】1/2【4気筒】,unit=rev */

    /**********************************************/
    /* 触媒OTﾚﾍﾞﾙｱｲﾄﾞﾙ時失火検出許可ｶｳﾝﾀのｲﾝｸﾘﾒﾝﾄ */
    /**********************************************/
    s4t_ecjmfoti = (s4)u2g_emfng_ecjmfoti;
    if ( ( ( s4t_ejmfne == (s4)u1g_EMFNE_REV_IDLE2 )
#if JEMFHOUKI == u1g_EJCC_USAMF                                /*【対米法規】*/
        || ( s4t_ejmfne == (s4)u1g_EMFNE_REV_IDLE1 )
#endif
         )
      && ( s4t_ene < (s4)s2t_MFNEL3_c ) )
    {
        if ( s4t_ecjmfoti < (s4)u2g_U2MAX )
        {
            s4t_ecjmfoti++;
        }
    }
    /* RAMデータに格納 */
    u2g_emfng_ecjmfoti = (u2)s4t_ecjmfoti;             /* lsb=1/3【6気筒】1/2【4気筒】,unit=rev */

    /***************************************/
    /* EM悪化ﾚﾍﾞﾙ失火検出許可ｶｳﾝﾀのｲﾝｸﾘﾒﾝﾄ */
    /***************************************/
    s4t_ecjmfem = (s4)u2g_emfng_ecjmfem;
    if ( s4t_ecjmfem < (s4)u2g_U2MAX )
    {
        s4t_ecjmfem++;
    }
    /* RAMデータに格納 */
    u2g_emfng_ecjmfem = (u2)s4t_ecjmfem;               /* lsb=1/3【6気筒】1/2【4気筒】,unit=rev */

#if ( JEEFI == u1g_EJCC_DUAL ) && ( JEHIPFI_E == u1g_EJCC_USE )   /*【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/
    /*******************************/
    /* DI失火検出許可ｶｳﾝﾀのｲﾝｸﾘﾒﾝﾄ */
    /*******************************/
    u2t_ecjmfdi = u2s_emfng_ecjmfdi;
    if ( s2t_ekpfi <= s2g_emfok_NGDI )
    {
        if ( u2t_ecjmfdi < u2g_U2MAX )
        {
            u2t_ecjmfdi++;
        }
        bis_emfng_exjmfdi = (u1)ON;  /* ecjmfdiｶｳﾝﾄｱｯﾌﾟ条件成立記憶ﾌﾗｸﾞ */
    }
    else    /* ecjmfdiｶｳﾝﾄｱｯﾌﾟ条件不成立 */
    {
        bis_emfng_exjmfdi = (u1)OFF;
    }
    /* RAMデータに格納 */
    u2s_emfng_ecjmfdi = u2t_ecjmfdi;         /* lsb=1/3【6気筒】1/2【4気筒】,unit=rev */

    /********************************/
    /* PFI失火検出許可ｶｳﾝﾀのｲﾝｸﾘﾒﾝﾄ */
    /********************************/
    u2t_ecjmfpfi = u2s_emfng_ecjmfpfi;
    if ( s2t_ekpfi >= s2g_emfok_NGPFI )
    {
        if ( u2t_ecjmfpfi < u2g_U2MAX )
        {
            u2t_ecjmfpfi++;
        }
        bis_emfng_exjmfpfi = (u1)ON;  /* ecjmfpfiｶｳﾝﾄｱｯﾌﾟ条件成立記憶ﾌﾗｸﾞ */
    }
    else    /* ecjmfpfiｶｳﾝﾄｱｯﾌﾟ条件不成立 */
    {
        bis_emfng_exjmfpfi = (u1)OFF;
    }
    /* RAMデータに格納 */
    u2s_emfng_ecjmfpfi = u2t_ecjmfpfi;       /* lsb=1/3【6気筒】1/2【4気筒】,unit=rev */
#endif

    /**********************/
    /* 失火回数の積算処理 */
    /**********************/
    if ( u1t_mfkind != u1g_EMFCNT_NOT )
    {
        emfng_mf_cnt_u1u1( u1t_mfkind, u1t_mfcyl );
    }

    /********************/
    /* 悪路判定関連処理 */
    /********************/
    emfng_roughjdg_u1u1( u1t_xmfne, u1t_mfkind );

    /*******************************************/
    /* 積算失火判定値,平均算出用積算回数の算出 */
    /*******************************************/
    s4t_emfrtotsum = s4s_emfng_emfrtotsum;
    u2t_ecmfotcnt = u2s_emfng_ecmfotcnt;

    s4t_emfrtot = (s4)s2g_glmap2h_s2s2pt( s2t_eklsm, (s2)s4t_ene, &s2g_emfng_emfrtot_map[0] );
                                                    /* lsb=256/128/256,unit=% */
#if JENGPF_E != u1g_EJCC_NOT_USE       /*【GPF有】*/
    if ( ( u1g_egpfrgcnd_exgpfrgsrv == (u1)ON )
      || ( u1g_egpfhtum_exdth == (u1)ON )
      || ( u1g_egpfhtum_exeff == (u1)ON )
 #if JERLOK == u1g_EJCC_USE            /*【ﾘｯﾁﾘｰﾝ運転可】*/
      || ( u1g_egpfrgum_exrgact == (u1)ON )
 #endif
       )
    {
        s4t_emfrtot = (s4)s2g_glmap2h_s2s2pt( s2t_eklsm, (s2)s4t_ene, &s2g_emfng_emfrtotgpf_map[0] );
                                                    /* lsb=256/128/256,unit=% */
    }
#endif
    s4t_emfrtotsum = s4g_gladdst_s4s4( s4t_emfrtotsum, s4t_emfrtot );

    if ( u2t_ecmfotcnt < u2g_U2MAX )
    {
        u2t_ecmfotcnt++;
    }

    /* RAMデータに格納 */
    s4s_emfng_emfrtotsum = s4t_emfrtotsum;
    u2s_emfng_ecmfotcnt = u2t_ecmfotcnt;
}

#if JENCYL == u1g_EJCC_6CYL     /*【6気筒】*/
/*********************************************************************/
/*  関数名        | emfng_mfmlt_ann_u1u1( )                          */
/*  処理内容      | 対向気筒失火判定結果通知処理                     */
/*  制御タイミング| ATDC30CAタイミング                               */
/*                | (emfcnt_neg_mlt()よりコール)                     */
/*  引数          | u1t_mfkind     :lsb=1 失火種別                   */
/*                | u1t_mfcyl      :lsb=1 失火気筒                   */
/*  戻り値        | なし                                             */
/*  注意事項      | 前提条件成立時に実施(emfcnt)                     */
/*                |【6気筒】                                         */
/*********************************************************************/
void
emfng_mfmlt_ann_u1u1( u1 u1t_mfkind, u1 u1t_mfcyl )
{
    if ( u1t_mfkind == u1g_EMFCNT_OPPS )
    {
        emfng_mf_cnt_u1u1( u1t_mfkind, u1t_mfcyl );         /* 失火回数の積算処理 */
 #if JEEFI == u1g_EJCC_DUAL      /*【ﾃﾞｭｱﾙINJ】*/
        emfok_mfneld_set();                                 /* 対向気筒失火判定時の処理 */
        emfok_mfareacnt_ann_u1u1( u1t_mfkind, u1t_mfcyl );  /* 領域内失火回数の積算 */
 #endif
    }
}
#endif

/*********************************************************************/
/*  関数名        | emfng_tdcm( )                                    */
/*  処理内容      | TDCﾀｲﾐﾝｸﾞ処理                                    */
/*  制御タイミング| TDC毎(emfcnt_tdcm()よりコール)                   */
/*  引数          | なし                                             */
/*  戻り値        | なし                                             */
/*  注意事項      |                                                  */
/*********************************************************************/
void
emfng_tdcm( void )
{
    u2 u2t_ekldmfae;       /* lsb=1,unit=回 :EM悪化ﾚﾍﾞﾙ失火判定値 */
    s2 s2t_ene;            /* lsb=12800/64/256,unit=rpm :ｴﾝｼﾞﾝ回転数 */
    u1 u1t_state;          /* lsb=1 :検出感度状態 */
    u1 u1t_exmfkpl;        /* lsb=1 :低回転状態継続ﾌﾗｸﾞ */
    u1 u1t_exmfkpm;        /* lsb=1 :中回転状態継続ﾌﾗｸﾞ */

    u1t_state = u1g_wsentivchg_state;

    /************************/
    /* EM悪化ﾚﾍﾞﾙ失火判定値 */
    /************************/
    if ( u2g_emfng_ecjmfem == (u2)0U )
    {
        u2t_ekldmfae = u2g_emfng_eldmfaem_mcr;          /* lsb=1,unit=回 */
        if ( ( u1s_emfng_ecjemrev < u1s_emfng_CJEMREV )
          && ( u1t_state != u1g_WSENTIVCHG_LVL2 ) )
        {
            /*****************/
            /*   ﾎｯﾄｽﾀｰﾄ時   */
            /*****************/
            if ( big_emfng_exhstmf == (u1)ON )                  /* ﾎｯﾄｽﾀｰﾄ */
            {
                u2t_ekldmfae = u2s_emfng_eldmfst2_mcr;  /* lsb=1,unit=回 */
            }

            /*******************/
            /*   ｺｰﾙﾄﾞｽﾀｰﾄ時   */
            /*******************/
            if ( big_emfng_excstmf == (u1)ON )                  /* ｺｰﾙﾄﾞｽﾀｰﾄ */
            {
                u2t_ekldmfae = u2s_emfng_eldmfst1_mcr;  /* lsb=1,unit=回 */
            }
        }
        if ( u1t_state == u1g_WSENTIVCHG_IUE )
        {
            u2t_ekldmfae = u2g_glmulst_u2u2u2( u2t_ekldmfae, u2s_emfng_KLEVEL3, (u2)6U );  /* lsb=1*(4/256) → 1 */
        }
        /* RAMデータに格納 */
        u2g_emfng_ekldmfae = u2t_ekldmfae;          /* lsb=1,unit=回 */
    }

    /********************/
    /* NE継続ﾌﾗｸﾞの操作 */
    /********************/
    if ( u2g_emfng_ecjmfot == (u2)0U )
    {
        /* ワークデータ初期化 */
        s2t_ene = s2g_ene_ene;

        /* 低回転状態継続ﾌﾗｸﾞ(XMFKPL)の操作 */
        u1t_exmfkpl = (u1)OFF;
        if ( s2t_ene <= (s2)(((2000.)/(12800./64./256.))+0.5) )
        {
            u1t_exmfkpl = (u1)ON;
        }
        glbitcp_bibi( u1t_exmfkpl, bis_emfng_exmfkpl );

        /* 中回転状態継続ﾌﾗｸﾞ(XMFKPM)の操作 */
        u1t_exmfkpm = (u1)OFF;
        if ( s2t_ene <= (s2)(((3000.)/(12800./64./256.))+0.5) )
        {
            u1t_exmfkpm = (u1)ON;
        }
        glbitcp_bibi( u1t_exmfkpm, bis_emfng_exmfkpm );
    }
}

/*********************************************************************/
/*  関数名        | emfng_xjmf_ann_u1( )                             */
/*  処理内容      | 失火仮判定結果通知処理                           */
/*  制御タイミング| TDC毎(emfcnt_tdcm()よりコール)                   */
/*  引数          | u1t_xjmf :lsb=1 仮判定結果                       */
/*  戻り値        | なし                                             */
/*  注意事項      | 仮判定後に実施                                   */
/*********************************************************************/
void
emfng_xjmf_ann_u1( u1 u1t_xjmf )
{
    /* RAMデータに格納 */
    glbitcp_bibi( u1t_xjmf, bis_emfng_exjmf );
}

/*********************************************************************/
/*  関数名        | emfng_cjpmf_inc_u1( )                            */
/*  処理内容      | ｾﾞﾛｸﾛｽｶｳﾝﾀの操作                                 */
/*  制御タイミング| BTDC60CA(emfccrnkよりコール)【4気筒】            */
/*                | BTDC30CA(emfccrnkよりコール)【6気筒】            */
/*                | ATDC30CA(emfccrnkよりコール)                     */
/*  引数          | u1t_timming :lsb=1 ecjpmfｶｳﾝﾄｱｯﾌﾟﾀｲﾐﾝｸﾞ          */
/*  戻り値        | なし                                             */
/*  注意事項      | DLTMFL,DDTCR1算出後に実施                        */
/*********************************************************************/
void
emfng_cjpmf_inc_u1( u1 u1t_timming )
{
    u1 u1t_ecjpmf;      /* lsb=1,unit=回 :ｾﾞﾛｸﾛｽｶｳﾝﾀ */
    u1 u1t_ejmfne;      /* lsb=1 :回転領域 */
    u1 u1t_timjdg;      /* lsb=1 :ワーク変数 */
    u1 u1t_exjtmf;      /* lsb=1 :失火の仮判定禁止ﾌﾗｸﾞ */

    /* ワークデータ初期化 */
    u1t_ecjpmf = u1s_emfng_ecjpmf;
    u1t_ejmfne = u1g_emfne_ejmfne;
    u1t_exjtmf = (u1)bis_emfng_exjtmf;

    u1t_timjdg = (u1)OFF;
    if ( ( u1t_timming == u1g_EMFCCRNK_DLTMFL_TIMMING )
      && ( u1t_ejmfne  != u1g_EMFNE_REV_HIGH )
#if JEMFMETHOD_D == u1g_EJCC_TIME_D    /*【失火ΔT法】*/
      && ( s2g_emfccrnk_edltmflxh[0] >= (s2)0 )
      && ( s2g_emfccrnk_edltmflxh[1] <  (s2)0 )
#endif
#if JEMFMETHOD_D == u1g_EJCC_OMEGA_D   /*【失火Δω法】*/
      && ( s2g_emfccrnk_ewmfl[0] <= (s2)0 )
      && ( s2g_emfccrnk_ewmfl[1] >  (s2)0 )
#endif
       )
    {
        u1t_timjdg = (u1)ON;
    }
    if ( ( u1t_timming == u1g_EMFCCRNK_DDTCRX_TIMMING )
      && ( u1t_ejmfne  == u1g_EMFNE_REV_HIGH )
#if JEMFMETHOD_D == u1g_EJCC_TIME_D    /*【失火ΔT法】*/
      && ( s2g_emfccrnk_eddtcrxh[0] >= (s2)0 )
      && ( s2g_emfccrnk_eddtcrxh[1] <  (s2)0 )
#endif
#if JEMFMETHOD_D == u1g_EJCC_OMEGA_D   /*【失火Δω法】*/
 #if JENCYL == u1g_EJCC_6CYL    /*【6気筒】*/
      && ( s2g_emfccrnk_ewmfh[0] <= (s2)0 )
      && ( s2g_emfccrnk_ewmfh[1] >  (s2)0 )
 #endif
 #if JENCYL == u1g_EJCC_4CYL    /*【4気筒】*/
      && ( s2g_emfccrnk_ewmfhp[0] <= (s2)0 )
      && ( s2g_emfccrnk_ewmfhp[1] >  (s2)0 )
 #endif
#endif
       )
    {
        u1t_timjdg = (u1)ON;
    }
    if ( ( u1t_timjdg == (u1)ON )
      && ( bis_emfng_exjmf == (u1)OFF )
      && ( u1t_exjtmf == (u1)ON ) )
    {
        if ( u1t_ecjpmf < u1g_U1MAX )
        {
            u1t_ecjpmf++;
        }
    }
    if ( u1t_exjtmf == (u1)OFF )
    {
        u1t_ecjpmf = (u1)0U;
    }

    /* RAMデータに格納 */
    u1s_emfng_ecjpmf = u1t_ecjpmf;      /* lsb=1,unit=回 */
}

/*********************************************************************/
/*  関数名        | emfng_mf_cnt_u1u1( )                             */
/*  処理内容      | 失火カウンタの算出                               */
/*  制御タイミング| 失火判定成立時                                   */
/*                | TDC毎(emfng_mfptn_ann()よりコール)               */
/*                | ATDC30CA(emfng_mfmlt_ann()よりコール)【6気筒】   */
/*  引数          | u1t_mfkind     :lsb=1 失火種別                   */
/*                | u1t_mfcyl      :lsb=1 失火気筒                   */
/*  戻り値        | なし                                             */
/*  注意事項      | 内部関数                                         */
/*********************************************************************/
static void
emfng_mf_cnt_u1u1( u1 u1t_mfkind, u1 u1t_mfcyl )
{
    s4 s4t_i;               /* lsb=1 :forループ用変数 */
    u1 u1t_bit;             /* :失火気筒ﾌﾗｸﾞ */
    s4 s4t_ecdmfaotbnk[2];  /* lsb=1,unit=回 :ﾊﾞﾝｸ別触媒OTﾚﾍﾞﾙ失火ｶｳﾝﾀ */
    s4 s4t_ecdmfaot;        /* lsb=1,unit=回 :触媒OTﾚﾍﾞﾙ全気筒失火ｶｳﾝﾀ */
    s4 s4t_ecdmfaoti;       /* lsb=1,unit=回 :ｱｲﾄﾞﾙ中全気筒失火ｶｳﾝﾀ */
    s4 s4t_ecdmfae2;        /* lsb=1,unit=回 :EM悪化ﾚﾍﾞﾙ全気筒失火ｶｳﾝﾀ */
    s4 s4t_ecdmfaem;        /* lsb=1,unit=回 :EM悪化ﾚﾍﾞﾙ全気筒失火ｶｳﾝﾀ */
    s4 s4t_ecmfot;          /* lsb=1,unit=回 :複数気筒失火ｶｳﾝﾀ */
#if JEOOBD == u1g_EJCC_USE             /*【市場調査有】*/
    s2 s2t_espd;            /* lsb=256/256,unit=km/h :車速 */
    s2 s2t_ene;             /* lsb=12800/64/256,unit=rpm :ｴﾝｼﾞﾝ回転数 */
    s2 s2t_eklsm;           /* lsb=320/64/256,unit=% :空気量負荷率（絶対質量） */
    u1 u1t_ejmfne;          /* lsb=1 :回転領域 */
#endif
    s4 s4t_idx;             /* lsb=1 :気筒群別の要素数 */
    s4 s4t_bnk;             /* lsb=1 :失火気筒の属するバンク */
    s4 s4t_indx;            /* lsb=1 :失火気筒の属する実気筒番号 */
    u1 u1t_ncyl;            /* ワーク変数 */
    u1 u1t_ncyl_c;          /* lsb=1 :気筒数 */
    u1 u1t_ncylw_c;         /* lsb=1 :気筒群数 */

    if ( u1t_mfkind == u1g_EMFCNT_MLT )
    {
        /******************************/
        /* 対向気筒以外の複数気筒失火 */
        /******************************/
        s4t_ecmfot = (s4)u2g_emfng_ecmfot;
        if ( s4t_ecmfot < (s4)u2g_U2MAX )
        {
            s4t_ecmfot++;                           /* CMFOTのｲﾝｸﾘﾒﾝﾄ */
        }
        u2g_emfng_ecmfot = (u2)s4t_ecmfot;          /* lsb=1,unit=回 */
    }
    else if ( u1t_mfkind == u1g_EMFCNT_OPPS )
    {
        /****************/
        /* 対向気筒失火 */
        /****************/
        u1t_ncylw_c = u1s_EMFNG_NCYLW;
        for ( s4t_i = (s4)0; s4t_i < (s4)u1t_ncylw_c; s4t_i++ )
        {
            if ( u1g_emfng_mfwcyl_tbl[s4t_i] == u1t_mfcyl )
            {
                s4t_idx = s4t_i;
                if ( u1g_emfng_ecdmfw[s4t_idx] < u1g_U1MAX )
                {
                    u1g_emfng_ecdmfw[s4t_idx]++;   /* 気筒群別の対向気筒失火ｶｳﾝﾀ */
                }
            }
        }
    }
    else
    {
        /****************************************/
        /* ｱｲﾄﾞﾙ/ﾗﾝﾀﾞﾑ/単一連続/2連続/間欠 失火 */
        /****************************************/
        s4t_ecdmfaot       = (s4)u1g_emfng_ecdmfaot;
        s4t_ecdmfae2       = (s4)u2g_emfng_ecdmfae2;
        s4t_ecdmfaem       = (s4)u1g_emfng_ecdmfaem;
        s4t_ecdmfaotbnk[0] = (s4)u1g_emfng_ecdmfaotbnk[0];
        s4t_ecdmfaotbnk[1] = (s4)u1g_emfng_ecdmfaotbnk[1];
        s4t_ecdmfaoti      = (s4)u2g_emfng_ecdmfaoti;
#if JEOOBD == u1g_EJCC_USE             /*【市場調査有】*/
        s2t_espd           = s2g_espd_espd;
        s2t_ene            = s2g_ene_ene;
        s2t_eklsm          = s2g_eklset_eklsm;
        u1t_ejmfne         = u1g_emfne_ejmfne;
#endif
        u1t_ncyl_c = u1g_ejcc_NCYL;
        u1t_ncyl = u1t_mfcyl;
        for ( s4t_i = (s4)0; s4t_i < (s4)u1t_ncyl_c; s4t_i++ )
        {
            u1t_bit = (u1)(u1t_ncyl & (u1)0x01U);
            if ( u1t_bit != (u1)0x00U )
            {
                s4t_bnk = (s4)u1s_emfng_mfcylbnk_tbl[s4t_i];   /* MFCYLからバンクを求める */
                s4t_ecdmfaotbnk[s4t_bnk]++;                    /* ﾊﾞﾝｸ別触媒OTﾚﾍﾞﾙ失火ｶｳﾝﾀ */
                s4t_ecdmfaot++;                                /* 触媒OTﾚﾍﾞﾙ全気筒失火ｶｳﾝﾀ */
                s4t_ecdmfaoti++;                               /* ｱｲﾄﾞﾙ中全気筒失火ｶｳﾝﾀ */

                s4t_indx = (s4)u1g_eengspc_ewcnvt_tbl[s4t_i];  /* MFCYLから実気筒番号を求める */
                if ( u1g_emfng_ecdmfem[s4t_indx] < u1g_U1MAX )
                {
                    u1g_emfng_ecdmfem[s4t_indx]++;             /* EM悪化ﾚﾍﾞﾙ気筒別失火ｶｳﾝﾀ */
                }
                if ( u2g_emfng_ecdmfem2[s4t_indx] < u2g_U2MAX )
                {
                    u2g_emfng_ecdmfem2[s4t_indx]++;            /* EM悪化ﾚﾍﾞﾙ気筒別失火ｶｳﾝﾀ */
                }
                if ( u1g_emfng_ecdmfemi[s4t_indx] < u1g_U1MAX )
                {
                    u1g_emfng_ecdmfemi[s4t_indx]++;            /* ｱｲﾄﾞﾙ中気筒別失火ｶｳﾝﾀ */
                }
                s4t_ecdmfae2++;            /* EM悪化ﾚﾍﾞﾙ全気筒失火ｶｳﾝﾀ */
                s4t_ecdmfaem++;            /* EM悪化ﾚﾍﾞﾙ全気筒失火ｶｳﾝﾀ */
#if ( JEEFI == u1g_EJCC_DUAL ) && ( JEHIPFI_E == u1g_EJCC_USE )   /*【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/
                if ( bis_emfng_exjmfdi == (u1)ON )
                {
                    if ( u1s_emfng_ecdmfdi[s4t_indx] < u1g_U1MAX )
                    {
                        u1s_emfng_ecdmfdi[s4t_indx]++;             /* 気筒別DI失火ｶｳﾝﾀ */
                    }
                }
                if ( bis_emfng_exjmfpfi == (u1)ON )
                {
                    if ( u1s_emfng_ecdmfpfi[s4t_indx] < u1g_U1MAX )
                    {
                        u1s_emfng_ecdmfpfi[s4t_indx]++;            /* 気筒別PFI失火ｶｳﾝﾀ */
                    }
                }
#endif

#if JEOOBD == u1g_EJCC_USE      /*【市場調査有】*/
                s4s_emfng_espdaem  = s4g_gladdst_s4s4( s4s_emfng_espdaem, (s4)s2t_espd );
                s4s_emfng_eneaem   = s4g_gladdst_s4s4( s4s_emfng_eneaem, (s4)s2t_ene );
                s4s_emfng_eklsmaem = s4g_gladdst_s4s4( s4s_emfng_eklsmaem, (s4)s2t_eklsm );

                if ( ( u1t_ejmfne == u1g_EMFNE_REV_IDLE2 )
 #if JEMFHOUKI == u1g_EJCC_USAMF /*【対米法規】*/
                  || ( u1t_ejmfne == u1g_EMFNE_REV_IDLE1 )
 #endif
                   )
                {
                    if ( u1s_emfng_ecjmfneemi < u1g_U1MAX )
                    {
                        u1s_emfng_ecjmfneemi++;
                    }
                }
                if ( u1t_ejmfne == u1g_EMFNE_REV_LOW )
                {
                    if ( u1s_emfng_ecjmfneeml < u1g_U1MAX )
                    {
                        u1s_emfng_ecjmfneeml++;
                    }
                }
                if ( u1t_ejmfne == u1g_EMFNE_REV_HIGH )
                {
                    if ( u1s_emfng_ecjmfneemh < u1g_U1MAX )
                    {
                        u1s_emfng_ecjmfneemh++;
                    }
                 }
#endif

            }
            u1t_ncyl = (u1)(u1t_ncyl >> 1);
        }

        /* ガード処理 */
        if ( s4t_ecdmfaotbnk[0] > (s4)u1g_U1MAX )
        {
            s4t_ecdmfaotbnk[0] = (s4)u1g_U1MAX;
        }
        if ( s4t_ecdmfaotbnk[1] > (s4)u1g_U1MAX )
        {
            s4t_ecdmfaotbnk[1] = (s4)u1g_U1MAX;
        }
        if ( s4t_ecdmfaot > (s4)u1g_U1MAX )
        {
            s4t_ecdmfaot = (s4)u1g_U1MAX;
        }
        if ( s4t_ecdmfaoti > (s4)u2g_U2MAX )
        {
            s4t_ecdmfaoti = (s4)u2g_U2MAX;
        }
        if ( s4t_ecdmfae2 > (s4)u2g_U2MAX )
        {
            s4t_ecdmfae2 = (s4)u2g_U2MAX;
        }
        if ( s4t_ecdmfaem > (s4)u1g_U1MAX )
        {
            s4t_ecdmfaem = (s4)u1g_U1MAX;
        }

        /* RAMデータに格納 */
        u2g_emfng_ecdmfae2       = (u2)s4t_ecdmfae2;            /* lsb=1,unit=回 */
        u1g_emfng_ecdmfaem       = (u1)s4t_ecdmfaem;            /* lsb=1,unit=回 */
        u1g_emfng_ecdmfaotbnk[0] = (u1)s4t_ecdmfaotbnk[0];      /* lsb=1,unit=回 */
        u1g_emfng_ecdmfaotbnk[1] = (u1)s4t_ecdmfaotbnk[1];      /* lsb=1,unit=回 */
        u1g_emfng_ecdmfaot       = (u1)s4t_ecdmfaot;            /* lsb=1,unit=回 */
        u2g_emfng_ecdmfaoti      = (u2)s4t_ecdmfaoti;           /* lsb=1,unit=回 */

    }   /* (u1t_mfkind == u1g_EMFCNT_MLT) end */
}

/*********************************************************************/
/*  関数名        | emfng_reset( )                                   */
/*  処理内容      | 失火判定ﾘｾｯﾄ処理                                 */
/*  制御タイミング| TDC毎(emfcnt_tdcm()よりコール)                   */
/*  引数          | なし                                             */
/*  戻り値        | なし                                             */
/*  注意事項      | 前提条件不成立時に実施(emfcnt)                   */
/*********************************************************************/
void
emfng_reset( void )
{
    bis_emfng_exjmf   = (u1)OFF;
    bis_emfng_exjtmf  = (u1)OFF;
    bis_emfng_exjtmf2 = (u1)OFF;
}

/*********************************************************************/
/*  関数名        | emfng_drvclchg( )                                */
/*  処理内容      | ドライビングサイクル移行時の初期化処理           */
/*  制御タイミング| emfcnt_drvclchg()よりコール                      */
/*  引数          | なし                                             */
/*  戻り値        | なし                                             */
/*  注意事項      |                                                  */
/*********************************************************************/
void
emfng_drvclchg( void )
{
    s4 s4t_i;             /* lsb=1 :for loop用 */
    u1 u1t_ncyl_c;        /* lsb=1 :気筒数 */
    u1 u1t_ncylw_c;       /* lsb=1 :気筒群数 */

    /* ▼▼▼ 割り込み禁止 ▼▼▼ */ /* コール元emfcnt_drvclchg()で実施 */
    u2g_emfng_ecjmfot        = (u2)0;     /* 0初期化のためLSB省略 */
    u2g_emfng_ecjmfem        = (u2)0;     /* 0初期化のためLSB省略 */
    u2g_emfng_ecjmfoti       = (u2)0;     /* 0初期化のためLSB省略 */
#if JETCORE_D == u1g_EJCC_USE     /*【t-CORE対応有】*/
    big_emfng_exdmf     = (u1)OFF;
#endif
#if ( JEEFI == u1g_EJCC_DUAL ) && ( JEHIPFI_E == u1g_EJCC_USE )   /*【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/
    u2s_emfng_ecjmfdi   = (u2)0;     /* 0初期化のためLSB省略 */
    u2s_emfng_ecjmfpfi  = (u2)0;     /* 0初期化のためLSB省略 */
#endif
    u1t_ncyl_c = u1g_ejcc_NCYL;
    u1t_ncylw_c = u1s_EMFNG_NCYLW;

    u2g_emfng_ecmfot         = (u2)0U;
    for ( s4t_i = (s4)0; s4t_i < (s4)u1t_ncylw_c; s4t_i++ )
    {
        u1g_emfng_ecdmfw[s4t_i]    = (u1)0U;
    }
    u1g_emfng_ecdmfaotbnk[0] = (u1)0U;     /* CJMFOTと同時にクリアする */
    u1g_emfng_ecdmfaotbnk[1] = (u1)0U;     /* CJMFOTと同時にクリアする */
    u1g_emfng_ecdmfaot       = (u1)0U;     /* CJMFOTと同時にクリアする */
    u2g_emfng_ecdmfaoti      = (u2)0U;     /* CJMFOTIと同時にクリアする */
    u2g_emfng_ecdmfae2       = (u2)0U;     /* CJMFEMと同時にクリアする */
    u1g_emfng_ecdmfaem       = (u1)0U;     /* CJMFEMと同時にクリアする */
    u1g_emfng_ecdtmf2        = (u1)0U;
    for ( s4t_i = (s4)0; s4t_i < (s4)u1t_ncyl_c ; s4t_i++ )
    {
        u1g_emfng_ecdmfem[s4t_i]   = (u1)0U;
        u2g_emfng_ecdmfem2[s4t_i]  = (u2)0U;
        u1g_emfng_ecdmfemi[s4t_i]  = (u1)0U;     /* CJMFEMと同時にクリアする */
    }
#if ( JEEFI == u1g_EJCC_DUAL ) && ( JEHIPFI_E == u1g_EJCC_USE )   /*【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/
    for ( s4t_i = (s4)0; s4t_i < (s4)u1t_ncyl_c; s4t_i++ )
    {
        u1s_emfng_ecdmfdi[s4t_i]   = (u1)(((0.)/(1.))+0.5);  /* ecjmfdiと同時にクリアする */
        u1s_emfng_ecdmfpfi[s4t_i]  = (u1)(((0.)/(1.))+0.5);  /* ecjmfpfiと同時にクリアする */
    }
#endif
    u2s_emfng_ecdtmfwa      = (u2)0U;

    u1s_emfng_ecxmf         = (u1)0U;
    u1s_emfng_ecxmf1        = (u1)0U;
    big_emfng_exjokmf       = (u1)OFF;
    u1s_emfng_ecjemrev      = (u1)0U;
    bis_emfng_excntmf       = (u1)OFF;
    bis_emfng_exdmf1        = (u1)OFF;
    bis_emfng_exottd        = (u1)OFF;
    s4s_emfng_emfrtotsum    = (s4)(((0.)/(256./128./256.))+0.5);
    u2s_emfng_ecmfotcnt     = (u2)0U;
#if ( JEEFI == u1g_EJCC_DUAL ) && ( JEHIPFI_E == u1g_EJCC_USE )   /*【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/
    big_emfng_exmfddi   = (u1)OFF;
    big_emfng_exmfdied  = (u1)OFF;
    big_emfng_exmfdpfi  = (u1)OFF;
    big_emfng_exmfpfied = (u1)OFF;
#endif

#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
    emfng_espdaemclr();
#endif
    /* ▲▲▲ 割り込み許可 ▲▲▲ */ /* コール元emfcnt_drvclchg()で実施 */
}

/*********************************************************************/
/*  関数名        | emfng_16msl( )                                   */
/*  処理内容      | 16msﾀｲﾐﾝｸﾞ処理                                   */
/*  制御タイミング| 16ms毎(emfcnt_16msl()よりコール)                 */
/*  引数          | なし                                             */
/*  戻り値        | なし                                             */
/*  注意事項      |                                                  */
/*********************************************************************/
void
emfng_16msl( void )
{
    s4 s4t_ejmfne;         /* lsb=1 :回転領域 */
    s2 s2t_ethw;           /* lsb=160/256,unit=℃ :ｴﾝｼﾞﾝ制御用水温 */
    s2 s2t_etha;           /* lsb=160/256,unit=℃ :ｴﾝｼﾞﾝ制御用吸気温 */
    s2 s2t_THA_c;          /* lsb=160/256,unit=℃ :ホットスタート判定吸気温 */
    s2 s2t_THWCSTH_c;      /* lsb=160/256,unit=℃ :コールドスタート判定水温 */
    s2 s2t_THACST_c;       /* lsb=160/256,unit=℃ :コールドスタート判定吸気温 */
    s2 s2t_MFNEL3_c;       /* lsb=12800/64/256,unit=rpm :アイドル領域判定値 */

    s2t_ethw = s2g_ethw_ethw;
    s2t_etha = s2g_etha_etha;
    s2t_MFNEL3_c = s2s_emfng_MFNEL3;

    if ( u1g_exst_exastnrm == (u1)OFF )
    {
        /***************/
        /* ﾎｯﾄｽﾀｰﾄ判定 */
        /***************/
        s2t_THA_c = s2s_emfng_THA;
        if ( ( s2t_ethw >= s2s_emfng_THW )
          && ( s2t_etha >= s2t_THA_c ) )
        {
            big_emfng_exhstmf = (u1)ON;
        }
        else
        {
            big_emfng_exhstmf = (u1)OFF;
        }

        /*****************/
        /* ｺｰﾙﾄﾞｽﾀｰﾄ判定 */
        /*****************/
        s2t_THWCSTH_c = s2s_emfng_THWCSTH;
        s2t_THACST_c = s2s_emfng_THACST;
        if ( ( s2t_ethw <= s2s_emfng_THWCSTL )
          || ( ( s2t_ethw <= s2t_THWCSTH_c )
            && ( s2t_etha <= s2t_THACST_c ) ) )
        {
            big_emfng_excstmf = (u1)ON;
        }
        else
        {
            big_emfng_excstmf = (u1)OFF;
        }
    }

    /**********************************************/
    /* 触媒OTﾚﾍﾞﾙｱｲﾄﾞﾙ時失火検出許可ｶｳﾝﾀのｸﾘｱ処理 */
    /**********************************************/
    /* ワークデータ初期化 */
    s4t_ejmfne = (s4)u1g_emfne_ejmfne;
    if ( ( ( s4t_ejmfne != (s4)u1g_EMFNE_REV_IDLE2 )
#if JEMFHOUKI == u1g_EJCC_USAMF                                /*【対米法規】*/
        && ( s4t_ejmfne != (s4)u1g_EMFNE_REV_IDLE1 )
#endif
         )
      || ( s2g_ene_ene >= s2t_MFNEL3_c ) )
    {
        emfng_cdmfi_reset();
    }
}

/*********************************************************************/
/*  関数名        | emfng_65msl( )                                   */
/*  処理内容      | 65msﾀｲﾐﾝｸﾞ処理                                   */
/*  制御タイミング| 65ms毎(emfcnt_65msl()よりコール)                 */
/*  引数          | なし                                             */
/*  戻り値        | なし                                             */
/*  注意事項      |                                                  */
/*********************************************************************/
void
emfng_65msl( void )
{
    u1 u1t_xdmfem;          /* lsb=1 :EM悪化ﾚﾍﾞﾙ異常検出ﾌﾗｸﾞ */
    u1 u1t_xdmfot;          /* lsb=1 :触媒OTﾚﾍﾞﾙ異常検出ﾌﾗｸﾞ */
    u1 u1t_xumfot;          /* lsb=1 :触媒OTﾚﾍﾞﾙ正常判定保留ﾌﾗｸﾞ */
    u1 u1t_xhmfoton;        /* lsb=1 :触媒OTによるMIL点滅要求ﾌﾗｸﾞ */
    u1 u1t_xhmfotoff;       /* lsb=1 :触媒OTによるMIL点滅ｷｬﾝｾﾙ要求ﾌﾗｸﾞ */
    u1 u1t_xumfem;          /* lsb=1 :EM悪化ﾚﾍﾞﾙ正常判定保留ﾌﾗｸﾞ */
    u2 u2t_cdtmfwa;         /* lsb=1,unit=回 :EM悪化ﾚﾍﾞﾙ判定用悪路ｶｳﾝﾀ */
    s4 s4t_cdmfae2;         /* lsb=1,unit=回 :EM悪化レベル全気筒失火カウンタ(判定用) */
    u4  u4t_cdmfaavTv;      /* lsb=1,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ(全気筒)ﾜｰｸ */
    u2  u2t_cdmfaavTv;      /* lsb=1,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ(全気筒)ﾜｰｸ(書き込み用) */
    u2  u2t_ECDMFAAVMNTL_MI_c;  /* lsb=1,unit=回 :テスト結果出力用過去10トリップEWMA失火カウンタ下限値(全気筒)初期値 */
    u2  u2t_cdmfaavMxtl;    /* lsb=1,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ(全気筒)ﾜｰｸ(書き込み用) */
    u4  u4t_cdmfaemTv;      /* lsb=1,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(全気筒)ﾜｰｸ           【mode$06-TID$0C】*/
    u4  u4t_cdmfavTv;       /* lsb=1,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ(気筒別)ﾜｰｸ 【mode$06-TID$0B】*/
    u2  u2t_cdmfavTv;       /* lsb=1,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ(気筒別)ﾜｰｸ(書き込み用) 【mode$06-TID$0B】*/
    u2  u2t_ECDMFAVMNTL_MI_c ;  /* lsb=1,unit=回 :テスト結果出力用過去10トリップEWMA失火カウンタ下限値(気筒別)初期値 */
    u2  u2t_cdmfavMxtl;     /* lsb=1,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ(気筒別)ﾜｰｸ(書き込み用) 【mode$06-TID$0B】*/
    u4  u4t_cdmfemTv;       /* lsb=1,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(気筒別)ﾜｰｸ           【mode$06-TID$0C】*/
    s4  s4t_cyl;            /* lsb=1,unit=気筒 */
    u1  u1t_exrcdmf;        /* lsb=1 :EWMA失火ｶｳﾝﾀ計算履歴ﾌﾗｸﾞ */
    u2  u2t_ECDMFAEMTV_MI_c;    /* lsb=1,unit=回 :テスト結果出力用今トリップ累積失火カウンタ(全気筒)初期値 */
    u2  u2t_ECDMFEMTV_MI_c;     /* lsb=1,unit=回 :テスト結果出力用今トリップ累積失火カウンタ(気筒別)初期値 */
#if JEMAT_OBD2 == u1g_EJCC_NOT_USE    /*【OBD2適合無】*/
    u1  u1t_xmfKwp;         /* lsb=1 :OBD2ﾀﾞｲｱｸﾞﾏｽｸ情報 失火 */
#endif

    u1  u1t_xjokmf;         /* lsb=1 : 始動後所定回転完了ﾌﾗｸﾞ */
    u1  u1t_cjemrev;        /* lsb=1, unit=回 : 始動後ｴﾐｯｼｮﾝ悪化ﾚﾍﾞﾙ判定回数 */
    u1  u1t_xreqclr;        /* lsb=1 :ﾃﾞｰﾀ消去要求ﾌﾗｸﾞ */
#if ( JEEFI == u1g_EJCC_DUAL ) && ( JEHIPFI_E == u1g_EJCC_USE )   /*【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/
    u1  u1t_xdmfdi;         /* lsb=1 : DI異常ﾌﾗｸﾞ */
    u1  u1t_xumfdi;         /* lsb=1 : DI異常による正常判定保留ﾌﾗｸﾞ */
    u1  u1t_xdmfpfi;        /* lsb=1 : PFI異常ﾌﾗｸﾞ */
    u1  u1t_xumfpfi;        /* lsb=1 : PFI異常による正常判定保留ﾌﾗｸﾞ */
    s4  s4t_i;              /* lsb=1 :forループ用変数 */
#endif
    u1  u1t_exdisable;      /* lsb=1 :Disableﾌﾗｸﾞ */
    u1  u1t_excomplete;     /* lsb=1 :Completeﾌﾗｸﾞ */
    u2  u2t_LJMFOT_c;       /* lsb=2/u1g_EJCC_NCYL,unit=rev :触媒OTレベル異常判定実行タイミング */
    u2  u2t_LJMFEM_c;       /* lsb=2/u1g_EJCC_NCYL,unit=rev :EM悪化レベル異常判定実行タイミング */
    u2  u2t_LJMFOTF_c;      /* lsb=2/u1g_EJCC_NCYL,unit=rev :触媒OTレベル異常判定実行タイミング */
#if ( JETCORE_D == u1g_EJCC_USE ) && ( JEOBDUDS_D == u1g_EJCC_NOT_USE )     /*【t-CORE対応有】AND【OBDonUDS対応無】*/
    u2 u2t_ecdmfaavTv;      /* lsb=1,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ(全気筒)(戻り値取得用) */
    u2 u2t_ecdmfaavMntl;    /* lsb=1,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ下限値(全気筒)(戻り値取得用) */
    u2 u2t_ecdmfaavMxtl;    /* lsb=1,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ上限値(全気筒)(戻り値取得用) */
    u2 u2t_dtrid;           /* lsb=1 :ﾃｽﾄ結果出力ＩＤ */
    u2 u2t_ecdmfavTv;       /* lsb=1,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ(気筒別)(戻り値取得用) */
    u2 u2t_ecdmfavMntl;     /* lsb=1,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ下限値(気筒別)(戻り値取得用) */
    u2 u2t_ecdmfavMxtl;     /* lsb=1,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ上限値(気筒別)(戻り値取得用) */
    u2 u2t_ecdmfaemTv;      /* lsb=1,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(全気筒)(戻り値取得用) */
    u2 u2t_ecdmfaemMntl;    /* lsb=1,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ下限値(全気筒)(戻り値取得用) */
    u2 u2t_ecdmfaemMxtl;    /* lsb=1,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ上限値(全気筒)(戻り値取得用) */
    u2 u2t_ecdmfemTv;       /* lsb=1,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(気筒別)(戻り値取得用) */
    u2 u2t_ecdmfemMntl;     /* lsb=1,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ下限値(気筒別)(戻り値取得用) */
    u2 u2t_ecdmfemMxtl;     /* lsb=1,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ上限値(気筒別)(戻り値取得用) */
#endif
    u1 u1t_ncyl_c;          /* lsb=1 :気筒数 */

    /* ワークデータ初期化 */
    u1t_xdmfem    = (u1)OFF;
    u1t_xdmfot    = (u1)OFF;
    u1t_xumfot    = (u1)OFF;
    u1t_xhmfoton  = (u1)OFF;
    u1t_xhmfotoff = (u1)OFF;
    u1t_xumfem    = (u1)OFF;
#if ( JEEFI == u1g_EJCC_DUAL ) && ( JEHIPFI_E == u1g_EJCC_USE )   /*【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/
    u1t_xumfdi    = (u1)OFF;
    u1t_xumfpfi   = (u1)OFF;
    u1t_xdmfdi    = (u1)OFF;
    u1t_xdmfpfi   = (u1)OFF;
#endif

    big_emfng_excdmfemclr  = (u1)OFF;                   /* ecdmfem[]ｸﾘｱ要求ﾌﾗｸﾞ */
    bis_emfng_excdmfem2clr = (u1)OFF;                   /* ecdmfem2[]ｸﾘｱ要求ﾌﾗｸﾞ */
    bis_emfng_excdmfwclr   = (u1)OFF;                   /* ecdmfw[]ｸﾘｱ要求ﾌﾗｸﾞ */
    bis_emfng_excdmfae2clr = (u1)OFF;
#if JETCORE_D == u1g_EJCC_USE         /*【t-CORE対応有】*/
    bis_emfng_exoutftp     = (u1)OFF;
    bis_emfng_exemfst      = (u1)OFF;
#endif

#if ( JEEFI == u1g_EJCC_DUAL ) && ( JEHIPFI_E == u1g_EJCC_USE )   /*【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/
    bis_emfng_excdmfdiclr  = (u1)OFF;
    bis_emfng_excdmfpficlr = (u1)OFF;
#endif

    u1t_xjokmf  = (u1)big_emfng_exjokmf;
    u1t_cjemrev = u1s_emfng_ecjemrev;
#if JEMAT_OBD2 == u1g_EJCC_NOT_USE    /*【OBD2適合無】*/
    u1t_xmfKwp  = (u1)big_wobd2msk_xmfKwp;
#endif
    u2t_LJMFOT_c = u2g_emfng_LJMFOT;
    u2t_LJMFEM_c = u2g_emfng_LJMFEM;
    u2t_LJMFOTF_c = u2s_emfng_LJMFOTF;
#if JEMFDTC_D == u1g_EJCC_USE         /*【失火OBD DTC有】*/
    u1s_emfng_xem_lstpnd_Lch = (u1)big_wmf_fdi_xem_lstpnd;
    u1s_emfng_xot_lstpnd_Lch = (u1)big_wmf_fdi_xot_lstpnd;
    u1s_emfng_xot_warning_Lch = (u1)big_wmf_fdi_xot_warning;
    u1s_emfng_xem_warning_Lch = (u1)big_wmf_fdi_xem_warning;
#endif
    u1s_emfng_state_Lch = u1g_wsentivchg_state;
    u1t_ncyl_c = u1g_ejcc_NCYL;
    u1s_emfng_xreqclr_Lch = (u1)big_wdiaginfclr_xreqclr;

    /********************/
    /* 計算値変数の算出 */
    /********************/
    emfng_mcr_cal();

    /*************************************************/
    /* ecdmfaavTv、ecdmfavTv[](mode$06 TID$0B)の算出 */
    /*************************************************/

    u1t_exrcdmf = u1s_emfng_exrcdmf();

    if ( ( bis_emfng_excntmf == (u1)OFF )   /* ﾄﾘｯﾌﾟ継続中でない */
      && ( u1t_exrcdmf == (u1)OFF ) )       /* 計算履歴がない */
    {
        u4t_cdmfaavTv = (u4)u2g_emfng_ecdmfaavTv();
        u4t_cdmfaavTv = u4g_glmulstngrd_u2u2u2( (u2)u4t_cdmfaavTv, (u2)(((0.9)/(1./256./256.))+0.5), (u2)16U );

        u4t_cdmfaemTv = (u4)u2g_emfng_ecdmfaemTv();
        u4t_cdmfaemTv = u4g_glmulstngrd_u2u2u2( (u2)u4t_cdmfaemTv, (u2)(((0.1)/(1./256./256.))+0.5), (u2)16U );

        /* 前回ecdmfaavTv*0.9 + ecdmfaemTv*0.1 */
        u4t_cdmfaavTv += u4t_cdmfaemTv;
        ELIB_HIGHGD2( u4t_cdmfaavTv, (u4)u2g_U2MAX, u4t_cdmfaavTv );
        u2t_cdmfaavTv = (u2)u4t_cdmfaavTv;

        /* 今回失火回数*0.1＋前回までの平均*0.9(全気筒)【mode$06-TID$0B】*/
        (void)s4g_ememctr_write( u2g_EMFNG_ECDMFAAVTV_U2_ID, (void *)&u2t_cdmfaavTv );                 /* 戻り値は使用しない */
        /* ecdmfaavTv判定下限 */
        u2t_ECDMFAAVMNTL_MI_c = u2g_emfng_ECDMFAAVMNTL_MI;
        (void)s4g_ememctr_write( u2g_EMFNG_ECDMFAAVMNTL_U2_ID, (void *)&u2t_ECDMFAAVMNTL_MI_c );       /* 戻り値は使用しない */
        /* ecdmfaavTv判定上限 */
        u2t_cdmfaavMxtl = (u2)0xffffU;
        (void)s4g_ememctr_write( u2g_EMFNG_ECDMFAAVMXTL_U2_ID, (void *)&u2t_cdmfaavMxtl );             /* 戻り値は使用しない */

#if ( JETCORE_D == u1g_EJCC_USE ) && ( JEOBDUDS_D == u1g_EJCC_NOT_USE )                                /*【t-CORE対応有】AND【OBDonUDS対応無】*/
        u2t_ecdmfaavTv   = u2g_emfng_ecdmfaavTv();
        u2t_ecdmfaavMntl = u2g_emfng_ecdmfaavMntl();
        u2t_ecdmfaavMxtl = u2g_emfng_ecdmfaavMxtl();
        (void)u1g_wtcif_dem_setdtr_u2s4s4s4u1( u2g_WTCIF_DEM_DTR_ECDMFAAV, (s4)u2t_ecdmfaavTv, (s4)u2t_ecdmfaavMntl, (s4)u2t_ecdmfaavMxtl, u1g_WTCIF_DEM_DTR_CTL_NORMAL );  /* 戻り値は使用しない */
#endif
        u2t_ECDMFAVMNTL_MI_c = u2g_emfng_ECDMFAVMNTL_MI;
        for ( s4t_cyl = (s4)0; s4t_cyl < (s4)u1t_ncyl_c; s4t_cyl++ )
        {
            u4t_cdmfavTv = (u4)u2g_emfng_ecdmfavTv( (u1)s4t_cyl );
            u4t_cdmfavTv = u4g_glmulstngrd_u2u2u2( (u2)u4t_cdmfavTv, (u2)(((0.9)/(1./256./256.))+0.5), (u2)16U );

            u4t_cdmfemTv = (u4)u2g_emfng_ecdmfemTv( (u1)s4t_cyl );
            u4t_cdmfemTv = u4g_glmulstngrd_u2u2u2( (u2)u4t_cdmfemTv, (u2)(((0.1)/(1./256./256.))+0.5), (u2)16U );

            /* 前回ecdmfavTv[]*0.9 + ecdmfemTv[]*0.1 */
            u4t_cdmfavTv += u4t_cdmfemTv;
            ELIB_HIGHGD2( u4t_cdmfavTv, (u4)u2g_U2MAX, u4t_cdmfavTv );
            u2t_cdmfavTv = (u2)u4t_cdmfavTv;

            /* 今回失火回数*0.1＋前回までの平均*0.9(気筒別) 【mode$06-TID$0B】 */
            (void)s4g_ememctr_write( u2s_ECDMFAVTV_U2_ID[s4t_cyl], (void *)&u2t_cdmfavTv );            /* 戻り値は使用しない */
            /* ecdmfavTv[]判定下限 */
            (void)s4g_ememctr_write( u2s_ECDMFAVMNTL_U2_ID[s4t_cyl], (void *)&u2t_ECDMFAVMNTL_MI_c );  /* 戻り値は使用しない */
            /* ecdmfavTv[]判定上限 */
            u2t_cdmfavMxtl = (u2)0xffffU;
            (void)s4g_ememctr_write( u2s_ECDMFAVMXTL_U2_ID[s4t_cyl], (void *)&u2t_cdmfavMxtl );        /* 戻り値は使用しない */
#if ( JETCORE_D == u1g_EJCC_USE ) && ( JEOBDUDS_D == u1g_EJCC_NOT_USE )                                /*【t-CORE対応有】AND【OBDonUDS対応無】*/
            u2t_dtrid = u2s_emfng_ecdmfav_tbl[s4t_cyl];
            u2t_ecdmfavTv   = u2g_emfng_ecdmfavTv( (u1)s4t_cyl );
            u2t_ecdmfavMntl = u2g_emfng_ecdmfavMntl( (u1)s4t_cyl );
            u2t_ecdmfavMxtl = u2g_emfng_ecdmfavMxtl( (u1)s4t_cyl );
            (void)u1g_wtcif_dem_setdtr_u2s4s4s4u1( u2t_dtrid, (s4)u2t_ecdmfavTv, (s4)u2t_ecdmfavMntl, (s4)u2t_ecdmfavMxtl, u1g_WTCIF_DEM_DTR_CTL_NORMAL );  /* 戻り値は使用しない */
#endif
        }
        /**************************************/
        /* exrcdmf(mode$06計算履歴ﾌﾗｸﾞ)の操作 */
        /**************************************/
        u1t_exrcdmf = (u1)ON;
        (void)s4g_ememctr_write( u2g_EMFNG_EXRCDMF_U1_ID, (void *)&u1t_exrcdmf );                      /* 戻り値は使用しない */
    }

    /*******************************/
    /* mode$06用 失火ｶｳﾝﾀのｸﾘｱ処理 */
    /*******************************/
    if ( ( bis_emfng_excntmf == (u1)OFF )       /* ﾄﾘｯﾌﾟ継続中でない */
#if JEOBDUDS_D == u1g_EJCC_NOT_USE              /*【OBDonUDS対応無】*/
      && ( u1g_exst_exastnrm == (u1)ON )        /* exastnrm OFF->ON */
#else                                           /*【OBDonUDS対応有】*/
      && ( big_wpsajdg_xpsa == (u1)ON )         /* xpsa OFF->ON */
#endif
       )
    {   /* 始動後、初回条件成立でbis_emfng_excntmf=ONとなる為、u1g_exst_exastnrm/big_wpsajdg_xpsa == ONで判定する */
        /* ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(全気筒)ｸﾘｱ【mode$06-TID$0C】*/
        u2t_ECDMFAEMTV_MI_c = u2g_emfng_ECDMFAEMTV_MI;
        (void)s4g_ememctr_write( u2g_EMFNG_ECDMFAEMTV_U2_ID, (void *)&u2t_ECDMFAEMTV_MI_c );           /* 戻り値は使用しない */

        u2t_ECDMFEMTV_MI_c = u2g_emfng_ECDMFEMTV_MI;
#if ( JETCORE_D == u1g_EJCC_USE ) && ( JEOBDUDS_D == u1g_EJCC_NOT_USE )                            /*【t-CORE対応有】AND【OBDonUDS対応無】*/
        u2t_ecdmfaemTv   = u2g_emfng_ecdmfaemTv();
        u2t_ecdmfaemMntl = u2g_emfng_ecdmfaemMntl();
        u2t_ecdmfaemMxtl = u2g_emfng_ecdmfaemMxtl();
         (void)u1g_wtcif_dem_setdtr_u2s4s4s4u1( u2g_WTCIF_DEM_DTR_ECDMFAEM, (s4)u2t_ecdmfaemTv, (s4)u2t_ecdmfaemMntl, (s4)u2t_ecdmfaemMxtl, u1g_WTCIF_DEM_DTR_CTL_NORMAL );  /* 戻り値は使用しない */
#endif
        for ( s4t_cyl = (s4)0; s4t_cyl < (s4)u1t_ncyl_c; s4t_cyl++ )
        {
            /* ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(気筒別)ｸﾘｱ【mode$06-TID$0C】*/
            (void)s4g_ememctr_write( u2s_ECDMFEMTV_U2_ID[s4t_cyl], (void *)&u2t_ECDMFEMTV_MI_c );      /* 戻り値は使用しない */
#if ( JETCORE_D == u1g_EJCC_USE ) && ( JEOBDUDS_D == u1g_EJCC_NOT_USE )                            /*【t-CORE対応有】AND【OBDonUDS対応無】*/
            u2t_dtrid       = u2s_emfng_ecdmfem_tbl[s4t_cyl];
            u2t_ecdmfemTv   = u2g_emfng_ecdmfemTv( (u1)s4t_cyl );
            u2t_ecdmfemMntl = u2g_emfng_ecdmfemMntl( (u1)s4t_cyl );
            u2t_ecdmfemMxtl = u2g_emfng_ecdmfemMxtl( (u1)s4t_cyl );
            (void)u1g_wtcif_dem_setdtr_u2s4s4s4u1( u2t_dtrid, (s4)u2t_ecdmfemTv, (s4)u2t_ecdmfemMntl, (s4)u2t_ecdmfemMxtl, u1g_WTCIF_DEM_DTR_CTL_NORMAL );  /* 戻り値は使用しない */
#endif
        }

        /* exrcdmf(mode$06計算履歴ﾌﾗｸﾞ)のｸﾘｱ */
        u1t_exrcdmf = (u1)OFF;
        (void)s4g_ememctr_write( u2g_EMFNG_EXRCDMF_U1_ID, (void *)&u1t_exrcdmf );                      /* 戻り値は使用しない */

        bis_emfng_excntmf = (u1)ON;     /* ﾄﾘｯﾌﾟ継続ﾌﾗｸﾞｾｯﾄ */
    }

    /**************************************/
    /* 触媒OTﾚﾍﾞﾙ異常検出仮記憶ﾌﾗｸﾞの操作 */
    /**************************************/
    emfng_xottd_cal();

    /**************************/
    /* 触媒OTﾚﾍﾞﾙの異常の検出 */
    /**************************/
    if ( ( u2g_emfng_ecjmfot >= u2t_LJMFOT_c )      /* lsb=1/3【6気筒】1/2【4気筒】,unit=rev */
      || ( ( u2g_emfng_ecjmfem >= u2t_LJMFEM_c )
        && ( u2g_emfng_ecjmfot >= u2t_LJMFOTF_c ) ) )
    {
        /****************/
        /* 検出条件判定 */
        /****************/
        emfng_ot_detect_ptptptpt( &u1t_xumfot, &u1t_xdmfot, &u1t_xhmfoton, &u1t_xhmfotoff );

#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
        /*******************/
        /* 市場調査RAM更新 */
        /*******************/
        emfng_eocmfmlt_pt( &u1g_emfng_ecdmfw[0] );
#endif

        /*****************************/
        /* mode$06対応失火ｶｳﾝﾀの操作 */
        /*****************************/
        emfng_emode06_cal();

        /**********************************/
        /* 触媒OTﾚﾍﾞﾙ異常検出終了時の処理 */
        /**********************************/
        glint_di(); /* ▼▼▼ 割り込み禁止 ▼▼▼ */

        /* 各種カウンタのクリア処理 */
        u2g_emfng_ecjmfot        = (u2)0U;
        u1g_emfng_ecdmfaotbnk[0] = (u1)0U;   /* CJMFOTと同時にクリアする */
        u1g_emfng_ecdmfaotbnk[1] = (u1)0U;   /* CJMFOTと同時にクリアする */
        u1g_emfng_ecdmfaot       = (u1)0U;   /* CJMFOTと同時にクリアする */
        u1g_emfng_ecdtmf2        = (u1)0U;   /* CJMFOTと同時にクリアする */
        u2g_emfng_ecmfot         = (u2)0U;   /* CJMFOTと同時にクリアする */
        s4s_emfng_emfrtotsum     = (s4)0;    /* CJMFOTと同時にクリアする */
        u2s_emfng_ecmfotcnt      = (u2)0U;   /* CJMFOTと同時にクリアする */
        bis_emfng_excdmfwclr  = (u1)ON;      /* CDMFWはﾀﾞｲｱｸﾞｺｰﾄﾞ処理後にクリアする */
        big_emfng_excdmfemclr = (u1)ON;      /* CDMFEMはﾀﾞｲｱｸﾞｺｰﾄﾞ処理後にクリアする */

        glint_ei(); /* ▲▲▲ 割り込み許可 ▲▲▲ */
    }   /* 触媒OTﾚﾍﾞﾙ異常検出ﾀｲﾐﾝｸﾞ end */

    /***************************/
    /* 失火時のNE,負荷更新通知 */
    /***************************/
    if ( ( u1t_xdmfot == (u1)ON )                  /* 触媒OTレベル異常 */
      && ( big_emfng_excdmfw == (u1)ON ) )         /* 対向気筒失火(【4気筒】時はOFF固定) */
    {
        emfok_mfneld_ann();
    }

    /************************/
    /* EM悪化ﾚﾍﾞﾙ異常の検出 */
    /************************/
#if ( JEMFDTC_D == u1g_EJCC_USE ) && ( JETCORE_D == u1g_EJCC_USE ) && ( JEOBDUDS_D == u1g_EJCC_USE )    /*【失火OBD DTC有】AND【t-CORE対応有】AND【OBDonUDS対応有】*/
    u1s_emfng_pas = (u1)OFF;                    /* ＥＭ悪化レベル判定タイミング不成立時判定ﾌﾗｸﾞOFF */
#endif
    if ( u2g_emfng_ecjmfem >= u2t_LJMFEM_c ) /* lsb=1/3【6気筒】1/2【4気筒】,unit=rev 実行ﾀｲﾐﾝｸﾞ */
    {
#if ( JEMFDTC_D == u1g_EJCC_USE ) && ( JETCORE_D == u1g_EJCC_USE ) && ( JEOBDUDS_D == u1g_EJCC_USE )    /*【失火OBD DTC有】AND【t-CORE対応有】AND【OBDonUDS対応有】*/
        u1s_emfng_pas = (u1)ON;                 /* ＥＭ悪化レベル判定タイミング成立時判定ﾌﾗｸﾞON */
                                                /* emfng_wxmf_out()コール前に実施 */
#endif
#if JEEFI == u1g_EJCC_DUAL   /*【ﾃﾞｭｱﾙINJ】*/
        /**********************************/
        /* 各噴射ﾓｰﾄﾞｸﾘｱ用1000rev判定通知 */
        /**********************************/
        emfok_mfcmodclr();
#endif

        /* 悪路状態カウンタが所定回数以上では「失火カウンタ-悪路カウンタ×n」で異常判定 */
        u2t_cdtmfwa = u2s_emfng_ecdtmfwa;

        s4t_cdmfae2 = (s4)u2g_emfng_ecdmfae2;
        if ( u2t_cdtmfwa >= (u2)u1g_emfng_CDTMFWA )
        {
            s4t_cdmfae2 -= (s4)u4g_glmulstngrd_u2u2u2( u2t_cdtmfwa, (u2)u1s_emfng_KCDTMFWA, (u2)5U );
                                     /* 1*8/256 -> 1(回)  u2*u1のため、s4からのオーバーフロー無し */
            ELIB_LOWGD2( s4t_cdmfae2, (s4)0, s4t_cdmfae2 );            /* 下限ガード */
        }

#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
        /*******************/
        /* 市場調査RAM更新 */
        /*******************/
        emfng_eocmfn_u2( (u2)s4t_cdmfae2 );    /* (u2-u2*u1)のため、u2の上限側ガード不要 */
#endif

        /****************/
        /* 検出条件判定 */
        /****************/
        emfng_em_detect_u2ptpt( (u2)s4t_cdmfae2, &u1t_xdmfem, &u1t_xumfem );

        /**********************************/
        /* EM悪化ﾚﾍﾞﾙ異常検出終了時の処理 */
        /**********************************/
        glint_di(); /* ▼▼▼ 割り込み禁止 ▼▼▼ */

        /* 各種カウンタのクリア処理 */
        u2g_emfng_ecjmfot        = (u2)0U;
        u1g_emfng_ecdmfaotbnk[0] = (u1)0U;      /* CJMFOTと同時にｸﾘｱする */
        u1g_emfng_ecdmfaotbnk[1] = (u1)0U;      /* CJMFOTと同時にｸﾘｱする */
        u1g_emfng_ecdmfaot       = (u1)0U;      /* CJMFOTと同時にｸﾘｱする */
        bis_emfng_excdmfwclr     = (u1)ON;      /* CDMFWはﾀﾞｲｱｸﾞｺｰﾄﾞ処理後にｸﾘｱする */
        u2g_emfng_ecjmfem        = (u2)0U;
        bis_emfng_excdmfae2clr   = (u1)ON;      /* CDMFAE2はﾀﾞｲｱｸﾞｺｰﾄﾞ処理後にｸﾘｱする */
        u1g_emfng_ecdmfaem       = (u1)0U;      /* CJMFEMと同時にｸﾘｱする */
        u2g_emfng_ecmfot         = (u2)0U;      /* CJMFOTと同時にｸﾘｱする */
        u2s_emfng_ecdtmfwa       = (u2)0U;      /* CJMFEMと同時にｸﾘｱする */
        u1g_emfng_ecdtmf2        = (u1)0U;      /* CJMFOTと同時にｸﾘｱする */
        s4s_emfng_emfrtotsum     = (s4)0;       /* CJMFOTと同時にｸﾘｱする */
        u2s_emfng_ecmfotcnt      = (u2)0U;      /* CJMFOTと同時にｸﾘｱする */
        big_emfng_excdmfemclr    = (u1)ON;      /* CDMFEMはﾀﾞｲｱｸﾞｺｰﾄﾞ処理後にｸﾘｱする */
        bis_emfng_excdmfem2clr   = (u1)ON;      /* CDMFEM2はﾀﾞｲｱｸﾞｺｰﾄﾞ処理後にｸﾘｱする */

        glint_ei(); /* ▲▲▲ 割り込み許可 ▲▲▲ */

        u1t_xjokmf  = (u1)ON;            /* EM悪化ﾚﾍﾞﾙ異常判定後に1ﾄﾘｯﾌﾟ判定実行ﾌﾗｸﾞON */
        if ( u1t_cjemrev < u1g_U1MAX )
        {
            u1t_cjemrev++;
        }
#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
        /* 市場調査RAMのｸﾘｱ処理 */
        emfng_espdaemclr();
#endif
    }   /* EM悪化ﾚﾍﾞﾙ判定ﾀｲﾐﾝｸﾞ end */

#if ( JEEFI == u1g_EJCC_DUAL ) && ( JEHIPFI_E == u1g_EJCC_USE )   /*【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/
    /********************/
    /* DI異常の検出判定 */
    /********************/
    if ( u2s_emfng_ecjmfdi >= u2t_LJMFOT_c )
    {
        emfng_di_detect_ptpt( &u1t_xdmfdi, &u1t_xumfdi );
    }

    /*********************/
    /* PFI異常の検出判定 */
    /*********************/
    if ( u2s_emfng_ecjmfpfi >= u2t_LJMFOT_c )
    {
        emfng_pfi_detect_ptpt( &u1t_xdmfpfi, &u1t_xumfpfi );
    }
#endif

    /* 触媒OTﾚﾍﾞﾙ,EM悪化ﾚﾍﾞﾙ,DI,PFIの異常･正常判定保留･MIL点滅要求の結果をRAMへ */
    /* emfng_wxmf_out()コール前に実施 */
    glbitcp_bibi( u1t_xdmfot,    bis_emfng_exdmfot );         /* 触媒OTﾚﾍﾞﾙ異常判定ﾌﾗｸﾞ */
    glbitcp_bibi( u1t_xumfot,    bis_emfng_exumfot );         /* 触媒OTﾚﾍﾞﾙ正常判定保留ﾌﾗｸﾞ */
    glbitcp_bibi( u1t_xhmfoton,  bis_emfng_exhmfoton );       /* 触媒OTによるMIL点滅要求ﾌﾗｸﾞ */
    glbitcp_bibi( u1t_xhmfotoff, bis_emfng_exhmfotoff );      /* 触媒OTによるMIL点滅ｷｬﾝｾﾙ要求ﾌﾗｸﾞ */
    glbitcp_bibi( u1t_xdmfem,    bis_emfng_exdmfem );         /* EM悪化ﾚﾍﾞﾙ異常検出ﾌﾗｸﾞ */
    glbitcp_bibi( u1t_xumfem,    bis_emfng_exumfem );         /* EM悪化ﾚﾍﾞﾙ正常判定保留ﾌﾗｸﾞ */
#if ( JEEFI == u1g_EJCC_DUAL ) && ( JEHIPFI_E == u1g_EJCC_USE )   /*【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/
    glbitcp_bibi( u1t_xdmfdi,    bis_emfng_exdmfdi );         /* DI異常ﾌﾗｸﾞ */
    glbitcp_bibi( u1t_xumfdi,    bis_emfng_exumfdi );         /* DI異常による正常判定保留ﾌﾗｸﾞ */
    glbitcp_bibi( u1t_xdmfpfi,   bis_emfng_exdmfpfi );        /* PFI異常ﾌﾗｸﾞ */
    glbitcp_bibi( u1t_xumfpfi,   bis_emfng_exumfpfi );        /* PFI異常による正常判定保留ﾌﾗｸﾞ */
#endif

#if JEMFDTC_D == u1g_EJCC_USE /*【失火OBD DTC有】*/
    /********************/
    /* 電技への通知処理 */
    /********************/
    emfng_wxmf_out();
#endif

    /**************************/
    /* ダイアグからの消去要求 */
    /**************************/
    u1t_xreqclr = u1s_emfng_wxreqclr();
    if ( u1t_xreqclr == (u1)ON )
    {
        u1t_xjokmf       = (u1)OFF;
        u1t_cjemrev      = (u1)0U;             /* exjokmfと同時にｸﾘｱ */
        bis_emfng_exdmf1 = (u1)OFF;
    }

#if ( JEEFI == u1g_EJCC_DUAL ) && ( JEHIPFI_E == u1g_EJCC_USE )   /*【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/
    if ( bis_emfng_excdmfdiclr == (u1)ON )
    {
        for ( s4t_i = (s4)0; s4t_i < (s4)u1t_ncyl_c; s4t_i++ )
        {
            u1s_emfng_exdmfdim[s4t_i] = (u1)OFF;
        }
    }
    if ( bis_emfng_excdmfpficlr == (u1)ON )
    {
        for ( s4t_i = (s4)0; s4t_i < (s4)u1t_ncyl_c; s4t_i++ )
        {
            u1s_emfng_exdmfpfim[s4t_i] = (u1)OFF;
        }
    }
#endif

    /* Disableﾌﾗｸﾞの操作 */
    u1t_exdisable = (u1)OFF;  /*【OBD2適合有】AND【対米法規】時はOFF固定 */
#if JEMAT_OBD2 == u1g_EJCC_NOT_USE    /*【OBD2適合無】*/
    if ( u1t_xmfKwp == (u1)ON )
    {
        u1t_exdisable = (u1)ON;
    }
#endif
#if JEMFHOUKI == u1g_EJCC_NOT_USAMF  /*【対米法規以外】*/
    if ( s2g_etha_etha < (s2)(((-10.)/(160./256.)) - 0.5) )
    {
        u1t_exdisable = (u1)ON;
    }
#endif
    if ( u1t_xreqclr == (u1)ON )   /* データ消去要求時 */
    {
        u1t_exdisable = (u1)OFF;
    }

    /* Completeﾌﾗｸﾞの操作 */
#if JEMAT_OBD2 == u1g_EJCC_NOT_USE    /*【OBD2適合無】*/
    u1t_excomplete = (u1)OFF;
    if ( u1t_xmfKwp == (u1)OFF )
#endif
    {
        u1t_excomplete = (u1)ON;  /*【OBD2適合有】時はON固定 */
    }

    /* RAMデータに格納 */
    u1s_emfng_ecjemrev = u1t_cjemrev;
    glbitcp_bibi( u1t_xjokmf, big_emfng_exjokmf );
    glbitcp_bibi( u1t_excomplete, big_emfng_excomplete );
    glbitcp_bibi( u1t_exdisable, big_emfng_exdisable );
}

/*********************************************************************/
/*  関数名        | emfng_mcr_cal( )                                 */
/*  処理内容      | 計算値変数の算出                                 */
/*  制御タイミング| pwon,65ms(emfng_pwon(),emfng_65msl()からコール)  */
/*  引数          | なし                                             */
/*  戻り値        | なし                                             */
/*  注意事項      | 内部関数                                         */
/*********************************************************************/
static void
emfng_mcr_cal( void )
{
    u4 u4t_ekldmfw1_mcr;        /* lsb=1,unit=回 :対向気筒失火回数判定下限値(計算値) */
#if JENCYL == u1g_EJCC_6CYL     /*【6気筒】*/
    u4 u4t_ekldmfw2_mcr;        /* lsb=1,unit=回 :対向気筒失火回数判定上限値(計算値) */
#endif
    u4 u4t_ekldmfoth_mcr;       /* lsb=1,unit=回 :触媒OTﾚﾍﾞﾙ失火回数判定値(計算値) */
    u4 u4t_eldmfaem_mcr;        /* lsb=1,unit=回 :EM悪化ﾚﾍﾞﾙ失火回数判定値(計算値) */
    u2 u2t_LJMFOT_c;            /* lsb=2/u1g_EJCC_NCYL,unit=rev :触媒OTレベル異常判定実行タイミング */
    u2 u2t_LJMFEM_c;            /* lsb=2/u1g_EJCC_NCYL,unit=rev :EM悪化ﾚﾍﾞﾙ異常判定実行ﾀｲﾐﾝｸﾞ */
    u2 u2t_KLDMFOT_c;           /* lsb=1,unit=回 :触媒OTﾚﾍﾞﾙ失火回数判定値(100点火/1気筒辺り) */

    /* ワークデータ初期化 */
    u2t_LJMFOT_c = u2g_emfng_LJMFOT;
    u2t_LJMFEM_c = u2g_emfng_LJMFEM;
    u2t_KLDMFOT_c = u1s_emfng_KLDMFOT;

    /* 対向気筒失火回数判定下限値(計算値)の算出 */
    /* ekldmfw1_mcr ← emfng_LJMFOT ＊ ((気筒数／2) ／ (気筒数／2)) ＊ emfng_KLD1 */
    u4t_ekldmfw1_mcr = u4g_glmulstngrd_u2u2u2( u2t_LJMFOT_c, (u2)u1s_emfng_KLD1, (u2)6U );
    u4t_ekldmfw1_mcr = (u4)u2g_gldiv_u4u2( u4t_ekldmfw1_mcr, (u2)u1g_ejcc_NCYL );
                                                /* lsb=(2/NCYL)*(2/256) -> 1,unit=回 */
    ELIB_HIGHGD2( u4t_ekldmfw1_mcr, (u4)u1g_U1MAX, u4t_ekldmfw1_mcr );
    u1s_emfng_ekldmfw1_mcr = (u1)u4t_ekldmfw1_mcr;

#if JENCYL == u1g_EJCC_6CYL     /*【6気筒】*/
    /* 対向気筒失火回数判定上限値(計算値)の算出 */
    /* ekldmfw2_mcr ← emfng_LJMFOT ＊ ((気筒数／2) ／ (気筒数／2)) ＊ emfng_KLD2 */
    u4t_ekldmfw2_mcr = u4g_glmulstngrd_u2u2u2( u2t_LJMFOT_c, (u2)u1s_emfng_KLD2, (u2)6U );
    u4t_ekldmfw2_mcr = (u4)u2g_gldiv_u4u2( u4t_ekldmfw2_mcr, (u2)u1g_ejcc_NCYL );
                                                /* lsb=(2/NCYL)*2/256 -> 1,unit=回 */
    ELIB_HIGHGD2( u4t_ekldmfw2_mcr, (u4)u1g_U1MAX, u4t_ekldmfw2_mcr );
    u1s_emfng_ekldmfw2_mcr = (u1)u4t_ekldmfw2_mcr;
#endif

    /* 触媒OTﾚﾍﾞﾙ失火回数判定値(計算値)の算出 */
    u4t_ekldmfoth_mcr = u4g_glmul_u2u2( (u2)u2t_KLDMFOT_c, (u2)u1g_ejcc_NCYL );
                                                /* lsb=1 * 1 -> 1,unit=回 */
    u4t_ekldmfoth_mcr = (u4)u2g_gldiv_u4u2( u4t_ekldmfoth_mcr, (u2)u1g_emfng_CATN );
                                                /* lsb=1 / 1 -> 1,unit=回 */
    u2g_emfng_ekldmfoth_mcr = (u2)u4t_ekldmfoth_mcr;

    /* EM悪化ﾚﾍﾞﾙ失火回数判定値(計算値)の算出 */
    /* eldmfaem_mcr ← emfng_LJMFEM ＊ (気筒数／2) ＊ emfng_MFDD ／ 100 ＊ emfng_MFD1 */
    u4t_eldmfaem_mcr = u4g_glmul_u2u2( u2t_LJMFEM_c, (u2)s2s_emfng_MFDD );  /* emfng_MFDD(失火率)は必ず正のためu2キャストしてOK */
                                                /* lsb=(2/NCYL)*(256/128/256) -> 256/128/256 */
                                                /* emfng_LJMFEMのLSB変換と(気筒数/2)の演算結果が1になるため処理省略 */
    u4t_eldmfaem_mcr = u4g_glmulst_u4u4u2( u4t_eldmfaem_mcr, (u4)u2s_emfng_MFD1, (u2)22U );
                                                /* lsb=(256/128/256)*(2/256/256) -> 1 */
    u4t_eldmfaem_mcr = (u4)u2g_gldiv_u4u2( u4t_eldmfaem_mcr, (u2)100U );
    u2g_emfng_eldmfaem_mcr = (u2)u4t_eldmfaem_mcr;

    /* ｺｰﾙﾄﾞｽﾀｰﾄ時EM悪化ﾚﾍﾞﾙ失火回数判定値(計算値)の算出 */
    u2s_emfng_eldmfst1_mcr = u2g_glmulst_u2u2u2( (u2)u4t_eldmfaem_mcr, (u2)u1s_emfng_ST1RTO, (u2)5U );
                                                /* lsb=1 * (8/256) -> 1,unit=回 */

    /* ﾎｯﾄｽﾀｰﾄ時EM悪化ﾚﾍﾞﾙ失火回数判定値(計算値)の算出 */
    u2s_emfng_eldmfst2_mcr = u2g_glmulst_u2u2u2( (u2)u4t_eldmfaem_mcr, (u2)u1s_emfng_ST2RTO, (u2)5U );
                                                /* lsb=1 * (8/256) -> 1,unit=回 */

    /* DTC出力気筒判定値(計算値)の算出 */
    u2s_emfng_emfk_mcr = u2g_glmulst_u2u2u2( (u2)u4t_eldmfaem_mcr, (u2)u1s_emfng_KMFK, (u2)7U );
                                                /* lsb=1 * (2/256) -> 1,unit=回 */

#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
    /* 失火回数小、中、大頻度判定回数の算出 */
    u2s_emfng_ecmfnl_mcr = u2g_glmulst_u2u2u2( (u2)u4t_eldmfaem_mcr, (u2)u1s_emfng_MFLOW, (u2)7U );
    u2s_emfng_ecmfnm_mcr = u2g_glmulst_u2u2u2( (u2)u4t_eldmfaem_mcr, (u2)u1s_emfng_MFMID, (u2)7U );
    u2s_emfng_ecmfnh_mcr = u2g_glmulst_u2u2u2( (u2)u4t_eldmfaem_mcr, (u2)u1s_emfng_MFHI,  (u2)7U );
                                                /* lsb=1 * (2/256) -> 1,unit=回 */
#endif

}

/*********************************************************************/
/*  関数名        | emfng_xottd_cal( )                               */
/*  処理内容      | ｱｲﾄﾞﾙ時対向気筒失火専用                          */
/*                | 触媒OTﾚﾍﾞﾙ異常検出仮記憶ﾌﾗｸﾞの操作               */
/*  制御タイミング| 65ms(emfng_65msl()からコール)                    */
/*  引数          | なし                                             */
/*  戻り値        | なし                                             */
/*  注意事項      | 内部関数                                         */
/*********************************************************************/
static void
emfng_xottd_cal( void )
{
    s4 s4t_i;           /* lsb=1 :forループ変数 */
    u1 u1t_ncyl_c;      /* lsb=1 :気筒数 */
#if JENCYL == u1g_EJCC_6CYL    /*【6気筒】*/
    s4 s4t_cnt;         /* lsb=1 :失火気筒数ｶｳﾝﾀ */
    u1 u1t_ncylw_c;     /* lsb=1 :気筒群数 */
#endif
    s4 s4t_mfci;        /* lsb=1 :ecdmfemi[] >= emfng_MFSTEMI成立回数ｶｳﾝﾀ */
    s4 s4t_mfrtot;      /* lsb=256/128/256,unit=% :emfrtotsum/ecmfotcnt */
    s4 s4t_cdmfaot;     /* lsb=1,unit=回 :触媒OTレベル全気筒失火カウンタ */
    s4 s4t_cdmfaotbnk[2];  /* lsb=1,unit=回 :バンク別触媒OTレベル失火カウンタ */
#if JENCYL == u1g_EJCC_6CYL    /*【6気筒】*/
    s4 s4t_cdmfw;       /* lsb=1,unit=回 :対向気筒失火ｶｳﾝﾀ */
    u1 u1t_excdmfwtmp;  /* lsb=1 :いずれかの気筒群別対向失火ｶｳﾝﾀにより触媒OT判定 */
    u1 u1t_ekldmfw1_mcr; /* lsb=1,unit=回 :対向気筒失火回数判定下限値(計算値) */
#endif
    u1 u1t_xcdmfaot;    /* lsb=1 :触媒OT失火ｶｳﾝﾀによる触媒OT判定成立ﾌﾗｸﾞ */
    u1 u1t_excdmfw;     /* lsb=1 :対向気筒失火判定成立ﾌﾗｸﾞ */
    u1 u1t_exottd;      /* lsb=1 :触媒OTﾚﾍﾞﾙ異常検出仮記憶ﾌﾗｸﾞ */
    u1 u1t_xdtmf2;      /* lsb=1 :触媒OT判定用悪路ｶｳﾝﾀ値判定 */
    s4 s4t_cdtmf2;      /* lsb=1,unit=回 :触媒OTﾚﾍﾞﾙ判定用悪路ｶｳﾝﾀ */
    s4 s4t_kcdmfaotb;   /* :ecdtmf2 ＊ emfng_KCDTMFAOTB */
    s4 s4t_cdmfaotb;    /* lsb=1,unit=回 :ecdmfaot-ecdtmf2のラッチ(MODE06用) */
    s4 s4t_emfrtotsum;  /* lsb=256/128/256,unit=% :積算失火判定値 */
    u2 u2t_ecmfotcnt;   /* lsb=1,unit=回 :平均算出用積算回数 */
    s4 s4t_ekldmfot;    /* lsb=1,unit=回 :触媒OTﾚﾍﾞﾙ失火回数判定値 */
#if JENCYL == u1g_EJCC_6CYL    /*【6気筒】*/
    u1 u1t_ecdmfw[u1g_EJCC_NCYL/2]; /* lsb=1,unit=回 :対向気筒失火ｶｳﾝﾀ */
#endif
    u2 u2t_ecjmfot;     /* lsb=1/3【6気筒】1/2【4気筒】,unit=rev :触媒OTﾚﾍﾞﾙ失火検出許可ｶｳﾝﾀ */
    u1 u1t_MFSTEMI_c;   /* lsb=1,unit=回 :アイドル中気筒別失火回数判定値 */
    u2 u2t_LJMFOT_c;    /* lsb=2/u1g_EJCC_NCYL,unit=rev :触媒OTレベル異常判定実行タイミング */
    u2 u2t_LJMFEM_c;    /* lsb=2/u1g_EJCC_NCYL,unit=rev :EM悪化レベル異常判定実行タイミング */
    u2 u2t_LJMFOTF_c;   /* lsb=2/u1g_EJCC_NCYL,unit=rev :触媒OTレベル異常判定実行タイミング */

    /* ワークデータ初期化 */
    u1t_exottd = (u1)bis_emfng_exottd;
#if JENCYL == u1g_EJCC_6CYL    /*【6気筒】*/
    u1t_ekldmfw1_mcr = u1s_emfng_ekldmfw1_mcr;
#endif
    u2t_ecjmfot   = u2g_emfng_ecjmfot;
    u2t_LJMFOT_c = u2g_emfng_LJMFOT;
    u2t_LJMFEM_c = u2g_emfng_LJMFEM;
    u2t_LJMFOTF_c = u2s_emfng_LJMFOTF;
    u1t_ncyl_c = u1g_ejcc_NCYL;

    if ( u2g_emfng_ecjmfoti >= u2s_emfng_LJMFOTI )  /* lsb=1/3【6気筒】1/2【4気筒】,unit=rev */
    {
        /**************************************/
        /* 触媒OTﾚﾍﾞﾙ異常検出仮記憶ﾌﾗｸﾞの操作 */
        /**************************************/
        u1t_MFSTEMI_c = u1s_emfng_MFSTEMI;
        s4t_mfci = (s4)0;
        for ( s4t_i = (s4)0; s4t_i < (s4)u1t_ncyl_c; s4t_i++ )
        {
            if ( u1g_emfng_ecdmfemi[s4t_i] >= u1t_MFSTEMI_c )
            {
                s4t_mfci++;
            }
        }

        u1t_exottd = (u1)OFF;
        if ( ( u2g_emfng_ecdmfaoti <= (u2)u1s_EMFNG_MFSTOTI )
          && ( s4t_mfci == (s4)2 ) )
        {
            u1t_exottd = (u1)ON;
        }
        emfng_cdmfi_reset(); /* 触媒OTﾚﾍﾞﾙｱｲﾄﾞﾙ時失火検出許可ｶｳﾝﾀのｸﾘｱ */
    }

    /* 触媒OTﾚﾍﾞﾙ異常検出ﾀｲﾐﾝｸﾞ */
    if ( ( u2t_ecjmfot >= u2t_LJMFOT_c )      /* lsb=1/3【6気筒】1/2【4気筒】,unit=rev */
      || ( ( u2g_emfng_ecjmfem >= u2t_LJMFEM_c )
        && ( u2t_ecjmfot >= u2t_LJMFOTF_c ) ) )
    {
        s4t_cdtmf2 = (s4)u1g_emfng_ecdtmf2;  /* 事前準備 */
        u1t_xdtmf2 = (u1)OFF;
        if ( s4t_cdtmf2 >= (s4)u1s_emfng_CDTMF2 )
        {
            u1t_xdtmf2 = (u1)ON;
        }

        /*********************************/
        /* 失火回数CDMFAOTによる条件判定 */
        /*********************************/
        s4t_cdmfaot       = (s4)u1g_emfng_ecdmfaot;           /* 参照用 */
        s4t_cdmfaotbnk[0] = (s4)u1g_emfng_ecdmfaotbnk[0];     /* 参照用 */
        s4t_cdmfaotbnk[1] = (s4)u1g_emfng_ecdmfaotbnk[1];     /* 参照用 */

        s4t_cdmfaotb = s4t_cdmfaot;

        if ( u1t_xdtmf2 == (u1)ON )
        {
            s4t_kcdmfaotb = s4g_glmulstngrd_s2s2u2( (s2)s4t_cdtmf2, (s2)u1s_emfng_KCDTMFAOTB, (u2)5U );          /* lsb=1*8/256 -> 1(回) */

            s4t_cdmfaot -= s4t_kcdmfaotb;
            s4t_cdmfaotb = s4t_cdmfaot;            /* MODE06算出用に下限ガード前にラッチ */

            s4t_cdmfaotbnk[0] -= s4t_kcdmfaotb;
            s4t_cdmfaotbnk[1] -= s4t_kcdmfaotb;
            if ( s4t_cdmfaot < (s4)0 )        /* 下限ガード */
            {
                s4t_cdmfaot = (s4)0;
            }
            if ( s4t_cdmfaotbnk[0] < (s4)0 )  /* 下限ガード */
            {
                s4t_cdmfaotbnk[0] = (s4)0;
            }
            if ( s4t_cdmfaotbnk[1] < (s4)0 )  /* 下限ガード */
            {
                s4t_cdmfaotbnk[1] = (s4)0;
            }
        }
        s2s_emfng_ecdmfaotb = (s2)s4t_cdmfaotb;  /* (u1 - u1 * KCDTMFAOTB)のためガード処理不要 */

        glint_di(); /* ▼▼▼ 割り込み禁止 ▼▼▼ */
        s4t_emfrtotsum = s4s_emfng_emfrtotsum;
        u2t_ecmfotcnt  = u2s_emfng_ecmfotcnt;
        glint_ei(); /* ▲▲▲ 割り込み許可 ▲▲▲ */

        s4t_mfrtot = s4g_gldiv_s4s4( s4t_emfrtotsum, (s4)u2t_ecmfotcnt );
                                                        /* lsb=(256/128/256)/1 -> 256/128/256,unit=% */
        ELIB_LOWGD2( s4t_mfrtot, (s4)s2g_emfng_MFOTL, s4t_mfrtot );
        s4t_ekldmfot = s4g_glmulst_s4s4u2( (s4)u2g_emfng_ekldmfoth_mcr, s4t_mfrtot, (u2)0U );
                                                        /* lsb=1*(256/128/256) -> 256/128/256,unit=% */
        s4t_ekldmfot = s4g_gldiv_s4s4( s4t_ekldmfot, (s4)(((100.)/(256./128./256.))+0.5) );
                                                        /* lsb=(256/128/256)/(256/128/256) -> 1,unit=回 */
        ELIB_HILOGD2( s4t_ekldmfot, (s4)u2g_U2MAX, (s4)u2g_U2MIN, s4t_ekldmfot );
        u2g_emfng_ekldmfot = (u2)s4t_ekldmfot;

        u1t_xcdmfaot = (u1)OFF;
        if ( u1g_emfng_CATN == (u1)1U )         /* バンク区別なし */
        {
            if ( s4t_cdmfaot >= s4t_ekldmfot )
            {
                u1t_xcdmfaot = (u1)ON;
            }
        }
        else                                    /* バンク区別あり */
        {
            if ( ( s4t_cdmfaotbnk[0] >= s4t_ekldmfot )
              || ( s4t_cdmfaotbnk[1] >= s4t_ekldmfot ) )
            {
                u1t_xcdmfaot = (u1)ON;
            }
        }

        /* RAMデータに格納 */
        glbitcp_bibi( u1t_xcdmfaot, bis_emfng_excdmfaot );

        /**********************************/
        /* 対向気筒失火判定成立ﾌﾗｸﾞの操作 */
        /**********************************/
        u1t_excdmfw = (u1)OFF;                  /*【4気筒】はOFF固定(対向気筒失火検出なし) */
#if JENCYL == u1g_EJCC_6CYL    /*【6気筒】*/
        u1t_excdmfwtmp = (u1)OFF;
        u1t_ncylw_c = u1s_EMFNG_NCYLW;
        for ( s4t_i = (s4)0; s4t_i < (s4)u1t_ncylw_c; s4t_i++ )
        {
            u1t_ecdmfw[s4t_i] = u1g_emfng_ecdmfw[s4t_i];
            if ( u1t_ecdmfw[s4t_i] > u1t_ekldmfw1_mcr )
            {
                u1t_excdmfwtmp = (u1)ON;
            }
        }
        s4t_cdmfw = (s4)u1t_ecdmfw[0];               /* [0]を代入 */
        for ( s4t_i = (s4)1; s4t_i < (s4)u1t_ncylw_c; s4t_i++ )
        {
            s4t_cdmfw += (s4)u1t_ecdmfw[s4t_i];      /* [1]～加算 */
        }
        if ( ( ( s4t_cdmfw <= (s4)u1s_emfng_ekldmfw2_mcr )
            && ( s4t_cdmfw >  (s4)u1t_ekldmfw1_mcr ) )
          && ( u1t_excdmfwtmp == (u1)ON ) )
        {
            u1t_excdmfw = (u1)ON;                /* CXMF1ｲﾝｸﾘﾒﾝﾄ条件 */
        }
#endif
        /* RAMデータに格納 */
        glbitcp_bibi( u1t_excdmfw, big_emfng_excdmfw );

        /**************************************/
        /* 触媒OTﾚﾍﾞﾙ異常検出仮記憶ﾌﾗｸﾞの操作 */
        /**************************************/
#if JENCYL == u1g_EJCC_6CYL    /*【6気筒】*/
        s4t_cnt = (s4)0;
        for ( s4t_i = (s4)0; s4t_i < (s4)u1t_ncyl_c; s4t_i++ )
        {
            if ( u1g_emfng_ecdmfem[s4t_i] >= u1s_emfng_MFSTOT )
            {
                s4t_cnt++;                       /* 失火気筒数のカウント */
            }
        }

        /* セット条件判定 a */
        if ( ( u1t_xcdmfaot == (u1)ON )
          && ( s4t_cdmfaot <= (s4)204 )
          && ( s4t_cnt == (s4)2 )
          && ( bis_emfng_exmfkpl == (u1)ON ) )
        {
            u1t_exottd = (u1)ON;
        }
#endif

        /* セット条件判定 b */
        if ( ( u1t_excdmfw == (u1)ON )           /*【4気筒】はOFF固定 */
          && ( bis_emfng_exmfkpm == (u1)ON ) )
        {
            u1t_exottd = (u1)ON;
        }
    }   /* 触媒OTﾚﾍﾞﾙ異常検出ﾀｲﾐﾝｸﾞ end */

    /* RAMデータに格納 */
    glbitcp_bibi( u1t_exottd, bis_emfng_exottd );
}

/*********************************************************************/
/*  関数名        | emfng_cdmfi_reset( )                             */
/*  処理内容      | 失火回数判定ｶｳﾝﾀ(ｱｲﾄﾞﾙ用)のｸﾘｱ処理               */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | なし                                             */
/*  戻り値        | なし                                             */
/*  注意事項      | 内部関数                                         */
/*                | 割込み干渉の考慮必要                             */
/*********************************************************************/
static void
emfng_cdmfi_reset( void )
{
    s4 s4t_i;
    u1 u1t_ncyl_c;                   /* lsb=1 :気筒数 */

    /* ワークデータ初期化 */
    u1t_ncyl_c = u1g_ejcc_NCYL;

    glint_di(); /* ▼▼▼ 割り込み禁止 ▼▼▼ */
    u2g_emfng_ecjmfoti  = (u2)0U;
    u2g_emfng_ecdmfaoti = (u2)0U;
    for ( s4t_i = (s4)0; s4t_i < (s4)u1t_ncyl_c; s4t_i++ )
    {
        u1g_emfng_ecdmfemi[s4t_i] = (u1)0U;
    }
    glint_ei(); /* ▲▲▲ 割り込み許可 ▲▲▲ */
}

/*********************************************************************/
/*  関数名        | emfng_ot_detect_ptptptpt( )                      */
/*  処理内容      | 触媒OTレベル異常判定                             */
/*  制御タイミング| 65ms毎(emfng_65msl()よりコール)                  */
/*  引数          | ptt_xumfot : 触媒OTﾚﾍﾞﾙの正常判定保留ﾌﾗｸﾞの格納先 */
/*                | ptt_xdmfot : 触媒OTﾚﾍﾞﾙ異常判定ﾌﾗｸﾞの格納先      */
/*                | ptt_xhmfoton  : 触媒OTによるMIL点滅要求ﾌﾗｸﾞの格納先 */
/*                | ptt_xhmfotoff : 触媒OTによるMIL点滅ｷｬﾝｾﾙ要求ﾌﾗｸﾞの格納先 */
/*  戻り値        | なし(引数で受け渡し)                             */
/*  注意事項      | 内部関数                                         */
/*********************************************************************/
static void
emfng_ot_detect_ptptptpt( u1 *ptt_xumfot, u1 *ptt_xdmfot, u1 *ptt_xhmfoton, u1 *ptt_xhmfotoff )
{
    s4 s4t_ecxmf1;      /* lsb=1,unit=回:触媒OTﾚﾍﾞﾙ異常検出回数 */
    s4 s4t_exmfotfc1;   /* lsb=1:emfng_exmfotfc_u1()コール条件１判定 */
#if JEMFFC == u1g_EJCC_USE  /*【触媒OT失火異常時FC有】*/
    s4 s4t_exmfotfc2;   /* lsb=1:emfng_exmfotfc_u1()コール条件２判定 */
#endif
    s4 s4t_xecxmf1inc;  /* lsb=1:ecxmfc1ｶｳﾝﾄｱｯﾌﾟ条件成立ﾌﾗｸﾞ */
    s4 s4t_xjdge;       /* lsb=1:触媒OTﾚﾍﾞﾙ異常条件２成立ﾌﾗｸﾞ */
    u1 u1t_xdmfot;      /* lsb=1:触媒OTﾚﾍﾞﾙ異常成立ﾌﾗｸﾞ */
    u1 u1t_xumfot;      /* lsb=1:触媒OTﾚﾍﾞﾙ正常判定保留ﾌﾗｸﾞ */
    u1 u1t_xhmfoton;    /* lsb=1:MIL点滅要求ﾌﾗｸﾞ */
    u1 u1t_xhmfotoff;   /* lsb=1:MIL点滅ｷｬﾝｾﾙﾌﾗｸﾞ */
    u1 u1t_exottd;      /* lsb=1:触媒OTﾚﾍﾞﾙ異常検出仮記憶ﾌﾗｸﾞ */
    u1 u1t_CXMF1L_c;    /* lsb=1,unit=回 :触媒OTレベル異常検出回数判定値 */
    u1 u1t_exinftp;     /* lsb=1:モード走行判定フラグ */
    u1 u1t_state;       /* lsb=1 :検出感度状態 */
#if JETCORE_D == u1g_EJCC_USE  /*【t-CORE対応有】*/
    u1 u1t_xoutftp;     /* lsb=1:OT失火モード領域外判定ﾌﾗｸﾞ */
#endif
#if JEMFDTC_D == u1g_EJCC_USE  /*【失火OBD DTC有】*/
    u1 u1t_xem_lstpnd;  /* lsb=1 :前ﾄﾞﾗｲﾋﾞﾝｸﾞｻｲｸﾙ仮異常状態(EM)(ラッチ用) */
    u1 u1t_xot_lstpnd;  /* lsb=1 :前ﾄﾞﾗｲﾋﾞﾝｸﾞｻｲｸﾙ仮異常状態(OT)(ラッチ用) */
#endif

    /* ワークデータ初期化 */
    s4t_ecxmf1     = (s4)u1s_emfng_ecxmf1;
    u1t_xdmfot     = (u1)OFF;
    s4t_xecxmf1inc = (s4)OFF;
    s4t_xjdge      = (s4)OFF;
    s4t_exmfotfc1  = (s4)OFF;
#if JEMFFC == u1g_EJCC_USE  /*【触媒OT失火異常時FC有】*/
    s4t_exmfotfc2  = (s4)OFF;
#endif
    u1t_xumfot     = (u1)OFF;
    u1t_xhmfoton   = (u1)OFF;
    u1t_xhmfotoff  = (u1)OFF;
    u1t_exottd     = (u1)bis_emfng_exottd;
    u1t_CXMF1L_c = u1s_emfng_CXMF1L;
    u1t_exinftp    = (u1)big_emfinftp_exinftp;
    u1t_state = u1s_emfng_state_Lch;
#if JETCORE_D == u1g_EJCC_USE  /*【t-CORE対応有】*/
    u1t_xoutftp    = (u1)OFF;
#endif
#if JEMFDTC_D == u1g_EJCC_USE  /*【失火OBD DTC有】*/
    u1t_xem_lstpnd = u1s_emfng_xem_lstpnd_Lch;
    u1t_xot_lstpnd = u1s_emfng_xot_lstpnd_Lch;
#endif

    /********************************/
    /* 触媒OTﾚﾍﾞﾙ異常検出回数の算出 */
    /********************************/
    if ( ( bis_emfng_excdmfaot == (u1)ON )
#if JENCYL == u1g_EJCC_6CYL /*【6気筒】*/
      || ( big_emfng_excdmfw    == (u1)ON )
#endif
       )
    {
        s4t_exmfotfc1 = (s4)ON;
    }

    if ( ( s4t_exmfotfc1 == (s4)ON )
      || ( u1t_exottd  == (u1)ON ) )
    {
        s4t_xecxmf1inc = (s4)ON;
        u1t_xumfot = (u1)ON;                    /* 触媒OTﾚﾍﾞﾙ正常判定保留 */
        if ( s4t_ecxmf1 < (s4)u1g_U1MAX )
        {
            s4t_ecxmf1++;
        }
    }

    /************************/
    /* 触媒OTﾚﾍﾞﾙ異常の判定 */
    /************************/
    if ( ( s4t_ecxmf1   >= (s4)u1s_emfng_CXMF1H )
      || ( ( s4t_ecxmf1 >= (s4)u1t_CXMF1L_c )
        && ( ( u1t_exinftp == (u1)OFF )
          || ( u1t_state == u1g_WSENTIVCHG_LVL2 )
#if JEMFDTC_D == u1g_EJCC_USE  /*【失火OBD DTC有】*/
          || ( u1t_xem_lstpnd == (u1)ON )
          || ( u1t_xot_lstpnd == (u1)ON )
#endif
           ) ) )
    {
#if JEMFFC == u1g_EJCC_USE  /*【触媒OT失火異常時FC有】*/
        s4t_exmfotfc2 = (s4)ON;
#endif
        s4t_xjdge = (s4)ON;
    }

#if ( JENCYL == u1g_EJCC_4CYL ) && ( JEMFHOUKI == u1g_EJCC_NOT_USAMF )  /*【4気筒】AND【対米法規以外】 */
    if ( ( s4t_ecxmf1 >= (s4)u1t_CXMF1L_c )
      && ( u1t_exottd  == (u1)ON ) )
    {
        s4t_xjdge = (s4)ON;
    }
#endif

    /* 2-1.(2)①(c)ｲ)ⅱ)条件変更時は、下記条件式も見直す事 */
#if JETCORE_D == u1g_EJCC_USE      /*【t-CORE対応有】*/
    if ( ( u1t_exinftp == (u1)OFF )
      || ( u1t_state == u1g_WSENTIVCHG_LVL2 )
 #if JEMFDTC_D == u1g_EJCC_USE     /*【失火OBD DTC有】*/
      || ( u1t_xem_lstpnd == (u1)ON )
      || ( u1t_xot_lstpnd == (u1)ON )
 #endif
 #if ( JENCYL == u1g_EJCC_4CYL ) && ( JEMFHOUKI == u1g_EJCC_NOT_USAMF ) /*【4気筒】AND【対米法規以外】*/
      || ( u1t_exottd  == (u1)ON )
 #endif
       )
    {
        u1t_xoutftp = (u1)ON;        /* OT失火モード領域外と判定 */
    }
    /* RAMデータに格納 */
    glbitcp_bibi( u1t_xoutftp, bis_emfng_exoutftp );         /* OT失火モード領域外判定ﾌﾗｸﾞ(65msl) */
#endif

    if ( s4t_xecxmf1inc == (s4)ON )  /* ecxmf1ｶｳﾝﾄｱｯﾌﾟ条件成立 */
    {
        if ( s4t_xjdge == (s4)ON )
        {
            u1t_xdmfot       = (u1)ON;                /* 触媒OTﾚﾍﾞﾙ現在異常 */
            bis_emfng_exdmf1 = (u1)ON;
        }
        if ( ( bis_emfng_exdmf1 == (u1)ON )
#if JEMFDTC_D == u1g_EJCC_USE /*【失火OBD DTC有】*/
          || ( ( u1s_emfng_xot_warning_Lch == (u1)ON )
            || ( u1s_emfng_xem_warning_Lch == (u1)ON ) )
#endif
#if JEMFFC == u1g_EJCC_USE  /*【触媒OT失火異常時FC有】*/
          || ( u1g_emffc_exfcbi != (u1)0x00U )  /* 触媒OT失火防止FC要求有り */
#endif
           )
        {
            u1t_xhmfoton = (u1)ON;              /* MIL点滅 */
        }
        else
        {
            u1t_xhmfotoff = (u1)ON;             /* MIL点滅ｷｬﾝｾﾙ */
        }
    }
    else
    {
        if ( ( u1g_emfng_ecdtmf2 < u1s_emfng_CDTMF2 )
#if JEMFFC == u1g_EJCC_USE  /*【触媒OT失火異常時FC有】*/
          && ( u1g_emffc_exfcbi == (u1)0x00U )  /* 触媒OT失火防止FC要求無し */
#endif
           )
        {
            u1t_xhmfotoff = (u1)ON;             /* MIL点滅ｷｬﾝｾﾙ */
        }
    }

    /* RAMデータに格納 */
    u1s_emfng_ecxmf1 = (u1)s4t_ecxmf1;

#if JEMFFC == u1g_EJCC_USE          /*【触媒OT失火異常時FC有】*/
    /* emfng_exmfotfc()のコール */
    if ( ( s4t_exmfotfc1 == (s4)ON )
      && ( s4t_exmfotfc2 == (s4)ON ) )
    {
        emfng_exmfotfc_u1( (u1)ON );
    }
#endif

    /* 戻り値の設定 */
    *ptt_xumfot    = u1t_xumfot;
    *ptt_xdmfot    = u1t_xdmfot;
    *ptt_xhmfoton  = u1t_xhmfoton;
    *ptt_xhmfotoff = u1t_xhmfotoff;
}

/***********************************************************************/
/*  関数名        | emfng_em_detect_u2ptpt( )                          */
/*  処理内容      | EM悪化レベル異常判定                               */
/*  制御タイミング| 65ms毎(emfng_65msl()よりコール)                    */
/*  引数          | u2t_cdmfae2  : EM悪化レベル異常判定値              */
/*                | ptt_xdmfem   : EM悪化ﾚﾍﾞﾙ異常判定ﾌﾗｸﾞの格納先      */
/*                | ptt_xumfem   : EM悪化ﾚﾍﾞﾙ正常判定保留ﾌﾗｸﾞの格納先  */
/*  戻り値        | なし(引数で受け渡し)                               */
/*  注意事項      | 内部関数                                           */
/***********************************************************************/
static void
emfng_em_detect_u2ptpt( u2 u2t_cdmfae2, u1 *ptt_xdmfem, u1 *ptt_xumfem )
{
    s4 s4t_ecxmf;       /* lsb=1,unit=回 :EM悪化ﾚﾍﾞﾙ異常検出回数 */
    u1 u1t_xdmfem;      /* lsb=1 :EM悪化ﾚﾍﾞﾙ異常判定ﾌﾗｸﾞ */
    u2 u2t_kldmfae;     /* lsb=1,unit=回 :EM悪化ﾚﾍﾞﾙ失火判定値 */
    u1 u1t_xumfem;      /* lsb=1 :EM悪化ﾚﾍﾞﾙ正常判定保留ﾌﾗｸﾞ */
    u1 u1t_state;       /* lsb=1 :検出感度状態 */
    u1 u1t_CXMFL_c;     /* lsb=1,unit=回 :EM悪化レベル異常検出回数判定値 */
#if JETCORE_D == u1g_EJCC_USE                      /*【t-CORE対応有】*/
    u1 u1t_xemfst;      /* lsb=1 :EM悪化レベル初回1000rev判定ﾌﾗｸﾞ */
#endif
#if JEMFHOUKI == u1g_EJCC_USAMF                    /*【対米法規】*/
    u2 u2t_ecdmfae2;    /* lsb=1,unit=回 :EM悪化ﾚﾍﾞﾙ全気筒失火ｶｳﾝﾀ */
    u1 u1t_exjokmf;     /* lsb=1 :始動後1000rev完了ﾌﾗｸﾞ */
#endif

    /* ワークデータ初期化 */
    s4t_ecxmf   = (s4)u1s_emfng_ecxmf;
    u1t_xdmfem  = *ptt_xdmfem;
    u1t_xumfem  = *ptt_xumfem;
    u2t_kldmfae = u2g_emfng_ekldmfae;
    u1t_state   = u1s_emfng_state_Lch;
    u1t_CXMFL_c = u1s_emfng_CXMFL;
#if JETCORE_D == u1g_EJCC_USE                      /*【t-CORE対応有】*/
    u1t_xemfst  = (u1)OFF;
#endif
#if JEMFHOUKI == u1g_EJCC_USAMF                    /*【対米法規】*/
    u2t_ecdmfae2 = u2g_emfng_ecdmfae2;
    u1t_exjokmf  = (u1)big_emfng_exjokmf;
#endif

    /********************************/
    /* EM悪化ﾚﾍﾞﾙ異常検出回数の算出 */
    /********************************/
    if ( u2t_cdmfae2 >= u2t_kldmfae )                      /* lsb=1,unit=回 */
    {
        u1t_xumfem = (u1)ON;                       /* EM悪化ﾚﾍﾞﾙ正常判定保留 */
        if ( s4t_ecxmf < (s4)u1g_U1MAX )
        {
            s4t_ecxmf++;
        }

        /************************/
        /* EM悪化ﾚﾍﾞﾙ異常の判定 */
        /************************/
        if ( ( s4t_ecxmf >= (s4)u1s_emfng_CXMFH )
          || ( ( s4t_ecxmf >= (s4)u1t_CXMFL_c )
            && ( ( u1t_state == u1g_WSENTIVCHG_LVL2 )
              || ( u1t_state == u1g_WSENTIVCHG_IUE ) ) )     /* 感度アップ（評価用） */
#if JEMFHOUKI == u1g_EJCC_USAMF                     /*【対米法規】*/
          || ( ( u2t_ecdmfae2 >= u2t_kldmfae )
            && ( u1t_exjokmf == (u1)OFF ) )
#endif
           )
        {
            u1t_xdmfem = (u1)ON;                   /* EM悪化ﾚﾍﾞﾙ現在異常 */
        }
    }

    /* 2-2.(2)①(c)ｲ)もしくはｳ)条件変更時は、下記条件式も見直す事 */
#if JETCORE_D == u1g_EJCC_USE                        /*【t-CORE対応有】*/
    if ( ( ( s4t_ecxmf >= (s4)u1t_CXMFL_c )
      && ( ( u1t_state == u1g_WSENTIVCHG_LVL2 )
        || ( u1t_state == u1g_WSENTIVCHG_IUE ) ) )   /* 感度アップ（評価用） */
 #if JEMFHOUKI == u1g_EJCC_USAMF                     /*【対米法規】*/
      || ( ( u2t_ecdmfae2 >= u2t_kldmfae )
        && ( u1t_exjokmf == (u1)OFF ) )
 #endif
       )
    {
        u1t_xemfst = (u1)ON;                         /* EM悪化レベル初回1000revと判定 */
    }
    /* RAMデータに格納 */
    glbitcp_bibi( u1t_xemfst, bis_emfng_exemfst );   /* EM悪化レベル初回1000rev判定ﾌﾗｸﾞ(65msl) */
#endif

    /* RAMデータに格納 */
    u1s_emfng_ecxmf = (u1)s4t_ecxmf;

    /* 戻り値の設定 */
    *ptt_xdmfem = u1t_xdmfem;
    *ptt_xumfem = u1t_xumfem;
}

#if ( JEEFI == u1g_EJCC_DUAL ) && ( JEHIPFI_E == u1g_EJCC_USE )   /*【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/
/***********************************************************************/
/*  関数名        | emfng_di_detect_ptpt( )                            */
/*  処理内容      | DI異常検出                                         */
/*  制御タイミング| 65ms毎(emfng_65msl()よりコール)                    */
/*  引数          | ptt_xdmfdi   : DI異常ﾌﾗｸﾞの格納先                  */
/*                | ptt_xumfdi   : DI異常による正常判定保留ﾌﾗｸﾞの格納先 */
/*  戻り値        | なし(引数で受け渡し)                               */
/*  注意事項      | 内部関数                                           */
/*                |【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】                   */
/***********************************************************************/
static void
emfng_di_detect_ptpt( u1 *ptt_xdmfdi, u1 *ptt_xumfdi )
{
    u1 u1t_xddi;            /* lsb=1 : DI異常判定ﾌﾗｸﾞ */
    s4 s4t_cyl;             /* lsb=1,unit=気筒 */
    u1 u1t_xdmfdi;          /* lsb=1 : DI異常ﾌﾗｸﾞ */
    u1 u1t_xumfdi;          /* lsb=1 : DI異常による正常判定保留ﾌﾗｸﾞ */
    u1 u1t_ncyl_c;          /* lsb=1 :気筒数 */

    /* ワーク変数の初期化 */
    u1t_xdmfdi = (u1)OFF;
    u1t_xumfdi = (u1)OFF;
    u1t_ncyl_c = u1g_ejcc_NCYL;

    u1t_xddi = (u1)OFF;
    for ( s4t_cyl = (s4)0; s4t_cyl < (s4)u1t_ncyl_c ; s4t_cyl++ )
    {
        if ( u1s_emfng_ecdmfdi[s4t_cyl] >= u1s_emfng_MFDDI )
        {
            u1t_xddi = (u1)ON;
            u1s_emfng_exdmfdim[s4t_cyl] = (u1)ON;
        }
    }

    /********************/
    /* DI異常の検出判定 */
    /********************/
    if ( u1t_xddi == (u1)ON )
    {
        u1t_xdmfdi = (u1)ON;     /* DI異常ﾌﾗｸﾞｾｯﾄ */
        u1t_xumfdi = (u1)ON;     /* DI異常による正常判定保留ﾌﾗｸﾞｾｯﾄ */
        big_emfng_exmfddi = (u1)ON;
    }
    bis_emfng_excdmfdiclr = (u1)ON;  /* exdmfdimｸﾘｱ用ﾌﾗｸﾞ */
    /* 検出ｶｳﾝﾀのｸﾘｱ */
    glint_di(); /* ▼▼▼ 割り込み禁止 ▼▼▼ */
    u2s_emfng_ecjmfdi = (u2)0;       /* 0初期化のためLSB省略 */
    /* DI失火検出終了ﾌﾗｸﾞのｾｯﾄ */
    big_emfng_exmfdied = (u1)ON;
    for ( s4t_cyl = (s4)0; s4t_cyl < (s4)u1t_ncyl_c ; s4t_cyl++ )
    {
        u1s_emfng_ecdmfdi[s4t_cyl] = (u1)(((0.)/(1.))+0.5);   /* ecjmfdiと同時にクリアする */
    }
    glint_ei(); /* ▲▲▲ 割り込み許可 ▲▲▲ */
    *ptt_xdmfdi = u1t_xdmfdi;
    *ptt_xumfdi = u1t_xumfdi;
}

/***********************************************************************/
/*  関数名        | emfng_pfi_detect_ptpt( )                           */
/*  処理内容      | PFI異常検出                                        */
/*  制御タイミング| 65ms毎(emfng_65msl()よりコール)                    */
/*  引数          | ptt_xdmfpfi :PFI異常ﾌﾗｸﾞの格納先                   */
/*                | ptt_xumfpfi :PFI異常による正常判定保留ﾌﾗｸﾞの格納先 */
/*  戻り値        | なし(引数で受け渡し)                               */
/*  注意事項      | 内部関数                                           */
/*                |【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】                   */
/***********************************************************************/
static void
emfng_pfi_detect_ptpt( u1 *ptt_xdmfpfi, u1 *ptt_xumfpfi )
{
    u1 u1t_xdpfi;           /* lsb=1 : PFI異常判定ﾌﾗｸﾞ */
    s4 s4t_cyl;             /* lsb=1,unit=気筒 */
    u1 u1t_xdmfpfi;         /* lsb=1 : PFI異常ﾌﾗｸﾞ */
    u1 u1t_xumfpfi;         /* lsb=1 : PFI異常による正常判定保留ﾌﾗｸﾞ */
    u1 u1t_MFDPFI_c;        /* lsb=1,unit=回 :PFI異常検出失火回数 */
    u1 u1t_ncyl_c;          /* lsb=1 :気筒数 */

    /* ワーク変数の初期化 */
    u1t_xdmfpfi = (u1)OFF;
    u1t_xumfpfi = (u1)OFF;
    u1t_MFDPFI_c = u1s_emfng_MFDPFI;
    u1t_ncyl_c = u1g_ejcc_NCYL;

    u1t_xdpfi = (u1)OFF;
    for ( s4t_cyl = (s4)0; s4t_cyl < (s4)u1t_ncyl_c; s4t_cyl++ )
    {
        if ( u1s_emfng_ecdmfpfi[s4t_cyl] >= u1t_MFDPFI_c )
        {
            u1t_xdpfi = (u1)ON;
            u1s_emfng_exdmfpfim[s4t_cyl] = (u1)ON;
        }
    }

    /*********************/
    /* PFI異常の検出判定 */
    /*********************/
    if ( u1t_xdpfi == (u1)ON )
    {
        u1t_xdmfpfi = (u1)ON;     /* PFI異常ﾌﾗｸﾞｾｯﾄ */
        u1t_xumfpfi = (u1)ON;     /* PFI異常による正常判定保留ﾌﾗｸﾞｾｯﾄ */
        big_emfng_exmfdpfi = (u1)ON;
    }
    bis_emfng_excdmfpficlr = (u1)ON;  /* exdmfpfimｸﾘｱ用ﾌﾗｸﾞ */
    /* 検出ｶｳﾝﾀのｸﾘｱ */
    glint_di(); /* ▼▼▼ 割り込み禁止 ▼▼▼ */
    u2s_emfng_ecjmfpfi = (u2)0;       /* 0初期化のためLSB省略 */
    /* PFI失火検出終了ﾌﾗｸﾞのｾｯﾄ */
    big_emfng_exmfpfied = (u1)ON;
    for ( s4t_cyl = (s4)0; s4t_cyl < (s4)u1t_ncyl_c; s4t_cyl++ )
    {
        u1s_emfng_ecdmfpfi[s4t_cyl] = (u1)(((0.)/(1.))+0.5);  /* ecjmfpfiと同時にｸﾘｱする */
    }
    glint_ei(); /* ▲▲▲ 割り込み許可 ▲▲▲ */
    *ptt_xdmfpfi = u1t_xdmfpfi;
    *ptt_xumfpfi = u1t_xumfpfi;
}
#endif /* JEEFI,JEHIPFI_E */

#if JEMFDTC_D == u1g_EJCC_USE /*【失火OBD DTC有】*/
/*********************************************************************/
/*  関数名        | emfng_wxmf_out( )                                */
/*  処理内容      | 電技への通知処理                                 */
/*  制御タイミング| 65ms毎(emfng_65msl()よりコール)                  */
/*  引数          | なし                                             */
/*  戻り値        | なし                                             */
/*  注意事項      | 内部関数【失火OBD DTC有】                        */
/*                | 触媒OTﾚﾍﾞﾙ,EM悪化ﾚﾍﾞﾙ,DI,PFI異常判定後にcallする */
/*********************************************************************/
static void
emfng_wxmf_out( void )
{
    static volatile u1 u1s_emfng_emfk;       /* lsb=1 :DTC出力気筒 */
 #pragma ghs startdata
 #if ( JENCYL == u1g_EJCC_4CYL )      /*【4気筒】*/
    static volatile const u1 u1s_emfng_cylwtocyl_tbl[2] = /* :cylw → cylwに対応する実気筒ビット列 変換テーブル */
    { (u1)0x06U, (u1)0x09U };                             /* 0,1 → #2#3,#1#4 */
 #endif
 #if ( JENCYL == u1g_EJCC_6CYL )      /*【6気筒】*/
  #if JEEGTYPE == u1g_EJCC_V6CYL       /*【V6】*/
    static volatile const u1 u1s_emfng_cylwtocyl_tbl[3] = /* :cylw → cylwに対応する実気筒ビット列 変換テーブル */
    { (u1)0x24U, (u1)0x09U, (u1)0x12U };                  /* 0,1,2 → #3#6,#1#4,#2#5 */
  #elif JEEGTYPE == u1g_EJCC_L6CYL     /*【L6】*/
    static volatile const u1 u1s_emfng_cylwtocyl_tbl[3] = /* :cylw → cylwに対応する実気筒ビット列 変換テーブル */
    { (u1)0x0cU, (u1)0x21U, (u1)0x12U };                  /* 0,1,2 → #3#4,#1#6,#2#5 */
  #endif
 #endif
 #pragma ghs enddata

    u1 u1t_i;
    s4 s4t_cnt;
    u1 u1t_xemfk;                    /* lsb=1 : 気筒に対応する実気筒ビット */
    u1 u1t_xemfkw;                   /* lsb=1 : 気筒群に対応する実気筒ビット(対向気筒用) */
    u1 u1t_xemfk_opps;               /* lsb=1 : 気筒群に対応する実気筒ビット(対向気筒用ワーク変数) */
    u4 u4t_ecdmfem2mx;               /* lsb=1,unit=回 : ecdmfem2[]最大値 */
    u4 u4t_ecdmfem2[u1g_EJCC_NCYL];  /* lsb=1,unit=回 : EM悪化ﾚﾍﾞﾙ気筒別失火ｶｳﾝﾀ */
    s4 s4t_ecdmfae2;                 /* lsb=1,unit=回 : 総失火回数×0.9 */
    u1  u1t_xdmfot;                  /* lsb=1 :触媒OTﾚﾍﾞﾙ異常判定ﾌﾗｸﾞ */
    u1  u1t_xumfot;                  /* lsb=1 :触媒OTﾚﾍﾞﾙ正常判定保留ﾌﾗｸﾞ */
    u1  u1t_xhmfoton;                /* lsb=1 :触媒OTによるMIL点滅要求ﾌﾗｸﾞ */
    u1  u1t_xhmfotoff;               /* lsb=1 :触媒OTによるMIL点滅ｷｬﾝｾﾙ要求ﾌﾗｸﾞ */
    u1  u1t_xdmfem;                  /* lsb=1 :EM悪化ﾚﾍﾞﾙ異常検出ﾌﾗｸﾞ */
    u1  u1t_xumfem;                  /* lsb=1 :EM悪化ﾚﾍﾞﾙ正常判定保留ﾌﾗｸﾞ */
 #if ( JEEFI == u1g_EJCC_DUAL ) && ( JEHIPFI_E == u1g_EJCC_USE )   /*【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/
    u1  u1t_xdmfdi;                  /* lsb=1 :DI異常ﾌﾗｸﾞ */
    u1  u1t_xumfdi;                  /* lsb=1 :DI異常による正常判定保留ﾌﾗｸﾞ */
    u1  u1t_xdmfpfi;                 /* lsb=1 :PFI異常ﾌﾗｸﾞ */
    u1  u1t_xumfpfi;                 /* lsb=1 :PFI異常による正常判定保留ﾌﾗｸﾞ */
 #endif
    u1  u1t_excdmfw;                 /* lsb=1:対向気筒失火判定成立ﾌﾗｸﾞ */
 #if JETCORE_D == u1g_EJCC_USE       /*【t-CORE対応有】*/
    u1  u1t_otcnd;                   /* lsb=1 :OT失火条件識別子 */
    u1  u1t_emcnd;                   /* lsb=1 :EM失火条件識別子 */
    u1  u1t_exdmf;                   /* lsb=1 :失火OBD現在異常ﾌﾗｸﾞ */
 #endif
    u1 u1t_ncyl_c;                   /* lsb=1 :気筒数 */
    u1 u1t_ncylw_c;                  /* lsb=1 :気筒群数 */

    /* ワークデータ初期化 */
    u1t_ncyl_c = u1g_ejcc_NCYL;
    u1t_excdmfw = (u1)big_emfng_excdmfw;
    u4t_ecdmfem2[0] = (u4)u2g_emfng_ecdmfem2[0];    /* 複数回参照のためラッチ */

    for ( u1t_i = (u1)1; u1t_i < u1t_ncyl_c; u1t_i++ )
    {
        u4t_ecdmfem2[u1t_i] = (u4)u2g_emfng_ecdmfem2[u1t_i];   /* 複数回参照のためラッチ */
    }

    /* データ値取得 */
    u1t_xdmfot    = (u1)bis_emfng_exdmfot;         /* 触媒OTﾚﾍﾞﾙ異常判定ﾌﾗｸﾞ */
    u1t_xumfot    = (u1)bis_emfng_exumfot;         /* 触媒OTﾚﾍﾞﾙ正常判定保留ﾌﾗｸﾞ */
    u1t_xhmfoton  = (u1)bis_emfng_exhmfoton;       /* 触媒OTによるMIL点滅要求ﾌﾗｸﾞ */
    u1t_xhmfotoff = (u1)bis_emfng_exhmfotoff;      /* 触媒OTによるMIL点滅ｷｬﾝｾﾙ要求ﾌﾗｸﾞ */
    u1t_xdmfem    = (u1)bis_emfng_exdmfem;         /* EM悪化ﾚﾍﾞﾙ異常検出ﾌﾗｸﾞ */
    u1t_xumfem    = (u1)bis_emfng_exumfem;         /* EM悪化ﾚﾍﾞﾙ正常判定保留ﾌﾗｸﾞ */
 #if ( JEEFI == u1g_EJCC_DUAL ) && ( JEHIPFI_E == u1g_EJCC_USE )   /*【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/
    u1t_xdmfdi    = (u1)bis_emfng_exdmfdi;         /* DI異常ﾌﾗｸﾞ */
    u1t_xumfdi    = (u1)bis_emfng_exumfdi;         /* DI異常による正常判定保留ﾌﾗｸﾞ */
    u1t_xdmfpfi   = (u1)bis_emfng_exdmfpfi;        /* PFI異常ﾌﾗｸﾞ */
    u1t_xumfpfi   = (u1)bis_emfng_exumfpfi;        /* PFI異常による正常判定保留ﾌﾗｸﾞ */
 #endif
 #if JETCORE_D == u1g_EJCC_USE                                      /*【t-CORE対応有】*/
    u1t_exdmf     = (u1)big_emfng_exdmf;           /* 失火OBD現在異常ﾌﾗｸﾞ */
 #endif

    /* ワークデータ初期化 */
    u1t_xemfk = (u1)0x00U;

    /*************************/
    /* 電技へのDTC出力の通知 */
    /*************************/

 #if ( JEEFI == u1g_EJCC_DUAL ) && ( JEHIPFI_E == u1g_EJCC_USE )   /*【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/
    /*******************/
    /* PFI異常時の処理 */
    /*******************/
    if ( u1t_xdmfpfi == (u1)ON )
    {
        /* PFI異常時の各気筒の失火有無判定 */
        for ( u1t_i = (u1)0; u1t_i < u1t_ncyl_c; u1t_i++ )
        {
            if ( u1s_emfng_exdmfpfim[u1t_i] == (u1)ON )
            {
                u1t_xemfk |= u1g_EMFCNT_CYL(u1t_i);
            }
        }
    }
    /******************/
    /* DI異常時の処理 */
    /******************/
    else if ( u1t_xdmfdi == (u1)ON )
    {
        /* DI異常時の各気筒の失火有無判定 */
        for ( u1t_i = (u1)0; u1t_i < u1t_ncyl_c; u1t_i++ )
        {
            if ( u1s_emfng_exdmfdim[u1t_i] == (u1)ON )
            {
                u1t_xemfk |= u1g_EMFCNT_CYL(u1t_i);
            }
        }
    }
    else
 #endif
    {
        /******************************/
        /* 触媒OTﾚﾍﾞﾙ異常検出時、     */
        /* EM悪化ﾚﾍﾞﾙ異常検出時の処理 */
        /******************************/
        if ( ( u1t_xdmfem == (u1)ON )       /* EM悪化ﾚﾍﾞﾙ異常 */
          || ( u1t_xdmfot == (u1)ON ) )     /* 触媒OTﾚﾍﾞﾙ異常 */
        {
            /***************************/
            /* 総失火回数の 90% の算出 */
            /***************************/
            s4t_ecdmfae2 = (s4)u2g_emfng_ecdmfae2;
            s4t_ecdmfae2 = s4g_glmulst_s4s4u2( s4t_ecdmfae2, (s4)(((0.9)/(1./128./256.))+0.5), (u2)15U );
                                                            /* lsb= 1 * 1/128/256 -> 1(回) */

            if ( u1t_excdmfw == (u1)OFF )             /* 対向気筒失火ではない */
            {
                /* 対向気筒失火時はecdmfem2[],ecdmfae2共にカウントアップされず            */
                /* 0(回)≧0(回)*90%となり、常時成立してしまうため対向気筒時は判定をしない */

                for ( u1t_i = (u1)0; u1t_i < u1t_ncyl_c; u1t_i++ )
                {
                    /****************************************************************/
                    /* 総失火回数の90%以上が1つの気筒に集中している → P0301～P0308 */
                    /****************************************************************/
                    if ( (s4)u4t_ecdmfem2[u1t_i] >= s4t_ecdmfae2 )
                    {
                        u1t_xemfk |= u1g_EMFCNT_CYL(u1t_i);
                    }
                } /* ﾙｰﾌﾟend */
            }

            /***********************************************/
            /* １つの気筒に失火集中していない場合 → P0300 */
            /***********************************************/
            if ( u1t_xemfk == (u1)0x00U )
            {
                /***************************************/
                /* 気筒別失火回数の最大値 * 0.1 の算出 */
                /***************************************/
                u4t_ecdmfem2mx = u4t_ecdmfem2[0];
                for ( u1t_i = (u1)1; u1t_i < u1t_ncyl_c; u1t_i++ )
                {
                    if ( u4t_ecdmfem2mx < u4t_ecdmfem2[u1t_i] )  /* Max値選択 */
                    {
                        u4t_ecdmfem2mx = u4t_ecdmfem2[u1t_i];
                    }
                }
                u4t_ecdmfem2mx = (u4)u2g_glmulst_u2u2u2( (u2)u4t_ecdmfem2mx, (u2)(((0.1)/(1./128./256.))+0.5), (u2)15U );
                                                                     /* lsb= 1 * 1/128/256 -> 1(回) */

                /* 気筒別失火回数が 所定値以上 */
                /* 一番失火回数が多い気筒の失火回数×0.1 を 上回る */
                s4t_cnt   = (s4)0;
                for ( u1t_i = (u1)0; u1t_i < u1t_ncyl_c; u1t_i++ )
                {
                    if ( ( u4t_ecdmfem2[u1t_i] >= (u4)u2s_emfng_emfk_mcr )   /* 判定(a) */
                      && ( u4t_ecdmfem2[u1t_i] >= u4t_ecdmfem2mx ) )
                    {
                        u1t_xemfk |= u1g_EMFCNT_CYL(u1t_i);
                        s4t_cnt++;
                    }
                } /* ﾙｰﾌﾟend */

                if ( ( s4t_cnt == (s4)1 )              /* 1気筒しか上記条件が成立しなかった。                  */
                  && ( u1t_excdmfw == (u1)OFF ) )      /* かつ、対向気筒失火ではない(4気筒は常時OFFのため成立) */
                {
                    u1t_xemfk = (u1)0x00U;      /* u1t_xemfkに値が残っているとP0301～P0308 がセットされるのでクリア */
                }
            }   /* 総失火回数の90%以上が1つの気筒に集中していなかったケース */

            u1t_xemfkw = (u1)0x00U;
            if ( u1t_excdmfw == (u1)ON )                          /* 対向気筒失火 */
            {
                u1t_ncylw_c = u1s_EMFNG_NCYLW;
                for ( u1t_i = (u1)0; u1t_i < u1t_ncylw_c; u1t_i++ )
                {
                    if ( u1g_emfng_ecdmfw[u1t_i] > u1s_emfng_ekldmfw1_mcr )    /* 判定(b) */
                    {
                        u1t_xemfk_opps = u1s_emfng_cylwtocyl_tbl[u1t_i];   /* 気筒群に対応する実気筒ビット */
 #if JENCYL == u1g_EJCC_6CYL    /*【6気筒】*/
                        if ( ( u1t_xemfk_opps & u1t_xemfk ) == (u1)0x00U )
                                                                /* ecdmfem[](ecdmfem2[])による判定で既にビットセット */
 #endif
                        {                                           /* されている場合はビットセットしない */
                            u1t_xemfkw |= u1t_xemfk_opps;           /* 気筒群に対応する実気筒ビットを1 */
                        }
                    }
                }
            }
            u1t_xemfk |= u1t_xemfkw;                                /* 判定(a)と(b)のORをとる */
        } /* EM悪化ﾚﾍﾞﾙ異常 or 触媒OTﾚﾍﾞﾙ異常の判定終了 */
    } /* DI,PFI異常検出以外の判定終了 */

    /* DTC出力気筒の算出 */
    u1s_emfng_emfk = u1t_xemfk;

    /***************************/
    /* 電技への失火OBD異常通知 */
    /***************************/
 #if JETCORE_D == u1g_EJCC_NOT_USE                 /*【t-CORE対応無】*/
    /* 触媒OTﾚﾍﾞﾙ現在異常通知 */
    if ( u1t_xdmfot == (u1)ON )
    {
        vdg_wmf_fdi_ot_fald_u1( u1t_xemfk );     /* 触媒OTﾚﾍﾞﾙ現在異常通知 */
    }

    /* EM悪化ﾚﾍﾞﾙ現在異常通知 */
    if ( ( u1t_xdmfem == (u1)ON )
  #if ( JEEFI == u1g_EJCC_DUAL ) && ( JEHIPFI_E == u1g_EJCC_USE )   /*【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/
      || ( u1t_xdmfdi == (u1)ON )
      || ( u1t_xdmfpfi == (u1)ON )
  #endif
       )
    {
        vdg_wmf_fdi_em_fald_u1( u1t_xemfk );       /* EM悪化ﾚﾍﾞﾙ現在異常通知 */
    }
 #endif

    /* 触媒OTﾚﾍﾞﾙ本ﾄﾘｯﾌﾟ異常通知 【t-CORE対応無】*/
    /* 触媒OT失火率超過通知 【t-CORE対応有】*/
    if ( u1t_xumfot == (u1)ON )
    {
 #if JETCORE_D == u1g_EJCC_NOT_USE    /*【t-CORE対応無】*/
        vdg_wmf_fdi_ot_excmf();     /* 触媒OTﾚﾍﾞﾙ正常判定保留通知 */
 #else                                /*【t-CORE対応有】*/
        u1t_otcnd = u1g_WMF_FDI_DURING_FTPCYCLE;
        if ( bis_emfng_exoutftp == (u1)ON )
        {
            u1t_otcnd = u1g_WMF_FDI_OUTOF_FTPCYCLE;
        }
        vdg_wmf_fdi_ot_excmf_u2u1( (u2)u1t_xemfk, u1t_otcnd );    /* 触媒OT失火率超過通知 */
 #endif
    }

    /* EM悪化ﾚﾍﾞﾙ本ﾄﾘｯﾌﾟ異常通知 【t-CORE対応無】*/
    /* EM悪化失火率超過通知 【t-CORE対応有】*/
    if ( ( u1t_xumfem == (u1)ON )
 #if ( JEEFI == u1g_EJCC_DUAL ) && ( JEHIPFI_E == u1g_EJCC_USE )   /*【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/
      || ( u1t_xumfdi == (u1)ON )
      || ( u1t_xumfpfi == (u1)ON )
 #endif
       )
    {
 #if JETCORE_D == u1g_EJCC_NOT_USE      /*【t-CORE対応無】*/
        vdg_wmf_fdi_em_excmf();       /* EM悪化ﾚﾍﾞﾙ正常判定保留 */
 #else                                  /*【t-CORE対応有】*/
        u1t_emcnd = u1g_WMF_FDI_AFTER_FIRST1000RPM;
        if ( bis_emfng_exemfst == (u1)ON )
        {
            u1t_emcnd = u1g_WMF_FDI_WITHIN_FIRST1000RPM;
        }
        vdg_wmf_fdi_em_excmf_u2u1( (u2)u1t_xemfk, u1t_emcnd );    /* EM悪化失火率超過通知 */
 #endif
    }

    /* 触媒OTによるMIL点滅通知 */
    if ( u1t_xhmfoton == (u1)ON )
    {
 #if JETCORE_D == u1g_EJCC_NOT_USE               /*【t-CORE対応無】*/
        vdg_wmf_fdi_mil_blink_u1( (u1)ON );    /* MIL点滅通知 */
 #endif
        vdg_wmf_fdi_ot_imdf();                 /* 触媒OT失火ﾌｪｰﾙｾｰﾌ情報現在異常通知 */
    }

    /* 触媒OTによるMIL点滅ｷｬﾝｾﾙ通知 */
    if ( u1t_xhmfotoff == (u1)ON )
    {
 #if JETCORE_D == u1g_EJCC_NOT_USE               /*【t-CORE対応無】*/
        vdg_wmf_fdi_mil_blink_u1( (u1)OFF );   /* MIL点滅ｷｬﾝｾﾙ通知 */
 #else                                           /*【t-CORE対応有】*/
        vdg_wmf_fdi_pas_u2u1( u2g_WMF_FDI_ID_OT,(u1)ON );   /* OT失火正常判定 */
 #endif
        vdg_wmf_fdi_ot_imdnf();                /* 触媒OT失火ﾌｪｰﾙｾｰﾌ情報現在異常外通知 */
    }

 #if JETCORE_D == u1g_EJCC_USE                  /*【t-CORE対応有】*/
    /* 失火OBD現在異常フラグの算出 */
    if ( ( u1t_xdmfot == (u1)ON )
      || ( u1t_xdmfem == (u1)ON )
  #if ( JEEFI == u1g_EJCC_DUAL ) && ( JEHIPFI_E == u1g_EJCC_USE )   /*【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/
      || ( u1t_xdmfdi == (u1)ON )
      || ( u1t_xdmfpfi == (u1)ON )
  #endif
       )
    {
        u1t_exdmf = (u1)ON;
    }

    if ( u1s_emfng_xreqclr_Lch == (u1)ON )
    {
        u1t_exdmf = (u1)OFF;
    }
    /* RAMデータに格納 */
    glbitcp_bibi( u1t_exdmf, big_emfng_exdmf );               /* 失火OBD現在異常ﾌﾗｸﾞ */

  #if JEOBDUDS_D == u1g_EJCC_USE                /*【OBDonUDS対応有】*/
    /* ﾚﾃﾞｨﾈｽｾｯﾄのための1000rev毎正常通知 */
    if ( ( u1s_emfng_pas == (u1)ON )
      && ( ( u1t_exdmf == (u1)OFF )
        && ( ( big_wmf_fdi_xot_pnd == (u1)OFF )
          && ( big_wmf_fdi_xem_pnd == (u1)OFF ) )
        && ( ( u1s_emfng_xot_lstpnd_Lch == (u1)OFF )
          && ( u1s_emfng_xem_lstpnd_Lch == (u1)OFF ) )
        && ( ( u1s_emfng_xot_warning_Lch == (u1)OFF )
          && ( u1s_emfng_xem_warning_Lch == (u1)OFF ) )
        && ( ( u1s_emfng_ecxmf1 == u1s_EMFNG_CXMF1PAS )
          && ( u1s_emfng_ecxmf == u1s_EMFNG_CXMFPAS ) ) ) )
    {
        vdg_wmf_fdi_pas_u2u1( u2g_WMF_FDI_ID_EM,(u1)ON );    /* EM失火正常判定 */
    }
  #endif
 #endif

}
#endif /* JEMFDTC_D */

#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
/*********************************************************************/
/*  関数名        | emfng_eocmfmlt_pt()                              */
/*  処理内容      | 市場調査RAM更新処理                              */
/*  制御タイミング| 65ms毎(emfng_65msl()よりコール)                  */
/*  引数          | ptt_ecdmfw[](先頭アドレス)                       */
/*  戻り値        | なし                                             */
/*  注意事項      | 内部関数【市場調査有】                           */
/*********************************************************************/
static void
emfng_eocmfmlt_pt( const u1 ptt_ecdmfw[] )
{
 #if JENCYL == u1g_EJCC_6CYL    /*【6気筒】*/
    s4 s4t_i;               /* lsb=1 :forループ用変数 */
    u2 u2t_ocmfmlth;        /* lsb=1,unit=回 :対向気筒失火回数(大)の頻度 */
    u2 u2t_ocmfmltl;        /* lsb=1,unit=回 :対向気筒失火回数(小)の頻度 */
    s4 s4t_ecdmfw[3];       /* lsb=1,unit=回 :気筒群最大数分 */
    s4 s4t_enejsm;          /* lsb=200/8,unit=rpm :NEなまし値 */
    u1 u1t_onemfmlt;        /* lsb=200/8,unit=rpm :対向気筒失火時のｴﾝｼﾞﾝ回転なまし値 */
    u1 u1t_jdg;
    u1 u1t_ncylw_c;         /* lsb=1 :気筒群数 */
 #endif
    u1 u1t_cdmfaot;         /* lsb=1,unit=回 :触媒OTﾚﾍﾞﾙ全気筒失火ｶｳﾝﾀ */
    u1 u1t_ocdmfaotmx;      /* lsb=1,unit=回 :200rev毎の最大総失火回数 */
    s2 s2t_thw;             /* lsb=160/128,unit=℃ :ｴﾝｼﾞﾝ制御用水温 */
    s1 s1t_othwot;          /* lsb=160/128,unit=℃ :最大失火ｶｳﾝﾀ更新時の水温 */
    s2 s2t_tha;             /* lsb=160/128,unit=℃ :ｴﾝｼﾞﾝ制御用吸気温 */
    s1 s1t_othaot;          /* lsb=160/128,unit=℃ :最大失火ｶｳﾝﾀ更新時の吸気温 */
    u1 u1t_ecjemrev;        /* lsb=1,unit=回 :始動後EM悪化レベル判定回数(ツール警告回避の為) */

    /* ワークデータ初期化 */
 #if JENCYL == u1g_EJCC_6CYL    /*【6気筒】*/
    u1t_jdg = (u1)OFF;
    u1t_ncylw_c = u1s_EMFNG_NCYLW;
    for ( s4t_i = (s4)0; s4t_i < (s4)u1t_ncylw_c; s4t_i++ )
    {
        s4t_ecdmfw[s4t_i] = (s4)ptt_ecdmfw[s4t_i];
    }
    if ( ( s4t_ecdmfw[0] >= (s4)50 )
      || ( s4t_ecdmfw[1] >= (s4)50 )
      || ( s4t_ecdmfw[2] >= (s4)50 ) )
    {
        u2t_ocmfmlth = u2g_emfng_eocmfmlth();         /* eocmfmlth取得 */
        if ( u2t_ocmfmlth < u2g_U2MAX )
        {
            u2t_ocmfmlth++;                               /* lsb=1,unit=回 :対向気筒失火回数大の頻度 */
        }
        (void)s4g_ememctr_write( u2g_EMFNG_EOCMFMLTH_U2_ID, (void *)&u2t_ocmfmlth );   /* 戻り値は使用しない */
        u1t_jdg = (u1)ON;
    }
    else if ( ( s4t_ecdmfw[0] >= (s4)20 )
           || ( s4t_ecdmfw[1] >= (s4)20 )
           || ( s4t_ecdmfw[2] >= (s4)20 ) )
    {
        u2t_ocmfmltl = u2g_emfng_eocmfmltl();         /* eocmfmltl取得 */
        if ( u2t_ocmfmltl < u2g_U2MAX )
        {
            u2t_ocmfmltl++;                               /* lsb=1,unit=回 :対向気筒失火回数小の頻度 */
        }
        (void)s4g_ememctr_write( u2g_EMFNG_EOCMFMLTL_U2_ID, (void *)&u2t_ocmfmltl );   /* 戻り値は使用しない */
        u1t_jdg = (u1)ON;
    }
    else
    {
        /* 処理なし */
    }

    if ( u1t_jdg == (u1)ON )
    {
        s4t_enejsm = s4g_glbitcal_s4sftr_s4u1( (s4)s2g_eobdsm_enejsm, (u1)5U );
        ELIB_HILOGD2( s4t_enejsm, (s4)u1g_U1MAX, (s4)u1g_U1MIN, s4t_enejsm );
        u1t_onemfmlt = (u1)s4t_enejsm;
        (void)s4g_ememctr_write( u2g_EMFNG_EONEMFMLT_U1_ID, (void *)&u1t_onemfmlt );   /* 戻り値は使用しない */
                                                 /* lsb=200/256→200/8,unit=rpm :市場調査RAM更新時のenejsm */
    }
 #endif

    /**********************/
    /*     初期化処理     */
    /**********************/
    u1t_cdmfaot = u1g_emfng_ecdmfaot;

    /* バックアップRAM取得 */
    u1t_ocdmfaotmx = u1g_emfng_eocdmfaotmx();        /* lsb=1, unit=回 */

    if ( u1t_ocdmfaotmx < u1t_cdmfaot )
    {
        s2t_thw = s2g_glbitcal_s2sftr_s2u1( s2g_ethw_ethw, (u1)1U );
        ELIB_HILOGD2( s2t_thw, (s2)s1g_S1MAX, (s2)s1g_S1MIN, s2t_thw );
        s1t_othwot = (s1)s2t_thw;
        s2t_tha = s2g_glbitcal_s2sftr_s2u1( s2g_etha_etha, (u1)1U );
        ELIB_HILOGD2( s2t_tha, (s2)s1g_S1MAX, (s2)s1g_S1MIN, s2t_tha );
        s1t_othaot = (s1)s2t_tha;

        /****************************************************/
        /* 200rev中の失火回数が最大時、バックアップRAM更新  */
        /****************************************************/
        (void)s4g_ememctr_write( u2g_EMFNG_EOCDMFAOTMX_U1_ID, (void *)&u1t_cdmfaot );  /* 戻り値は使用しない */
        u1t_ecjemrev = u1s_emfng_ecjemrev;
        (void)s4g_ememctr_write( u2g_EMFNG_EOCJREVOT_U1_ID, (void *)&u1t_ecjemrev );   /* 戻り値は使用しない */
        (void)s4g_ememctr_write( u2g_EMFNG_EOTHWOT_S1_ID, (void *)&s1t_othwot );       /* 戻り値は使用しない */
        (void)s4g_ememctr_write( u2g_EMFNG_EOTHAOT_S1_ID, (void *)&s1t_othaot );       /* 戻り値は使用しない */
    }
}

/*********************************************************************/
/*  関数名        | emfng_eocmfn_u2()                                */
/*  処理内容      | 市場調査RAM更新処理                              */
/*  制御タイミング| 65ms毎(emfng_65msl()よりコール)                  */
/*  引数          | u2t_cdmfae lsb=1 :EM悪化ﾚﾍﾞﾙ失火回数             */
/*                |                   (全気筒分)ｶｳﾝﾀ(CDMFAE2)        */
/*  戻り値        | なし                                             */
/*  注意事項      | 内部関数【市場調査有】                           */
/*********************************************************************/
static void
emfng_eocmfn_u2( u2 u2t_cdmfae )
{
    u2  u2t_ocmfnh;         /* lsb=1,unit=回 :失火回数(大)の頻度 */
    u2  u2t_ocmfnm;         /* lsb=1,unit=回 :失火回数(中)の頻度 */
    u2  u2t_ocmfnl;         /* lsb=1,unit=回 :失火回数(小)の頻度 */
    s4  s4t_i;              /* lsb=1 :forループ用変数 */
    u2  u2t_cdmfae2;        /* lsb=1,unit=回 :EM悪化ﾚﾍﾞﾙ全気筒失火ｶｳﾝﾀ */
    u2  u2t_cdmfae2gd;      /* lsb=1,unit=回 :EM悪化ﾚﾍﾞﾙ全気筒失火ｶｳﾝﾀｶﾞｰﾄﾞ値 */
    u1  u1t_cdmfae2gd;      /* lsb=1,unit=回 :EM悪化ﾚﾍﾞﾙ全気筒失火ｶｳﾝﾀｶﾞｰﾄﾞ値(書き込み用) */
    u1  u1t_ocdmfae2mx;     /* lsb=1,unit=回 :1000rev毎の最大総失火回数 */
    s2  s2t_thw;            /* lsb=160/128,unit=℃ :ｴﾝｼﾞﾝ制御用水温 */
    s1  s1t_othwem;         /* lsb=160/128,unit=℃ :最大失火ｶｳﾝﾀ更新時の水温 */
    s2  s2t_tha;            /* lsb=160/128,unit=℃ :ｴﾝｼﾞﾝ制御用吸気温 */
    s1  s1t_othaem;         /* lsb=160/128,unit=℃ :最大失火ｶｳﾝﾀ更新時の吸気温 */
    u2  u2t_ecdmfem2;       /* lsb=1,unit=回 :EM悪化ﾚﾍﾞﾙ気筒別失火ｶｳﾝﾀ */
    u1  u1t_ocdmfem2mx;     /* lsb=1,unit=回 :最大失火ｶｳﾝﾀ更新時の気筒別失火ｶｳﾝﾀ */
    s4  s4t_spdav;          /* lsb=256/256,unit=km/h :1000rev中の失火回数最大時の失火回数ｱｯﾌﾟ時平均車速 */
    u1  u1t_ospdemav;       /* lsb=256/256,unit=km/h :1000rev中の失火回数最大時の失火回数ｱｯﾌﾟ時平均車速(書き込み用) */
    s4  s4t_neemav;         /* lsb=12800/256,unit=rpm :失火回数UP時の平均ｴﾝｼﾞﾝ回転数 */
    u1  u1t_oneemav;        /* lsb=12800/256,unit=rpm :失火回数UP時の平均ｴﾝｼﾞﾝ回転数(書き込み用) */
    s4  s4t_klsmemav;       /* lsb=320/256,unit=% :失火回数UP時の平均負荷 */
    u1  u1t_oklsmemav;      /* lsb=320/256,unit=% :失火回数UP時の平均負荷(書き込み用) */
    u1  u1t_ecjmfneemi;     /* lsb=1,unit=回 :1000rev間のｱｲﾄﾞﾙﾓｰﾄﾞ回数(ツール警告回避の為) */
    u1  u1t_ecjmfneeml;     /* lsb=1,unit=回 :1000rev間のﾛｰﾓｰﾄﾞ回数(ツール警告回避の為) */
    u1  u1t_ecjmfneemh;     /* lsb=1,unit=回 :1000rev間のﾊｲﾓｰﾄﾞ回数(ツール警告回避の為) */
    u1  u1t_ecjemrev;       /* lsb=1,unit=回 :始動後EM悪化レベル判定回数(ツール警告回避の為) */
    u1  u1t_ncyl_c;         /* lsb=1 :気筒数 */

    if ( u2t_cdmfae >= u2s_emfng_ecmfnh_mcr )
    {
        u2t_ocmfnh = u2g_emfng_eocmfnh();         /* eocmfnh取得 */
        if ( u2t_ocmfnh < u2g_U2MAX )
        {
            u2t_ocmfnh++;                             /* lsb=1,unit=回 :失火回数(大)の頻度 */
        }
        (void)s4g_ememctr_write( u2g_EMFNG_EOCMFNH_U2_ID, (void *)&u2t_ocmfnh );               /* 戻り値は使用しない */
    }
    else if ( u2t_cdmfae >= u2s_emfng_ecmfnm_mcr )
    {
        u2t_ocmfnm = u2g_emfng_eocmfnm();         /* eocmfnm取得 */
        if ( u2t_ocmfnm < u2g_U2MAX )
        {
            u2t_ocmfnm++;                             /* lsb=1,unit=回 :失火回数(中)の頻度 */
        }
        (void)s4g_ememctr_write( u2g_EMFNG_EOCMFNM_U2_ID, (void *)&u2t_ocmfnm );               /* 戻り値は使用しない */
    }
    else if ( u2t_cdmfae >= u2s_emfng_ecmfnl_mcr )
    {
        u2t_ocmfnl = u2g_emfng_eocmfnl();         /* eocmfnl取得 */
        if ( u2t_ocmfnl < u2g_U2MAX )
        {
            u2t_ocmfnl++;                             /* lsb=1,unit=回 :失火回数(小)の頻度 */
        }
        (void)s4g_ememctr_write( u2g_EMFNG_EOCMFNL_U2_ID, (void *)&u2t_ocmfnl );               /* 戻り値は使用しない */
    }
    else
    {
        /* 処理なし */
    }

    /**********************/
    /*     初期化処理     */
    /**********************/
    u2t_cdmfae2 = u2g_emfng_ecdmfae2;
    ELIB_HIGHGD2( u2t_cdmfae2, (u2)u1g_U1MAX, u2t_cdmfae2gd );
    /* バックアップRAM取得 */
    u1t_ocdmfae2mx = u1g_emfng_eocdmfae2mx();        /* lsb=1, unit=回 */

    if ( u1t_ocdmfae2mx < (u1)u2t_cdmfae2gd )
    {
        s2t_thw = s2g_glbitcal_s2sftr_s2u1( s2g_ethw_ethw, (u1)1U );
        ELIB_HILOGD2( s2t_thw, (s2)s1g_S1MAX, (s2)s1g_S1MIN, s2t_thw );
        s1t_othwem = (s1)s2t_thw;
        s2t_tha = s2g_glbitcal_s2sftr_s2u1( s2g_etha_etha, (u1)1U );
        ELIB_HILOGD2( s2t_tha, (s2)s1g_S1MAX, (s2)s1g_S1MIN, s2t_tha );
        s1t_othaem = (s1)s2t_tha;

        s4t_spdav = s4g_gldiv_s4s4( s4s_emfng_espdaem, (s4)u2t_cdmfae2 );
        ELIB_HILOGD2( s4t_spdav, (s4)u1g_U1MAX, (s4)u1g_U1MIN, s4t_spdav );
        u1t_ospdemav = (u1)s4t_spdav;
        s4t_neemav = s4g_glbitcal_s4sftr_s4u1( s4s_emfng_eneaem, (u1)6U );  /* lsb=12800/64/256 -> 12800/256 */
        s4t_neemav = s4g_gldiv_s4s4( s4t_neemav, (s4)u2t_cdmfae2 );
        ELIB_HILOGD2( s4t_neemav, (s4)u1g_U1MAX, (s4)u1g_U1MIN, s4t_neemav );
        u1t_oneemav = (u1)s4t_neemav;
        s4t_klsmemav = s4g_glbitcal_s4sftr_s4u1( s4s_emfng_eklsmaem, (u1)6U );  /* lsb=320/64/256 -> 320/256 */
        s4t_klsmemav = s4g_gldiv_s4s4( s4t_klsmemav, (s4)u2t_cdmfae2 );
        ELIB_HILOGD2( s4t_klsmemav, (s4)u1g_U1MAX, (s4)u1g_U1MIN, s4t_klsmemav );
        u1t_oklsmemav = (u1)s4t_klsmemav;

        /****************************************************/
        /* 1000rev中の失火回数が最大時、バックアップRAM更新 */
        /****************************************************/
        u1t_cdmfae2gd = (u1)u2t_cdmfae2gd;
        (void)s4g_ememctr_write( u2g_EMFNG_EOCDMFAE2MX_U1_ID, (void *)&u1t_cdmfae2gd );        /* 戻り値は使用しない */
        u1t_ecjemrev = u1s_emfng_ecjemrev;
        (void)s4g_ememctr_write( u2g_EMFNG_EOCJREVEM_U1_ID, (void *)&u1t_ecjemrev );           /* 戻り値は使用しない */
        (void)s4g_ememctr_write( u2g_EMFNG_EOTHWEM_S1_ID, (void *)&s1t_othwem );               /* 戻り値は使用しない */
        (void)s4g_ememctr_write( u2g_EMFNG_EOTHAEM_S1_ID, (void *)&s1t_othaem );               /* 戻り値は使用しない */

        u1t_ncyl_c = u1g_ejcc_NCYL;
        for ( s4t_i = (s4)0; s4t_i < (s4)u1t_ncyl_c; s4t_i++ )
        {
            u2t_ecdmfem2 = u2g_emfng_ecdmfem2[s4t_i];
            ELIB_HIGHGD2( u2t_ecdmfem2, (u2)u1g_U1MAX, u2t_ecdmfem2 );
            u1t_ocdmfem2mx = (u1)u2t_ecdmfem2;
            (void)s4g_ememctr_write( u2s_EOCDMFEM2MX_U1_ID[s4t_i], (void *)&u1t_ocdmfem2mx );  /* 戻り値は使用しない */
        }

        (void)s4g_ememctr_write( u2g_EMFNG_EOSPDEMAV_U1_ID, (void *)&u1t_ospdemav );           /* 戻り値は使用しない */
        (void)s4g_ememctr_write( u2g_EMFNG_EONEEMAV_U1_ID, (void *)&u1t_oneemav );             /* 戻り値は使用しない */
        (void)s4g_ememctr_write( u2g_EMFNG_EOKLSMEMAV_U1_ID, (void *)&u1t_oklsmemav );         /* 戻り値は使用しない */

        u1t_ecjmfneemi = u1s_emfng_ecjmfneemi;
        (void)s4g_ememctr_write( u2g_EMFNG_EOCJMFNEEMI_U1_ID, (void *)&u1t_ecjmfneemi );       /* 戻り値は使用しない */
        u1t_ecjmfneeml = u1s_emfng_ecjmfneeml;
        (void)s4g_ememctr_write( u2g_EMFNG_EOCJMFNEEML_U1_ID, (void *)&u1t_ecjmfneeml );       /* 戻り値は使用しない */
        u1t_ecjmfneemh = u1s_emfng_ecjmfneemh;
        (void)s4g_ememctr_write( u2g_EMFNG_EOCJMFNEEMH_U1_ID, (void *)&u1t_ecjmfneemh );       /* 戻り値は使用しない */
    }
}
#endif /* JEOOBD */

/*********************************************************************/
/*  関数名        | emfng_cntclr_65msl( )                            */
/*  処理内容      | 外部参照失火ｶｳﾝﾀのｸﾘｱ処理                        */
/*  制御タイミング| 65ms毎(emfcnt_65mslよりコール)                   */
/*  引数          | なし                                             */
/*  戻り値        | なし                                             */
/*  注意事項      |                                                  */
/*********************************************************************/
void
emfng_cntclr_65msl( void )
{
    s4 s4t_i;           /* lsb=1 :forループ用変数 */
    u1 u1t_ncyl_c;      /* lsb=1 :気筒数 */
    u1 u1t_ncylw_c;     /* lsb=1 :気筒群数 */

    /* ワークデータ初期化 */
    u1t_ncyl_c = u1g_ejcc_NCYL;
    /***************/
    /* ｶｳﾝﾀｸﾘｱ処理 */
    /***************/
    glint_di(); /* ▼▼▼ 割り込み禁止 ▼▼▼ */
    if ( big_emfng_excdmfemclr == (u1)ON )      /* ecdmfem[]ｸﾘｱ要求有り */
    {
        for ( s4t_i = (s4)0; s4t_i < (s4)u1t_ncyl_c; s4t_i++ )
        {
            u1g_emfng_ecdmfem[s4t_i] = (u1)0U;
        }
    }
    if ( bis_emfng_excdmfem2clr == (u1)ON )     /* ecdmfem2[]ｸﾘｱ要求有り */
    {
        for ( s4t_i = (s4)0; s4t_i < (s4)u1t_ncyl_c; s4t_i++ )
        {
            u2g_emfng_ecdmfem2[s4t_i] = (u2)0U;
        }
    }
    if ( bis_emfng_excdmfwclr == (u1)ON )       /* ecdmfw[]ｸﾘｱ要求有り */
    {
        u1t_ncylw_c = u1s_EMFNG_NCYLW;
        for ( s4t_i = (s4)0; s4t_i < (s4)u1t_ncylw_c; s4t_i++ )
        {
            u1g_emfng_ecdmfw[s4t_i] = (u1)0U;
        }
    }

    if ( bis_emfng_excdmfae2clr == (u1)ON )     /* ecdmfae2ｸﾘｱ要求有り */
    {
        u2g_emfng_ecdmfae2 = (u2)0U;
    }

    glint_ei(); /* ▲▲▲ 割り込み許可 ▲▲▲ */
}

/*********************************************************************/
/*  関数名        | u2g_emfng_eocmfnl( )                             */
/*  処理内容      | 市場調査RAM 失火回数(小)の頻度 取得関数          */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | なし                                             */
/*  戻り値        | 失火回数(小)の頻度 lsb=1,unit=回                 */
/*  注意事項      | 【市場調査無】時は0を返す                        */
/*********************************************************************/
u2
u2g_emfng_eocmfnl( void )
{
    u2 u2t_eocmfnl;     /* lsb=1,unit=回 :失火回数(小)の頻度 */

    u2t_eocmfnl = (u2)0U;  /*【市場調査無】時は0固定 */
#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
    (void)s4g_ememctr_read( u2g_EMFNG_EOCMFNL_U2_ID, (void *)&u2t_eocmfnl );  /* 戻り値は使用しない */
#endif
    return( u2t_eocmfnl );
}

/*********************************************************************/
/*  関数名        | u2g_emfng_eocmfnm( )                             */
/*  処理内容      | 市場調査RAM 失火回数(中)の頻度 取得関数          */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | なし                                             */
/*  戻り値        | 失火回数(中)の頻度 lsb=1,unit=回                 */
/*  注意事項      | 【市場調査無】時は0を返す                        */
/*********************************************************************/
u2
u2g_emfng_eocmfnm( void )
{
    u2 u2t_eocmfnm;     /* lsb=1,unit=回 :失火回数(中)の頻度 */

    u2t_eocmfnm = (u2)0U;  /*【市場調査無】時は0固定 */
#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
    (void)s4g_ememctr_read( u2g_EMFNG_EOCMFNM_U2_ID, (void *)&u2t_eocmfnm );  /* 戻り値は使用しない */
#endif
    return( u2t_eocmfnm );
}

/*********************************************************************/
/*  関数名        | u2g_emfng_eocmfnh( )                             */
/*  処理内容      | 市場調査RAM 失火回数(大)の頻度 取得関数          */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | なし                                             */
/*  戻り値        | 失火回数(大)の頻度 lsb=1,unit=回                 */
/*  注意事項      | 【市場調査無】時は0を返す                        */
/*********************************************************************/
u2
u2g_emfng_eocmfnh( void )
{
    u2 u2t_eocmfnh;     /* lsb=1,unit=回 :失火回数(大)の頻度 */

    u2t_eocmfnh = (u2)0U;  /*【市場調査無】時は0固定 */
#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
    (void)s4g_ememctr_read( u2g_EMFNG_EOCMFNH_U2_ID, (void *)&u2t_eocmfnh );  /* 戻り値は使用しない */
#endif
    return( u2t_eocmfnh );
}

/*********************************************************************/
/*  関数名        | u2g_emfng_eocmfmltl( )                           */
/*  処理内容      | 市場調査RAM 対向気筒失火回数(小)の頻度 取得関数  */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | なし                                             */
/*  戻り値        | 対向気筒失火回数(小)の頻度 lsb=1,unit=回         */
/*  注意事項      | 【市場調査無】時は0を返す                        */
/*********************************************************************/
u2
u2g_emfng_eocmfmltl( void )
{
    u2 u2t_eocmfmltl;   /* lsb=1,unit=回 :対向気筒失火回数(小)の頻度 */

    u2t_eocmfmltl = (u2)0U;  /*【市場調査無】時は0固定 */
#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
    (void)s4g_ememctr_read( u2g_EMFNG_EOCMFMLTL_U2_ID, (void *)&u2t_eocmfmltl );  /* 戻り値は使用しない */
#endif
    return( u2t_eocmfmltl );
}

/*********************************************************************/
/*  関数名        | u2g_emfng_eocmfmlth( )                           */
/*  処理内容      | 市場調査RAM 対向気筒失火回数(大)の頻度 取得関数  */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | なし                                             */
/*  戻り値        | 対向気筒失火回数(大)の頻度 lsb=1,unit=回         */
/*  注意事項      | 【市場調査無】時は0を返す                        */
/*********************************************************************/
u2
u2g_emfng_eocmfmlth( void )
{
    u2 u2t_eocmfmlth;   /* lsb=1,unit=回 :対向気筒失火回数(大)の頻度 */

    u2t_eocmfmlth = (u2)0U;  /*【市場調査無】時は0固定 */
#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
    (void)s4g_ememctr_read( u2g_EMFNG_EOCMFMLTH_U2_ID, (void *)&u2t_eocmfmlth );  /* 戻り値は使用しない */
#endif
    return( u2t_eocmfmlth );
}

/*********************************************************************/
/*  関数名        | u1g_emfng_eonemfmlt( )                           */
/*  処理内容      | 市場調査RAM 対向気筒失火時のｴﾝｼﾞﾝ回転なまし値 取得関数 */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | なし                                             */
/*  戻り値        | 対向気筒失火時のｴﾝｼﾞﾝ回転なまし値                */
/*                | lsb=200/8, unit=rpm                              */
/*  注意事項      | 【市場調査無】時は0を返す                        */
/*********************************************************************/
u1
u1g_emfng_eonemfmlt( void )
{
    u1 u1t_eonemfmlt;   /* lsb=200/8,unit=rpm :対向気筒失火時のｴﾝｼﾞﾝ回転なまし値 */

    u1t_eonemfmlt = (u1)0U;  /*【市場調査無】時は0固定 */
#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
    (void)s4g_ememctr_read( u2g_EMFNG_EONEMFMLT_U1_ID, (void *)&u1t_eonemfmlt );  /* 戻り値は使用しない */
#endif
    return( u1t_eonemfmlt );        /* lsb=200/8,unit=rpm */
}

/*********************************************************************/
/*  関数名        | u2g_emfng_ecdmfaemTv( )                          */
/*  処理内容      | ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(全気筒)取得関数 */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | なし                                             */
/*  戻り値        | ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(全気筒)         */
/*                |                        lsb=1,unit=回             */
/*  注意事項      |  スケーリングID =24(ISO 15031-5)                 */
/*********************************************************************/
u2
u2g_emfng_ecdmfaemTv( void )
{
    u2 u2t_ecdmfaemTv;   /* lsb=1,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(全気筒) */

    u2t_ecdmfaemTv = (u2)0U;  /* ダミー設定 */
    (void)s4g_ememctr_read( u2g_EMFNG_ECDMFAEMTV_U2_ID, (void *)&u2t_ecdmfaemTv );  /* 戻り値は使用しない */

    return( u2t_ecdmfaemTv );
}

/*********************************************************************/
/*  関数名        | u2g_emfng_ecdmfaemMntl( )                        */
/*  処理内容      | ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ下限値(全気筒)取得関数 */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | なし                                             */
/*  戻り値        | ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ下限値(全気筒)   */
/*                |                        lsb=1,unit=回             */
/*  注意事項      |  スケーリングID =24(ISO 15031-5)                 */
/*********************************************************************/
u2
u2g_emfng_ecdmfaemMntl( void )
{
    u2 u2t_ecdmfaemMntl;   /* lsb=1,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ下限値(全気筒) */

    u2t_ecdmfaemMntl = (u2)0U;  /* ダミー設定 */
    (void)s4g_ememctr_read( u2g_EMFNG_ECDMFAEMMNTL_U2_ID, (void *)&u2t_ecdmfaemMntl );  /* 戻り値は使用しない */

    return( u2t_ecdmfaemMntl );
}

/*********************************************************************/
/*  関数名        | u2g_emfng_ecdmfaemMxtl( )                        */
/*  処理内容      | ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ上限値(全気筒)取得関数 */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | なし                                             */
/*  戻り値        | ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ上限値(全気筒)   */
/*                |                        lsb=1,unit=回             */
/*  注意事項      |  スケーリングID =24(ISO 15031-5)                 */
/*********************************************************************/
u2
u2g_emfng_ecdmfaemMxtl( void )
{
    u2 u2t_ecdmfaemMxtl;   /* lsb=1,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ上限値(全気筒) */

    u2t_ecdmfaemMxtl = (u2)0U;  /* ダミー設定 */
    (void)s4g_ememctr_read( u2g_EMFNG_ECDMFAEMMXTL_U2_ID, (void *)&u2t_ecdmfaemMxtl );  /* 戻り値は使用しない */

    return( u2t_ecdmfaemMxtl );
}

/*********************************************************************/
/*  関数名        | u2g_emfng_ecdmfemTv( )                           */
/*  処理内容      | ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(気筒別)取得関数 */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | u1t_cyl  :lsb=1 気筒                             */
/*  戻り値        | ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(気筒別)         */
/*                |                        lsb=1,unit=回             */
/*  注意事項      |  スケーリングID =24(ISO 15031-5)                 */
/*********************************************************************/
u2
u2g_emfng_ecdmfemTv( u1 u1t_cyl )
{
    u2 u2t_ecdmfemTv;    /* lsb=1,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(気筒別) */

    u2t_ecdmfemTv = u2g_emfng_ECDMFEMTV_MI;
    if ( u1t_cyl < u1g_ejcc_NCYL )
    {
        (void)s4g_ememctr_read( u2s_ECDMFEMTV_U2_ID[u1t_cyl], (void *)&u2t_ecdmfemTv );  /* 戻り値は使用しない */
    }

    return( u2t_ecdmfemTv );
}

/*********************************************************************/
/*  関数名        | u2g_emfng_ecdmfemMntl( )                         */
/*  処理内容      | ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ下限値(気筒別)取得関数 */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | u1t_cyl   :lsb=1 気筒                            */
/*  戻り値        | ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ下限値(気筒別)   */
/*                |                        lsb=1,unit=回             */
/*  注意事項      |  スケーリングID =24(ISO 15031-5)                 */
/*********************************************************************/
u2
u2g_emfng_ecdmfemMntl( u1 u1t_cyl )
{
    u2 u2t_ecdmfemMntl;    /* lsb=1,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ下限値(気筒別) */

    u2t_ecdmfemMntl = u2g_emfng_ECDMFEMMNTL_MI;
    if ( u1t_cyl < u1g_ejcc_NCYL )
    {
        (void)s4g_ememctr_read( u2s_ECDMFEMMNTL_U2_ID[u1t_cyl], (void *)&u2t_ecdmfemMntl );  /* 戻り値は使用しない */
    }

    return( u2t_ecdmfemMntl );
}

/*********************************************************************/
/*  関数名        | u2g_emfng_ecdmfemMxtl( )                         */
/*  処理内容      | ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ上限値(気筒別)取得関数 */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | u1t_cyl   :lsb=1 気筒                            */
/*  戻り値        | ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ上限値(気筒別)   */
/*                |                        lsb=1,unit=回             */
/*  注意事項      |  スケーリングID =24(ISO 15031-5)                 */
/*********************************************************************/
u2
u2g_emfng_ecdmfemMxtl( u1 u1t_cyl )
{
    u2 u2t_ecdmfemMxtl;    /* lsb=1,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ上限値(気筒別) */

    u2t_ecdmfemMxtl = u2g_emfng_ECDMFEMMXTL_MI;
    if ( u1t_cyl < u1g_ejcc_NCYL )
    {
        (void)s4g_ememctr_read( u2s_ECDMFEMMXTL_U2_ID[u1t_cyl], (void *)&u2t_ecdmfemMxtl );  /* 戻り値は使用しない */
    }

    return( u2t_ecdmfemMxtl );
}

/*********************************************************************/
/*  関数名        | u2g_emfng_ecdmfaavTv( )                          */
/*  処理内容      | ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ(全気筒)取得関数 */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | なし                                             */
/*  戻り値        | ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ(全気筒)     */
/*                |                        lsb=1,unit=回             */
/*  注意事項      |  スケーリングID =24(ISO 15031-5)                 */
/*********************************************************************/
u2
u2g_emfng_ecdmfaavTv( void )
{
    u2 u2t_ecdmfaavTv;   /* lsb=1,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ(全気筒) */

    u2t_ecdmfaavTv = (u2)0U;  /* ダミー設定 */
    (void)s4g_ememctr_read( u2g_EMFNG_ECDMFAAVTV_U2_ID, (void *)&u2t_ecdmfaavTv );  /* 戻り値は使用しない */

    return( u2t_ecdmfaavTv );
}

/*********************************************************************/
/*  関数名        | u2g_emfng_ecdmfaavMntl( )                        */
/*  処理内容      | ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ下限値(全気筒)取得関数 */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | なし                                             */
/*  戻り値        | ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ下限値(全気筒) */
/*                |                        lsb=1,unit=回             */
/*  注意事項      |  スケーリングID =24(ISO 15031-5)                 */
/*********************************************************************/
u2
u2g_emfng_ecdmfaavMntl( void )
{
    u2 u2t_ecdmfaavMntl;   /* lsb=1,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ下限値(全気筒) */

    u2t_ecdmfaavMntl = (u2)0U;  /* ダミー設定 */
    (void)s4g_ememctr_read( u2g_EMFNG_ECDMFAAVMNTL_U2_ID, (void *)&u2t_ecdmfaavMntl );  /* 戻り値は使用しない */

    return( u2t_ecdmfaavMntl );
}

/*********************************************************************/
/*  関数名        | u2g_emfng_ecdmfaavMxtl( )                        */
/*  処理内容      | ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ上限値(全気筒)取得関数 */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | なし                                             */
/*  戻り値        | ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ上限値(全気筒) */
/*                |                        lsb=1,unit=回             */
/*  注意事項      |  スケーリングID =24(ISO 15031-5)                 */
/*********************************************************************/
u2
u2g_emfng_ecdmfaavMxtl( void )
{
    u2 u2t_ecdmfaavMxtl;   /* lsb=1,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ上限値(全気筒) */

    u2t_ecdmfaavMxtl = (u2)0U;  /* ダミー設定 */
    (void)s4g_ememctr_read( u2g_EMFNG_ECDMFAAVMXTL_U2_ID, (void *)&u2t_ecdmfaavMxtl );  /* 戻り値は使用しない */

    return( u2t_ecdmfaavMxtl );
}

/*********************************************************************/
/*  関数名        | u2g_emfng_ecdmfavTv( )                           */
/*  処理内容      | ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ(気筒別)取得関数 */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | u1t_cyl   :lsb=1 気筒                            */
/*  戻り値        | ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ(気筒別)     */
/*                |                        lsb=1,unit=回             */
/*  注意事項      |  スケーリングID =24(ISO 15031-5)                 */
/*********************************************************************/
u2
u2g_emfng_ecdmfavTv( u1 u1t_cyl )
{
    u2 u2t_ecdmfavTv;    /* lsb=1,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ(気筒別) */

    u2t_ecdmfavTv = u2g_emfng_ECDMFAVTV_MI;
    if ( u1t_cyl < u1g_ejcc_NCYL )
    {
        (void)s4g_ememctr_read( u2s_ECDMFAVTV_U2_ID[u1t_cyl], (void *)&u2t_ecdmfavTv );  /* 戻り値は使用しない */
    }

    return( u2t_ecdmfavTv );
}

/*********************************************************************/
/*  関数名        | u2g_emfng_ecdmfavMntl( )                         */
/*  処理内容      | ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ下限値(気筒別)取得関数 */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | u1t_cyl   :lsb=1 気筒                            */
/*  戻り値        | ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ下限値(気筒別) */
/*                |                        lsb=1,unit=回             */
/*  注意事項      |  スケーリングID =24(ISO 15031-5)                 */
/*********************************************************************/
u2
u2g_emfng_ecdmfavMntl( u1 u1t_cyl )
{
    u2 u2t_ecdmfavMntl;    /* lsb=1,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ下限値(気筒別) */

    u2t_ecdmfavMntl = u2g_emfng_ECDMFAVMNTL_MI;
    if ( u1t_cyl < u1g_ejcc_NCYL )
    {
        (void)s4g_ememctr_read( u2s_ECDMFAVMNTL_U2_ID[u1t_cyl], (void *)&u2t_ecdmfavMntl );  /* 戻り値は使用しない */
    }

    return( u2t_ecdmfavMntl );
}

/*********************************************************************/
/*  関数名        | u2g_emfng_ecdmfavMxtl( )                         */
/*  処理内容      | ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ上限値(気筒別)取得関数 */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | u1t_cyl   :lsb=1 気筒                            */
/*  戻り値        | ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ上限値(気筒別) */
/*                |                        lsb=1,unit=回             */
/*  注意事項      |  スケーリングID =24(ISO 15031-5)                 */
/*********************************************************************/
u2
u2g_emfng_ecdmfavMxtl( u1 u1t_cyl )
{
    u2 u2t_ecdmfavMxtl;    /* lsb=1,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ上限値(気筒別) */

    u2t_ecdmfavMxtl = u2g_emfng_ECDMFAVMXTL_MI;
    if ( u1t_cyl < u1g_ejcc_NCYL )
    {
        (void)s4g_ememctr_read( u2s_ECDMFAVMXTL_U2_ID[u1t_cyl], (void *)&u2t_ecdmfavMxtl );  /* 戻り値は使用しない */
    }

    return( u2t_ecdmfavMxtl );
}

/*********************************************************************/
/*  関数名        | u1s_emfng_exrcdmf( )                             */
/*  処理内容      | EWMA失火ｶｳﾝﾀ計算履歴ﾌﾗｸﾞ取得処理                 */
/*  制御タイミング| emfng_65msl( )                                   */
/*  引数          | なし                                             */
/*  戻り値        | EWMA失火ｶｳﾝﾀ計算履歴ﾌﾗｸﾞ                         */
/*  注意事項      | 内部関数                                         */
/*********************************************************************/
static u1
u1s_emfng_exrcdmf( void )
{
    u1 u1t_exrcdmf;     /* lsb=1 :EWMA失火ｶｳﾝﾀ計算履歴ﾌﾗｸﾞ */

    u1t_exrcdmf = (u1)0U;  /* ダミー設定 */
    (void)s4g_ememctr_read( u2g_EMFNG_EXRCDMF_U1_ID, (void *)&u1t_exrcdmf );  /* 戻り値は使用しない */

    return( u1t_exrcdmf );
}

/*********************************************************************/
/*  関数名        | u1g_emfng_eocdmfae2mx( )                         */
/*  処理内容      | 市場調査RAM 1000rev毎の最大総失火回数 取得関数   */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | なし                                             */
/*  戻り値        | 1000rev毎の最大総失火回数 lsb=1,unit=回          */
/*  注意事項      | 【市場調査無】時は0を返す                        */
/*********************************************************************/
u1
u1g_emfng_eocdmfae2mx( void )
{
    u1  u1t_eocdmfae2mx;    /* lsb=1,unit=回 :1000rev毎の最大総失火回数 */

    u1t_eocdmfae2mx = (u1)0U;  /*【市場調査無】時は0固定 */
#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
    (void)s4g_ememctr_read( u2g_EMFNG_EOCDMFAE2MX_U1_ID, (void *)&u1t_eocdmfae2mx );  /* 戻り値は使用しない */
#endif
    return( u1t_eocdmfae2mx );
}

/*********************************************************************/
/*  関数名        | u1g_emfng_eocjrevem( )                           */
/*  処理内容      | 市場調査RAM 1000rev毎の始動後1000rev回数 取得関数 */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | なし                                             */
/*  戻り値        | 1000rev毎の始動後1000rev回数 lsb=1,unit=回       */
/*  注意事項      | 【市場調査無】時は0を返す                        */
/*********************************************************************/
u1
u1g_emfng_eocjrevem( void )
{
    u1 u1t_eocjrevem;   /* lsb=1,unit=回 :1000rev毎の始動後1000rev回数 */

    u1t_eocjrevem = (u1)0U;  /*【市場調査無】時は0固定 */
#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
    (void)s4g_ememctr_read( u2g_EMFNG_EOCJREVEM_U1_ID, (void *)&u1t_eocjrevem );  /* 戻り値は使用しない */
#endif
    return( u1t_eocjrevem );
}

/*********************************************************************/
/*  関数名        | s1g_emfng_eothwem( )                             */
/*  処理内容      | 市場調査RAM 1000rev毎の水温 取得関数             */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | なし                                             */
/*  戻り値        | 1000rev毎の水温 lsb=160/128,unit=℃              */
/*  注意事項      | 【市場調査無】時は0を返す                        */
/*********************************************************************/
s1
s1g_emfng_eothwem( void )
{
    s1 s1t_eothwem;     /* lsb=160/128,unit=℃ :1000rev毎の水温 */

    s1t_eothwem = (s1)0;  /*【市場調査無】時は0固定 */
#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
    (void)s4g_ememctr_read( u2g_EMFNG_EOTHWEM_S1_ID, (void *)&s1t_eothwem );  /* 戻り値は使用しない */
#endif
    return( s1t_eothwem );
}

/*********************************************************************/
/*  関数名        | s1g_emfng_eothaem( )                             */
/*  処理内容      | 市場調査RAM 1000rev毎の吸気温 取得関数           */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | なし                                             */
/*  戻り値        | 1000rev毎の吸気温 lsb=160/128,unit=℃            */
/*  注意事項      | 【市場調査無】時は0を返す                        */
/*********************************************************************/
s1
s1g_emfng_eothaem( void )
{
    s1 s1t_eothaem;     /* lsb=160/128,unit=℃ :1000rev毎の吸気温 */

    s1t_eothaem = (s1)0;  /*【市場調査無】時は0固定 */
#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
    (void)s4g_ememctr_read( u2g_EMFNG_EOTHAEM_S1_ID, (void *)&s1t_eothaem );  /* 戻り値は使用しない */
#endif
    return( s1t_eothaem );
}

/*********************************************************************/
/*  関数名        | u1g_emfng_eocdmfem2mx( )                         */
/*  処理内容      | 市場調査RAM 1000rev毎の気筒別失火回数 取得関数   */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | u1t_cyl   :lsb=1 気筒                            */
/*  戻り値        | 1000rev毎の気筒別失火回数 lsb=1,unit=回          */
/*  注意事項      | 【市場調査無】時は0を返す                        */
/*********************************************************************/
u1
u1g_emfng_eocdmfem2mx( u1 u1t_cyl )
{
    u1 u1t_eocdmfem2mx;     /* lsb=1,unit=回 :1000rev毎の気筒別失火回数 */

#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
    u1t_eocdmfem2mx = u1g_emfng_EOCDMFEM2MX_MI;
    if ( u1t_cyl < u1g_ejcc_NCYL )
    {
        (void)s4g_ememctr_read( u2s_EOCDMFEM2MX_U1_ID[u1t_cyl], (void *)&u1t_eocdmfem2mx );  /* 戻り値は使用しない */
    }
#else                             /*【市場調査無】*/
    u1t_eocdmfem2mx = (u1)0U;  /*【市場調査無】時は0固定 */
#endif
    return( u1t_eocdmfem2mx );
}

/*********************************************************************/
/*  関数名        | u1g_emfng_eospdemav( )                           */
/*  処理内容      | 市場調査RAM 失火ｶｳﾝﾄ時の平均車速 取得関数        */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | なし                                             */
/*  戻り値        | 失火ｶｳﾝﾄ時の平均車速 lsb=256/256,unit=km/h       */
/*  注意事項      | 【市場調査無】時は0を返す                        */
/*********************************************************************/
u1
u1g_emfng_eospdemav( void )
{
    u1 u1t_eospdemav;   /* lsb=256/256,unit=km/h :失火ｶｳﾝﾄ時の平均車速 */

    u1t_eospdemav = (u1)0U;  /*【市場調査無】時は0固定 */
#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
    (void)s4g_ememctr_read( u2g_EMFNG_EOSPDEMAV_U1_ID, (void *)&u1t_eospdemav );  /* 戻り値は使用しない */
#endif
    return( u1t_eospdemav );
}

/*********************************************************************/
/*  関数名        | u1g_emfng_eoneemav( )                            */
/*  処理内容      | 市場調査RAM 失火ｶｳﾝﾄ時の平均ｴﾝｼﾞﾝ回転数 取得関数 */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | なし                                             */
/*  戻り値        | 失火ｶｳﾝﾄ時の平均ｴﾝｼﾞﾝ回転数 lsb=12800/256,unit=rpm */
/*  注意事項      | 【市場調査無】時は0を返す                        */
/*********************************************************************/
u1
u1g_emfng_eoneemav( void )
{
    u1 u1t_eoneemav;    /* lsb=12800/256,unit=rpm :失火ｶｳﾝﾄ時の平均ｴﾝｼﾞﾝ回転数 */

    u1t_eoneemav = (u1)0U;  /*【市場調査無】時は0固定 */
#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
    (void)s4g_ememctr_read( u2g_EMFNG_EONEEMAV_U1_ID, (void *)&u1t_eoneemav );  /* 戻り値は使用しない */
#endif
    return( u1t_eoneemav );
}

/*********************************************************************/
/*  関数名        | u1g_emfng_eoklsmemav( )                          */
/*  処理内容      | 市場調査RAM 失火ｶｳﾝﾄ時の平均負荷 取得関数        */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | なし                                             */
/*  戻り値        | 失火ｶｳﾝﾄ時の平均負荷 lsb=320/256,unit=%          */
/*  注意事項      | 【市場調査無】時は0を返す                        */
/*********************************************************************/
u1
u1g_emfng_eoklsmemav( void )
{
    u1 u1t_eoklsmemav;      /* lsb=320/256,unit=% :失火ｶｳﾝﾄ時の平均負荷 */

    u1t_eoklsmemav = (u1)0U;  /*【市場調査無】時は0固定 */
#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
    (void)s4g_ememctr_read( u2g_EMFNG_EOKLSMEMAV_U1_ID, (void *)&u1t_eoklsmemav );  /* 戻り値は使用しない */
#endif
    return( u1t_eoklsmemav );
}

/*********************************************************************/
/*  関数名        | u1g_emfng_eocjmfneemi( )                         */
/*  処理内容      | 市場調査RAM 失火ｶｳﾝﾄ時のｱｲﾄﾞﾙﾓｰﾄﾞ回数 取得関数   */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | なし                                             */
/*  戻り値        | 失火ｶｳﾝﾄ時のｱｲﾄﾞﾙﾓｰﾄﾞ回数 lsb=1,unit=回          */
/*  注意事項      | 【市場調査無】時は0を返す                        */
/*********************************************************************/
u1
u1g_emfng_eocjmfneemi( void )
{
    u1 u1t_eocjmfneemi;     /* lsb=1,unit=回 :失火ｶｳﾝﾄ時のｱｲﾄﾞﾙﾓｰﾄﾞ回数 */

    u1t_eocjmfneemi = (u1)0U;  /*【市場調査無】時は0固定 */
#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
    (void)s4g_ememctr_read( u2g_EMFNG_EOCJMFNEEMI_U1_ID, (void *)&u1t_eocjmfneemi );  /* 戻り値は使用しない */
#endif
    return( u1t_eocjmfneemi );
}

/*********************************************************************/
/*  関数名        | u1g_emfng_eocjmfneeml( )                         */
/*  処理内容      | 市場調査RAM 失火ｶｳﾝﾄ時のﾛｰﾓｰﾄﾞ回数 取得関数      */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | なし                                             */
/*  戻り値        | 失火ｶｳﾝﾄ時のﾛｰﾓｰﾄﾞ回数 lsb=1,unit=回             */
/*  注意事項      | 【市場調査無】時は0を返す                        */
/*********************************************************************/
u1
u1g_emfng_eocjmfneeml( void )
{
    u1 u1t_eocjmfneeml;     /* lsb=1,unit=回 :失火ｶｳﾝﾄ時のﾛｰﾓｰﾄﾞ回数 */

    u1t_eocjmfneeml = (u1)0U;  /*【市場調査無】時は0固定 */
#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
    (void)s4g_ememctr_read( u2g_EMFNG_EOCJMFNEEML_U1_ID, (void *)&u1t_eocjmfneeml );  /* 戻り値は使用しない */
#endif
    return( u1t_eocjmfneeml );
}

/*********************************************************************/
/*  関数名        | u1g_emfng_eocjmfneemh( )                         */
/*  処理内容      | 市場調査RAM 失火ｶｳﾝﾄ時のﾊｲﾓｰﾄﾞ回数 取得関数      */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | なし                                             */
/*  戻り値        | 失火ｶｳﾝﾄ時のﾊｲﾓｰﾄﾞ回数 lsb=1,unit=回             */
/*  注意事項      | 【市場調査無】時は0を返す                        */
/*********************************************************************/
u1
u1g_emfng_eocjmfneemh( void )
{
    u1 u1t_eocjmfneemh;     /* lsb=1,unit=回 :失火ｶｳﾝﾄ時のﾊｲﾓｰﾄﾞ回数 */

    u1t_eocjmfneemh = (u1)0U;  /*【市場調査無】時は0固定 */
#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
    (void)s4g_ememctr_read( u2g_EMFNG_EOCJMFNEEMH_U1_ID, (void *)&u1t_eocjmfneemh );  /* 戻り値は使用しない */
#endif
    return( u1t_eocjmfneemh );
}

/*********************************************************************/
/*  関数名        | u1g_emfng_eocdmfaotmx( )                         */
/*  処理内容      | 市場調査RAM 200rev中失火ｶｳﾝﾀの最大値 取得関数    */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | なし                                             */
/*  戻り値        | 200rev中失火ｶｳﾝﾀの最大値 lsb=1,unit=回           */
/*  注意事項      | 【市場調査無】時は0を返す                        */
/*********************************************************************/
u1
u1g_emfng_eocdmfaotmx( void )
{
    u1 u1t_eocdmfaotmx;     /* lsb=1,unit=回 :200rev中失火ｶｳﾝﾀの最大値 */

    u1t_eocdmfaotmx = (u1)0U;  /*【市場調査無】時は0固定 */
#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
    (void)s4g_ememctr_read( u2g_EMFNG_EOCDMFAOTMX_U1_ID, (void *)&u1t_eocdmfaotmx );  /* 戻り値は使用しない */
#endif
    return( u1t_eocdmfaotmx );
}

/*********************************************************************/
/*  関数名        | u1g_emfng_eocjrevot( )                           */
/*  処理内容      | 市場調査RAM 最大失火ｶｳﾝﾀ更新時の1000rev回数 取得関数 */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | なし                                             */
/*  戻り値        | 最大失火ｶｳﾝﾀ更新時の1000rev回数 lsb=1,unit=回    */
/*  注意事項      | 【市場調査無】時は0を返す                        */
/*********************************************************************/
u1
u1g_emfng_eocjrevot( void )
{
    u1 u1t_eocjrevot;       /* lsb=1,unit=回 :最大失火ｶｳﾝﾀ更新時の1000rev回数 */

    u1t_eocjrevot = (u1)0U;  /*【市場調査無】時は0固定 */
#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
    (void)s4g_ememctr_read( u2g_EMFNG_EOCJREVOT_U1_ID, (void *)&u1t_eocjrevot );  /* 戻り値は使用しない */
#endif
    return( u1t_eocjrevot );
}

/*********************************************************************/
/*  関数名        | s1g_emfng_eothwot( )                             */
/*  処理内容      | 市場調査RAM 最大失火ｶｳﾝﾀ更新時の水温 取得関数    */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | なし                                             */
/*  戻り値        | 最大失火ｶｳﾝﾀ更新時の水温 lsb=160/128,unit=℃     */
/*  注意事項      | 【市場調査無】時は0を返す                        */
/*********************************************************************/
s1
s1g_emfng_eothwot( void )
{
    s1 s1t_eothwot;     /* lsb=160/128,unit=℃ :最大失火ｶｳﾝﾀ更新時の水温 */

    s1t_eothwot = (s1)0;  /*【市場調査無】時は0固定 */
#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
    (void)s4g_ememctr_read( u2g_EMFNG_EOTHWOT_S1_ID, (void *)&s1t_eothwot );  /* 戻り値は使用しない */
#endif
    return( s1t_eothwot );
}

/*********************************************************************/
/*  関数名        | s1g_emfng_eothaot( )                             */
/*  処理内容      | 市場調査RAM 最大失火ｶｳﾝﾀ更新時の吸気温 取得関数  */
/*  制御タイミング| 処理要求時                                       */
/*  引数          | なし                                             */
/*  戻り値        | 最大失火ｶｳﾝﾀ更新時の吸気温 lsb=160/128,unit=℃   */
/*  注意事項      | 【市場調査無】時は0を返す                        */
/*********************************************************************/
s1
s1g_emfng_eothaot( void )
{
    s1 s1t_eothaot;     /* lsb=160/128,unit=℃ :最大失火ｶｳﾝﾀ更新時の吸気温 */

    s1t_eothaot = (s1)0;  /*【市場調査無】時は0固定 */
#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
    (void)s4g_ememctr_read( u2g_EMFNG_EOTHAOT_S1_ID, (void *)&s1t_eothaot );  /* 戻り値は使用しない */
#endif
    return( s1t_eothaot );
}

/*********************************************************************/
/*  関数名        | u1s_emfng_wxreqclr( )                            */
/*  処理内容      | 市場調査RAM消去処理                              */
/*  制御タイミング| 65ms毎(emfng_65msl()よりコール)                  */
/*  引数          | なし                                             */
/*  戻り値        | ダイアグからの消去要求(ON:消去要求有り)          */
/*  注意事項      | 内部関数                                         */
/*********************************************************************/
static u1
u1s_emfng_wxreqclr( void )
{
    u1 u1t_xreqclr;
    s4 s4t_i;
#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
    u2 u2t_EOCMFMLTH_MI_c;        /* lsb=1,unit=回 :対向気筒失火回数(大)の頻度 初期値 */
    u2 u2t_EOCMFMLTL_MI_c;        /* lsb=1,unit=回 :対向気筒失火回数(小)の頻度 初期値 */
    u1 u1t_EONEMFMLT_MI_c;        /* lsb=200/8,unit=rpm :対向気筒失火時のエンジン回転なまし値 初期値 */
    u2 u2t_EOCMFNH_MI_c;          /* lsb=1,unit=回 :失火回数(大)の頻度 初期値 */
    u2 u2t_EOCMFNM_MI_c;          /* lsb=1,unit=回 :失火回数(中)の頻度 初期値 */
    u2 u2t_EOCMFNL_MI_c;          /* lsb=1,unit=回 :失火回数(小)の頻度 初期値 */
    u1 u1t_EOCDMFAE2MX_MI_c;      /* lsb=1,unit=回 :1000rev中失火カウンタの最大値 初期値 */
    u1 u1t_EOCJREVEM_MI_c;        /* lsb=1,unit=回 :最大失火カウンタ更新時の1000rev回数 初期値 */
    s1 s1t_EOTHWEM_MI_c;          /* lsb=160/128,unit=℃ :最大失火カウンタ更新時の水温 初期値 */
    s1 s1t_EOTHAEM_MI_c;          /* lsb=160/128,unit=℃ :最大失火カウンタ更新時の吸気温 初期値 */
    u1 u1t_EOCDMFEM2MX_MI_c;      /* lsb=1,unit=回 :最大失火カウンタ更新時の気筒別失火カウンタ 初期値 */
    u1 u1t_EOSPDEMAV_MI_c;        /* lsb=256/256,unit=km/h :失火カウント時の平均車速 初期値 */
    u1 u1t_EONEEMAV_MI_c;         /* lsb=12800/256,unit=rpm :失火カウント時の平均エンジン回転数 初期値 */
    u1 u1t_EOKLSMEMAV_MI_c;       /* lsb=320/256,unit=% :失火カウント時の平均負荷 初期値 */
    u1 u1t_EOCJMFNEEMI_MI_c;      /* lsb=1,unit=回 :失火カウント時のアイドルモード回数 初期値 */
    u1 u1t_EOCJMFNEEML_MI_c;      /* lsb=1,unit=回 :失火カウント時のローモード回数 初期値 */
    u1 u1t_EOCJMFNEEMH_MI_c;      /* lsb=1,unit=回 :失火カウント時のハイモード回数 初期値 */
    u1 u1t_EOCDMFAOTMX_MI_c;      /* lsb=1,unit=回 :200rev中最大カウンタ最大値  初期値 */
    u1 u1t_EOCJREVOT_MI_c;        /* lsb=1,unit=回 :最大失火カウンタ更新時の1000rev回数 初期値 */
    s1 s1t_EOTHWOT_MI_c;          /* lsb=160/128,unit=℃ :最大失火カウンタ更新時の水温 初期値 */
    s1 s1t_EOTHAOT_MI_c;          /* lsb=160/128,unit=℃ :最大失火カウンタ更新時の吸気温 初期値 */
#endif
    u2 u2t_ECDMFAEMTV_MI_c;       /* lsb=1,unit=回 :テスト結果出力用今トリップ累積失火カウンタ(全気筒)初期値 */
    u2 u2t_ECDMFAEMMNTL_MI_c;     /* lsb=1,unit=回 :テスト結果出力用今トリップ累積失火カウンタ下限値(全気筒)初期値 */
    u2 u2t_ECDMFAEMMXTL_MI_c;     /* lsb=1,unit=回 :テスト結果出力用今トリップ累積失火カウンタ上限値(全気筒)初期値 */
    u2 u2t_ECDMFAAVTV_MI_c;       /* lsb=1,unit=回 :テスト結果出力用過去10トリップEWMA失火カウンタ(全気筒)初期値 */
    u2 u2t_ECDMFAAVMNTL_MI_c;     /* lsb=1,unit=回 :テスト結果出力用過去10トリップEWMA失火カウンタ下限値(全気筒)初期値 */
    u2 u2t_ECDMFAAVMXTL_MI_c;     /* lsb=1,unit=回 :テスト結果出力用過去10トリップEWMA失火カウンタ上限値(全気筒)初期値 */
    u2 u2t_ECDMFEMTV_MI_c;        /* lsb=1,unit=回 :テスト結果出力用今トリップ累積失火カウンタ(気筒別)初期値 */
    u2 u2t_ECDMFEMMNTL_MI_c;      /* lsb=1,unit=回 :テスト結果出力用今トリップ累積失火カウンタ下限値(気筒別)初期値 */
    u2 u2t_ECDMFEMMXTL_MI_c;      /* lsb=1,unit=回 :テスト結果出力用今トリップ累積失火カウンタ上限値(気筒別)初期値 */
    u2 u2t_ECDMFAVTV_MI_c;        /* lsb=1,unit=回 :テスト結果出力用過去10トリップEWMA失火カウンタ(気筒別)初期値 */
    u2 u2t_ECDMFAVMNTL_MI_c;      /* lsb=1,unit=回 :テスト結果出力用過去10トリップEWMA失火カウンタ下限値(気筒別)初期値 */
    u2 u2t_ECDMFAVMXTL_MI_c;      /* lsb=1,unit=回 :テスト結果出力用過去10トリップEWMA失火カウンタ上限値(気筒別)初期値 */
    u1 u1t_ncyl_c;                /* lsb=1 :気筒数 */

    /* ワークデータ初期化 */
    u1t_xreqclr = u1s_emfng_xreqclr_Lch;

    if ( u1t_xreqclr == (u1)ON )
    {
        u1t_ncyl_c = u1g_ejcc_NCYL;
#if JEOOBD == u1g_EJCC_USE        /*【市場調査有】*/
        u2t_EOCMFMLTH_MI_c = u2g_emfng_EOCMFMLTH_MI;
        u2t_EOCMFMLTL_MI_c = u2g_emfng_EOCMFMLTL_MI;
        u1t_EONEMFMLT_MI_c = u1g_emfng_EONEMFMLT_MI;
        u2t_EOCMFNH_MI_c = u2g_emfng_EOCMFNH_MI;
        u2t_EOCMFNM_MI_c = u2g_emfng_EOCMFNM_MI;
        u2t_EOCMFNL_MI_c = u2g_emfng_EOCMFNL_MI;
        u1t_EOCDMFAE2MX_MI_c = u1g_emfng_EOCDMFAE2MX_MI;
        u1t_EOCJREVEM_MI_c = u1g_emfng_EOCJREVEM_MI;
        s1t_EOTHWEM_MI_c = s1g_emfng_EOTHWEM_MI;
        s1t_EOTHAEM_MI_c = s1g_emfng_EOTHAEM_MI;
        u1t_EOCDMFEM2MX_MI_c = u1g_emfng_EOCDMFEM2MX_MI;
        u1t_EOSPDEMAV_MI_c = u1g_emfng_EOSPDEMAV_MI;
        u1t_EONEEMAV_MI_c = u1g_emfng_EONEEMAV_MI;
        u1t_EOKLSMEMAV_MI_c = u1g_emfng_EOKLSMEMAV_MI;
        u1t_EOCJMFNEEMI_MI_c = u1g_emfng_EOCJMFNEEMI_MI;
        u1t_EOCJMFNEEML_MI_c = u1g_emfng_EOCJMFNEEML_MI;
        u1t_EOCJMFNEEMH_MI_c = u1g_emfng_EOCJMFNEEMH_MI;
        u1t_EOCDMFAOTMX_MI_c = u1g_emfng_EOCDMFAOTMX_MI;
        u1t_EOCJREVOT_MI_c = u1g_emfng_EOCJREVOT_MI;
        s1t_EOTHWOT_MI_c = s1g_emfng_EOTHWOT_MI;
        s1t_EOTHAOT_MI_c = s1g_emfng_EOTHAOT_MI;

        /* 市場調査RAM消去 */
        (void)s4g_ememctr_write( u2g_EMFNG_EOCMFMLTH_U2_ID, (void *)&u2t_EOCMFMLTH_MI_c );           /* 戻り値は使用しない */
        (void)s4g_ememctr_write( u2g_EMFNG_EOCMFMLTL_U2_ID, (void *)&u2t_EOCMFMLTL_MI_c );           /* 戻り値は使用しない */
        (void)s4g_ememctr_write( u2g_EMFNG_EONEMFMLT_U1_ID, (void *)&u1t_EONEMFMLT_MI_c );           /* 戻り値は使用しない */
        (void)s4g_ememctr_write( u2g_EMFNG_EOCMFNH_U2_ID,   (void *)&u2t_EOCMFNH_MI_c );             /* 戻り値は使用しない */
        (void)s4g_ememctr_write( u2g_EMFNG_EOCMFNM_U2_ID,   (void *)&u2t_EOCMFNM_MI_c );             /* 戻り値は使用しない */
        (void)s4g_ememctr_write( u2g_EMFNG_EOCMFNL_U2_ID,   (void *)&u2t_EOCMFNL_MI_c );             /* 戻り値は使用しない */
        (void)s4g_ememctr_write( u2g_EMFNG_EOCDMFAE2MX_U1_ID, (void *)&u1t_EOCDMFAE2MX_MI_c );       /* 戻り値は使用しない */
        (void)s4g_ememctr_write( u2g_EMFNG_EOCJREVEM_U1_ID, (void *)&u1t_EOCJREVEM_MI_c );           /* 戻り値は使用しない */
        (void)s4g_ememctr_write( u2g_EMFNG_EOTHWEM_S1_ID, (void *)&s1t_EOTHWEM_MI_c );               /* 戻り値は使用しない */
        (void)s4g_ememctr_write( u2g_EMFNG_EOTHAEM_S1_ID, (void *)&s1t_EOTHAEM_MI_c );               /* 戻り値は使用しない */
        for ( s4t_i = (s4)0; s4t_i < (s4)u1t_ncyl_c; s4t_i++ )
        {
            (void)s4g_ememctr_write( u2s_EOCDMFEM2MX_U1_ID[s4t_i], (void *)&u1t_EOCDMFEM2MX_MI_c );  /* 戻り値は使用しない */
        }
        (void)s4g_ememctr_write( u2g_EMFNG_EOSPDEMAV_U1_ID, (void *)&u1t_EOSPDEMAV_MI_c );           /* 戻り値は使用しない */
        (void)s4g_ememctr_write( u2g_EMFNG_EONEEMAV_U1_ID, (void *)&u1t_EONEEMAV_MI_c );             /* 戻り値は使用しない */
        (void)s4g_ememctr_write( u2g_EMFNG_EOKLSMEMAV_U1_ID, (void *)&u1t_EOKLSMEMAV_MI_c );         /* 戻り値は使用しない */
        (void)s4g_ememctr_write( u2g_EMFNG_EOCJMFNEEMI_U1_ID, (void *)&u1t_EOCJMFNEEMI_MI_c );       /* 戻り値は使用しない */
        (void)s4g_ememctr_write( u2g_EMFNG_EOCJMFNEEML_U1_ID, (void *)&u1t_EOCJMFNEEML_MI_c );       /* 戻り値は使用しない */
        (void)s4g_ememctr_write( u2g_EMFNG_EOCJMFNEEMH_U1_ID, (void *)&u1t_EOCJMFNEEMH_MI_c );       /* 戻り値は使用しない */
        (void)s4g_ememctr_write( u2g_EMFNG_EOCDMFAOTMX_U1_ID, (void *)&u1t_EOCDMFAOTMX_MI_c );       /* 戻り値は使用しない */
        (void)s4g_ememctr_write( u2g_EMFNG_EOCJREVOT_U1_ID, (void *)&u1t_EOCJREVOT_MI_c );           /* 戻り値は使用しない */
        (void)s4g_ememctr_write( u2g_EMFNG_EOTHWOT_S1_ID, (void *)&s1t_EOTHWOT_MI_c );               /* 戻り値は使用しない */
        (void)s4g_ememctr_write( u2g_EMFNG_EOTHAOT_S1_ID, (void *)&s1t_EOTHAOT_MI_c );               /* 戻り値は使用しない */
#endif
        /* mode$06対応失火カウンタ消去 */
        u2t_ECDMFAEMTV_MI_c = u2g_emfng_ECDMFAEMTV_MI;
        u2t_ECDMFAEMMNTL_MI_c = u2g_emfng_ECDMFAEMMNTL_MI;
        u2t_ECDMFAEMMXTL_MI_c = u2g_emfng_ECDMFAEMMXTL_MI;
        u2t_ECDMFAAVTV_MI_c = u2g_emfng_ECDMFAAVTV_MI;
        u2t_ECDMFAAVMNTL_MI_c = u2g_emfng_ECDMFAAVMNTL_MI;
        u2t_ECDMFAAVMXTL_MI_c = u2g_emfng_ECDMFAAVMXTL_MI;
        u2t_ECDMFEMTV_MI_c = u2g_emfng_ECDMFEMTV_MI;
        u2t_ECDMFEMMNTL_MI_c = u2g_emfng_ECDMFEMMNTL_MI;
        u2t_ECDMFEMMXTL_MI_c = u2g_emfng_ECDMFEMMXTL_MI;
        u2t_ECDMFAVTV_MI_c = u2g_emfng_ECDMFAVTV_MI;
        u2t_ECDMFAVMNTL_MI_c = u2g_emfng_ECDMFAVMNTL_MI;
        u2t_ECDMFAVMXTL_MI_c = u2g_emfng_ECDMFAVMXTL_MI;

        (void)s4g_ememctr_write( u2g_EMFNG_ECDMFAEMTV_U2_ID, (void *)&u2t_ECDMFAEMTV_MI_c );         /* 戻り値は使用しない */
        (void)s4g_ememctr_write( u2g_EMFNG_ECDMFAEMMNTL_U2_ID, (void *)&u2t_ECDMFAEMMNTL_MI_c );     /* 戻り値は使用しない */
        (void)s4g_ememctr_write( u2g_EMFNG_ECDMFAEMMXTL_U2_ID, (void *)&u2t_ECDMFAEMMXTL_MI_c );     /* 戻り値は使用しない */
        (void)s4g_ememctr_write( u2g_EMFNG_ECDMFAAVTV_U2_ID, (void *)&u2t_ECDMFAAVTV_MI_c );         /* 戻り値は使用しない */
        (void)s4g_ememctr_write( u2g_EMFNG_ECDMFAAVMNTL_U2_ID, (void *)&u2t_ECDMFAAVMNTL_MI_c );     /* 戻り値は使用しない */
        (void)s4g_ememctr_write( u2g_EMFNG_ECDMFAAVMXTL_U2_ID, (void *)&u2t_ECDMFAAVMXTL_MI_c );     /* 戻り値は使用しない */

        for ( s4t_i = (s4)0 ; s4t_i < (s4)u1t_ncyl_c ; s4t_i++ )
        {
            (void)s4g_ememctr_write( u2s_ECDMFEMTV_U2_ID[s4t_i], (void *)&u2t_ECDMFEMTV_MI_c );      /* 戻り値は使用しない */
            (void)s4g_ememctr_write( u2s_ECDMFEMMNTL_U2_ID[s4t_i], (void *)&u2t_ECDMFEMMNTL_MI_c );  /* 戻り値は使用しない */
            (void)s4g_ememctr_write( u2s_ECDMFEMMXTL_U2_ID[s4t_i], (void *)&u2t_ECDMFEMMXTL_MI_c );  /* 戻り値は使用しない */
            (void)s4g_ememctr_write( u2s_ECDMFAVTV_U2_ID[s4t_i], (void *)&u2t_ECDMFAVTV_MI_c );      /* 戻り値は使用しない */
            (void)s4g_ememctr_write( u2s_ECDMFAVMNTL_U2_ID[s4t_i], (void *)&u2t_ECDMFAVMNTL_MI_c );  /* 戻り値は使用しない */
            (void)s4g_ememctr_write( u2s_ECDMFAVMXTL_U2_ID[s4t_i], (void *)&u2t_ECDMFAVMXTL_MI_c );  /* 戻り値は使用しない */
        }
        /* u2g_EMFNG_EXRCDMF_U1_IDはクリア不要 */
    }
    return( u1t_xreqclr );
}

/*********************************************************************/
/*  関数名        | emfng_emode06_cal( )                             */
/*  処理内容      | mode$06対応失火ｶｳﾝﾀの操作                        */
/*  制御タイミング| emfng_65msl( )                                   */
/*  引数          | なし                                             */
/*  戻り値        | なし                                             */
/*  注意事項      | 内部関数                                         */
/*********************************************************************/
static void
emfng_emode06_cal( void )
{
#pragma ghs startdata
#if ( JENCYL == u1g_EJCC_6CYL )      /*【6気筒】*/
 #if ( JEEGTYPE == u1g_EJCC_V6CYL )   /*【V6】*/
    /*-----------------------------------------------------------------*/
    /*                         t_cyl=0,    1,    2,    3,    4,    5   */
    /* u1s_emfng_CYLMODTBL[t_cyl] → 1,    2,    0,    1,    2,    0   */
    /*                              #1#4, #2#5, #3#6, #1#4, #2#5, #3#6 */
    /*-----------------------------------------------------------------*/
    static volatile const u1 u1s_emfng_CYLMODTBL[u1g_EJCC_NCYL] =        /* :気筒群(ecdmfw[]) → 気筒変換テーブル  */
    { (u1)1U, (u1)2U, (u1)0U, (u1)1U, (u1)2U, (u1)0U };                  /* #3#6, #1#4, #2#5  → #1,#2,#3,#4,#5,#6 */
 #endif
 #if ( JEEGTYPE == u1g_EJCC_L6CYL )   /*【L6】*/
    /*-----------------------------------------------------------------*/
    /*                         t_cyl=0,    1,    2,    3,    4,    5   */
    /* u1s_emfng_CYLMODTBL[t_cyl] → 1,    2,    0,    0,    2,    1   */
    /*                              #1#6, #2#5, #3#4, #3#4, #2#5, #1#6 */
    /*-----------------------------------------------------------------*/
    static volatile const u1 u1s_emfng_CYLMODTBL[u1g_EJCC_NCYL] =        /* :気筒群(ecdmfw[]) → 気筒変換テーブル  */
    { (u1)1U, (u1)2U, (u1)0U, (u1)0U, (u1)2U, (u1)1U };                  /* #3#4, #1#6, #2#5  → #1,#2,#3,#4,#5,#6 */
 #endif
 #if ( JEEGTYPE == u1g_EJCC_V6CYL )   /*【V6】*/
    static volatile const u1 u1s_emfng_CYLWTBL[2][3] = /* :気筒群(t_cylw) → 気筒変換テーブル  */
    {{ (u1)2U, (u1)0U, (u1)1U },              /* #3, #1, #2 */
     { (u1)5U, (u1)3U, (u1)4U }};             /* #6, #4, #5 */
 #elif ( JEEGTYPE == u1g_EJCC_L6CYL ) /*【L6】*/
    static volatile const u1 u1s_emfng_CYLWTBL[2][3] = /* :気筒群(t_cylw) → 気筒変換テーブル  */
    {{ (u1)2U, (u1)0U, (u1)1U },              /* #3, #1, #2 */
     { (u1)3U, (u1)5U, (u1)4U }};             /* #4, #6, #5 */
 #endif
#endif
#pragma ghs enddata

    s4  s4t_ecdmfaemTv;     /* ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(全気筒) */
    u2  u2t_ecdmfaemTv;     /* ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(全気筒)(書き込み用) */
    u2  u2t_ECDMFAEMMNTL_MI_c;  /* lsb=1,unit=回 :テスト結果出力用今トリップ累積失火カウンタ下限値(全気筒)初期値 */
    u2  u2t_ecdmfaemMxtl;   /* ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(全気筒)(書き込み用) */
    s4  s4t_ecdmfaemTvo;    /* ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(全気筒)(前回値) */
    s4  s4t_ecdmfemTv;      /* ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(気筒別) */
    u2  u2t_ecdmfemTv;      /* ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(気筒別)(書き込み用) */
    u2  u2t_ECDMFEMMNTL_MI_c;   /* lsb=1,unit=回 :テスト結果出力用今トリップ累積失火カウンタ下限値(気筒別)初期値 */
    u2  u2t_ecdmfemMxtl;    /* ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(気筒別)(書き込み用) */
    s4  s4t_cyl;
    s4  s4t_cdmfwa;         /* lsb=1,unit=回:対向気筒失火カウンタ積算値 */
#if JENCYL == u1g_EJCC_6CYL     /*【6気筒】*/
    s4  s4t_i;              /* lsb=1 :forループ変数 */
    u1  u1t_tblidx;         /* :気筒群(ecdmfw[]) → 気筒変換ﾃｰﾌﾞﾙ 取得値 */
    s4  s4t_cdmfwtmp;       /* lsb=1,unit=回:気筒別失火カウンタの気筒群合計値 */
    s4  s4t_ecdmfw2;        /* lsb=1,unit=回:対向気筒失火ｶｳﾝﾀ(演算値) */
    s4  s4t_cdmfemTvtmp[u1g_EJCC_NCYL/2];   /*  lsb=1,unit=回 :対向気筒失火カウンタの１気筒あたり失火回数 */
    s4  s4t_ecdmfw;         /* lsb=1,unit=回 :対向気筒失火ｶｳﾝﾀ */
    u1  u1t_excdmfw;        /* lsb=1 :対向気筒失火判定成立ﾌﾗｸﾞ */
    u1  u1t_idx1;           /* lsb=1 :気筒群->気筒1変換用ﾜｰｸ変数 */
    u1  u1t_idx2;           /* lsb=1 :気筒群->気筒2変換用ﾜｰｸ変数 */
    u1 u1t_ncylw_c;         /* lsb=1 :気筒群数 */
#endif
    s4  s4t_ecdmfem[u1g_EJCC_NCYL];         /* lsb=1,unit=回:EM悪化ﾚﾍﾞﾙ気筒別失火ｶｳﾝﾀ */
#if ( JETCORE_D == u1g_EJCC_USE ) && ( JEOBDUDS_D == u1g_EJCC_NOT_USE )                            /*【t-CORE対応有】AND【OBDonUDS対応無】*/
    u2  u2t_ecdmfaemTv_get; /* lsb=1,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(全気筒)(戻り値取得用) */
    u2  u2t_ecdmfaemMntl_get;  /* lsb=1,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ下限値(全気筒)(戻り値取得用) */
    u2  u2t_ecdmfaemMxtl_get;  /* lsb=1,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ上限値(全気筒)(戻り値取得用) */
    u2  u2t_dtrid;          /* lsb=1 :ﾃｽﾄ結果出力ＩＤ */
    u2  u2t_ecdmfemTv_get;  /* lsb=1,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(気筒別)(戻り値取得用) */
    u2  u2t_ecdmfemMntl_get;   /* lsb=1,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ下限値(気筒別)(戻り値取得用) */
    u2  u2t_ecdmfemMxtl_get;   /* lsb=1,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ上限値(気筒別)(戻り値取得用) */
#endif
    u1  u1t_ncyl_c;         /* lsb=1 :気筒数 */

    /* ワークデータ初期化 */
    u1t_ncyl_c = u1g_ejcc_NCYL;
    s4t_ecdmfem[0] = (s4)u1g_emfng_ecdmfem[0];
    for ( s4t_cyl = (s4)1; s4t_cyl < (s4)u1t_ncyl_c; s4t_cyl++ )
    {
        s4t_ecdmfem[s4t_cyl] = (s4)u1g_emfng_ecdmfem[s4t_cyl];
    }
    u2t_ECDMFAEMMNTL_MI_c = u2g_emfng_ECDMFAEMMNTL_MI;
    u2t_ECDMFEMMNTL_MI_c = u2g_emfng_ECDMFEMMNTL_MI;

    /**************************/
    /* 気筒群の総失火回数算出 */
    /**************************/
    s4t_cdmfwa = (s4)0;
#if JENCYL == u1g_EJCC_6CYL     /*【6気筒】*/
    u1t_excdmfw = (u1)big_emfng_excdmfw;
    s4t_cdmfemTvtmp[0] = (s4)0; /* ﾀﾞﾐｰ設定(ﾁｪｯｶ警告回避) */
    u1t_ncylw_c = u1s_EMFNG_NCYLW;
    for ( s4t_i = (s4)0; s4t_i < (s4)u1t_ncylw_c ; s4t_i++  )
    {
        s4t_cdmfemTvtmp[s4t_i] = (s4)0;

        if ( u1t_excdmfw == (u1)ON )                      /* 対向気筒失火 */
        {
            s4t_ecdmfw = (s4)u1g_emfng_ecdmfw[s4t_i];

            u1t_idx1 = u1s_emfng_CYLWTBL[0][s4t_i];
            u1t_idx2 = u1s_emfng_CYLWTBL[1][s4t_i];

            s4t_cdmfwtmp =  s4t_ecdmfem[u1t_idx1];  /* 判定気筒群の気筒1 */
            s4t_cdmfwtmp += s4t_ecdmfem[u1t_idx2];  /* 判定気筒群の気筒2 */

            s4t_ecdmfw2 = (s4)s2g_glmulst_s2s2u2( (s2)s4t_ecdmfw, s2s_emfng_KCDMFWL, (u2)13U );   /* lsb=1*(2/64/256) -> 1 */
            if ( s4t_cdmfwtmp < s4t_ecdmfw2 )
            {
                s4t_cdmfwa += s4t_ecdmfw;
                /* 対向気筒用のカウンタが全て255回でもオーバーフローなし  */
                s4t_cdmfemTvtmp[s4t_i] = s4g_glbitcal_s4sftr_s4u1( s4t_ecdmfw, (u1)1U );     /* (ecdmfw[]の該当気筒分)÷２ */
            }
        }
    }
#endif

    /* ecdmfaem前回値 */
    s4t_ecdmfaemTvo = (s4)u2g_emfng_ecdmfaemTv();

    /************************************/
    /* 全気筒失火回数(ecdmfaemTv)の算出 */
    /************************************/
    s4t_ecdmfaemTv = s4t_ecdmfaemTvo            /* 前回値 */
                  + s4t_cdmfwa                  /* + EMFCNT_OPPS時の失火回数【6気筒】*/
                  + (s4)s2s_emfng_ecdmfaotb;    /* + (ecdmfaot - (ecdtmf2 * KCDTMFAOTB)) */

    /* 上下限ガード処理 */
    ELIB_HILOGD2( s4t_ecdmfaemTv, (s4)u2g_U2MAX, s4t_ecdmfaemTvo, s4t_ecdmfaemTv );
    u2t_ecdmfaemTv = (u2)s4t_ecdmfaemTv;

    /* ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(全気筒) */
    (void)s4g_ememctr_write( u2g_EMFNG_ECDMFAEMTV_U2_ID, (void *)&u2t_ecdmfaemTv );                /* 戻り値は使用しない */
    /* ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ下限値(全気筒) */
    (void)s4g_ememctr_write( u2g_EMFNG_ECDMFAEMMNTL_U2_ID, (void *)&u2t_ECDMFAEMMNTL_MI_c );       /* 戻り値は使用しない */
    /* ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ上限値(全気筒) */
    u2t_ecdmfaemMxtl = (u2)0xffffU;
    (void)s4g_ememctr_write( u2g_EMFNG_ECDMFAEMMXTL_U2_ID, (void *)&u2t_ecdmfaemMxtl );            /* 戻り値は使用しない */
#if ( JETCORE_D == u1g_EJCC_USE ) && ( JEOBDUDS_D == u1g_EJCC_NOT_USE )                            /*【t-CORE対応有】AND【OBDonUDS対応無】*/
    u2t_ecdmfaemTv_get   = u2g_emfng_ecdmfaemTv();
    u2t_ecdmfaemMntl_get = u2g_emfng_ecdmfaemMntl();
    u2t_ecdmfaemMxtl_get = u2g_emfng_ecdmfaemMxtl();
    (void)u1g_wtcif_dem_setdtr_u2s4s4s4u1( u2g_WTCIF_DEM_DTR_ECDMFAEM, (s4)u2t_ecdmfaemTv_get, (s4)u2t_ecdmfaemMntl_get, (s4)u2t_ecdmfaemMxtl_get, u1g_WTCIF_DEM_DTR_CTL_NORMAL );  /* 戻り値は使用しない */
#endif

    /**************************************************/
    /* ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(気筒別)の算出 */
    /**************************************************/
    for ( s4t_cyl = (s4)0; s4t_cyl < (s4)u1t_ncyl_c ; s4t_cyl++ )
    {
#if JENCYL == u1g_EJCC_6CYL     /*【6気筒】*/
        u1t_tblidx = u1s_emfng_CYLMODTBL[ s4t_cyl ];

        /* 対向気筒分 */
        s4t_ecdmfemTv = s4t_cdmfemTvtmp[u1t_tblidx];

        s4t_ecdmfemTv += (s4)u2g_emfng_ecdmfemTv( (u1)s4t_cyl );
#else                           /*【6気筒以外】*/
        s4t_ecdmfemTv = (s4)u2g_emfng_ecdmfemTv( (u1)s4t_cyl );
#endif
        /* 各気筒ごとの失火回数 */
        s4t_ecdmfemTv += s4t_ecdmfem[s4t_cyl];

        /* 上限ガード処理 */
        ELIB_HIGHGD2( s4t_ecdmfemTv,(s4)u2g_U2MAX, s4t_ecdmfemTv );
        u2t_ecdmfemTv = (u2)s4t_ecdmfemTv;

        /* ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(気筒別) */
        (void)s4g_ememctr_write( u2s_ECDMFEMTV_U2_ID[s4t_cyl], (void *)&u2t_ecdmfemTv );           /* 戻り値は使用しない */
        /* ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ下限値(気筒別) */
        (void)s4g_ememctr_write( u2s_ECDMFEMMNTL_U2_ID[s4t_cyl], (void *)&u2t_ECDMFEMMNTL_MI_c );  /* 戻り値は使用しない */
        /* ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ上限値(気筒別) */
        u2t_ecdmfemMxtl = (u2)0xffffU;
        (void)s4g_ememctr_write( u2s_ECDMFEMMXTL_U2_ID[s4t_cyl], (void *)&u2t_ecdmfemMxtl );       /* 戻り値は使用しない */
#if ( JETCORE_D == u1g_EJCC_USE ) && ( JEOBDUDS_D == u1g_EJCC_NOT_USE )                            /*【t-CORE対応有】AND【OBDonUDS対応無】*/
        u2t_dtrid = u2s_emfng_ecdmfem_tbl[s4t_cyl];
        u2t_ecdmfemTv_get   = u2g_emfng_ecdmfemTv( (u1)s4t_cyl );
        u2t_ecdmfemMntl_get = u2g_emfng_ecdmfemMntl( (u1)s4t_cyl );
        u2t_ecdmfemMxtl_get = u2g_emfng_ecdmfemMxtl( (u1)s4t_cyl );
        (void)u1g_wtcif_dem_setdtr_u2s4s4s4u1( u2t_dtrid, (s4)u2t_ecdmfemTv_get, (s4)u2t_ecdmfemMntl_get, (s4)u2t_ecdmfemMxtl_get, u1g_WTCIF_DEM_DTR_CTL_NORMAL );  /* 戻り値は使用しない */
#endif
    }
}

#if JEMFFC == u1g_EJCC_USE  /*【触媒OT失火異常時FC有】*/
/*********************************************************************/
/*  関数名        | emfng_exmfotfc_u1( )                             */
/*  処理内容      | FCｵﾌﾟｼｮﾝ判定用触媒OTﾚﾍﾞﾙ異常通知ﾌﾗｸﾞの処理       */
/*  制御タイミング| emfng_65msl( )                                   */
/*  引数          | u1t_flg   :lsb=1 フラグ情報                      */
/*  戻り値        | なし                                             */
/*  注意事項      | 【触媒OT失火異常時FC有】                         */
/*********************************************************************/
void
emfng_exmfotfc_u1( u1 u1t_flg )
{
    s4 s4t_exdmffc;   /* lsb=1:FCｵﾌﾟｼｮﾝ判定用触媒OTﾚﾍﾞﾙ異常通知ﾌﾗｸﾞ */

    s4t_exdmffc = (s4)OFF;
    if ( u1t_flg == (u1)ON )
    {
        s4t_exdmffc = (s4)ON;
    }
    u1g_emfng_exdmffc = (u1)s4t_exdmffc;
}
#endif

#if JEOOBD == u1g_EJCC_USE      /*【市場調査有】*/
/*********************************************************************/
/*  関数名        | emfng_espdaemclr( )                              */
/*  処理内容      | 市場調査RAMのｸﾘｱ                                 */
/*  制御タイミング| 処理要求時                                       */
/*  戻り値        | なし                                             */
/*  注意事項      | 内部関数【市場調査有】                           */
/*********************************************************************/
static void
emfng_espdaemclr( void )
{
    /* クリア処理 */
    s4s_emfng_espdaem  = (s4)0;
    s4s_emfng_eneaem   = (s4)0;
    s4s_emfng_eklsmaem = (s4)0;
    u1s_emfng_ecjmfneemi = (u1)0U;
    u1s_emfng_ecjmfneeml = (u1)0U;
    u1s_emfng_ecjmfneemh = (u1)0U;
}
#endif

/*********************************************************************/
/*  関数名        | emfng_roughjdg_u1u1( )                           */
/*  処理内容      | 悪路判定関連処理                                 */
/*  制御タイミング| TDC毎(emfng_mfptn_ann_u1u1u1()からコール)        */
/*  引数          | u1t_xmfne      :lsb=1 失火本判定                 */
/*                | u1t_mfkind     :lsb=1 失火種別                   */
/*  戻り値        | なし                                             */
/*  注意事項      | 内部関数                                         */
/*********************************************************************/
static void
emfng_roughjdg_u1u1( u1 u1t_xmfne, u1 u1t_mfkind )
{
#if JEMFMETHOD_D == u1g_EJCC_TIME_D    /*【失火ΔT法】*/
    static volatile s2 s2s_emfng_edltmaxlho; /* lsb=1/8,unit=us :低回転域回転変動量ピーク値記憶 */
    static volatile s2 s2s_emfng_edltmaxhho; /* lsb=1/8,unit=us :高回転域回転変動量ピーク値記憶 */
#endif
#if JEMFMETHOD_D == u1g_EJCC_OMEGA_D   /*【失火Δω法】*/
    static volatile s2 s2s_emfng_ewmaxlo;    /* lsb=32/128/256,unit=CA/ms :低回転域回転変動量ピーク値記憶 */
    static volatile s2 s2s_emfng_ewmaxho;    /* lsb=32/128/256,unit=CA/ms :高回転域回転変動量ピーク値記憶 */
#endif

    u2 u2t_ecdtmfwa;        /* lsb=1,unit=回 :EM悪化レベル判定用悪路カウンタ */
    u1 u1t_ecdtmf2;         /* lsb=1,unit=回 :触媒OTレベル判定用悪路カウンタ */
    u1 u1t_exjtmf;          /* lsb=1 :失火の仮判定禁止フラグ */
    u1 u1t_exjtmf2;         /* lsb=1 :失火の本判定許可フラグ */
#if JEMFMETHOD_D == u1g_EJCC_TIME_D    /*【失火ΔT法】*/
    s2 s2t_edltmaxlo;       /* lsb=1/8,unit=us :低回転域回転変動量ピーク値記憶 */
    s2 s2t_edltmaxho;       /* lsb=1/8,unit=us :高回転域回転変動量ピーク値記憶 */
    s2 s2t_edltmaxl;        /* lsb=1/8,unit=us :回転変動量ピーク値(Lowモード) */
    s2 s2t_edltmaxh;        /* lsb=1/8,unit=us :回転変動量ピーク値(Highモード) */
#endif
#if JEMFMETHOD_D == u1g_EJCC_OMEGA_D   /*【失火Δω法】*/
    s2 s2t_ewmaxlo;         /* lsb=32/128/256,unit=CA/ms :低回転域回転変動量ピーク値記憶 */
    s2 s2t_ewmaxho;         /* lsb=32/128/256,unit=CA/ms :高回転域回転変動量ピーク値記憶 */
    s2 s2t_ewmaxl;          /* lsb=32/128/256,unit=CA/ms :回転変動量ピーク値(Lowモード) */
    s2 s2t_ewmaxh;          /* lsb=32/128/256,unit=CA/ms :回転変動量ピーク値(Highモード) */
#endif
    s2 s2t_espd;            /* lsb=256/256,unit=km/h :車速 */
    u1 u1t_ejmfne;          /* lsb=1 :回転領域判定値 */
    s2 s2t_MFSPD_c;         /* lsb=256/256,unit=km/h :走行状態判定値 */
    u1 u1t_CJPMF_c;         /* lsb=1,unit=回 :ゼロクロスカウンタ判定値 */

    /* ワークデータ初期化 */
    u1t_exjtmf = (u1)bis_emfng_exjtmf;
    u1t_exjtmf2 = (u1)bis_emfng_exjtmf2;
#if JEMFMETHOD_D == u1g_EJCC_TIME_D    /*【失火ΔT法】*/
    s2t_edltmaxlo = s2s_emfng_edltmaxlho;
    s2t_edltmaxho = s2s_emfng_edltmaxhho;
    s2t_edltmaxl = s2g_emfcc_edltmaxlh;
    s2t_edltmaxh = s2g_emfcc_edltmaxhh;
#endif
#if JEMFMETHOD_D == u1g_EJCC_OMEGA_D   /*【失火Δω法】*/
    s2t_ewmaxlo = s2s_emfng_ewmaxlo;
    s2t_ewmaxho = s2s_emfng_ewmaxho;
    s2t_ewmaxl = s2g_emfcc_ewmaxl;
    s2t_ewmaxh = s2g_emfcc_ewmaxh;
#endif
    s2t_espd = s2g_espd_espd;
    u1t_ejmfne = u1g_emfne_ejmfne;
    s2t_MFSPD_c = s2g_emfcnt_MFSPD;
    u1t_CJPMF_c = u1s_emfng_CJPMF;

    if ( u1t_xmfne == (u1)ON )                             /* 本判定タイミング */
    {
        /**********************************************/
        /* EM悪化レベル判定用悪路カウンタ、           */
        /* 触媒OTレベル判定用悪路カウンタ、           */
        /* 正常判定用悪路カウンタのカウントアップ通知 */
        /**********************************************/
        u2t_ecdtmfwa = u2s_emfng_ecdtmfwa;
        u1t_ecdtmf2 = u1g_emfng_ecdtmf2;
        if ( ( u1t_mfkind == u1g_EMFCNT_NOT )          /* 失火判定不成立 */
          && ( u1t_exjtmf == (u1)OFF )                 /* 失火仮判定許可 */
#if JEMFHOUKI == u1g_EJCC_USAMF /*【対米法規】*/
          && ( big_emfcnt_exptnup == (u1)OFF )
#endif
          && ( s2t_espd >= s2t_MFSPD_c ) )
        {
            if ( u2t_ecdtmfwa < u2g_U2MAX )
            {
                u2t_ecdtmfwa++;
            }
            if ( u1t_ecdtmf2 < u1g_U1MAX )
            {
                u1t_ecdtmf2++;
            }
            emfok_roughcnt();
        }
        /* RAMデータに格納 */
        u2s_emfng_ecdtmfwa = u2t_ecdtmfwa;
        u1g_emfng_ecdtmf2 = u1t_ecdtmf2;

        /*******************************************************/
        /* 失火の仮判定禁止フラグ,失火の本判定許可フラグの操作 */
        /*******************************************************/
        if ( ( s2t_espd >= s2t_MFSPD_c )
          && ( ( u1t_mfkind == u1g_EMFCNT_LOW )        /* 低回転側ランダム失火 */
            || ( u1t_mfkind == u1g_EMFCNT_HIGH ) ) )   /* 高回転側ランダム失火 */
        {
            u1t_exjtmf = (u1)ON;
            u1t_exjtmf2 = (u1)ON;                      /* XJTMFと同時にON */
            u1s_emfng_ecjpmf = (u1)0U;                 /* ゼロクロスカウンタをクリア */
        }

        /**************************************/
        /* 回転変動量ピーク値の履歴の更新処理 */
        /**************************************/
        if ( ( u1t_mfkind == u1g_EMFCNT_NOT )          /* 失火判定不成立 */
          && ( u1t_exjtmf == (u1)ON ) )                /* 失火仮判定禁止 */
        {
            if ( u1t_exjtmf2 == (u1)ON )
            {
#if JEMFMETHOD_D == u1g_EJCC_TIME_D    /*【失火ΔT法】*/
                s2t_edltmaxlo = s2t_edltmaxl;
                s2t_edltmaxho = s2t_edltmaxh;
#endif
#if JEMFMETHOD_D == u1g_EJCC_OMEGA_D   /*【失火Δω法】*/
                s2t_ewmaxlo = s2t_ewmaxl;
                s2t_ewmaxho = s2t_ewmaxh;
#endif
                u1t_exjtmf2 = (u1)OFF;            /* 回転変動量ピーク値の履歴の更新時XJTMF2←OFF */
            }
            if ( ( u1t_ejmfne == u1g_EMFNE_REV_LOW )
#if JEMFMETHOD_D == u1g_EJCC_TIME_D    /*【失火ΔT法】*/
              && ( s2t_edltmaxlo >= s2t_edltmaxl )     /* 失火本判定許可 */
#endif
#if JEMFMETHOD_D == u1g_EJCC_OMEGA_D   /*【失火Δω法】*/
              && ( s2t_ewmaxlo <= s2t_ewmaxl )         /* 失火本判定許可 */
#endif
               )
            {
#if JEMFMETHOD_D == u1g_EJCC_TIME_D    /*【失火ΔT法】*/
                s2t_edltmaxlo = s2t_edltmaxl;
#endif
#if JEMFMETHOD_D == u1g_EJCC_OMEGA_D   /*【失火Δω法】*/
                s2t_ewmaxlo = s2t_ewmaxl;
#endif
                u1t_exjtmf2 = (u1)OFF;            /* 回転変動量ピーク値の履歴の更新時XJTMF2←OFF */
            }
            if ( ( u1t_ejmfne == u1g_EMFNE_REV_HIGH )
#if JEMFMETHOD_D == u1g_EJCC_TIME_D    /*【失火ΔT法】*/
              && ( s2t_edltmaxho >= s2t_edltmaxh )     /* 失火本判定許可 */
#endif
#if JEMFMETHOD_D == u1g_EJCC_OMEGA_D   /*【失火Δω法】*/
              && ( s2t_ewmaxho <= s2t_ewmaxh )         /* 失火本判定許可 */
#endif
               )
            {
#if JEMFMETHOD_D == u1g_EJCC_TIME_D    /*【失火ΔT法】*/
                s2t_edltmaxho = s2t_edltmaxh;
#endif
#if JEMFMETHOD_D == u1g_EJCC_OMEGA_D   /*【失火Δω法】*/
                s2t_ewmaxho = s2t_ewmaxh;
#endif
                u1t_exjtmf2 = (u1)OFF;            /* 回転変動量ピーク値の履歴の更新時XJTMF2←OFF */
            }
        }
    }   /* (t_xmfne ＝ ON) end */

    /*******************************************************/
    /* 失火の仮判定禁止フラグ,失火の本判定許可フラグの操作 */
    /*******************************************************/
    if ( ( s2t_espd < s2t_MFSPD_c )
      || ( u1s_emfng_ecjpmf >= u1t_CJPMF_c )
      || ( ( u1t_exjtmf2 == (u1)OFF )
        && ( ( ( u1t_ejmfne == u1g_EMFNE_REV_LOW )
#if JEMFMETHOD_D == u1g_EJCC_TIME_D    /*【失火ΔT法】*/
            && ( s2t_edltmaxlo < s2t_edltmaxl )
#endif
#if JEMFMETHOD_D == u1g_EJCC_OMEGA_D   /*【失火Δω法】*/
            && ( s2t_ewmaxlo > s2t_ewmaxl )
#endif
             )
          || ( ( u1t_ejmfne == u1g_EMFNE_REV_HIGH )
#if JEMFMETHOD_D == u1g_EJCC_TIME_D    /*【失火ΔT法】*/
            && ( s2t_edltmaxho < s2t_edltmaxh )
#endif
#if JEMFMETHOD_D == u1g_EJCC_OMEGA_D   /*【失火Δω法】*/
            && ( s2t_ewmaxho > s2t_ewmaxh )
#endif
             ) )
        && ( u1t_xmfne == (u1)ON )                     /* 本判定タイミング */
        && ( u1t_mfkind == u1g_EMFCNT_NOT ) ) )        /* 低回転or高回転失火判定不成立 */
    {
        u1t_exjtmf = (u1)OFF;                          /* 失火仮判定許可 */
        u1s_emfng_ecjpmf = (u1)0U;                     /* ゼロクロスカウンタをクリア */
    }
    if ( s2t_espd < s2t_MFSPD_c )
    {
        u1t_exjtmf2 = (u1)OFF;                         /* 失火本判定禁止 */
    }

    /* RAMデータに格納 */
    glbitcp_bibi( u1t_exjtmf, bis_emfng_exjtmf );
    glbitcp_bibi( u1t_exjtmf2, bis_emfng_exjtmf2 );
#if JEMFMETHOD_D == u1g_EJCC_TIME_D    /*【失火ΔT法】*/
    s2s_emfng_edltmaxlho = s2t_edltmaxlo;
    s2s_emfng_edltmaxhho = s2t_edltmaxho;
#endif
#if JEMFMETHOD_D == u1g_EJCC_OMEGA_D   /*【失火Δω法】*/
    s2s_emfng_ewmaxlo = s2t_ewmaxlo;
    s2s_emfng_ewmaxho = s2t_ewmaxho;
#endif
}
/**** End of File ****************************************************/
