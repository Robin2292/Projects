package src.monster;

import ch.aplu.jgamegrid.Actor;
import ch.aplu.jgamegrid.Location;
import src.Game;
import src.MonsterType;

import java.util.ArrayList;
import java.util.List;

/**
 * @author Robin
 * @version 1.0
 */
public class Orion extends Monster {
    private Location dest;
    private ArrayList<Location> visited = new ArrayList<>();

    public Orion(Game game, MonsterType type) {
        super(game, type);
    }

    @Override
    public void walkApproach() {
        List<Actor> unvisited = new ArrayList<>();
        List<Actor> unvisitedGold = new ArrayList<>();

        // 已经有目的地，在路上
        if (dest != null && !getLocation().equals(dest)) {
            Location.CompassDirection compassDir =
                    getLocation().get4CompassDirectionTo(dest);
            setDirection(compassDir);
            Location next = getNextMoveLocation();
            if (!isVisited(next) && canMove(next)) {
                setLocation(next);
            } else {
                walkRandom();
            }
        }

        // 到达目的地
        Actor destActor;
        if (dest != null && getLocation().equals(dest)) {
            visited.add(dest);
            dest = null;
            destActor = null;
            unvisited.remove(destActor);
            unvisitedGold.remove(destActor);
        }

        // information expert: game class knows all locations of gold pieces
        List<Actor> goldPieces = game.getGoldPieces();

        for (Actor actor: goldPieces) {
            if (!visited.contains(actor.getLocation())) {
                unvisited.add(actor);
                if (actor.getIdVisible() != -1) {
                    unvisitedGold.add(actor);
                }
            }
        }

        // if have visited all, revisit
        if (unvisited.size() == 0) {
            visited.clear();
        }

        // prefer locations still have gold
        if (unvisitedGold.size() > 0) {
            int i = randomiser.nextInt(unvisitedGold.size());
            dest = unvisitedGold.get(i).getLocation();
            destActor = unvisitedGold.get(i);
        } else {
            int i = randomiser.nextInt(unvisited.size());
            dest = unvisited.get(i).getLocation();
            destActor = unvisited.get(i);
        }

        Location.CompassDirection compassDir =
                getLocation().get4CompassDirectionTo(dest);
        setDirection(compassDir);

        Location next;
        next = getNextMoveLocation();
        if (!isVisited(next) && canMove(next)) {
            setLocation(next);
        } else {
            walkRandom();
        }
    }
}
