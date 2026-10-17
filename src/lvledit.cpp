
#include "lvledit.hpp"
void swap(i8& x, i8& y){
	x ^= y;
	y ^= x;
	x ^= y;
	return;
}

void WARN_bad_tile_size(u16 size){
	printf("\aWARNING: Tile count %u invalid for map size 300.\n", size);
}

void WARN_OOB(){
	puts("\aWARNING: Coordinate(s) out of bounds.");
}

EditorEntity::EditorEntity(i8 x, i8 y, u8 entityType){
	this->x = x;
	this->y = y;
	this->idx = entityType;
	return;
}

u8 EditorEntity::entityIndex(){
	return this->idx;
}

u8 EditorEntity::xPosition(){
	return this->x;
}
u8 EditorEntity::yPosition(){
	return this->y;
}

EditorTile::EditorTile(){
	this->gfx = 0;
	this->pfx = 0;
	return;
}

EditorTile::EditorTile(u8 graphicsID, u8 propertyID){
	this->gfx = graphicsID;
	this->pfx = propertyID;
	return;
}

u8 EditorTile::graphicsIndex(){
	return this->gfx;
}

u8 EditorTile::propertyIndex(){
	return this->pfx;
}

bool EditorTile::operator == (EditorTile& comprand){
	return ((this->gfx == comprand.gfx) && (this->pfx == comprand.pfx));
}

bool EditorTile::operator != (EditorTile& comprand){
	return !((this->gfx == comprand.gfx) && (this->pfx == comprand.pfx));
}

EditorPoint::EditorPoint(i16 rawPos){
	if(rawPos < 0) rawPos = 0;
	this->x = rawPos % LVL_WIDTH;
	this->y = rawPos / LVL_WIDTH;
	return;
}

EditorPoint::EditorPoint(i8 x, i8 y){
	this->x = x;
	this->y = y;
	return;
}

EditorPoint::EditorPoint(){
	this->x = 0;
	this->y = 0;
	return;
}

i8 EditorPoint::xPosition(){
	return this->x;
}

i8 EditorPoint::yPosition(){
	return this->y;
}

void EditorPoint::adjustCorners(EditorPoint& bottomRight){
	if(bottomRight.x < this->x) swap(bottomRight.x, this->x);
	if(bottomRight.y < this->y) swap(bottomRight.y, this->y);
	return;
}

bool EditorPoint::inBounds(){
	return (this->x < 20 && this->y < 15);
}

EditorLevelMap::EditorLevelMap(){
	this->actors = vector<EditorEntity>();
	return;
}

EditorLevelMap::EditorLevelMap(string directory){

	this->actors = vector<EditorEntity>();

	std::ifstream inputLevelFile(directory);
	if(inputLevelFile.fail()) return;

	u8 incomingFileTransferBuffer[3];

	//Tiles
	{

		EditorPoint pt;
		i16 repeatCurrentTile = 0;
		i16 leveltileWritingHead = 0;
		bool tilemapNotFinished = 1;

		while(tilemapNotFinished){

			inputLevelFile.read((char*)incomingFileTransferBuffer,3);
			repeatCurrentTile = incomingFileTransferBuffer[2];
			if(inputLevelFile.eof()) break;

			while(repeatCurrentTile-- != (i16)-1){

				pt = EditorPoint(leveltileWritingHead);

				tilemapNotFinished = (++leveltileWritingHead < LVL_SIZE);
				if(!tilemapNotFinished) continue;

				this->tiles[pt.yPosition()][pt.xPosition()] = EditorTile(incomingFileTransferBuffer[0],incomingFileTransferBuffer[1]);
			
			}

		}

		if (leveltileWritingHead != LVL_SIZE){

			WARN_bad_tile_size(leveltileWritingHead);
			if(inputLevelFile.eof()) return;

		}

	}

	//Entities
	{

		u8 allowableIterations = 0xff;

		while(allowableIterations--){

			inputLevelFile.read((char*)incomingFileTransferBuffer,3);
			if(inputLevelFile.eof()) break;
			if(incomingFileTransferBuffer[2] == 0x00) break;
			this->actors.push_back(EditorEntity(incomingFileTransferBuffer[0],incomingFileTransferBuffer[1],incomingFileTransferBuffer[2]));

		}

	}

	inputLevelFile.close();

	return;
}

