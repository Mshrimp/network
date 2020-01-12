
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static char card_num[15][16] = {"3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A", "2", "w", "W"};

static char card_color[5][16] = {"HongTao", "HeiTao", "FangKuai", "MeiHua", "Wang"};

#define ALL_CARD_NUM_MAX	54
#define	PLAYERS_NUM_MAX		3
#define CARD_LAST_REMAIN	3
#define CARD_DATA_MAX		13
#define ALL_CARD_COLOR_CNT_MAX	4
#define	PLAYER_CARD_NUM_MAX	((ALL_CARD_NUM_MAX - CARD_LAST_REMAIN) / PLAYERS_NUM_MAX + CARD_LAST_REMAIN)

typedef struct card_info {
	int data;
	int color;
} card_t;

typedef struct player_info {
	card_t card[PLAYER_CARD_NUM_MAX];
	int cnt;
	int num;
	int identity;
} player_t;

#define PLAYER_IDENTITY_HOST
#define PLAYER_IDENTITY_SLAVE

/*
enum card_color = {
	HONGTAO = 1,
	HEITAO = 2,

} color_t;
*/

player_t player[PLAYERS_NUM_MAX] = { 0 };

card_t all_card[ALL_CARD_NUM_MAX] = { 0 };
card_t player[PLAYERS_NUM_MAX][PLAYER_CARD_NUM_MAX] = { 0 };
card_t card_remain[CARD_LAST_REMAIN] = { 0 };


int exchange(int *data1, int *data2)
{
	int totle = 0;

	totle = *data1 + *data2;
	*data1 = totle - *data1;
	*data2 = totle - *data1;

	return 0;
}

int card_exchange(card_t *card1, card_t *card2)
{
	exchange(&card1->data, &card2->data);
	exchange(&card1->color, &card2->color);

	return 0;
}

int card_show(card_t *card, int num)
{
	int i = 0;

	for (i = 0; i < num; i++) {
	//	if ((0 != card[i].data) && (0 != card[i].color)) {
			printf("%d: %d, %s, %s\n", i, card[i].data, card_num[card[i].data - 1], card_color[card[i].color - 1]);
	//	}
	}
	printf("\n");

	return 0;
}

int players_card_show(void)
{
	int num = PLAYER_CARD_NUM_MAX;
	int i = 0;
	int people = 0;

	for (i = 0; i < num; i++) {
		printf("%d: ", i);
		for (people = 0; people < PLAYERS_NUM_MAX; people++) {
			if ((0 != player[people][i].data) && (0 != player[people][i].color)) {
				printf("%s, %s\t",
						card_num[player[people][i].data - 1], card_color[player[people][i].color - 1]);
			}
		}
		printf("\n");
	}
	printf("\n");

	return 0;
}

int card_init(card_t *card, int num)
{
	int cnt = 0;

	for (cnt = 0; cnt < ALL_CARD_NUM_MAX - 2; cnt++) {
		all_card[cnt].data = cnt % CARD_DATA_MAX + 1;
		all_card[cnt].color = cnt / CARD_DATA_MAX + 1;
	}

	all_card[cnt].data = 14;
	all_card[cnt++].color = ALL_CARD_COLOR_CNT_MAX + 1;
	all_card[cnt].data = 15;
	all_card[cnt++].color = ALL_CARD_COLOR_CNT_MAX + 1;

	return 0;
}

int card_shuffle(card_t *card, int num)
{
	int where = 0;
	int i = 0;

	srand((unsigned int)time(NULL));

	for (i = num - 1; i > 0; i--) {
		where = rand() % i;
		card_exchange(&card[i], &card[where]);
	}

	return 0;
}

int card_start(card_t *card, int num)
{
	int cnt = 0;
	int i = 0;

	for (i = 0; i < num - CARD_LAST_REMAIN; i++) {
		memcpy(&player[i % 3][i / 3], &card[i], sizeof(card_t));
	}

	cnt = i;
	for (i = 0; i < CARD_LAST_REMAIN; i++) {
		memcpy(&card_remain[i], &card[cnt++], sizeof(card_t));
	}

	return 0;
}

int card_sort(card_t *card, int num)
{
	int i = 0;
	int j = 0;

	for (i = 0; i < num - 1; i++) {
		for (j = i; j < num; j++) {
			if (card[i].data > card[j].data) {
				card_exchange(&card[i], &card[j]);
			}
		}
	}

	return 0;
}

int card_pop(card_t *player, card_t card)
{
	int i = 0;

	for (i = 0; i < PLAYER_CARD_NUM_MAX; i++) {
		if (player[i].data == card.data) {
			player[i].data = 0;
			player[i].color = 0;
			break;
		}
	}

	return 0;
}

int card_pop_by_data(card_t *player, int data)
{
	int i = 0;

	data -= 2;

	for (i = 0; i < PLAYER_CARD_NUM_MAX; i++) {
		if (player[i].data == data) {
			player[i].data = 0;
			player[i].color = 0;
			break;
		}
	}

	return 0;
}

int main(void)
{
	int num = 0;
	int i = 0;
	int cnt = 0;

	num = sizeof(all_card) / sizeof(all_card[0]);
	printf("num: %d\n", num);
	card_init(all_card, num);
	card_show(all_card, num);

	card_shuffle(all_card, num);
	card_show(all_card, num);

	card_start(all_card, num);
	for (i = 0; i < PLAYERS_NUM_MAX; i++) {
		//card_sort(player[i], PLAYER_CARD_NUM_MAX);
		card_show(player[i], PLAYER_CARD_NUM_MAX);
	}
	card_show(card_remain, CARD_LAST_REMAIN);
	players_card_show();

	printf("Please input the card num to pop:\n");
	scanf("%d", &cnt);

	card_pop_by_data(player[2], cnt);

	for (i = 0; i < PLAYERS_NUM_MAX; i++) {
		//card_sort(player[i], PLAYER_CARD_NUM_MAX);
		//;card_show(player[i], PLAYER_CARD_NUM_MAX);
	}
	players_card_show();

	return 0;
}





