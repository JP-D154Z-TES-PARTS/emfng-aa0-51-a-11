# base仕様書とemfng_l_mat.cの対応関係マップ

## 1. 概要

本文書は、baseフォルダの仕様書（D0emfng-aa0-50-a.doc）とソースコード（emfng_l_mat.c）の対応関係を分析した結果をまとめたものである。

### 1.1 対象ファイル

- **仕様書**: `/base/emfng-aa0-50-a_spec/D0emfng-aa0-50-a.doc`
- **ソースコード**: `/base/emfng-aa0-50-a_src/emfng_l_mat.c`
- **ヘッダファイル**: `/base/emfng-aa0-50-a_src/emfng.h`

### 1.2 部品概要

**部品名**: emfng-pa000-5000-a-M4  
**目的**: 複数気筒ミスファイア判定部（失火OBD診断）  
**対応システム**: 4気筒、6気筒（ω法、ΔT法）  
**法規対応**: 対米法規（OBD2）、制御再構築

## 2. 仕様書の構成と対応コード

### 2.1 主要処理タイミング

仕様書では以下のタイミングで処理が実行される：

| 処理タイミング | 仕様書記載 | 対応関数 | 行番号 |
|---------------|-----------|---------|--------|
| 電源ON時 | 初期化処理 | `emfng_pwon()` | 545 |
| TDC(中) | 検出許可カウンタ操作 | `emfng_tdcm()` | 753 |
| 16ms(低) | 16ms周期処理 | `emfng_16msl()` | 1215 |
| 65ms(低) | 65ms周期処理 | `emfng_65msl()` | 1287 |
| リセット時 | リセット処理 | `emfng_reset()` | 1121 |
| 運転条件変化時 | 運転条件変化時処理 | `emfng_drvclchg()` | 1137 |

## 3. 仕様書とコードの詳細対応

### 3.1 検出許可カウンタの操作（TDC周期）

#### 仕様書セクション: 「１．検出許可カウンタの操作（TDC(中)）」

**対応関数**: `emfng_mfptn_ann_u1u1u1()` (行564)

| 仕様書項目 | 変数名 | コード実装 | 処理内容 |
|-----------|--------|-----------|---------|
| １－１．ecjmfot | `u2g_emfng_ecjmfot` | 行594-600 | 触媒OTレベル失火検出許可カウンタ |
| １－２．ecjmfoti | `u2g_emfng_ecjmfoti` | 行605-619 | 触媒OTレベルアイドル時失火検出許可カウンタ |
| １－３．ecjmfem | `u2g_emfng_ecjmfem` | 行624-630 | EM悪化レベル失火検出許可カウンタ |
| １－４．ecjmfdi | `u2s_emfng_ecjmfdi` | 行636-650 | DI失火検出許可カウンタ（デュアルINJ時） |
| １－５．ecjmfpfi | `u2s_emfng_ecjmfpfi` | 行655-669 | PFI失火検出許可カウンタ（デュアルINJ時） |

**処理フロー**:
1. 各カウンタをカウントアップ（オーバーフロー防止付き）
2. 条件に応じてカウントアップの実施可否を判定
3. デュアルINJ時は噴射量に応じてDI/PFI判定を実施

### 3.2 失火カウンタの操作（TDC周期）

#### 仕様書セクション: 「２．失火カウンタの操作」

**対応関数**: `emfng_mf_cnt_u1u1()` (行925)

| 仕様書項目 | 処理内容 | コード実装箇所 |
|-----------|---------|---------------|
| ２－１．ecdmfot | 触媒OTレベル全気筒失火カウンタ | 行925-1120 |
| ２－２．ecdmfem | EM悪化レベル気筒別失火カウンタ | 変数配列 `u1g_emfng_ecdmfem[]` |
| ２－３．ecdmfw | 対向気筒失火カウンタ | 変数配列 `u1g_emfng_ecdmfw[]` |

**主要ロジック**:
- 失火種別（mfkind）と気筒番号（mfcyl）に基づくカウンタ更新
- 対向気筒判定（6気筒時）
- カウンタ上限値チェック

### 3.3 失火異常判定（65ms周期）

#### 仕様書セクション: 「２－４．失火異常の判定」

