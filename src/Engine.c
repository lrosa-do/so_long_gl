#include "Engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
# include <sys/time.h>

#include "glad.h"


#define STBI_MALLOC mem_malloc
#define STBI_FREE mem_free
#define STBI_REALLOC mem_realloc
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"


#define STBIW_MALLOC mem_malloc
#define STBIW_FREE mem_free
#define STBIW_REALLOC mem_realloc

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


///***********************************************************MEM LEAKS

typedef struct mem_node_t
{
  struct mem_node_t *prev, *next;
  const char *file;
  size_t line;
  size_t size;

} mem_node_t;


mem_node_t *mem_head;



int _mem_has_node(mem_node_t *n)
 {
  mem_node_t *node = mem_head;
  while (node != NULL)
  {
    if (node == n) return 1;
    node = node->next;
  }
  return 0;
}



void _mem_abort(void)
{

  abort();
}



void *_mem_alloc(size_t sz, int zeroset, const char *file, unsigned line)
 {
  mem_node_t *node = NULL;

  if (zeroset)
  {
    node = calloc(sizeof(*node) + sz, 1);
  } else
  {
    node = malloc(sizeof(*node) + sz);
    if (node != NULL)
    {
      memset(node, 0, sizeof(*node));
    }
  }

  if (node == NULL)
  {

    fprintf(stderr, "Couldn't allocate: %s, line %u\n", file, line);
    _mem_abort();

  }

  node->line = line;
  node->file = file;
  node->size = sz;


  if (mem_head)
   {
    mem_head->prev = node;
    node->next = mem_head;
  }
  mem_head = node;

  return (char*)node + sizeof(*node);
}



void *_mem_realloc(void *ptr, size_t sz, const char *file, unsigned line)
 {
  mem_node_t *node = (mem_node_t*)((char*)ptr - sizeof(*node));
  mem_node_t *old_node = node;

  if (ptr == NULL) return _mem_alloc(sz, 0, file, line);

  if (!_mem_has_node(node))
   {
    fprintf(stderr, "Bad realloc: %p %s, line %u\n", ptr, file, line);
    _mem_abort();
  }


  node = realloc(node, sizeof(*node) + sz);

  if (node == NULL)
  {

    fprintf(stderr, "Couldn't reallocate: %s, line %u\n", file, line);
    _mem_abort();

  }

  node->size = sz;
  if (mem_head == old_node) mem_head = node;
  if (node->prev) node->prev->next = node;
  if (node->next) node->next->prev = node;

  return (char*)node + sizeof(*node);
}



void _mem_free(void *ptr, const char *file, unsigned line)
 {
  mem_node_t *node = (mem_node_t*)((char*)ptr - sizeof(*node));

  if (ptr == NULL) return;

  if (!_mem_has_node(node))
   {
    fprintf(stderr, "Bad free: %p %s, line %u\n", ptr, file, line);
    _mem_abort();
  }


  if (node == mem_head) mem_head = node->next;
  if (node->prev) node->prev->next = node->next;
  if (node->next) node->next->prev = node->prev;

  free(node);
}



void mem_dump(FILE *fp)
{
  mem_node_t *node = mem_head;
  size_t total = 0;

  if (!fp) fp = stdout;

  while (node != NULL)
   {
    fprintf(fp, "Unfreed: %p %s, line %lu (%lu bytes)\n",
            (char*)node + sizeof(*node), node->file,
            (unsigned long)node->line, (unsigned long)node->size);

    total += node->size;
    node = node->next;
  }

  fprintf(fp, "Total unfreed: %lu bytes\n", (unsigned long)total);
}



size_t _mem_size(void *ptr, const char* file, unsigned line)
 {
  mem_node_t *node = (mem_node_t*)((char*)ptr - sizeof(*node));


  if (!_mem_has_node(node))
  {
    fprintf(stderr, "Bad pointer: %p %s, line %u\n", ptr, file, line);
    _mem_abort();
  }


  return node->size;
}



size_t mem_usage(void)
{
  mem_node_t *node = mem_head;
  size_t total = 0;

  while (node != NULL)
  {
    total += node->size;
    node = node->next;
  }

  return total;
}



int mem_has(void *ptr)
{
  mem_node_t *node = (mem_node_t*)((char*)ptr - sizeof(*node));
  return _mem_has_node(node);
}


//**************************************************************UTILS

char * ftstrdup(const char *str)
{
        char *tmp;
        int len;

        len = strlen(str) + 1;
        tmp = mem_malloc(len * sizeof(char));
        strcpy(tmp,str);
        return tmp;
}

size_t	strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;

	if (size)
	{
		i = 0;
		while (src[i] && i < size - 1)
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
	}
	return (strlen(src));
}
char *ftltrim(char *s)
{
    while(isspace(*s)) s++;
    return s;
}

char *ftrtrim(char *s)
{
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}


char *fttrim(char *s)
{
    return ftrtrim(ftltrim(s));
}



size_t	ftstrlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	if (dstsize == 0)
		return (strlen(src));
	while (i < dstsize - 1 && src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	if (i < dstsize)
		dst[i] = '\0';
	while (src[i] != '\0')
		i++;
	return (i);
}

const char	*file_extension(const char *file_name)
{
	const char	*dot;

	dot = strrchr(file_name, '.');
	if (!dot || dot == file_name)
		return (NULL);
	return (dot);
}

int	isfile_extension(const char *file_name, const char *ext)
{
	if (strncmp(strchr(file_name, '.'), ext, 5) != 0)
		return (0);
	return (1);
}


unsigned char *LoadDataFile(const char *fileName, unsigned int *bytesRead)
{
     unsigned char *data = NULL;
    *bytesRead = 0;

    if (fileName != NULL)
    {
        SDL_RWops* file= SDL_RWFromFile(fileName, "rb");


        if (file != NULL)
        {
            int size =(int) SDL_RWsize(file);

            if (size > 0)
            {
                data = (unsigned char *)mem_malloc(size*sizeof(unsigned char));

                unsigned int count = (unsigned int) SDL_RWread(file, data, sizeof(unsigned char), size);
                *bytesRead = count;
            }
            else SDL_LogWarn(0, "Failed to read file: [%s]", fileName);
            SDL_RWclose(file);
        }
        else SDL_LogWarn(0, "Failed to open file: [%s]", fileName);
    }
    return data;
}




char *LoadTextFile(const char *fileName)
{
    char *text = NULL;

    if (fileName != NULL)
    {

          SDL_RWops* file= SDL_RWFromFile(fileName, "rt");


        if (file != NULL)
        {
            int size =(int) SDL_RWsize(file);

            if (size > 0)
            {
                text = (char *)mem_malloc((size + 1)*sizeof(char));
                unsigned int count = (unsigned int)SDL_RWread(file, text, sizeof( char), size);

                if (count < size) text = mem_realloc(text, count + 1);
                text[count] = '\0';
                SDL_Log( " Text file loaded successfully: [%s]", fileName);
            }
            else SDL_Log( "Failed to read text file: [%s]", fileName);

            SDL_RWclose(file);
        }
        else SDL_Log( "Failed to open text file: [%s]", fileName);
    }


    return text;
}

//*************************************************************HASHMAP
HashMap *CreateHashMap()
{
    HashMap* map = (HashMap*) mem_malloc(sizeof(HashMap));
    bzero(map,sizeof(HashMap));
    map->size=0;

    map->root=NULL;
    return map;
}

HashItem *CreateHashItem()
{
    HashItem* n=(HashItem*) mem_malloc(sizeof(HashItem));
    bzero(n,sizeof(HashItem));
    n->next=NULL;
return n;
}

void FreeHashMapItem(HashItem *node)
{
    if (node)
    {
        switch (node->type)
        {
            case MAP_INT:
            {
                SDL_Log("Free Map Int Item (%s) Value:(%d) .", node->key,node->value.number);
            }break;
            case MAP_REAL:
            {
               SDL_Log("Free Map Float Item (%s) Value:(%f) .",node->key,node->value.real);
            }break;
            case MAP_STRING:
            {

                SDL_Log("Free Map String Item (%s) Value:(%s) .",node->key,node->value.string);
                if (node->value.string)
                    mem_free((void*)node->value.string);
            }break;
            case MAP_TEXTURE:
            {

                 FreeTexture(node->value.texture);
            }break;
        }

        if (node->key)
            mem_free((void*)node->key);
    }
}


void FreeHashMap(HashMap *map)
{
     SDL_Log("Free HashMap");
     if(map)
     {
        HashItem* current;
        HashItem* next;
        if (map->root)
        {
            current = map->root;
            while(current != NULL)
            {
                next = current->next;
                FreeHashMapItem(current);
                mem_free(current);
                current = next;
            }
            map->root = NULL;
        }
        mem_free(map);
        map = NULL;
     }
}


int isHashMapEmpty(HashMap *map)
{
     if(map)
     {
        return (map->size>0);
     }
     return (1);
}
void ClearHashMap(HashMap *map)
{
     if(map)
     {
        HashItem* current;
        HashItem* next;
        int index=0;

        if (map->root)
        {
            current = map->root;
            while(current != NULL)
            {
                next = current->next;
                FreeHashMapItem(current);
                mem_free(current);
                current = next;
                index +=1;
            }
            map->root = NULL;
        }
     }
}

HashItem *GetHashMapItem(HashMap *map, const char* key)
{
     if(map)
     {
        HashItem* current;
        if (map->root)
        {
            current = map->root;
            while(current != NULL)
            {
                int count = strcasecmp(key,current->key);
                if (count == 0)
                    return (current);
                current = current->next;
            }
        }
     }
    return (NULL);
}

int GetHashMapInt(HashMap *map, const char* key)
{
     if(map)
     {
        HashItem* current;
        if (map->root)
        {
            current = map->root;
            while(current != NULL)
            {
                int count = strcasecmp(key,current->key);
                if (count == 0 && current->type == MAP_INT)
                    return (current->value.number);
                current = current->next;
            }
        }
     }
    return (-1);
}

float GetHashMapReal(HashMap *map, const char* key)
{
     if(map)
     {
        HashItem* current;
        if (map->root)
        {
            current = map->root;
            while(current != NULL)
            {
                int count = strcasecmp(key,current->key);
                if (count == 0 && current->type == MAP_REAL)
                    return (current->value.real);
                current = current->next;
            }
        }
     }
    return (-1);
}

const char* GetHashMapString(HashMap *map, const char* key)
{
     if(map)
     {
        HashItem* current;
        if (map->root)
        {
            current = map->root;
            while(current != NULL)
            {
                int count = strcasecmp(key,current->key);
                if (count == 0 && current->type == MAP_STRING)
                    return (current->value.string);
                current = current->next;
            }
        }
     }
    return (NULL);
}

Texture2D GetHashMapTexture(HashMap *map, const char* key)
{
    Texture2D tex ={0};
     if(map)
     {
        HashItem* current;
        if (map->root)
        {
            current = map->root;
            while(current != NULL)
            {
                int count = strcasecmp(key,current->key);
                if (count == 0 && current->type == MAP_TEXTURE)
                {
                    memcpy(&tex,&current->value.texture,sizeof(Texture2D));
                    return (tex);
                }
                current = current->next;
            }
        }
     }

    return (tex);
}


int HashMapContainsKey(HashMap *map, const char* key)
{
     if(map)
     {
        HashItem* current;
        if (map->root)
        {
            current = map->root;
            while(current != NULL)
            {
                int count = strcasecmp(key,current->key);
                if (count == 0)
                    return (1);
                current = current->next;
            }
        }
     }
     return (-1);
}

int HashMapRemoveItem(HashMap *map, const char* key)
{
    return 0;
}

void HashMapAppendIndex(HashMap *map,HashItem* item)
{
    if(map)
    {
        if (item)
        {
/*         if (map->size > =map->indexSize)
         {
            map->indexSize += 10;
            map->index = (HashItem**) mem_realloc(map->index, (map->indexSize) * sizeof(HashItem));
         }
         map->index[map->size] = item;*/
        }
    }
}

//add on the end of the list
int AddHashMapInt(HashMap *map, const char* key, int value)
{
    if(!map)
        return (-1);

    HashItem *node;
    HashItem *current;

    node               = CreateHashItem();
    node->index        = map->size;
    node->type         = MAP_INT;
    node->value.number = value;
    node->key          = ftstrdup(key);

    if (!map->root)
        map->root=node;
    else
    {
        current        = map->root;
        while (current->next != NULL)
                current     = current ->next;
        current->next       = node;
        current->next->next = NULL;
    }
    HashMapAppendIndex(map,node);
    map->size += 1;
    return (map->size -1);
}

//add on the begin of the list
int InsertHashMapInt(HashMap *map, const char* key, int value)
{
    if(!map)
        return (-1);

    HashItem *node;
    HashItem *tmp;

    node               = CreateHashItem();
    node->index        = map->size;
    node->type         = MAP_INT;
    node->value.number = value;
    node->key          = ftstrdup(key);

    if (!map->root)
        map->root=node;
    else
    {
        tmp        = map->root;
        map->root  = node;
        node->next = tmp;
    }
    HashMapAppendIndex(map,node);
    map->size += 1;
    return (map->size -1);
}




