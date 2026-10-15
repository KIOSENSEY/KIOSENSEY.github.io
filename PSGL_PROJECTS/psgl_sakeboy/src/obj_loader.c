#include "common.h"
#include "obj_loader.h"

/* Автономный загрузчик RAW TGA с аппаратным переворотом байт под Big-Endian архитектуру PS3 Cell */
static uint32_t load_tga_texture_file(const char* filename)
{
    FILE* f = fopen(filename, "rb");
    if (!f) {
        printf("[ОШИБКА] Не удалось открыть TGA файл: %s\n", filename);
        return 0;
    }

    unsigned char header[18];
    if (fread(header, 1, 18, f) != 18) {
        fclose(f);
        return 0;
    }

    int width  = header[12] | (header[13] << 8);
    int height = header[14] | (header[15] << 8);
    int bpp    = header[16];

    if (header[2] != 2) {
        printf("[ОШИБКА] Файл %s должен быть в формате RAW TGA (без сжатия RLE)!\n", filename);
        fclose(f);
        return 0;
    }

    int bytes_per_pixel = bpp / 8;
    int image_size = width * height * bytes_per_pixel;

    unsigned char* data = (unsigned char*)malloc(image_size);
    if (fread(data, 1, image_size, f) != image_size) {
        free(data);
        fclose(f);
        return 0;
    }
    fclose(f);

    for (int i = 0; i < image_size; i += bytes_per_pixel) {
        unsigned char temp = data[i];
        data[i] = data[i + 2];
        data[i + 2] = temp;
    }

    GLuint tex_id = 0;
    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLenum format = (bytes_per_pixel == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    free(data);
    printf("[УСПЕХ] Текстура TGA загружена: %s (%dx%d)\n", filename, width, height);
    return (uint32_t)tex_id;
}

int32_t obj_load(ObjModel* model, const char* path, const char* texture_path)
{
    FILE* file = fopen(path, "r");
    if (!file) {
        printf("[ОШИБКА] Не удалось открыть OBJ файл: %s\n", path);
        return -1;
    }

    memset(model, 0, sizeof(ObjModel));

    uint32_t max_elements = 32768;
    float* temp_v  = (float*)malloc(max_elements * 3 * sizeof(float));
    float* temp_vt = (float*)malloc(max_elements * 2 * sizeof(float));
    float* temp_vn = (float*)malloc(max_elements * 3 * sizeof(float));

    uint32_t count_v = 0, count_vt = 0, count_vn = 0;

    uint32_t max_vertices = 65536;
    model->vertices  = (float*)malloc(max_vertices * 3 * sizeof(float));
    model->normals   = (float*)malloc(max_vertices * 3 * sizeof(float));
    model->texcoords = (float*)malloc(max_vertices * 2 * sizeof(float));

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            float x, y, z;
            if (sscanf(line + 2, "%f %f %f", &x, &y, &z) == 3) {
                if (count_v >= max_elements) {
                    max_elements *= 2;
                    temp_v  = (float*)realloc(temp_v,  max_elements * 3 * sizeof(float));
                    temp_vt = (float*)realloc(temp_vt, max_elements * 2 * sizeof(float));
                    temp_vn = (float*)realloc(temp_vn, max_elements * 3 * sizeof(float));
                }
                temp_v[count_v * 3]     = x; 
                temp_v[count_v * 3 + 1] = y; 
                temp_v[count_v * 3 + 2] = z;
                count_v++;
            }
        }
        else if (line[0] == 'v' && line[1] == 't') {
            float u, v;
            if (sscanf(line + 3, "%f %f", &u, &v) == 2) {
                temp_vt[count_vt * 2]     = u; 
                temp_vt[count_vt * 2 + 1] = v; /* СБРОШЕНО: Оставляем V чистым, так как инверсия будет ниже */
                count_vt++;
            }
        }
        else if (line[0] == 'v' && line[1] == 'n') {
            float nx, ny, nz;
            if (sscanf(line + 3, "%f %f %f", &nx, &ny, &nz) == 3) {
                temp_vn[count_vn * 3]     = nx; 
                temp_vn[count_vn * 3 + 1] = ny; 
                temp_vn[count_vn * 3 + 2] = nz;
                count_vn++;
            }
        }
        else if (line[0] == 'f' && line[1] == ' ') {
            /* ИСПРАВЛЕНО: Теперь это полноценные массивы для хранения ТРЕХ вершин полигона */
            int v_indices[3] = {0, 0, 0};
            int vt_indices[3] = {0, 0, 0};
            int vn_indices[3] = {0, 0, 0};
            int vertex_idx = 0;

            char* token = strtok(line + 2, " \t\r\n");
            while (token && vertex_idx < 3) {
                int v = 0, vt = 0, vn = 0;
                
                if (sscanf(token, "%d/%d/%d", &v, &vt, &vn) == 3) {
                    v_indices[vertex_idx] = v; vt_indices[vertex_idx] = vt; vn_indices[vertex_idx] = vn;
                } else if (sscanf(token, "%d//%d", &v, &vn) == 2) {
                    v_indices[vertex_idx] = v; vn_indices[vertex_idx] = vn;
                } else if (sscanf(token, "%d/%d", &v, &vt) == 2) {
                    v_indices[vertex_idx] = v; vt_indices[vertex_idx] = vt;
                } else if (sscanf(token, "%d", &v) == 1) {
                    v_indices[vertex_idx] = v;
                }

                vertex_idx++;
                token = strtok(NULL, " \t\r\n");
            }

            if (vertex_idx == 3) {
                if (model->vertex_count >= max_vertices - 3) {
                    max_vertices *= 2;
                    model->vertices  = (float*)realloc(model->vertices,  max_vertices * 3 * sizeof(float));
                    model->normals   = (float*)realloc(model->normals,   max_vertices * 3 * sizeof(float));
                    model->texcoords = (float*)realloc(model->texcoords, max_vertices * 2 * sizeof(float));
                }

                for (int i = 0; i < 3; i++) {
                    uint32_t dst_3 = model->vertex_count * 3;
                    uint32_t dst_2 = model->vertex_count * 2;
                    
                    if (v_indices[i] > 0 && v_indices[i] <= (int)count_v) {
                        uint32_t src_v = (v_indices[i] - 1) * 3;
                        model->vertices[dst_3]     = temp_v[src_v];
                        model->vertices[dst_3 + 1] = temp_v[src_v + 1];
                        model->vertices[dst_3 + 2] = temp_v[src_v + 2];
                    }

                    /* ИСПРАВЛЕНО: Точная склейка текстурных координат по индексам из массива */
                    if (vt_indices[i] > 0 && vt_indices[i] <= (int)count_vt) {
                        uint32_t src_t = (vt_indices[i] - 1) * 2;
                        model->texcoords[dst_2]     = temp_vt[src_t];
                        model->texcoords[dst_2 + 1] = 1.0f - temp_vt[src_t + 1]; /* Аппаратный флип развертки */
                    } else {
                        model->texcoords[dst_2] = 0.0f; model->texcoords[dst_2 + 1] = 0.0f;
                    }

                    if (vn_indices[i] > 0 && vn_indices[i] <= (int)count_vn) {
                        uint32_t src_n = (vn_indices[i] - 1) * 3;
                        model->normals[dst_3]     = temp_vn[src_n];
                        model->normals[dst_3 + 1] = temp_vn[src_n + 1];
                        model->normals[dst_3 + 2] = temp_vn[src_n + 2];
                    } else {
                        model->normals[dst_3] = 0.0f; model->normals[dst_3+1] = 1.0f; model->normals[dst_3+2] = 0.0f;
                    }

                    model->vertex_count++;
                }
            }
        }
    }

    free(temp_v); free(temp_vt); free(temp_vn);
    fclose(file);

    if (model->vertex_count == 0) {
        obj_free(model);
        return -2;
    }

    model->vertices  = (float*)realloc(model->vertices,  model->vertex_count * 3 * sizeof(float));
    model->normals   = (float*)realloc(model->normals,   model->vertex_count * 3 * sizeof(float));
    model->texcoords = (float*)realloc(model->texcoords, model->vertex_count * 2 * sizeof(float));
    
    if (texture_path && strlen(texture_path) > 0) {
        model->texture_id = load_tga_texture_file(texture_path);
    }
    model->is_loaded = 1;

    return 0;
}

