#include<stdio.h>
#include<stdlib.h>
#include<time.h>

extern char getChar(void);

#define MAP_X 60
#define MAP_Y 20
#define ROBOT_NUM 100

#define DEAD 0
#define ALIVE 1

#define INVALID 0
#define ACTIVE 1

#define PLAYER 0
#define ROBOTS 1
#define CRASHED 2
#define SPACE 3

#define GAMEOVER 0

#define rnd(x) (rand() % (x))

typedef struct Robots {
  int x;
  int y;
  int Activation;
  int Alive;
} Robots;

typedef struct Player {
  int x;
  int y;
  int Alive;
} Player;

typedef struct Map {
  int robotNum;
  int Kind; 
} Map;

void MapInit(Map FieldMap[MAP_X][MAP_Y]);
void MapPrint(Map FieldMap[MAP_X][MAP_Y]);
void LevelInit(Player *player, Robots robots[ROBOT_NUM]);
void MapSet(Map FieldMap[MAP_X][MAP_Y], Robots robots[ROBOT_NUM], Player *player, int robotNum);
int Move(Map FieldMap[MAP_X][MAP_Y], Robots robots[ROBOT_NUM], Player *player, int robotNum, int command);
int Mapping(Map FieldMap[MAP_X][MAP_Y], Robots robots[ROBOT_NUM], Player *player, int robotNum, int *score, int level);
void GameOver(int score);
int min(int x, int y);


int main(void) {
  srand(time(NULL));
  struct Robots robots[ROBOT_NUM];
  struct Player player;
  struct Map FieldMap[MAP_X][MAP_Y];

  int command,robotNum;
  int level = 0;
  int score = 0;
  int TotalScore = 0;
  int GameFlag = 1;

  while(1) {
    level++;
    score = 0;
    LevelInit(&player,robots);
    robotNum = min(level*5,40);
    MapSet(FieldMap, robots, &player, robotNum);
    while(score < robotNum) {      
      if(GameFlag == GAMEOVER) {
	TotalScore += score;
	GameOver(TotalScore);
	return 0;
      }
      MapPrint(FieldMap);
      printf("(level:%d score:%d)?",level,(TotalScore+score));
      scanf("%d",&command);
      Move(FieldMap, robots, &player, robotNum, command);
      GameFlag = Mapping(FieldMap, robots, &player, robotNum, &score, level);
    }
    printf("clear!\n");
    TotalScore += level * 10;
    TotalScore += score;
  }
}


void MapInit(Map FieldMap[MAP_X][MAP_Y]) {
  int x,y;

  for(y = 0; y < MAP_Y; y++) {
    for(x = 0; x < MAP_X; x++) {
      FieldMap[x][y].Kind = SPACE;
    }
  }
}

void MapPrint(Map FieldMap[MAP_X][MAP_Y]) {
  int x,y;

  for(y = -1; y <= MAP_Y; y++) {
    
    for(x = -1; x <= MAP_X; x++) {
      
      if(x == -1 || x == MAP_X) {
	if(y == -1 || y == MAP_Y) printf("+");
	else printf("|");
      } else if(y == -1 || y == MAP_Y) {
	printf("-");
      } else {
	switch(FieldMap[x][y].Kind) {
	case PLAYER:
	  printf("@"); break;
	case ROBOTS:
	  printf("+"); break;
	case CRASHED:
	  printf("*"); break;
	case SPACE:
	  printf(" "); break;
	}
      }
    }
    printf("\n");
  }
}


void LevelInit(Player *player, Robots robots[ROBOT_NUM]) {
  player->Alive = DEAD;
  for(int i = 0; i < ROBOT_NUM; i++) {
    robots[i].Activation = INVALID;
    robots[i].Alive = DEAD;
  }
}

void MapSet(Map FieldMap[MAP_X][MAP_Y], Robots robot[ROBOT_NUM], Player *player, int robotNum) {
  MapInit(FieldMap);
  int x,y,i;
  x = rnd(MAP_X);
  y = rnd(MAP_Y);
  player->x = x;
  player->y = y;
  player->Alive = ALIVE;
  FieldMap[x][y].Kind = PLAYER;
  
  i = 0;
  while(i < robotNum) {
    x = rnd(MAP_X);
    y = rnd(MAP_Y);
    if(FieldMap[x][y].Kind == SPACE) {
      robot[i].x = x;
      robot[i].y = y;
      robot[i].Activation = ACTIVE;
      robot[i].Alive = ALIVE;
      FieldMap[x][y].Kind = ROBOTS;
      FieldMap[x][y].robotNum = i;
      i++;      
    }
  }
}      
      
