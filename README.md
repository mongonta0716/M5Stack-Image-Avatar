# M5Stack-Image-Avatar(beta version)
 M5Stack Image Avatar(beta version)

日本語 | [English](README_en.md)

# ベータ版の注意

　M5Stack Image AvatarはSpriteの拡大縮小・回転機能を利用しているため[LovyanGFXライブラリ](https://github.com/lovyan03/LovyanGFX)が必要です。（M5Stackライブラリには拡大縮小機能がありません。）現在も開発中なので何らかの障害を引き起こす可能性があります。また、大きな仕様変更を行う場合があります。LovyanGFXとバージョンが合わない場合、動作しない場合もあります。予めご了承ください。

2020/4時点ではM5Stackのライブラリとは同時利用はできないのでボタンイベントはdigitalReadを使用して実装する必要があります。（暫定ですがM5Stackライブラリと同機能を持つ[ESP32-Chimera-Core](https://github.com/lovyan03/ESP32-Chimera-Core)とは同居できます。）


# 概要(Overview)

　あらかじめ用意した画像ファイル（BMP）を組み合わせてAvatarを作成できるアプリです。[AquesTalk for ESP32](https://www.a-quest.com/products/aquestalk_esp32.html)と組み合わせて言葉を喋ることもできます。

# 開発環境(Environment)
- ArduinoIDE 1.8.10(または1.8.12)
## 必要なライブラリ
- [LovyanGFX](https://github.com/lovyan03/LovyanGFX)
- [M5StackSDUpdater](https://github.com/tobozo/M5Stack-SD-Updater)
## AquesTalkのインストール
[AquesTalk for ESP32](https://www.a-quest.com/products/aquestalk_esp32.html)に従ってインストールしてください。<br>
AquesTalkのインストールが難しい方はM5StackImageAvatar.inoの先頭にある#define USE_TTSをコメントアウトしてください。<br>

# 対応機種
- M5Stack Basic,Gray,Fire,Go

## 環境についての注意事項
320x240サイズの8bit Color(256Color)画像ファイルを利用するとメモリを多く消費します。M5Stack Fire以外の機種ではギリギリのため、メモリが足らなくなった場合は4bitColorに減色して利用してください。

# 使い方
SDカードのルートに「bmp」というフォルダを作成しbmpフォルダのファイルを入れてください。
M5StackImageAvatarのフォルダをスケッチフォルダにコピーしてビルドします。

- ボタンA：Avatarが動きます。
- ボタンB：目玉が左右に動きます。
- ボタンC：感情が変わります。

## Avatarモデル
exampleフォルダに[五積ほむら](example/homura_gotsumi)と[アマビエ](example/amabie)の２種類ありますので参考にしてください。

# 画像の準備
表情を豊かにするためにAvatarの部品を7つ用意する必要があります。

## 画像パーツを作成する場合に注意する点
オリジナルのImageAvatarを作成するためには部品となるBMP画像ファイルが必要です。

## 画像作成で使用したツール
- [コミPo!](https://www.comipo.com/)<br>五積ほむらの元データ作成で利用しました。
- [Adobe Illustrator と Photoshop](https://www.adobe.com/)<br>画像の減色・変換及びパーツの切り抜き等で使用しました。アマビエの画像作成でも使用。
- [GIMP](https://www.gimp.org/)<br>パレット情報の書き出しで利用しました。

### 透明色と肌色
部品の重ね合わせを利用するために予め透明色と肌色、目玉の色を決めます。

五積ほむらの場合、透明色（0x00FF00U）,肌色（0xFFE9DBU）,目玉の色（0xFFFFFFU)
アマビエの場合は、透明色(0x00FF00U), 肌色(0xFFFFFFU) ,目玉の色(0xFFFFFFU)

### 回転のためのマージン
萌え軸を利用する場合、Spriteのマージンが必要なので元画像は傾けても大丈夫な大きさで作成します。

サンプルで確認してください。
五積ほむらは液晶サイズ（320x240）で作成しており傾くと一部欠けます。アマビエは液晶サイズ+20（340x260）で作成しているので欠けません。

## 必要なパーツ
- 固定パーツ1（１パターン用意する）
1. 頭部<br>（顔部分は肌色で塗る、ボディーパーツと重なる部分は透明色）
1. 首と胴体部<br>（頭部と重なる部分は透明色）
- 固定パーツ2（感情の数だけ用意する）
1. 目玉<br>黒目以外は目玉の色で塗る
1. 眉毛<br>右だけ用意します。

- 状態が変化するパーツ（開閉ｘ感情の数だけ用意する）
1. 開いたまぶた<br>右だけ用意します。（周りは肌色、まぶたの中は透明色）
1. 閉じたまぶた<br>右だけ用意します。（周りは肌色）
1. 開いた口<br>口の周りは肌色
1. 閉じた口<br>口の周りは肌色

まぶたと口は開いた状態と閉じた状態を同じ大きさで用意します。

### 感情を表現したい場合
感情を表現したいときは固定パーツ（頭部、胴体部）を除くパーツを用意すればBMPファイルを入れ替えることによって切り替えが可能です。

五積ほむらは普通・困った・怒った３つの状態をボタンCを押すことにより切り替え可能なので参考にしてください。

## 画像ファイルのカラーパレット情報を統一する
2020/4の時点では各画像のカラーパレットを共通にして埋め込む必要があります。ここが面倒だと思いますがGIMPやPhotoshopを利用することで共通のカラーパレット（カラープロファイル、カラーマップ）情報を埋め込む作業を行ってください。**基本となるカラーパレットは透明色、肌色を含んだ全色を使用した画像ファイルから作成する必要があります。**[colorpalette.png](png/colorpalette.png)を参考にしてください。

この仕様については、肌色のみ共通で大丈夫なように改善予定です。


# オプション機能
オプション機能については以下の３つがありそれぞれM5StackImageAvatar.inoの#defineで切り替えます。(必要ない場合はコメントアウトする。)
```
#define USE_TTS     // AquesTalk is necessary.(https://www.a-quest.com/products/aquestalk_esp32.html)
#define USE_MIC     // M5Go Bottom's MIC is necessary.
#define USE_WIFI    // M5StackFire is required when using 8bit color.Because Gray and Basic don't have enough memory.

```
- USE_TTS<br>音声合成を利用する。(AquesTalk for ESP32のインストールが必要)
- USE_MIC<br>GoボトムについているMICに合わせてlipsyncします。
- USE_WIFI<br>現在は接続するだけで特に機能は実装していません。メモリを80K程度消費するのでOut Of Memoryに注意してください。4bitBMPでAvatarを作成するかM5Stack Fireを推奨します。

## 設定ファイル
- [src/colorpalette.h](M5StackImageAvatar/src/colopalette.h)<br>画像共通のカラーパレット情報を設定します。
- [src/avatarconfig.h](M5StackImageAvatar/src/avatarconfig.h)<br>Avatarのパラメータ、各画像ファイルの位置、大きさ、角度等を設定します。
- [src/optionconfig.h](M5StackImageAvatar/src/optionconfig.h)<br>WiFi設定・AquesTalkのセリフなどオプション設定。

# 参考にしたリポジトリ
- [m5stack-avatar](https://github.com/meganetaaan/m5stack-avatar)
- [M5Stack_WebRadio_Avator](https://github.com/robo8080/M5Stack_WebRadio_Avator)
- [M5StackFire用サンプル M5StackFire_MicrophoneSpectrumTFT](https://github.com/m5stack/M5Stack/tree/master/examples/Fire/M5StackFire_MicrophoneSpectrumTFT)

# 謝辞
このソフトを作成するにあたり、動きや構造の元となった[M5Stack-Avatar](https://github.com/meganetaaan/m5stack-avatar)を作成・公開してくださった[meganetaaan](https://github.com/meganetaaan)氏に感謝いたします。

ImageAvatarを実現するにあたり優れたパフォーマンス、機能を持ったLovyanGFXの作者[lovyan03](https://github.com/lovyan03)氏に感謝いたします。

五積ほむらを作成するにあたり、 初期の頃からたくさんのアドバイスを頂き、参考にさせていただいた[M5Stack_WebRadio_Avatar](https://github.com/robo8080/M5Stack_WebRadio_Avator)の作者[robo8080](https://github.com/robo8080)氏に感謝いたします。


# Credit
[meganetaaan](https://github.com/meganetaaan)
[lovyan03](https://github.com/lovyan03/LovyanGFX)
[robo8080](https://github.com/robo8080)

# LICENSE
[MIT](LICENSE)

# Author
[Takao Akaki](https://github.com/mongonta0716)

