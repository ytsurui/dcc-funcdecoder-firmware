# Atmel ATtiny45 DCC Function Decoder Firmware

[English Document](README_en.md)

NMRA規格の信号を受信して作動する、ファンクション専用のDCCデコーダーのファームウェアです。

Atmel AVR ATtiny45 / ATtiny85で動作できるように作成してあります。
<br>(プログラム領域が足りないため、ATtiny25には入りません)

## 主要スペック
  * 3つのファンクション出力に対応
  * それぞれの出力に対し、F0-F28から任意の番号で動作するように設定可能、また走行方向による制御も可能
  * それぞれの出力の照度を個別に設定可能 (16段階)
  * ヘッドライト減光機能を搭載し、停車時と走行時でヘッドライトに設定したファンクションの照度を自動的に変更する機能および、走行中でもファンクションの制御で減光状態に切り替え可能な機能を搭載
  * 進行方向にかかわらず、ヘッドライトが常時点灯するように制御する機能を設定可能
  * 7-Bit Address、14-Bit Addressどちらの設定にも対応
  * Pagedモード、Directモード、Operationモードでのプログラミングに対応
  * アナログコントローラーでの操作に対応。<br>純直流出力コントローラーだけではなく、PWM出力コントローラーにおいてもある程度は正常に制御可能

## 動作確認済みのコマンドステーション・コントローラー類

### DCC走行
  * Digitrax DCS50K (Kato D101)
  * Digitrax DCS100
  * Roco Z21

### アナログ走行
  * TOMIX N-1000CL
  * KATO スタンダードS
  * KATO スタンダードSX

## 対応する基板設計データ
  * Atmel ATtiny45 DCC Decoder PCB for TOMIX 8919
  * Atmel ATtiny45 DCC Decoder PCB for TOMIX 0507/0773

回路を独自に組んで動かす場合は、基板設計データに含まれる回路図を参考にしてください。

## 各種ドキュメント

  * [CV値一覧 / ファンクション番号の設定方法](docs/CVList_ja.md)

## AVR Fuse Bitの設定内容

8MHz内蔵オシレーターで動作するようにするため、下記の内容を書きます。
<br>(表記はAtmel Studioのものとなります)

  * EXTENDED: 0xFF
  * HIGH: 0xDE
  * LOW: 0xE2

Atmel Studioで設定を行う場合、「LOW.CKDIV8」のチェックを外し、「HIGH.BODLEVEL」を「Brown-out detection at VCC=1.8V」に切り替えてください。

![avr-fuse-screenshot](docs/images/avr-fuse-config.png)