| 判定種別 | 対応関数 | 行番号 | 入出力パラメータ |
|---------|---------|--------|----------------|
| 触媒OTレベル異常判定 | `emfng_ot_detect_ptptptpt()` | 2098 | xumfot, xdmfot, xhmfoton, xhmfotoff |
| EM悪化レベル異常判定 | `emfng_em_detect_u2ptpt()` | 2282 | cdmfae2, xdmfem, xumfem |
| DI失火異常判定 | `emfng_di_detect_ptpt()` | 2378 | xdmfdi, xumfdi |
| PFI失火異常判定 | `emfng_pfi_detect_ptpt()` | 2436 | xdmfpfi, xumfpfi |

**判定アルゴリズム**:
1. カウンタ値と閾値を比較
2. 判定回数（ecxmf1, ecxmf）のカウントアップ
3. 2トリップ判定の実施
4. 現在異常フラグ（xdmf）と未確定異常フラグ（xumf）の設定

### 3.4 電技への失火OBD異常通知

#### 仕様書セクション: 「２－６．電技への失火OBD異常通知」

**対応関数**: `emfng_wxmf_out()` (行2497)

| 出力フラグ | 変数名 | 処理内容 |
|-----------|--------|---------|
| exdmf | 失火OBD現在異常フラグ | t-CORE対応時に出力 |
| excdmfw | 対向気筒失火判定成立フラグ | 6気筒時の対向判定結果 |
| exdmffc | FC判定用触媒OT異常通知 | FCオプション時の通知 |
| exmfddi | DI異常フラグ | デュアルINJ時のDI異常 |
| exmfdpfi | PFI異常フラグ | デュアルINJ時のPFI異常 |

### 3.5 計算値変数の算出

#### 仕様書セクション: 「１４．計算値変数の算出」

**対応関数**: `emfng_mcr_cal()` (行1758)

計算される主要変数：

| 計算値変数 | 変数名 | 用途 |
|-----------|--------|------|
| ekldmfot | `u2g_emfng_ekldmfot` | 触媒OTレベル失火判定回数閾値 |
| ekldmfae | `u2g_emfng_ekldmfae` | EM悪化レベル失火判定回数閾値 |
| ekldmfw1_mcr | `u1s_emfng_ekldmfw1_mcr` | 対向気筒失火判定閾値（低回転） |
| ekldmfw2_mcr | `u1s_emfng_ekldmfw2_mcr` | 対向気筒失火判定閾値（高回転）【6気筒】 |

**計算タイミング**: 電源ON時、運転条件変化時

### 3.6 Mode06関連処理

#### 仕様書セクション: Mode06データ算出

**対応関数**: `emfng_emode06_cal()` (行3968)

| Mode06データ | 出力関数 | 行番号 | 内容 |
|-------------|---------|--------|------|
| eocmfnl | `u2g_emfng_eocmfnl()` | 3125 | 失火回数低回転域 |
| eocmfnm | `u2g_emfng_eocmfnm()` | 3145 | 失火回数中回転域 |
| eocmfnh | `u2g_emfng_eocmfnh()` | 3165 | 失火回数高回転域 |
| eocmfmltl | `u2g_emfng_eocmfmltl()` | 3185 | 複数気筒失火回数低回転 |
| eocmfmlth | `u2g_emfng_eocmfmlth()` | 3205 | 複数気筒失火回数高回転 |
| eonemfmlt | `u1g_emfng_eonemfmlt()` | 3226 | 単一/複数気筒失火回数 |

### 3.7 市場調査RAM

#### 仕様書セクション: 「１１．市場調査ＲＡＭの操作」

対応する出力関数群（行3628-3828）：

| データ項目 | 関数名 | 内容 |
|-----------|--------|------|
| eospdemav | `u1g_emfng_eospdemav()` | 1000rev間の車速積算値 |
| eoneemav | `u1g_emfng_eoneemav()` | 1000rev間のエンジン回転数積算値 |
| eoklsmemav | `u1g_emfng_eoklsmemav()` | 1000rev間の負荷率積算値 |
| eocjmfneemi | `u1g_emfng_eocjmfneemi()` | 1000rev間のアイドル以外回数 |
| eocjmfneeml | `u1g_emfng_eocjmfneeml()` | 1000rev間の低回転以外回数 |
| eocjmfneemh | `u1g_emfng_eocjmfneemh()` | 1000rev間の高回転以外回数 |

