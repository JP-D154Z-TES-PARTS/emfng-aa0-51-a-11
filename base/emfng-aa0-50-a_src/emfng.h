/* emfng-pa000-5000-a-M4 */
/***********************************************************************/
/*  Copyright Toyota Motor Corporation                                 */
/***********************************************************************/

/***********************************************************************/
/* オブジェクト名  | 失火検出異常判定部                                */
/* 注意事項        | 【失火OBD有】AND(【4気筒】OR【6気筒】)            */
/*---------------------------------------------------------------------*/
/*  I/F変更履歴    | aa0-41                                            */
/*                 |  再構築対応として新設(a00-43ベース)               */
/*                 |  部品全体コンパイルSWに【4気筒】OR【6気筒】を追加 */
/*                 |  以下の関数を削除                                 */
/*                 |   emfng_360cam(),emfng_cigfps_inc_u1()            */
/*                 |  以下のバックアップRAM取得関数名を変更            */
/*                 |   u2g_emfng_ecdmfaemc() → u2g_emfng_ecdmfaemTv() */
/*                 |   u2g_emfng_ecdmfaeml() → u2g_emfng_ecdmfaemMntl() */
/*                 |   u2g_emfng_ecdmfaemh() → u2g_emfng_ecdmfaemMxtl() */
/*                 |   u2g_emfng_ecdmfemc() → u2g_emfng_ecdmfemTv()   */
/*                 |   u2g_emfng_ecdmfeml() → u2g_emfng_ecdmfemMntl() */
/*                 |   u2g_emfng_ecdmfemh() → u2g_emfng_ecdmfemMxtl() */
/*                 |   u2g_emfng_ecdmfaavc() → u2g_emfng_ecdmfaavTv() */
/*                 |   u2g_emfng_ecdmfaavl() → u2g_emfng_ecdmfaavMntl() */
/*                 |   u2g_emfng_ecdmfaavh() → u2g_emfng_ecdmfaavMxtl() */
/*                 |   u2g_emfng_ecdmfavc() → u2g_emfng_ecdmfavTv()   */
/*                 |   u2g_emfng_ecdmfavl() → u2g_emfng_ecdmfavMntl() */
/*                 |   u2g_emfng_ecdmfavh() → u2g_emfng_ecdmfavMxtl() */
/*                 |  以下のバックアップRAM取得(平均負荷)の関数名,LSB,単位を変更 */
/*                 |   u1g_emfng_eognsmemav() → u1g_emfng_eoklsmemav() */
/*                 |   LSB,単位:lsb=4/256,unit=g/rev → lsb=320/256,unit=% */
/*                 |  以下の変数を削除                                 */
/*                 |   u2g_emfng_ecdigf                                */
/*                 |  以下の非公開オートカウンタを削除                 */
/*                 |   s1g_emfng_ecigtctmf_4m,s1g_emfng_ecigswmf_8m    */
/*                 |   s1g_emfng_ecstamf_8m,s1g_emfng_ecbatmf_8m       */
/*                 |  以下のフラグを追加                               */
/*                 |   big_emfng_exenable,big_emfng_excomplete         */
/*                 |  以下のバックアップRAM初期値定数を追加            */
/*                 |   u2g_emfng_EOCMFNL_MI,u2g_emfng_EOCMFNM_MI       */
/*                 |   u2g_emfng_EOCMFNH_MI,u2g_emfng_EOCMFMLTL_MI     */
/*                 |   u2g_emfng_EOCMFMLTH_MI,u1g_emfng_EONEMFMLT_MI   */
/*                 |  バックアップRAM-ID名を変更(u2g_****_SID → u2g_****_ID) */
/*                 |  バックアップRAM初期値名を変更(u2g_**** → u2g_****_MI) */
/*                 | aa0-42                                            */
/*                 |  公開変数u1g_emfng_ecdmffcbnk[]【触媒OT失火異常時FC有】を削除 */
/*                 | aa0-43                                            */
/*                 | バックアップRAMのID、初期値及び取得関数から       */
/*                 | 【ﾀﾞｲｱｸﾞCAN対応有】を削除                         */
/*                 |  以下の公開関数を削除                             */
/*                 |   emfng_ecdtmfzn_reset()                          */
/*                 |   emfng_360cam()                                  */
/*                 |  以下の公開変数を削除                             */
/*                 |   u1g_emfng_ecdtmfzn                              */
/*                 |  以下の公開フラグを削除                           */
/*                 |   big_emfng_exenable                              */
/*                 |  以下の公開フラグを追加                           */
/*                 |   big_emfng_exdisable                             */
/*                 | aa0-44                                            */
/*                 |  部品全体コンパイルSWから【4気筒】OR【6気筒】を削除 */
/*                 |  以下の公開関数を削除                             */
/*                 |   emfng_igsw_reset()                              */
/*                 |  以下の公開関数を追加                             */
/*                 |   emfng_drvclchg()                                */
/*                 |   emfng_reset_ecwamask() 【6気筒】                */
/*                 |  以下の公開変数を削除                             */
/*                 |   s2g_emfng_ekneh【6気筒】                        */
/*                 | aa0-46                                            */
/*                 |  公開関数emfng_reset_ecwamask()を削除 【6気筒】   */
/*                 | aa0-47                                            */
/*                 |  部品全体コンパイルSWに【4気筒】OR【6気筒】を追加 */
/*                 | aa0-48                                            */
/*                 |  【8気筒】時のI/Fを削除                           */
/*                 | aa0-50                                            */
/*                 |  以下の公開変数を追加                             */
/*                 |   big_emfng_exdmf【t-CORE対応有】                 */
/*                 |  以下の変数をstatic変数から公開変数に変更         */
/*                 |   u1g_emfng_ecdtmf2                               */
/*                 |  以下の公開mapを追加                              */
/*                 |   s2g_emfng_emfrtotgpf_map【GPF有】               */
/*---------------------------------------------------------------------*/
/*  ファイル構成   | emfng_l_mat.c,emfng_4_c_mat.c【4気筒】            */
/*                 | emfng_6_c_mat.c【6気筒】                          */
/*---------------------------------------------------------------------*/
/*     個別変更履歴 （個別変更時は以下に変更日、内容を記入すること）   */
/***********************************************************************/

#ifndef EMFNG_H
#define EMFNG_H

/*-------------------------------------------------------------------*/
/* ヘッダファイルのインクルード                                      */
/*-------------------------------------------------------------------*/
#include <engsrc/espc/ejcc.h>              /* u1g_EJCC_NCYL */

/*-------------------------------------------------------------------*/
/* コンパイルSW未定義チェック                                        */
/*-------------------------------------------------------------------*/
#if !defined u1g_EJCC_NCYL
#error "u1g_EJCC_NCYL is undeclared"
#endif

/*---------------------------------------------------------------------*/
/* 型定義                                                              */
/*---------------------------------------------------------------------*/

/*---------------------------------------------------------------------*/
/* 関数                                                                */
/*---------------------------------------------------------------------*/
void emfng_pwon( void );                         /* :初期化処理 */
                                                 /* コール元 : emfcnt_pwon() */

void emfng_tdcm( void );                         /* :TDCﾀｲﾐﾝｸﾞ処理           */
                                                 /* コール元 : emfcnt_tdcm() */

void emfng_16msl( void );                        /* :16msﾀｲﾐﾝｸﾞ処理           */
                                                 /* コール元 : emfcnt_16msl() */

void emfng_65msl( void );                        /* :65msﾀｲﾐﾝｸﾞ処理           */
                                                 /* コール元 : emfcnt_65msl() */

