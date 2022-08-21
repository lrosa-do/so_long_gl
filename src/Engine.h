#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>
//#define GL_GLEXT_PROTOTYPES 1
//#include <SDL2/SDL_opengl.h>
//#include <SDL2/SDL_opengl_glext.h>
#include "glad.h"

#include <stdio.h>
#define mem_malloc(sz)        _mem_alloc(sz, 0, __FILE__, __LINE__)
#define mem_calloc(num, sz)   _mem_alloc((num) * (sz), 1, __FILE__, __LINE__)
#define mem_realloc(ptr, sz)  _mem_realloc(ptr, sz, __FILE__, __LINE__)
#define mem_free(ptr)         _mem_free(ptr, __FILE__, __LINE__)
#define mem_size(ptr)         _mem_size(ptr, __FILE__, __LINE__)

void   *_mem_alloc(size_t, int, const char*, unsigned);
void   *_mem_realloc(void*, size_t, const char*, unsigned);
void    _mem_free(void*, const char*, unsigned);
size_t  _mem_size(void*, const char*, unsigned);

void    mem_dump(FILE*);
size_t  mem_usage(void);
int     mem_has(void *ptr);

//****************DEFINES

#ifdef NDEBUG
#define trace(M, ...)
#else
#define trace(M, ...) fprintf(stderr, "TRACE %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif


#define PI 3.14159265358979323846f
#define EPSILON 0.000001f
#define DEG2RAD (PI/180.0f)
#define RAD2DEG (180.0f/PI)

#define MAP_INT      0
#define MAP_REAL     1
#define MAP_STRING   2
#define MAP_VOID     3
#define MAP_TEXTURE  4

#if defined(__cplusplus)
    #define CLITERAL(type)      type
#else
    #define CLITERAL(type)      (type)
#endif



//raylib defines
typedef enum
{
    IGRAYSCALE = 1,     // 8 bit per pixel (no alpha)
    IGRAY_ALPHA,        // 8*2 bpp (2 channels)
    IR5G6B5,            // 16 bpp
    IR8G8B8,            // 24 bpp
    IR5G5B5A1,          // 16 bpp (1 bit alpha)
    IR4G4B4A4,          // 16 bpp (4 bit alpha)
    IR8G8B8A8,          // 32 bpp
} PixelFormat;

#define LIGHTGRAY  CLITERAL(Color){ 200, 200, 200, 255 }   // Light Gray
#define GRAY       CLITERAL(Color){ 130, 130, 130, 255 }   // Gray
#define DARKGRAY   CLITERAL(Color){ 80, 80, 80, 255 }      // Dark Gray
#define YELLOW     CLITERAL(Color){ 253, 249, 0, 255 }     // Yellow
#define GOLD       CLITERAL(Color){ 255, 203, 0, 255 }     // Gold
#define ORANGE     CLITERAL(Color){ 255, 161, 0, 255 }     // Orange
#define PINK       CLITERAL(Color){ 255, 109, 194, 255 }   // Pink
#define RED        CLITERAL(Color){ 230, 41, 55, 255 }     // Red
#define MAROON     CLITERAL(Color){ 190, 33, 55, 255 }     // Maroon
#define GREEN      CLITERAL(Color){ 0, 228, 48, 255 }      // Green
#define LIME       CLITERAL(Color){ 0, 158, 47, 255 }      // Lime
#define DARKGREEN  CLITERAL(Color){ 0, 117, 44, 255 }      // Dark Green
#define SKYBLUE    CLITERAL(Color){ 102, 191, 255, 255 }   // Sky Blue
#define BLUE       CLITERAL(Color){ 0, 121, 241, 255 }     // Blue
#define DARKBLUE   CLITERAL(Color){ 0, 82, 172, 255 }      // Dark Blue
#define PURPLE     CLITERAL(Color){ 200, 122, 255, 255 }   // Purple
#define VIOLET     CLITERAL(Color){ 135, 60, 190, 255 }    // Violet
#define DARKPURPLE CLITERAL(Color){ 112, 31, 126, 255 }    // Dark Purple
#define BEIGE      CLITERAL(Color){ 211, 176, 131, 255 }   // Beige
#define BROWN      CLITERAL(Color){ 127, 106, 79, 255 }    // Brown
#define DARKBROWN  CLITERAL(Color){ 76, 63, 47, 255 }      // Dark Brown

#define WHITE      CLITERAL(Color){ 255, 255, 255, 255 }   // White
#define BLACK      CLITERAL(Color){ 0, 0, 0, 255 }         // Black
#define BLANK      CLITERAL(Color){ 0, 0, 0, 0 }           // Blank (Transparent)
#define MAGENTA    CLITERAL(Color){ 255, 0, 255, 255 }     // Magenta
#define RAYWHITE   CLITERAL(Color){ 245, 245, 245, 255 }   // My own White (raylib logo)



//****************STRUCTS

// Color, 4 components, R8G8B8A8 (32bit)
typedef struct Color {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
} Color;



