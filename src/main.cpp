//#pragma once
#include "common.hpp"
#include "lvledit.hpp"
#define ARGSC 6
using std::cin;
using std::cout;
using std::getline;
using std::string;

int main(){
	
	char cmd;
	i16 nargs[ARGSC];
	string darg;
	EditorLevelMap level;
	bool loop = true;

	while(loop){

		cin
		>> cmd;

		switch(cmd){

			case 'n':{

				level = EditorLevelMap();
				cout << "New Levelmap created.\n";
				break;

			}
			case 'l':{

				getline(cin,darg);
				cout << "Attempting to load levelmap from file " << darg << "...\n";
				level = EditorLevelMap(darg);
				cout << "Levelmap loaded.\n";
				break;

			}
			case 's':{

				getline(cin,darg);
				cout << "Attempting to save levelmap to file " << darg << "...\n";
				level.save(darg);
				cout << "Levelmap saved.\n";
				break;

			}
			case 'x':{

				cout << "Exiting program.\n";
				level.save();
				loop = false;
				break;

			}
			case 'e':{

				cin
				>> std::hex >> nargs[0]
				>> std::dec >> nargs[1] >> nargs[2];

				level.addEntity(
					EditorEntity(
					(i8)--nargs[1],
					(i8)--nargs[2],
					(u8)nargs[0])
				);
				break;

			}
			case 'E':{

				cin >> std::dec >> nargs[0];

				level.removeEntity((u8)nargs[0]);
				break;

			}
			case 't':{

				cin
				>> std::hex >> nargs[0] >> nargs[1]
				>> std::dec >> nargs[2] >> nargs[3];

				level.setTile(
					EditorPoint(
					(i8)nargs[2] - 1,
					(i8)nargs[3] - 1),
					EditorTile(
					(u8)nargs[0],
					(u8)nargs[1])
				);
				break;

			}
			case 'T':{

				cin
				>> std::hex >> nargs[0] >> nargs[1]
				>> std::dec >> nargs[2] >> nargs[3]
							>> nargs[4] >> nargs[5];

				level.fillTiles(
					EditorPoint(
					(i8)nargs[2] - 1,
					(i8)nargs[3] - 1),
					EditorPoint(
					(i8)nargs[4] - 1,
					(i8)nargs[5] - 1),
					EditorTile(
					(u8)nargs[0],
					(u8)nargs[1])
				);

				break;

			}
			case 'd':{

				level.viewTiles();
				break;

			}
			case 'D':{

				level.viewEntities();
				break;

			}
			default:{

				cout << "WARNING: Command not recognized." << '\n';
				break;

			}

		}


	}
	return 0;
}