/* ----------------------------------------------------------------------------
@COPYRIGHT  :
              Copyright 1993,1994,1995 David MacDonald,
              McConnell Brain Imaging Centre,
              Montreal Neurological Institute, McGill University.
              Permission to use, copy, modify, and distribute this
              software and its documentation for any purpose and without
              fee is hereby granted, provided that the above copyright
              notice appear in all copies.  The author and McGill University
              make no representations about the suitability of this
              software for any purpose.  It is provided "as is" without
              express or implied warranty.
---------------------------------------------------------------------------- */

#ifndef lint
static char rcsid[] = "$Header: /private-cvsroot/visualization/Register/User_interface/event_callbacks/tag_events.c,v 1.9 2001-05-23 04:24:42 stever Exp $";
#endif

#include  <user_interface.h>

/* ARGSUSED */

private  DEFINE_EVENT_FUNCTION( add_tag_point_callback )
{
    record_tag( get_ui_struct(), IF_get_n_tag_points() );
}

public  void  install_tag_events(
    UI_struct                *ui )
{
    int             volume, view;
    Viewport_types  viewport;

    for_less( volume, 0, N_VOLUMES_DISPLAYED )
    {
        for_less( view, 0, N_VIEWS )
        {
            add_event_viewport_callback(
                   &ui->graphics_window.event_viewports,
                   ui_get_slice_viewport_index(volume,view),
                   KEY_DOWN_EVENT, -1, -1, -1, -1,
                   slice_key_down_callback, ANY_MODIFIER, (void *) 0 );
            add_event_viewport_callback(
                   &ui->graphics_window.event_viewports,
                   ui_get_slice_viewport_index(volume,view),
                   RIGHT_MOUSE_DOWN_EVENT, -1, -1, -1, -1,
                   add_tag_point_callback, NO_SHIFT_ONLY, (void *) 0 );
        }
    }

    for_less( viewport, (Viewport_types) 0, N_UI_viewports )
    {
        add_event_viewport_callback(
               &ui->graphics_window.event_viewports, viewport,
               KEY_DOWN_EVENT, -1, -1, -1, -1,
               slice_key_down_callback, ANY_MODIFIER, (void *) 0 );
        add_event_viewport_callback(
               &ui->graphics_window.event_viewports, viewport,
               RIGHT_MOUSE_DOWN_EVENT, -1, -1, -1, -1,
               add_tag_point_callback, NO_SHIFT_ONLY, (void *) 0 );
    }
}
