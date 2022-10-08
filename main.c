/****************************************************/
/******** Author     : Shadi Bahaa   ****************/
/******** File name  : main.c        ****************/
/******** Date       : 7/10/2022     ****************/
/******** Version    : V1            ****************/
/******** Desc       : ...           ****************/
/****************************************************/
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "LCD_INTERFACE.h"
#include "KPD_INTERFACE.h"
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
/* Direction macros */
#define TOP 0
#define LEFT 1
#define RIGHT 2
#define BOTTOM 3
/* Array that represents the 32 cells of 16x2 LCD */
u8 arr[32][8] = { { 0 } };
/* Previous snake direction */
s8 prevDir = -1;
/* Current snake direction */
s8 direction = 2;
/* Flag for indicating if the game is over */
u8 flag = 0;
/* game score */
u8 score = 0;
/* new variable for storing the accurate position of a bit */
typedef struct Pos {
	s8 row;
	s8 col;
	s8 byte;
	s8 bit;
	struct Pos* prev;
	struct Pos* next;
} Pos;
typedef Pos* PosPtr;
/* two pointers for indicating the head and tail of snake */
PosPtr head = NULL;
PosPtr tail = NULL;
/* pointer referring to the position of the apple */
PosPtr rando = NULL;
/* functions prototypes */
void shift();
void generateRandom();
u8 trueRandom();
void drawSnake();
void main(void) {
	/* Initializing LCD and Keypad */
	LCD_VidInitialize();
	KPD_VidInit();
	/* cursor OFF */
	LCD_VidSendCommand(0b00001100);
	while (1) {
		/* Getting the current pressed key */
		u8 x = KPD_VidGetPressedKey();
		if ((x >= TOP && x <= BOTTOM)) {
			prevDir = direction;
			direction = x;
		}
		/* moving the snake */
		shift();
		/* checking if the game is over */
		if (flag) {
			LCD_VidClearLCD();
			LCD_VidMoveTo(1, 5);
			LCD_VidSendString("GAME OVER");
			LCD_VidMoveTo(2, 5);
			LCD_VidSendString("Score: ");
			LCD_VidSendIntNumber(score);
			break;
		}
		/* checking if the game is finished */
		if (score == 32) {
			LCD_VidClearLCD();
			LCD_VidMoveTo(1, 5);
			LCD_VidSendString("Congrats!");
			LCD_VidMoveTo(2, 5);
			LCD_VidSendString("Score: ");
			LCD_VidSendIntNumber(score);
			break;
		}
	}
}
/* Function for moving the snake to a specific direction */
void shift() {
	/* If there is no sanke on the screen */
	if (head == NULL) {
		head = (PosPtr) malloc(sizeof(Pos));
		head->row = 1;
		head->col = 0;
		head->byte = 7;
		head->bit = 4;
		head->next = NULL;
		head->prev = NULL;
		tail = (PosPtr) malloc(sizeof(Pos));
		tail = head;
		SET_BIT(arr[(head->row) * 16 + head->col][head->byte], head->bit);
		generateRandom();
		while (!trueRandom()) {
			generateRandom();
		}
		SET_BIT(arr[(rando->row) * 16 + rando->col][rando->byte], rando->bit);
		_delay_ms(50);
		drawSnake();
		return;
	}
	/* if the direction is the reverses of the previous one, keep the previous one */
	if (direction + prevDir == 3) {
		direction = prevDir;
	}
	/* Pointer represents the new head */
	PosPtr new = (PosPtr) malloc(sizeof(Pos));
	/* Moving the snake according to the direction */
	switch (direction) {
	case TOP:
		if (head->byte == 0) {
			new->byte = 7;
			(head->row == 0) ? (new->row = 1) : (new->row = 0);
		} else {
			new->byte = head->byte - 1;
			new->row = head->row;
		}
		new->col = head->col;
		new->bit = head->bit;
		break;
	case LEFT:
		if (head->bit == 4) {
			new->bit = 0;
			(head->col == 0) ? (new->col = 15) : (new->col = head->col - 1);
		} else {
			new->bit = head->bit + 1;
			new->col = head->col;
		}
		new->row = head->row;
		new->byte = head->byte;
		break;
	case RIGHT:
		if (head->bit == 0) {
			new->bit = 4;
			(head->col == 15) ? (new->col = 0) : (new->col = head->col + 1);
		} else {
			new->bit = head->bit - 1;
			new->col = head->col;
		}
		new->row = head->row;
		new->byte = head->byte;
		break;
	case BOTTOM:
		if (head->byte == 7) {
			new->byte = 0;
			(head->row == 1) ? (new->row = 0) : (new->row = 1);
		} else {
			new->byte = head->byte + 1;
			new->row = head->row;
		}
		new->col = head->col;
		new->bit = head->bit;
		break;
	}
	_delay_ms(50);
	s8 x = GET_BIT(arr[new->row * 16 + new->col][new->byte], new->bit);
	/* Checking if the new head is set on the LCD */
	if (x) {
		/* if the new head position is same as the apple position, then add it to the snake */
		if (new->bit == rando->bit && new->byte == rando->byte
				&& new->col == rando->col && new->row == rando->row) {
			head->prev = rando;
			rando->next = head;
			rando->prev = NULL;
			head = rando;
			SET_BIT(arr[head->row * 16 + head->col][head->byte], head->bit);
			generateRandom();
			while (!trueRandom()) {
				generateRandom();
			}
			SET_BIT(arr[rando->row * 16 + rando->col][rando->byte], rando->bit);
			score++;
			_delay_ms(50);
			drawSnake();
			return;
			/* if the reached position is one of the snake bits, then game is over */
		} else {
			flag = 1;
			return;
		}
		/* if new head not set, set it and remove the tail bit */
	} else {
		head->prev = new;
		new->next = head;
		new->prev = NULL;
		head = new;
		SET_BIT(arr[head->row * 16 + head->col][head->byte], head->bit);
		CLR_BIT(arr[tail->row * 16 + tail->col][tail->byte], tail->bit);
		PosPtr tmp = tail->prev;
		free(tail);
		tail = tmp;
		tail->next = NULL;
	}
	_delay_ms(50);
	drawSnake();
}
/* function that generates the apple position */
void generateRandom() {
	rando = (PosPtr) malloc(sizeof(Pos));
	rando->row = rand() % 2;
	rando->col = rand() % 16;
	rando->byte = rand() % 8;
	rando->bit = rand() % 5;
	rando->prev = NULL;
	rando->next = NULL;
}
/* function that checks if the apple position is set or not
 * Output:
 * 1->apple position not set, 0->apple position is set
 */
u8 trueRandom() {
	return (GET_BIT(arr[rando->row * 16 + rando->col][rando->byte], rando->bit))
			!= 1;
}
/* function that draws the snake cell by cell */
void drawSnake() {
	u8 pos = 0;
	/* removing the old screen output */
	LCD_VidClearLCD();
	for (u8 cell = 0; cell < 32; ++cell) {
		/* checking if the cell is used at least once */
		u8 allZero = 1;
		for (u8 byte = 0; byte < 8; ++byte) {
			if (arr[cell][byte] != 0) {
				allZero = 0;
				break;
			}
		}
		/* drawing the cell if it has at least one set bit */
		if (!allZero) {
			u8 rr = (cell < 16) ? 1 : 2;
			u8 cc = (cell < 16) ? cell + 1 : cell - 15;
			LCD_VidDrawChar(arr[cell], pos, rr, cc);
			pos++;
		}
	}
	_delay_ms(50);
}
