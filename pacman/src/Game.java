// PacMan.java
// Simple PacMan implementation
package src;

import ch.aplu.jgamegrid.*;
import src.mode.GameMode;
import src.mode.MultiverseGameMode;
import src.mode.SimpleGameMode;
import src.monster.*;
import src.utility.GameCallback;

import java.awt.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Properties;

/**
 * lots of magic numbers
 * information expert, tx5 knows stop 5s initially
 * lots of repeated block of codes
 */

public class Game extends GameGrid
{
  private final static int nbHorzCells = 20;
  private final static int nbVertCells = 11;
  protected PacManGameGrid grid = new PacManGameGrid(nbHorzCells, nbVertCells);
  private static final int SLOW_DOWN = 3;

  private GameMode gameMode;
  public PacActor pacActor = new PacActor(this);
  private ArrayList<Monster> monsters = new ArrayList<>();

  private ArrayList<Location> pillAndItemLocations = new ArrayList<Location>();
  private ArrayList<Actor> iceCubes = new ArrayList<Actor>();
  private ArrayList<Actor> goldPieces = new ArrayList<Actor>();
  private GameCallback gameCallback;
  private Properties properties;
  private int seed = 30006;
  private ArrayList<Location> propertyPillLocations = new ArrayList<>();
  private ArrayList<Location> propertyGoldLocations = new ArrayList<>();

  public GameMode getGameMode() {
    return gameMode;
  }

  public void setGameMode(Properties properties) {
    switch (properties.getProperty("version")) {
      case "simple":
        gameMode = new SimpleGameMode();
        break;
      case "multiverse":
        gameMode = new MultiverseGameMode();
        break;
    }
  }

  public List<Actor> getGoldPieces() {
    return goldPieces;
  }

  public Game(GameCallback gameCallback, Properties properties)
  {
    //Setup game
    super(nbHorzCells, nbVertCells, 20, false);
    this.gameCallback = gameCallback;
    this.properties = properties;
    setSimulationPeriod(100);
    setTitle("[PacMan in the Multiverse]");

    setGameMode(properties);

    //Setup for auto test
    pacActor.setPropertyMoves(properties.getProperty("PacMan.move"));
    pacActor.setAuto(Boolean.parseBoolean(properties.getProperty("PacMan.isAuto")));
    loadPillAndItemsLocations();

    GGBackground bg = getBg();
    drawGrid(bg);

    gameMode.gameInitialise(this);

    //Setup Random seeds
    seed = Integer.parseInt(properties.getProperty("seed"));
    pacActor.setSeed(seed);

    addKeyRepeatListener(pacActor);
    setKeyRepeatPeriod(150);

    pacActor.setSlowDown(SLOW_DOWN);
    for(Monster monster: monsters) {
      monster.setSeed(seed);
      monster.setSlowDown(SLOW_DOWN);
    }
    for (Monster monster: monsters) {
      if (monster instanceof TX5) {
        monster.stopMoving(TX5.STOP_MOVING_TIME);
      }
    }
    setupPacManLocations();



    //Run the game
    doRun();
    show();
    // Loop to look for collision in the application thread
    // This makes it improbable that we miss a hit
    boolean hasPacmanBeenHit = false;
    boolean hasPacmanEatAllPills;
    setupPillAndItemsLocations();
    int maxPillsAndItems = countPillsAndItems();
    
    do {
      for (Monster monster: monsters) {
        if (monster.getLocation().equals(pacActor.getLocation())) {
          hasPacmanBeenHit = true;
          break;
        }
      }
      hasPacmanEatAllPills = pacActor.getNbPills() >= maxPillsAndItems;
      delay(10);
    } while(!hasPacmanBeenHit && !hasPacmanEatAllPills);
    delay(120);

    Location loc = pacActor.getLocation();
    for (Monster monster: monsters) {
      monster.setStopMoving(true);
    }

    pacActor.removeSelf();

    String title = "";
    if (hasPacmanBeenHit) {
      bg.setPaintColor(Color.red);
      title = "GAME OVER";
      addActor(new Actor("sprites/explosion3.gif"), loc);
    } else if (hasPacmanEatAllPills) {
      bg.setPaintColor(Color.yellow);
      title = "YOU WIN";
    }
    setTitle(title);
    gameCallback.endOfGame(title);

    doPause();
  }

