package src.monster;

import ch.aplu.jgamegrid.Location;
import src.Game;
import src.MonsterType;

import java.util.ArrayList;

/**
 * @author Robin
 * @version 1.0
 */
public class Alien extends Monster {

    public Alien(Game game, MonsterType type) {
        super(game, type);
    }

    @Override
    public void walkApproach() {
        Location pacLoc = game.pacActor.getLocation();
        ArrayList<Location> neighbourLocations = pacLoc.getNeighbourLocations(1.5);

        double shortest = Integer.MAX_VALUE;
        Location dest = null;
        for (Location location: neighbourLocations) {
            double distance = location.getDistanceTo(getLocation());
            if (distance < shortest) {
                shortest = distance;
                dest = location;
            }
        }
        Location.CompassDirection compassDir =
                getLocation().get4CompassDirectionTo(pacLoc);
        Location next = getLocation().getNeighbourLocation(compassDir);
        setDirection(compassDir);
        if (!isVisited(next) && canMove(next)) {
            setLocation(next);
            game.getGameCallback().monsterLocationChanged(this);
            addVisitedList(next);
        }
        walkRandom();

    }

}
