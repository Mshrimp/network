
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
	char sym_str[4];
	char col_str[16];
} card_t;

typedef struct player_info {
	card_t card[PLAYER_CARD_NUM_MAX];
	int index;
	int num;
	int identity;
} player_t;

#define PLAYER_IDENTITY_HOST	0
#define PLAYER_IDENTITY_SLAVE	1


player_t players[PLAYERS_NUM_MAX] = { 0 };

card_t all_card[ALL_CARD_NUM_MAX] = { 0 };
card_t card_remain[CARD_LAST_REMAIN] = { 0 };



char user_input[64] = { 0 };

int data_exchange(int *data1, int *data2)
{
	int totle = 0;

	totle = *data1 + *data2;
	*data1 = totle - *data1;
	*data2 = totle - *data1;

	return 0;
}

int str_exchange(char *str1, char *str2, int max_len)
{
	char *pTmp = NULL;

	pTmp = malloc(max_len);

	strncpy(pTmp, str1, max_len);
	strncpy(str1, str2, max_len);
	strncpy(str2, pTmp, max_len);
	
	return 0;
}

int card_exchange(card_t *card1, card_t *card2)
{
	data_exchange(&card1->data, &card2->data);
	data_exchange(&card1->color, &card2->color);
	str_exchange(card1->sym_str, card2->sym_str, 4);
	str_exchange(card1->col_str, card2->col_str, 16);

	return 0;
}

int get_player_max_card_num(player_t *player)
{
	int max = 0;
	int i = 0;

	for (i = 0; i < PLAYERS_NUM_MAX; i++) {
		if (max < player[i].num) {
			max = player[i].num;
		}
	}

	return max;
}

int card_show(card_t *card, int num)
{
	int i = 0;

	for (i = 0; i < num; i++) {
		if ((0 != card[i].data) && (0 != card[i].color)) {
			printf("%d: %d, %s, %s\n", i + 1, card[i].data, card[i].sym_str, card[i].col_str);
		}
	}
	printf("\n");

	return 0;
}

