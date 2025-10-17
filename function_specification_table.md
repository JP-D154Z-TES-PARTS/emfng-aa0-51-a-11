# emfng_l_mat.c 関数一覧と仕様書対応表

## 1. 関数一覧

本文書は、emfng_l_mat.cに定義されている全関数と、仕様書（D0emfng-aa0-50-a.doc）との対応関係を一覧化したものである。

### 1.1 ファイル情報

- **ファイル名**: emfng_l_mat.c
- **総行数**: 4,395行
- **関数数**: 60個
- **部品番号**: emfng-pa000-5000-a-M4

## 2. 関数対応表

### 2.1 初期化・制御関数

| No | 関数名 | 行番号 | 呼び出しタイミング | 仕様書対応セクション | 処理概要 |
|----|--------|--------|-------------------|---------------------|---------|
| 1 | `emfng_pwon()` | 545 | 電源ON時 | 初期化処理 | 計算値変数の初期算出 |
| 2 | `emfng_reset()` | 1121 | リセット時 | リセット処理 | カウンタ・フラグのリセット |
| 3 | `emfng_drvclchg()` | 1137 | 運転条件変化時 | 運転条件変化処理 | 計算値の再計算 |
| 4 | `emfng_tdcm()` | 753 | TDC(中) | TDC周期処理 | TDCタイミング処理の統括 |
| 5 | `emfng_16msl()` | 1215 | 16ms(低) | 16ms周期処理 | 16ms周期処理の統括 |
| 6 | `emfng_65msl()` | 1287 | 65ms(低) | 65ms周期処理 | 65ms周期処理の統括 |

### 2.2 検出許可カウンタ操作関数

| No | 関数名 | 行番号 | パラメータ | 仕様書対応 | 処理概要 |
|----|--------|--------|-----------|-----------|---------|
| 7 | `emfng_mfptn_ann_u1u1u1()` | 564 | u1t_mfkind, u1t_mfcyl, u1t_xmfne | １．検出許可カウンタの操作 | 失火パターン判定時のカウンタ操作 |
| 8 | `emfng_cjpmf_inc_u1()` | 848 | u1t_timming | 保持カウンタ更新 | 保持カウンタの増加処理 |

**詳細対応**:

`emfng_mfptn_ann_u1u1u1()` 関数内で実施される処理：

| 処理内容 | 対応変数 | 仕様書参照 |
|---------|---------|-----------|
| 触媒OTレベル失火検出許可カウンタ | `u2g_emfng_ecjmfot` | １－１．ecjmfot |
| 触媒OTレベルアイドル時許可カウンタ | `u2g_emfng_ecjmfoti` | １－２．ecjmfoti |
| EM悪化レベル失火検出許可カウンタ | `u2g_emfng_ecjmfem` | １－３．ecjmfem |
| DI失火検出許可カウンタ | `u2s_emfng_ecjmfdi` | １－４．ecjmfdi |
| PFI失火検出許可カウンタ | `u2s_emfng_ecjmfpfi` | １－５．ecjmfpfi |

### 2.3 失火カウンタ操作関数

| No | 関数名 | 行番号 | パラメータ | 仕様書対応 | 処理概要 |
|----|--------|--------|-----------|-----------|---------|
| 9 | `emfng_mf_cnt_u1u1()` | 925 | u1t_mfkind, u1t_mfcyl | ２．失火カウンタの操作 | 失火種別・気筒別のカウンタ更新 |
| 10 | `emfng_mfmlt_ann_u1u1()` | 731 | u1t_mfkind, u1t_mfcyl | 複数気筒失火カウント | 複数気筒失火時のカウント処理 |

**対応カウンタ**:

| カウンタ変数 | 仕様書記載 | 用途 |
|------------|-----------|------|
| `u1g_emfng_ecdmfot` | ２－１．ecdmfot | 触媒OTレベル全気筒失火カウンタ |
| `u2g_emfng_ecdmfae2` | ２－２．ecdmfae2 | EM悪化レベル全気筒失火カウンタ |
| `u1g_emfng_ecdmfem[]` | 気筒別カウンタ | EM悪化レベル気筒別失火カウンタ |
| `u1g_emfng_ecdmfw[]` | ２－３．ecdmfw | 対向気筒失火カウンタ |

### 2.4 失火異常判定関数

