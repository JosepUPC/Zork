
#include "World.h"
#include <stdio.h>

bool World::Puzzle(){
	printf("!!! You go through the portal to appear in the exact same room. There are now four portals.\n");
	char action[30], *fp = action;
	
	int puzzle = 0;
	do {
		printf("Action: ");
		fgets(action, 30, stdin);
		String input(fp);
		Vector<String> Support;
		Support = input.Tokenize(Support);

		if ((Support[0] == "north") || ((Support[0] == "go") && (Support[1] == "north")) && ((puzzle == 0) || (puzzle == 2)) ){
			if (puzzle == 2){
				printf("A door has been opened somewhere. The portals have disappeared\n");
				exits[7].locked = false;
				return true;
			}
			puzzle++;
		}
		else if ((Support[0] == "west") || ((Support[0] == "go") && (Support[1] == "west")) && (puzzle == 1)){
			puzzle++;
		}
		else if ((Support[0] != "south") || ((Support[0] != "go") && (Support[1] != "south"))){
			puzzle = -1;
		}

		else {
			printf("A high pitched scream can be heard. It comes from the other side of the portals, three of them fade away.\n");
			return false;
		}
	} while ( puzzle > 0  );
	return false;


}



bool World::GetRod(const String & part1, const String & part2){
	bool partpiece = false, partcore = false;
	if (((part1 == "piece") && (part2 == "core"))){
		for (int i = 0; i < inventory.num_elements; ++i){
			if ((!partpiece) && inventory.buffer[i].name == items[piece].name)
				partpiece = true;
			if ((!partcore) && inventory.buffer[i].name == items[core].name)
				partcore = true;
		}
		if (partpiece && partcore){
			for (int i = 0; i < inventory.num_elements; ++i){
				if (inventory.buffer[i].name == items[piece].name)
					inventory = clear_at(i, inventory);
			}
			for (int i = 0; i < inventory.num_elements; ++i){
				if (inventory.buffer[i].name == items[core].name)
					inventory = clear_at(i, inventory);
			}		
			}
			inventory.push_back(items[rod]);
			NEWLINE;
			printf("---> The core starts to shine as your piece is attached by a supernatural force. You get the rod.\n");
			return true;
		}
		else
		return false;
}



bool World::LookItem(const String & item) const
{
	for (uint i = 0; i < inventory.num_elements; ++i){
		if (item == inventory.buffer[i].name){
			printf("--> %s", inventory.buffer[i].desc);
			NEWLINE;
			NEWLINE;
			return true;
		}
	}
	return false;
}
void World::Inventory() const {
	uint n_item;
	if (inventory.num_elements == 0){ // It's empty
		printf("The inventory is empty.\n");
	}
	else{
		for (int i = 0; i < inventory.num_elements; ++i){

			printf("-- %d. %s.\n", i, inventory.buffer[i].name);
			/*	for (int i = 0; i < 9; ++i){
					if ((equipment_sub_travel(items[i].name, n_item) && ((i = rod) || (i = sword) || (i = shield)) )){
					printf("Equipped.\n");
					break;
					}
					else if (((i = rod) || (i = sword) || (i = shield))){
					printf("Unequipped.\n");
					break;
					}
					else if (((i = core) || (i = piece))){
					printf("Incomplete.\n");
					break;
					}
					else printf("Regular Item.\n");
					break;
					}
					}*/
		}
	}
	NEWLINE;

}
void World::PickItem(const String & item){
	uint n_room = NULL;
	uint n_item = NULL;
	bool success = false;
	if (inventory.num_elements > 3){ // Are we already carrying 4 objects?
		printf("Your inventory is full! You cannot carry more than 4 objects.\n");
	}
	else {
		for (int i = 0; (i < 9); ++i){
			if ((item == items[i].name) && (room_sub_travel(items[i].name, n_room)) && (!success)){
					if ((players->currentpos == rooms[n_room].name) && (items[i].free))
					{
						success = true;
						printf("You picked %s.\n- \%s.", items[i].name, items[i].desc);
						// Place item in inventory
						inventory.push_back(items[i]);
						// change item 'free' state
						items[i].free = true;
						// change Location
						n_item = item_sub_travel(items[i].name, n_room);
						rooms[n_room].Location = clear_at(n_item, rooms[n_room].Location);
					}
			}
		}
	}
	if (!success) printf("Pick what?\n");
	NEWLINE;
}
void World::DropItem(const String & item){
	uint n_room = NULL;
	uint n_item = NULL;
	bool success = false;
	if (inventory.num_elements == 0){ // It's empty
		printf("The inventory is empty.\n");
	}
	else {
		for (int i = 0; (i < 9); ++i){
			if ((item == items[i].name) && (inventory_sub_travel(items[i].name, n_item)) && (!success)){
				{
					if (items[i].name == inventory.buffer[n_item].name){
						success = true;
						
						// Place item in room
						n_room = player_sub_travel(players->currentpos);
						rooms[n_room].Location.push_back(items[i]);
						// change item 'free' state
						items[i].free = true;
						// change Location
						inventory = clear_at(n_item, inventory);
						if (equipment_sub_travel(items[i].name, n_item)){
							equipment = clear_at(n_item, equipment);
							printf("You unequipped %s.\n", items[i].name);
						}
						printf("You dropped %s.\n", items[i].name);
					}
				}
			}
		}
		if (!success) printf("You don't have such object.\n");
		NEWLINE;
	}
}