void EditorLevelMap::save(string directory){

	std::ofstream outputLevelFile(directory);
	if(outputLevelFile.fail()) return;
	u8 outgoingFileTransferBuffer[3];

	//Tiles
	{

		i16 leveltileReadingHead = 0;
		u8 tileRepetitions = 0;

		EditorPoint pt = EditorPoint(leveltileReadingHead);

		EditorTile currentOutgoingTile = this->tiles[pt.yPosition()][pt.xPosition()];
		EditorTile candidateOutgoingTile;

		for(leveltileReadingHead = 1; leveltileReadingHead < 300; ++leveltileReadingHead){

			pt = EditorPoint(leveltileReadingHead);
			candidateOutgoingTile = this->tiles[pt.yPosition()][pt.xPosition()];

			if(candidateOutgoingTile == currentOutgoingTile && tileRepetitions != 0xff){
				++tileRepetitions;
				if(leveltileReadingHead < 299) continue;
			}
			
			outgoingFileTransferBuffer[0] = currentOutgoingTile.graphicsIndex();
			outgoingFileTransferBuffer[1] = currentOutgoingTile.propertyIndex();
			outgoingFileTransferBuffer[2] = tileRepetitions;
			outputLevelFile.write((char*)outgoingFileTransferBuffer,3);
			tileRepetitions = 0;
			currentOutgoingTile = candidateOutgoingTile;

		}

	}

	//Entities
	{

		for(EditorEntity givenEntity : this->actors){

			outgoingFileTransferBuffer[0] = givenEntity.xPosition();
			outgoingFileTransferBuffer[1] = givenEntity.yPosition();
			outgoingFileTransferBuffer[2] = givenEntity.entityIndex();

			outputLevelFile.write((char*)outgoingFileTransferBuffer,3);
		}

		outgoingFileTransferBuffer[0] = 0x80;
		outgoingFileTransferBuffer[1] = 0x80;
		outgoingFileTransferBuffer[2] = 0x00;
		outputLevelFile.write((char*)outgoingFileTransferBuffer,3);

	}

	outputLevelFile.close();

}

void EditorLevelMap::save(){
	this->save(string("unsaved.tlb"));
}

void EditorLevelMap::viewTiles(){

	printf("\x1b[1m\n   |");
	for(u8 n = 0; n < 20; ++n){
		printf("  -%02d-  ", n + 1);
	}
	printf("\x1b[0m\n");

	for(u8 y = 0; y < 15; ++y){
		printf("%02d |", y + 1);
		for(u8 x = 0; x < 20; ++x) printf(" %02X (%02X)", this->tiles[y][x].graphicsIndex(), this->tiles[y][x].propertyIndex());
		putc('\n',stdout);
	}
	putc('\n',stdout);

}

void EditorLevelMap::viewEntities(){

	u8 l = 0;
	for(EditorEntity a : actors) printf("%2d) %02X (%2d,%2d)\n", l++, a.entityIndex(), a.xPosition(), a.yPosition());
	
}

void EditorLevelMap::addEntity(EditorEntity entity){

	if(!entity.entityIndex()) return;
	this->actors.push_back(entity);

}

void EditorLevelMap::removeEntity(u8 entityID){

	if(entityID >= this->actors.capacity()) return;
	this->actors.erase(this->actors.begin() + entityID);

}

void EditorLevelMap::setTile(EditorPoint location, EditorTile tile){

	u8 cx = location.xPosition();
	u8 cy = location.yPosition();

	if(!location.inBounds()){
		WARN_OOB();
		return;
	}
	
	this->tiles[cy][cx] = tile;

}

void EditorLevelMap::fillTiles(EditorPoint corner1, EditorPoint corner2, EditorTile tile){

	corner1.adjustCorners(corner2);
	if((!corner1.inBounds()) || (!corner2.inBounds())){
		WARN_OOB();
		return;
	}

	u8 farX = corner2.xPosition();
	u8 farY = corner2.yPosition();

	for(u8 y = corner1.yPosition(); y <= farY; ++y){
		for(u8 x = corner1.xPosition(); x <= farX; ++x){
			this->tiles[y][x] = tile;
		}
	}

}