package src.mode;

import ch.aplu.jgamegrid.Location;
import src.Game;

/**
 * @author Robin
 * @version 1.0
 */
public interface GameMode {
    void gameInitialise(Game game);
    void eatPill(Game game, Location location);
    void eatGold(Game game, Location location);
    void eatIce(Game game, Location location);
}