typedef struct Texture2D
{
unsigned int ID;
int width;
int height;
int textureWidth;
int textureHeight;
int format;
}Texture2D;

typedef struct Vec2
{
float x;
float y;
}Vec2;

typedef struct Vec3
{
float x;
float y;
float z;
}Vec3;

typedef struct Vec4
{
float x;
float y;
float z;
float w;
}Vec4;

typedef struct Mat4
{
    float m0, m4, m8, m12;
    float m1, m5, m9, m13;
    float m2, m6, m10, m14;
    float m3, m7, m11, m15;
}Mat4;

typedef struct Rect {
    float x;
    float y;
    float width;
    float height;
} Rect;


typedef struct Camera2D {
    Vec2 offset;
    Vec2 position;         // Camera target (rotation and zoom origin)
    float rotation;         // in degrees
    float zoom;             //  1.0f by default
    Rect bound;
    Color color;
    Mat4 frustum;
    Mat4 transform;
} Camera2D;


typedef union mapvalue
{
    int type;
    int number;
    void* data;
    float real;
    const char  *string;
    Texture2D texture;
} mapvalue;


typedef struct HashItem
{
    int type;
    int index;
    const char *key;
    mapvalue value;
    struct HashItem* next;
} HashItem;

typedef struct HashMap
{
    int size;
    HashItem *root;
} HashMap;

typedef struct Shader
{
unsigned int id;
HashMap *locs;
}Shader;

typedef struct PrimitivesBatch
{
struct
{
    float *vextex;
    unsigned int VBO,VAO;
    int index;
    int currentBatchSize;
}Lines;

struct
{
    float *vextex;
    unsigned int VBO,VAO;
    int index;
    int currentBatchSize;
}Solid;

Shader shader;

int capacity;
int maxCapacity;
int vertexStrideSize;



}PrimitivesBatch;


typedef struct SpriteBatch
{
float *vextex;
Shader shader;
unsigned int VBO,VAO;
int index;
int currentBatchSize;
int capacity;
int maxCapacity;
int vertexStrideSize;
int countDrawCalls;
int countTextureBind;
Texture2D currentBaseTexture;

}SpriteBatch;


typedef struct PostProcessor
{

struct
{
    unsigned int MSFBO;
    unsigned int FBO;
    unsigned int RBO;
    unsigned int VAO;
    Texture2D texture;
}Buffer;

 unsigned int Width;
 unsigned int Height;
 int Blur;
 int Chaos;
 int Shake;
 Shader    shader;

}PostProcessor;


typedef struct Graph
{
	Uint8           *data;
	int             channels;
	int             format;
	int				width;
	int				height;
	int				id;
	struct Graph	*next;
}Graph;


typedef struct Assets
{
    HashMap *map;
    struct
    {
        Rect *clips;
        int  *ids;
        int count;
        Texture2D texture;
    }TextureAtlas;

    struct
    {
        int count;
        Graph *root;
    }Graphics;

}Assets;




//*****************UTILS
char            *LoadTextFile(const char *fileName);
unsigned char   *LoadDataFile(const char *fileName, unsigned int *bytesRead);
const char	    *file_extension(const char *file_name);
int	            isfile_extension(const char *file_name, const char *ext);
size_t	        strlcpy(char *dest, const char *src, size_t size);
char            *ftstrdup(const char *str);
char            *ftltrim(char *s);
char            *ftrtrim(char *s);
char            *fttrim(char *s);


//****************HASHMAP
HashMap *CreateHashMap();
void     PrintHashMap(HashMap *map);
void     FreeHashMap(HashMap *map);
void     ClearHashMap(HashMap *map);
//O(N)
int      isHashMapEmpty(HashMap *map);

//O(N)
int          HashMapContainsKey(HashMap *map, const char* key);
const char* GetHashMapString(HashMap *map, const char* key);
float       GetHashMapReal(HashMap *map, const char* key);
int         GetHashMapInt(HashMap *map, const char* key);
Texture2D   GetHashMapTexture(HashMap *map, const char* key);

//add integer on the end of the list //O(N)
int     AddHashMapInt(HashMap *map, const char* key, int value);
//add integer on the begin of the list //O(1)
int     InsertHashMapInt(HashMap *map, const char* key, int value);
int     InsertHashMapString(HashMap *map, const char* key, const char* value);
int     AddHashMapString(HashMap *map, const char* key, const char* value);
int     InsertHashMapReal(HashMap *map, const char* key, float value);
int     AddHashMapReal(HashMap *map, const char* key, float value);

int     AddHashMapTexture(HashMap *map, const char* key, Texture2D value);

//******************ASSETS