| No | 関数名 | 行番号 | パラメータ | 仕様書対応 | 処理概要 |
|----|--------|--------|-----------|-----------|---------|
| 11 | `emfng_ot_detect_ptptptpt()` | 2098 | xumfot, xdmfot, xhmfoton, xhmfotoff | ２－４．(1)触媒OTレベル異常判定 | 触媒OTレベルの失火異常判定 |
| 12 | `emfng_em_detect_u2ptpt()` | 2282 | cdmfae2, xdmfem, xumfem | ２－４．(2)EM悪化レベル異常判定 | EM悪化レベルの失火異常判定 |
| 13 | `emfng_di_detect_ptpt()` | 2378 | xdmfdi, xumfdi | ２－４．(3)DI失火異常判定 | DI失火の異常判定 |
| 14 | `emfng_pfi_detect_ptpt()` | 2436 | xdmfpfi, xumfdi | ２－４．(4)PFI失火異常判定 | PFI失火の異常判定 |

**判定アルゴリズム**: 各関数で以下の処理を実施
1. カウンタ値と閾値の比較
2. 異常検出回数のカウントアップ（ecxmf1, ecxmf）
3. 2トリップ判定の実施
4. 現在異常フラグ（xdmf）と未確定異常フラグ（xumf）の設定

### 2.5 計算・判定支援関数

| No | 関数名 | 行番号 | パラメータ | 仕様書対応 | 処理概要 |
|----|--------|--------|-----------|-----------|---------|
| 15 | `emfng_mcr_cal()` | 1758 | なし | １４．計算値変数の算出 | 判定閾値などの計算値算出 |
| 16 | `emfng_xottd_cal()` | 1842 | なし | 触媒OT異常仮記憶計算 | 触媒OT異常の仮判定処理 |
| 17 | `emfng_cdmfi_reset()` | 2068 | なし | カウンタリセット処理 | アイドル時カウンタのリセット |
| 18 | `emfng_roughjdg_u1u1()` | 4201 | u1t_xmfne, u1t_mfkind | ラフロード判定 | 運転条件による粗判定 |

**計算値変数**:

| 変数名 | 仕様書記載 | 計算内容 |
|--------|-----------|---------|
| `u2g_emfng_ekldmfot` | ekldmfot | 触媒OTレベル失火判定回数閾値 |
| `u2g_emfng_ekldmfae` | ekldmfae | EM悪化レベル失火判定回数閾値 |
| `u1s_emfng_ekldmfw1_mcr` | ekldmfw1 | 対向気筒失火判定閾値（低回転） |
| `u1s_emfng_ekldmfw2_mcr` | ekldmfw2 | 対向気筒失火判定閾値（高回転） |

### 2.6 フラグ操作関数

| No | 関数名 | 行番号 | パラメータ | 仕様書対応 | 処理概要 |
|----|--------|--------|-----------|-----------|---------|
| 19 | `emfng_xjmf_ann_u1()` | 831 | u1t_xjmf | 仮判定フラグ処理 | 失火仮判定フラグの処理 |
| 20 | `emfng_exmfotfc_u1()` | 4157 | u1t_flg | FCオプション通知 | 触媒OT異常のFC通知処理 |

### 2.7 電技通知関数

| No | 関数名 | 行番号 | パラメータ | 仕様書対応 | 処理概要 |
|----|--------|--------|-----------|-----------|---------|
| 21 | `emfng_wxmf_out()` | 2497 | なし | ２－６．電技への失火OBD異常通知 | 電技への各種異常通知 |

**出力フラグ**:

| フラグ変数 | 仕様書記載 | 通知内容 |
|-----------|-----------|---------|
| `exdmf` | t-CORE対応時 | 失火OBD現在異常フラグ |
| `u1g_emfng_excdmfw` | excdmfw | 対向気筒失火判定成立フラグ |
| `u1g_emfng_exdmffc` | exdmffc | FC判定用触媒OT異常通知 |
| `exmfddi` | デュアルINJ時 | DI異常フラグ |
| `exmfdpfi` | デュアルINJ時 | PFI異常フラグ |

### 2.8 Mode06データ出力関数

