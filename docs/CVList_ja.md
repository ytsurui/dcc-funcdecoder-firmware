# CV List

<table>
  <tr>
    <td>CV番号</td>
    <td>初期値</td>
    <td>内容</td>
  </tr>
  <tr>
    <td>1</td>
    <td>3</td>
    <td>2桁アドレス (7-bit Address)</td>
  </tr>
  <tr>
    <td>7</td>
    <td>1</td>
    <td>バージョン番号</td>
  </tr>
  <tr>
    <td>8</td>
    <td>13</td>
    <td>メーカーID</td>
  </tr>
  <tr>
    <td>15</td>
    <td>0</td>
    <td>デコーダロック番号</td>
  </tr>
  <tr>
    <td>16</td>
    <td>0</td>
    <td>デコーダロック番号</td>
  </tr>
  <tr>
    <td>17</td>
    <td>192</td>
    <td>4桁アドレス下位 (14-bit Address)</td>
  </tr>
  <tr>
    <td>18</td>
    <td>0</td>
    <td>4桁アドレス上位 (14-bit Address)</td>
  </tr>
  <tr>
    <td>19</td>
    <td>0</td>
    <td>Consist Address</td>
  </tr>
  <tr>
    <td>29</td>
    <td>6</td>
    <td>デコーダ基本設定</td>
  </tr>
  <tr>
    <td>33</td>
    <td>4</td>
    <td>ヘッドライト減光 (Default: F4)</td>
  </tr>
  <tr>
    <td>34</td>
    <td>30</td>
    <td>入れ替え標識灯 (Default: Disable)</td>
  </tr>
  <tr>
    <td>35</td>
    <td>64</td>
    <td>Func1 アドレス・ディレクション設定
    <br />(初期値: F0/前進)</td>
  </tr>
  <tr>
    <td>36</td>
    <td>128</td>
    <td>Func2 アドレス・ディレクション設定
    <br />(初期値: F0/後進)</td>
  </tr>
  <tr>
    <td>37</td>
    <td>3</td>
    <td>Func3 アドレス・ディレクション設定
    <br />(Default: F3)</td>
  </tr>
  <tr>
    <td>112</td>
    <td>64</td>
    <td>ヘッドライト減光機能作動時の照度
    <br />(7bit-4bit)</td>
  </tr>
  <tr>
    <td>114</td>
    <td>241</td>
    <td>Func1 照度およびファンクション種別
    <br>初期値: 照度15 (最大) / ヘッドライト</td>
  </tr>
  <tr>
    <td>115</td>
    <td>242</td>
    <td>Func2 照度およびファンクション種別
    <br>初期値: 照度15 (最大) / テールライト</td>
  </tr>
  <tr>
    <td>116</td>
    <td>240</td>
    <td>Func3 照度およびファンクション種別
    <br>初期値: 照度15 (最大) / ノーマルファンクション</td>
  </tr>
</table>

---

# ファンクションの設定方法

ファンクションに関係するCV値は、Function1からFunction3までの出力ごとにCV番号が2つずつ割り当てられており、それぞれアドレスとディレクションの制御、LED照度とファンクション種別の制御のために割り当てています。

また、出力ポートを持たない仮想ファンクション (ヘッドライト減光制御、ヘッドライト常時点灯) については、CV番号が1つまたは2つ割り当てられています。

<table>
  <tr>
    <th>番号</th>
    <th>アドレス・方向</th>
    <th>照度・種別</th>
  </tr>
  <tr>
    <th>Function 1</th>
    <td>CV35</td>
    <td>CV114</td>
  </tr>
  <tr>
    <th>Function 2</th>
    <td>CV36</td>
    <td>CV115</td>
  </tr>
  <tr>
    <th>Function 3</th>
    <td>CV37</td>
    <td>CV116</td>
  </tr>
  <tr>
    <th>ヘッドライト減光</th>
    <td>CV33</td>
    <td>CV112</td>
  </tr>
  <tr>
    <th>ヘッドライト常時点灯</th>
    <td>CV45</td>
    <td>-</td>
  </tr>
</table>

------

アドレスとディレクション制御のCV値については、下記に示す構造となっています。

<table>
  <tr>
    <td>Bit7</td>
    <td>Bit6</td>
    <td>Bit5</td>
    <td>Bit4 - Bit0</td>
  </tr>
  <tr>
    <td>後進のみ</td>
    <td>前進のみ</td>
    <td>将来の拡張のため予約</td>
    <td>ファンクション番号<br />(0-28, 30, 31)</td>
  </tr>
</table>

ディレクションによる制御を行う場合は、Bit7またはBit6を1にします。

ファンクション番号については、0から28から任意の番号に設定します。

30にした場合はそのファンクションは無効、31にした場合は常時動作するようになります。 <br />ファンクション番号を31にした場合でも、Bit7またはBit6による方向制御は動作しますので、例えば車両が操作されている間、ファンクションボタンを操作しなくても進行方向に応じて点灯させるような設定も可能です。

下記に一部設定例を示します。

<table>
  <tr>
    <th>設定例の動作</th>
    <th>10進数</th>
    <th>2進数</th>
  </tr>
  <tr>
    <th>F0で前進時のみ動作する</th>
    <td>64</td>
    <td>0100 0000</td>
  </tr>
  <tr>
    <th>F0で後進時のみ動作する</th>
    <td>128</td>
    <td>1000 0000</td>
  </tr>
  <tr>
    <th>F3で走行方向にかかわらず動作する</th>
    <td>3</td>
    <td>0000 0011</td>
  </tr>
  <tr>
    <th>F1で前進時のみ動作する</th>
    <td>65</td>
    <td>0100 0001</td>
  </tr>
  <tr>
    <th>Func Buttonの操作にかかわらず<br>前進時に常時点灯する</th>
    <td>95</td>
    <td>0101 1111</td>
  </tr>
  <tr>
    <th>Func Buttonの操作にかかわらず<br>後進時に常時点灯する</th>
    <td>159</td>
    <td>1001 1111</td>
  </tr>
</table>

------

LED照度とファンクション種別のCV値については、下記に示す構造となっています。

<table>
  <tr>
    <th>Bit7 - Bit4</th>
    <th>Bit3 - Bit0</th>
  </tr>
  <tr>
    <td>照度 (100Hz PWM, 16段階)</td>
    <td>ファンクション種別</td>
  </tr>
</table>