void emfng_xjmf_ann_u1( u1 u1t_xjmf );           /* :失火仮判定結果通知処理  */
                                                 /* コール元 : emfcnt_tdcm() */
                                                 /* 引数 u1t_xjmf     :lsb=1 :仮判定結果 */

void emfng_mfptn_ann_u1u1u1( u1 u1t_mfkind, u1 u1t_mfcyl, u1 u1t_xmfne ); /* :検出許可ｶｳﾝﾀの操作 */
                                                 /* コール元 : emfcnt_tdcm()             */
                                                 /* 引数 u1t_mfkind   :lsb=1 :失火種別   */
                                                 /* 引数 u1t_mfcyl    :lsb=1 :失火気筒   */
                                                 /* 引数 u1t_xmfne    :lsb=1 :失火本判定 */

void emfng_reset( void );                        /* :失火判定ﾘｾｯﾄ処理        */
                                                 /* コール元 : emfcnt_tdcm() */

void emfng_drvclchg( void );                     /* ドライビングサイクル移行時の初期化処理 */
                                                 /* コール元 : emfcnt_drvclchg()           */

void emfng_cjpmf_inc_u1( u1 u1t_timming );       /* :ｾﾞﾛｸﾛｽｶｳﾝﾀ操作 */
                                                 /* コール元 : emfccrnk_b30mb60m()                           */
                                                 /*          : emfccrnk_atdc30m()                            */
                                                 /* 引数 u1t_timming  :lsb=1 :ecjpmfカウントアップタイミング */

void emfng_mfmlt_ann_u1u1( u1 u1t_mfkind, u1 u1t_mfcyl );    /* :対向気筒失火通知処理  */
                                                 /* コール元 : emfcnt_neg_mlt()        */
                                                 /* 引数 u1t_mfkind   :lsb=1 :失火種別 */
                                                 /* 引数 u1t_mfcyl    :lsb=1 :失火気筒 */
                                                 /* 注意事項 : 【6気筒】               */

void emfng_exmfotfc_u1( u1 u1t_flg );            /* :FCｵﾌﾟｼｮﾝ判定用触媒OTﾚﾍﾞﾙ異常通知ﾌﾗｸﾞの処理 */
                                                 /* コール元 : emffc_65msl()           */
                                                 /* 引数 u1t_flg :lsb=1 :フラグ情報    */
                                                 /* 注意事項 : 【触媒OT失火異常時FC有】*/

void emfng_cntclr_65msl( void );                 /* 外部参照失火ｶｳﾝﾀのｸﾘｱ処理 */
                                                 /* コール元 : emfcnt_65msl() */

u2 u2g_emfng_eocmfnl( void );                    /* :市場調査RAM 失火回数(小)の頻度 取得関数 */
                                                 /* コール元 : 任意                          */
                                                 /* 戻り値   :lsb=1 ,unit=回 :失火回数(小)の頻度 */
                                                 /* 注意事項 : 【市場調査無】時は0を返す     */

u2 u2g_emfng_eocmfnm( void );                    /* :市場調査RAM 失火回数(中)の頻度 取得関数 */
                                                 /* コール元 : 任意                          */
                                                 /* 戻り値   :lsb=1 ,unit=回 :失火回数(中)の頻度 */
                                                 /* 注意事項 : 【市場調査無】時は0を返す     */

u2 u2g_emfng_eocmfnh( void );                    /* :市場調査RAM 失火回数(大)の頻度 取得関数 */
                                                 /* コール元 : 任意                          */
                                                 /* 戻り値   :lsb=1 ,unit=回 :失火回数(大)の頻度 */
                                                 /* 注意事項 : 【市場調査無】時は0を返す     */

u2 u2g_emfng_eocmfmltl( void );                  /* :市場調査RAM 対向気筒失火回数(小)の頻度 取得関数 */
                                                 /* コール元 : 任意                                  */
                                                 /* 戻り値   :lsb=1 ,unit=回 :対向気筒失火回数(小)の頻度 */
                                                 /* 注意事項 : 【市場調査無】時は0を返す             */

u2 u2g_emfng_eocmfmlth( void );                  /* :市場調査RAM 対向気筒失火回数(大)の頻度 取得関数 */
                                                 /* コール元 : 任意                                  */
                                                 /* 戻り値   :lsb=1 ,unit=回 :対向気筒失火回数(大)の頻度 */
                                                 /* 注意事項 : 【市場調査無】時は0を返す             */

u1 u1g_emfng_eonemfmlt( void );                  /* :市場調査RAM 対向気筒失火時のｴﾝｼﾞﾝ回転なまし値 取得関数 */
                                                 /* コール元 : 任意                                 */
                                                 /* 戻り値   :lsb=200/8 ,unit=rpm :対向気筒失火時のｴﾝｼﾞﾝ回転なまし値 */
                                                 /* 注意事項 : 【市場調査無】時は0を返す            */

u2 u2g_emfng_ecdmfaemTv( void );                 /* :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(全気筒)取得関数 */
                                                 /* コール元 : 任意                                   */
                                                 /* 戻り値   :lsb=1 ,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(全気筒) */
                                                 /* 注意事項 : スケーリングID = 24                    */

u2 u2g_emfng_ecdmfaemMntl( void );               /* :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ下限値(全気筒)取得関数 */
                                                 /* コール元 : 任意                                         */
                                                 /* 戻り値   :lsb=1 ,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ下限値(全気筒) */
                                                 /* 注意事項 : スケーリングID = 24                          */

u2 u2g_emfng_ecdmfaemMxtl( void );               /* :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ上限値(全気筒)取得関数 */
                                                 /* コール元 : 任意                                         */
                                                 /* 戻り値   :lsb=1 ,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ上限値(全気筒) */
                                                 /* 注意事項 : スケーリングID = 24                          */

u2 u2g_emfng_ecdmfemTv( u1 u1t_cyl );            /* :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(気筒別)取得関数 */
                                                 /* コール元 : 任意                                   */
                                                 /* 引数 u1t_cyl    :lsb=1 :気筒番号                  */
                                                 /* 戻り値          :lsb=1 ,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(気筒別) */
                                                 /* 注意事項 : スケーリングID = 24                    */

u2 u2g_emfng_ecdmfemMntl( u1 u1t_cyl );          /* :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ下限値(気筒別)取得関数 */
                                                 /* コール元 : 任意                                         */
                                                 /* 引数 u1t_cyl    :lsb=1 :気筒番号                        */
                                                 /* 戻り値          :lsb=1 ,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ下限値(気筒別) */
                                                 /* 注意事項 : スケーリングID = 24                          */

u2 u2g_emfng_ecdmfemMxtl( u1 u1t_cyl );          /* :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ上限値(気筒別)取得関数 */
                                                 /* コール元 : 任意                                         */
                                                 /* 引数 u1t_cyl    :lsb=1 :気筒番号                        */
                                                 /* 戻り値          :lsb=1 ,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ上限値(気筒別) */
                                                 /* 注意事項 : スケーリングID = 24                          */

u2 u2g_emfng_ecdmfaavTv( void );                 /* :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ(全気筒)取得関数 */
                                                 /* コール元 : 任意                                       */
                                                 /* 戻り値   :lsb=1 ,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ(全気筒) */
                                                 /* 注意事項 : スケーリングID = 24                        */

u2 u2g_emfng_ecdmfaavMntl( void );               /* :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ下限値(全気筒)取得関数 */
                                                 /* コール元 : 任意                                             */
                                                 /* 戻り値   :lsb=1 ,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ下限値(全気筒) */
                                                 /* 注意事項 : スケーリングID = 24                              */

