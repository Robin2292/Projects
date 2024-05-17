package src.mode;

import ch.aplu.jgamegrid.Location;
import src.Game;

/**
 * @author Robin
 * @version 1.0
 */
public class MultiverseGameMode implements GameMode {
    @Override
    public void gameInitialise(Game game) {
        game.setUpMultiverseGame(game);
    }

    @Override
    public void eatPill(Game game, Location location) {

    }

    @Override
    public void eatGold(Game game, Location location) {
        game.makeMonstersFurious();
    }

    @Override
    public void eatIce(Game game, Location location) {
        game.makeMonstersFrozen();
    }
}
