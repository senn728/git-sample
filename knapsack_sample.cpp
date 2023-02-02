//=============================================================================
//
//  [ knapsack ] ��`�I�A���S���Y���̃T���v���i�i�b�v�T�b�N���j
//
//=============================================================================

//----- �C���N���[�h�t�@�C��
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

//----- �萔��`
#define BAG_SIZE	400			// �T�b�N�ɓ���ő�d��
#define ITEM_NUM	20			// �ΏۃA�C�e����

#define POP_SIZE	10			// ��`�q����
#define GENE_LENGTH	(ITEM_NUM)	// ��`�q�̃r�b�g��

#define CROS_RATE	0.8			// �����p�x
#define MUTE_RATE	0.1			// �ˑR�ψٕp�x

#define GENE_MAX	10			// �����㐔

const int g_DataWeight[] = {80,70,60,34,89,10,20,30,90,38,75,24,55,15,28,40,50,64,15,79,};	// �d��
const int g_DataValue [] = {10, 5,20, 8, 2, 5, 7,14,22, 9,11, 3,18,13, 6,21,24, 1, 4,16,};	// ���l

//----- �O���[�o���ϐ�
int g_Generation;						// ���㐔
int g_Gene[POP_SIZE][GENE_LENGTH];		// ��`�q�W�c
int g_Fitness[POP_SIZE];				// �K���x�W�c
int g_AllMaxFitness;					// �ō��K���x
int g_AllMaxFitnessGeneration;			// �ō��K���x�o������
int g_AllMaxFitnessIndex;				// �ō��K���x��`�q�ԍ�
int g_AllMaxFitnessGene[GENE_LENGTH];	// �ō��K���x��`�q�i���ꂪ���ƂȂ�j

//----- �v���g�^�C�v
void check_result();
void initialize_pop_binary();
void M_selection();
void M_crossover();
void M_mutation();
void calc_fitness_pop();
int calc_fitness_gene(int gene_no);

//-----------------------------------------------------------------------------
//	���C��
//-----------------------------------------------------------------------------
void main(void)
{
	int		i, j, wk_weight;

	// ������
	i = 0;
	j = 0;
	g_Generation = 0;
	g_AllMaxFitness = 0;
	g_AllMaxFitnessGeneration = 0;
	g_AllMaxFitnessIndex = 0;
	initialize_pop_binary();	// ��`�q�W�c�̏�����

	// ��`�q�W�c�̕\��
	printf("-------- ��`�q�W�c --------\n");
	for (i = 0; i < POP_SIZE; i++) {
		printf("��`�q�ԍ�[%2d] = ", i);
		for (j = 0; j < GENE_LENGTH; j++) {
			printf("%2d ", g_Gene[i][j]);
		}
		printf("\n");
	}

	// ��`�q�W�c�̓K���x���v�Z
	calc_fitness_pop();

	// ����������Ȃ����`�q��i��������
	for (g_Generation = 1; g_Generation < GENE_MAX; g_Generation++) {

		printf("\n******** ���� = %d ********\n", g_Generation);

		M_selection();			// �I��

		M_crossover();			// ����

		M_mutation();			// �ˑR�ψ�

		calc_fitness_pop();		// �K���x�v�Z

		check_result();			// ���݂̈�`�q����\��

	}

	// ���ʂ��o��
	printf("======== �v�Z�I�� ========\n");
	printf("�ō��K���x      = %d\n", g_AllMaxFitness);
	printf("�ō���`�q����  = %d\n", g_AllMaxFitnessGeneration);
	printf("�ō���`�q[%2d] = ", g_AllMaxFitnessIndex);
	wk_weight = 0;
	for (i = 0; i < GENE_LENGTH; i++) {
		printf("%2d", g_AllMaxFitnessGene[i]);
		if (g_AllMaxFitnessGene[i] == 1) wk_weight += g_DataWeight[i];
	}
	printf("\n���d��          = %d\n", wk_weight);

	_getch();
}

//-----------------------------------------------------------------------------
//	��`�q�A�K���x�̏�Ԃ�\��
//-----------------------------------------------------------------------------
void check_result()
{
	int		i, j;

	for (i = 0; i < POP_SIZE; i++) {
		printf("��`�q�ԍ�[%2d] = ", i);
		for (j = 0; j < GENE_LENGTH; j++) {
			printf("%d ", g_Gene[i][j]);
		}
		printf(" > �K���x = %d\n", g_Fitness[i]);
	}
}

