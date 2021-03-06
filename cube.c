#include "cube.h"
#include <math.h>
Cube *cube[LEVEL][LEVEL][LEVEL];

void setColor(Cube *cube, int surfaceIndex, float red, float green,
              float blue) {
  cube->surface[surfaceIndex].red = red;
  cube->surface[surfaceIndex].green = green;
  cube->surface[surfaceIndex].blue = blue;
}

void setCoordinates(Cube *cube, int vertexIndex, double x, double y, double z) {
  cube->coordinate[vertexIndex][0] = x;
  cube->coordinate[vertexIndex][1] = y;
  cube->coordinate[vertexIndex][2] = z;
}

void swap3(int x1, int y1, int z1, int x2, int y2, int z2) {
  Cube temp = *cube[x1][y1][z1];
  *cube[x1][y1][z1] = *cube[x2][y2][z2];
  *cube[x2][y2][z2] = temp;
}

void swap2(int i, int j, int k, int type) {
  switch (type) {
  case 0:
    swap3(i, j, k, LEVEL - 1 - i, j, k);
    break;
  case 1:
    swap3(i, k, j, LEVEL - 1 - i, k, j);
    break;
  default:
    swap3(k, i, j, k, LEVEL - 1 - i, j);
    break;
  }
}

void swap1(int i, int j, int k, int type) {
  switch (type) {
  case 0:
    swap3(i, j, k, j, i, k);
    break;
  case 1:
    swap3(i, k, j, j, k, i);
    break;
  default:
    swap3(k, i, j, k, j, i);
    break;
  }
}

// 除了交换坐标，还要交换在数组中的索引。那为什么不是坐标跟索引绑定，只交换索引呢？
// 交换索引的话需要中间变量先复制相关数据，否则会被覆盖，除了复制一份整个二维数组，还可像这样减少空间复杂度。
void swap0(int i, int j, int k, int type) {
  switch (type) {
  case 0:
    swap3(i, j, k, LEVEL - 1 - j, LEVEL - 1 - i, k);
    break;
  case 1:
    swap3(i, k, j, LEVEL - 1 - j, k, LEVEL - 1 - i);
    break;
  default:
    swap3(k, i, j, k, LEVEL - 1 - j, LEVEL - 1 - i);
    break;
  }
}

static void (*swaps[2])(int i, int j, int k, int type) = {swap0, swap1};
// operation：0表示顺时针，1表示逆时针。
void swap(int k, int type, int operation) {
  for (int i = 0; i < LEVEL; i++) {
    int base = operation * (i + 1);
    for (int j = base; j < base + LEVEL - 1 - i; j++)
      if (onEdge0(i, j, k))
        swaps[operation](i, j, k, type);
  }
  for (int i = 0; i < LEVEL / 2; i++)
    for (int j = 0; j < LEVEL; j++)
      if (onEdge0(i, j, k))
        swap2(i, j, k, type);
}

Cube *getCube(int i, int j, int k, int type) {
  switch (type) {
  case 0:
    return cube[i][j][k];
  case 1:
    return cube[i][k][j];
  default:
    return cube[k][i][j];
  }
}

int onEdge1(int i) { return 0 == i || i == LEVEL - 1; }

int onEdge0(int i, int j, int k) {
  return onEdge1(i) || onEdge1(j) || onEdge1(k);
}

static int coordinate[3][2] = {{0, 1}, {0, 2}, {1, 2}};
// 函数指针真的很方便，Java或者说面向对象的话，当然就是传入一个对象，之形参是一个父类或者说抽象类或者说接口。或者把traverse()放到一个抽象类里，operate()作为其中的一个抽象方法，实例化该抽象类就要先实现该抽象方法，这个在Ippotim项目中用过。当然，由于C语言的结构体中的元素也可以是函数指针，所以实际也可以通过传入结构指针实现与（直接）传入函数指针一样的功能或者说效果。
// 函数指针作为函数参数确实类似或者说就是回调，作为实参的函数（指针）就是作为形参的函数（指针）的实现。
// 形参里直接写void operate(Cube *, int, int, int)也可以，甚至还可以似void
// operate(Cube *cube, int vertexIndex, int x, int
// y)之有参数名，但还是统一写法，毕竟确实是函数指针，而函数指针的定义在其他地方都是类似void
// (*operate)(Cube *, int, int,
// int)的，C语言（的标准）固然很灵活，但我们需要（代码）规范，取C语言（语法）的一个子集即可，不需要面面俱到或者说知道茴字的八种写法。
void traverse(int k, int type, void (*operate)(Cube *, int, int, int)) {
  int x = coordinate[type][0], y = coordinate[type][1];
  for (int i = 0; i < LEVEL; i++)
    for (int j = 0; j < LEVEL; j++) {
      // 魔方内部都是空白方块，就不处理了。表示魔方的三维数组的元素不是Cube而是Cube*的话当然会更好，涉及到结构体都是用指针的好，只是现在这样也是一种解决方案或者说思路，权当收集吧。
      if (onEdge0(i, j, k)) {
        for (int vertexIndex = 0; vertexIndex < 8; vertexIndex++) {
          // 用到结构体最好还是用指针，至少结构体作为函数参数就得传指针。像这里Cube
          // currentCube = getCube(i, j, k,
          // type);是像函数传参那样，是复制了一份，这是与Java不同的。
          // 定义结构变量或结构数组后就已经分配空间或者说内存了，比如Cube
          // cube或Cube cube[6]，不需要malloc或者说new了，这也是与Java不同的。
          Cube *currentCube = getCube(i, j, k, type);
          operate(currentCube, vertexIndex, x, y);
        }
      }
    }
}

void rotate1(Cube *cube, int vertexIndex, int x, int y) {
  cube->coordinate[vertexIndex][x] =
      cube->coordinate[vertexIndex][x] * cos(angle * c) -
      cube->coordinate[vertexIndex][y] * sin(angle * c);
  cube->coordinate[vertexIndex][y] =
      cube->coordinate[vertexIndex][x] * sin(angle * c) +
      cube->coordinate[vertexIndex][y] * cos(angle * c);
}

void rotate0(int k, int type) { traverse(k, type, rotate1); }

void saveCoordinates1(Cube *cube, int vertexIndex, int x, int y) {
  cube->preCoordinate[vertexIndex][x] = cube->coordinate[vertexIndex][x];
  cube->preCoordinate[vertexIndex][y] = cube->coordinate[vertexIndex][y];
}

void saveCoordinates0(int k, int type) { traverse(k, type, saveCoordinates1); }

void reviseCoordinates1(Cube *cube, int vertexIndex, int x, int y) {
  cube->coordinate[vertexIndex][x] =
      -cube->preCoordinate[vertexIndex][y] * angle;
  cube->coordinate[vertexIndex][y] =
      cube->preCoordinate[vertexIndex][x] * angle;
}

// 旋转后调整Cube中coordinate次序，这样就能从其他层中获取无偏差坐标了，但这样时间复杂度似乎略大，故新增preCoordinate。
// 修正坐标很重要，毕竟pi是有误差的，不实现旋转动画可以没有误差之不涉及pi，但旋转动画当然是要的。
void reviseCoordinates0(int k, int type) {
  traverse(k, type, reviseCoordinates1);
}
