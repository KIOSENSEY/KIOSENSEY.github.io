#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <stdint.h>

typedef struct {
    float* vertices;          /* Выровненный массив координат (X,Y,Z) */
    float* normals;           /* Выровненный массив нормалей (NX,NY,NZ) */
    float* texcoords;         /* ДОБАВЛЕНО: Выровненный массив UV-координат (U,V) */
    uint32_t vertex_count;    
    uint32_t texture_id;      
    int is_loaded;            
} ObjModel;

int32_t obj_load(ObjModel* model, const char* path, const char* texture_path);
void obj_draw(ObjModel* model, float x, float y, float z, float scale, float rotY);
void obj_free(ObjModel* model);

#endif /* OBJ_LOADER_H */
