#include "tilemap.h"
#define CUTE_TILED_IMPLEMENTATION
#include "external/cute_tiled.h"

static Texture gameMap = { 0 };

std::vector<Rectangle> GridCollisionCoord;

TileMap::TileMap() : MapGrid() {}
TileMap::~TileMap() {}


// Function to load the map file into a buffer
static std::vector<char> load_file(const std::string& filename) {
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filename << std::endl;
		return {};
	}

	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<char> buffer(size);
	if (!file.read(buffer.data(), size)) {
		std::cerr << "Failed to read file: " << filename << std::endl;
		return {};
	}

	return buffer;
}

//// Add a helper function to convert cute_tiled_string_t to const char*
//static const char* cute_tiled_string_to_cstr(cute_tiled_string_t str) {
//	return str.ptr; // Assuming `ptr` contains the C-string representation
//} // this serves no purpose


void TileMap::LoadMap()

{
	if (gameMap.id == 0)
	
	{
		gameMap = LoadTexture("assets/Tileset.png");
	}

	auto buffer = load_file("assets/untitled.json");

	if (!buffer.empty()) {

		cute_tiled_map_t* map = cute_tiled_load_map_from_memory(buffer.data(), static_cast<int>(buffer.size()), 0);
		// Update the problematic line to use the helper function
		/*const char* tileset_image_path = cute_tiled_string_to_cstr(map->tilesets->image);*/
	/*	Texture2D tileset_texture = LoadTexture(tileset_image_path);*/

		int tile_width = map->tilewidth;
		int tile_height = map->tileheight;
		int tileset_columns = map->tilesets->imagewidth / tile_width;

		cute_tiled_layer_t* layer = map->layers;

		if (layer && layer->data) {
			for (int y = 0; y < layer->height; ++y) {
				for (int x = 0; x < layer->width; ++x) {
					int tile_index = y * layer->width + x;
					int gid = layer->data[tile_index];
					if (gid == 0) continue; // 0 means no tile

					int local_id = gid - map->tilesets->firstgid;
					int sx = (local_id % tileset_columns) * tile_width;
					int sy = (local_id / tileset_columns) * tile_height;

					Rectangle src = { (float)sx, (float)sy, (float)tile_width, (float)tile_height };
					Rectangle dest = { (float)(x * tile_width), (float)(y * tile_height), (float)tile_width, (float)tile_height };
					/*DrawTextureRec(tileset_texture, src, { dest.x, dest.y }, WHITE);*/ // will remove later
					this->MapGrid.push_back(std::make_pair(src, dest));
					GridCollisionCoord.push_back(dest);
				}
			}
		}
	/*UnloadTexture(tileset_texture);*/
	}
}

//void TileMap::MakeMap()
//
//// TODO <-Make the actual map and try to optimise it->
//{
//
//	for (int i = 0;i < 21;i++) {
//		for (int j = 0;j < 40;j++) {
//			this->MapCoord += "00";
//		}
//	}
//	for (int j = 0;j < 40;j++) {
//		if (j > 21) {
//			this->MapCoord += "01";
//		}
//		else {
//			this->MapCoord += "00";
//		}
//	}
//
//	this->MapCoord += "01";
//	for (int i = 0; i < 38;i++)
//	
//	{
//		if (i != 30 &&  i!=31) {
//			this->MapCoord += "02";
//		}
//		else {
//			this->MapCoord += "00";
//		}
//	}
//
//	for (int i = 0;i < 24;i++) {
//		for (int j = 0;j < 40;j++) {
//			this->MapCoord += "00";
//		}
//	}
//
//
//
//	this->MapCoord += "01";
//	for (int i = 0; i < 38;i++)
//
//	{
//		if (i != 30 && i!=31) {
//			this->MapCoord += "02";
//		}
//		else {
//			this->MapCoord += "00";
//		}
//	}
//
//
//	this->MapCoord += "63";
//	int size = this->MapCoord.size();
//	int whichTile1 = -1;
//	int whichTile2 = -1;
//	int whichTile = -1;
//	int tileX = -1;
//	int tileY = -1;
//	int desX = -1;
//	int desY = -1;
//	Vector2 tile = { 0 };
//	Vector2 des = { 0 };
//
//	for (int i = 0; i < (size)-1; i+=2)
//
//	{
//		if (this->MapCoord[i] == '0' && this->MapCoord[i + 1] == '0')
//		
//		{
//			continue;
//		}
//
//		else 
//		
//		{
//			whichTile1 = this->MapCoord[i] - '0';
//			whichTile2 = this->MapCoord[i+1] - '0';
//			whichTile = ((10 * whichTile1) + whichTile2)-1;
//
//			tileX = whichTile % 8;
//			tileY = whichTile / 8;
//			desX = ((i / 2) % 40) + (40*(i/2000));
//			desY = ((i / 2) / 40) - (25*(i/2000));
//
//
//			tile.x = tileX;
//			tile.y = tileY;
//			des.x = desX;
//			des.y = desY;
//			
//			MapGrid.push_back(std::make_pair(tile, des));
//
//			Rectangle x = { desX * 32, desY * 32, 32, 32 };
//			//std::cout << x.x << " "<< x.y << " ";
//			GridCollisionCoord.push_back(x);
//		}
//	}
//
//} // Will remove later

void TileMap::DrawMap()

{
	for (int i = 0;i < MapGrid.size();i++)
	
	{
		DrawTexturePro(gameMap,MapGrid[i].first,MapGrid[i].second, {0,0}, 0.0f, WHITE);
	
	}
}

void TileMap::UnloadMap()

{
	UnloadTexture(gameMap);
}
