#include "global.h"
#include "event_data.h"
#include "fieldmap.h"
#include "random.h"
#include "winstons_burrow.h"
#include "constants/layouts.h"
#include "constants/vars.h"

#define BURROW_WIDTH 36
#define BURROW_HEIGHT 24

#define BURROW_START_X 2
#define BURROW_START_Y 21
#define BURROW_EXIT_X 33
#define BURROW_EXIT_Y 2

#define METATILE_RusturfTunnel_Floor 0x201
#define METATILE_RusturfTunnel_FloorAlt 0x202
#define METATILE_RusturfTunnel_CrackedFloor 0x207
#define METATILE_RusturfTunnel_Ladder 0x216
#define METATILE_RusturfTunnel_Rock 0x211
#define METATILE_RusturfTunnel_RockAlt1 0x205
#define METATILE_RusturfTunnel_RockAlt2 0x206
#define METATILE_RusturfTunnel_RockAlt3 0x21B
#define METATILE_RusturfTunnel_RockAlt4 0x21C
#define METATILE_RusturfTunnel_RockAlt5 0x219
#define METATILE_RusturfTunnel_RockAlt6 0x209

struct BurrowRoom
{
    s16 x;
    s16 y;
    s16 width;
    s16 height;
    s16 centerX;
    s16 centerY;
};

EWRAM_DATA static bool8 sBurrowOpen[BURROW_HEIGHT][BURROW_WIDTH] = {0};

u16 BurrowDisabledSpecial(void)
{
    return 0;
}

static u16 GetBurrowFloorMetatile(rng_value_t *rng)
{
    u16 roll = LocalRandom(rng) % 16;

    if (roll == 0)
        return METATILE_RusturfTunnel_CrackedFloor;
    if (roll < 4)
        return METATILE_RusturfTunnel_FloorAlt;
    return METATILE_RusturfTunnel_Floor;
}

static u16 GetBurrowRockMetatile(rng_value_t *rng, s16 x, s16 y)
{
    static const u16 rockMetatiles[] =
    {
        METATILE_RusturfTunnel_Rock,
        METATILE_RusturfTunnel_RockAlt1,
        METATILE_RusturfTunnel_RockAlt2,
        METATILE_RusturfTunnel_RockAlt3,
        METATILE_RusturfTunnel_RockAlt4,
        METATILE_RusturfTunnel_RockAlt5,
        METATILE_RusturfTunnel_RockAlt6,
    };

    if ((x == 0 || y == 0 || x == BURROW_WIDTH - 1 || y == BURROW_HEIGHT - 1)
     || (LocalRandom(rng) % 5) != 0)
        return METATILE_RusturfTunnel_Rock;

    return rockMetatiles[LocalRandom(rng) % ARRAY_COUNT(rockMetatiles)];
}

static u32 GetBurrowSeed(u16 floor)
{
    u32 trainerId = gSaveBlock2Ptr->playerTrainerId[0]
                  | (gSaveBlock2Ptr->playerTrainerId[1] << 8)
                  | (gSaveBlock2Ptr->playerTrainerId[2] << 16)
                  | (gSaveBlock2Ptr->playerTrainerId[3] << 24);
    u32 playTime = gSaveBlock2Ptr->playTimeVBlanks
                 | (gSaveBlock2Ptr->playTimeSeconds << 8)
                 | (gSaveBlock2Ptr->playTimeMinutes << 16)
                 | (gSaveBlock2Ptr->playTimeHours << 24);

    return Random32() ^ trainerId ^ playTime ^ (floor * 0x45D9F3B);
}

static void ClearBurrowOpenTiles(void)
{
    s16 x, y;

    for (y = 0; y < BURROW_HEIGHT; y++)
    {
        for (x = 0; x < BURROW_WIDTH; x++)
            sBurrowOpen[y][x] = FALSE;
    }
}

static void CarveBurrowTile(s16 x, s16 y)
{
    if (x > 0 && x < BURROW_WIDTH - 1 && y > 0 && y < BURROW_HEIGHT - 1)
        sBurrowOpen[y][x] = TRUE;
}

static void CarveBurrowRoom(const struct BurrowRoom *room)
{
    s16 x, y;

    for (y = room->y; y < room->y + room->height; y++)
    {
        for (x = room->x; x < room->x + room->width; x++)
            CarveBurrowTile(x, y);
    }
}

static void CarveBurrowNarrowHorizontal(s16 x1, s16 x2, s16 y)
{
    s16 x;

    if (x1 > x2)
    {
        s16 temp = x1;
        x1 = x2;
        x2 = temp;
    }

    for (x = x1; x <= x2; x++)
        CarveBurrowTile(x, y);
}

static void CarveBurrowNarrowVertical(s16 y1, s16 y2, s16 x)
{
    s16 y;

    if (y1 > y2)
    {
        s16 temp = y1;
        y1 = y2;
        y2 = temp;
    }

    for (y = y1; y <= y2; y++)
        CarveBurrowTile(x, y);
}