void obj_draw(ObjModel* model, float x, float y, float z, float scale, float rotY)
{
    if (!model || !model->is_loaded || model->vertex_count == 0) return;

    glPushMatrix();
        glTranslatef(x, y, z);
        glScalef(scale, scale, scale);
        glRotatef(rotY, 0.0f, 1.0f, 0.0f);

        if (model->texture_id > 0) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, model->texture_id);
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(2, GL_FLOAT, 0, model->texcoords);
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f); 
        }

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);

        glVertexPointer(3, GL_FLOAT, 0, model->vertices);
        glNormalPointer(GL_FLOAT, 0, model->normals);

        glDrawArrays(GL_TRIANGLES, 0, model->vertex_count);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        
        if (model->texture_id > 0) {
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            glDisable(GL_TEXTURE_2D);
        }
    glPopMatrix();
}

void obj_free(ObjModel* model)
{
    if (model->vertices)  { free(model->vertices);  model->vertices  = NULL; }
    if (model->normals)   { free(model->normals);   model->normals   = NULL; }
    if (model->texcoords) { free(model->texcoords); model->texcoords = NULL; }
    if (model->texture_id > 0) {
        GLuint tex = model->texture_id;
        glDeleteTextures(1, &tex);
        model->texture_id = 0;
    }
    model->vertex_count = 0;
    model->is_loaded = 0;
}
