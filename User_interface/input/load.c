#include  <def_user_interface.h>

private  void  volume_has_been_loaded( UI_struct  *, load_struct * );

private  DEFINE_EVENT_FUNCTION( more_input )
{
    load_struct   *data;
    Real          fraction_done;

    data = (load_struct *) callback_data;

    if( input_more_of_volume( &data->volume, &data->input, &fraction_done ) )
    {
        set_load_popup_meter( data, fraction_done );
    }
    else
    {
        volume_has_been_loaded( get_ui_struct(), data );
    }
}

private  void  delete_popup_interaction(
    load_struct    *data )
{
    set_load_activity( get_ui_struct(), data->volume_index, ON );

    remove_global_event_callback( NO_EVENT, more_input, (void *) data );

    delete_load_popup( data );

    FREE( data );
}

public  Status  initialize_loading_volume(
    UI_struct  *ui_info,
    int        volume,
    char       filename[],
    Boolean    this_is_resampled_volume )
{
    Status        status;
    load_struct   *data;
    int           x_window, y_window;
    int           x_min, y_min, x_max, y_max;

    ALLOC( data, 1 );

    data->volume_index = volume;
    data->this_is_resampled_volume = this_is_resampled_volume;

    status = start_volume_input( filename, &data->volume, &data->input );

    if( status == OK )
    {
        set_load_activity( ui_info, volume, OFF );

        if( status == OK )
        {
            add_global_event_callback( NO_EVENT, more_input, ANY_MODIFIER,
                                       (void *) data );
        }

        get_graphics_viewport( &ui_info->graphics_window.graphics, 
                               get_volume_menu_viewport_index(volume),
                               &x_min, &x_max, &y_min, &y_max );

        G_get_window_position( ui_info->graphics_window.window,
                               &x_window, &y_window );

        initialize_load_popup( data, x_window + x_min, y_window + y_min,
                               filename );
    }
    else
        FREE( data );

    return( status );
}

private  void  volume_has_been_loaded(
    UI_struct      *ui_info,
    load_struct    *data )
{
    set_volume_widgets_activity( ui_info, data->volume_index, ON );

    if( !IF_volume_is_loaded( data->volume_index ) )
    {
        install_slice_events( &ui_info->graphics_window.event_viewports,
                              data->volume_index );
    }

    if( data->this_is_resampled_volume )
    {
        IF_set_resampled_volume( &data->volume,
                                 ui_info->original_filename_volume_2,
                                 &ui_info->resampling_transform );
        set_resampled_label_activity( ui_info, ON );
    }
    else
    {
        IF_set_volume( data->volume_index, &data->volume );
        set_resampled_label_activity( ui_info, OFF );
    }

    update_position_counters( ui_info, data->volume_index );

    if( IF_volume_is_loaded( 0 ) &&
        IF_volume_is_loaded( 1 ) )
    {
        set_merged_activity( ui_info, ON );
    }

    if( IF_volume_is_loaded( 1 - data->volume_index ) )
    {
        update_other_volume_positions( ui_info, 1 - data->volume_index );
    }

    delete_popup_interaction( data );
}

public  void  cancel_loading(
    load_struct    *data )
{
    cancel_volume_input( &data->volume, &data->input );

    delete_popup_interaction( data );
}