static void CarveBurrowNarrowSegment(s16 x1, s16 y1, s16 x2, s16 y2)
{
    if (x1 == x2)
    {
        CarveBurrowNarrowVertical(y1, y2, x1);
    }
    else if (y1 == y2)
    {
        CarveBurrowNarrowHorizontal(x1, x2, y1);
    }
}

static void CarveBurrowMainPath(void)
{
    static const s16 path[][2] =
    {
        {2, 21},
        {2, 20},
        {10, 20},
        {10, 16},
        {18, 16},
        {18, 11},
        {26, 11},
        {26, 6},
        {32, 6},
        {32, 2},
        {33, 2},
    };
    u8 i;

    for (i = 1; i < ARRAY_COUNT(path); i++)
        CarveBurrowNarrowSegment(path[i - 1][0], path[i - 1][1], path[i][0], path[i][1]);
}

static struct BurrowRoom MakeBurrowRoom(s16 x, s16 y, s16 width, s16 height)
{
    struct BurrowRoom room;

    room.x = x;
    room.y = y;
    room.width = width;
    room.height = height;
    room.centerX = x + width / 2;
    room.centerY = y + height / 2;

    return room;
}

static void CarveBurrowFixedRooms(void)
{
    struct BurrowRoom startRoom = MakeBurrowRoom(1, 19, 5, 3);
    struct BurrowRoom exitRoom = MakeBurrowRoom(31, 1, 4, 4);
    struct BurrowRoom optionalRoom1 = MakeBurrowRoom(3, 14, 5, 4);
    struct BurrowRoom optionalRoom2 = MakeBurrowRoom(24, 14, 5, 4);

    CarveBurrowRoom(&startRoom);
    CarveBurrowRoom(&exitRoom);
    CarveBurrowRoom(&optionalRoom1);
    CarveBurrowRoom(&optionalRoom2);

    CarveBurrowNarrowSegment(5, 20, 5, 16);
    CarveBurrowNarrowSegment(22, 11, 22, 15);
    CarveBurrowNarrowSegment(22, 15, 26, 15);
}

static void CarveBurrowRandomFalsePaths(rng_value_t *rng)
{
    static const struct
    {
        s16 x;
        s16 y;
        s8 dx;
        s8 dy;
    } branchPoints[] =
    {
        {6, 20, 0, 1},
        {14, 16, 0, 1},
        {18, 13, -1, 0},
        {26, 8, 1, 0},
        {30, 6, 0, 1},
    };
    u8 i;

    for (i = 0; i < ARRAY_COUNT(branchPoints); i++)
    {
        s16 x = branchPoints[i].x;
        s16 y = branchPoints[i].y;
        s16 length = 3 + (LocalRandom(rng) % 5);
        s16 endX = min(BURROW_WIDTH - 2, max(1, x + branchPoints[i].dx * length));
        s16 endY = min(BURROW_HEIGHT - 2, max(1, y + branchPoints[i].dy * length));

        CarveBurrowNarrowSegment(x, y, endX, endY);
    }
}

static void CarveBurrowLayout(rng_value_t *rng)
{
    ClearBurrowOpenTiles();
    CarveBurrowFixedRooms();
    CarveBurrowMainPath();
    CarveBurrowRandomFalsePaths(rng);
}

static void SetBurrowMapTile(s16 x, s16 y, u16 block)
{
    u16 *map = gBackupMapLayout.map;
    map[(y + MAP_OFFSET) * gBackupMapLayout.width + x + MAP_OFFSET] = block;
}

static void WriteBurrowLayoutToMap(rng_value_t *rng)
{
    s16 x, y;

    for (y = 0; y < BURROW_HEIGHT; y++)
    {
        for (x = 0; x < BURROW_WIDTH; x++)
        {
            if (sBurrowOpen[y][x])
                SetBurrowMapTile(x, y, PACK_ELEVATION(ELEVATION_DEFAULT) | GetBurrowFloorMetatile(rng));
            else
                SetBurrowMapTile(x, y, PACK_COLLISION(1) | GetBurrowRockMetatile(rng, x, y));
        }
    }

    SetBurrowMapTile(BURROW_START_X, BURROW_START_Y, PACK_ELEVATION(ELEVATION_DEFAULT) | METATILE_RusturfTunnel_Floor);
    SetBurrowMapTile(BURROW_START_X, BURROW_START_Y - 1, PACK_ELEVATION(ELEVATION_DEFAULT) | METATILE_RusturfTunnel_Floor);
    SetBurrowMapTile(BURROW_EXIT_X, BURROW_EXIT_Y, PACK_ELEVATION(ELEVATION_DEFAULT) | METATILE_RusturfTunnel_Ladder);
}

void TryGenerateWinstonsBurrowMap(void)
{
    u16 floor;
    rng_value_t rng;

    if (gMapHeader.mapLayoutId != LAYOUT_RUSTURF_TUNNEL)
        return;

    floor = VarGet(VAR_WINSTONS_BURROW_FLOOR);
    if (floor == 0)
        floor = 1;

    rng = LocalRandomSeed(GetBurrowSeed(floor));
    CarveBurrowLayout(&rng);
    WriteBurrowLayoutToMap(&rng);
}
