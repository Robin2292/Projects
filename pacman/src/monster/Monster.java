// Monster.java
// Used for PacMan
package src.monster;

import ch.aplu.jgamegrid.*;
import src.Game;
import src.MonsterType;

import java.awt.Color;
import java.util.*;

public abstract class Monster extends Actor {
    protected Game game;
    private MonsterType type;
    private ArrayList<Location> visitedList = new ArrayList<Location>();
    private final int listLength = 10;
    private boolean stopMoving = false;
    private int seed = 0;
    protected Random randomiser = new Random(0);
    protected boolean furious = false;
    protected boolean frozen = false;
    public static final int FURIOUS_TIME = 3;
    public static final int FROZEN_TIME = 3;




    public Monster(Game game, MonsterType type) {
        super("sprites/" + type.getImageName());
        this.game = game;
        this.type = type;
    }

    public void getFurious(int seconds) {
        if (frozen) return;
        furious = true;
        Timer timer = new Timer(); // Instantiate Timer Object
        int SECOND_TO_MILLISECONDS = 1000;
        final Monster monster = this;
        timer.schedule(new TimerTask() {
            @Override
            public void run() {
                monster.furious = false;
            }
        }, seconds * SECOND_TO_MILLISECONDS);
    }

    public void stopMoving(int seconds) {
        this.stopMoving = true;
        Timer timer = new Timer(); // Instantiate Timer Object
        int SECOND_TO_MILLISECONDS = 1000;
        final Monster monster = this;
        timer.schedule(new TimerTask() {
            @Override
            public void run() {
                monster.stopMoving = false;
            }
        }, seconds * SECOND_TO_MILLISECONDS);
    }

    public void getFrozen(int seconds) {
        frozen = true;
        this.stopMoving = true;
        Timer timer = new Timer(); // Instantiate Timer Object
        int SECOND_TO_MILLISECONDS = 1000;
        final Monster monster = this;
        timer.schedule(new TimerTask() {
            @Override
            public void run() {
                monster.stopMoving = false;
                monster.frozen = false;
            }
        }, seconds * SECOND_TO_MILLISECONDS);
    }

    public void setSeed(int seed) {
        this.seed = seed;
        randomiser.setSeed(seed);
    }

    public void setStopMoving(boolean stopMoving) {
        this.stopMoving = stopMoving;
    }

    public void act() {
        if (stopMoving) {
            return;
        }
        walk();
        if (getDirection() > 150 && getDirection() < 210)
            setHorzMirror(false);
        else
            setHorzMirror(true);
    }

    public abstract void walkApproach();
    public void walk() {
        if (furious) {
            walkApproach();
            walkApproach();
        } else {
            walkApproach();
        }
    }

    public MonsterType getType() {
        return type;
    }

    public void addVisitedList(Location location) {
        visitedList.add(location);
        if (visitedList.size() == listLength)
            visitedList.remove(0);
    }

    public boolean isVisited(Location location) {
        for (Location loc : visitedList)
            if (loc.equals(location))
                return true;
        return false;
    }

    public boolean canMove(Location location) {
        Color c = getBackground().getColor(location);
        if (c.equals(Color.gray) || location.getX() >= game.getNumHorzCells()
                || location.getX() < 0 || location.getY() >= game.getNumVertCells() || location.getY() < 0)
            return false;
        else
            return true;
    }

    public void walkRandom() {
        double oldDirection = getDirection();
        // Walking approach:
        Location next;
        // Random walk
        int sign = randomiser.nextDouble() < 0.5 ? 1 : -1;
        setDirection(oldDirection);
        turn(sign * 90);  // Try to turn left/right
        next = getNextMoveLocation();
        if (canMove(next)) {
            setLocation(next);
        } else {
            setDirection(oldDirection);
            next = getNextMoveLocation();
            if (canMove(next)) {// Try to move forward
                setLocation(next);
            } else {
                setDirection(oldDirection);
                turn(-sign * 90);  // Try to turn right/left
                next = getNextMoveLocation();
                if (canMove(next)) {
                    setLocation(next);
                } else {
                    setDirection(oldDirection);
                    turn(180);  // Turn backward
                    next = getNextMoveLocation();
                    setLocation(next);
                }
            }
        }
        game.getGameCallback().monsterLocationChanged(this);
        addVisitedList(next);
    }
}

