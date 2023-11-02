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