u2 u2g_emfng_ecdmfaavMxtl( void );               /* :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ上限値(全気筒)取得関数 */
                                                 /* コール元 : 任意                                             */
                                                 /* 戻り値   :lsb=1 ,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ上限値(全気筒) */
                                                 /* 注意事項 : スケーリングID = 24                              */

u2 u2g_emfng_ecdmfavTv( u1 u1t_cyl );            /* :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ(気筒別)取得関数 */
                                                 /* コール元 : 任意                                       */
                                                 /* 引数 u1t_cyl    :lsb=1 :気筒番号                      */
                                                 /* 戻り値          :lsb=1 ,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ(気筒別) */
                                                 /* 注意事項 : スケーリングID = 24                        */

u2 u2g_emfng_ecdmfavMntl( u1 u1t_cyl );          /* :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ下限値(気筒別)取得関数 */
                                                 /* コール元 : 任意                                             */
                                                 /* 引数 u1t_cyl    :lsb=1 :気筒番号                            */
                                                 /* 戻り値          :lsb=1 ,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ下限値(気筒別) */
                                                 /* 注意事項 : スケーリングID = 24                              */

u2 u2g_emfng_ecdmfavMxtl( u1 u1t_cyl );          /* :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ上限値(気筒別)取得関数 */
                                                 /* コール元 : 任意                                             */
                                                 /* 引数 u1t_cyl    :lsb=1 :気筒番号                            */
                                                 /* 戻り値          :lsb=1 ,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ上限値(気筒別) */
                                                 /* 注意事項 : スケーリングID = 24                              */

u1  u1g_emfng_eocdmfae2mx( void );               /* :市場調査RAM 1000rev毎の最大総失火回数 取得関数     */
                                                 /* コール元 : 任意                                     */
                                                 /* 戻り値   :lsb=1 ,unit=回 :1000rev毎の最大総失火回数 */
                                                 /* 注意事項 : 【市場調査無】時は0を返す                */

u1  u1g_emfng_eocjrevem( void );                 /* :市場調査RAM 1000rev毎の始動後1000rev回数 取得関数     */
                                                 /* コール元 : 任意                                        */
                                                 /* 戻り値   :lsb=1 ,unit=回 :1000rev毎の始動後1000rev回数 */
                                                 /* 注意事項 : 【市場調査無】時は0を返す                   */

s1  s1g_emfng_eothwem( void );                   /* :市場調査RAM 1000rev毎の水温 取得関数          */
                                                 /* コール元 : 任意                                */
                                                 /* 戻り値   :lsb=160/128,unit=℃ :1000rev毎の水温 */
                                                 /* 注意事項 : 【市場調査無】時は0を返す           */

s1  s1g_emfng_eothaem( void );                   /* :市場調査RAM 1000rev毎の吸気温 取得関数          */
                                                 /* コール元 : 任意                                  */
                                                 /* 戻り値   :lsb=160/128,unit=℃ :1000rev毎の吸気温 */
                                                 /* 注意事項 : 【市場調査無】時は0を返す             */

u1  u1g_emfng_eocdmfem2mx( u1 u1t_cyl );         /* :市場調査RAM 1000rev毎の気筒別失火回数 取得関数    */
                                                 /* コール元 : 任意                                    */
                                                 /* 引数 u1t_cyl    :lsb=1 :気筒番号                   */
                                                 /*  戻り値  :1000rev毎の気筒別失火回数 lsb=1,unit=回  */
                                                 /* 注意事項 : 【市場調査無】時は0を返す               */

u1  u1g_emfng_eospdemav( void );                 /* :市場調査RAM 失火ｶｳﾝﾄ時の平均車速 取得関数            */
                                                 /* コール元 : 任意                                       */
                                                 /* 戻り値   :lsb=256/256,unit=km/h :失火ｶｳﾝﾄ時の平均車速 */
                                                 /* 注意事項 : 【市場調査無】時は0を返す                  */

u1  u1g_emfng_eoneemav( void );                  /* :市場調査RAM 失火ｶｳﾝﾄ時の平均ｴﾝｼﾞﾝ回転数 取得関数             */
                                                 /* コール元 : 任意                                               */
                                                 /* 戻り値   :lsb=12800/256,unit=rpm :失火ｶｳﾝﾄ時の平均ｴﾝｼﾞﾝ回転数 */
                                                 /* 注意事項 : 【市場調査無】時は0を返す                          */

u1  u1g_emfng_eoklsmemav( void );                /* :市場調査RAM 失火ｶｳﾝﾄ時の平均負荷 取得関数           */
                                                 /* コール元 : 任意                                      */
                                                 /* 戻り値   :lsb=320/256,unit=% :失火ｶｳﾝﾄ時の平均負荷   */
                                                 /* 注意事項 : 【市場調査無】時は0を返す                 */

u1  u1g_emfng_eocjmfneemi( void );               /* :市場調査RAM 失火ｶｳﾝﾄ時のｱｲﾄﾞﾙﾓｰﾄﾞ回数 取得関数    */
                                                 /* コール元 : 任意                                    */
                                                 /* 戻り値   :lsb=1,unit=回 :失火ｶｳﾝﾄ時のｱｲﾄﾞﾙﾓｰﾄﾞ回数 */
                                                 /* 注意事項 : 【市場調査無】時は0を返す               */

u1  u1g_emfng_eocjmfneeml( void );               /* :市場調査RAM 失火ｶｳﾝﾄ時のﾛｰﾓｰﾄﾞ回数 取得関数    */
                                                 /* コール元 : 任意                                 */
                                                 /* 戻り値   :lsb=1,unit=回 :失火ｶｳﾝﾄ時のﾛｰﾓｰﾄﾞ回数 */
                                                 /* 注意事項 : 【市場調査無】時は0を返す            */

u1  u1g_emfng_eocjmfneemh( void );               /* :市場調査RAM 失火ｶｳﾝﾄ時のﾊｲﾓｰﾄﾞ回数 取得関数    */
                                                 /* コール元 : 任意                                 */
                                                 /* 戻り値   :lsb=1,unit=回 :失火ｶｳﾝﾄ時のﾊｲﾓｰﾄﾞ回数 */
                                                 /* 注意事項 : 【市場調査無】時は0を返す            */

u1  u1g_emfng_eocdmfaotmx( void );               /* :市場調査RAM 200rev中失火ｶｳﾝﾀの最大値 取得関数    */
                                                 /* コール元 : 任意                                   */
                                                 /* 戻り値   :lsb=1,unit=回 :200rev中失火ｶｳﾝﾀの最大値 */
                                                 /* 注意事項 : 【市場調査無】時は0を返す              */

u1  u1g_emfng_eocjrevot( void );                 /* :市場調査RAM 最大失火ｶｳﾝﾀ更新時の1000rev回数 取得関数 */
                                                 /* コール元 : 任意                               */
                                                 /* 戻り値   :lsb=1,unit=回 :最大失火ｶｳﾝﾀ更新時の1000rev回数 */
                                                 /* 注意事項 : 【市場調査無】時は0を返す          */

s1  s1g_emfng_eothwot( void );                   /* :市場調査RAM 最大失火ｶｳﾝﾀ更新時の水温 取得関数          */
                                                 /* コール元 : 任意                                         */
                                                 /* 戻り値   :lsb=160/128,unit=℃ :最大失火ｶｳﾝﾀ更新時の水温 */
                                                 /* 注意事項 : 【市場調査無】時は0を返す                    */

s1  s1g_emfng_eothaot( void );                   /* :市場調査RAM 最大失火ｶｳﾝﾀ更新時の吸気温 取得関数          */
                                                 /* コール元 : 任意                                           */
                                                 /* 戻り値   :lsb=160/128,unit=℃ :最大失火ｶｳﾝﾀ更新時の吸気温 */
                                                 /* 注意事項 : 【市場調査無】時は0を返す                      */

