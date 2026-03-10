#include "tizenclaw-webview.h"
#include <EWebKit.h>
#include <Ecore.h>

typedef struct appdata {
    Evas_Object *win;
    Evas_Object *conform;
    Evas_Object *web_view;
} appdata_s;

static void
win_delete_request_cb(void *data, Evas_Object *obj, void *event_info)
{
    ui_app_exit();
}

static void
win_back_cb(void *data, Evas_Object *obj, void *event_info)
{
    appdata_s *ad = static_cast<appdata_s *>(data);
    if (ewk_view_back_possible(ad->web_view)) {
        ewk_view_back(ad->web_view);
    } else {
        ui_app_exit();
    }
}

static void
create_base_gui(appdata_s *ad)
{
    /* Window */
    ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
    elm_win_autodel_set(ad->win, EINA_TRUE);

    if (elm_win_wm_rotation_supported_get(ad->win)) {
        int rots[4] = { 0, 90, 180, 270 };
        elm_win_wm_rotation_available_rotations_set(ad->win, (const int *)(&rots), 4);
    }

    evas_object_smart_callback_add(ad->win, "delete,request", win_delete_request_cb, NULL);
    eext_object_event_callback_add(ad->win, EEXT_CALLBACK_BACK, win_back_cb, ad);

    /* Conformant */
    ad->conform = elm_conformant_add(ad->win);
    elm_win_indicator_mode_set(ad->win, ELM_WIN_INDICATOR_SHOW);
    elm_win_indicator_opacity_set(ad->win, ELM_WIN_INDICATOR_OPAQUE);
    evas_object_size_hint_weight_set(ad->conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_win_resize_object_add(ad->win, ad->conform);
    evas_object_show(ad->conform);

    /* WebView */
    Evas *evas = evas_object_evas_get(ad->win);
    ad->web_view = ewk_view_add(evas);
    
    if (ad->web_view) {
        evas_object_size_hint_weight_set(ad->web_view, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
        evas_object_size_hint_align_set(ad->web_view, EVAS_HINT_FILL, EVAS_HINT_FILL);
        elm_object_content_set(ad->conform, ad->web_view);
        evas_object_show(ad->web_view);
        
        // Initial dummy load to ensure it's working
        ewk_view_url_set(ad->web_view, "about:blank");
    } else {
        dlog_print(DLOG_ERROR, LOG_TAG, "Failed to create ewk_view");
    }

    /* Show window after base gui is set up */
    evas_object_show(ad->win);
}

static bool
app_create(void *data)
{
    /* Hook to take necessary actions before main event loop starts
       Initialize UI resources and application's data
       If this function returns true, the main loop of application starts
       If this function returns false, the application is terminated */
    appdata_s *ad = static_cast<appdata_s *>(data);
    
    // Initialize ewebkit
    ewk_init();

    create_base_gui(ad);

    return true;
}

static void
app_control(app_control_h app_control, void *data)
{
    /* Handle the launch request. */
    char *uri = nullptr;
    appdata_s *ad = static_cast<appdata_s *>(data);
    
    if (app_control_get_uri(app_control, &uri) == APP_CONTROL_ERROR_NONE && uri != nullptr) {
        dlog_print(DLOG_INFO, LOG_TAG, "Received URI from app_control: %s", uri);
        if (ad->web_view) {
            ewk_view_url_set(ad->web_view, uri);
        }
        free(uri);
    } else {
        dlog_print(DLOG_INFO, LOG_TAG, "No URI provided in app_control");
    }
}

static void
app_pause(void *data)
{
    /* Take necessary actions when application becomes invisible. */
    appdata_s *ad = static_cast<appdata_s *>(data);
    if (ad->web_view) {
        ewk_view_suspend(ad->web_view);
    }
}

static void
app_resume(void *data)
{
    /* Take necessary actions when application becomes visible. */
    appdata_s *ad = static_cast<appdata_s *>(data);
    if (ad->web_view) {
        ewk_view_resume(ad->web_view);
    }
}

static void
app_terminate(void *data)
{
    /* Release all resources. */
    ewk_shutdown();
}

static void
ui_app_lang_changed(app_event_info_h event_info, void *user_data)
{
    /*APP_EVENT_LANGUAGE_CHANGED*/
    char *locale = NULL;
    system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &locale);
    elm_language_set(locale);
    free(locale);
    return;
}

static void
ui_app_orient_changed(app_event_info_h event_info, void *user_data)
{
    /*APP_EVENT_DEVICE_ORIENTATION_CHANGED*/
    return;
}

static void
ui_app_region_changed(app_event_info_h event_info, void *user_data)
{
    /*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void
ui_app_low_battery(app_event_info_h event_info, void *user_data)
{
    /*APP_EVENT_LOW_BATTERY*/
}

static void
ui_app_low_memory(app_event_info_h event_info, void *user_data)
{
    /*APP_EVENT_LOW_MEMORY*/
}

int
main(int argc, char *argv[])
{
    appdata_s ad = {};
    int ret = 0;

    ui_app_lifecycle_callback_s event_callback = {};
    app_event_handler_h handlers[5] = {nullptr, };

    event_callback.create = app_create;
    event_callback.terminate = app_terminate;
    event_callback.pause = app_pause;
    event_callback.resume = app_resume;
    event_callback.app_control = app_control;

    ui_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, ui_app_low_battery, &ad);
    ui_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, ui_app_low_memory, &ad);
    ui_app_add_event_handler(&handlers[APP_EVENT_DEVICE_ORIENTATION_CHANGED], APP_EVENT_DEVICE_ORIENTATION_CHANGED, ui_app_orient_changed, &ad);
    ui_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, ui_app_lang_changed, &ad);
    ui_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, ui_app_region_changed, &ad);

    ret = ui_app_main(argc, argv, &event_callback, &ad);
    if (ret != APP_ERROR_NONE) {
        dlog_print(DLOG_ERROR, LOG_TAG, "app_main() is failed. err = %d", ret);
    }

    return ret;
}