| No | 関数名 | 行番号 | 戻り値型 | 仕様書対応 | 出力データ |
|----|--------|--------|---------|-----------|-----------|
| 22 | `emfng_emode06_cal()` | 3968 | void | Mode06データ算出 | Mode06データ計算統括 |
| 23 | `u2g_emfng_eocmfnl()` | 3125 | u2 | eocmfnl | 失火回数低回転域 |
| 24 | `u2g_emfng_eocmfnm()` | 3145 | u2 | eocmfnm | 失火回数中回転域 |
| 25 | `u2g_emfng_eocmfnh()` | 3165 | u2 | eocmfnh | 失火回数高回転域 |
| 26 | `u2g_emfng_eocmfmltl()` | 3185 | u2 | eocmfmltl | 複数気筒失火回数低回転 |
| 27 | `u2g_emfng_eocmfmlth()` | 3205 | u2 | eocmfmlth | 複数気筒失火回数高回転 |
| 28 | `u1g_emfng_eonemfmlt()` | 3226 | u1 | eonemfmlt | 単一/複数気筒失火 |
| 29 | `emfng_eocmfmlt_pt()` | 2837 | void | 複数気筒失火データ | 複数気筒失火のMode06データ処理 |
| 30 | `emfng_eocmfn_u2()` | 2942 | void | 失火回数データ | 失火回数のMode06データ処理 |

### 2.9 EM悪化レベルMode06データ関数

| No | 関数名 | 行番号 | パラメータ | 仕様書対応 | 出力データ |
|----|--------|--------|-----------|-----------|-----------|
| 31 | `u2g_emfng_ecdmfaemTv()` | 3247 | なし | EM悪化全気筒モニタ値 | ecdmfaemTv |
| 32 | `u2g_emfng_ecdmfaemMntl()` | 3267 | なし | EM悪化全気筒最小モニタ値 | ecdmfaemMntl |
| 33 | `u2g_emfng_ecdmfaemMxtl()` | 3287 | なし | EM悪化全気筒最大モニタ値 | ecdmfaemMxtl |
| 34 | `u2g_emfng_ecdmfemTv()` | 3307 | u1t_cyl | EM悪化気筒別モニタ値 | ecdmfemTv[n] |
| 35 | `u2g_emfng_ecdmfemMntl()` | 3330 | u1t_cyl | EM悪化気筒別最小モニタ値 | ecdmfemMntl[n] |
| 36 | `u2g_emfng_ecdmfemMxtl()` | 3353 | u1t_cyl | EM悪化気筒別最大モニタ値 | ecdmfemMxtl[n] |

### 2.10 触媒OTレベルMode06データ関数

| No | 関数名 | 行番号 | パラメータ | 仕様書対応 | 出力データ |
|----|--------|--------|-----------|-----------|-----------|
| 37 | `u2g_emfng_ecdmfaavTv()` | 3376 | なし | 触媒OT全気筒平均モニタ値 | ecdmfaavTv |
| 38 | `u2g_emfng_ecdmfaavMntl()` | 3396 | なし | 触媒OT全気筒平均最小モニタ値 | ecdmfaavMntl |
| 39 | `u2g_emfng_ecdmfaavMxtl()` | 3416 | なし | 触媒OT全気筒平均最大モニタ値 | ecdmfaavMxtl |
| 40 | `u2g_emfng_ecdmfavTv()` | 3436 | u1t_cyl | 触媒OT気筒別平均モニタ値 | ecdmfavTv[n] |
| 41 | `u2g_emfng_ecdmfavMntl()` | 3459 | u1t_cyl | 触媒OT気筒別平均最小モニタ値 | ecdmfavMntl[n] |
| 42 | `u2g_emfng_ecdmfavMxtl()` | 3482 | u1t_cyl | 触媒OT気筒別平均最大モニタ値 | ecdmfavMxtl[n] |

### 2.11 市場調査RAM出力関数（JEOOBD=USE時）

| No | 関数名 | 行番号 | 戻り値型 | 仕様書対応 | 出力データ |
|----|--------|--------|---------|-----------|-----------|
| 43 | `u1g_emfng_eospdemav()` | 3628 | u1 | １１－３．eospdemav | 1000rev間の車速積算値 |
| 44 | `u1g_emfng_eoneemav()` | 3648 | u1 | １１－３．eoneemav | 1000rev間のエンジン回転数積算値 |
| 45 | `u1g_emfng_eoklsmemav()` | 3668 | u1 | １１－３．eoklsmemav | 1000rev間の負荷率積算値 |
| 46 | `u1g_emfng_eocjmfneemi()` | 3688 | u1 | １１－４．eocjmfneemi | 1000rev間のアイドル以外回数 |
| 47 | `u1g_emfng_eocjmfneeml()` | 3708 | u1 | １１－４．eocjmfneeml | 1000rev間の低回転以外回数 |
| 48 | `u1g_emfng_eocjmfneemh()` | 3728 | u1 | １１－４．eocjmfneemh | 1000rev間の高回転以外回数 |

