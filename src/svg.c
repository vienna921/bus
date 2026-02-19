/**
 * @file svg.c
 * @brief Implementation of simple SVG drawing interface.
 *
 * Implements the basic functions for creating SVG documents.
 */
#include "svg.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>


/**
 * @brief Opaque SVG drawing context.
 *
 * Holds the necessary data to implement functions.
 */
struct SVG_CONTEXT{
    svg_write_fn write_fn;
    svg_cleanup_fn cleanup_fn;
    svg_user_context_ptr user;
    double width;
    double height;
} SVG_CONTEXT;


svg_context_ptr svg_create(svg_write_fn write_fn, 
                           svg_cleanup_fn cleanup_fn, 
                           svg_user_context_ptr user, 
                           svg_px_t width, 
                           svg_px_t height){

    if (!write_fn || !cleanup_fn){
        return NULL;
    }
    svg_context_ptr context = malloc(sizeof(SVG_CONTEXT));
    context->write_fn = write_fn;
    context->cleanup_fn = cleanup_fn;
    context->user = user;
    context->width = width;
    context->height = height;

    char header[256];
    snprintf(header, sizeof(header), "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" "<svg width=\"%d\" height=\"%d\" xmlns=\"http://www.w3.org/2000/svg\">\n", width, height);
    context->write_fn(context->user,header);

    return context;

}

svg_return_t svg_destroy(svg_context_ptr context){
    if (!context){
        return SVG_ERR_NULL;
    }
    context->write_fn(context->user, "</svg>\n");
    svg_return_t result = SVG_OK;
    if (context->cleanup_fn){
        result = context->cleanup_fn(context->user);
        if(result != SVG_OK){
            return result;
        }
    }
    free(context);
    return SVG_OK;
}

svg_return_t svg_circle(svg_context_ptr context,
                        const svg_point_t *center,
                        svg_real_t radius,
                        const char *style){
    if (!context || !center) return SVG_ERR_NULL;
    int size = snprintf(NULL, 0, "<circle cx= \"%lf\" cy=\"%lf\" r= \"%lf\" style= \"%s\" />\n", center->x, center->y, radius, style ? style : "");
    char* buffer = malloc(size + 1);
	snprintf(buffer, size + 1, "<circle cx= \"%lf\" cy=\"%lf\" r= \"%lf\" style= \"%s\" />\n", center->x, center->y, radius, style ? style : "");
	svg_return_t retur = context->write_fn(context->user, buffer);
    free(buffer);
    return retur;

}


svg_return_t svg_rect(svg_context_ptr context,
                      const svg_point_t *top_left,
                      const svg_size_t *size,
                      const char* style){
    if (!context || !size || !top_left) return SVG_ERR_NULL;
    int storage = snprintf(NULL, 0, "<rectangle x= \"%lf\" y= \"%lf\" width= \"%lf\" height= \"%lf\" style= \"%s\" />\n", top_left->x, top_left->y, size->width, size->height, style ? style : "");
    char* buffer = malloc(storage + 1);
    snprintf(buffer, storage+1, "<rectangle x= \"%lf\" y= \"%lf\" width= \"%lf\" height= \"%lf\" style= \"%s\" />\n", top_left->x, top_left->y, size->width, size->height, style ? style : "");
    svg_return_t retur = context->write_fn(context->user, buffer);
    free(buffer);
    return retur;
}

svg_return_t svg_line(svg_context_ptr context,
                      const svg_point_t *start,
                      const svg_point_t *end,
                      const char* style){
    if (!context || !start || !end) return SVG_ERR_NULL;
    int size = snprintf(NULL, 0, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" style=\"%s\" />\n",start->x, start->y, end->x, end->y, style ? style : "");
    char* buffer = malloc(size + 1);
    snprintf(buffer, size + 1, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" style=\"%s\" />\n",start->x, start->y, end->x, end->y, style ? style : "");
    svg_return_t retur = context->write_fn(context->user, buffer);
    free(buffer);
    return retur;
}

svg_return_t svg_group_begin(svg_context_ptr context, 
                             const char* attrs){
    if (!context) return SVG_ERR_NULL;
    int size = snprintf(NULL, 0, "<g style=\"%s\">\n", attrs ? attrs : "");
    char* buffer = malloc(size + 1);
    snprintf(buffer, size + 1, "<g style=\"%s\">\n", attrs ? attrs : "");
    svg_return_t result = context->write_fn(context->user, buffer);
    free(buffer);
    return result;
}

svg_return_t svg_group_end(svg_context_ptr context){
    if (!context) return SVG_ERR_NULL;
    const char *text = "</g>\n";
    return context->write_fn(context->user, text);
    
}
