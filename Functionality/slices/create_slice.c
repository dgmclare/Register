#include  <def_register.h>

public  void  create_slice_pixels(
    main_struct   *main,
    int           volume_index,
    int           view )
{
    int              x_axis_index, y_axis_index;
    int              x_size, y_size, axis;
    Pixel_types      pixel_type;
    Real             x_translation, y_translation, x_scale, y_scale;
    volume_struct    *volume;
    Real             *position;
    unsigned short   *cmode_colour_map;
    Colour           *rgb_colour_map;

    volume = get_slice_volume( main, volume_index );
    position = get_volume_cursor( main, volume_index );
    get_slice_axes( view, &x_axis_index, &y_axis_index );
    get_slice_transform( main, volume_index, view,
                         &x_translation, &y_translation, &x_scale, &y_scale );

    if( G_get_colour_map_state(main->window) )
        pixel_type = COLOUR_INDEX_16BIT_PIXEL;
    else
        pixel_type = RGB_PIXEL;

    get_slice_viewport_size( main, volume_index, view, &x_size, &y_size );

    cmode_colour_map = main->trislice[volume_index].cmode_colour_map;
    rgb_colour_map = main->trislice[volume_index].rgb_colour_map;

    axis = get_slice_axis( view );

    create_volume_slice( volume, (volume_struct *) NULL, position[axis],
                    x_axis_index, FALSE,
                    y_axis_index, FALSE,
                    x_translation, y_translation, x_scale, y_scale,
                    x_size, y_size, pixel_type,
                    main->interpolation_flag,
                    &cmode_colour_map,
                    &rgb_colour_map,
                    &main->trislice[volume_index].slices[view].n_pixels_alloced,
                    main->trislice[volume_index].slices[view].pixels );
}

public  void  create_merged_pixels(
    main_struct   *main,
    int           view )
{
    int              x_axis_index, y_axis_index;
    int              x_size, y_size, axis;
    Pixel_types      pixel_type;
    Real             x_translation, y_translation, x_scale, y_scale;
    volume_struct    *volume1, *volume2;
    Real             *position;

    volume1 = get_slice_volume( main, 0 );
    volume2 = get_slice_volume( main, 1 );
    position = get_volume_cursor( main, MERGED_VOLUME_INDEX );
    get_slice_axes( view, &x_axis_index, &y_axis_index );
    get_slice_transform( main, MERGED_VOLUME_INDEX, view,
                         &x_translation, &y_translation, &x_scale, &y_scale );

    if( G_get_colour_map_state(main->window) )
        pixel_type = COLOUR_INDEX_16BIT_PIXEL;
    else
        pixel_type = RGB_PIXEL;

    get_slice_viewport_size( main, MERGED_VOLUME_INDEX, view, &x_size, &y_size);

    axis = get_slice_axis( view );

    create_volume_slice( volume1, volume2, position[axis],
                    x_axis_index, FALSE,
                    y_axis_index, FALSE,
                    x_translation, y_translation, x_scale, y_scale,
                    x_size, y_size, pixel_type,
                    main->interpolation_flag,
                    main->merged.cmode_colour_map,
                    main->merged.rgb_colour_map,
                    &main->merged.slices[view].n_pixels_alloced,
                    main->merged.slices[view].pixels );
}

public  Boolean   convert_pixel_to_voxel(
    main_struct     *main,
    int             volume_index,
    int             view_index,
    int             x_pixel,
    int             y_pixel,
    Real            voxel_position[N_DIMENSIONS] )
{
    Boolean        in_volume;
    int            x_axis_index, y_axis_index;
    Real           x_translation, y_translation, x_scale, y_scale;
    volume_struct  *volume;
    Real           *position;

    volume = get_slice_volume( main, volume_index );
    position = get_volume_cursor( main, volume_index );
    get_slice_axes( view_index, &x_axis_index, &y_axis_index );
    get_slice_transform( main, volume_index, view_index,
                         &x_translation, &y_translation, &x_scale, &y_scale );

    in_volume = convert_slice_pixel_to_voxel(
              volume, x_pixel, y_pixel, position,
              x_axis_index, FALSE, y_axis_index, FALSE,
              x_translation, y_translation, x_scale, y_scale,
              voxel_position );

    return( in_volume );
}

public  void   convert_voxel_to_pixel(
    main_struct     *main,
    int             volume_index,
    int             view_index,
    Real            voxel_position[N_DIMENSIONS],
    int             *x_pixel,
    int             *y_pixel )
{
    int            x_axis_index, y_axis_index;
    Real           x_translation, y_translation, x_scale, y_scale;
    volume_struct  *volume;

    volume = get_slice_volume( main, volume_index );
    get_slice_axes( view_index, &x_axis_index, &y_axis_index );
    get_slice_transform( main, volume_index, view_index,
                         &x_translation, &y_translation, &x_scale, &y_scale );

    convert_voxel_to_slice_pixel( volume, voxel_position,
                                  x_axis_index, FALSE, y_axis_index, FALSE,
                                  x_translation, y_translation,
                                  x_scale, y_scale, x_pixel, y_pixel );
}

