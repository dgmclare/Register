#include  <def_register.h>

private  void  update_rgb_colour_maps(
    main_struct  *main,
    int          volume )
{
    int   i;

    for_less( i, 0, N_VOXEL_VALUES )
    {
        main->trislice[volume].rgb_colour_map[i] = 
                    get_colour_code( &main->trislice[volume].colour_coding,
                                     (Real) i );
    }
}

private  void  update_cmode_colour_maps(
    main_struct  *main,
    int          volume )
{
    int   i, voxel_value, min_ind, max_ind;

    min_ind = main->trislice[volume].start_colour_map;
    max_ind = min_ind + main->trislice[volume].n_colour_entries-1;

    for_less( voxel_value, 0, N_VOXEL_VALUES )
    {
        main->trislice[volume].cmode_colour_map[voxel_value] = 
                    CONVERT_INTEGER_RANGE( voxel_value, 0, N_VOXEL_VALUES-1,
                                           min_ind, max_ind );
    }

    G_set_bitplanes( main->window, NORMAL_PLANES );

    for_inclusive( i, min_ind, max_ind )
    {
        voxel_value = CONVERT_INTEGER_RANGE( i, min_ind, max_ind,
                                             0, N_VOXEL_VALUES-1 );
        G_set_colour_map_entry( main->window, i, 
                    get_colour_code( &main->trislice[volume].colour_coding,
                                     (Real) voxel_value ) );
    }
}

public  void   update_colour_maps(
    main_struct  *main,
    int          volume )
{
    if( G_get_colour_map_state( main->window ) )
        update_cmode_colour_maps( main, volume );
    else
        update_rgb_colour_maps( main, volume );
}

public  void  set_start_of_colour_maps(
    main_struct  *main,
    int          start_index )
{
    int        volume;
    int        total_colours, n_volume, n_merged;

    total_colours = G_get_n_colour_map_entries( main->window ) - start_index;

    n_merged = Merged_colour_table_fraction * total_colours;

    n_volume = (total_colours - n_merged) / 2;

    if( n_volume > N_VOXEL_VALUES )
        n_volume = N_VOXEL_VALUES;

    n_merged = total_colours - 2 * n_volume;

    main->trislice[0].start_colour_map = start_index;
    main->trislice[0].n_colour_entries = n_volume;

    main->trislice[1].start_colour_map = start_index + n_volume;
    main->trislice[1].n_colour_entries = n_volume;

    main->trislice[2].start_colour_map = start_index + 2 * n_volume;
    main->trislice[2].n_colour_entries = n_merged;

    update_colour_maps( main, 0 );
    update_colour_maps( main, 1 );

    for_less( volume, 0, N_VOLUMES )
        set_recreate_3_slices_flags( main, volume );
}

private  colour_coding_has_changed (
    main_struct          *main,
    int                  volume_index )
{
    update_colour_maps( main, volume_index );

    if( !G_get_colour_map_state( main->window ) )
        set_recreate_3_slices_flags( main, volume_index );
}

public  void   set_volume_colour_coding_type( 
    main_struct          *main,
    int                  volume_index,
    Colour_coding_types  type )
{
    set_colour_coding_type( &main->trislice[volume_index].colour_coding,
                            type );

    colour_coding_has_changed( main, volume_index );
}

public  Colour_coding_types   get_volume_colour_coding_type( 
    main_struct          *main,
    int                  volume_index )
{
    return( main->trislice[volume_index].colour_coding.type );
}

public  void  set_volume_colour_coding_limits(
    main_struct          *main,
    int                  volume_index,
    Real                 min_value,
    Real                 max_value )
{
    set_colour_coding_min_max( &main->trislice[volume_index].colour_coding,
                               min_value, max_value );

    colour_coding_has_changed( main, volume_index );
}