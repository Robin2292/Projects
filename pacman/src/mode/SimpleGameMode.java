package src.mode;

import ch.aplu.jgamegrid.Location;
import src.Game;

/**
 * @author Robin
 * @version 1.0
 */
public class SimpleGameMode implements GameMode{

    @Override
    public void gameInitialise(Game game) {
        game.setUpSimpleGame(game);
    }

    @Override
    public void eatPill(Game game, Location location) {

    }

    @Override
    public void eatGold(Game game, Location location) {

    }

    @Override
    public void eatIce(Game game, Location location) {

    }
}