### 2.12 その他出力関数

| No | 関数名 | 行番号 | 戻り値型 | 仕様書対応 | 出力データ |
|----|--------|--------|---------|-----------|-----------|
| 49 | `u1s_emfng_exrcdmf()` | 3504 | u1 | 失火確定情報 | 失火確定異常フラグ |
| 50 | `u1g_emfng_eocdmfae2mx()` | 3523 | u1 | EM悪化全気筒最大値 | eocdmfae2mx |
| 51 | `u1g_emfng_eocjrevem()` | 3543 | u1 | EM判定回数 | eocjrevem |
| 52 | `s1g_emfng_eothwem()` | 3563 | s1 | 水温（EM異常時） | eothwem |
| 53 | `s1g_emfng_eothaem()` | 3583 | s1 | 吸気温（EM異常時） | eothaem |
| 54 | `u1g_emfng_eocdmfem2mx()` | 3603 | u1 | EM気筒別最大値 | eocdmfem2mx[n] |
| 55 | `u1g_emfng_eocdmfaotmx()` | 3748 | u1 | 触媒OT全気筒最大値 | eocdmfaotmx |
| 56 | `u1g_emfng_eocjrevot()` | 3768 | u1 | 触媒OT判定回数 | eocjrevot |
| 57 | `s1g_emfng_eothwot()` | 3788 | s1 | 水温（触媒OT異常時） | eothwot |
| 58 | `s1g_emfng_eothaot()` | 3808 | s1 | 吸気温（触媒OT異常時） | eothaot |

### 2.13 内部処理関数

| No | 関数名 | 行番号 | パラメータ | 仕様書対応 | 処理概要 |
|----|--------|--------|-----------|-----------|---------|
| 59 | `u1s_emfng_wxreqclr()` | 3828 | なし | クリア要求判定 | データ消去要求時の処理 |
| 60 | `emfng_espdaemclr()` | 4179 | なし | 車速データクリア | 市場調査RAM車速データクリア |
| 61 | `emfng_cntclr_65msl()` | 3073 | なし | カウンタクリア | 65ms周期でのカウンタクリア処理 |

## 3. 関数呼び出し階層

### 3.1 電源ON時

```
emfng_pwon()
  └─ emfng_mcr_cal()  // 計算値変数の算出
```

### 3.2 TDC周期

```
emfng_tdcm()  // TDC周期メイン処理
  └─ (emfcntより各種判定結果を受けて処理)
      ├─ emfng_mfptn_ann_u1u1u1()  // 検出許可カウンタ操作
      ├─ emfng_mf_cnt_u1u1()       // 失火カウンタ操作
      ├─ emfng_mfmlt_ann_u1u1()    // 複数気筒失火カウント
      ├─ emfng_xjmf_ann_u1()       // 仮判定フラグ処理
      └─ emfng_cjpmf_inc_u1()      // 保持カウンタ更新
```

### 3.3 16ms周期

```
emfng_16msl()  // 16ms周期メイン処理
  └─ (特定の処理を実施)
```

### 3.4 65ms周期

```
emfng_65msl()  // 65ms周期メイン処理
  ├─ emfng_xottd_cal()              // 触媒OT異常仮記憶計算
  ├─ emfng_ot_detect_ptptptpt()     // 触媒OT異常判定
  ├─ emfng_em_detect_u2ptpt()       // EM悪化異常判定
  ├─ emfng_di_detect_ptpt()         // DI異常判定
  ├─ emfng_pfi_detect_ptpt()        // PFI異常判定
  ├─ emfng_wxmf_out()               // 電技通知
  ├─ emfng_emode06_cal()            // Mode06データ計算
  │   ├─ emfng_eocmfmlt_pt()        // 複数気筒失火データ
  │   └─ emfng_eocmfn_u2()          // 失火回数データ
  └─ emfng_cntclr_65msl()           // カウンタクリア
```

### 3.5 運転条件変化時

