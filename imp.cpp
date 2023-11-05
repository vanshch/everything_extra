
//finding the number of ships left 


int ships_left(deque<shipInfo>& shipList) {
	int count = 0;
	for (int i = 0;i < shipList.size();i++) if (!shipList[i].getDiedStatus()) count++;
	return count-1;
	}
int confront_id1;
int confront_id2;
int radar5id;
double count1 = 0;
double arr[6] = {0,0,6,35016,73,22};
double dist(Greed::coords, Greed::coords);


//finding the confrontation


bool confrontation(deque<shipInfo>& shipList) {
	//remove our id for smoother exp
	for (int i = 0;i < shipList.size();i++) {
		for (int j = 0;j < shipList.size();j++) {
			if (i == j) continue;
			if (dist(shipList[i].getCurrentTile(), shipList[j].getCurrentTile())<=5) {
				//cout << dist(shipList[i].getCurrentTile(), shipList[j].getCurrentTile()) << '/n';
				confront_id1 = i;
				confront_id2 = j;
				return true;
			}
		}
	}
	return false;
}

//part of the direction finding sub



Direction direct_sub(int x, int y) {
	float tan = y/x;
	if (x >= 0) {
		if (tan > 1) return Direction::NORTH;
		else if (tan <= 1 && tan > 0) return Direction::NORTH_EAST;
		else if (tan <= 0 && tan > -1) return Direction::SOUTH_EAST;
		else if (tan <= -1) return Direction::SOUTH;
	}
	else {
		if (tan > 1) return Direction::SOUTH;
		else if (tan <= 1 && tan > 0) return Direction::SOUTH_WEST;
		else if (tan <= 0 && tan > -1) return Direction::NORTH_WEST;
		else if (tan <= -1) return Direction::SOUTH;
	}
}

//part of the direction finding functions 

bool direct(Greed::coords a, Greed::coords ar,ship& ob) {
	//shifting origin 
	int vecx, vecy = 0;
	vecx = ar.c - a.c;
	vecy = ar.r - a.r;
	int posx, posy;
	posx = ob.getAbsolutePosition().x - a.c;
	posy = ob.getAbsolutePosition().y - a.r;
	if (direct_sub(vecx, vecy) == direct_sub(posx, posy)) return true;
	return false;
}

//checking if any ship is in radius 
//only invoke when the event is none 


bool radar5(deque<shipInfo>& shipList, ship&ob) {
	radar5id = -1;
	for (int i = 0;i < shipList.size();i++) if (dist(ob.getCurrentTile(), shipList[i].getCurrentTile()) <= 5.0) {
		if (direct(shipList[i].getCurrentTile(), shipList[i].getCurrentRearTile(), ob)) {
			radar5id = i;
			return true;
		}
	};
	return false;
}

//finding other ships to kill

int find_ship_to_kill(deque<shipInfo>& shipList, int myid, ship& ob,int hate_id)
{
		//making a cont var
	int cont = 0;
		if (ships_left(shipList) == 1) {
			//strat for last game is premove 
			for (int i = 0;i <shipList.size();i++) if (!shipList[i].getDiedStatus() && i !=ob.getShipId()) return i;
		}
			if (confrontation(shipList)) {
				if (confrontation(shipList) == 1 && count1++ > arr[shipList.size()]) {
					Greed::coords myPos = ob.getCurrentTile();
					Greed::coords myPos1 = shipList[confront_id1].getCurrentTile();
					Greed::coords myPos2 = shipList[confront_id2].getCurrentTile();
					if (shipList[confront_id1].getCurrentHealth() <= shipList[confront_id1].getCurrentHealth()) return confront_id1;
					else return confront_id2;
				}
			}
			return -1;
			}
