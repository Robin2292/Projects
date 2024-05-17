package src.monster;

import ch.aplu.jgamegrid.Location;
import src.Game;
import src.MonsterType;

/**
 * @author Robin
 * @version 1.0
 */
public class TX5 extends Monster {
    public final static int STOP_MOVING_TIME = 5;

    public TX5(Game game, MonsterType type) {
        super(game, type);
    }

    @Override
    public void walkApproach() {
        Location pacLocation = game.pacActor.getLocation();

        // Walking approach:
        // TX5: Determine direction to pacActor and try to move in that direction. Otherwise, random walk.
        Location.CompassDirection compassDir =
                getLocation().get4CompassDirectionTo(pacLocation);
        Location next = getLocation().getNeighbourLocation(compassDir);
        setDirection(compassDir);
        if (!isVisited(next) && canMove(next)) {
            setLocation(next);
        } else {
            walkRandom();
        }
    }
}
