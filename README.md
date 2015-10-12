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
なお、今回はビーム幅に加え、探索するノードの数を制限する優先幅、追加するノードを制限する追加幅も加えたがあまり変わっていないような気がする。尚、優先幅>追加幅となればなるほどノードの親が保持されやすく、考え直しが効きやすい…かもしれない。制限が3つでもDoubleLimit。別に名前書き換えるの面倒とかそういうのじゃないんだからね！

### Perfect
盤面を打ち切る。

>**現在の盤面の空きブロック > 残りブロックの面積総和**
>という条件が満たされし時、空は落ち、大地は割れ、完全回答(空きマス数0)が提出することは失われるだろう。  

Perectはそれだけではない、その他にもブロックの位相的特徴量というものを利用したものも今回は使用している。
  
---

それと練習場ではアメリカ高専のトムとかHal Stroemeriaとか名乗ってました。自発お願いします。



## りやさん

りやさんです．

ブロックの操作によって生じる石に囲まれた領域，「閉空間」に対するアプローチを中心に設計したよ．

## アルゴリズム

### Heuristics

評価関数ははるくんのものをいくつか流用し，加えて閉空間の埋め立て条件判定を行ってガンガン枝刈りしていきます．

### Matirx Feature

石の行列特徴量を用いて閉空間の埋め立てが可能かを判断します．

0,1で表現された行列において，ブロックの全ての行，列要素の最初の１と最後の１幅を計算します． 
```例：[1,1,0,0,1,0] = 5```
同様に，配置対象の全ての行，列要素の閉空間の０の最大連続数を計算します．　
```例: [1,0,0,0,1,0] = 3```

また，行数と列数について，大きい方をL, 小さい方をSとして上記の特徴量を降順にソートし，LとSに従属させます．
この時，閉空間にその石を配置できる必要条件（必要十分条件ではない）は

```
閉空間.L.size >= ブロック.L.size && 閉空間.S.size >= ブロック.S.size
```
かつ
```
FOR i=0 to ブロック.L.size do
 閉空間.L.feature[i] >= ブロック.L.feature[i]

FOR i=0 to ブロック.S.size do
 閉空間.S.feature[i] >= ブロック.S.feature[i]
 ```
 
以上の条件を満たさないものは，その閉空間に反転させようが回転させようが何しようが配置できません．

Contain.hは，石適用後の差分閉空間について以上の計算を行い，埋め立て不可能な閉空間が一つでもあるのかをboolで返してくます.

### Closed Field Solver

3×3以下の閉空間において，全ての解答パターンをコンパイル時計算します．少しだけコンパイルに時間がかかります．実行時でいいならdefine定数であるCONSTEXPR_RIYAをconstexprから半角スペースに変えるといいと思います．
ブロックは反転・回転について一般化されたハッシュ化アルゴリズムを適用し，テーブルに格納することで，実行時はNを石の数として，与えられた任意の数のブロックで3×3以下の閉空間を解くことができるかをO(N)で判断できます．
PetternMatching.hはブロックのリストを渡すと，埋め立てできるか否かと，閉空間の解くことのできる石のインデックスを返してくれます．

ここら辺を考慮しつつ，発生した閉空間を使用可能ブロックを使い埋めた上で使用した未来ブロックにロックをかけたりするのはPerfectContainです．無理だったらfalseを返します．

### Solver

基本戦略は貪欲法の中でも最も単純な山登り法．
山登り法で得た解の近傍を焼きなまし法(Simulated Annealing)で探索し，冷却スケジュール中で最もよい解を吐き出します．

あ，宣伝なんですけど，焼きなまし法のモジュールは，りやさんが開発中のライブラリMTLから流用したんだよ．
みてね．[MTL]（https://github.com/Riyaaaaa/Metaheuristics-Template-Library）
