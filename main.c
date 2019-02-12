#include "cube.h"
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

double c = 3.14159 / 180; //弧度和角度转换参数
// angle的值为1或-1,表示每次逆时针或顺时针旋转1度。
int angle;
static double r = 200; // r是视点到原点距离
static int alpha, beta; // alpha是视点绕y轴的角度, beta是视点高度即在y轴上的坐标
static int preX, preY;
static int size = 6;

void initCube1(Cube *cube, int i, int j, int k) {
  double x, y, z;
  double offset = (LEVEL - 1) * (size + 0.5);
  x = i * (2 * size + 1) - offset;
  y = j * (2 * size + 1) - offset;
  z = k * (2 * size + 1) - offset;
  setCoordinates(cube, 0, size + x, size + y, -size + z);
  setCoordinates(cube, 1, size + x, size + y, size + z);
  setCoordinates(cube, 2, size + x, -size + y, size + z);
  setCoordinates(cube, 3, size + x, -size + y, -size + z);
  setCoordinates(cube, 4, -size + x, size + y, size + z);
  setCoordinates(cube, 5, -size + x, size + y, -size + z);
  setCoordinates(cube, 6, -size + x, -size + y, -size + z);
  setCoordinates(cube, 7, -size + x, -size + y, size + z);
  // 红
  if (i != LEVEL - 1) {
    setColor(cube, 0, 1.0, 1.0, 1.0);
  } else {
    setColor(cube, 0, 1.0, 0.0, 0.0);
  }
  // 品红
  if (i != 0) {
    setColor(cube, 1, 1.0, 1.0, 1.0);
  } else {
    setColor(cube, 1, 1.0, 0.0, 1.0);
  }
  // 黑
  if (j != LEVEL - 1) {
    setColor(cube, 2, 1.0, 1.0, 1.0);
  } else {
    setColor(cube, 2, 0.0, 0.0, 0.0);
  }
  // 黄
  if (j != 0) {
    setColor(cube, 3, 1.0, 1.0, 1.0);
  } else {
    setColor(cube, 3, 1.0, 1.0, 0.0);
  }
  // 绿
  if (k != LEVEL - 1) {
    setColor(cube, 4, 1.0, 1.0, 1.0);
  } else {
    setColor(cube, 4, 0.0, 1.0, 0.0);
  }
  // 蓝
  if (k != 0) {
    setColor(cube, 5, 1.0, 1.0, 1.0);
  } else {
    setColor(cube, 5, 0.0, 0.0, 1.0);
  }
}

void initCube0() {
  int i, j, k;
  for (i = 0; i < LEVEL; i++)
    for (j = 0; j < LEVEL; j++)
      for (k = 0; k < LEVEL; k++)
        if (onEdge0(i, j, k)) {
          cube[i][j][k] = (Cube *)malloc(sizeof(Cube));
          initCube1(cube[i][j][k], i, j, k);
        }
}

// 如果可以根据序号或者说索引得到顶点序列当然最好或者说最优雅，不行的话像这样枚举也无妨，毕竟并没有复杂或者说麻烦多少，而且也与阶数无关。
// 当然这样也算是高明的，不设置vertex[6][4]的话就是在代码里手写或者说枚举了，就是写6段结构一模一样的代码，这就蛋疼了。
static int vertex[6][4] = {{0, 1, 2, 3}, {4, 5, 6, 7}, {0, 5, 4, 1},
                           {2, 7, 6, 3}, {1, 4, 7, 2}, {3, 6, 5, 0}};
void drawCube1(Cube *cube) {
  int i, j;
  for (i = 0; i < 6; i++) {
    Surface surface = cube->surface[i];
    glColor3f(surface.red, surface.green, surface.blue);
    for (j = 0; j < 4; j++)
      glVertex3d(cube->coordinate[vertex[i][j]][0],
                 cube->coordinate[vertex[i][j]][1],
                 cube->coordinate[vertex[i][j]][2]);
  }
}

void drawCube0() {
  glBegin(GL_QUADS);
  int i, j, k;
  for (i = 0; i < LEVEL; i++)
    for (j = 0; j < LEVEL; j++)
      for (k = 0; k < LEVEL; k++)
        if (onEdge0(i, j, k))
          drawCube1(cube[i][j][k]);
  glEnd();
}