//add on the end of the list
int AddHashMapReal(HashMap *map, const char* key, float value)
{
    if(!map)
        return (-1);

    HashItem *node;
    HashItem *current;

    node               = CreateHashItem();
    node->index        = map->size;
    node->type         = MAP_REAL;
    node->value.real = value;
    node->key          = ftstrdup(key);

    if (!map->root)
        map->root=node;
    else
    {
        current        = map->root;
        while (current->next != NULL)
                current     = current ->next;
        current->next       = node;
        current->next->next = NULL;
    }
    HashMapAppendIndex(map,node);
    map->size += 1;
    return (map->size -1);
}

//add on the begin of the list
int InsertHashMapReal(HashMap *map, const char* key, float value)
{
    if(!map)
        return (-1);

    HashItem *node;
    HashItem *tmp;

    node               = CreateHashItem();
    node->index        = map->size;
    node->type         = MAP_REAL;
    node->value.real = value;
    node->key          = ftstrdup(key);

    if (!map->root)
        map->root=node;
    else
    {
        tmp        = map->root;
        map->root  = node;
        node->next = tmp;
    }
    map->size += 1;
    return (map->size -1);
}

//add on the end of the list
int AddHashMapString(HashMap *map, const char* key, const char* value)
{
    if(!map)
        return (-1);

    HashItem *node;
    HashItem *current;

    node               = CreateHashItem();
    node->index        = map->size;
    node->type         = MAP_STRING;
    node->value.string = ftstrdup(value);
    node->key          = ftstrdup(key);

    if (!map->root)
        map->root=node;
    else
    {
        current        = map->root;
        while (current->next != NULL)
                current     = current ->next;
        current->next       = node;
        current->next->next = NULL;
    }
  map->size += 1;
    return (map->size -1);
}

//add on the begin of the list
int InsertHashMapString(HashMap *map, const char* key, const char* value)
{
    if(!map)
        return (-1);

    HashItem *node;
    HashItem *tmp;

    node               = CreateHashItem();
    node->index        = map->size;
    node->type         = MAP_STRING;
    node->value.string = ftstrdup(value);
    node->key          = ftstrdup(key);

    if (!map->root)
        map->root=node;
    else
    {
        tmp        = map->root;
        map->root  = node;
        node->next = tmp;
    }
    map->size += 1;
    return (map->size -1);
}

int     AddHashMapTexture(HashMap *map, const char* key, Texture2D value)
{
    if(!map)
        return (-1);

    HashItem *node;
    HashItem *tmp;

    node               = CreateHashItem();
    node->index        = map->size;
    node->type         = MAP_TEXTURE;
    memcpy(&node->value.texture,&value,sizeof(Texture2D));
    node->key          = ftstrdup(key);

    if (!map->root)
        map->root=node;
    else
    {
        tmp        = map->root;
        map->root  = node;
        node->next = tmp;
    }
    map->size += 1;
    return (map->size -1);
}

void PrintHashMap(HashMap *map)
{
    if(map)
    {
        HashItem *current;
        int        index;

        current = map->root;
        index   = 0;
        while(current != NULL)
        {
                switch (current->type)
                {
                    case MAP_INT:
                    {
                        printf("Map (%d) Item (%s) Value:(%d) .\n",current->index, current->key,current->value.number);
                    }break;

                    case MAP_REAL:
                    {
                     printf("Map (%d) Item (%s) Value:(%f) .\n", current->index,current->key,current->value.real);
                    }break;
                    case MAP_STRING:
                    {
                        printf("Map (%d) Item (%s) Value:(%s) .\n",current->index, current->key,current->value.string);
                    }break;
                     case MAP_VOID:
                    {
                        printf("Map (%d) Item (%s) Value:(void) .\n",current->index, current->key);
                    }break;
                    case MAP_TEXTURE:
                    {
                        printf("Map (%d) Item (%s) Value:(TEXTURE2D)  (%d %d) - (%d) .\n",current->index, current->key,current->value.texture.width,current->value.texture.height,current->value.texture.ID);
                    }break;
                }
            current = current->next;
            index =0;
        }
    }
}

//******************ASSETS

Assets* CreateAssetsManager()
{

    Assets *assets=mem_malloc(sizeof(Assets));
    bzero(assets,sizeof(Assets));

    assets->map= CreateHashMap();
    assets->Graphics.count = 0;
    assets->Graphics.root = NULL;


    return(assets);
}
void FreeAssetManager(Assets *a)
{
    SDL_Log("Free Assets manager");
  if (a)
  {
    if(a->map)
    {
        FreeHashMap(a->map);
    }
    if (a->Graphics.root)
    {
        Graph *current = a->Graphics.root;
        Graph *next;
        while(current != NULL)
        {
            next = current->next;
            //mem_free(current->data);
            //mem_free(current);
            FreeGraphic(current);
            current = next;
        }
        a->Graphics.root = NULL;
    }
    if (a->TextureAtlas.count>=1)
    {
        if (a->TextureAtlas.clips)
            mem_free(a->TextureAtlas.clips);
         if (a->TextureAtlas.ids)
            mem_free(a->TextureAtlas.ids);
        if (a->TextureAtlas.texture.ID != -1)
        {
           // SaveTextureFile(a->TextureAtlas.texture,"Atlas.png");
            FreeTexture(a->TextureAtlas.texture);
        }
    }
    mem_free(a);
  }
}
Rect        GetAssetsAtlasClipById(Assets *a, int id)
{
    Rect r = {0};
    if (a)
    {
        for (int i=0;i<a->TextureAtlas.count;i++)
        {
            if (a->TextureAtlas.ids[i]==id)
                return (a->TextureAtlas.clips[i]);
        }
    }
    return r;
}

Rect GetAssetsAtlasClip(Assets *a,int index)
{
    Rect r = {0};
    if (a)
    {
        if (a->TextureAtlas.count >= 0 && index <=a->TextureAtlas.count)
        {
            return (a->TextureAtlas.clips[index]);
        }
    }
    return r;
}

Texture2D LoadTextureAsset(Assets *a,const char* name,const char* fileName)
{
    Texture2D tex = LoadTexture(fileName);
    if (a)
    {
        AddHashMapTexture(a->map,name,tex);
    }
    return tex;
}
void AddTextureAsset(Assets *a,const char* name,Texture2D texture)
{
    if (a && texture.ID !=-1)
    {
        AddHashMapTexture(a->map,name,texture);
    }
}

void AddGraphiAsset(Assets *a, Graph	*graph)
{
    if (a)
    {

	      Graph	 *current;
          if (!a->Graphics.root)
          {
             a->Graphics.root = graph;
          }
          else
          {
          current = a->Graphics.root;
          //add at end of the list
          while(current->next != NULL)
                current= current->next;
          current->next = graph;
          current->next->next=NULL;
          }
          a->Graphics.count +=1;
    }
}

Graph   *LoadGraphic(const char* fileName)
{
          Graph	 *graph;
          graph = (Graph *)mem_malloc(sizeof(Graph));
          bzero(graph,sizeof(Graph));
          graph->id      = -1;
          graph->next    = NULL;
          graph->data    = stbi_load(fileName, &graph->width , &graph->height, &graph->channels, 0);
            if (graph->channels == 1) graph->format = IGRAYSCALE;
            else if (graph->channels == 2) graph->format = IGRAY_ALPHA;
            else if (graph->channels == 3) graph->format = IR8G8B8;
            else if (graph->channels == 4) graph->format = IR8G8B8A8;
        return graph;
}

void    SaveGraphic(Graph *gr, const char* fileName)
{




    if (gr)
    {
        if (isfile_extension(fileName,".png"))
            stbi_write_png(fileName,gr->width,gr->height,gr->channels,(const unsigned char *)gr->data,gr->width*gr->channels);
        else
        if (isfile_extension(fileName,".jpg"))
            stbi_write_jpg(fileName,gr->width,gr->height,gr->channels,(const unsigned char *)gr->data,90);
        else
        if (isfile_extension(fileName,".bmp"))
            stbi_write_bmp(fileName,gr->width,gr->height,gr->channels,(const unsigned char *)gr->data);
        else
        if (isfile_extension(fileName,".tga"))
            stbi_write_tga(fileName,gr->width,gr->height,gr->channels,(const unsigned char *)gr->data);
        else
            SDL_LogError(0,"Unknow file format to save");


    }


}



typedef struct  s_xpm_col
{
  int           name;
  Uint32           col;
} t_xpm_col;


struct  s_col_name
{
  char  *name;
  Uint32   color;
};

#include "mlx_rgb.inc"


#define	RETURN	{ if (colors) mem_free(colors); if (tab) mem_free(tab); \
		  if (colors_direct) mem_free(colors_direct); \
                  if (img) FreeGraphic( img);   \
                  return img; }

int	mlx_int_str_str(char *str,char *find,int len)
{
  int	len_f;
  int	pos;
  char	*s;
  char	*f;

  len_f = strlen(find);
  if (len_f>len)
    return (-1);
  pos = 0;
  while (*(str+len_f-1))
    {
      s = str;
      f = find;
      while (*(f++) == *(s++))
        if (!*f)
          return (pos);
      str ++;
      pos ++;
    }
  return (-1);
}



int	mlx_int_str_str_cote(char *str,char *find,int len)
{
  int	len_f;
  int	pos;
  char	*s;
  char	*f;
  int	cote;

  len_f = strlen(find);
  if (len_f>len)
    return (-1);
  cote = 0;
  pos = 0;
  while (*(str+len_f-1))
    {
      if (*str=='"')
	cote = 1-cote;
      if (!cote)
	{
	  s = str;
	  f = find;
	  while (*(f++) == *(s++))
	    if (!*f)
	      return (pos);
	}
      str ++;
      pos ++;
    }
  return (-1);
}


char	**mlx_int_str_to_wordtab(char *str)
{
  char	**tab;
  int	pos;
  int	nb_word;
  int	len;

  len = strlen(str);
  nb_word = 0;
  pos = 0;
  while (pos<len)
  {
    while (*(str+pos)==' ' || *(str+pos)=='\t')
      pos ++;
    if (*(str+pos))
      nb_word ++;
    while (*(str+pos) && *(str+pos)!=' ' && *(str+pos)!='\t')
      pos ++;
  }
  if (!(tab = mem_malloc((1+nb_word)*sizeof(*tab))))
    return ((char **)0);
  nb_word = 0;
  pos = 0;
  while (pos<len)
    {
      while (*(str+pos)==' ' || *(str+pos)=='\t')
	{
	  *(str+pos) = 0;
	  pos ++;
	}
      if (*(str+pos))
	{
	  tab[nb_word] = str+pos;
	  nb_word ++;
	}
      while (*(str+pos) && *(str+pos)!=' ' && *(str+pos)!='\t')
	pos ++;
    }
  tab[nb_word] = 0;
  return (tab);
}


char	*mlx_int_get_line(char *ptr,int *pos,int size)
{
  int	pos2;
  int	pos3;
  int	pos4;

  if ((pos2 = mlx_int_str_str(ptr+*pos,"\"",size-*pos))==-1)
    return ((char *)0);
  if ((pos3 = mlx_int_str_str(ptr+*pos+pos2+1,"\"",size-*pos-pos2-1))==-1)
    return ((char *)0);
  *(ptr+*pos+pos2) = 0;
  *(ptr+*pos+pos2+1+pos3) = 0;
  pos4 = *pos+pos2+1;
  *pos += pos2+pos3+2;
  return (ptr+pos4);
}



char	*mlx_int_static_line(char **xpm_data,int *pos,int size)
{
  static char	*copy = 0;
  static int	len = 0;
  int		len2;
  char		*str;

  str = xpm_data[(*pos)++];
  if ((len2 = strlen(str))>len)
    {
      if (copy)
        mem_free(copy);
      if (!(copy = mem_malloc(len2+1)))
	return ((char *)0);
      len = len2;
    }
  strlcpy(copy, str, len2+1);
  return (copy);
}


int	mlx_int_get_col_name(char *str,int size)
{
  int	result;

  result = 0;
  while (size--)
    result = (result<<8)+*(str++);
  return (result);
}

Uint32	mlx_int_get_text_rgb(char *name, char *end)
{
  int	i;
  char	buff[64];

  if (*name == '#')
    return (strtol(name+1,0,16));
  if (end)
    {
      snprintf(buff, 64, "%s %s", name, end);
      name = buff;
    }
  i = 0;
  while (mlx_col_name[i].name)
    {
      if (!strcasecmp(mlx_col_name[i].name, name))
	return (mlx_col_name[i].color);
      i ++;
    }
  return (0);
}




int ColorToInt(Color color)
{
    return (((int)color.r << 24) | ((int)color.g << 16) | ((int)color.b << 8) | (int)color.a);
}


