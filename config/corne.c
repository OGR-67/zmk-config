#include <zmk/event_manager.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/led.h>

const uint8_t fallback_color[3] = {100, 100, 100}; // Gris par défaut

const struct
{
    uint8_t layer;
    uint8_t key;
    uint8_t color[3];
} led_overrides[] = {
    {0, 1, {255, 0, 0}}, // Couche 0, Touche 1 : Rouge
    {1, 1, {0, 255, 0}}, // Couche 1, Touche 1 : Vert
};

const uint8_t *get_key_color(uint8_t layer, uint8_t key)
{
    for (int i = 0; i < sizeof(led_overrides) / sizeof(led_overrides[0]); i++)
    {
        if (led_overrides[i].layer == layer && led_overrides[i].key == key)
        {
            return led_overrides[i].color;
        }
    }
    return fallback_color;
}

void update_led_colors(uint8_t layer)
{
    for (int key = 0; key < KEY_COUNT; key++)
    {
        const uint8_t *color = get_key_color(layer, key);
        zmk_rgb_set_hsv_at_key(key, color[0], color[1], color[2]);
    }
}

int layer_state_changed_listener(const zmk_event_t *eh)
{
    const struct layer_state_changed *event = as_layer_state_changed(eh);
    if (event)
    {
        update_led_colors(event->layer);
    }
    return 0;
}

ZMK_LISTENER(layer_state_changed_listener, layer_state_changed_listener);
ZMK_SUBSCRIPTION(layer_state_changed_listener, layer_state_changed);
