package src.monster;

import ch.aplu.jgamegrid.Location;
import src.Game;
import src.MonsterType;

/**
 * @author Robin
 * @version 1.0
 */
public class Troll extends Monster {
    public Troll(Game game, MonsterType type) {
        super(game, type);
    }

    @Override
    public void walkApproach() {
        // random walk
        walkRandom();
    }


}
