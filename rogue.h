#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>


typedef struct tile_store {
    int xcor;
    int ycor;
    struct item_store *anitem;
    struct monster_store *mons;
    char*color;
    char*colorbackground;
    char form;
    struct room_store *aroom;
} *tile,tile_store;

typedef struct item_store {
    char * name;
    char form;
    int exp;
    int atk;
    int def;
    int maxhp;
    int curhp;
    int luck;
} item_store, *item;

typedef struct room_store {
    int xsize;
    int ysize;
    int colw;
    int cold;
    int coldoor;
    int colladder;
    int colitems;
    tile *tilearray;
    int nx,ny,ex,ey,wx,wy,sx,sy;
    struct room_store *north;
    struct room_store *east;
    struct room_store *west;
    struct room_store *south;
    int downroom;
    int finalroom;
    int rainbowroom;
} *room,room_store;

typedef struct move_store {
  char *name;
  char *type;
  int tohit; //0-100
  int power;
} move_store, *amove;

typedef struct hero_store {
  int level;
  int maxhp;
  int curhp;
  int atk;
  int def;
  int luck;
  int exp;
} hero_store, *hero;

typedef struct move_cons_store {
  amove contents;
  struct move_cons_store *next;
} move_cons_store, *move_cons;

typedef struct moves_store {
  struct move_cons_store* front;
} moves_store, *moves;

typedef struct pokemonT_store {
  char *name;
  char *type;
  char **movelist;
  char form;
  int baseatk;
  int basedef;
  int basehp;
  int basexp;
} pokemonT_store, *pokemonT;

typedef struct pokemon_cons_store {
  pokemonT contents;
  struct pokemon_cons_store *next;
} pokemon_cons_store, *pokemon_cons;

typedef struct pokemons_store {
  pokemon_cons front;
}pokemons_store, *pokemons;

typedef struct monster_store {
  pokemonT poketype;
  int maxhp;
  int curhp;
  int atk;
  int def; //strictly positive
  int xp;
  int level;
} monster_store, *monster;

typedef struct listcons_store {
  char *contents;
  struct listcons_store*next;
} listcons_store, *listcons;

typedef struct list_store {
  listcons front;
} list_store, *list;
void add_pokemon(pokemonT newpoke, pokemons result);
int laddercheck(WINDOW*topwin,WINDOW*botwin);
room init_room(int width,int height,int colw,int cold,int colladder,int coldoor,int colitems);
void display_room(WINDOW *win,int startx,int starty,room aroom);
WINDOW*create_newwin(int height, int width,int starty,int startx);
void destroy_win(WINDOW *local_win);
room get_next(room aroom,int i);
int room_match(room rooma,room roomb);
int mover(char ch,WINDOW*pwin,WINDOW*dwin,room rm,int side,int*x,int*y);
int create_portals(room ar);
void create_monsters(room rm,int flr,char*pokemon,pokemons pokelist);
room map_initialize(int flr,pokemons pokelist);
tile get_tile(WINDOW* w, room rm, int x,int y);
item init_item(int flr);
void create_items(room rm,int flr);
tile get_local(room rm, int x, int y);

int is_move(char *move, list movelist);
listcons new_listcons(char*contents, listcons next);
list new_list();
void addtolist(char*thing,list stuff);
amove new_move(char *name, char *type, int tohit, int power);

void free_move(amove m);

amove find_move(char *movename, moves movelist);

move_cons new_move_cons(amove m, move_cons next);

void free_move_cons(move_cons m);

moves new_moves();

void add_move(amove m, moves movelist);
  

int num_moves(moves m);

void free_moves(moves m);

moves read_moves(FILE *fp);

pokemonT new_pokemonT(char *name, char form, char *type, char **movelist, int baseatk, int basedef, int basehp);

void free_pokemon(pokemonT m);

pokemon_cons new_pokemon_cons(pokemonT m, pokemon_cons next);

void free_pokemon_cons(pokemon_cons m);

pokemons new_pokemons();

pokemons read_pokemons(FILE *fp);

pokemonT find_pokemon(char *poke, pokemons pokelist);

void free_pokemons(pokemons m);

monster init_monster(int level, char *poketype, pokemons pokelist);

void freemonster(monster m);

int monster_attack(amove m, monster p, hero h);

int player_attack(amove m, monster p, hero h);

//Graphics

int battle(WINDOW*westwin, monster p, hero h, int movefirst, list yourmovelist, moves movelist,WINDOW*win);

int leng(char** array);

hero new_hero(int level, int maxhp, int curhp, int atk, int def);

void *free_hero(hero h);

char *accesspokename(int i,pokemons pokelist);

