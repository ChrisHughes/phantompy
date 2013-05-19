#include <stdint.h>
#include "page.hpp"
#include "image.hpp"
#include "webelement.hpp"

extern "C" {
    // Initialize methods
    void* ph_engine_init();
    void ph_engine_free(void* engine);

    // Page Methods
    void* ph_page_create();
    void ph_page_set_viewpoint_size(void *page, int x, int y);
    void ph_page_free(void *page);

    int32_t ph_page_load(void *page, char *url);
    int32_t ph_page_is_loaded(void *page);

    char* ph_page_to_html(void *page);
    char* ph_page_evaluate_javascript(void *page, char* js);

    // Image Methods
    void* ph_page_capture_image(void *page, const char *format, int quality);
    void ph_image_free(void *image);
    int64_t ph_image_get_size(void* image);
    const char* ph_image_get_format(void* image);
    void ph_image_get_bytes(void *image, void *buffer, int64_t size);

    // Web element methods
    void* ph_page_find_first(void *page, const char *selector);
    void ph_webelement_free(void *el);

    char* ph_webelement_tag_name(void *el);
    char* ph_webelement_inner_html(void *el);
    char* ph_webelement_inner_text(void *el);

    int32_t ph_webelement_has_class(void *el, const char *classname);
    int32_t ph_webelement_has_attr(void *el, const char *attrname);
    int32_t ph_webelement_has_attrs(void *el);
    int32_t ph_webelement_is_null(void *el);
}