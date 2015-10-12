## なにこれ

[第26回プログラミングコンテスト](http://www.procon.gr.jp/)に使用した、問題解くアレです。

### 開発環境
* MacOSX Yosemite,El Capitan
* Ubuntu 15.04
* clang-600.0.57

## どうしたら動くの

おもむろに
```console:
git clone https://github.com/hal1437/Procon26/
```
を実行します。もちろんgitが入ってないと怒られます。  
　  

```console:
cd Procon26/Hal/
```
ディレクトリを移動して  
```console:
cmake .
```
```console:
make
```
ここでcmakeして、makeをします。同様にここでcmake入ってないと怒られるので、
ご自慢のパッケージマネージャーで入れるなりなんなりして…  


運良くビルドできたならば
```console:
./Procon26-Hal
```
を実行することで問題を解くことができる。

問題変えたいとかネットワーク使って問題取得するとかもあったけど、コメントアウトだのなんだのしてあるので、
main.cppを適宜変更してください。


## アルゴリズム

ここからプログラムの内部実装の話。  
プログラムは大きく分けて3に分類される。
* 盤面を評価する Heuristics
* 評価値を元に問題を解く Solver
* ある条件のとき探索を打ち切る Perfect

基本的にHeuristicsの値が高くなるように探索し、Perfectで無駄な探索を省く形になる。  
今回基本的な動作単位となるブロックは`Matrix`型として扱い、
特に大きさが8x8のものを`Block`、32x32のものを`Field`として定義している。
地獄のようなプロキシクラスのおかげで省メモリしつつランダムアクセスっぽいことができる。


### Heuristics
盤面を評価する。  
評価基準はいくつかあり、それぞれ重みをかけた総和が最終的な評価値になる。これはWeightCompositがうまくやってくれる。
評価基準は以下の通り
* 埋めたブロックの集積度 DensityAround
* 埋めていないブロックの集積度 AntiDensityAround
* 埋まっていない空間の面積 MinArea
* 盤面端への集積度 Frame
* 乱数 Random

それぞれ見てもらえればわかるが、実装はそれほど難しくない。
重要なのはそれぞれの重みであり、これは問題によって最適化される必要があるが、
解決が難しいため最後まで人力での調整になった。  
乱数の評価関数は回答が局所解で止まってしまうことを防ぐために導入した。これ高めに入れると神頼みができる。

### Solver
盤面を解く。
今回はビームサーチを基本にしていろいろ加えた探索を行った。
探索の完全性と最適性、どちらもビームサーチには備わっていないため、雑魚探索のように聞こえるが勿論良い点もある。  
探索は並列化しにくいという問題があったりするが、ビームサーチは探索アルゴリズムのなかでも並列化しやすい。実際速い。  
なお、今回はビーム幅に加え、探索するノードの数を制限する優先幅、追加するノードを制限する追加幅も加えたがあまり変わっていないような気がする。尚、優先幅>追加幅となればなるほどノードの親が保持されやすく、考え直しが効きやすい…かもしれない。

### Perfect
盤面を打ち切る。

>**現在の盤面の空きブロック > 残りブロックの面積総和**
>という条件が満たされし時、空は落ち、大地は割れ、完全回答(空きマス数0)が提出することは失われるだろう。  

Perectはそれだけではない、その他にもブロックの位相的特徴量というものを利用したものも今回は使用している。
  
---

それと練習場ではアメリカ高専のトムとかHal Stroemeriaとか名乗ってました。自発お願いします。
