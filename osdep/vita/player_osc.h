#pragma once

#include <stdbool.h>

struct player_osc_ctx;
struct ui_context;
struct mpv_handle;
struct mpv_event;

struct player_osc_ctx *player_osc_create_ctx(void *parent, struct mpv_handle *mpv);

void player_osc_on_draw(struct player_osc_ctx *c, struct ui_context *ctx);

void player_osc_on_event(struct player_osc_ctx *c, struct ui_context *ctx, struct mpv_event *e);

void player_osc_on_poll(struct player_osc_ctx *c, struct ui_context *ctx);