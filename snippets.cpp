//making the ship to evade bullets 
void evadeBullets(ship& ob) {
    // Get the current event
    Event e;
    ob.getNextCurrentEvent(e);
    //change the eventtype to shipfire
    if (e.eventType == Event::EventType::CannonFire){
        // Get information about the incoming cannon fire
        CannonFireInfo cannonFireInfo = e.cannonFire.getInfo()[0];

        // Calculate the direction perpendicular to the bullet's trajectory
        Direction evadeDirection = ob.getPerpendicularDirection(cannonFireInfo.getDirection());

        // Calculate the new position to evade the bullet by 2 tiles
        Greed::coords currentPosition = ob.getCurrentTile();
        Greed::coords evadePosition = currentPosition + ob.getDirectionOffset(evadeDirection, 2);

        // Set the path to move to the evade position
        ob.Greed_setPath(evadePosition);
    }
}
// invoking the function if the ship is fired upon 

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
int wheremove(shipInfo info, ship& ob) {
	int* p = minDist(ob, info);
	int x = *(p + 1);
	Greed::coords taret, mine;
	if (x)
		taret = info.getCurrentRearTile();
	else
		taret = info.getCurrentTile();

	x = *p;
	if (x)
		mine = info.getCurrentRearTile();
	else
		mine = info.getCurrentTile();

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

	if (info.getShipDirection() == Direction::NA) {
		int r = info.getCurrentTile().r + 3;
		int c = info.getCurrentTile().c + 3;
		ob.Greed_setPath(Greed::coords(r, c));
	}
}