## 4. データフロー

### 4.1 入力データ

| データ名 | 型 | 取得元 | 用途 |
|---------|---|--------|------|
| s2g_ene_ene | s2 | エンジン回転数 | カウンタ判定条件 |
| s2g_espd_espd | s2 | 車速 | 市場調査データ |
| s2g_etha_etha | s2 | 吸気温度 | 市場調査データ |
| s2g_ethw_ethw | s2 | 水温 | 市場調査データ |
| u1g_exst_exastnrm | u1 | 失火仮判定フラグ | emfneより入力 |

### 4.2 出力データ

| データ名 | 型 | 出力先 | 用途 |
|---------|---|--------|------|
| u2g_emfng_ekldmfae | u2 | 他モジュール | EM悪化レベル判定値 |
| u1g_emfng_exdmffc | u1 | FC判定 | 触媒OT異常通知 |
| excdmfw | flag | 電技 | 対向気筒失火判定 |

## 5. コンパイルスイッチと条件分岐

### 5.1 主要コンパイルスイッチ

| スイッチ名 | 値 | 影響範囲 |
|-----------|---|---------|
| JENCYL | u1g_EJCC_4CYL / u1g_EJCC_6CYL | 4気筒/6気筒対応 |
| JEEFI | u1g_EJCC_DUAL | デュアルINJ対応 |
| JEHIPFI_E | u1g_EJCC_USE | 温間ポート噴射対応 |
| JEMFFC | u1g_EJCC_USE | 触媒OT失火異常時FC対応 |
| JEOOBD | u1g_EJCC_USE | 市場調査対応 |
| JEMFHOUKI | u1g_EJCC_USAMF / u1g_EJCC_NOT_USAMF | 対米法規/対米以外 |
| JEMFMETHOD_D | u1g_EJCC_OMEGA / u1g_EJCC_DELTAT | ω法/ΔT法 |
| JETCORE_D | u1g_EJCC_USE | t-CORE対応 |
| JEOBDUDS_D | u1g_EJCC_USE | OBDonUDS対応 |
| JENGPF_E | u1g_EJCC_USE | GPF対応 |
| JERLOK | u1g_EJCC_USE | リッチリーン運転対応 |

### 5.2 条件分岐の影響

```c
// デュアルINJ対応時のみコンパイル
#if ( JEEFI == u1g_EJCC_DUAL ) && ( JEHIPFI_E == u1g_EJCC_USE )
    // DI/PFI失火判定処理
#endif

// 6気筒時のみコンパイル
#if JENCYL == u1g_EJCC_6CYL
    // 対向気筒失火判定処理
#endif

// 市場調査対応時のみコンパイル
#if JEOOBD == u1g_EJCC_USE
    // 市場調査RAM操作
#endif
```

## 6. 処理フロー図

### 6.1 メイン処理フロー（TDC周期）

```
emfng_tdcm() [TDC周期]
  ↓
emfng_mfptn_ann_u1u1u1() [失火パターン判定時にコール]
  ↓
  ├─ 検出許可カウンタのカウントアップ
  │   ├─ ecjmfot（触媒OTレベル）
  │   ├─ ecjmfoti（触媒OTアイドル時）
  │   ├─ ecjmfem（EM悪化レベル）
  │   ├─ ecjmfdi（DI失火）※デュアルINJ時
  │   └─ ecjmfpfi（PFI失火）※デュアルINJ時
  ↓
emfng_mf_cnt_u1u1() [失火カウント時にコール]
  ↓
  ├─ 失火カウンタの更新
  │   ├─ ecdmfot（触媒OT全気筒）
  │   ├─ ecdmfem[]（EM悪化気筒別）
  │   └─ ecdmfw[]（対向気筒）
  ↓
emfng_xjmf_ann_u1() [仮判定フラグ処理]
  ↓
emfng_cjpmf_inc_u1() [保持カウンタ更新]
```

### 6.2 判定処理フロー（65ms周期）