void draw(void) {
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  // 过原点的射线，知其xy平面上与y轴正方向的夹角du、与xy平面的夹角h，求其与球心在原点、半径为r的球面的交点坐标。
  double scale = cos(c * beta);
  double eyeX, eyeY, eyeZ;
  eyeX = r * cos(c * alpha) * scale;
  eyeY = r * sin(c * beta);
  eyeZ = r * sin(c * alpha) * scale;
  double upX, upY, upZ;
  double factor = r * r / (eyeX * eyeX + eyeY * eyeY);
  upX = eyeX - eyeX * factor;
  upY = eyeY;
  upZ = eyeZ - eyeZ * factor;
  // 本来是该根据alpha和beta求过视点的圆的逆时针切向量的，这样当然也是一种方法，也不算繁琐。
  if (0 == beta) {
    upY = 1;
  }
  if (180 == beta) {
    upY = -1;
  }
  if ((90 < beta && beta < 180) || (270 < beta && beta < 360)) {
    upX = -upX;
    upY = -upY;
    upZ = -upZ;
  }
  gluLookAt(eyeX, eyeY, eyeZ, 0, 0, 0, upX, upY, upZ);
  drawCube0();
  glFlush();
  glutSwapBuffers();
}

static int start, rotateAngle, layer, type;
#define VOLUME 32
// 不用数组而是链表当然更好，但其实也够用了，日后如果觉得有必要再做打算。
static int list[VOLUME][3], index, recover;

void readList() {
  layer = list[index][0];
  type = list[index][1];
  angle = list[index][2];
  saveCoordinates0(layer, type);
  index -= 1;
  if (-1 == index) {
    recover = 0;
  }
}

void onRotate(int value) {
  if (start) {
    if (rotateAngle < 90) {
      rotateAngle += 1;
      rotate0(layer, type);
    } else {
      rotateAngle = 0;
      reviseCoordinates0(layer, type);
      // 顺时针旋转就是逆时针旋转3次，这样就不用新开一个函数了，所谓精简指令集。
      // 但这样要牺牲时间复杂度，权衡再三还是增加顺时针处理函数，之代码复杂度换时间复杂度。
      swap(layer, type, angle > 0);
      if (recover) {
        readList();
      } else {
        if (-1 == index) {
          index = 0;
          start = 0;
        } else {
          list[index][0] = layer;
          list[index][1] = type;
          list[index][2] = -angle;
          if (VOLUME - 1 == index) {
            recover = 1;
            readList();
          } else {
            index += 1;
            start = 0;
          }
        }
      }
    }
    glutPostRedisplay();
  }
  glutTimerFunc(1, onRotate, value);
}

static int currentButton; // 0是按下了左键，1是按下了右键
void mouseFunc(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    currentButton = 0;
    preX = x;
    preY = y;
  }
  if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
    currentButton = 1;
    if (!start) {
      start = 1;
      layer = rand() % LEVEL;
      type = rand() % 3;
      // 允许顺时针旋转是必要的，不然就要转3次，这是很频繁的开销，所谓精简指令集也要顾及性能，当然理论模型也是必要的。
      angle = (rand() % 2) ? 1 : -1;
      saveCoordinates0(layer, type);
    }
  }
}

int fixedAngle(int angle) {
  if (angle < 0) {
    angle += 360;
  }
  if (angle >= 360) {
    angle -= 360;
  }
  return angle;
}

void motionFunc(int x, int y) {
  // 左键拖动才旋转视角
  if (0 == currentButton) {
    alpha += x - preX;
    beta += y - preY;
    alpha = fixedAngle(alpha);
    beta = fixedAngle(beta);
    preX = x;
    preY = y;
    glutPostRedisplay();
  }
}

void keyboardFunc(unsigned char key, int x, int y) {
  printf("%d %d\n", x, y);
  if (!start && 0 != index)
    if ('r' == key) {
      index -= 1;
      recover = 1;
      start = 1;
      readList();
    }
}

static int fixedWindowSize = 888;

void reshape(int w, int h) {
  // 锁纵横比，不然改变窗口大小图形会变形或者说拉伸。
  w = h = fixedWindowSize;
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(75, w / h, 1.0, 1000.0);
  glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char *argv[]) {
  srand((unsigned)time(NULL));
  initCube0();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_MULTISAMPLE);
  //  glutInitWindowPosition(100, 100);
  glutInitWindowSize(fixedWindowSize, fixedWindowSize);
  glutCreateWindow("OpenGL");
  glEnable(GL_DEPTH_TEST);
  glutReshapeFunc(reshape);
  glutDisplayFunc(draw);
  //  glutIdleFunc(draw);
  glutTimerFunc(1, onRotate, 0);
  glutMouseFunc(mouseFunc);
  glutMotionFunc(motionFunc);
  glutKeyboardFunc(keyboardFunc);
  glutMainLoop();
  return 0;
}