int players_card_show(player_t *players)
{
	int num = PLAYER_CARD_NUM_MAX;
	int i = 0;
	int people = 0;

	num = get_player_max_card_num(players);

	for (i = 0; i < num; i++) {
		printf("%d: ", i + 1);
		for (people = 0; people < PLAYERS_NUM_MAX; people++) {
			if ((0 != players[people].card[i].data) && (0 != players[people].card[i].color)) {
				printf("%d, %s, %-8s\t",
						players[people].card[i].data, players[people].card[i].sym_str, players[people].card[i].col_str);
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
	int i = 0;

	for (cnt = 0; cnt < ALL_CARD_NUM_MAX - 2; cnt++) {
		all_card[cnt].data = cnt % CARD_DATA_MAX + 1;
		all_card[cnt].color = cnt / CARD_DATA_MAX + 1;
		strcpy(all_card[cnt].sym_str, card_num[cnt % CARD_DATA_MAX]);
		strcpy(all_card[cnt].col_str, card_color[cnt / CARD_DATA_MAX]);
	}

	for (i = 0; i < 2; i++, cnt++) {
		all_card[cnt].data = 14 + i;
		all_card[cnt].color = ALL_CARD_COLOR_CNT_MAX + 1;
		strcpy(all_card[cnt].sym_str, card_num[cnt % CARD_DATA_MAX + CARD_DATA_MAX]);
		strcpy(all_card[cnt].col_str, card_color[cnt / CARD_DATA_MAX]);
	}

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
		memcpy(&players[i % 3].card[i / 3], &card[i], sizeof(card_t));
		players[i % 3].num++;
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
			if (((card[i].data > card[j].data) 
					|| (0 == card[i].data)) 
					&& (0 != card[j].data)) {
				card_exchange(&card[i], &card[j]);
			}
		}
	}

	return 0;
}

int players_card_sort(player_t *player, int num)
{
	int i = 0;

	for (i = 0; i < num; i++) {
		//card_show(player[i].card, PLAYER_CARD_NUM_MAX);
		card_sort(player[i].card, PLAYER_CARD_NUM_MAX);
		//card_show(player[i].card, PLAYER_CARD_NUM_MAX);
	}

	return 0;
}

int data_to_card_num(int data)
{
	int value = 0;

	if (data < 3) {
		value = data + 11;
	} else if (data < 14) {
		value = data - 2;
	} else {
		value = data;
	}
	
	return value;
}

int card_num_to_data()
{
	return 0;
}

int card_pop(card_t *player, card_t card)
{
	int i = 0;

	for (i = 0; i < PLAYER_CARD_NUM_MAX; i++) {
		if (player[i].data == card.data) {
			player[i].data = 0;
			player[i].color = 0;
			memset(player[i].sym_str, 0, sizeof(player[i].sym_str));
			memset(player[i].col_str, 0, sizeof(player[i].col_str));
			break;
		}
	}

	return 0;
}

int card_pop_by_data(player_t *player, int data)
{
	int i = 0;

	data = data_to_card_num(data);

	for (i = 0; i < PLAYER_CARD_NUM_MAX; i++) {
		if (player->card[i].data == data) {
			player->card[i].data = 0;
			player->card[i].color = 0;
			memset(player->card[i].sym_str, 0, sizeof(player->card[i].sym_str));
			memset(player->card[i].col_str, 0, sizeof(player->card[i].col_str));
			player->num--;
			break;
		}
	}

	return 0;
}

int card_pop_by_symbol(player_t *player, char *str)
{
	int i = 0;
	int len = 0;

	len = strlen(str);
	if (len > 2) {
		printf("len: %d, error\n", len);
		return -1;
	}

	//printf("player->num: %d\n", player->num);
	for (i = 0; i < PLAYER_CARD_NUM_MAX - 3; i++) {
		if ((0 != len) && (!memcmp(player->card[i].sym_str, str, len))) {
			player->card[i].data = 0;
			player->card[i].color = 0;
			memset(player->card[i].sym_str, 0, sizeof(player->card[i].sym_str));
			memset(player->card[i].col_str, 0, sizeof(player->card[i].col_str));
			player->num--;
			break;
		}
	}

	return 0;
}

int is_card_finish(player_t *player)
{
	int i = 0;

	for (i = 0; i < PLAYERS_NUM_MAX; i++) {
		if (0 == player[i].num) {
			return 1;
		}
	}

	return 0;
}

int card_user_input(char *input)
{
	printf("Your turn, please show your cards:\n");
	scanf("%s", input);
	printf("Your select: %s\n", input);

	return strlen(input) + 1;
}

int card_input_scan(char *input, int len, int *match, int *mis)
{
	char *pstr = input;
	int str[16] = { '*' };
	int match_num = 0;
	int mis_match = 0;
	int i = 0;

	while ('\0' != *pstr) {
		for (i = 0; i < 15; i++) {
			printf("scan i: %d\n", i);
			if (!memcmp(pstr, card_num[i], strlen(card_num[i]))) {
				printf("scan prase: %s, %s\n", card_num[i], pstr);

				pstr += strlen(card_num[i]);
				printf("scan prase: %s\n", pstr);
				match_num++;
				break;
			}
		}
		if (15 == i) {
			pstr++;
			mis_match++;
		}
	}

	*match = match_num;
	*mis = mis_match;

	return 0;
}

int card_player_input_scan(player_t *player, char *input, int len, int *match, int *mis)
{
	char *pstr = input;
	int str[16] = { '*' };
	int match_num = 0;
	int mis_match = 0;
	int i = 0;

	while ('\0' != *pstr) {
		for (i = 0; i < player->num; i++) {
			if (!memcmp(pstr, player->card[i].sym_str, strlen(player->card[i].sym_str))) {
				pstr += strlen(player->card[i].sym_str);
				match_num++;
				break;
			}
		}
		if (player->num == i) {
			pstr++;
			mis_match++;
		}
	}

	*match = match_num;
	*mis = mis_match;

	return 0;
}

int card_user_input_process(player_t *player, char *input)
{
	char *pstr = input;
	int str[16] = { '*' };
	char tmp[4] = { 0 };
	int len = 0;
	int i = 0;

	while ('\0' != *pstr) {
		for (i = 0; i < PLAYER_CARD_NUM_MAX - 3; i++) {
			len = strlen(player->card[i].sym_str);
			if ((0 != len) && (!memcmp(pstr, player->card[i].sym_str, len))) {
				memset(tmp, 0, sizeof(tmp));
				strcpy(tmp, player->card[i].sym_str);
				card_pop_by_symbol(player, tmp);

				pstr += len;
				break;
			}
		}
		// Maybe error
		if (PLAYER_CARD_NUM_MAX <= i) {
			pstr++;
		}
	}

	return 0;
}

int player_output(player_t *player, char *input)
{
	int totle = 0;
	int match = 0;
	int mis = 0;

	card_player_input_scan(player, input, 0, &match, &mis);
	if (0 != mis) {
		printf("Player card mis %d, error\n", mis);
		return -1;
	}

	totle = match + mis;
	printf("totle: %d, match: %d, mis match: %d\n", totle, match, mis);

	card_user_input_process(player, input);
	players_card_sort(players, PLAYERS_NUM_MAX);

	return 0;
}

int main(void)
{
	int num = 0;
	int i = 0;
	int cnt = 0;
	int ret = 0;
	int match = 0;
	int mis = 0;
	int totle = 0;

	num = sizeof(all_card) / sizeof(all_card[0]);
	printf("num: %d\n", num);
	card_init(all_card, num);
	//card_show(all_card, num);

	card_shuffle(all_card, num);
	//card_show(all_card, num);

	card_start(all_card, num);
	players_card_sort(players, PLAYERS_NUM_MAX);

	card_show(card_remain, CARD_LAST_REMAIN);
	players_card_show(players);

	card_user_input(user_input);
	player_output(&players[2], user_input);

	players_card_show(players);

	return 0;
}





