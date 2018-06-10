#pragma region Copyright (c) 2014-2017 OpenRCT2 Developers
/*****************************************************************************
 * OpenRCT2, an open source clone of Roller Coaster Tycoon 2.
 *
 * OpenRCT2 is the work of many authors, a full list can be found in contributors.md
 * For more information, visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * A full copy of the GNU General Public License can be found in licence.txt
 *****************************************************************************/
#pragma endregion

#include "../../interface/Viewport.h"
#include "../../paint/Paint.h"
#include "../../paint/Supports.h"
#include "../RideData.h"
#include "../Track.h"
#include "../TrackPaint.h"
#include "../VehiclePaint.h"

#ifndef NO_VEHICLES
/**
 *
 *  rct2: 0x006D44D5
 */
void vehicle_visual_submarine(paint_session * session, sint32 x, sint32 imageDirection, sint32 y, sint32 z,
                              const rct_vehicle * vehicle, const rct_ride_entry_vehicle * vehicleEntry)
{
    sint32 baseImage_id = imageDirection;
    sint32 image_id;
    if (vehicle->restraints_position >= 64)
    {
        if ((vehicleEntry->sprite_flags & VEHICLE_SPRITE_FLAG_RESTRAINT_ANIMATION) && !(imageDirection & 3))
        {
            baseImage_id /= 8;
            baseImage_id += ((vehicle->restraints_position - 64) / 64) * 4;
            baseImage_id *= vehicleEntry->base_num_frames;
            baseImage_id += vehicleEntry->restraint_image_id;
        }
    }
    else
    {
        if (vehicleEntry->flags & VEHICLE_ENTRY_FLAG_11)
        {
            baseImage_id /= 2;
        }
        if (vehicleEntry->sprite_flags & VEHICLE_SPRITE_FLAG_15)
        {
            baseImage_id /= 8;
        }
        baseImage_id *= vehicleEntry->base_num_frames;
        baseImage_id += vehicleEntry->base_image_id;
        baseImage_id += vehicle->swing_sprite;
    }

    vehicle_boundbox bb = VehicleBoundboxes[vehicleEntry->draw_order][imageDirection / 2];

    image_id =
        baseImage_id | (vehicle->colours.body_colour << 19) | (vehicle->colours.trim_colour << 24) | IMAGE_TYPE_REMAP_2_PLUS;
    paint_struct * ps = sub_98197C(
        session, image_id, 0, 0, bb.length_x, bb.length_y, bb.length_z, z, bb.offset_x, bb.offset_y, bb.offset_z + z);
    if (ps != nullptr)
    {
        ps->tertiary_colour = vehicle->colours_extended;
    }

    image_id = (baseImage_id + 1) | (vehicle->colours.body_colour << 19) | (vehicle->colours.trim_colour << 24) |
               IMAGE_TYPE_REMAP_2_PLUS;
    ps = sub_98197C(session, image_id, 0, 0, bb.length_x, bb.length_y, 2, z, bb.offset_x, bb.offset_y, bb.offset_z + z - 10);
    if (ps != nullptr)
    {
        ps->tertiary_colour = vehicle->colours_extended;
    }

    assert(vehicleEntry->effect_visual == 1);
}
#endif

static void submarine_ride_paint_track_station(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    sint32                   height,
    const rct_tile_element * tileElement)
{
    LocationXY16                             position      = session->MapPosition;
    Ride *                               ride          = get_ride(rideIndex);
    const rct_ride_entrance_definition * entranceStyle = &RideEntranceDefinitions[ride->entrance_style];
    sint32                               heightLower   = height - 16;
    uint32                               imageId;

    if (direction & 1)
    {
        imageId = SPR_TRACK_SUBMARINE_RIDE_MINI_HELICOPTERS_FLAT_SE_NW | session->TrackColours[SCHEME_TRACK];
        sub_98197C(session, imageId, 0, 0, 20, 32, 3, heightLower, 6, 0, heightLower);

        paint_util_push_tunnel_right(session, height, TUNNEL_6);
        track_paint_util_draw_pier(
            session, ride, entranceStyle, position, direction, height, tileElement, session->CurrentRotation);
    }
    else
    {
        imageId = SPR_TRACK_SUBMARINE_RIDE_MINI_HELICOPTERS_FLAT_NE_SW | session->TrackColours[SCHEME_TRACK];
        sub_98197C(session, imageId, 0, 0, 32, 20, 3, heightLower, 0, 6, heightLower);

        paint_util_push_tunnel_left(session, height, TUNNEL_6);
        track_paint_util_draw_pier(
            session, ride, entranceStyle, position, direction, height, tileElement, session->CurrentRotation);
    }

    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 32, 0x20);
}

