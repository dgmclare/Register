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
static char rcsid[] = "$Header: /private-cvsroot/visualization/Register/Functionality/slices/updates.c,v 1.7 1996-12-09 20:21:34 david Exp $";
#endif

#include <register.h>

public  void  set_recreate_3_slices_flags(
    main_struct  *main,
    int          volume_index )
{
    int  view;

    for_less( view, 0, N_VIEWS )
        set_recreate_slice_flag( main, volume_index, view );
}

public  void  set_recreate_slice_flag(
    main_struct  *main,
    int          volume,
    int          view )
{
    if( volume == MERGED_VOLUME_INDEX )
    {
        main->merged.slices[view].pixels_are_up_to_date = FALSE;
    }
    else
    {
        main->trislice[volume].slices[view].pixels_are_up_to_date = FALSE;
    }
}

public  void  set_update_slice_viewport_flag(
    main_struct     *main,
    int             volume,
    int             view,
    Bitplane_types  bitplane )
{
    set_viewport_update_flag( &main->graphics,
                              get_slice_viewport_index(volume,view),
                              bitplane );
}
