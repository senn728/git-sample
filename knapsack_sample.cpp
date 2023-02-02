//=============================================================================
//
//  [ knapsack ] 遺伝的アルゴリズムのサンプル（ナップサック問題）
//
//=============================================================================

//----- インクルードファイル
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

//----- 定数定義
#define BAG_SIZE	400			// サックに入る最大重量
#define ITEM_NUM	20			// 対象アイテム数

#define POP_SIZE	10			// 遺伝子総数
#define GENE_LENGTH	(ITEM_NUM)	// 遺伝子のビット数

#define CROS_RATE	0.8			// 交叉頻度
#define MUTE_RATE	0.1			// 突然変異頻度

#define GENE_MAX	10			// 世代交代数

const int g_DataWeight[] = {80,70,60,34,89,10,20,30,90,38,75,24,55,15,28,40,50,64,15,79,};	// 重量
const int g_DataValue [] = {10, 5,20, 8, 2, 5, 7,14,22, 9,11, 3,18,13, 6,21,24, 1, 4,16,};	// 価値

//----- グローバル変数
int g_Generation;						// 世代数
int g_Gene[POP_SIZE][GENE_LENGTH];		// 遺伝子集団
int g_Fitness[POP_SIZE];				// 適応度集団
int g_AllMaxFitness;					// 最高適応度
int g_AllMaxFitnessGeneration;			// 最高適応度出現世代
int g_AllMaxFitnessIndex;				// 最高適応度遺伝子番号
int g_AllMaxFitnessGene[GENE_LENGTH];	// 最高適応度遺伝子（これが解となる）

//----- プロトタイプ
void check_result();
void initialize_pop_binary();
void M_selection();
void M_crossover();
void M_mutation();
void calc_fitness_pop();
int calc_fitness_gene(int gene_no);

//-----------------------------------------------------------------------------
//	メイン
//-----------------------------------------------------------------------------
void main(void)
{
	int		i, j, wk_weight;

	// 初期化
	i = 0;
	j = 0;
	g_Generation = 0;
	g_AllMaxFitness = 0;
	g_AllMaxFitnessGeneration = 0;
	g_AllMaxFitnessIndex = 0;
	initialize_pop_binary();	// 遺伝子集団の初期化

	// 遺伝子集団の表示
	printf("-------- 遺伝子集団 --------\n");
	for (i = 0; i < POP_SIZE; i++) {
		printf("遺伝子番号[%2d] = ", i);
		for (j = 0; j < GENE_LENGTH; j++) {
			printf("%2d ", g_Gene[i][j]);
		}
		printf("\n");
	}

	// 遺伝子集団の適応度を計算
	calc_fitness_pop();

	// 世代交代をしながら遺伝子を進化させる
	for (g_Generation = 1; g_Generation < GENE_MAX; g_Generation++) {

		printf("\n******** 世代 = %d ********\n", g_Generation);

		M_selection();			// 選択

		M_crossover();			// 交叉

		M_mutation();			// 突然変異

		calc_fitness_pop();		// 適応度計算

		check_result();			// 現在の遺伝子情報を表示

	}

	// 結果を出力
	printf("======== 計算終了 ========\n");
	printf("最高適応度      = %d\n", g_AllMaxFitness);
	printf("最高遺伝子世代  = %d\n", g_AllMaxFitnessGeneration);
	printf("最高遺伝子[%2d] = ", g_AllMaxFitnessIndex);
	wk_weight = 0;
	for (i = 0; i < GENE_LENGTH; i++) {
		printf("%2d", g_AllMaxFitnessGene[i]);
		if (g_AllMaxFitnessGene[i] == 1) wk_weight += g_DataWeight[i];
	}
	printf("\n総重量          = %d\n", wk_weight);

	_getch();
}

//-----------------------------------------------------------------------------
//	遺伝子、適応度の状態を表示
//-----------------------------------------------------------------------------
void check_result()
{
	int		i, j;

	for (i = 0; i < POP_SIZE; i++) {
		printf("遺伝子番号[%2d] = ", i);
		for (j = 0; j < GENE_LENGTH; j++) {
			printf("%d ", g_Gene[i][j]);
		}
		printf(" > 適応度 = %d\n", g_Fitness[i]);
	}
}

//-----------------------------------------------------------------------------
//	遺伝子の初期集団を作成
//-----------------------------------------------------------------------------
void initialize_pop_binary()
{
	int		i, j;

	srand((unsigned)time(NULL));	// 乱数シードの設定
	for (i = 0; i < POP_SIZE; i++) {
		// ０，１で初期化（＝遺伝子の作成）
		for (j = 0; j < GENE_LENGTH; j++) g_Gene[i][j] = rand() % 2;
	}
}