```
emfng_drvclchg()  // 運転条件変化時処理
  └─ emfng_mcr_cal()  // 計算値変数の再計算
```

## 4. 仕様書セクションと関数の対応マトリクス

| 仕様書セクション | 主要関数 | 補助関数 | タイミング |
|----------------|---------|---------|-----------|
| １．検出許可カウンタの操作 | emfng_mfptn_ann_u1u1u1 | - | TDC |
| ２．失火カウンタの操作 | emfng_mf_cnt_u1u1 | emfng_mfmlt_ann_u1u1 | TDC |
| ２－４．失火異常の判定 | emfng_ot_detect, emfng_em_detect | emfng_di_detect, emfng_pfi_detect | 65ms |
| ２－６．電技への通知 | emfng_wxmf_out | - | 65ms |
| １０．exjokmf | emfng_65msl内で処理 | - | 65ms |
| １１．市場調査RAM | 各種eo*関数群 | - | 65ms |
| １４．計算値変数の算出 | emfng_mcr_cal | - | pwon, drvclchg |
| Mode06データ | emfng_emode06_cal | 各種eo*関数群 | 65ms |

## 5. データフロー図

### 5.1 入力データソース

| 入力データ | 変数名 | 取得元モジュール | 使用関数 |
|-----------|--------|----------------|---------|
| エンジン回転数 | s2g_ene_ene | ene | emfng_mfptn_ann_u1u1u1 |
| 車速 | s2g_espd_espd | espd | 市場調査関数群 |
| 吸気温 | s2g_etha_etha | etha | 市場調査関数群 |
| 水温 | s2g_ethw_ethw | ethw | 市場調査関数群 |
| 失火仮判定 | u1g_exst_exastnrm | exst | emfng_xjmf_ann_u1 |
| 回転領域判定値 | u1g_emfne_ejmfne | emfne | emfng_mfptn_ann_u1u1u1 |

### 5.2 出力データ送信先

| 出力データ | 変数名 | 送信先モジュール | 生成関数 |
|-----------|--------|----------------|---------|
| EM判定値 | u2g_emfng_ekldmfae | 診断モジュール | emfng_mcr_cal |
| 触媒OT異常通知 | u1g_emfng_exdmffc | FC制御 | emfng_exmfotfc_u1 |
| 対向気筒失火 | excdmfw | 電技 | emfng_wxmf_out |
| Mode06データ | 各種eo*変数 | OBD診断 | Mode06関数群 |

## 6. コンパイルスイッチ別関数有効化

| コンパイルスイッチ | 影響を受ける関数 | 有効/無効の条件 |
|------------------|----------------|----------------|
| JEEFI==DUAL && JEHIPFI_E==USE | emfng_di_detect, emfng_pfi_detect | デュアルINJ時のみ有効 |
| JENCYL==6CYL | 対向気筒判定処理（emfng_mf_cnt内） | 6気筒時のみ有効 |
| JEOOBD==USE | 市場調査RAM関数群 | 市場調査対応時のみ有効 |
| JEMFFC==USE | emfng_exmfotfc_u1 | FCオプション時のみ有効 |
| JETCORE_D==USE | exdmf出力処理（emfng_wxmf_out内） | t-CORE対応時のみ有効 |

## 7. 重要な関数の詳細仕様

### 7.1 emfng_mfptn_ann_u1u1u1()

**目的**: 失火検出許可カウンタの操作

**入力パラメータ**:
- `u1t_mfkind`: 失火種別（触媒OT/EM悪化/DI/PFI）
- `u1t_mfcyl`: 失火気筒番号
- `u1t_xmfne`: 失火本判定フラグ

**処理内容**:
1. ecjmfot（触媒OTレベル）のカウントアップ
2. ecjmfoti（アイドル時触媒OT）のカウントアップ（条件付き）
3. ecjmfem（EM悪化レベル）のカウントアップ
4. ecjmfdi（DI失火）のカウントアップ【デュアルINJ時】
5. ecjmfpfi（PFI失火）のカウントアップ【デュアルINJ時】
6. 失火率の計算
7. 複数気筒失火のカウント

**仕様書対応**: セクション１全体

### 7.2 emfng_mf_cnt_u1u1()

**目的**: 失火カウンタの操作

**入力パラメータ**:
- `u1t_mfkind`: 失火種別
- `u1t_mfcyl`: 失火気筒番号

