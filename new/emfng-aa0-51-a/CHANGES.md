# emfng_l_mat.c ロジック変更内容

## 変更概要
emfng-aa0-50-a → emfng-aa0-51-a へのロジック差分を反映

## 変更履歴
- バージョン: emfng-aa0-51-a
- 日付: 2025-10-17
- 参照: ロジック変更シート ver9.09 (S51a)

## 変更内容

### (1) t-CORE対応(DI/PFI異常検出タイミング仕様不備対応)【t-CORE対応有】

#### 変更理由
emfng-aa0-50-aにてt-CORE対応を実施したが、D4Sインジェクター故障検出(DI/PFI異常検出)の
異常検出タイミングが200rev×4回となっており、異常検出タイミングが遅れるため対策が必要。

#### 変更内容の詳細
エミッション悪化失火率超過通知( wmf_fdi_em_excmf() )の第2引数の通知識別子について：
- WMF_FDI_WITHIN_FIRST1000RPMは1回の通知で異常判定
- WMF_FDI_AFTER_FIRST1000RPMは4回の通知で異常判定

DI/PFI異常検出時にWMF_FDI_AFTER_FIRST1000RPM(4回)で通知していたため、
WMF_FDI_WITHIN_FIRST1000RPM(1回)を通知する条件に、
初回1000revで異常判定時に加えてDI/PFI異常検出時の条件をORで追加する。

#### 変更箇所
ファイル: emfng_l_mat.c
行番号: 2758-2766

**変更前:**
```c
        u1t_emcnd = u1g_WMF_FDI_AFTER_FIRST1000RPM;
        if ( bis_emfng_exemfst == (u1)ON )
        {
            u1t_emcnd = u1g_WMF_FDI_WITHIN_FIRST1000RPM;
        }
```

**変更後:**
```c
        u1t_emcnd = u1g_WMF_FDI_AFTER_FIRST1000RPM;
        if ( ( bis_emfng_exemfst == (u1)ON )
#if ( JEEFI == u1g_EJCC_DUAL ) && ( JEHIPFI_E == u1g_EJCC_USE )   /*【デュアルINJ】AND【高圧直噴有】*/
          || ( u1t_xumfdi == (u1)ON )
          || ( u1t_xumfpfi == (u1)ON )
#endif
           )
        {
            u1t_emcnd = u1g_WMF_FDI_WITHIN_FIRST1000RPM;
        }
```

## 技術仕様
- ファイルエンコーディング: CP932 (Shift-JIS)
- 総行数: 4400行 (変更前: 4395行、+5行)
- 構文チェック: OK
  - 括弧バランス: OK (297対297)
  - プリプロセッサディレクティブバランス: OK (230対230)

## 影響範囲
- t-CORE対応有効時のみ影響
- デュアルインジェクター かつ 高圧直噴有効時のみ追加条件が有効
- DI/PFI異常検出時の異常判定タイミングが200rev×1回に短縮される

## 参考資料
- ロジック変更シート: ロジック変更シート_ver9.09_emfng-aa0.xlsx (S51a)
- 仕様確認書: DENG-STD-24050 No1
- ベース仕様書: D0emfng-aa0-50-a.doc
- 新規仕様書: D0emfng-aa0-51-a.doc