//-----------------------------------------------------------------------------
//	遺伝子集団から選択（トーナメント戦略みたいな方法）
//-----------------------------------------------------------------------------
void M_selection()
{
	int		i, j, k;

	// ２つずつ適応度を比較し適応度の高い方の遺伝子をもう片方にコピーする
	for (i = 1; i < POP_SIZE; i += 2) {
		j = (i + 1) % POP_SIZE;
		if (g_Fitness[i] < g_Fitness[j]) {
			for (k = 0; k < GENE_LENGTH; k++) g_Gene[i][k] = g_Gene[j][k];
		} else {
			for (k = 0; k < GENE_LENGTH; k++) g_Gene[j][k] = g_Gene[i][k];
		}
	}
}

//-----------------------------------------------------------------------------
//	遺伝子を交叉
//-----------------------------------------------------------------------------
void M_crossover()
{
	int				gene1[GENE_LENGTH], gene2[GENE_LENGTH];
	unsigned char	work;
	int				i, j;
	int				c_pos;
	double			r;

	for (i = 0; i < (POP_SIZE - 1); i += 2) {
		// 交叉させるかどうかを乱数から判定する
		r = ((double)(rand() % 10001)) / 10000.0;
		if (r <= CROS_RATE) {
			// 一度交叉させる２つの遺伝子をワークにコピーする
			for (j = 0; j < GENE_LENGTH; j++) {
				gene1[j] = g_Gene[i  ][j];
				gene2[j] = g_Gene[i+1][j];
			}

			// 乱数を持ちいて交叉位置を決定し、その値をc_posへ代入する
			c_pos = rand() % GENE_LENGTH;

			// 値以降の遺伝子情報を交叉させる
			for (j = c_pos; j < GENE_LENGTH; j++) {
				work	 = gene1[j];
				gene1[j] = gene2[j];
				gene2[j] = work;
			}

			// 交叉した新しい遺伝子を遺伝子情報に上書きコピー
			for (j = 0; j < GENE_LENGTH; j++) {
				g_Gene[i  ][j] = gene1[j];
				g_Gene[i+1][j] = gene2[j];
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	遺伝子集団に突然変異を起こす 突然変異の方法は何でもよい
//-----------------------------------------------------------------------------
void M_mutation()
{
	int		i, j;
	double	r;
	int		pos;

	for (i = 0; i < POP_SIZE; i++) {
		for (j = 0; j < GENE_LENGTH; j++) {
			// 突然変異が起きるかを乱数で決定
			r = (rand() % 10001)/(10000.0);
			if (MUTE_RATE > r) {
				pos = rand() % GENE_LENGTH;
				// 数値を反転（突然変異）
				g_Gene[i][pos] ^= 1;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	遺伝子集団全ての適応度を計算して最高値を保存
//-----------------------------------------------------------------------------
void calc_fitness_pop()
{
	int		i, work_i, index;

	work_i = 0;
	index = 0;
	for (i = 0; i < POP_SIZE; i++) {
		// 指定遺伝子の適応度を計算
		g_Fitness[i] = calc_fitness_gene(i);

		// 適応度の最大値を求める
		if (work_i < g_Fitness[i]) {
			work_i = g_Fitness[i];
			index = i;
		}
	}

	// 今までの最高適応度よりも上回ったら更新
	if (work_i > g_AllMaxFitness) {
		// 最高遺伝子情報を保存
		g_AllMaxFitness		      = work_i;
		g_AllMaxFitnessGeneration = g_Generation;
		g_AllMaxFitnessIndex	  = index;
		for (i = 0; i < GENE_LENGTH; i++) {
			g_AllMaxFitnessGene[i] = g_Gene[g_AllMaxFitnessIndex][i];
		}

		// 情報を出力
		printf("!!!! より良い遺伝子を発見 !!!!\n");
		printf("遺伝子[%2d] = ",g_AllMaxFitnessIndex);
		for (i = 0; i < GENE_LENGTH; i++) {
			printf("%2d",g_AllMaxFitnessGene[i]);
		}
		printf("\n");
		printf("適応度 = %d\n",g_AllMaxFitness);
	}
}

//-----------------------------------------------------------------------------
//	遺伝子の適応度を計算
//-----------------------------------------------------------------------------
int calc_fitness_gene(int gene_no)
{
	int		i, work_w, work_v;

	work_w = 0;
	work_v = 0;
	for (i = 0; i < GENE_LENGTH; i++) {
		if (g_Gene[gene_no][i] == 1) {
			work_w += g_DataWeight[i];
			work_v += g_DataValue [i];
		}
	}
	// もし合計値がバッグの許容量をオーバーしたら適応度０とする
	if (work_w > BAG_SIZE) work_v = 0;

	return work_v;
}

//=============================================================================
//  End of File
//=============================================================================
