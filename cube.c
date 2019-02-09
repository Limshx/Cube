#include "cube.h"
#include <math.h>
// size是组成魔方的小方块的边长，level是魔方阶数。
int size = 6, level = 7;
Cube ***cube;
// angle的值为1或-1,表示每次逆时针或顺时针旋转1度。
int angle;

void setColor(Cube *cube, int surfaceIndex, float red, float green,
              float blue) {
  cube->surface[surfaceIndex].red = red;
  cube->surface[surfaceIndex].green = green;
  cube->surface[surfaceIndex].blue = blue;
}

void setCoordinate(Cube *cube, int vertexIndex, double x, double y, double z) {
  cube->coordinate[vertexIndex][0] = x;
  cube->coordinate[vertexIndex][1] = y;
  cube->coordinate[vertexIndex][2] = z;
}

void swap3(int x1, int y1, int z1, int x2, int y2, int z2) {
  Cube temp = cube[x1][y1][z1];
  cube[x1][y1][z1] = cube[x2][y2][z2];
  cube[x2][y2][z2] = temp;
}

void swap2(int i, int j, int k, int type) {
  switch (type) {
  case 0:
    swap3(i, j, k, level - 1 - i, j, k);
    break;
  case 1:
    swap3(i, k, j, level - 1 - i, k, j);
    break;
  default:
    swap3(k, i, j, k, level - 1 - i, j);
    break;
  }
}

// 除了交换坐标，还要交换在数组中的索引。那为什么不是坐标跟索引绑定，只交换索引呢？
// 交换索引的话需要中间变量先复制相关数据，否则会被覆盖，除了复制一份整个二维数组，还可像这样减少空间复杂度。
void swap1(int i, int j, int k, int type) {
  //  Cube currentCube = getCube(i, j, k, type);
  //  setCube(i, j, k, type, preCube);
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

void swap0(int k, int type) {
  for (int i = 0; i < level; i++)
    for (int j = i + 1; j < level; j++)
      swap1(i, j, k, type);
  for (int i = 0; i < level / 2; i++)
    for (int j = 0; j < level; j++)
      swap2(i, j, k, type);
}

Cube *getCube(int i, int j, int k, int type) {
  switch (type) {
  case 0:
    return &cube[i][j][k];
  case 1:
    return &cube[i][k][j];
  default:
    return &cube[k][i][j];
  }
}

int onEdge(int i) { return 0 == i || i == level - 1; }

static int coordinate[3][2] = {{0, 1}, {0, 2}, {1, 2}};
// 函数指针真的很方便，Java或者说面向对象的话，当然就是传入一个对象，之形参是一个父类或者说抽象类或者说接口。或者把traverse()放到一个抽象类里，operate()作为其中的一个抽象方法，实例化该抽象类就要先实现该抽象方法，这个在Ippotim项目中用过。
void traverse(int k, int type,
              void operate(Cube *cube, int vertexIndex, int x, int y)) {
  int x = coordinate[type][0], y = coordinate[type][1];
  for (int i = 0; i < level; i++)
    for (int j = 0; j < level; j++) {
        // 魔方内部都是空白方块，就不处理了。表示魔方的三维数组的元素不是Cube而是Cube*的话当然会更好，涉及到结构体都是用指针的好，只是现在这样也是一种解决方案或者说思路，权当收集吧。
      if (onEdge(k) || onEdge(i) || onEdge(j)) {
        for (int vertexIndex = 0; vertexIndex < 8; vertexIndex++) {
          // 用到结构体最好还是用指针，至少结构体作为函数参数就得传指针。像这里Cube
          // currentCube = getCube(i, j, k,
          // type);是像函数传参那样，是复制了一份，这是与Java不同的。
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
  cube->coordinate[vertexIndex][x] = -cube->preCoordinate[vertexIndex][y] * angle;
  cube->coordinate[vertexIndex][y] = cube->preCoordinate[vertexIndex][x] * angle;
}

// 旋转后调整Cube中coordinate次序，这样就能从其他层中获取无偏差坐标了，但这样时间复杂度似乎略大，故新增preCoordinate。
// 修正坐标很重要，毕竟pi是有误差的，不实现旋转动画可以没有误差之不涉及pi，但旋转动画当然是要的。
void reviseCoordinates0(int k, int type) {
  traverse(k, type, reviseCoordinates1);
}