/*---------------------------------------------------------------------*/
/* 公開変数                                                            */
/*---------------------------------------------------------------------*/
extern u2 u2g_emfng_ekldmfae;        /* m=emfng,lsb=1,unit=回 :EM悪化ﾚﾍﾞﾙ失火判定値 */
                                     /* 更新タイミング : tdcm */

extern u1 u1g_emfng_ecdmfem[u1g_EJCC_NCYL];       /* m=emfng,lsb=1,unit=回 :EM悪化ﾚﾍﾞﾙ気筒別失火ｶｳﾝﾀ */
                                                  /* 更新タイミング : tdcm,ne30m(atdc30m【6気筒】),65msl,drvclchg */

extern u2 u2g_emfng_ecdmfem2[u1g_EJCC_NCYL];      /* m=emfng,lsb=1,unit=回 :EM悪化ﾚﾍﾞﾙ気筒別失火ｶｳﾝﾀ */
                                                  /* 更新タイミング : tdcm,ne30m(atdc30m【6気筒】),65msl,drvclchg */

extern u1 u1g_emfng_ecdmfemi[u1g_EJCC_NCYL];      /* m=emfng,lsb=1,unit=回 :ｱｲﾄﾞﾙ中気筒別失火ｶｳﾝﾀ */
                                                  /* 更新タイミング : tdcm,ne30m(atdc30m【6気筒】),16msl,65msl,drvclchg */
    /* ecdmfem[],ecdmfem2[],ecdmfemi[]のインデックスは気筒順 [0]-[5]:#1#2#3#4#5#6    【V6】     */
    /*                                                       [0]-[5]:#1#2#3#4#5#6    【L6】     */
    /*                                                       [0]-[3]:#1#2#3#4        【4気筒】  */

extern u2 u2g_emfng_ecdmfae2;        /* m=emfng,lsb=1,unit=回 :EM悪化ﾚﾍﾞﾙ全気筒失火ｶｳﾝﾀ */
                                     /* 更新タイミング : tdcm,ne30m(atdc30m【6気筒】),65msl,drvclchg */

extern u1 u1g_emfng_ecdmfaem;        /* m=emfng,lsb=1,unit=回 :EM悪化ﾚﾍﾞﾙ全気筒失火ｶｳﾝﾀ */
                                     /* 更新タイミング : tdcm,ne30m(atdc30m【6気筒】),65msl,drvclchg */

extern u1 u1g_emfng_ecdmfaotbnk[2];  /* m=emfng,lsb=1,unit=回 :ﾊﾞﾝｸ別触媒OTﾚﾍﾞﾙ失火ｶｳﾝﾀ */
                                     /* 更新タイミング : tdcm,ne30m(atdc30m【6気筒】),65msl,drvclchg */

extern u1 u1g_emfng_ecdmfaot;        /* m=emfng,lsb=1,unit=回 :触媒OTﾚﾍﾞﾙ全気筒失火ｶｳﾝﾀ */
                                     /* 更新タイミング : tdcm,ne30m(atdc30m【6気筒】),65msl,drvclchg */

extern u2 u2g_emfng_ecdmfaoti;       /* m=emfng,lsb=1,unit=回 :ｱｲﾄﾞﾙ中全気筒失火ｶｳﾝﾀ */
                                     /* 更新タイミング : tdcm,ne30m(atdc30m【6気筒】),16msl,65msl,drvclchg */

extern u2 u2g_emfng_ecjmfot;         /* m=emfng,lsb=1/3【6気筒】1/2【4気筒】,unit=rev :触媒OTﾚﾍﾞﾙ失火検出許可ｶｳﾝﾀ */
                                     /* 更新タイミング : tdcm,65msl,drvclchg */

extern u2 u2g_emfng_ecjmfoti;        /* m=emfng,lsb=1/3【6気筒】1/2【4気筒】,unit=rev :触媒OTﾚﾍﾞﾙｱｲﾄﾞﾙ時失火検出許可ｶｳﾝﾀ */
                                     /* 更新タイミング : tdcm,16msl,65msl,drvclchg */

extern u2 u2g_emfng_ecjmfem;         /* m=emfng,lsb=1/3【6気筒】1/2【4気筒】,unit=rev :EM悪化ﾚﾍﾞﾙ失火検出許可ｶｳﾝﾀ */
                                     /* 更新タイミング : tdcm,65msl,drvclchg */

extern u2 u2g_emfng_ecmfot;          /* m=emfng,lsb=1,unit=回 :複数気筒失火ｶｳﾝﾀ */
                                     /* 更新タイミング : tdcm,ne30m(atdc30m【6気筒】),65msl,drvclchg */

extern u1 u1g_emfng_ecdmfw[u1g_EJCC_NCYL/2];      /* m=emfng,lsb=1,unit=回 :対向気筒失火ｶｳﾝﾀ */
                                                  /* 更新タイミング : tdcm,ne30m(atdc30m【6気筒】),65msl,drvclchg */

extern u1 u1g_emfng_exdmffc;         /* m=emfng,lsb=1 :FCｵﾌﾟｼｮﾝ判定用触媒OTﾚﾍﾞﾙ異常通知ﾌﾗｸﾞ */
                                     /* 更新タイミング : 65msl */
                                     /* 注意事項 : ON=異常 */
                                     /*          :【触媒OT失火異常時FC有】*/

extern u1 u1g_emfng_ecdtmf2;         /* m=emfng,lsb=1,unit=回 :触媒OTﾚﾍﾞﾙ判定用悪路ｶｳﾝﾀ */
                                     /* 更新タイミング : tdcm,65ms,drvclchg */

extern u2 u2g_emfng_ekldmfot;        /* m=emfng,lsb=1,unit=回 :触媒OTﾚﾍﾞﾙ失火回数判定値 */
                                     /* 更新タイミング : 65ms */

extern u2 u2g_emfng_ekldmfoth_mcr;   /* m=emfng,lsb=1,unit=回 :触媒OTﾚﾍﾞﾙ失火回数判定値(計算値) */
                                     /* 更新タイミング : pwon,65ms */

extern u2 u2g_emfng_eldmfaem_mcr;    /* m=emfng,lsb=1,unit=回 :EM悪化ﾚﾍﾞﾙ失火回数判定値(計算値) */
                                     /* 更新タイミング : pwon,65ms */

extern stflag8 stg_emfng_flag1;                     /* m=emfng :グローバルフラグ用構造体 */
#define big_emfng_exjokmf     (stg_emfng_flag1.b0)  /* m=emfng :始動後1000rev完了ﾌﾗｸﾞ  */
                                                    /* 更新タイミング : 65msl,drvclchg */
                                                    /* 注意事項       : ON=完了        */
#define big_emfng_excdmfw     (stg_emfng_flag1.b1)  /* m=emfng :対向気筒失火判定成立ﾌﾗｸﾞ */
                                                    /* 更新タイミング : 65msl            */
                                                    /* 注意事項       : ON=成立          */
                                                    /*                :【4気筒】はOFF固定*/
#define big_emfng_excdmfemclr (stg_emfng_flag1.b2)  /* m=emfng :ecdmfemｸﾘｱ要求ﾌﾗｸﾞ */
                                                    /* 更新タイミング : 65msl      */
                                                    /* 注意事項       : ON=成立    */
#define big_emfng_exhstmf     (stg_emfng_flag1.b3)  /* m=emfng :ﾎｯﾄｽﾀｰﾄ記憶ﾌﾗｸﾞ */
                                                    /* 更新タイミング : 16msl   */
                                                    /* 注意事項       : ON=成立 */
