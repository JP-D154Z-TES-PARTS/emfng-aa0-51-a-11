# Task2 分析結果サマリ

## 実施内容

base仕様書（D0emfng-aa0-50-a.doc）とソースコード（emfng_l_mat.c）の対応関係を詳細に分析し、以下の成果物を作成しました。

## 成果物一覧

### 1. base_spec_code_mapping.md
**仕様書とコードの対応関係マップ**

- 仕様書の構成と対応する関数のマッピング
- 処理タイミング（TDC、16ms、65ms）別の処理内容
- 検出許可カウンタ、失火カウンタ、異常判定の詳細対応
- データフロー（入力/出力）
- コンパイルスイッチと条件分岐の影響分析
- 処理フロー図とアルゴリズム解説

### 2. function_specification_table.md
**関数一覧と仕様書対応表**

- 全60個の関数の完全一覧
- 関数カテゴリ別分類（初期化、カウンタ操作、判定、Mode06等）
- 各関数の詳細仕様（パラメータ、戻り値、処理概要）
- 関数呼び出し階層図
- 仕様書セクションと関数の対応マトリクス
- Task4での活用方法（差分抽出の手順）

## 分析結果の要点

### ファイル基本情報

| 項目 | 値 |
|------|-----|
| ファイル名 | emfng_l_mat.c |
| 総行数 | 4,395行 |
| 関数数 | 60個 |
| 部品番号 | emfng-pa000-5000-a-M4 |
| 目的 | 複数気筒ミスファイア判定部（失火OBD診断） |

### 処理タイミング構造

```
電源ON
  └─ emfng_pwon() - 初期化処理

TDC周期（中）
  └─ emfng_tdcm()
      ├─ emfng_mfptn_ann_u1u1u1() - 検出許可カウンタ操作
      ├─ emfng_mf_cnt_u1u1() - 失火カウンタ操作
      ├─ emfng_mfmlt_ann_u1u1() - 複数気筒失火カウント
      ├─ emfng_xjmf_ann_u1() - 仮判定フラグ処理
      └─ emfng_cjpmf_inc_u1() - 保持カウンタ更新

16ms周期（低）
  └─ emfng_16msl() - 16ms周期処理

65ms周期（低）
  └─ emfng_65msl()
      ├─ emfng_xottd_cal() - 触媒OT異常仮記憶計算
      ├─ 失火異常判定
      │   ├─ emfng_ot_detect_ptptptpt() - 触媒OT判定
      │   ├─ emfng_em_detect_u2ptpt() - EM悪化判定
      │   ├─ emfng_di_detect_ptpt() - DI判定
      │   └─ emfng_pfi_detect_ptpt() - PFI判定
      ├─ emfng_wxmf_out() - 電技への異常通知
      ├─ emfng_emode06_cal() - Mode06データ計算
      └─ emfng_cntclr_65msl() - カウンタクリア

運転条件変化時
  └─ emfng_drvclchg() - 計算値再計算

リセット時
  └─ emfng_reset() - カウンタ/フラグリセット
```

### 主要機能カテゴリ

| カテゴリ | 関数数 | 主要関数 |
|---------|-------|---------|
| 初期化・制御 | 6 | pwon, reset, drvclchg, tdcm, 16msl, 65msl |
| カウンタ操作 | 3 | mfptn_ann, mf_cnt, cjpmf_inc |
| 異常判定 | 4 | ot_detect, em_detect, di_detect, pfi_detect |
| 計算・支援 | 4 | mcr_cal, xottd_cal, cdmfi_reset, roughjdg |
| 電技通知 | 2 | wxmf_out, exmfotfc |
| Mode06出力 | 21 | eocmfnl, eocmfnm, eocmfnh, ecdmfaemTv等 |
| 市場調査RAM | 10 | eospdemav, eoneemav, eoklsmemav等 |
| その他出力 | 10 | exrcdmf, eocdmfae2mx等 |

### 失火診断の種類

1. **触媒OTレベル失火判定**
   - 触媒の過熱を防ぐための失火検出
   - 判定関数: `emfng_ot_detect_ptptptpt()`
   - カウンタ: ecjmfot, ecdmfot

2. **EM悪化レベル失火判定**
   - エミッション悪化を検出する失火判定
   - 判定関数: `emfng_em_detect_u2ptpt()`
   - カウンタ: ecjmfem, ecdmfae2, ecdmfem[]

3. **DI失火判定**（デュアルINJ時のみ）
   - 直噴噴射時の失火判定
   - 判定関数: `emfng_di_detect_ptpt()`
   - カウンタ: ecjmfdi, ecdmfdi[]

4. **PFI失火判定**（デュアルINJ時のみ）
   - ポート噴射時の失火判定
   - 判定関数: `emfng_pfi_detect_ptpt()`
   - カウンタ: ecjmfpfi, ecdmfpfi[]

### コンパイルスイッチ構成

| スイッチ名 | 用途 | 影響する機能 |
|-----------|------|-------------|
| JENCYL | 気筒数（4気筒/6気筒） | 対向気筒失火判定 |
| JEEFI | デュアルINJ | DI/PFI失火判定 |
| JEHIPFI_E | 温間ポート噴射 | PFI失火判定条件 |
| JEMFFC | FC判定 | 触媒OT異常通知 |
| JEOOBD | 市場調査 | 市場調査RAM機能 |
| JEMFHOUKI | 法規（対米/対米以外） | 判定条件の変更 |
| JEMFMETHOD_D | 失火検出法（ω法/ΔT法） | 失火検出アルゴリズム |
| JETCORE_D | t-CORE対応 | exdmf出力処理 |
| JEOBDUDS_D | OBDonUDS対応 | UDS通信対応 |
| JENGPF_E | GPF対応 | GPF関連処理 |
| JERLOK | リッチリーン運転 | 運転モード対応 |
| JEMAT_OBD2 | OBD2適合 | OBD2規格対応 |