static void submarine_ride_paint_track_flat(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    sint32                   height,
    const rct_tile_element * tileElement)
{
    LocationXY16 position    = session->MapPosition;
    sint32   heightLower = height - 16;
    uint32   imageId;

    if (direction & 1)
    {
        imageId = SPR_TRACK_SUBMARINE_RIDE_MINI_HELICOPTERS_FLAT_SE_NW | session->TrackColours[SCHEME_TRACK];
        sub_98197C(session, imageId, 0, 0, 20, 32, 3, heightLower, 6, 0, heightLower);
        paint_util_push_tunnel_right(session, heightLower, TUNNEL_0);
    }
    else
    {
        imageId = SPR_TRACK_SUBMARINE_RIDE_MINI_HELICOPTERS_FLAT_NE_SW | session->TrackColours[SCHEME_TRACK];
        sub_98197C(session, imageId, 0, 0, 32, 20, 3, heightLower, 0, 6, heightLower);
        paint_util_push_tunnel_left(session, heightLower, TUNNEL_0);
    }

    if (track_paint_util_should_paint_supports(position))
    {
        metal_a_supports_paint_setup(session, (direction & 1) ? METAL_SUPPORTS_STICK_ALT : METAL_SUPPORTS_STICK, 4, -1,
                                     heightLower, session->TrackColours[SCHEME_SUPPORTS]);
    }

    paint_util_set_segment_support_height(session, paint_util_rotate_segments(SEGMENT_D0 | SEGMENT_C4 | SEGMENT_CC, direction),
                                          0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 16, 0x20);
}

static void submarine_ride_paint_track_left_quarter_turn_3_tiles(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    sint32                   height,
    const rct_tile_element * tileElement)
{
    track_paint_util_left_quarter_turn_3_tiles_paint(
        session, 3, height - 16, direction, trackSequence, session->TrackColours[SCHEME_TRACK],
        trackSpritesSubmarineRideMiniHelicoptersQuarterTurn3Tiles);
    track_paint_util_left_quarter_turn_3_tiles_tunnel(session, height - 16, TUNNEL_0, direction, trackSequence);

    switch (trackSequence)
    {
    case 0:
        metal_a_supports_paint_setup(session, METAL_SUPPORTS_STICK, 4, -1, height - 16, session->TrackColours[SCHEME_SUPPORTS]);
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_D0 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_B4, direction), 0xFFFF, 0);
        break;
    case 2:
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C8 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_B8, direction), 0xFFFF, 0);
        break;
    case 3:
        metal_a_supports_paint_setup(session, METAL_SUPPORTS_STICK, 4, -1, height - 16, session->TrackColours[SCHEME_SUPPORTS]);
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C8 | SEGMENT_C4 | SEGMENT_D4 | SEGMENT_C0, direction), 0xFFFF, 0);
        break;
    }

    paint_util_set_general_support_height(session, height + 16, 0x20);
}

static constexpr const uint8 submarine_ride_right_quarter_turn_3_tiles_to_left_turn_map[] = { 3, 1, 2, 0 };
static void                  submarine_ride_paint_track_right_quarter_turn_3_tiles(
                     paint_session *          session,
                     uint8                    rideIndex,
                     uint8                    trackSequence,
                     uint8                    direction,
                     sint32                   height,
                     const rct_tile_element * tileElement)
{
    trackSequence = submarine_ride_right_quarter_turn_3_tiles_to_left_turn_map[trackSequence];
    submarine_ride_paint_track_left_quarter_turn_3_tiles(session, rideIndex, trackSequence, (direction + 3) % 4, height,
                                                         tileElement);
}

static void submarine_ride_paint_track_left_quarter_turn_1_tile(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    sint32                   height,
    const rct_tile_element * tileElement)
{
    track_paint_util_left_quarter_turn_1_tile_paint(
        session, 1, height - 16, 0, direction, session->TrackColours[SCHEME_TRACK],
        trackSpritesSubmarineRideMiniHelicoptersQuarterTurn1Tile);
    track_paint_util_left_quarter_turn_1_tile_tunnel(session, direction, height - 16, 0, TUNNEL_0, 0, TUNNEL_0);

    paint_util_set_segment_support_height(
        session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C8 | SEGMENT_C4 | SEGMENT_D0, direction), 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 16, 0x20);
}

static void submarine_ride_paint_track_right_quarter_turn_1_tile(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    sint32                   height,
    const rct_tile_element * tileElement)
{
    submarine_ride_paint_track_left_quarter_turn_1_tile(session, rideIndex, trackSequence, (direction + 3) % 4, height,
                                                        tileElement);
}

/**
 * rct2: 0x008995D4
 */
TRACK_PAINT_FUNCTION get_track_paint_function_submarine_ride(sint32 trackType, sint32 direction)
{
    switch (trackType)
    {
    case TRACK_ELEM_BEGIN_STATION:
    case TRACK_ELEM_MIDDLE_STATION:
    case TRACK_ELEM_END_STATION:
        return submarine_ride_paint_track_station;

    case TRACK_ELEM_FLAT:
        return submarine_ride_paint_track_flat;

    case TRACK_ELEM_LEFT_QUARTER_TURN_3_TILES:
        return submarine_ride_paint_track_left_quarter_turn_3_tiles;
    case TRACK_ELEM_RIGHT_QUARTER_TURN_3_TILES:
        return submarine_ride_paint_track_right_quarter_turn_3_tiles;

    case TRACK_ELEM_LEFT_QUARTER_TURN_1_TILE:
        return submarine_ride_paint_track_left_quarter_turn_1_tile;
    case TRACK_ELEM_RIGHT_QUARTER_TURN_1_TILE:
        return submarine_ride_paint_track_right_quarter_turn_1_tile;
    }

    return nullptr;
}