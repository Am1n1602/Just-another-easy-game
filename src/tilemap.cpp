#include "tilemap.h"
#define CUTE_TILED_IMPLEMENTATION
#include "external/cute_tiled.h"


std::vector<Rectangle> GridCollisionCoord;
std::vector<Rectangle> ObjectCollisionCoord;
std::deque <Rectangle> SavePointQueue;
Vector2 SourceForSave;
std::deque <Rectangle> SavePointQueueBackup;


TileMap::TileMap() : MapGrid() {}
TileMap::~TileMap() {}
Objects Lava;
Animation AnimLava;

static Texture gameMap = { 0 };

// Function to load the map file into a buffer
static std::vector<char> load_file(const std::string& filename)

{

	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	if (!file.is_open())

	{
		std::cerr << "Failed to open file: " << filename << std::endl;
		return {};
	}

	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<char> buffer(size);
	if (!file.read(buffer.data(), size))

	{
		std::cerr << "Failed to read file: " << filename << std::endl;
		return {};
	}

	return buffer;
}



void TileMap::LoadMap()

{
	if (gameMap.id == 0)

	{
		gameMap = LoadTexture("assets/tile21.png");
	}

	auto buffer = load_file("assets/TileMap1.json");

	if (!buffer.empty())

	{

		cute_tiled_map_t* map = cute_tiled_load_map_from_memory(buffer.data(), static_cast<int>(buffer.size()), 0);

		int tile_width = map->tilewidth;
		int tile_height = map->tileheight;
		int tileset_columns = map->tilesets->imagewidth / tile_width;

		cute_tiled_layer_t* layer = map->layers;

		if (layer && layer->data)

		{
			for (int y = 0; y < layer->height; ++y)

			{
				for (int x = 0; x < layer->width; ++x)

				{
					int tile_index = y * layer->width + x;
					int gid = layer->data[tile_index];
					if (gid == 0) continue; // 0 means no tile

					int local_id = gid - map->tilesets->firstgid;

					int sx = (local_id % tileset_columns) * tile_width;
					int sy = (local_id / tileset_columns) * tile_height;

					Rectangle src = { (float)sx, (float)sy, (float)tile_width, (float)tile_height };

					Rectangle dest = {};
					Rectangle dest1 = {};
					tile_width *= 2;
					tile_height *= 2;

					if (gid == 51) // Right Spikes

					{

						dest = { (float)(x * tile_width), (float)(y * tile_height), (float)tile_width, (float)tile_height };
						dest1 = { ((float)(x * tile_width) + tile_width / 2)+2, (float)(y * tile_height) + 2, ((float)tile_width/4), (float)tile_height - 4 };
						ObjectCollisionCoord.push_back(dest1);

					}

					else if (gid == 52) // Downward Spikes

					{

						dest = { (float)(x * tile_width), (float)(y * tile_height), (float)tile_width, (float)tile_height };
						dest1 = { (float)(x * tile_width+1)+2, (float)(y * tile_height+1), (float)tile_width-2, (((float)tile_height-1) / 2)-2 };
						ObjectCollisionCoord.push_back(dest1);

					}

					else if (gid == 39) // Upward Spike

					{

						dest = { (float)(x * tile_width), (float)(y * tile_height), (float)tile_width, (float)tile_height };
						dest1 = { (float)(x * tile_width+2) , (float)(y * tile_height) + 3*tile_height / 4, (float)tile_width - 4, (float)tile_height / 2 };
						ObjectCollisionCoord.push_back(dest1);

					}

					else if (gid == 60) // lava

					{

						LavaPoint.push_back({ (float)(x * tile_width), (float)(y * tile_height), (float)tile_width, (float)tile_height });
						dest1 = { (float)(x * tile_width)+5 , (float)(y * tile_height+1), (float)tile_width - 8, (float)tile_height };
						ObjectCollisionCoord.push_back(dest1);

					}

					else if (gid == 50) //left spike

					{

						dest = { (float)(x * tile_width), (float)(y * tile_height), (float)tile_width, (float)tile_height };
						dest1 = { (float)(x * tile_width+1), (float)(y * tile_height+1), ((float)tile_width / 2)-1, ((float)tile_height-1) };
						ObjectCollisionCoord.push_back(dest1);

					}

					else if (gid == 16) // Card

					{

						Rectangle dester = { (float)(x * tile_width), (float)(y * tile_height), (float)tile_width, (float)tile_height };
						SavePointQueue.push_back(dester);
						SavePointQueueBackup.push_back(dester);
						SourceForSave.x = src.x;
						SourceForSave.y = src.y;

					}

					else if (gid == 35) // half slab

					{
						dest = { (float)(x * tile_width), (float)(y * tile_height), (float)tile_width, (float)tile_height };
						dest1 = { (float)(x * tile_width + 1), (float)(y * tile_height + 1)+tile_height/4, (float)tile_width, (float)tile_height/6 };
					}

					else if (gid == 22) // left outward slab
					{

						dest = { (float)(x * tile_width), (float)(y * tile_height), (float)tile_width, (float)tile_height };
						dest1 = { (float)(x * tile_width ), (float)(y * tile_height)+1 , (float)tile_width, 3*(float)tile_height/4 };
					}

					else if (gid == 32 || gid == 33 || gid == 45 || gid == 46)
					{
						dest = { (float)(x * tile_width), (float)(y * tile_height), (float)tile_width, (float)tile_height };
					}

					else

					{

						dest = { (float)(x * tile_width), (float)(y * tile_height), (float)tile_width, (float)tile_height };
						dest1 = { (float)(x * tile_width+1), (float)(y * tile_height+1), (float)tile_width-1, (float)tile_height-1 };

					}

					this->MapGrid.push_back(std::make_pair(src, dest));
					GridCollisionCoord.push_back(dest1);

					tile_width *= 0.5;
					tile_height *= 0.5;
				}
			}
		}

	}
}


void TileMap::DrawMap()

{
	for (int i = 0;i < MapGrid.size();i++)

	{

		DrawTexturePro(gameMap, MapGrid[i].first, MapGrid[i].second, { 0,0 }, 0.0f, WHITE);

	}

	//for (int i = 0;i < GridCollisionCoord.size();i++) {
	//	DrawRectangleLinesEx({ GridCollisionCoord[i].x, GridCollisionCoord[i].y, GridCollisionCoord[i].width, GridCollisionCoord[i].height}, 1, SKYBLUE);
	//}

}

//void TileMap::DrawObjects()
//
//{
//	for (int i = 0;i < SavePointQueue.size();i++)
//
//	{
//		DrawTexturePro(gameMap, { SourceForSave.x,SourceForSave.y,16,16 }, SavePointQueue[i], { 0,0 }, 0.0f, WHITE);
//
//	}
//}

void TileMap::UnloadMap()

{

	UnloadTexture(gameMap);

}