int Move(Map FieldMap[MAP_X][MAP_Y], Robots robots[ROBOT_NUM], Player *player, int robotNum, int command) {
  int x,y;
  int flag = 0;

  if(command == 0) {
    while(1) {
      x = rnd(MAP_X);
      y = rnd(MAP_Y);
      if(FieldMap[x][y].Kind == SPACE) {
	player->x = x;
	player->y = y;
	break;
      }
    }
  } else {
    x = y = 0;
    switch(command) {
    case 1: x--; y++; break;
    case 2: y++; break;
    case 3: x++; y++; break;
      
    case 4: x--; break;
    case 5: break;
    case 6: x++; break;
      
    case 7: x--; y--; break;
    case 8: y--; break;
    case 9: x++; y--; break;
    }
    
    player->x += x;
    player->y += y;
  }
  
  if(player->x == MAP_X) {
    player->x--;
    flag = 1;
  }
  if(player->x == -1) {
    player->x++;
    flag = 1;
  }
  if(player->y == MAP_Y) {
    player->y--;
    flag = 1;
  }
  if(player->y == -1) {
    player->y++;
    flag = 1;
  }
  if(flag == 1) return 1;

  for(int i = 0; i < robotNum; i++) {
    if(robots[i].Activation == ACTIVE && robots[i].Alive == ALIVE) {
      if(robots[i].x < player->x) robots[i].x++;
      if(robots[i].x > player->x) robots[i].x--;
      if(robots[i].y < player->y) robots[i].y++;
      if(robots[i].y > player->y) robots[i].y--;
    }
  }
}
    

int Mapping(Map FieldMap[MAP_X][MAP_Y], Robots robots[ROBOT_NUM], Player *player, int robotNum, int *score, int level) {
  MapInit(FieldMap);

  FieldMap[player->x][player->y].Kind = PLAYER;
  for(int i = 0; i < robotNum; i++) {
    if(robots[i].Activation == ACTIVE && robots[i].Alive == ALIVE) {
      switch(FieldMap[robots[i].x][robots[i].y].Kind) {
      case PLAYER:
	player->Alive = DEAD;
	return 0;
	break;
      case ROBOTS:	  
	FieldMap[robots[i].x][robots[i].y].Kind = CRASHED;
	robots[i].Alive = DEAD;
	robots[FieldMap[robots[i].x][robots[i].y].robotNum].Alive = DEAD;
	(*score) += 2;
	break;
      case CRASHED:
	FieldMap[robots[i].x][robots[i].y].Kind = CRASHED;
	FieldMap[robots[i].x][robots[i].y].robotNum = i;
	robots[i].Alive = DEAD;
	(*score)++;
	break;
      case SPACE:
	FieldMap[robots[i].x][robots[i].y].Kind = ROBOTS;
	FieldMap[robots[i].x][robots[i].y].robotNum = i;
	break;
      }

    } else if(robots[i].Activation == ACTIVE && robots[i].Alive == DEAD) {
      switch(FieldMap[robots[i].x][robots[i].y].Kind) {
      case PLAYER:
	player->Alive = DEAD;
	return 0;
	break;
      case ROBOTS:	  
	FieldMap[robots[i].x][robots[i].y].Kind = CRASHED;
	robots[FieldMap[robots[i].x][robots[i].y].robotNum].Alive = DEAD;
	(*score)++;
	break;
      case CRASHED:
	FieldMap[robots[i].x][robots[i].y].Kind = CRASHED;
	FieldMap[robots[i].x][robots[i].y].robotNum = i;
	break;
      case SPACE:
	FieldMap[robots[i].x][robots[i].y].Kind = CRASHED;
	FieldMap[robots[i].x][robots[i].y].robotNum = i;
	break;
      }
    }
  }
  return 1;
}
   
void GameOver(int score) {
  printf("Game Over!\nScore:%d\n",score);
}

int min(int x, int y) {
  if(x < y) {
    return x;
  } else {
    return y;
  }
}