Assets*     CreateAssetsManager();
Texture2D   LoadTextureAsset(Assets *a,const char* name,const char* fileName);
void        AddTextureAsset(Assets *a,const char* name,Texture2D texture);
void        AddGraphiAsset(Assets *a, Graph	*graph);
Graph	    *LoadGraphicAsset(Assets *a, const char* fileName,int id);
Graph       *GetAssetGraph(Assets *a,int id);
void        FreeAssetManager(Assets *a);
int         GererateGraphicsAtlas(Assets *a, int padding,int GenerateTextureAtlas,int SaveAtlasFile);
Rect        GetAssetsAtlasClip(Assets *a, int index);
Rect        GetAssetsAtlasClipById(Assets *a, int index);
void        SaveAtlasText(Assets *a,const char* FileName);
void        LoadAtlasText(Assets *a,const char* FileName,const char* TextureName);

//*******************IMAGE GRAPHIC
Graph   *LoadGraphic(const char* fileName);
Graph*   LoadXpmGraph(const char* Filename);
void    SaveGraphic(Graph *gr, const char* fileName);
Graph   *CreateGraphic(Uint32 width, Uint32 height, Uint8 channels);
Graph   *CreateGraphicFromData(int width, int height, int channels, void *data);
void    GraphCopy(Graph *dst, Graph src, Rect srcRec, Rect dstRec, Color tint);//slow
void    FreeGraphic(Graph *gr);




void        *GetTexturePixels( unsigned int id , int width, int height, int format);
int         GetPixelDataSize(int width, int height, int format);
int         SavePixelsDataFile(int width, int height,int format,void* data, const char* fileName);
SDL_Surface *CreateSurfaceFromTexture(Texture2D tex, void *data);
SDL_Surface *CreateSurfaceFromData(int width, int height, int channels, void *data);
SDL_Surface *CreateSurfaceFromGraph(Graph *gr);


//*****************SHADER

Shader CreateShader(const char *vertexSource, const char *fragmentSource);
Shader LoadShader(const char *vertexSource, const char *fragmentSource);
void   ShaderBind(Shader shader);
void   FreeShader(Shader shader);
int    GetShaderUniform(Shader shader,const char *name);
void   AddShaderUniform(Shader shader,const char *name);
void   SetShaderMatrix4(Shader shader,const char* name, Mat4 mat);
void   SetShaderInt(Shader shader,const char* name, int value);
void   SetShaderFloat(Shader shader,const char* name, float value);


//********************TEXTURE2D

Texture2D CreateTexture(Uint32 width, Uint32 height, Uint8* data, Uint8 channels);
Texture2D CreateTextureFromGraph(Graph *gr);
Texture2D LoadTexture(const char* Filename);
int       SaveTextureFile(Texture2D texture, const char* fileName );
void      FreeTexture(Texture2D tex);
void      BindTexture(Texture2D tex);


//******************************************RENDER
void UpdateCamera(Camera2D *camera);
void ClearCamera(Camera2D camera);

void FreePrimitivesBatch(PrimitivesBatch *batch);
void BeginPrimitivesBatch(PrimitivesBatch* batch);
void EndPrimitivesBatch(PrimitivesBatch* batch, Camera2D camera);
void RenderSolidCircle(PrimitivesBatch* batch, float x, float y, int segments, float radius, Color c);
void RenderSolidRectangle(PrimitivesBatch* batch, float x, float y, float width,float height, Color c);
void RenderTriangle(PrimitivesBatch* batch, float x1, float y1, float x2,float y2,float x3,float y3, Color c);
PrimitivesBatch *CreatePrimitivesBatch(int capacity);
void RenderLine(PrimitivesBatch* batch, float x1, float y1, float x2,float y2,Color c);
void RenderRectangle(PrimitivesBatch* batch, float x, float y, float x2,float y2, Color c);
void RenderCircle(PrimitivesBatch* batch, float x, float y, int segments, float radius, Color c);



SpriteBatch *CreateSpriteBatch(int capacity);
void BeginSpriteBatch(SpriteBatch* batch, Camera2D camera);
void EndSpriteBatch(SpriteBatch* batch);
void FreeSpriteBatch(SpriteBatch *batch);
void RenderTextureTriangle(SpriteBatch* batch,float x1, float y1, float ux1, float uy1,float x2, float y2, float ux2, float uy2,float x3, float y3, float ux3, float uy3, Color c);
void RenderTexture(SpriteBatch* batch, Texture2D tex, float x, float y, float width,float height, Color c);
void RenderTextureClip(SpriteBatch* batch, Texture2D tex, float x, float y, float width,float height, Rect src,Color c);
void RenderAssetTexture(Assets *a, SpriteBatch* batch, float x, float y, int id ,Color c);

PostProcessor *CreatePostProcessor( int width,  int height);
void FreePostProcessor(PostProcessor *proc);
void BeginPostProcessor(PostProcessor *proc);
void EndPostProcessor(PostProcessor *proc);
void RenderPostProcessor(PostProcessor *proc,float time);


//*****************MATH
float Clamp(float value, float min, float max);
float Lerp(float start, float end, float amount);
float Normalize(float value, float start, float end);
float Remap(float value, float inputStart, float inputEnd, float outputStart, float outputEnd);
float Wrap(float value, float min, float max);
int FloatEquals(float x, float y);
Mat4 OrthoMatrix(double left, double right, double bottom, double top, double near, double far);

#endif // ENGINE_H