  public GameCallback getGameCallback() {
    return gameCallback;
  }

  public void setUpSimpleGame(Game game) {
    Monster troll = new Troll(this, MonsterType.Troll);
    Monster tx5 = new TX5(this, MonsterType.TX5);

    monsters.add(troll);
    monsters.add(tx5);

    String[] trollLocations = this.properties.getProperty("Troll.location").split(",");
    String[] tx5Locations = this.properties.getProperty("TX5.location").split(",");

    int trollX = Integer.parseInt(trollLocations[0]);
    int trollY = Integer.parseInt(trollLocations[1]);

    int tx5X = Integer.parseInt(tx5Locations[0]);
    int tx5Y = Integer.parseInt(tx5Locations[1]);

    addActor(troll, new Location(trollX, trollY), Location.NORTH);
    addActor(tx5, new Location(tx5X, tx5Y), Location.NORTH);
  }

  public void setUpMultiverseGame(Game game) {
    setUpSimpleGame(game);
    Monster alien = new Alien(this, MonsterType.Alien);
    Monster orion = new Orion(this, MonsterType.Orion);
    Monster wizard = new Wizard(this, MonsterType.Wizard);

    monsters.add(alien);
    monsters.add(orion);
    monsters.add(wizard);

    String[] alienLocations = this.properties.getProperty("Alien.location").split(",");
    String[] orionLocations = this.properties.getProperty("Orion.location").split(",");
    String[] wizardLocations = this.properties.getProperty("Wizard.location").split(",");

    int alienX = Integer.parseInt(alienLocations[0]);
    int alienY = Integer.parseInt(alienLocations[1]);

    int orionX = Integer.parseInt(orionLocations[0]);
    int orionY = Integer.parseInt(orionLocations[1]);

    int wizardX = Integer.parseInt(wizardLocations[0]);
    int wizardY = Integer.parseInt(wizardLocations[1]);

    addActor(alien, new Location(alienX, alienY), Location.NORTH);
    addActor(orion, new Location(orionX, orionY), Location.NORTH);
    addActor(wizard, new Location(wizardX, wizardY), Location.NORTH);

  }

  private void setupPacManLocations() {
    String[] pacManLocations = this.properties.getProperty("PacMan.location").split(",");
    int pacManX = Integer.parseInt(pacManLocations[0]);
    int pacManY = Integer.parseInt(pacManLocations[1]);
    addActor(pacActor, new Location(pacManX, pacManY));
  }

  private int countPillsAndItems() {
    int pillsAndItemsCount = 0;
    for (int y = 0; y < nbVertCells; y++)
    {
      for (int x = 0; x < nbHorzCells; x++)
      {
        Location location = new Location(x, y);
        int a = grid.getCell(location);
        if (a == 1 && propertyPillLocations.size() == 0) { // Pill
          pillsAndItemsCount++;
        } else if (a == 3 && propertyGoldLocations.size() == 0) { // Gold
          pillsAndItemsCount++;
        }
      }
    }
    if (propertyPillLocations.size() != 0) {
      pillsAndItemsCount += propertyPillLocations.size();
    }

    if (propertyGoldLocations.size() != 0) {
      pillsAndItemsCount += propertyGoldLocations.size();
    }

    return pillsAndItemsCount;
  }

  public ArrayList<Location> getPillAndItemLocations() {
    return pillAndItemLocations;
  }


