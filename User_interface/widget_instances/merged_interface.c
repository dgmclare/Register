#include  <def_user_interface.h>

typedef  enum
{
    OPACITY_SLIDER,
    RESET_VIEW_BUTTON,
    N_MERGED_WIDGETS
}
Merged_widgets;

static  int  widget_indices[N_MERGED_WIDGETS];

private  DEFINE_WIDGET_CALLBACK( opacity_callback ) /* ARGSUSED */
{
    Viewport_types        viewport_index;

    viewport_index = widget->viewport_index;
}

public  void  add_merged_widgets(
    UI_struct         *ui_info,
    Viewport_types    viewport_index )
{
    int            x, y, height, volume_index;

    volume_index = get_viewport_volume_index( viewport_index );

    x = Volume_menu_x_offset;
    y = Volume_menu_y_offset;

    ui_info->colour_bar_start_index[volume_index] =
             add_colour_bar_widgets( ui_info, viewport_index, x, y, &height );

    y += height + Interface_y_spacing;

    widget_indices[OPACITY_SLIDER] = create_slider( ui_info,
                   viewport_index, x, y, Opacity_slider_width,
                   Opacity_slider_height,
                   0.5, 0.0, 1.0,
                   OFF,
                   SLIDER_ACTIVE_COLOUR, SLIDER_INACTIVE_COLOUR,
                   SLIDER_PEG_COLOUR,
                   opacity_callback );

    if( volume_index == 2 )
    {
        y += Opacity_slider_height + Volume_button_height +
             Interface_y_spacing;

        widget_indices[RESET_VIEW_BUTTON] = create_button( ui_info,
                   viewport_index, x, y,
                   Volume_button_width, Volume_button_height,
                   "Reset View",
                   OFF, BUTTON_ACTIVE_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   reset_view_callback );

         y += Volume_button_height + Interface_y_spacing;

         ui_info->position_text_start_index[volume_index] =
                           add_cursor_position_widgets(
                                    ui_info, viewport_index, x, y, &height );
    }
}

public  void  set_merged_widgets_activity(
    UI_struct         *ui_info,
    int               volume_index,
    Boolean           activity )
{
    Viewport_types       viewport_index;

    viewport_index = get_volume_menu_viewport_index( volume_index );

    set_widget_activity_and_update( ui_info,
                             ui_info->widget_list[viewport_index].widgets
                                            [widget_indices[OPACITY_SLIDER]],
                             activity );

    set_colour_bar_widgets_activity( ui_info, viewport_index,
                           ui_info->colour_bar_start_index[volume_index],
                           activity );

    if( volume_index == 2 )
    {
        set_voxel_position_widgets_activity( ui_info, viewport_index,
                           ui_info->position_text_start_index[volume_index],
                           activity );
        set_widget_activity_and_update( ui_info,
                             ui_info->widget_list[viewport_index].widgets
                                            [widget_indices[RESET_VIEW_BUTTON]],
                             activity );
    }
}