#define big_emfng_excstmf     (stg_emfng_flag1.b4)  /* m=emfng :ｺｰﾙﾄﾞｽﾀｰﾄ記憶ﾌﾗｸﾞ */
                                                    /* 更新タイミング : 16msl     */
                                                    /* 注意事項       : ON=成立   */
#define big_emfng_exdisable   (stg_emfng_flag1.b5)  /* m=emfng :失火 Disableﾌﾗｸﾞ */
                                                    /* 更新タイミング : 65msl    */
                                                    /* 注意事項       : ON=成立  */
#define big_emfng_excomplete  (stg_emfng_flag1.b6)  /* m=emfng :失火 Completeﾌﾗｸﾞ */
                                                    /* 更新タイミング : 65msl     */
                                                    /* 注意事項       : ON=完了   */

extern stflag8 stg_emfng_flag2;                     /* m=emfng :グローバルフラグ用構造体 */
#define big_emfng_exmfddi     (stg_emfng_flag2.b0)  /* m=emfng :DI異常ﾌﾗｸﾞ             */
                                                    /* 更新タイミング : 65msl,drvclchg */
                                                    /* 注意事項       : ON=異常        */
                                                    /*                :【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/
#define big_emfng_exmfdied    (stg_emfng_flag2.b1)  /* m=emfng :DI失火検出終了ﾌﾗｸﾞ     */
                                                    /* 更新タイミング : 65msl,drvclchg */
                                                    /* 注意事項       : ON=完了        */
                                                    /*                :【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/
#define big_emfng_exmfdpfi    (stg_emfng_flag2.b2)  /* m=emfng :PFI異常ﾌﾗｸﾞ            */
                                                    /* 更新タイミング : 65msl,drvclchg */
                                                    /* 注意事項       : ON=異常        */
                                                    /*                :【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/
#define big_emfng_exmfpfied   (stg_emfng_flag2.b3)  /* m=emfng :PFI失火検出終了ﾌﾗｸﾞ */
                                                    /* 更新タイミング : 65msl,drvclchg */
                                                    /* 注意事項       : ON=完了        */
                                                    /*                :【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/

extern stflag8 stg_emfng_flag3;                     /* m=emfng :グローバルフラグ用構造体 */
#define big_emfng_exdmf       (stg_emfng_flag3.b0)  /* m=emfng :失火OBD現在異常ﾌﾗｸﾞ */
                                                    /* 更新タイミング : 65msl,drvclchg */
                                                    /* 注意事項       : ON=異常 */
                                                    /*                :【t-CORE対応有】*/

/*---------------------------------------------------------------------*/
/* 公開適合定数                                                        */
/*---------------------------------------------------------------------*/
/* 適合定数 */
extern volatile const u1 u1g_emfng_CDTMFWA;     /* m=emfng,lsb=1,unit=回 :EM悪化判定用悪路ｶｳﾝﾀ判定値 */
extern volatile const u1 u1g_emfng_CATN;        /* m=emfng,lsb=1 :バンク数区別の有無(無:1,有:2) */
extern volatile const s2 s2g_emfng_MFOTL;       /* m=emfng,lsb=256/128/256,unit=% :触媒OTﾚﾍﾞﾙ失火判定下限値 */

/* 固定定数 */
extern volatile const u2 u2g_emfng_LJMFOT;      /* m=emfng,lsb=2/u1g_EJCC_NCYL,unit=rev :触媒OTﾚﾍﾞﾙ異常判定実行ﾀｲﾐﾝｸﾞ */
extern volatile const u2 u2g_emfng_LJMFEM;      /* m=emfng,lsb=2/u1g_EJCC_NCYL,unit=rev :EM悪化ﾚﾍﾞﾙ異常判定実行ﾀｲﾐﾝｸﾞ */

/* バックアップRAM初期値 */
extern volatile const u2 u2g_emfng_ECDMFAEMTV_MI;       /* m=emfng,lsb=1,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(全気筒)初期値 */
extern volatile const u2 u2g_emfng_ECDMFAEMMXTL_MI;     /* m=emfng,lsb=1,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ上限値(全気筒)初期値 */
extern volatile const u2 u2g_emfng_ECDMFAEMMNTL_MI;     /* m=emfng,lsb=1,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ下限値(全気筒)初期値 */
extern volatile const u2 u2g_emfng_ECDMFEMTV_MI;        /* m=emfng,lsb=1,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ(気筒別)初期値 */
extern volatile const u2 u2g_emfng_ECDMFEMMXTL_MI;      /* m=emfng,lsb=1,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ上限値(気筒別)初期値 */
extern volatile const u2 u2g_emfng_ECDMFEMMNTL_MI;      /* m=emfng,lsb=1,unit=回 :ﾃｽﾄ結果出力用今ﾄﾘｯﾌﾟ累積失火ｶｳﾝﾀ下限値(気筒別)初期値 */
extern volatile const u2 u2g_emfng_ECDMFAAVTV_MI;       /* m=emfng,lsb=1,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ(全気筒)初期値 */
extern volatile const u2 u2g_emfng_ECDMFAAVMXTL_MI;     /* m=emfng,lsb=1,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ上限値(全気筒)初期値 */
extern volatile const u2 u2g_emfng_ECDMFAAVMNTL_MI;     /* m=emfng,lsb=1,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ下限値(全気筒)初期値 */
extern volatile const u2 u2g_emfng_ECDMFAVTV_MI;        /* m=emfng,lsb=1,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ(気筒別)初期値 */
extern volatile const u2 u2g_emfng_ECDMFAVMXTL_MI;      /* m=emfng,lsb=1,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ上限値(気筒別)初期値 */
extern volatile const u2 u2g_emfng_ECDMFAVMNTL_MI;      /* m=emfng,lsb=1,unit=回 :ﾃｽﾄ結果出力用過去10ﾄﾘｯﾌﾟEWMA失火ｶｳﾝﾀ下限値(気筒別)初期値 */
extern volatile const u1 u1g_emfng_EXRCDMFI_MI;         /* m=emfng :EWMA失火ｶｳﾝﾀ計算履歴フラグ初期値 */
extern volatile const u2 u2g_emfng_EOCMFNL_MI;          /* m=emfng,lsb=1,unit=回 :失火回数(小)の頻度 初期値 */
                                                        /* 注意事項 : 【市場調査有】*/
extern volatile const u2 u2g_emfng_EOCMFNM_MI;          /* m=emfng,lsb=1,unit=回 :失火回数(中)の頻度 初期値 */
                                                        /* 注意事項 : 【市場調査有】*/
extern volatile const u2 u2g_emfng_EOCMFNH_MI;          /* m=emfng,lsb=1,unit=回 :失火回数(大)の頻度 初期値 */
                                                        /* 注意事項 : 【市場調査有】*/
extern volatile const u2 u2g_emfng_EOCMFMLTL_MI;        /* m=emfng,lsb=1,unit=回 :対向気筒失火回数(小)の頻度 初期値 */
                                                        /* 注意事項 : 【市場調査有】*/
extern volatile const u2 u2g_emfng_EOCMFMLTH_MI;        /* m=emfng,lsb=1,unit=回 :対向気筒失火回数(大)の頻度 初期値 */
                                                        /* 注意事項 : 【市場調査有】*/
extern volatile const u1 u1g_emfng_EONEMFMLT_MI;        /* m=emfng,lsb=200/8,unit=rpm :対向気筒失火時のｴﾝｼﾞﾝ回転なまし値 初期値 */
                                                        /* 注意事項 : 【市場調査有】*/
