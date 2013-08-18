/* Skippy - Seduces Kids Into Perversion
 *
 * Copyright (C) 2004 Hyriand <hyriand@thegraveyard.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef SKIPPY_CLIENT_H
#define SKIPPY_CLIENT_H

// 2d int maths helpers
typedef struct Vec2i {	// todo: refactor to use this
	int x,y;
} Vec2i;

typedef struct Rect2i {
	Vec2i min,max;
} Rect2i;

inline int invlerpi(int lo,int hi,int v,int one) { return ((v-lo)*one)/(hi-lo);}
inline int lerpi(int lo,int hi,int f,int one) { return lo+(((hi-lo)*f)/one);}
inline void v2i_set(Vec2i* dst, int x, int y) { dst->x=x;dst->y=y;}
inline Vec2i vec2i_mk(int x, int y) { Vec2i v; v.x=x; v.y=y; return v;}
inline Vec2i v2i_sub(Vec2i a,Vec2i b) { Vec2i ret= {b.x-a.x,b.y-a.y};return ret; }
inline Vec2i v2i_add(Vec2i a,Vec2i b) { Vec2i ret= {b.x+a.x,b.y+a.y};return ret; }
inline Vec2i v2i_mul(Vec2i a,int f,int prec) { Vec2i ret= {(a.x*f)/prec,(a.y*f)/prec};return ret; }
inline Vec2i v2i_mad(Vec2i a,Vec2i b,int f,int prec) { return v2i_add(a, v2i_mul(b,f,prec)); }
inline Vec2i v2i_lerp(Vec2i a,Vec2i b,int f,int prec) { return v2i_add(a, v2i_mul(v2i_sub(b,a),f,prec)); }
inline Vec2i v2i_invlerp(const Vec2i vmin,const Vec2i vmax, Vec2i v,int precision ) { Vec2i ret={invlerpi(vmin.x, vmax.x, v.x,precision), invlerpi(vmin.y, vmax.y, v.y,precision)}; return ret; }
inline Vec2i rect2i_lerp(const Rect2i* r, Vec2i v, int precision) { Vec2i ret={lerpi(r->min.x,r->max.x, v.x,precision),lerpi(r->min.y,r->max.y, v.y,precision)}; return ret;}
inline Vec2i rect2i_invlerp(const Rect2i* r, Vec2i v, int precision) { return v2i_invlerp(r->min,r->max, v, precision);}
inline Vec2i v2i_min(Vec2i a,Vec2i b) { Vec2i ret={MIN(a.x,b.x),MIN(a.y,b.y)}; return ret;}
inline Vec2i v2i_max(Vec2i a,Vec2i b) { Vec2i ret={MAX(a.x,b.x),MAX(a.y,b.y)}; return ret;}
inline Rect2i rect2i_init(void){ Rect2i ret={{INT_MAX,INT_MAX},{-INT_MAX,-INT_MAX}}; return ret;}
inline Rect2i rect2i_mk_at(Vec2i pos, Vec2i size){ Rect2i ret; ret.min=pos; ret.max=v2i_add(pos,size);  return ret;}
inline Rect2i rect2i_mk(Vec2i a, Vec2i b){ Rect2i ret; ret.min=a; ret.max=b;  return ret;}
inline void rect2i_set_init(Rect2i* ret){  v2i_set(&ret->min,INT_MAX,INT_MAX);v2i_set(&ret->max,-INT_MAX,-INT_MAX); }
inline void rect2i_include(Rect2i* r,Vec2i v){ r->min=v2i_min(r->min,v);r->max=v2i_max(r->max,v); }
inline void rect2i_include_rect2i(Rect2i* r,const Rect2i* src){ r->min=v2i_min(r->min,src->min);r->max=v2i_max(r->max,src->max); }
inline Vec2i rect2i_size(const Rect2i* r) { return v2i_sub(r->max,r->min);}
inline int rect2i_aspect(const Rect2i* r,int precision) { Vec2i sz=rect2i_size(r); return (sz.x*precision)/sz.y;}
inline Rect2i rect2i_add(const Rect2i* a, Vec2i ofs) { Rect2i ret={v2i_add(a->min,ofs),v2i_add(a->max,ofs)}; return ret;}
inline int rect2i_area( const Rect2i* r) { Vec2i sz=rect2i_size(r); return (sz.x*sz.y);}
inline Rect2i rect2i_intersect( const Rect2i* a,const Rect2i* b ) { Rect2i r; r.min=v2i_max(a->min,b->min); r.max=v2i_min(a->max,b->max); return r;}
inline int rect2i_overlap( const Rect2i* a,const Rect2i* b) { Rect2i ri=rect2i_intersect(a,b);Vec2i sz= rect2i_size(&ri);if (sz.x>0 && sz.y>0) return sz.x*sz.y; else return 0;}
inline bool rect2i_valid(const Rect2i* a) { return a->max.x>=a->min.x && a->max.y >= a->min.y; }

typedef struct {
	Window window;
	int x, y;
	unsigned int width, height;
	XRenderPictFormat *format;
} SkippyWindow;

#define SKIPPYWINT_INIT { .window = None }

struct _MainWin;
typedef struct {
	struct _MainWin *mainwin;
	
	SkippyWindow client;
	bool redirected;
	Pixmap cpixmap;
	SkippyWindow mini;
	
	Pixmap pixmap;
	Picture origin, destination;
	Damage damage;
	float factor;
	
	int focused;
	
	bool damaged;
	/* XserverRegion repair; */
	
	/* These are virtual positions set by the layout routine */
	int x, y;
} ClientWin;

#define CLIENTWT_INIT { \
	.client = SKIPPYWINT_INIT, \
	.mini = SKIPPYWINT_INIT, \
	.mainwin = NULL \
}

int clientwin_validate_func(dlist *, void *);
int clientwin_sort_func(dlist *, dlist *, void *);
ClientWin *clientwin_create(struct _MainWin *, Window);
void clientwin_destroy(ClientWin *, bool destroyed);
void clientwin_move(ClientWin *, float, int, int);
void clientwin_map(ClientWin *);
void clientwin_unmap(ClientWin *);
int clientwin_handle(ClientWin *, XEvent *);
int clientwin_cmp_func(dlist *, void*);
void clientwin_update(ClientWin *cw);
int clientwin_check_group_leader_func(dlist *l, void *data);
void clientwin_render(ClientWin *);
void clientwin_schedule_repair(ClientWin *cw, XRectangle *area);
void clientwin_repair(ClientWin *cw);
// accessors, less needed if code is refactored to use vec2i etc.
inline Vec2i skippywindow_pos(const SkippyWindow* w)	{ vec2i_mk(w->x,w->y); }
inline Vec2i skippywindow_size(const SkippyWindow* w)	{ vec2i_mk(w->width,w->height); }
inline Rect2i clientwin_rect(const ClientWin* w)	{ return rect2i_mk_at(skippywindow_pos(&w->client),skippywindow_size(&w->client));}
inline Rect2i clientwin_mini_rect(const ClientWin* w)	{ return rect2i_mk_at(skippywindow_pos(&w->mini),skippywindow_size(&w->mini));}

#endif /* SKIPPY_CLIENT_H */