Color GetColor(unsigned int hexValue)
{
    Color color;

    color.r = (unsigned char)(hexValue >> 24) & 0xFF;
    color.g = (unsigned char)(hexValue >> 16) & 0xFF;
    color.b = (unsigned char)(hexValue >> 8) & 0xFF;
    color.a = (unsigned char)hexValue & 0xFF;

    return color;
}




Graph	*mlx_int_parse_xpm(void *data,int info_size,char *(*f)())
{
  int	pos;
  char	*line;
  char	**tab;
  char	*clip_data;
  int	nc;

  int	cpp;
  Uint32	col;
  int	rgb_col;
  int	col_name;
  int	method;
  int	x;
  int	i;
  int	j;
  Graph	*img;
  t_xpm_col	*colors;
  int		*colors_direct;

  colors = 0;
  colors_direct = 0;
  img = 0;
  tab = 0;
  pos = 0;
  int width;
  int height;
  if (!(line = f(data,&pos,info_size)) ||
      !(tab = mlx_int_str_to_wordtab(line)) || !(width = atoi(tab[0])) ||
      !(height = atoi(tab[1])) || !(nc = atoi(tab[2])) ||
      !(cpp = atoi(tab[3])) )
    RETURN;
  mem_free(tab);
  tab = 0;

  method = 0;
  if (cpp<=2)
    {
      method = 1;
      if (!(colors_direct = mem_malloc((cpp==2?65536:256)*sizeof(int))))
	RETURN;
    }
  else
    if (!(colors = mem_malloc(nc*sizeof(*colors))))
      RETURN;

  clip_data = 0;

  i = nc;
  while (i--)
    {
      if (!(line = f(data,&pos,info_size)) ||
	  !(tab = mlx_int_str_to_wordtab(line+cpp)) )
	RETURN;
      j = 0;
      while (tab[j] && strcmp(tab[j++],"c"));

      if (!tab[j])
	RETURN;

      rgb_col = mlx_int_get_text_rgb(tab[j], tab[j+1]);
      if (method)
            colors_direct[mlx_int_get_col_name(line,cpp)] = rgb_col;
      else
	{
	  colors[i].name = mlx_int_get_col_name(line,cpp);
	  colors[i].col = rgb_col;
	}
      mem_free(tab);
      tab = 0;
    }

  if (!(img = CreateGraphic(width,height,4)))
        RETURN;


  Uint8 *dst;
  dst = (Uint8 *)img->data;


  i = height;
  while (i--)
  {
      if (!(line = f(data,&pos,info_size)))
            RETURN;
      x = 0;
    while (x<width)
	{
	  col = 0;
	  col_name = mlx_int_get_col_name(line+cpp*x,cpp);
	  if (method)
            col = colors_direct[col_name];
	  else
	    {
	      j = nc;
	      while (j--)
		  if (colors[j].name==col_name)
	 	  {
		    col = colors[j].col;
		    j = 0;
		  }
	    }


        int r = (col&0xFF);
        int g = ((col>>8)&0xFF);
        int b = ((col>>16)&0xFF);

         if (col==-1)
         {
            Uint32 argb = (((int)0 << 24) | ((int)r << 16) | ((int)g << 8) | (int)b);
           *((Uint32 *)(dst+4*x)) =argb;
           }
        else
        {
            Uint32 argb = (((int)255 << 24) | ((int)r << 16) | ((int)g << 8) | (int)b);
            *((Uint32*)(dst+4*x)) =argb;
        }
	  x ++;
	}
      dst += img->width*4;
    }


  if (colors)
    mem_free(colors);
  if (colors_direct)
    mem_free(colors_direct);
  return (img);
}


void	mlx_int_file_get_rid_comment(char *ptr, int size)
{
  int	com_begin;
  int	com_end;

  while ((com_begin = mlx_int_str_str_cote(ptr,"/*",size))!=-1)
    {
      com_end = mlx_int_str_str(ptr+com_begin+2,"*/",size-com_begin-2);
      memset(ptr+com_begin,' ',com_end+4);
    }
  while ((com_begin = mlx_int_str_str_cote(ptr,"//",size))!=-1)
    {
      com_end = mlx_int_str_str(ptr+com_begin+2,"\n",size-com_begin-2);
      memset(ptr+com_begin,' ',com_end+3);
    }
}



Graph* LoadXpmGraph(const char* Filename)
{
  unsigned int	size;
  char	*ptr;
  Graph	*img;
  ptr = (char*) LoadDataFile(Filename, &size);
  if (!ptr)
      return ((void *)0);
  mlx_int_file_get_rid_comment(ptr, size);
  img = mlx_int_parse_xpm(ptr,size,mlx_int_get_line);
  mem_free(ptr);
  return (img);
}



Graph   *CreateGraphic(Uint32 width, Uint32 height, Uint8 channels)
{
          Graph	 *graph;
          graph = (Graph *)mem_malloc(sizeof(Graph));
          bzero(graph,sizeof(Graph));
          graph->id      = -1;
          graph->width =width;
          graph->height=height;
          graph->next    = NULL;
          graph->data    =(Uint8*) mem_malloc((width*height)*channels);
          graph->channels = channels;
            if (graph->channels == 1) graph->format = IGRAYSCALE;
            else if (graph->channels == 2) graph->format = IGRAY_ALPHA;
            else if (graph->channels == 3) graph->format = IR8G8B8;
            else if (graph->channels == 4) graph->format = IR8G8B8A8;
            return graph;
}

Graph *CreateGraphicFromData(int width, int height, int channels, void *data)
{
     Graph *gr=CreateGraphic(width,height,channels);
     gr->data=data;
     return gr;
}
void    FreeGraphic(Graph *gr)
{
    if(gr)
    {
      if (gr->data)
      {
        mem_free(gr->data);
        gr->data=NULL;
      }

      mem_free(gr);
      gr=NULL;
    }
}

Graph	*LoadGraphicAsset(Assets *a, const char* fileName,int id)
{

    SDL_assert(a!=NULL);

          Graph	 *graph=NULL;
          if (isfile_extension(fileName,".xpm"))
            graph= LoadXpmGraph(fileName);
          else
            graph= LoadGraphic(fileName);
        
           SDL_assert(graph!=NULL);
          
            graph->id = id;
            AddGraphiAsset(a, graph);
          
         return (graph);
   
    
}


SDL_Surface *CreateSurfaceFromData(int width, int height, int channels, void *data)
{


    int depth, pitch;





Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  int shift = (channels == 3) ? 8 : 0;
  rmask = 0xff000000 >> shift;
  gmask = 0x00ff0000 >> shift;
  bmask = 0x0000ff00 >> shift;
  amask = 0x000000ff >> shift;
#else // little endian, like x86
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = (channels == 3) ? 0 : 0xff000000;
#endif


if (channels == 3)
{
  depth = 24;
  pitch = 3*width; // 3 bytes per pixel * pixels per row
} else
{ //  (RGBA)
  depth = 32;
  pitch = 4*width;
}


SDL_Surface* surf = SDL_CreateRGBSurfaceFrom((void*)data, width, height, depth, pitch,rmask, gmask, bmask, amask);

//SDL_Surface* surf =SDL_CreateRGBSurfaceWithFormatFrom((void*)data,tex.width,tex.height,bpp,pitch,format);

if (surf == NULL)
{
    SDL_LogError(0,"Creating surface failed: %s", SDL_GetError());
    return (NULL);
}

return surf;
}

SDL_Surface *CreateSurfaceFromGraph(Graph *gr)
{
return CreateSurfaceFromData(gr->width,gr->height,gr->channels,gr->data);
}
SDL_Surface *CreateSurfaceFromTexture(Texture2D tex, void *data)
{


    int bytesPerpixel=3;
    int depth, pitch;

    if (tex.format == IR8G8B8A8)
        bytesPerpixel=4;




Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  int shift = (bytesPerpixel == 3) ? 8 : 0;
  rmask = 0xff000000 >> shift;
  gmask = 0x00ff0000 >> shift;
  bmask = 0x0000ff00 >> shift;
  amask = 0x000000ff >> shift;
#else // little endian, like x86
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = (bytesPerpixel == 3) ? 0 : 0xff000000;
#endif


if (bytesPerpixel == 3)
{
  depth = 24;
  pitch = 3*tex.width; // 3 bytes per pixel * pixels per row
} else
{ //  (RGBA)
  depth = 32;
  pitch = 4*tex.width;
}


SDL_Surface* surf = SDL_CreateRGBSurfaceFrom((void*)data, tex.width, tex.height, depth, pitch,rmask, gmask, bmask, amask);

//SDL_Surface* surf =SDL_CreateRGBSurfaceWithFormatFrom((void*)data,tex.width,tex.height,bpp,pitch,format);

if (surf == NULL)
{
    SDL_LogError(0,"Creating surface failed: %s", SDL_GetError());
    return (NULL);
}

return surf;
}

int AssetContainsGraph(Assets *a,int id)
{

    if (a)
    {
        if(a->Graphics.root)
        {
          Graph	 *current;
          current = a->Graphics.root;
          while (current != NULL)
          {
            if(current->id == id)
                return (1);
            current = current->next;
          }
        }
    }
    return (0);
}

void WriteString(SDL_RWops* file,const char* str)
{
    size_t len = SDL_strlen(str);
    SDL_RWwrite(file,str,1,len);
    //SDL_RWwrite(file,'\n',1,1 * sizeof(char));
}


void SaveAtlasText(Assets *a,const char* FileName)
{
         SDL_RWops* file= SDL_RWFromFile(FileName, "wt");


        if (file != NULL)
        {

                    {
                     char *save = (char*)mem_malloc(50 * sizeof(char));
                     sprintf(save, "%d\n",a->TextureAtlas.count);
                     WriteString(file,save);
                     mem_free(save);
                     }


                for (int i=0; i < a->TextureAtlas.count;i++)
                {
                     char *save = (char*)mem_malloc(100 * sizeof(char));
                     SDL_Rect r;
                     int id  = a->TextureAtlas.ids[i];
                     r.x=(int)a->TextureAtlas.clips[i].x;
                     r.y=(int)a->TextureAtlas.clips[i].y;
                     r.w=(int)a->TextureAtlas.clips[i].width;
                     r.h=(int)a->TextureAtlas.clips[i].height;
                     sprintf(save, "%d,%d,%d,%d,%d\n",id,r.x,r.y,r.w,r.h);
                     WriteString(file,save);
                     mem_free(save);
                 }

                SDL_Log( " Atlas Text file save successfully: [%s]", FileName);
        }
        SDL_RWclose(file);
}

char* ReadString(SDL_RWops* file)
{
    char *text = (char*) mem_malloc(50 *sizeof(char));
    bzero(text,50 *sizeof(char));
    int len=0;
    while (1)
    {
      char c;
      SDL_RWread(file, &c,1,sizeof(c));
      text[len] =c;
      if (c=='\n')
            break;
      len++;
    }
    return (text);
}

void LoadAtlasText(Assets *a,const char* FileName,const char* TextureName)
{
         SDL_RWops* file= SDL_RWFromFile(FileName, "rt");


        if (file != NULL)
        {

             char *count =fttrim(ReadString(file));


             int numAtls = atoi(count);
             a->TextureAtlas.count = numAtls;

             if (a->TextureAtlas.clips)
             {
                mem_free(a->TextureAtlas.clips);
                a->TextureAtlas.clips = NULL;
             }

             a->TextureAtlas.clips = (Rect*) mem_malloc(numAtls * sizeof(Rect));
             a->TextureAtlas.texture = LoadTexture(TextureName);




             SDL_Log( "Atlas Count %s %d ", count,numAtls);
             mem_free(count);


             for (int i=0 ; i<numAtls;i++)
             {
                 char *token;

                 char *line =fttrim(ReadString(file));
                 //SDL_Log( " Atlas  %d line %s ",i, line);

                   int pack[4];
                   int index=0;
                   for (token = strtok(line, ","); token; token = strtok(NULL, ","))
                    {
                      //  SDL_Log("token=%s", token);
                        pack[index] = atoi(token);
                        index++;
                    }
                    a->TextureAtlas.clips[i]= (Rect){pack[0],pack[1],pack[2],pack[3]};

                 mem_free(line);
             }




        SDL_Log( " Atlas Text file load successfully: [%s]", FileName);
        }
        SDL_RWclose(file);
}



