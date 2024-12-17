#include "maze.c"
#include <string.h> 

int test_tree_gen_integrity(maze m, int mes) {
  int res = 1;
  if (m->mat_expl == NULL) {
    if (mes)
      printf("error mat_expl is null\n");
    res = 0;
  }
  for (int i = 0; i < m->height; i++) {
    if (m->mat_expl[i] == NULL) {
      if (mes)
        printf("error mat_expl line:%d is null\n", i);
      res = 0;
    }
  }
  if (m->mat_expl[m->height - 1][m->width - 1]->father != NULL) {
    if (mes)
      printf("the exit have a father\n");
    res = 0;
  }

  for (int y = 0; y < m->height; y++) {
    for (int x = 0; x < m->width; x++) {
      tree_t t = m->mat_expl[y][x];
      if (t == NULL) {
        res = 0;
        if (mes)
          printf("error mat_expl %d %d is null\n", x, y);
      } else {
        while (t->father != NULL) {
          t = t->father;
        }
        if (t->x != m->width - 1 && t->y != m->height - 1) {
          res = 0;
          if (mes)
            printf("error tree %d %d is not connected to the exit\n", t->x,
                   t->y);
        }
      }
    }
  }
  return res;
}
int test_tree() {

  int res = 1;
  maze m1 = maze_gen(1, 1);

  if (!test_tree_gen_integrity(m1, 1)) {
    res = 0;
    printf("unique cells maze have an uncorrect tree\n");
  }
  free(m1);
  maze m2 = maze_gen(2, 2);
  if (!test_tree_gen_integrity(m2, 1)) {
    res = 0;
    printf("2x2 maze have an uncorrect tree\n");
  }

  free(m2);
  maze m3 = maze_gen(10, 10);
  if (!test_tree_gen_integrity(m3, 1)) {
    res = 0;
    printf("10x10 maze have an uncorrect tree\n");
  }
  free(m3);
  
  maze m0 = maze_gen(10, 10);
  m0->mat_expl[5][5] = NULL;
  if (test_tree_gen_integrity(m0, 0)) {
    res = 0;
    printf("uncorrect maze have a correct tree\n");
  }
  printf("ok\n");
  path_to_exit_gen(m0, rand() % (m0->width - 1), rand() % (m0->height - 1));
  maze_show(m0);
  free(m0);

  return res;
}
int test_queue() {
  int res = 1;
  maze m3 = maze_gen(10, 10);
  path_to_exit_gen(m3, rand() % (m3->width - 1), rand() % (m3->height - 1));

  queue_t q = {0};
  if (!is_empty_queue(q)) {
    res = 0;
    printf("error empty queue is not empty\n");
  }
  tree_t t = tree_init(NULL, 0, 0);
  enqueue(&q, t);
  if (is_empty_queue(q)) {
    res = 0;
    printf("error not empty queue is empty\n");
  }
  if (q.head != q.tail) {
    res = 0;
    printf("error head and tail are not the same for a one element queue\n");
  }
  if (dequeue(&q)->x != t->x) {
    res = 0;
    printf("error dequeue value is not the good one\n");
  }
  if (!is_empty_queue(q)) {
    res = 0;
    printf("error empty queue is not empty\n");
  }
  enqueue(&q, t);
  tree_t t1 = tree_init(NULL, 1, 0);
  enqueue(&q, t1);
  tree_t t2 = tree_init(NULL, 2, 0);
  enqueue(&q, t2);
  tree_t t3 = tree_init(NULL, 3, 0);
  enqueue(&q, t3);
  if (is_empty_queue(q)) {
    res = 0;
    printf("error not empty queue is empty\n");
  }
  if (q.head == q.tail) {
    res = 0;
    printf("error head and tail are the same for a multiple element queue\n");
  }
  if (dequeue(&q)->x != t->x) {
    res = 0;
    printf("error dequeue value is not the good one\n");
  }
  if (dequeue(&q)->x != t1->x) {
    res = 0;
    printf("error dequeue value is not the good one\n");
  }
  if (dequeue(&q)->x != t2->x) {
    res = 0;
    printf("error dequeue value is not the good one\n");
  }
  if (dequeue(&q)->x != t3->x) {
    res = 0;
    printf("error dequeue value is not the good one\n");
  }
  if (dequeue(&q) != NULL) {
    res = 0;
    printf("error dequeue value is not NULL for an empty queue\n");
  }
  return res;
}
int test_value() {
  maze m = maze_gen(10, 10);
  path_to_exit_gen(m, rand() % (m->width - 1), rand() % (m->height - 1));
  for (int i = 0; i < m->height; i++) {
    for (int j = 0; j < m->width; j++) {
      if (m->matrix[i][j] < 0 || m->matrix[i][j] >= (2 * PATH))
        return 0;
    }
  }
  return 1;
}
int test_border() {
  maze m = maze_gen(-1, -1);
  for (int i = 0; i < m->width; i++) {
    if (m->matrix[0][i] & NORTH) {
      printf("error wall at (0, %d) NORTH is open\n", i);
      return 0;
    }
    if (m->matrix[m->height - 1][i] & SOUTH) {
      printf("error wall at (%d, %d) SOUTH is open\n", m->height - 1, i);
      return 0;
    }
  }
  for (int i = 0; i < m->height; i++) {
    if (m->matrix[i][0] & WEST) {
      printf("error wall at (%d, 0) WEST is open\n", i);
      return 0;
    }
    if (m->matrix[i][m->width - 1] & EAST) {
      printf("error wall at (%d, %d) EAST is open\n", i, m->width - 1);
      return 0;
    }
  }
  path_to_exit_gen(m, rand() % (m->width - 1), rand() % (m->height - 1));
  return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 2) 
    {
        printf("No test specified\n");
        return 1;
    }
  	srand(time(NULL));
    int res;
    if (strcmp(argv[1], "value") == 0) {
        res=test_value();
    } 
    else if (strcmp(argv[3], "border") == 0) {
        res=test_border();
    }
    else if (strcmp(argv[1], "tree") == 0) {
        res=test_tree();
    } 
    else if (strcmp(argv[1], "queue") == 0) {
        return test_queue(); 
    } 
      else {
        printf("Unknown test: %s\n", argv[1]);
        return 1;
    }
    return res;
}