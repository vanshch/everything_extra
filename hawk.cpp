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
double dist(Greed::coords a, Greed::coords b) {
		return sqrt(pow(a.r - b.r, 2) + pow(a.c - b.c, 2));
	}
void newThread(Event e, ship& ob, deque<shipInfo> shipList) {
		
			deque<Event> q = ob.getPassiveEvent();

			for (int i = 0; i < q.size(); i++)
			{
				if (q[i].eventType == Event::EventType::ShipsInMyRadius)
				{

					for (int j = 0; j < q[i].radiusShip.getShipId().size(); j++)
					{

						if (q[i].radiusShip.getShipId()[j] == -1)
						{
							continue;
						}
						fire(ob, minDist(ob, shipList[q[i].radiusShip.getShipId()[j]]), q[i].radiusShip.getShipId()[j]);
						//wheremove(shipList[q[i].radiusShip.getShipId()[j]], ob);
					}
				}
				if (q[i].eventType == Event::EventType::CannonsInMyRadius)
				{
					vector<Greed::cannon> ls = ob.getCannonList();
					if (ls[q[i].radiusCannon.getCannonId()[0]].isCannonDead() == 0)
					{
						ob.Greed_fireAtCannon(q[i].radiusCannon.getCannonId()[0], cannon::FRONT);
					}
				}
				if (q[i].eventType == Event::EventType::NoAmmo) {
					ob.Greed_upgradeAmmo(ob.getCurrentGold() / 2);
				}
				if (q[i].eventType == Event::EventType::NoFuel) {
					ob.Greed_upgradeFuel(ob.getCurrentGold() / 2);
				}
				if (q[i].eventType == Event::EventType::LowFuel) {
					if (ob.getCurrentGold() >= 30) {
						ob.Greed_upgradeFuel(20);
					}
					else {
						ob.Greed_upgradeFuel(ob.getCurrentGold() / 4);
					}
				}
				if (q[i].eventType == Event::EventType::IamInShipRadius) {
					fire(ob, minDist(ob, shipList[q[i].radiusShip.getShipId()[0]]), q[i].radiusShip.getShipId()[0]);///kya shi hai 
					if (q[i].radiusShip.getShipId().size() >= 2) {
						ob.Greed_setPath(shipList[find_ship_to_kill(shipList, ob.getShipId(), ob, q[i].radiusShip.getShipId()[0])].getShipId());
					}
					else {
						//wheremove(shipList[q[i].radiusShip.getShipId()[0]], ob);
					}
				}
				if (q[i].eventType == Event::EventType::IamInCannonRadius) {
					for (int j = 0;j < 3;j++) {
						ob.Greed_fireAtCannon(e.cannonFire.getInfo().at(0).id, cannon::FRONT);
					}
					if (e.cannonFire.getInfo().at(0).getTargetShipId() != ob.getShipId()) {
						ob.Greed_fireAtCannon(e.cannonFire.getInfo().at(0).id, cannon::FRONT);
						ob.Greed_fireAtCannon(e.cannonFire.getInfo().at(0).id, cannon::FRONT);
						ob.Greed_fireAtCannon(e.cannonFire.getInfo().at(0).id, cannon::FRONT);
					}
					else {
						goSafe(ob);
					}
				}
			}



  //full algo

  #include "greed_offline.hpp"
#include "stdio.h"
#include "math.h"
int ships_left(deque<shipInfo>& shipList) {
	int count = 0;
	for (int i = 0;i < shipList.size();i++) if (!shipList[i].getDiedStatus()) count++;
	return count-1;
	}
int min(double a[]) {
	double  mini = a[0];
	int x = 4;
	int index;
	for (int i = 0; i < x; i++)
	{

		if (a[i] < mini)
		{
			mini = a[i];//a heuristic to find the best prey
			index = i;
		}
	}
	return index;
}
int confront_id1;
int confront_id2;
int radar5id;
double count1 = 0;
double arr[6] = {0,0,6,35016,73,22};
void prefire(ship& ob, deque<shipInfo> shipList, int index);
double dist(Greed::coords, Greed::coords);
void same(ship& ob, shipInfo info);
void finaleThread(ship& ob, deque<shipInfo>& shipList, int index) {
	deque<Event> q = ob.getPassiveEvent();
	for (int i = 0;i < q.size();i++) {
		if (q[i].eventType == Event::EventType::LowHealth) {
			if (ob.getCurrentGold() >= 250) {
				ob.Greed_upgradeHealth(40);
			}
			else {
				ob.Greed_upgradeHealth(ob.getCurrentGold() / 7);
			}
			if (q[i].eventType == Event::EventType::NoAmmo) {
				ob.Greed_upgradeAmmo(ob.getCurrentGold() / 2);
			}
			if (q[i].eventType == Event::EventType::NoFuel) {
				ob.Greed_upgradeFuel(ob.getCurrentGold() / 2);
			}
			if (q[i].eventType == Event::EventType::LowFuel) {
				if (ob.getCurrentGold() >= 30) {
					ob.Greed_upgradeFuel(20);
				}
				else {
					ob.Greed_upgradeFuel(ob.getCurrentGold() / 4);
				}
			}
		}

	}

}
void finale(ship& ob, deque<shipInfo>& shipList, int index) {
	//checking 

	while (!shipList[index].getDiedStatus() && ob.getDistance(index)<5) {
		prefire(ob, shipList,index);
	}	
}
Direction prep(Direction dir) {
	switch (dir) {
	case Direction::NORTH:
		return Direction::EAST;
	case Direction::EAST:
		return Direction::SOUTH;
	case Direction::SOUTH:
		return Direction::WEST;
	case Direction::WEST:
		return Direction::NORTH;
	case Direction::NORTH_WEST:
		return Direction::NORTH_EAST;
	case Direction::NORTH_EAST:
		return Direction::SOUTH_EAST;
	case Direction::SOUTH_EAST:
		return Direction::SOUTH_WEST;
	case Direction::SOUTH_WEST:
		return Direction::NORTH_WEST;

	}
}

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
void opposite(ship& ob, shipInfo info) {
	if (info.getShipDirection() == Direction::WEST) {
		ob.Greed_sail(Direction::EAST, 1);
	}
	else if (info.getShipDirection() == Direction::EAST) {
		ob.Greed_sail(Direction::WEST, 1);
	}
	else if (info.getShipDirection() == Direction::NORTH) {
		ob.Greed_sail(Direction::SOUTH, 1);
	}
	else if (info.getShipDirection() == Direction::SOUTH) {
		ob.Greed_sail(Direction::NORTH, 1);
	}
	else if (info.getShipDirection() == Direction::NORTH_EAST) {
		ob.Greed_sail(Direction::NORTH_WEST, 1);
	}
	else if (info.getShipDirection() == Direction::NORTH_WEST) {
		ob.Greed_sail(Direction::NORTH_EAST, 1);
	}
	else if (info.getShipDirection() == Direction::SOUTH_EAST) {
		ob.Greed_sail(Direction::SOUTH_WEST, 1);
	}
	else if (info.getShipDirection() == Direction::SOUTH_WEST) {
		ob.Greed_sail(Direction::SOUTH_EAST, 1);
	}
}
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

void goSafe(ship& ob){
		if (ob.getCurrentTile().c >= 4 &&  ob.getCurrentTile().r < 6) {
			ob.Greed_setPath(Greed::coords(6,2));
		}
		else if (ob.getCurrentTile().c >= 9 && ob.getCurrentTile().c <= 15 && ob.getCurrentTile().r > 2 && ob.getCurrentTile().r <= 7) {
			if (ob.getCurrentTile().r > 5) {
				ob.Greed_setPath(Greed::coords(9, 9));
			}
			else {
				ob.Greed_setPath(Greed::coords(1, 9));
			}
		}
	}
	
double dist(Greed::coords a, Greed::coords b) {
		return sqrt(pow(a.r - b.r, 2) + pow(a.c - b.c, 2));
	}
int minDist(ship& ob, shipInfo info) { // front 0 rear 1
	double rr = dist(ob.getCurrentRearTile(), info.getCurrentRearTile());
	double rf = dist(ob.getCurrentRearTile(), info.getCurrentTile());
	double fr = dist(ob.getCurrentTile(), info.getCurrentRearTile());
	double ff = dist(ob.getCurrentTile(), info.getCurrentTile());
	double pos[] = { rr,rf,fr,ff };
	int min2 = min(pos);
	if (min2 == 0) {//1 11 2 10 3 01 4 00
		return 1;
	}
	if (min2 == 1) {

		return 2;
	}
	if (min2 == 2) {
		return 3;
	}
	if (min2 == 3) {
		return 4;
	}
}


bool checkNearObstacle(ship& ob) {
	if (ob.getCurrentTile().r == 0 || ob.getCurrentTile().c == 0 || ob.getCurrentTile().c == 23 || ob.getCurrentTile().r == 11) {
		return true;
	}
	return false;
}
void wheremove(shipInfo info, ship& ob) {
	int p = minDist(ob, info);
	int x = p;
	Greed::coords taret, mine;
	if (x == 1 || x == 3)
		taret = info.getCurrentRearTile();
	else
		taret = info.getCurrentTile();

	if (x == 1 || x == 2)
		mine = info.getCurrentRearTile();
	else
		mine = info.getCurrentTile();


	if (info.getShipDirection() == Direction::NA || checkNearObstacle(ob)) {
		deque<shipInfo> x = ob.getShipList();
		ob.chaseShip(find_ship_to_kill(x, ob.getShipId(), ob, info.getShipId()));
		return;
	}

	if (mine.r > taret.r) {
		if (info.getShipDirection() == Direction::SOUTH || info.getShipDirection() == Direction::SOUTH_EAST || info.getShipDirection() == Direction::SOUTH_WEST) {
			same(ob, info);
		}
		else if (info.getShipDirection() == Direction::EAST || info.getShipDirection() == Direction::WEST) {
			opposite(ob, info);
		}
		else {
			opposite(ob, info);
		}
	}
	else if (mine.r < taret.r) {
		if (info.getShipDirection() == Direction::SOUTH || info.getShipDirection() == Direction::SOUTH_EAST || info.getShipDirection() == Direction::SOUTH_WEST) {
			opposite(ob, info);
		}
		else if (info.getShipDirection() == Direction::EAST || info.getShipDirection() == Direction::WEST) {
			opposite(ob, info);
		}
		else {
			same(ob, info);
		}
	}
	else if (mine.c < taret.r) {
		if (info.getShipDirection() == Direction::WEST || info.getShipDirection() == Direction::NORTH_WEST || info.getShipDirection() == Direction::SOUTH_WEST) {
			same(ob, info);
		}
		else if (info.getShipDirection() == Direction::NORTH || info.getShipDirection() == Direction::SOUTH) {
			opposite(ob, info);
		}
		else {
			opposite(ob, info);
		}
	}
	else if (mine.c > taret.r) {
		if (info.getShipDirection() == Direction::WEST || info.getShipDirection() == Direction::NORTH_WEST || info.getShipDirection() == Direction::SOUTH_WEST) {
			opposite(ob, info);
		}
		else if (info.getShipDirection() == Direction::NORTH || info.getShipDirection() == Direction::SOUTH) {
			opposite(ob, info);
		}
		else {
			same(ob, info);
		}
	}

}

void same(ship& ob, shipInfo info) {
		if (info.getShipDirection() == Direction::WEST) {
			ob.Greed_sail(Direction::WEST, 1);
		}
		else if (info.getShipDirection() == Direction::EAST) {
			ob.Greed_sail(Direction::EAST, 1);
		}
		else if (info.getShipDirection() == Direction::NORTH_EAST) {
			ob.Greed_sail(Direction::NORTH_EAST, 1);
		}
		else if (info.getShipDirection() == Direction::NORTH_WEST) {
			ob.Greed_sail(Direction::NORTH_WEST, 1);
		}
		else if (info.getShipDirection() == Direction::SOUTH_EAST) {
			ob.Greed_sail(Direction::SOUTH_EAST, 1);
		}
		else if (info.getShipDirection() == Direction::SOUTH_WEST) {
			ob.Greed_sail(Direction::SOUTH_WEST, 1);
		}
	}

void fire(ship& ob, int p, int id) {
	//cout << "entered the fire function" << endl;
	for (int i = 0;i < 3;i++) {
		if (p == 1) ob.Greed_fireCannon(cannon::REAR, id, ShipSide::REAR);
		else if (p == 2)
			ob.Greed_fireCannon(cannon::REAR, id, ShipSide::FRONT);
		else if (p == 3) ob.Greed_fireCannon(cannon::FRONT, id, ShipSide::REAR);
		else
			ob.Greed_fireCannon(cannon::FRONT, id, ShipSide::FRONT);
	}
}

void newThread(Event e, ship& ob, deque<shipInfo> shipList) {
		
			deque<Event> q = ob.getPassiveEvent();

			for (int i = 0; i < q.size(); i++)
			{
				if (q[i].eventType == Event::EventType::ShipsInMyRadius)
				{

					for (int j = 0; j < q[i].radiusShip.getShipId().size(); j++)
					{

						if (q[i].radiusShip.getShipId()[j] == -1)
						{
							continue;
						}
						//fire(ob, minDist(ob, shipList[q[i].radiusShip.getShipId()[j]]), q[i].radiusShip.getShipId()[j]);
						wheremove(shipList[q[i].radiusShip.getShipId()[j]], ob);
					}
				}
				if (q[i].eventType == Event::EventType::CannonsInMyRadius)
				{
					vector<Greed::cannon> ls = ob.getCannonList();
					if (ls[q[i].radiusCannon.getCannonId()[0]].isCannonDead() == 0)
					{
						ob.Greed_fireAtCannon(q[i].radiusCannon.getCannonId()[0], cannon::FRONT);
					}
				}
				if (q[i].eventType == Event::EventType::NoAmmo) {
					ob.Greed_upgradeAmmo(ob.getCurrentGold() / 2);
				}
				if (q[i].eventType == Event::EventType::NoFuel) {
					ob.Greed_upgradeFuel(ob.getCurrentGold() / 2);
				}
				if (q[i].eventType == Event::EventType::LowFuel) {
					if (ob.getCurrentGold() >= 30) {
						ob.Greed_upgradeFuel(20);
					}
					else {
						ob.Greed_upgradeFuel(ob.getCurrentGold() / 4);
					}
				}
				if (q[i].eventType == Event::EventType::IamInShipRadius) {
					fire(ob, minDist(ob, shipList[q[i].radiusShip.getShipId()[0]]), q[i].radiusShip.getShipId()[0]);///kya shi hai 
					if (q[i].radiusShip.getShipId().size() >= 2) {
						ob.Greed_setPath(shipList[find_ship_to_kill(shipList, ob.getShipId(), ob, q[i].radiusShip.getShipId()[0])].getShipId());
					}
					else {
						//wheremove(shipList[q[i].radiusShip.getShipId()[0]], ob);
					}
				}
				if (q[i].eventType == Event::EventType::IamInCannonRadius) {
					for (int j = 0;j < 3;j++) {
						ob.Greed_fireAtCannon(e.cannonFire.getInfo().at(0).id, cannon::FRONT);
					}
					if (e.cannonFire.getInfo().at(0).getTargetShipId() != ob.getShipId()) {
						ob.Greed_fireAtCannon(e.cannonFire.getInfo().at(0).id, cannon::FRONT);
						ob.Greed_fireAtCannon(e.cannonFire.getInfo().at(0).id, cannon::FRONT);
						ob.Greed_fireAtCannon(e.cannonFire.getInfo().at(0).id, cannon::FRONT);
					}
					else {
						goSafe(ob);
					}
				}
			}
	}

void prefire(ship& ob, deque<shipInfo> shipList,int index) {
	ob.Greed_sail(prep(shipList[index].getShipDirection()), 1);	
	fire(ob, 1, index);
}

void main2(ship& ob, int index) {

		deque<shipInfo> shipList = ob.getShipList();
		//cout << ob.getAbsolutePosition().x << " " << ob.getAbsolutePosition().y << '\n';
		ob.Greed_setPath(Greed::coords(6, 7));
		if (ships_left(shipList) == 1) {
			//cout << "shipleft = 1 " << endl;
			radar5id = index;
			//cout << "before prefire" << '\n';
			//finale(ob,shipList, index);
			//prefire(ob, shipList);
			finale(ob, shipList, index);
			//cout << "after prefire" << '\n';
		}
		if (radar5) prefire(ob,shipList,radar5id);
		Event e;
		ob.getNextCurrentEvent(e);
		if (e.eventType == Event::EventType::LowHealth) {
			if (ob.getCurrentGold() >= 250) {
				ob.Greed_upgradeHealth(40);
			}
			else {
				ob.Greed_upgradeHealth(ob.getCurrentGold() / 7);
			}
		}
		if (static_cast<int>(e.eventType) == static_cast<int>(Event::EventType::ShipFire))

		{
			for (auto it : e.shipFire.getShipId())
			{
				int p = minDist(ob, shipList[it.first]);
				while (static_cast<int>(e.eventType) == 4) {
					fire(ob, p, it.first);
					//cout<< "fire  " << it.first;
					//wheremove(shipList[it.first], ob);
				}
				
			}
			//is making new threaD important for funnctioning of code 
			newThread(e, ob, shipList);
		}
		if (e.eventType == Event::EventType::LowAmmo)
		{
			if (ob.getCurrentGold() >= 20) {
				ob.Greed_upgradeAmmo(15);
			}
			else {
				ob.Greed_upgradeAmmo(ob.getCurrentGold() / 2);
			}
		}
		if (e.eventType == Event::EventType::ShipCollision)
		{
			//cout << "\n my ship collided," << e.shipCollision.getShipId()[0] << " finding new hunt..";

			int p = minDist(ob, shipList[e.shipCollision.getShipId()[0]]);
			fire(ob, p, e.shipCollision.getShipId()[0]);
			//wheremove(shipList[e.shipCollision.getShipId()[0]], ob);
			index = find_ship_to_kill(shipList, ob.getShipId(), ob, e.shipCollision.getShipId()[0]);
			if (index != -1)
			{
				//cout << "\n new hunt is==>" << index;
				ob.Greed_chaseShip(index);
			}
			if (ob.getCurrentAmmo() < 15)
			{
				if (ob.getCurrentGold() >= 20) {
					ob.Greed_upgradeAmmo(10);
				}
				else {
					ob.Greed_upgradeAmmo(ob.getCurrentGold() / 2);
				}
			}
		}

		if (e.eventType == Event::EventType::CannonFire) {//later 
			for (int j = 0;j < 3;j++) {
				ob.Greed_fireAtCannon(e.cannonFire.getInfo().at(0).id, cannon::FRONT);
			}


			ob.chaseShip(shipList[find_ship_to_kill(shipList, ob.getShipId(), ob, ob.getShipId())].getShipId());
		}


	}
void GreedMain(ship& ob)
	{
		//setting the aim
	cout << "manga" << '\n';
		vector <Greed::cannon> cannonList = ob.getCannonList();//has to be called multiple times
		for (int j = 0; j < cannonList.size(); j++)
		{
			vector<Greed::coords> arr = ob.getRadiusCoords_cannon(j);
			for (int i = 0; i < arr.size(); i++)
			{
				ob.Greed_updateCost(arr[i], 50);
			}
		}

		deque<shipInfo> shipList = ob.getShipList();

		
		int index = find_ship_to_kill(shipList, ob.getShipId(), ob, ob.getShipId());
		/*if (ships_left(shipList) == 1) {
			radar5id = index;
			cout << "before prefire" << '\n';
			prefire(ob, shipList);
			cout << "after prefire" << '\n';
		}*/
		

		//the code is giving correct id 
		//index 

		/*if (index >= 0 && shipList[index].getDiedStatus() == 0)
		{
			index = find_ship_to_kill(shipList, ob.getShipId(), ob, ob.getShipId());
			if (index != -1)
			{
				ob.Greed_chaseShip(index);
			}

		}*/

		int frame_rate = 0;
		while (1)
		{

			if (ob.frame_rate_limiter())
			{
				cout << "entered the main2"<<'\n';
				main2(ob, index);
				
			}

		}


	}
	}