int         GererateGraphicsAtlas(Assets *a,int padding, int GenerateTextureAtlas, int SaveAtlasFile)
{
        if(!a)
            return 0;
        int graphicsCount= a->Graphics.count;
        if (graphicsCount<=1)
            return 0;



        SDL_Log("[ATLAS] Build from %d images",graphicsCount);

        // 1 PASS
        Graph **chars = (Graph**) mem_malloc(graphicsCount* sizeof(Graph));

        float requiredArea = 0;
        if (a->Graphics.root)
        {
          Graph	 *current;
          current = a->Graphics.root;
          int index = 0;
          while (current != NULL)
          {
            requiredArea += ((current->width + 2 *padding)*(current->height + 2 * padding));
            chars[index] = current;
            index++;
            current = current->next;
          }
        }

        if (!GenerateTextureAtlas)
            SaveAtlasFile=0;


        float guessSize = sqrtf(requiredArea)*1.4f;
        int imageSize = (int)powf(2, ceilf(logf((float)guessSize)/logf(2)));    // Calculate next POT


        SDL_Log("[ATLAS] Create  image (%d,%d) atalas ",imageSize,imageSize);

        SDL_Surface * atlas = SDL_CreateRGBSurfaceWithFormat(0,imageSize,imageSize,32,SDL_PIXELFORMAT_RGBA32);


        Rect *recs = (Rect *)mem_malloc(graphicsCount*sizeof(Rect));




        stbrp_context *context = (stbrp_context *)mem_malloc(sizeof(*context));
        stbrp_node *nodes = (stbrp_node *)mem_malloc(graphicsCount*sizeof(*nodes));

        stbrp_init_target(context, imageSize, imageSize, nodes, graphicsCount);
        stbrp_rect *rects = (stbrp_rect *)mem_malloc(graphicsCount*sizeof(stbrp_rect));


        for (int i = 0; i < graphicsCount; i++)
        {
            rects[i].id = i;
            rects[i].w = chars[i]->width  + 2*padding;
            rects[i].h = chars[i]->height + 2*padding;
        }

         stbrp_pack_rects(context, rects, graphicsCount);

         if (a->TextureAtlas.clips)
         {
            mem_free(a->TextureAtlas.clips);
            a->TextureAtlas.clips=NULL;
         }

         a->TextureAtlas.clips = (Rect*) mem_malloc(graphicsCount * sizeof(Rect));
         bzero(a->TextureAtlas.clips,graphicsCount * sizeof(Rect));
         a->TextureAtlas.ids = (int*) mem_malloc(graphicsCount * sizeof(int));
         bzero(a->TextureAtlas.ids,graphicsCount * sizeof(int));
         a->TextureAtlas.count = graphicsCount;


  clock_t t;
  t = clock();

       for (int i = 0; i < graphicsCount; i++)
        {
            // It return image rectangles in atlas
            recs[i].x = rects[i].x + (float)padding;
            recs[i].y = rects[i].y + (float)padding;
            recs[i].width  = (float)chars[i]->width;
            recs[i].height = (float)chars[i]->height;

            a->TextureAtlas.clips[i] = (Rect){recs[i].x,recs[i].y,recs[i].width,recs[i].height};
            a->TextureAtlas.ids[i] = chars[i]->id;

            if (rects[i].was_packed)
            {
                if (GenerateTextureAtlas)
                {
                    Graph *gr = chars[i];
                             SDL_Surface *surface =   CreateSurfaceFromGraph(gr);
                             SDL_Rect dstrect;
                             dstrect.x = (int)recs[i].x + padding;
                             dstrect.y = (int)recs[i].y + padding;
                             dstrect.w = rects[i].w;
                             dstrect.h = rects[i].h;
                             SDL_BlitSurface(surface ,NULL, atlas, &dstrect);

                            // char *save = (char*)malloc(13 * sizeof(char));
                            // sprintf(save, "save_%d.bmp", i);
                            // SDL_SaveBMP(surface,save);
                           // free(save);


                             SDL_FreeSurface(surface);


             }
            }
            else  SDL_LogError(0, "[ATLAS]: Failed to package graphic (%i)", i);
        }


     t = clock() - t;
     double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
     SDL_Log("Surface copy  took %f seconds to execute \n", time_taken);


        mem_free(chars);
        mem_free(rects);
        mem_free(nodes);
        mem_free(context);
        mem_free(recs);

        a->TextureAtlas.texture = CreateTexture(atlas->w,atlas->h,atlas->pixels,4);

        if (SaveAtlasFile==1)
        {
         //SDL_SaveBMP(atlas,"atlas.bmp");
         //SavePixelsDataFile(atlas->w,atlas->h,IR8G8B8A8,atlas->pixels,"Atlas.png");
         stbi_write_png("Atlas.png",atlas->w,atlas->h,4,atlas->pixels,imageSize*4);

        }







        SDL_FreeSurface(atlas);
        return 1;
}

Graph *GetAssetGraph(Assets *a,int id)
{

    if (a)
    {
        if(a->Graphics.root)
        {
          Graph	 *current;
          current = a->Graphics.root;
          while (current != NULL)
          {
            if(current->id == id)
                return (current);
            current = current->next;
          }
        }
    }
    return (NULL);
}



//**************************************************************MATH

Mat4 OrthoMatrix(double left, double right, double bottom, double top, double near, double far)
{
    Mat4 result = { 0 };

    float rl = (float)(right - left);
    float tb = (float)(top - bottom);
    float fn = (float)(far - near);

    result.m0 = 2.0f/rl;
    result.m1 = 0.0f;
    result.m2 = 0.0f;
    result.m3 = 0.0f;
    result.m4 = 0.0f;
    result.m5 = 2.0f/tb;
    result.m6 = 0.0f;
    result.m7 = 0.0f;
    result.m8 = 0.0f;
    result.m9 = 0.0f;
    result.m10 = -2.0f/fn;
    result.m11 = 0.0f;
    result.m12 = -((float)left + (float)right)/rl;
    result.m13 = -((float)top + (float)bottom)/tb;
    result.m14 = -((float)far + (float)near)/fn;
    result.m15 = 1.0f;

    return result;
}

Mat4 MultiplyMatrix(Mat4 left, Mat4 right)
{
    Mat4 result = { 0 };

    result.m0 = left.m0*right.m0 + left.m1*right.m4 + left.m2*right.m8 + left.m3*right.m12;
    result.m1 = left.m0*right.m1 + left.m1*right.m5 + left.m2*right.m9 + left.m3*right.m13;
    result.m2 = left.m0*right.m2 + left.m1*right.m6 + left.m2*right.m10 + left.m3*right.m14;
    result.m3 = left.m0*right.m3 + left.m1*right.m7 + left.m2*right.m11 + left.m3*right.m15;
    result.m4 = left.m4*right.m0 + left.m5*right.m4 + left.m6*right.m8 + left.m7*right.m12;
    result.m5 = left.m4*right.m1 + left.m5*right.m5 + left.m6*right.m9 + left.m7*right.m13;
    result.m6 = left.m4*right.m2 + left.m5*right.m6 + left.m6*right.m10 + left.m7*right.m14;
    result.m7 = left.m4*right.m3 + left.m5*right.m7 + left.m6*right.m11 + left.m7*right.m15;
    result.m8 = left.m8*right.m0 + left.m9*right.m4 + left.m10*right.m8 + left.m11*right.m12;
    result.m9 = left.m8*right.m1 + left.m9*right.m5 + left.m10*right.m9 + left.m11*right.m13;
    result.m10 = left.m8*right.m2 + left.m9*right.m6 + left.m10*right.m10 + left.m11*right.m14;
    result.m11 = left.m8*right.m3 + left.m9*right.m7 + left.m10*right.m11 + left.m11*right.m15;
    result.m12 = left.m12*right.m0 + left.m13*right.m4 + left.m14*right.m8 + left.m15*right.m12;
    result.m13 = left.m12*right.m1 + left.m13*right.m5 + left.m14*right.m9 + left.m15*right.m13;
    result.m14 = left.m12*right.m2 + left.m13*right.m6 + left.m14*right.m10 + left.m15*right.m14;
    result.m15 = left.m12*right.m3 + left.m13*right.m7 + left.m14*right.m11 + left.m15*right.m15;

    return result;
}

Mat4 TranslateMatrix(float x, float y, float z)
{
    Mat4 result = { 1.0f, 0.0f, 0.0f, x,
                      0.0f, 1.0f, 0.0f, y,
                      0.0f, 0.0f, 1.0f, z,
                      0.0f, 0.0f, 0.0f, 1.0f };

    return result;
}

Mat4 RotateMatrix(Vec3 axis, float angle)
{
    Mat4 result = { 0 };

    float x = axis.x, y = axis.y, z = axis.z;

    float lengthSquared = x*x + y*y + z*z;

    if ((lengthSquared != 1.0f) && (lengthSquared != 0.0f))
    {
        float ilength = 1.0f/sqrtf(lengthSquared);
        x *= ilength;
        y *= ilength;
        z *= ilength;
    }

    float sinres = sinf(angle);
    float cosres = cosf(angle);
    float t = 1.0f - cosres;

    result.m0 = x*x*t + cosres;
    result.m1 = y*x*t + z*sinres;
    result.m2 = z*x*t - y*sinres;
    result.m3 = 0.0f;

    result.m4 = x*y*t - z*sinres;
    result.m5 = y*y*t + cosres;
    result.m6 = z*y*t + x*sinres;
    result.m7 = 0.0f;

    result.m8 = x*z*t + y*sinres;
    result.m9 = y*z*t - x*sinres;
    result.m10 = z*z*t + cosres;
    result.m11 = 0.0f;

    result.m12 = 0.0f;
    result.m13 = 0.0f;
    result.m14 = 0.0f;
    result.m15 = 1.0f;

    return result;
}

 Mat4 ScaleMatrix(float x, float y, float z)
{
    Mat4 result = { x, 0.0f, 0.0f, 0.0f,
                      0.0f, y, 0.0f, 0.0f,
                      0.0f, 0.0f, z, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f };

    return result;
}

Mat4 GetCamerTransform2D(Camera2D camera)
{
    Mat4 matTransform = { 0 };
    Mat4 matOrigin = TranslateMatrix(-camera.position.x, -camera.position.y, 0.0f);
    Mat4 matRotation = RotateMatrix((Vec3){ 0.0f, 0.0f, 1.0f }, camera.rotation*DEG2RAD);
    Mat4 matScale = ScaleMatrix(camera.zoom, camera.zoom, 1.0f);
    Mat4 matTranslation = TranslateMatrix(camera.offset.x, camera.offset.y, 0.0f);

    matTransform = MultiplyMatrix(MultiplyMatrix(matOrigin, MultiplyMatrix(matScale, matRotation)), matTranslation);

    return matTransform;
}


void UpdateCamera(Camera2D *camera)
{
    camera->frustum = OrthoMatrix(
    camera->bound.x,
    camera->bound.width,
    camera->bound.height,
    camera->bound.y,
    -0.5f, 1.5f);
    camera->transform = GetCamerTransform2D(*camera);
    glViewport(camera->bound.x, camera->bound.y, camera->bound.width,camera->bound.height);


}

void ClearCamera(Camera2D camera)
{

    float r, g, b, a;

    r = (camera.color.r/255.0f);
    g = (camera.color.g/255.0f);
    b = (camera.color.b/255.0f);
    a = (camera.color.a/255.0f);


    glClearColor(r,g,b,1.0f);
    glClear( GL_COLOR_BUFFER_BIT  );

}
//*****************************************************************SHADER


unsigned int loadShaderProgram(unsigned int vShaderId, unsigned int fShaderId)
{
    unsigned int program = 0;

    GLint success = 0;
    program = glCreateProgram();

    glAttachShader(program, vShaderId);
    glAttachShader(program, fShaderId);

    //  attribute shader locations must be binded before linking
    glLinkProgram(program);


    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (success == GL_FALSE)
    {
        trace( "SHADER: [ID %i] Failed to link shader program", program);

        int maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        if (maxLength > 0)
        {
            int length = 0;
            char *log = (char *)mem_calloc(maxLength, sizeof(char));
            glGetProgramInfoLog(program, maxLength, &length, log);
            trace("SHADER: [ID %i] Link error: %s", program, log);
            mem_free(log);
        }

        glDeleteProgram(program);

        program = 0;
    }
    else
        SDL_Log("SHADER: [ID %i] Program shader loaded successfully", program);


    return program;
}



unsigned int compileShader(const char *shaderCode, int type)
{
    unsigned int shader = 0;
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderCode, NULL);

    GLint success = 0;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE)
    {
        switch (type)
        {
            case GL_VERTEX_SHADER: trace("SHADER: [ID %i] Failed to compile vertex shader code", shader); break;
            case GL_FRAGMENT_SHADER: trace("SHADER: [ID %i] Failed to compile fragment shader code", shader); break;
            default: break;
        }

        int maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        if (maxLength > 0)
        {
            int length = 0;
            char *log = (char *)mem_calloc(maxLength, sizeof(char));
            glGetShaderInfoLog(shader, maxLength, &length, log);
            trace("SHADER: [ID %i] Compile error: %s", shader, log);
            mem_free(log);
        }
    }
    else
    {
        switch (type)
        {
            case GL_VERTEX_SHADER: SDL_Log("SHADER: [ID %i] Vertex shader compiled successfully", shader); break;
            case GL_FRAGMENT_SHADER: SDL_Log("SHADER: [ID %i] Fragment shader compiled successfully", shader); break;
            default: break;
        }
    }
    return shader;
}

