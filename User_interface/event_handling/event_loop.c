#include  <def_user_interface.h>

private  void   handle_event(
    Event_types        event_type,
    window_struct      *event_window,
    int                key_pressed );

private  Boolean   quit_flag = FALSE;

public  void  set_quit_program_flag()
{
    quit_flag = TRUE;
}

public  void  event_loop()
{
    int                     key_pressed;
    Event_types             event_type;
    window_struct           *event_window;

    do
    {
        event_type = G_get_event( &event_window, &key_pressed );

        handle_event( event_type, event_window, key_pressed );

        if( event_type == NO_EVENT )
            make_windows_up_to_date();
    }
    while( !quit_flag );
}

private  void   handle_event(
    Event_types              event_type,
    window_struct            *event_window,
    int                      key_pressed )
{
    int                     x_mouse, y_mouse;
    event_viewports_struct  *event_viewports;

    if( !execute_global_event_callbacks( event_type, key_pressed ) &&
        event_is_allowable( event_type ) &&
        (!mouse_must_be_in_window(event_type) ||
         G_get_mouse_position( event_window, &x_mouse, &y_mouse )) &&
        lookup_event_viewports( event_window, &event_viewports ) )
    {
        execute_event_viewport_events( event_viewports,
                                       x_mouse, y_mouse,
                                       event_type, key_pressed );
    }
}