typedef struct Surface {
  float red, green, blue;
} Surface;
typedef struct Cube {
  Surface surface[6];
  double coordinate[8][3];
  double preCoordinate[8][3];
} Cube;
#define LEVEL 7
extern Cube *cube[LEVEL][LEVEL][LEVEL];
extern int angle;
extern double c;
void setColor(Cube *cube, int surfaceIndex, float red, float green, float blue);
void setCoordinates(Cube *cube, int vertexIndex, double x, double y, double z);
int onEdge0(int i, int j, int k);
void rotate0(int k, int type);
void swap(int k, int type, int operation);
void saveCoordinates0(int k, int type);
void reviseCoordinates0(int k, int type);