### 重要なアルゴリズム

#### 1. 2トリップ判定
```
異常検出回数（ecxmf）をカウント
  ↓
ecxmf >= 2 の場合
  ├─ YES → 現在異常フラグ（xdmf）ON
  └─ NO  → 未確定異常フラグ（xumf）ON
```

#### 2. 対向気筒失火判定（6気筒専用）
```
180度位相差の気筒ペアで失火を検出
  ↓
両方の気筒で失火発生時
  ↓
対向気筒失火判定成立（excdmfw）
```

#### 3. 始動後1000rev判定
```
エンジン始動後の回転数をカウント
  ↓
1000rev到達
  ↓
exjokmf フラグ ON
  ↓
FTPサイクル判定や市場調査データの基準として使用
```

### データフロー概要

#### 入力データ
- **s2g_ene_ene**: エンジン回転数（カウンタ判定条件）
- **s2g_espd_espd**: 車速（市場調査データ）
- **s2g_etha_etha**: 吸気温度（市場調査データ）
- **s2g_ethw_ethw**: 水温（市場調査データ）
- **u1g_exst_exastnrm**: 失火仮判定フラグ（emfneより）

#### 出力データ
- **u2g_emfng_ekldmfae**: EM悪化レベル判定値
- **u1g_emfng_exdmffc**: 触媒OT異常通知（FC判定用）
- **excdmfw**: 対向気筒失火判定成立フラグ
- **Mode06データ群**: OBD診断データ（約20種類）

### グローバル変数の主要カテゴリ

#### カウンタ変数
- **ecjmfot, ecjmfoti, ecjmfem**: 検出許可カウンタ
- **ecdmfot, ecdmfae2, ecdmfem[]**: 失火カウンタ
- **ecdmfw[]**: 対向気筒失火カウンタ
- **ecxmf1, ecxmf**: 異常検出回数

#### フラグ変数
- **stg_emfng_flag1, flag2, flag3**: グローバルフラグ
- **sts_emfng_flagi1, flagi2**: 内部処理フラグ
- **exdmf系**: 現在異常フラグ群
- **xumf系**: 未確定異常フラグ群

#### 計算値変数
- **ekldmfot, ekldmfae**: 判定閾値
- **ekldmfw1_mcr, ekldmfw2_mcr**: 対向気筒判定閾値

## Task4での活用方法

### 1. 差分抽出の手順

1. **関数レベルの差分確認**
   - function_specification_table.mdの関数一覧を基に、newバージョンの関数一覧を作成
   - 追加/削除された関数を特定
   - 関数シグネチャ（パラメータ、戻り値）の変更を確認

2. **処理フローの差分確認**
   - base_spec_code_mapping.mdの処理フロー図と比較
   - 処理順序の変更を確認
   - 新規追加された処理ステップを特定

3. **データフローの差分確認**
   - 入力データの追加/削除/変更
   - 出力データの追加/削除/変更
   - グローバル変数の追加/削除/型変更

4. **コンパイルスイッチの差分確認**
   - 新規スイッチの追加
   - 既存スイッチの条件変更
   - 条件分岐の追加/変更

### 2. 実装時の優先順位

| 優先度 | 対象 | 理由 |
|-------|------|------|
| 最優先 | 関数シグネチャ変更 | 呼び出し元への影響が大きい |
| 高 | 初期化処理の変更 | システム起動の基盤 |
| 高 | 周期処理メイン関数 | メインループへの影響 |
| 中 | 判定ロジックの変更 | 診断機能の核心部分 |
| 中 | コンパイルスイッチ追加 | ビルド設定への影響 |
| 低 | Mode06データ出力 | 診断データの追加/変更 |
| 低 | 市場調査RAM | 補助的なデータ収集 |

### 3. 注意点

1. **タイミング依存**: TDC、16ms、65msの各周期で実行される処理を正しく配置
2. **オーバーフロー対策**: カウンタの上限値チェックを維持
3. **排他制御**: グローバル変数へのアクセスを適切に管理
4. **条件分岐**: コンパイルスイッチによる条件分岐を正確に実装
5. **2トリップ判定**: OBD診断の基本アルゴリズムを維持

## まとめ

本Task2では、baseフォルダの仕様書とemfng_l_mat.cソースコードの詳細な対応関係を分析し、以下を明確化しました：

### 成果
✅ 60個の関数の仕様書対応関係を完全マッピング  
✅ 処理タイミング（TDC、16ms、65ms）別の処理内容を整理  
✅ 4種類の失火判定（触媒OT、EM悪化、DI、PFI）の実装を解明  
✅ 12種類のコンパイルスイッチの影響範囲を分析  
✅ データフロー（入力/出力）を可視化  
✅ Task4での差分抽出手順を策定  

### 次のステップ
本分析結果を基に、Task3で対応関係（new側）の分析を実施し、Task4でロジック差分の実装を行います。

---
**作成日**: 2025-10-17  
**Task**: Task2 - base仕様書とl_mat.cの対応関係分析  
**ステータス**: 完了