private  void  record_slice_display_limits(
    main_struct  *main,
    int          volume,
    int          view )
{
    int            axis, x_viewport_size, y_viewport_size, axis_index[2];
    slice_struct   *slice;
    Real           voxel_position[N_DIMENSIONS];

    slice = get_slice_struct( main, volume, view );
    get_slice_viewport_size( main, volume, view,
                             &x_viewport_size, &y_viewport_size );

    get_slice_axes( view, &axis_index[0], &axis_index[1] );

    for_less( axis, 0, 2 )
    {
        (void) convert_pixel_to_voxel( main, volume, view, 0, 0,
                                       voxel_position );
        slice->lower_display_limits[axis] = voxel_position[axis_index[axis]];

        (void) convert_pixel_to_voxel( main, volume, view,
                                       x_viewport_size-1, y_viewport_size-1,
                                       voxel_position );
        slice->upper_display_limits[axis] = voxel_position[axis_index[axis]];
    }
}

public  void  translate_slice(
    main_struct  *main,
    int          volume,
    int          view,
    int          x_translation_offset,
    int          y_translation_offset )
{
    Real           x_translation, y_translation, x_scale, y_scale;

    get_slice_transform( main, volume, view, &x_translation, &y_translation,
                         &x_scale, &y_scale );

    x_translation += (Real) x_translation_offset;
    y_translation += (Real) y_translation_offset;

    set_slice_translation( main, volume, view, x_translation, y_translation );
    record_slice_display_limits( main, volume, view );
    update_volume_cursor( main, volume, view );
    set_recreate_slice_flag( main, volume, view );
    update_slice_tag_objects( main, volume, view );
}

public  void  scale_slice(
    main_struct  *main,
    int          volume,
    int          view,
    Real         scale_factor )
{
    Real  x_translation, y_translation, x_scale, y_scale;
    Real  x_centre, y_centre;
    int   x_size, y_size;

    get_slice_viewport_size( main, volume, view, &x_size, &y_size );
    x_centre = (Real) x_size / 2.0;
    y_centre = (Real) y_size / 2.0;

    get_slice_transform( main, volume, view, &x_translation, &y_translation,
                         &x_scale, &y_scale );

    x_translation = x_centre - scale_factor * (x_centre - x_translation);
    y_translation = y_centre - scale_factor * (y_centre - y_translation);
    x_scale *= scale_factor;
    y_scale *= scale_factor;

    set_slice_translation( main, volume, view, x_translation, y_translation );
    set_slice_scale( main, volume, view, x_scale, y_scale );
    record_slice_display_limits( main, volume, view );
    update_volume_cursor( main, volume, view );
    set_recreate_slice_flag( main, volume, view );
    update_slice_tag_objects( main, volume, view );
}

public  void  fit_slice_to_view(
    main_struct  *main,
    int          volume_index,
    int          view )
{
    int            viewport_size[2], axis, axis_index[2];
    Real           voxel_diff[2], start_voxel[2];
    Real           scales[2], scale_factor;
    Real           translation[2];
    volume_struct  *volume;
    slice_struct   *slice;

    volume = get_slice_volume( main, volume_index );
    slice = get_slice_struct( main, volume_index, view );
    get_slice_axes( view, &axis_index[0], &axis_index[1] );
    get_slice_viewport_size( main, volume_index, view, &viewport_size[0],
                             &viewport_size[1] );

    for_less( axis, 0, 2 )
    {
        voxel_diff[axis] = slice->upper_display_limits[axis] -
                           slice->lower_display_limits[axis];

        scales[axis] = (Real) (viewport_size[axis] - 1) / voxel_diff[axis] /
                       volume->thickness[axis_index[axis]]; 
    }

    scale_factor = MIN( scales[0], scales[1] );

    for_less( axis, 0, 2 )
    {
        start_voxel[axis] = slice->lower_display_limits[axis] -
              ((Real) (viewport_size[axis] - 1) / scale_factor /
               volume->thickness[axis_index[axis]] - voxel_diff[axis]) / 2.0;

        translation[axis] = -scale_factor *
                            volume->thickness[axis_index[axis]] *
                            start_voxel[axis] + 0.5;
    }

    set_slice_translation( main, volume_index, view,
                           translation[0], translation[1] );

    set_slice_scale( main, volume_index, view, scale_factor, scale_factor );

    update_volume_cursor( main, volume_index, view );
    set_recreate_slice_flag( main, volume_index, view );
    update_slice_tag_objects( main, volume_index, view );
}

public  void  reset_slice_view(
    main_struct  *main,
    int          volume_index,
    int          view )
{
    int            x_axis_index, y_axis_index;
    Real           x_max_voxel, y_max_voxel, boundary;
    volume_struct  *volume;
    slice_struct   *slice;

    volume = get_slice_volume( main, volume_index );
    slice = get_slice_struct( main, volume_index, view );
    get_slice_axes( view, &x_axis_index, &y_axis_index );

    x_max_voxel = (Real) volume->sizes[x_axis_index] - 0.5;
    y_max_voxel = (Real) volume->sizes[y_axis_index] - 0.5;

    boundary = (1.0-Slice_fit_size)/2.0*x_max_voxel;

    slice->lower_display_limits[0] = -boundary;
    slice->upper_display_limits[0] = x_max_voxel + boundary;
    slice->lower_display_limits[1] = -boundary;
    slice->upper_display_limits[1] = y_max_voxel + boundary;

    fit_slice_to_view( main, volume_index, view );
}
