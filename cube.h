typedef struct Surface {
  float red, green, blue;
} Surface;
typedef struct Cube {
  Surface surface[6];
  double coordinate[8][3];
  double preCoordinate[8][3];
} Cube;
extern Cube ***cube;
// 不要extern也行，但不能把int size = 6, level = 7;写在这里，否则重定义。
extern int size, level;
extern double angle;
void setColor(Cube *cube, int surfaceIndex, float red, float green, float blue);
void setCoordinate(Cube *cube, int vertexIndex, double x, double y, double z);
void rotate0(int k, int type);
void swap0(int k, int type);
void saveCoordinates0(int k, int type);
void reviseCoordinates0(int k, int type);
