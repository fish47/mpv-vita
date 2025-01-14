#pragma once

#include "common/common.h"

#include <libavcodec/codec.h>

struct ui_font;
struct ui_texture;
struct ui_context;
struct ui_color_vertex;

typedef unsigned int ui_color;

enum ui_texure_fmt {
    TEX_FMT_UNKNOWN,
    TEX_FMT_RGBA,
    TEX_FMT_YUV420,
};

struct ui_font_draw_args {
    const char *text;
    int size;
    int x;
    int y;
    ui_color color;
};

struct ui_texture_data_args {
    const uint8_t **data;
    const int *strides;
    int width;
    int height;
    int planes;
};

struct ui_texture_draw_args {
    struct mp_rect *src;
    struct mp_rect *dst;
    ui_color *tint;
};

struct ui_platform_driver {
    int priv_size;
    bool (*init)(struct ui_context *ctx, int argc, char *argv[]);
    void (*uninit)(struct ui_context *ctx);
    void (*exit)(void);
    void (*poll_events)(struct ui_context *ctx);
    uint32_t (*poll_keys)(struct ui_context *ctx);
    const char* (*get_files_dir)(struct ui_context *ctx);
    int (*get_battery_level)(struct ui_context *ctx);
};

struct ui_audio_driver {
    int buffer_count;
    bool (*init)(void **ctx, int samples, int freq, int channels);
    void (*uninit)(void **ctx);
    int (*output)(void *ctx, void *buff);
};

struct ui_render_driver {
    int priv_size;

    bool (*init)(struct ui_context *ctx);
    void (*uninit)(struct ui_context *ctx);

    void (*render_start)(struct ui_context *ctx);
    void (*render_end)(struct ui_context *ctx);

    int (*dr_align)(enum ui_texure_fmt fmt, int *w, int *h);
    bool (*dr_prepare)(struct ui_context *ctx, const AVCodec *codec, AVDictionary **opts);
    bool (*dr_vram_init)(struct ui_context *ctx, int size, void **vram);
    void (*dr_vram_uninit)(struct ui_context *ctx, void **vram);
    void (*dr_vram_lock)(struct ui_context *ctx, void *vram);
    void (*dr_vram_unlock)(struct ui_context *ctx, void *vram);

    bool (*texture_init)(struct ui_context *ctx, struct ui_texture **tex,
                         enum ui_texure_fmt fmt, int w, int h, bool dr);
    bool (*texture_decode)(struct ui_context *ctx, struct ui_texture **tex,
                           const void *data, int size, int *w, int *h);
    void (*texture_uninit)(struct ui_context *ctx, struct ui_texture **tex);
    void (*texture_upload)(struct ui_context *ctx, struct ui_texture *tex,
                           struct ui_texture_data_args *args);
    bool (*texture_attach)(struct ui_context *ctx, struct ui_texture *tex,
                           struct ui_texture_data_args *args);
    void (*texture_detach)(struct ui_context *ctx, struct ui_texture *tex);

    bool (*font_init)(struct ui_context *ctx, struct ui_font **font);
    void (*font_uninit)(struct ui_context *ctx, struct ui_font **font);
    void (*font_measure)(struct ui_context *ctx, struct ui_font *font,
                         const char* text, int size, int *w, int *h);

    void (*clip_start)(struct ui_context *ctx, struct mp_rect *rect);
    void (*clip_end)(struct ui_context *ctx);

    void (*draw_font)(struct ui_context *ctx, struct ui_font *font,
                      struct ui_font_draw_args *args);
    void (*draw_texture)(struct ui_context *ctx, struct ui_texture *tex,
                         struct ui_texture_draw_args *args);
    bool (*draw_vertices_prepare)(struct ui_context *ctx,
                                  struct ui_color_vertex **verts, int n);
    void (*draw_vertices_copy)(struct ui_context *ctx,
                               struct ui_color_vertex *verts, int dst, int src);
    void (*draw_vertices_compose)(struct ui_context *ctx,
                                  struct ui_color_vertex *verts,
                                  int i, float x, float y, ui_color color);
    void (*draw_vertices_commit)(struct ui_context *ctx,
                                 struct ui_color_vertex *verts, int n);
};

extern const struct ui_platform_driver ui_platform_driver_vita;
extern const struct ui_audio_driver ui_audio_driver_vita;
extern const struct ui_render_driver ui_render_driver_vita;
