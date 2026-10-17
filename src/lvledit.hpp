#pragma once
#include "common.hpp"
#ifndef LVLEDIT_ASTC_CPP

	#define LVLEDIT_ASTC_CPP

	#define LVL_WIDTH 20
	#define LVL_HEIGT 15
	#define LVL_SIZE LVL_WIDTH * LVL_HEIGT

	using std::vector;
	using std::string;

	class EditorEntity{

		private:

		i8 x;
		i8 y;
		u8 idx;

		public:

		EditorEntity(i8 x, i8 y, u8 entityType);
		u8 entityIndex();
		u8 xPosition();
		u8 yPosition();

	};

	class EditorTile{

		private:

		u8 gfx;
		u8 pfx;

		public:

		EditorTile();
		EditorTile(u8 graphicsID, u8 propertyID);

		u8 graphicsIndex();
		u8 propertyIndex();

		bool operator == (EditorTile& comprand);
		bool operator != (EditorTile& comprand);

	};

	class EditorPoint{

		private:

		i8 x;
		i8 y;

		public:

		EditorPoint(i8 x, i8 y);
		EditorPoint(i16 rawPos);
		EditorPoint();
		i8 xPosition();
		i8 yPosition();
		void adjustCorners(EditorPoint& bottomRight);
		bool inBounds();

	};

	class EditorLevelMap{
		
		private:
		EditorTile tiles[LVL_HEIGT][LVL_WIDTH];
		vector<EditorEntity> actors;

		public:
		EditorLevelMap();
		EditorLevelMap(string directory);
		void save(string directory);
		void save();

		void viewTiles();
		void viewEntities();

		void addEntity(EditorEntity entity);
		void removeEntity(u8 entityID);

		void setTile(EditorPoint location, EditorTile tile);
		void fillTiles(EditorPoint corner1, EditorPoint corner2, EditorTile tile);

	};

#endif
