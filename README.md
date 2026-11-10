# RogueLike

C++ と DxLib で制作している Windows 向けのローグライクゲームです。現在は既存実装をクラス構成へ整理しながら開発しています。

## 現在の機能

- 部屋と通路を持つダンジョンの自動生成
- プレイヤーの移動と方向別の歩行アニメーション
- 階段に乗ったときの確認メニューと次フロアへの移動
- ミニマップ、視界の明暗、HP・満腹度などの表示
- ターンに応じた満腹度の更新とメッセージ表示

## 開発環境

- Windows
- Visual Studio の「C++ によるデスクトップ開発」ワークロード
- MSVC v145 ツールセットと Windows 10/11 SDK（現在のプロジェクト設定）
- C++20
- [DxLib](https://dxlib.xsrv.jp/)

DxLib 本体はこのリポジトリに含めていません。DxLib の Visual C++ 向けパッケージを別途用意し、`DxLib.h` と対応する `.lib` ファイルを、リポジトリと同じ親ディレクトリの `DxLib` フォルダに置いてください。

```text
Game/
├── DxLib/
│   ├── DxLib.h
│   └── *.lib
└── RogueLike/
    ├── dx1.sln
    ├── dx1.vcxproj
    └── Graphic/
```

## ビルド・実行

1. `dx1.sln` を Visual Studio で開きます。
2. ソリューションの構成で `Debug` または `Release`、プラットフォームで `x86` を選びます。プロジェクト側の名称は `Win32` です。
3. ビルドして実行します。画像を相対パスで読み込むため、作業ディレクトリは `Graphic` があるリポジトリ直下にしてください。Visual Studio ではデバッグの「作業ディレクトリ」を `$(ProjectDir)` に設定できます。

Developer PowerShell / Developer Command Prompt からビルドする場合:

```powershell
msbuild dx1.vcxproj /p:Configuration=Debug /p:Platform=Win32
```

公開準備時に `Debug|Win32` のビルド成功を確認しています。数値変換、符号付き・符号なし比較、`Field.cpp` の文字コードなどの警告は残っています。

x64 の構成も存在しますが、DxLib のインクルード・ライブラリのパス設定は未整備です。使用する場合は x64 用ライブラリに合わせて設定してください。

## 操作

| キー・状況 | 動作 |
| --- | --- |
| 方向キー | 移動 / メニュー項目の選択 |
| Shift | メインメニューを開く |
| Enter | メニュー項目を決定 |
| 階段に乗る | 階段メニューを自動表示 |
| ウィンドウを閉じる | 終了 |

「足元」からの階段メニュー再表示とメッセージ履歴の閲覧は未実装です。階段で「戻る」を選んだ場合は、一度離れて乗り直すと再表示されます。

## ファイル構成

- `main.cpp` / `Control.*`: 起動、ゲーム進行、メニュー操作
- `Chara.h` / `Player.*` / `Animator.h`: キャラクターと移動・アニメーション
- `Field.*` / `Camera.*`: マップ生成と描画
- `UI.*` / `Frame.*` / `Menu.*`: UI とメニュー
- `Input.*` / `TurnManager.h` / `LogManager.*`: 入力、ターン通知、ログ
- `Graphic/`: 画像素材
- `Font/`: Work Sans フォントとライセンス
- `css/ui.html`: UI デザインの試作

ビルド生成物、実行ファイル、Visual Studio の個人設定、ログは Git の管理対象から除外しています。

## 参考・素材

元の参考資料と制作メモは `Readme.txt` に残しています。現在の開発中コードには、旧実装の機能すべてが移植されているわけではありません。

- 参考動画: [制作時の参考動画](https://youtu.be/xEA-K5HSw08)
- Work Sans: [Work Sans Project](https://github.com/weiweihuanghuang/Work-Sans)、SIL Open Font License 1.1。ライセンス全文は `Font/OFL.txt` を参照してください。

現在のフォント登録コードは `Graphic/WorkSans-Bold.ttf` を参照していますが、実ファイルは `Font/WorkSans-Bold.ttf` にあります。Work Sans を使用する場合は登録パスを合わせてください。