//-----------------------------------------------------------------------------
//	��`�q�̏����W�c���쐬
//-----------------------------------------------------------------------------
void initialize_pop_binary()
{
	int		i, j;

	srand((unsigned)time(NULL));	// �����V�[�h�̐ݒ�
	for (i = 0; i < POP_SIZE; i++) {
		// �O�C�P�ŏ������i����`�q�̍쐬�j
		for (j = 0; j < GENE_LENGTH; j++) g_Gene[i][j] = rand() % 2;
	}
}

//-----------------------------------------------------------------------------
//	��`�q�W�c����I���i�g�[�i�����g�헪�݂����ȕ��@�j
//-----------------------------------------------------------------------------
void M_selection()
{
	int		i, j, k;

	// �Q���K���x���r���K���x�̍������̈�`�q�������Е��ɃR�s�[����
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
//	��`�q������
//-----------------------------------------------------------------------------
void M_crossover()
{
	int				gene1[GENE_LENGTH], gene2[GENE_LENGTH];
	unsigned char	work;
	int				i, j;
	int				c_pos;
	double			r;

	for (i = 0; i < (POP_SIZE - 1); i += 2) {
		// ���������邩�ǂ����𗐐����画�肷��
		r = ((double)(rand() % 10001)) / 10000.0;
		if (r <= CROS_RATE) {
			// ��x����������Q�̈�`�q�����[�N�ɃR�s�[����
			for (j = 0; j < GENE_LENGTH; j++) {
				gene1[j] = g_Gene[i  ][j];
				gene2[j] = g_Gene[i+1][j];
			}

			// �������������Č����ʒu�����肵�A���̒l��c_pos�֑������
			c_pos = rand() % GENE_LENGTH;

			// �l�ȍ~�̈�`�q��������������
			for (j = c_pos; j < GENE_LENGTH; j++) {
				work	 = gene1[j];
				gene1[j] = gene2[j];
				gene2[j] = work;
			}

			// ���������V������`�q����`�q���ɏ㏑���R�s�[
			for (j = 0; j < GENE_LENGTH; j++) {
				g_Gene[i  ][j] = gene1[j];
				g_Gene[i+1][j] = gene2[j];
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	��`�q�W�c�ɓˑR�ψق��N���� �ˑR�ψق̕��@�͉��ł��悢
//-----------------------------------------------------------------------------
void M_mutation()
{
	int		i, j;
	double	r;
	int		pos;

	for (i = 0; i < POP_SIZE; i++) {
		for (j = 0; j < GENE_LENGTH; j++) {
			// �ˑR�ψق��N���邩�𗐐��Ō���
			r = (rand() % 10001)/(10000.0);
			if (MUTE_RATE > r) {
				pos = rand() % GENE_LENGTH;
				// ���l�𔽓]�i�ˑR�ψفj
				g_Gene[i][pos] ^= 1;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	��`�q�W�c�S�Ă̓K���x���v�Z���čō��l��ۑ�
//-----------------------------------------------------------------------------
void calc_fitness_pop()
{
	int		i, work_i, index;

	work_i = 0;
	index = 0;
	for (i = 0; i < POP_SIZE; i++) {
		// �w���`�q�̓K���x���v�Z
		g_Fitness[i] = calc_fitness_gene(i);

		// �K���x�̍ő�l�����߂�
		if (work_i < g_Fitness[i]) {
			work_i = g_Fitness[i];
			index = i;
		}
	}

	// ���܂ł̍ō��K���x������������X�V
	if (work_i > g_AllMaxFitness) {
		// �ō���`�q����ۑ�
		g_AllMaxFitness		      = work_i;
		g_AllMaxFitnessGeneration = g_Generation;
		g_AllMaxFitnessIndex	  = index;
		for (i = 0; i < GENE_LENGTH; i++) {
			g_AllMaxFitnessGene[i] = g_Gene[g_AllMaxFitnessIndex][i];
		}

		// �����o��
		printf("!!!! ���ǂ���`�q�𔭌� !!!!\n");
		printf("��`�q[%2d] = ",g_AllMaxFitnessIndex);
		for (i = 0; i < GENE_LENGTH; i++) {
			printf("%2d",g_AllMaxFitnessGene[i]);
		}
		printf("\n");
		printf("�K���x = %d\n",g_AllMaxFitness);
	}
}

//-----------------------------------------------------------------------------
//	��`�q�̓K���x���v�Z
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
	// �������v�l���o�b�O�̋��e�ʂ��I�[�o�[������K���x�O�Ƃ���
	if (work_w > BAG_SIZE) work_v = 0;

	return work_v;
}

//=============================================================================
//  End of File
//=============================================================================
