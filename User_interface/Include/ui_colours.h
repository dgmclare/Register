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

#ifndef  DEF_UI_COLOURS
#define  DEF_UI_COLOURS

#ifndef lint
static char ui_colours_rcsid[] = "$Header: /private-cvsroot/visualization/Register/User_interface/Include/ui_colours.h,v 1.7 1998-06-29 15:01:51 david Exp $";
#endif

typedef  enum
{
    BACKGROUND_COLOUR,
    DIVIDER_COLOUR,
    BUTTON_ACTIVE_COLOUR,
    BUTTON_SELECTED_COLOUR,
    BUTTON_INACTIVE_COLOUR,
    BUTTON_TEXT_COLOUR,
    TEXT_ENTRY_ACTIVE_COLOUR,
    TEXT_ENTRY_SELECTED_COLOUR,
    TEXT_ENTRY_INACTIVE_COLOUR,
    TEXT_ENTRY_EDIT_COLOUR,
    TEXT_ENTRY_EDIT_TEXT_COLOUR,
    TEXT_ENTRY_TEXT_COLOUR,
    TEXT_ENTRY_CURSOR_COLOUR,
    LABEL_ACTIVE_COLOUR,
    LABEL_SELECTED_COLOUR,
    LABEL_INACTIVE_COLOUR,
    LABEL_TEXT_COLOUR,
    SLIDER_ACTIVE_COLOUR,
    SLIDER_INACTIVE_COLOUR,
    SLIDER_PEG_COLOUR,
    VOLUME1_UNDER_COLOUR,
    VOLUME1_OVER_COLOUR,
    VOLUME2_UNDER_COLOUR,
    VOLUME2_OVER_COLOUR,
    N_UI_COLOURS
} UI_colours;

#endif