extern volatile const u1 u1g_emfng_EOCDMFAE2MX_MI;      /* m=emfng,lsb=1,unit=回 :1000rev中失火ｶｳﾝﾀの最大値 初期値 */
                                                        /* 注意事項 : 【市場調査有】*/
extern volatile const u1 u1g_emfng_EOCJREVEM_MI;        /* m=emfng,lsb=1,unit=回 :最大失火ｶｳﾝﾀ更新時の1000rev回数 初期値 */
                                                        /* 注意事項 : 【市場調査有】*/
extern volatile const s1 s1g_emfng_EOTHWEM_MI;          /* m=emfng,lsb=160/128,unit=℃ :最大失火ｶｳﾝﾀ更新時の水温 初期値 */
                                                        /* 注意事項 : 【市場調査有】*/
extern volatile const s1 s1g_emfng_EOTHAEM_MI;          /* m=emfng,lsb=160/128,unit=℃ :最大失火ｶｳﾝﾀ更新時の吸気温 初期値 */
                                                        /* 注意事項 : 【市場調査有】*/
extern volatile const u1 u1g_emfng_EOCDMFEM2MX_MI;      /* m=emfng,lsb=1,unit=回 :最大失火ｶｳﾝﾀ更新時の気筒別失火ｶｳﾝﾀ 初期値 */
                                                        /* 注意事項 : 【市場調査有】*/
extern volatile const u1 u1g_emfng_EOSPDEMAV_MI;        /* m=emfng,lsb=256/256,unit=km/h :失火ｶｳﾝﾄ時の平均車速 初期値 */
                                                        /* 注意事項 : 【市場調査有】*/
extern volatile const u1 u1g_emfng_EONEEMAV_MI;         /* m=emfng,lsb=12800/256,unit=rpm :失火ｶｳﾝﾄ時の平均ｴﾝｼﾞﾝ回転数 初期値 */
                                                        /* 注意事項 : 【市場調査有】*/
extern volatile const u1 u1g_emfng_EOKLSMEMAV_MI;       /* m=emfng,lsb=320/256,unit=% :失火ｶｳﾝﾄ時の平均負荷 初期値 */
                                                        /* 注意事項 : 【市場調査有】*/
extern volatile const u1 u1g_emfng_EOCJMFNEEMI_MI;      /* m=emfng,lsb=1,unit=回 :失火ｶｳﾝﾄ時のｱｲﾄﾞﾙﾓｰﾄﾞ回数 初期値 */
                                                        /* 注意事項 : 【市場調査有】*/
extern volatile const u1 u1g_emfng_EOCJMFNEEML_MI;      /* m=emfng,lsb=1,unit=回 :失火ｶｳﾝﾄ時のﾛｰﾓｰﾄﾞ回数 初期値 */
                                                        /* 注意事項 : 【市場調査有】*/
extern volatile const u1 u1g_emfng_EOCJMFNEEMH_MI;      /* m=emfng,lsb=1,unit=回 :失火ｶｳﾝﾄ時のﾊｲﾓｰﾄﾞ回数 初期値 */
                                                        /* 注意事項 : 【市場調査有】*/
extern volatile const u1 u1g_emfng_EOCDMFAOTMX_MI;      /* m=emfng,lsb=1,unit=回 :200rev中最大ｶｳﾝﾀ最大値  初期値 */
                                                        /* 注意事項 : 【市場調査有】*/
extern volatile const u1 u1g_emfng_EOCJREVOT_MI;        /* m=emfng,lsb=1,unit=回 :最大失火ｶｳﾝﾀ更新時の1000rev回数 初期値 */
                                                        /* 注意事項 : 【市場調査有】*/
extern volatile const s1 s1g_emfng_EOTHWOT_MI;          /* m=emfng,lsb=160/128,unit=℃ :最大失火ｶｳﾝﾀ更新時の水温 初期値 */
                                                        /* 注意事項 : 【市場調査有】*/
extern volatile const s1 s1g_emfng_EOTHAOT_MI;          /* m=emfng,lsb=160/128,unit=℃ :最大失火ｶｳﾝﾀ更新時の吸気温 初期値 */
                                                        /* 注意事項 : 【市場調査有】*/