**処理内容**:
1. 触媒OTレベル全気筒失火カウンタ更新
2. EM悪化レベル気筒別失火カウンタ更新
3. 対向気筒失火カウンタ更新【6気筒時】
4. カウンタオーバーフロー防止

**仕様書対応**: セクション２

### 7.3 emfng_ot_detect_ptptptpt()

**目的**: 触媒OTレベル失火異常の判定

**出力パラメータ（ポインタ）**:
- `ptt_xumfot`: 未確定異常フラグ
- `ptt_xdmfot`: 現在異常フラグ
- `ptt_xhmfoton`: ハイステートONフラグ
- `ptt_xhmfotoff`: ハイステートOFFフラグ

**処理内容**:
1. カウンタ値と閾値の比較
2. 異常検出回数（ecxmf1）のカウントアップ
3. 2トリップ判定
4. ランプ点灯/点滅判定

**仕様書対応**: セクション２－４．(1)

### 7.4 emfng_mcr_cal()

**目的**: 計算値変数の算出

**入力**: なし（グローバル変数から取得）

**出力**: 計算値変数（グローバル変数へ設定）

**処理内容**:
1. 触媒OTレベル判定閾値の計算
2. EM悪化レベル判定閾値の計算
3. 対向気筒失火判定閾値の計算
4. 回転領域別閾値の設定

**仕様書対応**: セクション１４

## 8. 変更時の影響範囲

### 8.1 関数シグネチャ変更の影響

| 関数名 | 呼び出し元 | 変更時の影響範囲 |
|--------|-----------|----------------|
| emfng_pwon | 初期化処理 | 全体の初期化に影響 |
| emfng_tdcm | emfcnt_tdcm | TDC周期の全処理に影響 |
| emfng_65msl | emfcnt_65msl | 65ms周期の全処理に影響 |
| emfng_mfptn_ann_u1u1u1 | emfcnt_tdcm | 失火検出許可判定に影響 |
| emfng_mf_cnt_u1u1 | emfcnt_tdcm | 失火カウント処理に影響 |

### 8.2 グローバル変数変更の影響

| 変数名 | 使用関数数 | 影響範囲 |
|--------|-----------|---------|
| u2g_emfng_ecjmfot | 多数 | 触媒OT判定全体 |
| u2g_emfng_ecdmfae2 | 多数 | EM悪化判定全体 |
| u1g_emfng_ecdmfem[] | 多数 | 気筒別判定全体 |
| u1g_emfng_ecdmfw[] | 複数 | 対向気筒判定【6気筒】 |

## 9. Task4での使用方法

### 9.1 差分抽出時の確認ポイント

1. **関数追加/削除**: 新旧の関数一覧を比較
2. **関数シグネチャ変更**: パラメータや戻り値の変更確認
3. **処理フロー変更**: 関数内の処理順序やロジック変更
4. **コンパイルスイッチ変更**: 条件分岐の追加/変更
5. **グローバル変数変更**: 変数の追加/削除/型変更

### 9.2 実装時の優先順位

| 優先度 | 対象関数カテゴリ | 理由 |
|-------|----------------|------|
| 高 | 初期化関数 | システム起動の基盤 |
| 高 | 周期処理関数 | メインループ処理 |
| 中 | 判定関数 | 診断ロジックの核心 |
| 中 | Mode06関数 | OBD診断データ出力 |
| 低 | 市場調査関数 | 補助的なデータ収集 |

## 10. まとめ

本文書では、emfng_l_mat.cの全60個の関数を仕様書と対応付けて一覧化した。

**主要な発見**:
- 失火OBD診断は、TDC、16ms、65msの3つの周期で処理される
- 触媒OTレベル、EM悪化レベル、DI/PFI失火の4種類の判定が実施される
- Mode06データ出力関数が多数存在（約20個）
- 市場調査RAM関連の関数も多数存在（約10個）
- コンパイルスイッチにより有効化される関数が多い

**Task4での活用**:
- この対応表を基に、新旧コードの差分を関数レベルで抽出
- 関数シグネチャの変更有無を確認
- 処理フローの変更箇所を特定
- 影響範囲を把握して効率的な実装を実施

---
**作成日**: 2025-10-17  
**作成者**: GitHub Copilot  
**対象バージョン**: emfng-aa0-50-a