void World::EquipItem(const String & item){
	uint n_item = NULL;
	bool success = false;
	for (int i = 0; (i < 9); ++i){
		if ( (    (item == "shield") || (item == "sword") || (item == "rod")   )   &&    (inventory_sub_travel(items[i].name, n_item)) && (!success) && ((i == shield) || (i == sword) || (i == rod)) ){
				success = true;
				printf("You equipped %s.\n", items[i].name);
				// Place item in equipment
				equipment.push_back(items[i]);
		}
	}
	 if(!success) printf("Equip what?\n");
		NEWLINE;
}

void World::UnequipItem(const String & item){
	uint n_item = NULL;
	bool success = false;
	for (int i = 0; (i < 9); ++i){
		if (((item == "shield") || (item == "sword") || (item == "rod")) && (equipment_sub_travel(items[i].name, n_item)) && (!success)){
			success = true;
			printf("You unequipped %s.\n", items[i].name);
			//Clear from equipment
			equipment = clear_at(n_item, equipment);
		}
	}
	if(!success) printf("Unequip what?\n");
	NEWLINE;
}



void World::HelpCommand() const{
	// ------- HELP COMMANDS----------
	printf("If you are in trouble you can try these commands:\ngo + (direction).\nlook or look + (direction)\nopen or unlock door + (direction)\nclose or lock door + (direction)\nquit\n");
	printf("pick or drop + (name of item)\nequip or unequip + (name of item)\ninventory\n");
	NEWLINE;
}

// -------- CLOSE AND OPEN DOORS COMMANDS ------------
void World::CloseDoor( int direction){

	for (int i = 0; (i < 9); ++i){
		if ((players->currentpos == exits[i].room1) && (exits[i].dir1 == direction) || (players->currentpos == exits[i].room2) && (exits[i].dir2 == direction)){
			if (!exits[i].locked){
				exits[i].locked = true;
				printf("You locked the door.\n");
			}
			else printf("Nothing to close\n");
		}
	}
	NEWLINE;
}
void World::OpenDoor(int direction){
	int i = 0;
	for (i = 0; (i < 9); ++i){
		if ((players->currentpos == exits[i].room1) && (exits[i].dir1 == direction) || (players->currentpos == exits[i].room2) && (exits[i].dir2 == direction)){
			if (exits[i].locked == true){
				exits[i].locked = false;
				printf("You unlocked the door.\n");
			}
			else printf("Nothing to open\n");
		}
	}
	NEWLINE;
}
void World::currpos() const{

	for (int i = 0; i < 10; i++){
		if (players->currentpos == rooms[i].name){
			printf("--> %s\n%s\n", rooms[i].name, rooms[i].desc);
			NEWLINE;
		}
	}
	if ((players->currentpos == "Egyptian") && (!puzzle)){
		printf("!!! You can hear a faint noise behind the walls, like hundreds of whispers.\n");
		NEWLINE;
	}
}

// ----------- CHECKS THE EXITS IN BOTH WAYS --------------
bool World::updatepos( int direction){

	int i = 0;
	for (i = 0; (i < 9); ++i){
		if ((players->currentpos == exits[i].room1) && (exits[i].dir1 == direction)){
			if (exits[i].locked == true){ // But can we go that way?
				printf("The door is closed\n");
				return false;
			}
			else {
				players->currentpos = exits[i].room2;
				return true;
			}
		}
	}
	for (i = 0; (i < 9); ++i){
		if ((players->currentpos == exits[i].room2) && (exits[i].dir2 == direction)){
			if (exits[i].locked == true){
				printf("The door is closed\n");
				return false;
			}
			else {
				players->currentpos = exits[i].room1;
				return true;
			}
		}
	}
	printf("There is no exit that way.\n"); //If there's no exit
	return false;
}
// ----------- How does the exit look like? -----------
void World::lookexit( int direction) const{

	for (int i = 0; (i < 10); ++i){
		if ((players->currentpos == exits[i].room1) && (exits[i].dir1 == direction) || (players->currentpos == exits[i].room2) && (exits[i].dir2 == direction)){
			printf("- %s\n%s\n", exits[i].name, exits[i].desc);
			NEWLINE;
		}
	}
}