/* バックアップRAM-IDと初期値は以下のように設定すること */
/* １バイトバックアップRAM */
/* u2g_EMFNG_EONEMFMLT_U1_ID       u1g_emfng_EONEMFMLT_MI     【市場調査有】                           */
/* u2g_EMFNG_EXRCDMF_U1_ID         u1g_emfng_EXRCDMFI_MI                                               */
/* u2g_EMFNG_EOCDMFAE2MX_U1_ID     u1g_emfng_EOCDMFAE2MX_MI   【市場調査有】                           */
/* u2g_EMFNG_EOCJREVEM_U1_ID       u1g_emfng_EOCJREVEM_MI     【市場調査有】                           */
/* u2g_EMFNG_EOTHWEM_S1_ID         s1g_emfng_EOTHWEM_MI       【市場調査有】                           */
/* u2g_EMFNG_EOTHAEM_S1_ID         s1g_emfng_EOTHAEM_MI       【市場調査有】                           */
/* u2g_EMFNG_EOCDMFEM2MX1_U1_ID    u1g_emfng_EOCDMFEM2MX_MI   【市場調査有】                           */
/* u2g_EMFNG_EOCDMFEM2MX2_U1_ID    u1g_emfng_EOCDMFEM2MX_MI   【市場調査有】                           */
/* u2g_EMFNG_EOCDMFEM2MX3_U1_ID    u1g_emfng_EOCDMFEM2MX_MI   【市場調査有】                           */
/* u2g_EMFNG_EOCDMFEM2MX4_U1_ID    u1g_emfng_EOCDMFEM2MX_MI   【市場調査有】                           */
/* u2g_EMFNG_EOCDMFEM2MX5_U1_ID    u1g_emfng_EOCDMFEM2MX_MI   【市場調査有】AND【6気筒】               */
/* u2g_EMFNG_EOCDMFEM2MX6_U1_ID    u1g_emfng_EOCDMFEM2MX_MI   【市場調査有】AND【6気筒】               */
/* u2g_EMFNG_EOSPDEMAV_U1_ID       u1g_emfng_EOSPDEMAV_MI     【市場調査有】                           */
/* u2g_EMFNG_EONEEMAV_U1_ID        u1g_emfng_EONEEMAV_MI      【市場調査有】                           */
/* u2g_EMFNG_EOKLSMEMAV_U1_ID      u1g_emfng_EOKLSMEMAV_MI    【市場調査有】                           */
/* u2g_EMFNG_EOCJMFNEEMI_U1_ID     u1g_emfng_EOCJMFNEEMI_MI   【市場調査有】                           */
/* u2g_EMFNG_EOCJMFNEEML_U1_ID     u1g_emfng_EOCJMFNEEML_MI   【市場調査有】                           */
/* u2g_EMFNG_EOCJMFNEEMH_U1_ID     u1g_emfng_EOCJMFNEEMH_MI   【市場調査有】                           */
/* u2g_EMFNG_EOCDMFAOTMX_U1_ID     u1g_emfng_EOCDMFAOTMX_MI   【市場調査有】                           */
/* u2g_EMFNG_EOCJREVOT_U1_ID       u1g_emfng_EOCJREVOT_MI     【市場調査有】                           */
/* u2g_EMFNG_EOTHWOT_S1_ID         s1g_emfng_EOTHWOT_MI       【市場調査有】                           */
/* u2g_EMFNG_EOTHAOT_S1_ID         s1g_emfng_EOTHAOT_MI       【市場調査有】                           */
/* ２バイトバックアップRAM */
/* u2g_EMFNG_EOCMFNL_U2_ID         u2g_emfng_EOCMFNL_MI       【市場調査有】                           */
/* u2g_EMFNG_EOCMFNM_U2_ID         u2g_emfng_EOCMFNM_MI       【市場調査有】                           */
/* u2g_EMFNG_EOCMFNH_U2_ID         u2g_emfng_EOCMFNH_MI       【市場調査有】                           */
/* u2g_EMFNG_EOCMFMLTL_U2_ID       u2g_emfng_EOCMFMLTL_MI     【市場調査有】                           */
/* u2g_EMFNG_EOCMFMLTH_U2_ID       u2g_emfng_EOCMFMLTH_MI     【市場調査有】                           */
/* u2g_EMFNG_ECDMFAEMMXTL_U2_ID    u2g_emfng_ECDMFAEMMXTL_MI                                           */
/* u2g_EMFNG_ECDMFEMMXTL1_U2_ID    u2g_emfng_ECDMFEMMXTL_MI                                            */
/* u2g_EMFNG_ECDMFEMMXTL2_U2_ID    u2g_emfng_ECDMFEMMXTL_MI                                            */
/* u2g_EMFNG_ECDMFEMMXTL3_U2_ID    u2g_emfng_ECDMFEMMXTL_MI                                            */
/* u2g_EMFNG_ECDMFEMMXTL4_U2_ID    u2g_emfng_ECDMFEMMXTL_MI                                            */
/* u2g_EMFNG_ECDMFEMMXTL5_U2_ID    u2g_emfng_ECDMFEMMXTL_MI   【6気筒】                                */
/* u2g_EMFNG_ECDMFEMMXTL6_U2_ID    u2g_emfng_ECDMFEMMXTL_MI   【6気筒】                                */
/* u2g_EMFNG_ECDMFAAVMXTL_U2_ID    u2g_emfng_ECDMFAAVMXTL_MI                                           */
/* u2g_EMFNG_ECDMFAVMXTL1_U2_ID    u2g_emfng_ECDMFAVMXTL_MI                                            */
/* u2g_EMFNG_ECDMFAVMXTL2_U2_ID    u2g_emfng_ECDMFAVMXTL_MI                                            */
/* u2g_EMFNG_ECDMFAVMXTL3_U2_ID    u2g_emfng_ECDMFAVMXTL_MI                                            */
/* u2g_EMFNG_ECDMFAVMXTL4_U2_ID    u2g_emfng_ECDMFAVMXTL_MI                                            */
/* u2g_EMFNG_ECDMFAVMXTL5_U2_ID    u2g_emfng_ECDMFAVMXTL_MI   【6気筒】                                */
/* u2g_EMFNG_ECDMFAVMXTL6_U2_ID    u2g_emfng_ECDMFAVMXTL_MI   【6気筒】                                */
/* u2g_EMFNG_ECDMFAEMTV_U2_ID      u2g_emfng_ECDMFAEMTV_MI                                             */
/* u2g_EMFNG_ECDMFAEMMNTL_U2_ID    u2g_emfng_ECDMFAEMMNTL_MI                                           */
/* u2g_EMFNG_ECDMFAAVTV_U2_ID      u2g_emfng_ECDMFAAVTV_MI                                             */
/* u2g_EMFNG_ECDMFAAVMNTL_U2_ID    u2g_emfng_ECDMFAAVMNTL_MI                                           */
/* u2g_EMFNG_ECDMFEMTV1_U2_ID      u2g_emfng_ECDMFEMTV_MI                                              */
/* u2g_EMFNG_ECDMFEMTV2_U2_ID      u2g_emfng_ECDMFEMTV_MI                                              */
/* u2g_EMFNG_ECDMFEMTV3_U2_ID      u2g_emfng_ECDMFEMTV_MI                                              */
/* u2g_EMFNG_ECDMFEMTV4_U2_ID      u2g_emfng_ECDMFEMTV_MI                                              */
/* u2g_EMFNG_ECDMFEMTV5_U2_ID      u2g_emfng_ECDMFEMTV_MI     【6気筒】                                */
/* u2g_EMFNG_ECDMFEMTV6_U2_ID      u2g_emfng_ECDMFEMTV_MI     【6気筒】                                */
/* u2g_EMFNG_ECDMFEMMNTL1_U2_ID    u2g_emfng_ECDMFEMMNTL_MI                                            */
/* u2g_EMFNG_ECDMFEMMNTL2_U2_ID    u2g_emfng_ECDMFEMMNTL_MI                                            */
/* u2g_EMFNG_ECDMFEMMNTL3_U2_ID    u2g_emfng_ECDMFEMMNTL_MI                                            */
/* u2g_EMFNG_ECDMFEMMNTL4_U2_ID    u2g_emfng_ECDMFEMMNTL_MI                                            */
/* u2g_EMFNG_ECDMFEMMNTL5_U2_ID    u2g_emfng_ECDMFEMMNTL_MI   【6気筒】                                */
/* u2g_EMFNG_ECDMFEMMNTL6_U2_ID    u2g_emfng_ECDMFEMMNTL_MI   【6気筒】                                */
/* u2g_EMFNG_ECDMFAVTV1_U2_ID      u2g_emfng_ECDMFAVTV_MI                                              */
/* u2g_EMFNG_ECDMFAVTV2_U2_ID      u2g_emfng_ECDMFAVTV_MI                                              */
/* u2g_EMFNG_ECDMFAVTV3_U2_ID      u2g_emfng_ECDMFAVTV_MI                                              */
/* u2g_EMFNG_ECDMFAVTV4_U2_ID      u2g_emfng_ECDMFAVTV_MI                                              */
/* u2g_EMFNG_ECDMFAVTV5_U2_ID      u2g_emfng_ECDMFAVTV_MI     【6気筒】                                */
/* u2g_EMFNG_ECDMFAVTV6_U2_ID      u2g_emfng_ECDMFAVTV_MI     【6気筒】                                */
/* u2g_EMFNG_ECDMFAVMNTL1_U2_ID    u2g_emfng_ECDMFAVMNTL_MI                                            */
/* u2g_EMFNG_ECDMFAVMNTL2_U2_ID    u2g_emfng_ECDMFAVMNTL_MI                                            */
/* u2g_EMFNG_ECDMFAVMNTL3_U2_ID    u2g_emfng_ECDMFAVMNTL_MI                                            */
/* u2g_EMFNG_ECDMFAVMNTL4_U2_ID    u2g_emfng_ECDMFAVMNTL_MI                                            */
/* u2g_EMFNG_ECDMFAVMNTL5_U2_ID    u2g_emfng_ECDMFAVMNTL_MI   【6気筒】                                */
/* u2g_EMFNG_ECDMFAVMNTL6_U2_ID    u2g_emfng_ECDMFAVMNTL_MI   【6気筒】                                */