unsigned int loadShaderCode(const char *vsCode, const char *fsCode)
{
    unsigned int id = 0;

    unsigned int vertexShaderId = 0;
    unsigned int fragmentShaderId = 0;

    if (vsCode != NULL) vertexShaderId = compileShader(vsCode, GL_VERTEX_SHADER);
    if (fsCode != NULL) fragmentShaderId = compileShader(fsCode, GL_FRAGMENT_SHADER);

    id = loadShaderProgram(vertexShaderId, fragmentShaderId);
    if (id == 0)
        trace("SHADER: Failed to load  shader code");


    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
    return id;
}

Shader CreateShader(const char *vertexSource, const char *fragmentSource)
{
    Shader shader = { 0 };
    shader.id =-1;
    shader.locs= CreateHashMap();
    shader.id = loadShaderCode(vertexSource,fragmentSource);
    return shader;
}

Shader LoadShader(const char *vsFileName, const char *fsFileName)
{
   Shader shader = { 0 };
   shader.id =-1;

    char *vShaderStr = NULL;
    char *fShaderStr = NULL;

    if (vsFileName != NULL) vShaderStr = LoadTextFile(vsFileName);
    if (fsFileName != NULL) fShaderStr = LoadTextFile(fsFileName);


    shader.locs= CreateHashMap();
    shader.id = loadShaderCode(vShaderStr,fShaderStr);
    glUseProgram(shader.id);

    mem_free(vShaderStr);
    mem_free(fShaderStr);
    return shader;
}


void SetShaderMatrix4(Shader shader,const char* name, Mat4 mat)
{
    float result[16];
    result[0] = mat.m0;
    result[1] = mat.m1;
    result[2] = mat.m2;
    result[3] = mat.m3;
    result[4] = mat.m4;
    result[5] = mat.m5;
    result[6] = mat.m6;
    result[7] = mat.m7;
    result[8] = mat.m8;
    result[9] = mat.m9;
    result[10] = mat.m10;
    result[11] = mat.m11;
    result[12] = mat.m12;
    result[13] = mat.m13;
    result[14] = mat.m14;
    result[15] = mat.m15;

    int loc = GetHashMapInt (shader.locs,name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, result);
}

void   SetShaderInt(Shader shader,const char* name, int value)
{
    int loc = GetHashMapInt (shader.locs,name);
     glUniform1i(loc,  value);
}

void   SetShaderFloat(Shader shader,const char* name, float value)
{
    int loc = GetHashMapInt (shader.locs,name);
     glUniform1f(loc,  (int)value);
}

int    GetShaderUniform(Shader shader,const char *name)
{
    int loc =glGetUniformLocation(shader.id, name);
    if (loc == -1)
    {
        SDL_LogError(0,"Fail Get Uniform(%s) from Shader [%d]",name,shader.id);
    }
    return loc;
}

void    AddShaderUniform(Shader shader,const char *name)
{
    int loc =GetShaderUniform(shader, name);
    if (loc >= 0)
    {
        SDL_Log("Add  Uniform(%s) to Shader [%d] position [%d] ",name,shader.id,loc);
        AddHashMapInt(shader.locs,name,loc);
    }

}

void FreeShader(Shader shader)
{
         if (shader.locs)
            FreeHashMap(shader.locs);
         glDeleteProgram(shader.id);
         SDL_Log("SHADER: [ID %i] Unloaded shader program", shader.id);

}

void ShaderBind(Shader shader)
{
        if(shader.id>0)
            glUseProgram(shader.id);
}

int powerOfTwo( int value )
{

	int result = 1 ;

	while ( result < value )
		result *= 2 ;

	return result ;
}


//********************************************************************TEXTURE2D

void GetGlTextureFormats(int format, unsigned int *glInternalFormat, unsigned int *glFormat, unsigned int *glType)
{
    *glInternalFormat = 0;
    *glFormat = 0;
    *glType = 0;

    switch (format)
    {

     /*   // NOTE: on OpenGL ES 2.0 (WebGL), internalFormat must match format and options allowed are: GL_LUMINANCE, GL_RGB, GL_RGBA
        case IGRAYSCALE: *glInternalFormat = GL_LUMINANCE; *glFormat = GL_LUMINANCE; *glType = GL_UNSIGNED_BYTE; break;
        case IGRAY_ALPHA: *glInternalFormat = GL_LUMINANCE_ALPHA; *glFormat = GL_LUMINANCE_ALPHA; *glType = GL_UNSIGNED_BYTE; break;
        case IR5G6B5: *glInternalFormat = GL_RGB; *glFormat = GL_RGB; *glType = GL_UNSIGNED_SHORT_5_6_5; break;
        case IR8G8B8: *glInternalFormat = GL_RGB; *glFormat = GL_RGB; *glType = GL_UNSIGNED_BYTE; break;
        case IR5G5B5A1: *glInternalFormat = GL_RGBA; *glFormat = GL_RGBA; *glType = GL_UNSIGNED_SHORT_5_5_5_1; break;
        case IR4G4B4A4: *glInternalFormat = GL_RGBA; *glFormat = GL_RGBA; *glType = GL_UNSIGNED_SHORT_4_4_4_4; break;
        case IR8G8B8A8: *glInternalFormat = GL_RGBA; *glFormat = GL_RGBA; *glType = GL_UNSIGNED_BYTE; break;
  */

        case IGRAYSCALE: *glInternalFormat = GL_R8; *glFormat = GL_RED; *glType = GL_UNSIGNED_BYTE; break;
        case IGRAY_ALPHA: *glInternalFormat = GL_RG8; *glFormat = GL_RG; *glType = GL_UNSIGNED_BYTE; break;
        case IR5G6B5: *glInternalFormat = GL_RGB565; *glFormat = GL_RGB; *glType = GL_UNSIGNED_SHORT_5_6_5; break;
        case IR8G8B8: *glInternalFormat = GL_RGB8; *glFormat = GL_RGB; *glType = GL_UNSIGNED_BYTE; break;
        case IR5G5B5A1: *glInternalFormat = GL_RGB5_A1; *glFormat = GL_RGBA; *glType = GL_UNSIGNED_SHORT_5_5_5_1; break;
        case IR4G4B4A4: *glInternalFormat = GL_RGBA4; *glFormat = GL_RGBA; *glType = GL_UNSIGNED_SHORT_4_4_4_4; break;
        case IR8G8B8A8: *glInternalFormat = GL_RGBA8; *glFormat = GL_RGBA; *glType = GL_UNSIGNED_BYTE; break;
        }

}
Texture2D CreateTexture(Uint32 width, Uint32 height, Uint8* data, Uint8 channels)
{

    Texture2D tex = {0};
    tex.ID=-1;
    tex.width  = width;
    tex.height = height;




    if (channels == 1) tex.format = IGRAYSCALE;
    else if (channels == 2) tex.format = IGRAY_ALPHA;
    else if (channels == 3) tex.format = IR8G8B8;
    else if (channels == 4) tex.format = IR8G8B8A8;

    unsigned int glInternalFormat, glFormat, glType;
    GetGlTextureFormats(tex.format, &glInternalFormat, &glFormat, &glType);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &tex.ID);
    glBindTexture(GL_TEXTURE_2D, tex.ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, width, height, 0, glFormat, glType, data);
    glGenerateMipmap(GL_TEXTURE_2D);


    glBindTexture(GL_TEXTURE_2D, 0);


      if (tex.ID > 0) SDL_Log("TEXTURE: [ID %i] Texture2D created successfully (%ix%i)", tex.ID, tex.width, tex.height);
     else SDL_LogError(0, "TEXTURE: Failed to create Texture2D");


    return tex;
}

Texture2D CreateTextureFromGraph(Graph *gr)
{
  return CreateTexture(gr->width,gr->height,gr->data,gr->channels);
}


Texture2D LoadTexture(const char* Filename)
{
Texture2D tex = {0};


  int width, height, nrChannels;
//
//    stbi_set_flip_vertically_on_load(1);
    unsigned char *data = stbi_load(Filename, &width, &height, &nrChannels, 0);
    if (data)
    {
         tex = CreateTexture(width, height,data,nrChannels);
    }
    else
        SDL_LogError(0, "Failed to load texture");
    stbi_image_free(data);
   return tex;
}


Texture2D LoadPngTexture(const char* Filename)
{

Texture2D tex = {0};

return tex;
}

Texture2D LoadXpmTexture(const char* Filename)
{
Texture2D tex = {0};

return tex;
}

void FreeTexture(Texture2D tex)
{
   if (tex.ID>0)
   {
    glDeleteTextures(1, &tex.ID);
     SDL_Log("TEXTURE: [ID %i] Unloaded Texture 2D ", tex.ID);
   }
}
void BindTexture(Texture2D tex)
{
    if (tex.ID>=0)
   {
     glActiveTexture(GL_TEXTURE0);
     glBindTexture(GL_TEXTURE_2D,tex.ID);

   }
}

//************************************************************************************************************************
PrimitivesBatch *CreatePrimitivesBatch(int capacity)
{
   PrimitivesBatch *batch;



   batch = (PrimitivesBatch*) mem_malloc(sizeof(PrimitivesBatch));

   static const GLchar* vertex_shader_source =
    "#version 300 es\n"
      "precision mediump float;\n"
    "in vec3 position;\n"
    "in vec4 vColor;\n"
    "out vec4 fColor;\n"
    "uniform mat4 projection;\n"
    "uniform mat4 view;\n"
    "void main() {\n"
    "    gl_Position = projection * view * vec4(position, 1.0f);\n"
    "    fColor = vColor;\n"
    "}\n";
static const GLchar* fragment_shader_source =
    "#version 300 es\n"
      "precision mediump float;\n"
    "in vec4 fColor;\n"
    "out vec4 color;\n"
    "void main() {\n"
    "    color = vec4(fColor);\n"
    "}\n";

    batch->shader = CreateShader(vertex_shader_source,fragment_shader_source);
    AddShaderUniform(batch->shader,"projection");
    AddShaderUniform(batch->shader,"view");


   batch->capacity = capacity;

   batch->vertexStrideSize = (3 + 4) * sizeof(float);
   batch->maxCapacity = capacity * batch->vertexStrideSize;




    batch->Lines.vextex = (float* ) mem_malloc(batch->maxCapacity);
    bzero(batch->Lines.vextex,batch->maxCapacity);
    batch->Lines.currentBatchSize=0;
    batch->Lines.index=0;


    batch->Solid.vextex = (float* ) mem_malloc(batch->maxCapacity);
    bzero(batch->Solid.vextex,batch->maxCapacity);
    batch->Solid.currentBatchSize=0;
    batch->Solid.index=0;

    //Lines
    // vbo
    glGenBuffers(1, &batch->Lines.VBO);
    glBindBuffer(GL_ARRAY_BUFFER,  batch->Lines.VBO);
    glBufferData(GL_ARRAY_BUFFER,  batch->maxCapacity, batch->Lines.vextex, GL_DYNAMIC_DRAW);



    // Buffers.
    glGenVertexArrays(1, &batch->Lines.VAO);
    glBindVertexArray(batch->Lines.VAO);


    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  batch->vertexStrideSize, (void*)0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,  batch->vertexStrideSize, (void*)(3 * sizeof(float)));
    glBindVertexArray(0);


    //Triangles
    glGenVertexArrays(1, &batch->Solid.VAO);
    glBindVertexArray(batch->Solid.VAO);

    glGenBuffers(1, &batch->Solid.VBO);
    glBindBuffer(GL_ARRAY_BUFFER,  batch->Solid.VBO);
    glBufferData(GL_ARRAY_BUFFER,  batch->maxCapacity, batch->Solid.vextex, GL_DYNAMIC_DRAW);



    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  batch->vertexStrideSize, (void*)0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,  batch->vertexStrideSize, (void*)(3 * sizeof(float)));

    glBindVertexArray(0);
    return batch;
}



void RenderLine(PrimitivesBatch* batch, float x1, float y1, float x2,float y2,Color c)
{
    if (!batch)
        return;

    //  vertices  = (3 position + 4 color)  +  (3 position + 4 color) = 14

    int index = batch->Lines.currentBatchSize * 14 ;

    float r, g, b, a;

    r = (c.r/255.0f);
    g = (c.g/255.0f);
    b = (c.b/255.0f);
    a = (c.a/255.0f);




    //1 vertex

    //position
    batch->Lines.vextex[index++] = (float)x1;
    batch->Lines.vextex[index++] = (float)y1;
    batch->Lines.vextex[index++] = 0.0f;
    //color
    batch->Lines.vextex[index++] = r;
    batch->Lines.vextex[index++] = g;
    batch->Lines.vextex[index++] = b;
    batch->Lines.vextex[index++] = a;


    //2 vertex
    batch->Lines.vextex[index++] = (float)x2;
    batch->Lines.vextex[index++] = (float)y2;
    batch->Lines.vextex[index++] = 0.0f;

        //color
    batch->Lines.vextex[index++] = r;
    batch->Lines.vextex[index++] = g;
    batch->Lines.vextex[index++] = b;
    batch->Lines.vextex[index++] = a;




    batch->Lines.currentBatchSize++;
    batch->Lines.index += 14; //7 * 2;



}