```
emfng_65msl() [65ms周期]
  ↓
  ├─ emfng_xottd_cal() [触媒OT異常仮記憶計算]
  ↓
  ├─ 失火異常判定
  │   ├─ emfng_ot_detect_ptptptpt() [触媒OT判定]
  │   ├─ emfng_em_detect_u2ptpt() [EM悪化判定]
  │   ├─ emfng_di_detect_ptpt() [DI判定]
  │   └─ emfng_pfi_detect_ptpt() [PFI判定]
  ↓
  ├─ emfng_wxmf_out() [電技への異常通知]
  ↓
  ├─ emfng_emode06_cal() [Mode06データ計算]
  ↓
  └─ emfng_cntclr_65msl() [カウンタクリア処理]
```

## 7. 重要なアルゴリズム

### 7.1 2トリップ判定

仕様書記載: 「2トリップエミッション悪化レベルの異常時、ランプを点灯」

実装箇所: `emfng_em_detect_u2ptpt()` (行2282)

```c
// 判定回数カウントアップ
if (異常条件成立) {
    ecxmf++; // 異常検出回数
}

// 2トリップ判定
if (ecxmf >= 2) {
    xdmfem = ON;  // 現在異常フラグON
} else {
    xumfem = ON;  // 未確定異常フラグON
}
```

### 7.2 対向気筒失火判定（6気筒専用）

仕様書記載: 「対向気筒失火判定成立フラグ」

実装箇所: `emfng_eocmfmlt_pt()` (行2837)

6気筒エンジンでは、対向する気筒（180度位相差）での失火を検出して判定。

### 7.3 始動後1000rev判定

仕様書記載: 「始動後1000rev完了フラグ」

変数: `u1g_emfng_exjokmf`

FTPサイクル判定や市場調査データの基準として使用。

## 8. メモリ管理

### 8.1 EEPROM保存データ

市場調査データは以下の関数でEEPROMへ読み書き：

- `s4g_ememctr_write()` - 書き込み
- `s4g_ememctr_read()` - 読み込み

保存対象データID例：
- `u2g_EMFNG_EOCMFNL_U2_ID` - 失火回数低回転域
- `u2g_EMFNG_ECDMFAEMTV_U2_ID` - EM悪化レベルモニタ値

### 8.2 フラグ構造体

```c
stflag8 stg_emfng_flag1;  // グローバルフラグ（16ms, 65ms, drvclchg）
stflag8 stg_emfng_flag2;  // デュアルINJ用フラグ
stflag8 stg_emfng_flag3;  // t-CORE用フラグ

static stflag8 sts_emfng_flagi1;  // TDCタイミング用フラグ
static stflag8 sts_emfng_flagi2;  // 65msタイミング用フラグ
```

## 9. 依存関係

### 9.1 呼び出し元

| 呼び出し元 | 対象関数 | タイミング |
|-----------|---------|-----------|
| emfcnt_tdcm() | emfng_tdcm() | TDC周期 |
| emfcnt_16msl() | emfng_16msl() | 16ms周期 |
| emfcnt_65msl() | emfng_65msl() | 65ms周期 |
| 初期化処理 | emfng_pwon() | 電源ON時 |
| 初期化処理 | emfng_reset() | リセット時 |

### 9.2 呼び出し先

| 呼び出し先 | 用途 |
|-----------|------|
| エンジン状態取得関数 | ne, spd, tha, thw等の取得 |
| EEPROM制御関数 | 市場調査データの保存/読み出し |
| DTC制御関数 | Mode06データ送信 |

## 10. 注意事項

### 10.1 実装上の注意

1. **オーバーフロー対策**: 各カウンタは上限値チェックを実施
2. **タイミング依存**: TDC、16ms、65msの各周期で適切な処理を実施
3. **コンパイルスイッチ**: 車両仕様に応じて適切なスイッチ設定が必要
4. **排他制御**: グローバル変数へのアクセスは排他制御を考慮

### 10.2 後続作業への影響

本分析結果はTask4のロジック差分実装で使用される：
- 関数シグネチャの変更有無確認
- 処理フローの差分抽出
- コンパイルスイッチの変更影響範囲確認

## 11. 変更履歴

| 日付 | 版数 | 変更内容 | 作成者 |
|------|------|---------|--------|
| 2025-10-17 | 1.0 | 初版作成 | - |
