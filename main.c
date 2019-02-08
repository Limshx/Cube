#include "cube.h"
#include <GL/glut.h>
#include <math.h>
#include <time.h>
static double c = 3.14159 / 180; //弧度和角度转换参数
static double r = 200;           // r是视点绕y轴的半径
static int alpha, beta; // alpha是视点绕y轴的角度, beta是视点高度即在y轴上的坐标
static int preX, preY;

void initCube1(Cube *cube, int i, int j, int k) {
  double x, y, z;
  double offset = (level - 1) * (size + 0.5);
  x = i * (2 * size + 1) - offset;
  y = j * (2 * size + 1) - offset;
  z = k * (2 * size + 1) - offset;
  setCoordinate(cube, 0, size + x, size + y, -size + z);
  setCoordinate(cube, 1, size + x, size + y, size + z);
  setCoordinate(cube, 2, size + x, -size + y, size + z);
  setCoordinate(cube, 3, size + x, -size + y, -size + z);
  setCoordinate(cube, 4, -size + x, size + y, size + z);
  setCoordinate(cube, 5, -size + x, size + y, -size + z);
  setCoordinate(cube, 6, -size + x, -size + y, -size + z);
  setCoordinate(cube, 7, -size + x, -size + y, size + z);
  // 红
  if (i != level - 1) {
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
  if (j != level - 1) {
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
  if (k != level - 1) {
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
  cube = (Cube ***)malloc((unsigned long)level * sizeof(Cube **));
  for (i = 0; i < level; i++) {
    cube[i] = (Cube **)malloc((unsigned long)level * sizeof(Cube *));
    for (j = 0; j < level; j++) {
      cube[i][j] = (Cube *)malloc((unsigned long)level * sizeof(Cube));
      for (k = 0; k < level; k++) {
        initCube1(&cube[i][j][k], i, j, k);
      }
    }
  }
}

// 如果可以根据序号或者说索引得到顶点序列当然最好或者说最优雅，不行的话像这样枚举也无妨，毕竟并没有复杂或者说麻烦多少，而且也与阶数无关。
// 当然这样也算是高明的，不设置vertex[6][4]的话就是在代码里手写或者说枚举了，就是写6段结构一模一样的代码，这就蛋疼了。
static int vertex[6][4] = {{0, 1, 2, 3}, {4, 5, 6, 7}, {0, 5, 4, 1},
                           {2, 7, 6, 3}, {1, 4, 7, 2}, {3, 6, 5, 0}};
void drawCube1(Cube cube) {
  int i, j;
  for (i = 0; i < 6; i++) {
    Surface surface = cube.surface[i];
    glColor3f(surface.red, surface.green, surface.blue);
    for (j = 0; j < 4; j++) {
      glVertex3d(cube.coordinate[vertex[i][j]][0],
                 cube.coordinate[vertex[i][j]][1],
                 cube.coordinate[vertex[i][j]][2]);
    }
  }
}

void drawCube0() {
  glBegin(GL_QUADS);
  int i, j, k;
  for (i = 0; i < level; i++)
    for (j = 0; j < level; j++)
      for (k = 0; k < level; k++)
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
  gluLookAt(eyeX, eyeY, eyeZ, 0, 0, 0, upX, upY,
            upZ); //从视点看远点,y轴方向(0,1,0)是上方向
  drawCube0();
  glFlush();
  glutSwapBuffers();
}

static int start, rotateAngle, layer, type;
void onRotate(int value) {
  if (start) {
    if (rotateAngle < 90) {
      rotateAngle += 1;
      angle = 1 * c;
      rotate0(layer, type);
    } else {
      start = 0;
      rotateAngle = 0;
      reviseCoordinates0(layer, type);
      swap0(layer, type);
      layer = rand() % level;
      type = (type + 1) % 3;
    }
    glutPostRedisplay();
  }
  glutTimerFunc(1, onRotate, value);
}

static int currentButton; // 0是按下了左键，1是按下了右键
void Mouse(int button, int state, int x, int y) //处理鼠标点击
{
  if (button == GLUT_LEFT_BUTTON &&
      state == GLUT_DOWN) //第一次鼠标按下时,记录鼠标在窗口中的初始坐标
  {
    currentButton = 0;
    preX = x;
    preY = y;
  }
  if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
    currentButton = 1;
    if (!start) {
      start = 1;
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

void onMouseMove(int x, int y) //处理鼠标拖动
{
  // 左键拖动才旋转视角
  if (0 == currentButton) {
    // printf("%d\n",du);
    alpha +=
        x -
        preX; //鼠标在窗口x轴方向上的增量加到视点绕y轴的角度上，这样就左右转了
    beta += y - preY; //鼠标在窗口y轴方向上的改变加到视点的y坐标上，就上下转了
    alpha = fixedAngle(alpha);
    beta = fixedAngle(beta);
    // if(h>66.6f) h=66.6f; //视点y坐标作一些限制，不会使视点太奇怪
    // else if(h<-1.0f) h=-1.0f;
    preX = x;
    preY = y; //把此时的鼠标坐标作为旧值，为下一次计算增量做准备
    glutPostRedisplay();
  }
}
void reshape(int w, int h) {
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
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(888, 888);
  glutCreateWindow("OpenGL");
  glEnable(GL_DEPTH_TEST);
  glutReshapeFunc(reshape);
  glutDisplayFunc(draw);
  //  glutIdleFunc(draw); //设置不断调用显示函数
  glutTimerFunc(1, onRotate, 0);
  glutMouseFunc(Mouse);
  glutMotionFunc(onMouseMove);
  glutMainLoop();
  return 0;
}