void RenderRectangle(PrimitivesBatch* batch, float x, float y, float x2,float y2, Color c)
{
    RenderLine(batch,x, y, x2, y, c);
	RenderLine(batch,x, y2, x2, y2, c);
	RenderLine(batch,x, y, x, y2,  c);
	RenderLine(batch,x2, y, x2, y2, c);
}


void RenderCircle(PrimitivesBatch* batch, float x, float y, int segments, float radius, Color c)
{
        if (!batch)
            return;

        float  angle = 2.0f * 3.1415926f / segments;
	    float  cos = cosf(angle);
	    float sin = sinf(angle);
		float cx = radius;
		float cy = 0;


    for (int i=0; i < segments;i++)
    {
                float x1 = (float)x + cx;
                float y1 = (float)y + cy;
				float temp = cx;
				cx = cos * cx - sin * cy;
				cy = sin * temp + cos * cy;
			    float x2 = (float)x + cx;
                float y2 = (float)y + cy;
                RenderLine(batch,x1, y1, x2, y2, c);
    }
    //RenderLine(batch,x + cx, y + cy, x, y, c); // line from center to border ;)
}



void RenderTriangle(PrimitivesBatch* batch, float x1, float y1, float x2,float y2,float x3,float y3, Color c)
{
    if (!batch)
        return;

    //  vertices  = (3 position + 4 color)  +  (3 position + 4 color) +  (3 position + 4 color) = 21
    int index = batch->Solid.currentBatchSize * 21 ;

    float r, g, b, a;

    r = (c.r/255.0f);
    g = (c.g/255.0f);
    b = (c.b/255.0f);
    a = (c.a/255.0f);





    //1 vertex

    //position
    batch->Solid.vextex[index++] = (float)x3;
    batch->Solid.vextex[index++] = (float)y3;
    batch->Solid.vextex[index++] = 0.0f;


    batch->Solid.vextex[index++] = r;
    batch->Solid.vextex[index++] = g;
    batch->Solid.vextex[index++] = b;
    batch->Solid.vextex[index++] = a;


    //2 vertex
    batch->Solid.vextex[index++] = (float)x2;
    batch->Solid.vextex[index++] = (float)y2;
    batch->Solid.vextex[index++] = 0.0f;


    batch->Solid.vextex[index++] = r;
    batch->Solid.vextex[index++] = g;
    batch->Solid.vextex[index++] = b;
    batch->Solid.vextex[index++] = a;


  //3 vertex
    batch->Solid.vextex[index++] = (float)x1;
    batch->Solid.vextex[index++] = (float)y1;
    batch->Solid.vextex[index++] = 0.0f;


    batch->Solid.vextex[index++] = r;
    batch->Solid.vextex[index++] = g;
    batch->Solid.vextex[index++] = b;
    batch->Solid.vextex[index++] = a;



    batch->Solid.currentBatchSize++;
    batch->Solid.index += 21 ; //7 * 3;

/*
    RenderLine(batch,x1,y1,x2,y2,RED);
    RenderLine(batch,x2,y2,x3,y3,RED);
    RenderLine(batch,x3,y3,x1,y1,RED);
*/
}


void RenderSolidRectangle(PrimitivesBatch* batch, float x, float y, float width,float height, Color c)
{
	RenderTriangle(batch,
	 x,y,
	 x + width, y,
	 x + width, y + height,c);
	RenderTriangle(batch,
	 x + width, y + height,
     x, y + height,
	 x , y ,c);
}

void RenderSolidCircle(PrimitivesBatch* batch, float x, float y, int segments, float radius, Color c)
{
        if (!batch)
            return;

        float  angle = 2.0f * 3.1415926f / segments;
	    float  cos = cosf(angle);
	    float sin = sinf(angle);
		float cx = radius;
		float cy = 0;


        for (int i=0; i < segments;i++)
        {

                    float x1 = (float)x;
                    float y1 = (float)y;

                    float x2 = (float)x + cx;
                    float y2 = (float)y + cy;

                    float temp = cx;
                    cx = cos * cx - sin * cy;
                    cy = sin * temp + cos * cy;

                    float x3 = (float)x + cx;
                    float y3 = (float)y + cy;

                    RenderTriangle(batch,x3,y3,x2,y2,x1,y1,c);



        }
    //RenderLine(batch,x + cx, y + cy, x, y, c); // line from center to border ;)
}

void BeginPrimitivesBatch(PrimitivesBatch* batch)
{
    if (!batch)
         return;

    if (!batch->Lines.vextex || !batch->Solid.vextex)
        return;




    batch->Lines.currentBatchSize=0;
    batch->Solid.currentBatchSize=0;
    batch->Lines.index=0;
    batch->Solid.index=0;

}

void EndPrimitivesBatch(PrimitivesBatch* batch,Camera2D camera)
{
    if(!batch)
        return;

   if (!batch->Lines.vextex || !batch->Solid.vextex)
       return;

 //   glDisable(GL_CULL_FACE);
 glDisable(GL_BLEND);

    ShaderBind (batch->shader);
    SetShaderMatrix4(batch->shader,"projection",camera.frustum);
    SetShaderMatrix4(batch->shader,"view",camera.transform);


    if (batch->Lines.currentBatchSize > batch->maxCapacity/batch->vertexStrideSize)
    {
            batch->Lines.currentBatchSize = 0;
    }



     if (batch->Solid.currentBatchSize>0)
     {
        glBindVertexArray(batch->Solid.VAO);
        glBindBuffer(GL_ARRAY_BUFFER,batch->Solid.VBO);
        glBufferSubData(GL_ARRAY_BUFFER,0, (batch->Solid.currentBatchSize * 3 ) * batch->vertexStrideSize , batch->Solid.vextex);
        glDrawArrays(GL_TRIANGLES,  0 ,batch->Solid.currentBatchSize * 3);
     }

    if (batch->Lines.currentBatchSize>0)
    {
    glBindVertexArray(batch->Lines.VAO);
    glBindBuffer(GL_ARRAY_BUFFER,batch->Lines.VBO);
    glBufferSubData(GL_ARRAY_BUFFER,0, (batch->Lines.currentBatchSize * 2 ) * batch->vertexStrideSize , batch->Lines.vextex);
    glDrawArrays(GL_LINES,  0 , batch->Lines.currentBatchSize * 2);
    }



   // glBindVertexArray(0);
  //  glBindBuffer(GL_ARRAY_BUFFER, 0);

}


void FreePrimitivesBatch(PrimitivesBatch *batch)
{
    if (!batch)
        return;

    if(batch->Lines.vextex)
        mem_free(batch->Lines.vextex);


    if (batch->Solid.vextex)
        mem_free(batch->Solid.vextex);

    FreeShader(batch->shader);
    glDeleteBuffers(1, &batch->Lines.VBO);
    glDeleteVertexArrays(1, &batch->Lines.VAO);
    glDeleteBuffers(1, &batch->Solid.VBO);
    glDeleteVertexArrays(1, &batch->Solid.VAO);
    mem_free(batch);
}



