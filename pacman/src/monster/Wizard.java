package src.monster;

import src.Game;
import src.MonsterType;

/**
 * @author Robin
 * @version 1.0
 */
public class Wizard extends Monster {
    public Wizard(Game game, MonsterType type) {
        super(game, type);
    }

    @Override
    public void walkApproach() {
        walkRandom();
    }
}