  private void loadPillAndItemsLocations() {
    String pillsLocationString = properties.getProperty("Pills.location");
    if (pillsLocationString != null) {
      String[] singlePillLocationStrings = pillsLocationString.split(";");
      for (String singlePillLocationString: singlePillLocationStrings) {
        String[] locationStrings = singlePillLocationString.split(",");
        propertyPillLocations.add(new Location(Integer.parseInt(locationStrings[0]), Integer.parseInt(locationStrings[1])));
      }
    }

    String goldLocationString = properties.getProperty("Gold.location");
    if (goldLocationString != null) {
      String[] singleGoldLocationStrings = goldLocationString.split(";");
      for (String singleGoldLocationString: singleGoldLocationStrings) {
        String[] locationStrings = singleGoldLocationString.split(",");
        propertyGoldLocations.add(new Location(Integer.parseInt(locationStrings[0]), Integer.parseInt(locationStrings[1])));
      }
    }
  }
  private void setupPillAndItemsLocations() {
    for (int y = 0; y < nbVertCells; y++)
    {
      for (int x = 0; x < nbHorzCells; x++)
      {
        Location location = new Location(x, y);
        int a = grid.getCell(location);
        if (a == 1 && propertyPillLocations.size() == 0) {
          pillAndItemLocations.add(location);
        }
        if (a == 3 &&  propertyGoldLocations.size() == 0) {
          pillAndItemLocations.add(location);
        }
        if (a == 4) {
          pillAndItemLocations.add(location);
        }
      }
    }


    if (propertyPillLocations.size() > 0) {
      for (Location location : propertyPillLocations) {
        pillAndItemLocations.add(location);
      }
    }
    if (propertyGoldLocations.size() > 0) {
      for (Location location : propertyGoldLocations) {
        pillAndItemLocations.add(location);
      }
    }
  }

  private void drawGrid(GGBackground bg)
  {
    bg.clear(Color.gray);
    bg.setPaintColor(Color.white);
    for (int y = 0; y < nbVertCells; y++)
    {
      for (int x = 0; x < nbHorzCells; x++)
      {
        bg.setPaintColor(Color.white);
        Location location = new Location(x, y);
        int a = grid.getCell(location);
        if (a > 0)
          bg.fillCell(location, Color.lightGray);
        if (a == 1 && propertyPillLocations.size() == 0) { // Pill
          putPill(bg, location);
        } else if (a == 3 && propertyGoldLocations.size() == 0) { // Gold
          putGold(bg, location);
        } else if (a == 4) {
          putIce(bg, location);
        }
      }
    }

    for (Location location : propertyPillLocations) {
      putPill(bg, location);
    }

    for (Location location : propertyGoldLocations) {
      putGold(bg, location);
    }
  }

  private void putPill(GGBackground bg, Location location){
    bg.fillCircle(toPoint(location), 5);
  }

  private void putGold(GGBackground bg, Location location){
    bg.setPaintColor(Color.yellow);
    bg.fillCircle(toPoint(location), 5);
    Actor gold = new Actor("sprites/gold.png");
    this.goldPieces.add(gold);
    addActor(gold, location);
  }

  private void putIce(GGBackground bg, Location location){
    bg.setPaintColor(Color.blue);
    bg.fillCircle(toPoint(location), 5);
    Actor ice = new Actor("sprites/ice.png");
    this.iceCubes.add(ice);
    addActor(ice, location);
  }

  public void removeItem(String type,Location location){
    if(type.equals("gold")){
      for (Actor item : this.goldPieces){
        if (location.getX() == item.getLocation().getX() && location.getY() == item.getLocation().getY()) {
          item.hide();
        }
      }
    }else if(type.equals("ice")){
      for (Actor item : this.iceCubes){
        if (location.getX() == item.getLocation().getX() && location.getY() == item.getLocation().getY()) {
          item.hide();
        }
      }
    }
  }

  public int getNumHorzCells(){
    return this.nbHorzCells;
  }
  public int getNumVertCells(){
    return this.nbVertCells;
  }

  public void makeMonstersFurious() {
    for (Monster monster: monsters) {
      monster.getFurious(Monster.FURIOUS_TIME);
    }
  }
  public void makeMonstersFrozen() {
    for (Monster monster: monsters) {
      monster.getFrozen(Monster.FROZEN_TIME);
    }
  }
}