/*-------------------------------------------------------------------*/
/* 非公開適合定数（部品内データ）                                    */
/*-------------------------------------------------------------------*/
extern volatile const s2 s2s_emfng_MFDD;           /* m=emfng,lsb=256/128/256,unit=% :EM悪化ﾚﾍﾞﾙ失火率 */
extern volatile const u2 u2s_emfng_MFD1;           /* m=emfng,lsb=2/256/256 :EM悪化ﾚﾍﾞﾙ失火検出率 */
extern volatile const s2 s2s_emfng_MFNEL3;         /* m=emfng,lsb=12800/64/256,unit=rpm :アイドル領域判定値 */
extern volatile const u2 u2s_emfng_LJMFOTI;        /* m=emfng,lsb=2/u1g_EJCC_NCYL,unit=rev :触媒OTﾚﾍﾞﾙｱｲﾄﾞﾙ時異常判定実行ﾀｲﾐﾝｸﾞ */
extern volatile const u2 u2s_emfng_LJMFOTF;        /* m=emfng,lsb=2/u1g_EJCC_NCYL,unit=rev :触媒OTﾚﾍﾞﾙ異常判定実行ﾀｲﾐﾝｸﾞ */
extern volatile const u1 u1s_emfng_CXMFH;          /* m=emfng,lsb=1,unit=回 :EM悪化ﾚﾍﾞﾙ異常検出回数判定値 */
extern volatile const u1 u1s_emfng_CXMFL;          /* m=emfng,lsb=1,unit=回 :EM悪化ﾚﾍﾞﾙ異常検出回数判定値 */
extern volatile const u1 u1s_emfng_CXMF1H;         /* m=emfng,lsb=1,unit=回 :触媒OTﾚﾍﾞﾙ異常検出回数判定値 */
extern volatile const u1 u1s_emfng_CXMF1L;         /* m=emfng,lsb=1,unit=回 :触媒OTﾚﾍﾞﾙ異常検出回数判定値 */
extern volatile const u1 u1s_emfng_MFSTOT;         /* m=emfng,lsb=1,unit=回 :気筒別失火回数判定値 */
extern volatile const u1 u1s_emfng_MFSTEMI;        /* m=emfng,lsb=1,unit=回 :ｱｲﾄﾞﾙ中気筒別失火回数判定値 */
extern volatile const u1 u1s_emfng_CJPMF;          /* m=emfng,lsb=1,unit=回 :ｾﾞﾛｸﾛｽｶｳﾝﾀ判定値 */
extern volatile const u1 u1s_emfng_CDTMF2;         /* m=emfng,lsb=1,unit=回 :触媒OT判定用悪路ｶｳﾝﾀ判定値 */
extern volatile const u1 u1s_emfng_KLDMFOT;        /* m=emfng,lsb=1,unit=回 :触媒OTﾚﾍﾞﾙ失火回数判定値(100点火/1気筒辺り) */
extern volatile const u1 u1s_emfng_KLD1;           /* m=emfng,lsb=2/256 :対向気筒失火検出率下限 */
extern volatile const u1 u1s_emfng_KLD2;           /* m=emfng,lsb=2/256 :対向気筒失火検出率上限 */
                                                   /* 注意事項 :【6気筒】*/
extern volatile const u1 u1s_emfng_ST1RTO;         /* m=emfng,lsb=8/256,unit=倍 :ｺｰﾙﾄﾞｽﾀｰﾄ時EM悪化ﾚﾍﾞﾙ失火回数判定値算出係数 */
extern volatile const u1 u1s_emfng_ST2RTO;         /* m=emfng,lsb=8/256,unit=倍 :ﾎｯﾄｽﾀｰﾄ時EM悪化ﾚﾍﾞﾙ失火回数判定値算出係数 */
extern volatile const u1 u1s_emfng_KMFK;           /* m=emfng,lsb=2/256,unit=倍 :DTC出力気筒判定係数 */
extern volatile const u1 u1s_emfng_KCDTMFWA;       /* m=emfng,lsb=8/256,unit=倍 :EM悪化判定用悪路ｶｳﾝﾀ減算係数 */
extern volatile const u1 u1s_emfng_KCDTMFAOTB;     /* m=emfng,lsb=8/256,unit=倍 :触媒OT判定用悪路ｶｳﾝﾀ減算係数(ﾊﾞﾝｸ別) */
extern volatile const u2 u2s_emfng_KLEVEL3;        /* m=emfng,lsb=4/256,unit=倍 :LEVEL3要求時の失火回数反映係数 */
extern volatile const u1 u1s_emfng_MFDDI;          /* m=emfng,lsb=1,unit=回 :DI異常検出失火回数 */
                                                   /* 注意事項 :【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/
extern volatile const u1 u1s_emfng_MFDPFI;         /* m=emfng,lsb=1,unit=回 :PFI異常検出失火回数 */
                                                   /* 注意事項 :【ﾃﾞｭｱﾙINJ】AND【温間ﾎﾟｰﾄ噴射有】*/
extern volatile const s2 s2s_emfng_THW;            /* m=emfng,lsb=160/256,unit=℃ :ﾎｯﾄｽﾀｰﾄ判定水温 */
extern volatile const s2 s2s_emfng_THA;            /* m=emfng,lsb=160/256,unit=℃ :ﾎｯﾄｽﾀｰﾄ判定吸気温 */
extern volatile const s2 s2s_emfng_THWCSTL;        /* m=emfng,lsb=160/256,unit=℃ :ｺｰﾙﾄﾞｽﾀｰﾄ判定水温 */
extern volatile const s2 s2s_emfng_THWCSTH;        /* m=emfng,lsb=160/256,unit=℃ :ｺｰﾙﾄﾞｽﾀｰﾄ判定水温 */
extern volatile const s2 s2s_emfng_THACST;         /* m=emfng,lsb=160/256,unit=℃ :ｺｰﾙﾄﾞｽﾀｰﾄ判定吸気温 */
extern volatile const u1 u1s_emfng_CJEMREV;        /* m=emfng,lsb=1,unit=回 :EM悪化ﾚﾍﾞﾙ失火回数嵩上げ1000rev回数 */
extern volatile const s2 s2s_emfng_KCDMFWL;        /* m=emfng,lsb=2/64/256 :ﾃｽﾄ結果出力用 対向気筒失火ｶｳﾝﾀ加算判定値 */
                                                   /* 注意事項 :【6気筒】*/
extern volatile const u1 u1s_emfng_MFLOW;          /* m=emfng,lsb=2/256 :失火回数小判定係数 */
                                                   /* 注意事項 :【市場調査有】*/
extern volatile const u1 u1s_emfng_MFMID;          /* m=emfng,lsb=2/256 :失火回数中判定係数 */
                                                   /* 注意事項 :【市場調査有】*/
extern volatile const u1 u1s_emfng_MFHI;           /* m=emfng,lsb=2/256 :失火回数大判定係数 */
                                                   /* 注意事項 :【市場調査有】*/

/*---------------------------------------------------------------------*/
/* 公開マップ                                                          */
/*---------------------------------------------------------------------*/
/* テーブル */
extern volatile const u1 u1g_emfng_mfwcyl_tbl[u1g_EJCC_NCYL/2]; /* m=emfng:cylwに対応するmfcyl ←→ cylw変換テーブル */
/* マップ */
extern volatile const s2 s2g_emfng_emfrtot_map[];  /* m=emfng,lsb=256/128/256,unit=% :積算失火判定値補正係数マップ */
extern volatile const s2 s2g_emfng_emfrtotgpf_map[];    /* m=emfng,lsb=256/128/256,unit=% :積算失火判定値補正係数マップ */
                                                        /* 注意事項:【GPF有】*/

/*-------------------------------------------------------------------*/
/* 非公開マップ（部品内データ）                                      */
/*-------------------------------------------------------------------*/
extern volatile const u1 u1s_emfng_mfcylbnk_tbl[u1g_EJCC_NCYL]; /* m=emfng:mfcyl → バンク 変換テーブル */

/*---------------------------------------------------------------------*/
/* オートカウンタ                                                      */
/*---------------------------------------------------------------------*/

/*---------------------------------------------------------------------*/
/* マクロ定義                                                          */
/*---------------------------------------------------------------------*/
/* $$$マクロ定数_標準_v4.11 */
/* $$$マクロ定数_標準_v4.11 */

#endif /* EMFNG_H */
/**** End of File ****************************************************/