SpriteBatch *CreateSpriteBatch(int capacity)
{
   SpriteBatch *batch;



   batch = (SpriteBatch*) mem_malloc(sizeof(SpriteBatch));
   bzero(batch,sizeof(SpriteBatch));

   static const GLchar* vertex_shader_source =
    "#version 300 es\n"
      "precision mediump float;\n"
    "layout (location = 0) in vec3 vPosition;\n"
    "layout (location = 1) in vec2 vTexCoords;\n"
    "layout (location = 2) in vec4 vColor;\n"
    "out vec2 fTexCoords;\n"
    "out vec4 fColor;\n"
    "uniform mat4 projection;\n"
    "uniform mat4 view;\n"
    "void main() {\n"
    "    gl_Position =   projection * view * vec4(vPosition, 1.0f);\n"
    "    fColor = vColor;\n"
    "    fTexCoords = vec2(vTexCoords.x, vTexCoords.y);\n"
    "}\n";


static const GLchar* fragment_shader_source =
    "#version 300 es\n"
      "precision mediump float;\n"
    "out vec4 FragColor;\n"
    "in vec2 fTexCoords;\n"
    "in vec4 fColor;\n"
    "uniform sampler2D texture1;\n"
    "void main() {\n"
    "    FragColor =  fColor *  texture(texture1, fTexCoords);\n"
    "}\n";



    bzero(&batch->currentBaseTexture,sizeof(Texture2D));
    batch->currentBaseTexture.ID=-1;



   // SDL_Log("%d %d  - %d  %d",batch->currentBaseTexture.width,batch->currentBaseTexture.height,batch->currentBaseTexture.ID,batch->currentBaseTexture.format);


    batch->shader = CreateShader(vertex_shader_source,fragment_shader_source);
    ShaderBind(batch->shader);
    AddShaderUniform(batch->shader,"projection");
    AddShaderUniform(batch->shader,"view");
    AddShaderUniform(batch->shader,"texture1");
    SetShaderInt(batch->shader,"texture1",0);


       batch->capacity = capacity;

       batch->countDrawCalls=0;
       batch->countTextureBind=0;

       batch->vertexStrideSize = (3 + 2 + 4) * sizeof(float);
       batch->maxCapacity = capacity * batch->vertexStrideSize;




    batch->vextex = (float* ) mem_malloc(batch->maxCapacity);
    bzero(batch->vextex,batch->maxCapacity);
    batch->currentBatchSize=0;
    batch->index=0;

    glGenVertexArrays(1, &batch->VAO);
    glBindVertexArray(batch->VAO);

    // vbo
    glGenBuffers(1, &batch->VBO);
    glBindBuffer(GL_ARRAY_BUFFER,  batch->VBO);
    glBufferData(GL_ARRAY_BUFFER,  batch->maxCapacity, batch->vextex, GL_DYNAMIC_DRAW);






    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  batch->vertexStrideSize, (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,  batch->vertexStrideSize, (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE,  batch->vertexStrideSize, (void*)(6 * sizeof(float)));
    glBindVertexArray(0);

    return batch;
}

void BeginSpriteBatch(SpriteBatch* batch, Camera2D camera)
{
    if (!batch)
         return;

    if (!batch->vextex)
        return;

    bzero(&batch->currentBaseTexture,sizeof(Texture2D));
    batch->currentBaseTexture.ID=-1;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
     //SDL_Log("%d %d  - %d  %d",batch->currentBaseTexture.width,batch->currentBaseTexture.height,batch->currentBaseTexture.ID,batch->currentBaseTexture.format);




 //   glDisable(GL_CULL_FACE);
    ShaderBind (batch->shader);
  //  Mat4 matProj = OrthoMatrix( 0.0f,800,650,0.0f, -1.5f, 1.5f);
  //  SetShaderMatrix4(batch->shader,"projection",matProj);

    SetShaderMatrix4(batch->shader,"projection",camera.frustum);
    SetShaderMatrix4(batch->shader,"view",camera.transform);
    SetShaderInt(batch->shader,"texture1",0);

    batch->currentBatchSize=0;
    batch->index=0;
    batch->countDrawCalls=0;
    batch->countTextureBind=0;


}


void FlushSpriteBatch(SpriteBatch* batch)
{

 if (batch->currentBatchSize>0)
 {

    if (batch->currentBaseTexture.ID != -1)
    {
        BindTexture(batch->currentBaseTexture);
        batch->countTextureBind+=1;
      //  SDL_Log("flush %d %d  - ID: %d  %d",batch->currentBaseTexture.width,batch->currentBaseTexture.height,batch->currentBaseTexture.ID,batch->currentBaseTexture.format);
    }

    glBindVertexArray(batch->VAO);
    glBindBuffer(GL_ARRAY_BUFFER,batch->VBO);
    glBufferSubData(GL_ARRAY_BUFFER,0, (batch->currentBatchSize * 3 ) * batch->vertexStrideSize , batch->vextex);
    glDrawArrays(GL_TRIANGLES,  0 , batch->currentBatchSize * 3);
    batch->countDrawCalls +=1;
 }


    batch->currentBatchSize=0;
    batch->index=0;
}

void SwitchSpriteTexture(SpriteBatch* batch, Texture2D tex)
{
    if (tex.ID != -1)
    {
        FlushSpriteBatch(batch);
        memcpy(&batch->currentBaseTexture, &tex, sizeof(Texture2D));
     //   SDL_Log("switch %d %d  - %d  %d",batch->currentBaseTexture.width,batch->currentBaseTexture.height,batch->currentBaseTexture.ID,batch->currentBaseTexture.format);
    }
}


void EndSpriteBatch(SpriteBatch* batch)
{
    if(!batch)
        return;

   if (!batch->vextex )
       return;


    if (batch->currentBatchSize > batch->maxCapacity/batch->vertexStrideSize)
    {
            batch->currentBatchSize = 0;
    }


    FlushSpriteBatch(batch);



    //SDL_Log("Draw Calls %d  texture %d",batch->countDrawCalls,batch->countTextureBind);
   // glBindVertexArray(0);
   // glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void FreeSpriteBatch(SpriteBatch *batch)
{
    if (!batch)
        return;

    if(batch->vextex)
        mem_free(batch->vextex);

    FreeShader(batch->shader);
    glDeleteBuffers(1, &batch->VBO);
    glDeleteVertexArrays(1, &batch->VAO);
    mem_free(batch);
}



void RenderTextureTriangle(SpriteBatch* batch,
 float x1, float y1, float ux1, float uy1,
 float x2, float y2, float ux2, float uy2,
 float x3, float y3, float ux3, float uy3,Color c)
{
    if (!batch)
        return;

    //  vertices  = (3 position + 2 texture coord + 4 color) + (3 position + 2 texture coord + 4 color) + (3 position + 2 texture coord + 4 color)   = 27
    int index = batch->currentBatchSize * 27 ;

    float r, g, b, a;

    r = (c.r/255.0f);
    g = (c.g/255.0f);
    b = (c.b/255.0f);
    a = (c.a/255.0f);





    //1 vertex

    //position
    batch->vextex[index++] = (float)x3;
    batch->vextex[index++] = (float)y3;
    batch->vextex[index++] = 0.0f;

    batch->vextex[index++] = (float)ux3;
    batch->vextex[index++] = (float)uy3;

    batch->vextex[index++] = r;
    batch->vextex[index++] = g;
    batch->vextex[index++] = b;
    batch->vextex[index++] = a;


    //2 vertex
    batch->vextex[index++] = (float)x2;
    batch->vextex[index++] = (float)y2;
    batch->vextex[index++] = 0.0f;

    batch->vextex[index++] = (float)ux2;
    batch->vextex[index++] = (float)uy2;

    batch->vextex[index++] = r;
    batch->vextex[index++] = g;
    batch->vextex[index++] = b;
    batch->vextex[index++] = a;


  //3 vertex
    batch->vextex[index++] = (float)x1;
    batch->vextex[index++] = (float)y1;
    batch->vextex[index++] = 0.0f;

    batch->vextex[index++] = (float)ux1;
    batch->vextex[index++] = (float)uy1;

    batch->vextex[index++] = r;
    batch->vextex[index++] = g;
    batch->vextex[index++] = b;
    batch->vextex[index++] = a;



    batch->currentBatchSize++;
    batch->index += 27 ; //7 * 3;


}

void RenderTexture(SpriteBatch* batch, Texture2D tex, float x, float y, float width,float height, Color c)
{


   if (batch->currentBaseTexture.ID != tex.ID)
   {
        SwitchSpriteTexture(batch,tex);
   }


	RenderTextureTriangle(batch,
	 x,y,                    0.0f,0.0f,
	 x + width, y,           1.0f,0.0f,
	 x + width, y + height,  1.0f,1.0f,
	 c);

	RenderTextureTriangle(batch,
	 x + width, y + height, 1.0f,1.0f,
     x, y + height,         0.0f,1.0f,
	 x , y ,                0.0f,0.0f,
	 c);
}

void RenderAssetTexture(Assets *a, SpriteBatch* batch, float x, float y, int id ,Color c)
{
    if (!a || !batch)
        return ;
    if(a->TextureAtlas.texture.ID==-1)
        return;
    Rect clip= GetAssetsAtlasClipById(a,id);
    RenderTextureClip(batch,a->TextureAtlas.texture,x,y,clip.height,clip.height,clip,c);
}

void RenderTextureClip(SpriteBatch* batch, Texture2D texture, float x, float y, float width,float height, Rect source,Color c)
{


   if (batch->currentBaseTexture.ID != texture.ID)
   {
        SwitchSpriteTexture(batch,texture);
   }

    float w  = (float)texture.width;
    float h  = (float)texture.height;


	RenderTextureTriangle(batch,
	 x,y,                    source.x/w                 ,source.y/h,
	 x + width, y,           (source.x + source.width)/w,source.y/h,
	 x + width, y + height,  (source.x + source.width)/w,(source.y + source.height)/h,
	 c);

	RenderTextureTriangle(batch,
	 x + width, y + height, (source.x + source.width)/w,(source.y + source.height)/h,
     x, y + height,          source.x/w,                (source.y + source.height)/h,
	 x , y ,                 source.x/w,                source.y/h,
	 c);
}

//*******************************************************************************************************************************************
PostProcessor *CreatePostProcessor( int width,  int height)
{
  PostProcessor * proc = (PostProcessor*) mem_malloc(sizeof(PostProcessor)) ;
  //bzero(proc,sizeof(PostProcessor));
  proc->Shake  =0;
  proc->Chaos  =0;
  proc->Blur=0;

  proc->Buffer.defaultFramebuffer=-1;
  proc->Buffer.colorRenderbuffer=-1;
  proc->Buffer.textureRenderbuffer=-1;


    glGenFramebuffers(1, &proc->Buffer.defaultFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, proc->Buffer.defaultFramebuffer);
    


    glGenRenderbuffers(1, &proc->Buffer.colorRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, proc->Buffer.colorRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height );
    //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, proc->Buffer.colorRenderbuffer); // attach MS render buffer object to framebuffer
    //if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    //    SDL_LogError(0,"POSTPROCESSOR: Failed to initialize Color BUffer");
    
    
    glGenRenderbuffers(1, &proc->Buffer.textureRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, proc->Buffer.textureRenderbuffer);
    proc->Buffer.texture = CreateTexture(width,height,NULL,3);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, proc->Buffer.texture.ID, 0); // attach texture to framebuffer as its color attachment
    //if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    //    SDL_LogError(0,"POSTPROCESSOR: Failed to initialize Texture BUffer");


    glBindFramebuffer(GL_FRAMEBUFFER, proc->Buffer.defaultFramebuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, proc->Buffer.colorRenderbuffer);
    //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_renderbuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, proc->Buffer.texture.ID, 0); // attach texture to framebuffer as its color attachment

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        SDL_LogError(0,"POSTPROCESSOR: Failed to initialize  BUffer");


    glBindFramebuffer(GL_FRAMEBUFFER, 0);



    proc->shader = LoadShader("assets/post_processing.vs","assets/post_processing.fs");
    ShaderBind(proc->shader);
    AddShaderUniform(proc->shader,"chaos");
    AddShaderUniform(proc->shader,"blur");
    AddShaderUniform(proc->shader,"shake");
    AddShaderUniform(proc->shader,"time");
    SetShaderInt(proc->shader,"scene",1);


//create view buffers , VBOS

// configure VAO/VBO
    unsigned int VBO;
    float vertices[] = {
        // pos        // tex
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,

        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f
    };
    glGenVertexArrays(1, &proc->Buffer.VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(proc->Buffer.VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

//config shader
 float offset = 1.0f / 300.0f;
    float offsets[9][2] = {
        { -offset,  offset  },  // top-left
        {  0.0f,    offset  },  // top-center
        {  offset,  offset  },  // top-right
        { -offset,  0.0f    },  // center-left
        {  0.0f,    0.0f    },  // center-center
        {  offset,  0.0f    },  // center - right
        { -offset, -offset  },  // bottom-left
        {  0.0f,   -offset  },  // bottom-center
        {  offset, -offset  }   // bottom-right
    };
    glUniform2fv(glGetUniformLocation(proc->shader.id, "offsets"), 9, (float*)offsets);
    float edge_kernel[9] = {
        -1.0f, -1.0f, -1.0f,
        -1.0f,  8.0f, -1.0f,
        -1.0f, -1.0f, -1.0f
    };
    glUniform1fv(glGetUniformLocation(proc->shader.id, "edge_kernel"), 9, edge_kernel);
    float blur_kernel[9] = {
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
        2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
    };
    glUniform1fv(glGetUniformLocation(proc->shader.id, "blur_kernel"), 9, blur_kernel);

    SDL_Log("[POSTPROCESSOR]: Created");
    return proc;
}

void BeginPostProcessor(PostProcessor *proc)
{
    if(proc)
    {
     glBindFramebuffer(GL_FRAMEBUFFER, proc->Buffer.defaultFramebuffer);
     glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
     glClear(GL_COLOR_BUFFER_BIT);
    }
}

void EndPostProcessor(PostProcessor *proc)
{
    if(proc)
    {
     glBindFramebuffer(GL_FRAMEBUFFER, 0); // binds both READ and WRITE framebuffer to default framebuffer
    }
}
void RenderPostProcessor(PostProcessor *proc,float time)
{
    if(proc)
    {
        ShaderBind(proc->shader);
        SetShaderFloat(proc->shader,"time", time);
        SetShaderInt(proc->shader,"blur",proc->Blur);
        SetShaderInt(proc->shader,"chaos",proc->Chaos);
        SetShaderInt(proc->shader,"shake",proc->Shake);

        // render textured quad
        glActiveTexture(GL_TEXTURE0);

        BindTexture(proc->Buffer.texture);

        glBindVertexArray(proc->Buffer.VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
}

void FreePostProcessor(PostProcessor *proc)
{
    if(proc)
    {
      if (proc->Buffer.defaultFramebuffer)
        {
            glDeleteFramebuffers(1, &proc->Buffer.defaultFramebuffer);
      
        }
        
        if (proc->Buffer.colorRenderbuffer)
        {
            glDeleteRenderbuffers(1, &proc->Buffer.colorRenderbuffer);
            
        }
		
		
		if( proc->Buffer.textureRenderbuffer )
		{
			glDeleteRenderbuffers(1, &proc->Buffer.textureRenderbuffer);
			
		}

     glDeleteBuffers(1, &proc->Buffer.VAO);
     FreeShader(proc->shader);

     FreeTexture(proc->Buffer.texture);
     mem_free(proc);
    }
}
//*******************************************************************************************GL
int GetPixelDataSize(int width, int height, int format)
{
    int dataSize = 0;       // Size in bytes
    int bpp = 0;            // Bits per pixel

    switch (format)
    {
        case IGRAYSCALE: bpp = 8; break;
        case IGRAY_ALPHA:
        case IR5G6B5:
        case IR5G5B5A1:
        case IR4G4B4A4: bpp = 16; break;
        case IR8G8B8A8: bpp = 32; break;
        case IR8G8B8: bpp = 24; break;
        default: break;
    }

    dataSize = width*height*bpp/8;  // Total data size in bytes

    return dataSize;
}


// Get color from a pixel from certain format
Color GetPixelColor(void *srcPtr, int format)
{
    Color color = { 0 };

    switch (format)
    {
        case IGRAYSCALE: color = (Color){ ((unsigned char *)srcPtr)[0], ((unsigned char *)srcPtr)[0], ((unsigned char *)srcPtr)[0], 255 }; break;
        case IGRAY_ALPHA: color = (Color){ ((unsigned char *)srcPtr)[0], ((unsigned char *)srcPtr)[0], ((unsigned char *)srcPtr)[0], ((unsigned char *)srcPtr)[1] }; break;
        case IR5G6B5:
        {
            color.r = (unsigned char)((((unsigned short *)srcPtr)[0] >> 11)*255/31);
            color.g = (unsigned char)(((((unsigned short *)srcPtr)[0] >> 5) & 0b0000000000111111)*255/63);
            color.b = (unsigned char)((((unsigned short *)srcPtr)[0] & 0b0000000000011111)*255/31);
            color.a = 255;

        } break;
        case IR5G5B5A1:
        {
            color.r = (unsigned char)((((unsigned short *)srcPtr)[0] >> 11)*255/31);
            color.g = (unsigned char)(((((unsigned short *)srcPtr)[0] >> 6) & 0b0000000000011111)*255/31);
            color.b = (unsigned char)((((unsigned short *)srcPtr)[0] & 0b0000000000011111)*255/31);
            color.a = (((unsigned short *)srcPtr)[0] & 0b0000000000000001)? 255 : 0;

        } break;
        case IR4G4B4A4:
        {
            color.r = (unsigned char)((((unsigned short *)srcPtr)[0] >> 12)*255/15);
            color.g = (unsigned char)(((((unsigned short *)srcPtr)[0] >> 8) & 0b0000000000001111)*255/15);
            color.b = (unsigned char)(((((unsigned short *)srcPtr)[0] >> 4) & 0b0000000000001111)*255/15);
            color.a = (unsigned char)((((unsigned short *)srcPtr)[0] & 0b0000000000001111)*255/15);

        } break;
        case IR8G8B8A8: color = (Color){ ((unsigned char *)srcPtr)[0], ((unsigned char *)srcPtr)[1], ((unsigned char *)srcPtr)[2], ((unsigned char *)srcPtr)[3] }; break;
        case IR8G8B8: color = (Color){ ((unsigned char *)srcPtr)[0], ((unsigned char *)srcPtr)[1], ((unsigned char *)srcPtr)[2], 255 }; break;

        default: break;
    }

    return color;
}

// Set pixel color formatted into destination pointer
void SetPixelColor(void *dstPtr, Color color, int format)
{
    switch (format)
    {
        case IGRAYSCALE:
        {
            // NOTE: Calculate grayscale equivalent color
            Vec3 coln = { (float)color.r/255.0f, (float)color.g/255.0f, (float)color.b/255.0f };
            unsigned char gray = (unsigned char)((coln.x*0.299f + coln.y*0.587f + coln.z*0.114f)*255.0f);

            ((unsigned char *)dstPtr)[0] = gray;

        } break;
        case IGRAY_ALPHA:
        {
            // NOTE: Calculate grayscale equivalent color
            Vec3 coln = { (float)color.r/255.0f, (float)color.g/255.0f, (float)color.b/255.0f };
            unsigned char gray = (unsigned char)((coln.x*0.299f + coln.y*0.587f + coln.z*0.114f)*255.0f);

            ((unsigned char *)dstPtr)[0] = gray;
            ((unsigned char *)dstPtr)[1] = color.a;

        } break;
        case IR5G6B5:
        {
            // NOTE: Calculate R5G6B5 equivalent color
            Vec3 coln = { (float)color.r/255.0f, (float)color.g/255.0f, (float)color.b/255.0f };

            unsigned char r = (unsigned char)(round(coln.x*31.0f));
            unsigned char g = (unsigned char)(round(coln.y*63.0f));
            unsigned char b = (unsigned char)(round(coln.z*31.0f));

            ((unsigned short *)dstPtr)[0] = (unsigned short)r << 11 | (unsigned short)g << 5 | (unsigned short)b;

        } break;
        case IR5G5B5A1:
        {
            // NOTE: Calculate R5G5B5A1 equivalent color
            Vec4 coln = { (float)color.r/255.0f, (float)color.g/255.0f, (float)color.b/255.0f, (float)color.a/255.0f };

            unsigned char r = (unsigned char)(round(coln.x*31.0f));
            unsigned char g = (unsigned char)(round(coln.y*31.0f));
            unsigned char b = (unsigned char)(round(coln.z*31.0f));
            unsigned char a = (coln.w > ((float)50/255.0f))? 1 : 0;

            ((unsigned short *)dstPtr)[0] = (unsigned short)r << 11 | (unsigned short)g << 6 | (unsigned short)b << 1 | (unsigned short)a;

        } break;
        case IR4G4B4A4:
        {
            // NOTE: Calculate R5G5B5A1 equivalent color
            Vec4 coln = { (float)color.r/255.0f, (float)color.g/255.0f, (float)color.b/255.0f, (float)color.a/255.0f };

            unsigned char r = (unsigned char)(round(coln.x*15.0f));
            unsigned char g = (unsigned char)(round(coln.y*15.0f));
            unsigned char b = (unsigned char)(round(coln.z*15.0f));
            unsigned char a = (unsigned char)(round(coln.w*15.0f));

            ((unsigned short *)dstPtr)[0] = (unsigned short)r << 12 | (unsigned short)g << 8 | (unsigned short)b << 4 | (unsigned short)a;

        } break;
        case IR8G8B8:
        {
            ((unsigned char *)dstPtr)[0] = color.r;
            ((unsigned char *)dstPtr)[1] = color.g;
            ((unsigned char *)dstPtr)[2] = color.b;

        } break;
        case IR8G8B8A8:
        {
            ((unsigned char *)dstPtr)[0] = color.r;
            ((unsigned char *)dstPtr)[1] = color.g;
            ((unsigned char *)dstPtr)[2] = color.b;
            ((unsigned char *)dstPtr)[3] = color.a;

        } break;
        default: break;
    }
}

// Get src alpha-blended into dst color with tint
Color ColorAlphaBlend(Color dst, Color src, Color tint)
{
    Color out = WHITE;

    // Apply color tint to source color
    src.r = (unsigned char)(((unsigned int)src.r*((unsigned int)tint.r+1)) >> 8);
    src.g = (unsigned char)(((unsigned int)src.g*((unsigned int)tint.g+1)) >> 8);
    src.b = (unsigned char)(((unsigned int)src.b*((unsigned int)tint.b+1)) >> 8);
    src.a = (unsigned char)(((unsigned int)src.a*((unsigned int)tint.a+1)) >> 8);

    if (src.a == 0) out = dst;
    else if (src.a == 255) out = src;
    else
    {
        unsigned int alpha = (unsigned int)src.a + 1;     // We are shifting by 8 (dividing by 256), so we need to take that excess into account
        out.a = (unsigned char)(((unsigned int)alpha*256 + (unsigned int)dst.a*(256 - alpha)) >> 8);

        if (out.a > 0)
        {
            out.r = (unsigned char)((((unsigned int)src.r*alpha*256 + (unsigned int)dst.r*(unsigned int)dst.a*(256 - alpha))/out.a) >> 8);
            out.g = (unsigned char)((((unsigned int)src.g*alpha*256 + (unsigned int)dst.g*(unsigned int)dst.a*(256 - alpha))/out.a) >> 8);
            out.b = (unsigned char)((((unsigned int)src.b*alpha*256 + (unsigned int)dst.b*(unsigned int)dst.a*(256 - alpha))/out.a) >> 8);
        }
    }

    return out;
}
void GraphCopy(Graph *dst, Graph src, Rect srcRec, Rect dstRec, Color tint)
{
    // Security check to avoid program crash
    if ((dst->data == NULL) || (dst->width == 0) || (dst->height == 0) ||
        (src.data == NULL) || (src.width == 0) || (src.height == 0)) return;


        Graph *srcPtr = &src;       // Pointer to source image


        if (srcRec.x < 0) { srcRec.width += srcRec.x; srcRec.x = 0; }
        if (srcRec.y < 0) { srcRec.height += srcRec.y; srcRec.y = 0; }
        if ((srcRec.x + srcRec.width) > src.width) srcRec.width = src.width - srcRec.x;
        if ((srcRec.y + srcRec.height) > src.height) srcRec.height = src.height - srcRec.y;



        if (dstRec.x < 0)
        {
            srcRec.x = -dstRec.x;
            srcRec.width += dstRec.x;
            dstRec.x = 0;
        }
        else if ((dstRec.x + srcRec.width) > dst->width) srcRec.width = dst->width - dstRec.x;

        if (dstRec.y < 0)
        {
            srcRec.y = -dstRec.y;
            srcRec.height += dstRec.y;
            dstRec.y = 0;
        }
        else if ((dstRec.y + srcRec.height) > dst->height) srcRec.height = dst->height - dstRec.y;

        if (dst->width < srcRec.width) srcRec.width = (float)dst->width;
        if (dst->height < srcRec.height) srcRec.height = (float)dst->height;

        Color colSrc, colDst, blend;
        int blendRequired = 1;

        // Fast path: Avoid blend if source has no alpha to blend
        if ( (tint.a == 255) && (srcPtr->channels != 4))
            blendRequired = 0;

        int strideDst = GetPixelDataSize(dst->width, 1, dst->format);
        int bytesPerPixelDst = strideDst/(dst->width);

        int strideSrc = GetPixelDataSize(srcPtr->width, 1, srcPtr->format);
        int bytesPerPixelSrc = strideSrc/(srcPtr->width);

        unsigned char *pSrcBase = (unsigned char *)srcPtr->data + ((int)srcRec.y*srcPtr->width + (int)srcRec.x)*bytesPerPixelSrc;
        unsigned char *pDstBase = (unsigned char *)dst->data + ((int)dstRec.y*dst->width + (int)dstRec.x)*bytesPerPixelDst;

        for (int y = 0; y < (int)srcRec.height; y++)
        {
            unsigned char *pSrc = pSrcBase;
            unsigned char *pDst = pDstBase;

            // Fast path: Avoid moving pixel by pixel if no blend required and same format
            if (!blendRequired && (srcPtr->format == dst->format)) memcpy(pDst, pSrc, (int)(srcRec.width)*bytesPerPixelSrc);
            else
            {
                for (int x = 0; x < (int)srcRec.width; x++)
                {
                    colSrc = GetPixelColor(pSrc, srcPtr->format);
                    colDst = GetPixelColor(pDst, dst->format);

                    // Fast path: Avoid blend if source has no alpha to blend
                    if (blendRequired) blend = ColorAlphaBlend(colDst, colSrc, tint);
                    else blend = colSrc;

                    SetPixelColor(pDst, blend, dst->format);

                    pDst += bytesPerPixelDst;
                    pSrc += bytesPerPixelSrc;
                }
            }

            pSrcBase += strideSrc;
            pDstBase += strideDst;
        }


}

// Read texture pixel data
void *GetTexturePixels( unsigned int id , int width, int height, int format)
{
    void *pixels = NULL;


    glBindTexture(GL_TEXTURE_2D, id);

    // NOTE: Using texture id, we can retrieve some texture info (but not on OpenGL ES 2.0)
    // Possible texture info: GL_TEXTURE_RED_SIZE, GL_TEXTURE_GREEN_SIZE, GL_TEXTURE_BLUE_SIZE, GL_TEXTURE_ALPHA_SIZE
    //int width, height, format;
    //glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    //glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    //glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &format);

    // NOTE: Each row written to or read from by OpenGL pixel operations like glGetTexImage are aligned to a 4 byte boundary by default, which may add some padding.
    // Use glPixelStorei to modify padding with the GL_[UN]PACK_ALIGNMENT setting.
    // GL_PACK_ALIGNMENT affects operations that read from OpenGL memory (glReadPixels, glGetTexImage, etc.)
    // GL_UNPACK_ALIGNMENT affects operations that write to OpenGL memory (glTexImage, etc.)
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    unsigned int glInternalFormat, glFormat, glType;
    GetGlTextureFormats(format, &glInternalFormat, &glFormat, &glType);
    unsigned int size = GetPixelDataSize(width, height, format);

    if ((glInternalFormat != -1) )
    {
        pixels = mem_malloc(size);
       // glGetTexImage(GL_TEXTURE_2D, 0, glFormat, glType, pixels);
    }


    glBindTexture(GL_TEXTURE_2D, 0);
/*
    // glGetTexImage() is not available on OpenGL ES 2.0
    // Texture width and height are required on OpenGL ES 2.0. There is no way to get it from texture id.
    // Two possible Options:
    // 1 - Bind texture to color fbo attachment and glReadPixels()
    // 2 - Create an fbo, activate it, render quad with texture, glReadPixels()
    // We are using Option 1, just need to care for texture format on retrieval
    // NOTE: This behaviour could be conditioned by graphic driver...
    unsigned int fboId = rlLoadFramebuffer(width, height);

    glBindFramebuffer(GL_FRAMEBUFFER, fboId);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Attach our texture to FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id, 0);

    // We read data as RGBA because FBO texture is configured as RGBA, despite binding another texture format
    pixels = (unsigned char *)RL_MALLOC(rlGetPixelDataSize(width, height, RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8));
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Clean up temporal fbo
    rlUnloadFramebuffer(fboId);
*/

    return pixels;
}

int SaveTextureFile(Texture2D texture, const char* fileName)
{

    int result = 0;
    int channels = 4;
    if (texture.format == IGRAYSCALE) channels = 1;
    else if (texture.format == IGRAY_ALPHA) channels = 2;
    else if (texture.format == IR8G8B8) channels = 3;
    else if (texture.format == IR8G8B8A8) channels = 4;
    void *data = GetTexturePixels(texture.ID, texture.width, texture.height, texture.format);
    if (data)
    {
        if (isfile_extension(fileName,".png"))
            stbi_write_png(fileName,texture.width,texture.height,channels,(const unsigned char *)data,texture.width*channels);
        else
        if (isfile_extension(fileName,".jpg"))
            stbi_write_jpg(fileName,texture.width,texture.height,channels,(const unsigned char *)data,90);
        else
        if (isfile_extension(fileName,".bmp"))
            stbi_write_bmp(fileName,texture.width,texture.height,channels,(const unsigned char *)data);
        else
        if (isfile_extension(fileName,".tga"))
            stbi_write_tga(fileName,texture.width,texture.height,channels,(const unsigned char *)data);
        else
            SDL_LogError(0,"Unknow file format to save");

        mem_free(data);
    }
    return (result);
}

int SavePixelsDataFile(int width, int height,int format,void* data, const char* fileName)
{

    int result = 0;
    int channels = 4;
    if (format == IGRAYSCALE) channels = 1;
    else if (format == IGRAY_ALPHA) channels = 2;
    else if (format == IR8G8B8) channels = 3;
    else if (format == IR8G8B8A8) channels = 4;
    if (data)
    {
        if (isfile_extension(fileName,".png"))
            stbi_write_png(fileName,width,height,channels,(const unsigned char *)data,width*channels);
        else
        if (isfile_extension(fileName,".jpg"))
            stbi_write_jpg(fileName,width,height,channels,(const unsigned char *)data,90);
        else
        if (isfile_extension(fileName,".bmp"))
            stbi_write_bmp(fileName,width,height,channels,(const unsigned char *)data);
        else
        if (isfile_extension(fileName,".tga"))
            stbi_write_tga(fileName,width,height,channels,(const unsigned char *)data);
        else
            SDL_LogError(0,"Unknow file format to save");

    }
    return (result);
}

