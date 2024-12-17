#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NORTH 1
#define EAST 2
#define SOUTH 4
#define WEST 8
#define ENTRANCE 16
#define EXIT 32
#define VERTICAL 1 
#define VISITE 64
#define PATH 128

#define CHK(op)                                                                \
  do {                                                                         \
    if ((op) == -1)                                                            \
      raler(#op);                                                              \
  } while (0)

#define CHKM(op)                                                               \
  do {                                                                         \
    if ((op) == NULL)                                                          \
      raler("error malloc");                                                   \
  } while (0)
void raler(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}
typedef struct st_tree {
  struct st_tree *father;
  int x;
  int y;
} nod_t, *tree_t;

typedef struct st_queue_node {
  struct st_queue_node *next;
  tree_t data;
} queue_node_t;

typedef struct st_queue {
  queue_node_t *head;
  queue_node_t *tail;
} queue_t;

typedef struct st_maze {
  int **matrix;
  int height;
  int width;
  tree_t **mat_expl;
} *maze;

typedef struct st_point {
  int x;
  int y;
} point;
maze init(int height, int width) {
  maze m = malloc(sizeof(struct st_maze));
  CHKM(m);
  m->height = height;
  m->width = width;
  m->matrix = malloc(sizeof(int *) * height);
  m->mat_expl = malloc(sizeof(tree_t *) * height);
  CHKM(m->matrix);
  CHKM(m->mat_expl);

  for (int i = 0; i < height; i++) {
    m->matrix[i] = malloc(sizeof(int) * width);
    m->mat_expl[i] = malloc(sizeof(tree_t) * width);
    CHKM(m->matrix[i]);
    CHKM(m->mat_expl[i]);
    for (int j = 0; j < width; j++) {
      m->matrix[i][j] = 0;
      m->mat_expl[i][j] = NULL;
    }
  }
  m->matrix[height - 1][width - 1] += EXIT;
  return m;
}
void maze_free(maze m) {
  for (int i = 0; i < m->height; i++) {
    free(m->matrix[i]);
    for (int j = 0; j < m->width; j++) {
      if (m->mat_expl[i][j] != NULL)
        free(m->mat_expl[i][j]);
    }
    free(m->mat_expl[i]);
  }
  free(m->matrix);
  free(m->mat_expl);
  
  free(m);
}
void maze_print(maze m) {
  for (int i = 0; i < m->height; i++) {
    for (int j = 0; j < m->width; j++) {
      printf("%d\t", m->matrix[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}
void maze_show(maze m) {
  for (int i = 0; i < m->height; i++) {
    for (int j = 0; j < m->width; j++) {
      printf("+");
      if (m->matrix[i][j] & NORTH) {
        printf("   ");
      } else {
        printf("---");
      }
    }
    printf("+\n");

    for (int j = 0; j < m->width; j++) {
      if (m->matrix[i][j] & WEST) {
        printf(" ");
      } else {
        printf("|");
      }
      // print the cells
      if (m->matrix[i][j] & ENTRANCE) {
        printf(" X ");
      } else if (m->matrix[i][j] & EXIT) {
        printf(" O ");
      } else if (m->matrix[i][j] & PATH) {
        printf(" * ");
      } else {
        printf("   ");
      }
    }
    printf("|\n");
  }

  for (int j = 0; j < m->width; j++) {
    printf("+");
    if (m->matrix[m->height - 1][j] & SOUTH) {
      printf("   ");
    } else {
      printf("---");
    }
  }
  printf("+\n");
}
void open_wall(maze m, int x, int y,
               int orient) // orient is 0 if we open de south wall and 1 if it's
                           // the east wall
{
  if ((x < m->width - 1 || x == 0) && (y < m->height - 1 || y == 0)) {
    m->matrix[y][x] += orient == VERTICAL ? EAST : SOUTH;
    m->matrix[y + (1 - orient)][x + orient] +=
        orient == VERTICAL ? WEST : NORTH;
  }
}
void fusion(maze res, maze m1, maze m2, int orient) {
  if (res->width > res->height && orient != VERTICAL) {
    printf("error fusion 1\n");
    exit(1);
  }
  if (res->width < res->height && orient == VERTICAL) {
    printf("error fusion 2 \n");
    exit(1);
  }
  for (int i = 0; i < m1->height; i++) {
    for (int j = 0; j < m1->width; j++) {
      res->matrix[i][j] = m1->matrix[i][j];
    }
  }
  for (int i = 0; i < m2->height; i++) {
    for (int j = 0; j < m2->width; j++) {
      res->matrix[i + m1->height * (1 - orient)][j + m1->width * orient] =
          m2->matrix[i][j];
    }
  }
}
int orient_nb = 0;
int orient_sum = 0;
maze divide(maze m) {
  if (m->height > 1 || m->width > 1) {
    int x = 0;
    int y = 0;
    if (m->width > 1) {
      x = rand() % (m->width - 1);
    }
    if (m->height > 1) {
      y = rand() % (m->height - 1);
    }
    maze m1;
    maze m2;
    int orient;
    if (m->width == m->height) {
      orient = rand() % 2;
    } else if (m->width > m->height) {
      orient = VERTICAL;
    } else {
      orient = 1 - VERTICAL;
    }
    orient_sum += orient;
    orient_nb++;
    if (orient == VERTICAL) {
      open_wall(m, x, y, VERTICAL);
      m1 = init(m->height, x + 1);
      m2 = init(m->height, m->width - (x + 1));
    } else {
      open_wall(m, x, y, 1 - VERTICAL);
      m1 = init(y + 1, m->width);
      m2 = init(m->height - (y + 1), m->width);
    }
    // feel m1:
    for (int i = 0; i < m1->height; i++) {
      for (int j = 0; j < m1->width; j++) {
        m1->matrix[i][j] = m->matrix[i][j];
      }
    }
    // feel m2:
    for (int i = 0; i < m2->height; i++) {
      for (int j = 0; j < m2->width; j++) {
        m2->matrix[i][j] =
            m->matrix[i + (y + 1) * (1 - orient)][j + (x + 1) * orient];
      }
    }
    divide(m1);
    divide(m2);
    fusion(m, m1, m2, orient);
    maze_free(m1);
    maze_free(m2);
  }
  return m;
}
int is_entrance(maze m, int x, int y) {
  if (x < 0 || y < 0 || x >= m->width || y >= m->height)
    return 0;
  return (m->matrix[x][y] & ENTRANCE) == ENTRANCE;
}

tree_t tree_init(tree_t father, int x, int y) {
  tree_t res = malloc(sizeof(nod_t));
  CHKM(res);
  res->father = father;
  res->x = x;
  res->y = y;
  return res;
}

void enqueue(queue_t *q, tree_t t) {
  queue_node_t *new = malloc(sizeof(queue_node_t));
  CHKM(new);
  new->next = NULL;
  new->data = t;
  if (q->head == NULL) {
    q->head = new;
  } else {
    q->tail->next = new;
  }
  q->tail = new;
}
tree_t dequeue(queue_t *q) {
  if (q->head == NULL)
    return NULL;
  tree_t res = q->head->data;
  queue_node_t* trash=q->head;
  q->head = q->head->next;
  free(trash);
  return res;
}
int is_empty_queue(queue_t q) { return q.head == NULL; }

void queue_show(queue_t q) {
  queue_node_t *current = q.head;
  printf("queue: ");
  while (current != NULL) {
    printf("(%d,%d) -> \t", current->data->x, current->data->y);
    current = current->next;
  }
  printf("null\n");
}
int is_not_leaf(int x, int y, maze m) {
  int res = 0;
  if (m->matrix[y][x] & NORTH && y < m->height &&
      (m->matrix[y - 1][x] & VISITE) == 0)
    res += NORTH;
  if (m->matrix[y][x] & EAST && x < m->width &&
      (m->matrix[y][x + 1] & VISITE) == 0)
    res += EAST;
  if (m->matrix[y][x] & SOUTH && y >= 0 && (m->matrix[y + 1][x] & VISITE) == 0)
    res += SOUTH;
  if (m->matrix[y][x] & WEST && x >= 0 && (m->matrix[y][x - 1] & VISITE) == 0)
    res += WEST;
  m->matrix[y][x] += VISITE;
  // printf("if_not_leaf(%d,%d): %d\n", x, y, res);

  return res;
}
void tree_gen(maze m) {
  tree_t t = tree_init(NULL, m->width - 1, m->height - 1);
  m->mat_expl[m->height - 1][m->width - 1] = t;
  queue_t q = {0};
  enqueue(&q, t);
  while (!is_empty_queue(q)) {
    tree_t father = dequeue(&q);
    int x = father->x;
    int y = father->y;
    int n = is_not_leaf(x, y, m);
    if (n & NORTH) {
      m->mat_expl[y - 1][x] = tree_init(father, x, y - 1);
      enqueue(&q, m->mat_expl[y - 1][x]);
    }
    if (n & EAST) {
      m->mat_expl[y][x + 1] = tree_init(father, x + 1, y);
      enqueue(&q, m->mat_expl[y][x + 1]);
    }
    if (n & SOUTH) {
      m->mat_expl[y + 1][x] = tree_init(father, x, y + 1);
      enqueue(&q, m->mat_expl[y + 1][x]);
    }
    if (n & WEST) {
      m->mat_expl[y][x - 1] = tree_init(father, x - 1, y);
      enqueue(&q, m->mat_expl[y][x - 1]);
    }
  }
}
void tree_show(tree_t t) {
  while (t != NULL) {
    printf("(%d,%d) ->  ", t->x, t->y);
    t = t->father;
  }
  printf("null\n");
}
maze maze_gen(int height, int width) {
  maze m = init(height, width);
  m = divide(m);
  tree_gen(m);
  return m;
}

void path_to_exit_gen(maze m, int x, int y) {
  m->matrix[y][x] += ENTRANCE;
  tree_t t = m->mat_expl[y][x];
  printf("path to exit: ");
  while (t != NULL) {
    m->matrix[t->y][t->x] += PATH;
    printf("(%d,%d) ", t->x, t->y);
    t = t->father;
  }
  printf("\n");
}

void path_to_exit_show(maze m, int x, int y) {
  tree_t t = m->mat_expl[y][x];
  while (t != NULL) {
    printf("(%d,%d) ->  ", t->x, t->y);
    t = t->father;
  }
  printf("null\n");
}

#ifdef MAIN_PROGRAM
int main(int argc, char *argv[]) {
  if (argc < 3 || argc > 4) {
    raler("./maze height width [-show]");
  }
  int height = atoi(argv[1]);
  int width = atoi(argv[2]);
  int show = 0;
  if (argc == 4) {
    if (argv[3][0] != '-' || argv[3][1] != 's' || argv[3][2] != 'h' ||
        argv[3][3] != 'o' || argv[3][4] != 'w') {
      raler("./maze height width [-show]");
    } else
      show = 1;
  }
  if (width <= 0 || height <= 0) {
    raler("./maze height width \n width and heigh can't be negative or null");
  }
  srand(time(NULL));
  maze m = maze_gen(height, width);
  if (height != 1 && width != 1)
    path_to_exit_gen(m, rand() % (m->width - 1), rand() % (m->height - 1));
  else
    path_to_exit_gen(m, 0, 0);

  maze_show(m);
  maze_free(m);
  return 0;
}

#endif