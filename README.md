# Projects
navigate to pacman/src/Drive to run the game

1. The player opens the game which initiates and places PacMan, pills, ice cubes, gold pieces, and
monsters (one Troll and one T-X5) into the map. The initial locations of PacMan and monsters are
configurable.
2. The game moves the monsters automatically within the maze. A monster will move one cell at a
time. A monster determines a next location to which it will move based on a given direction. 

Each type of monsters has the following walking approach.
Monster Name Walking Approach
* Troll
Random Walker: Troll will randomly turn left or right (i.e., -90 or
+90 from the current direction). If it hits the maze wall, it will turn
back to the original direction and go forward or turn the other side.
Otherwise, go backward.
* T-X5
Aggressive Follower: T-X5 walks towards PacMan if it can. It
determines the compass direction from itself to PacMan. Then, it will
move towards that direction. If the next location is a maze wall or has
been visited, it will randomly walk like Troll. When the game starts, T-
X5 will wait (no moving) for 5 seconds before start moving.
3. The player controls PacMan with keyboard actions to walk in the maze. The Left and right keys move
PacMan horizontally. The Up and down keys move PacMan vertically.
Repeat steps 2-3
Alternate Scenarios

### 2a.,3a 
If there is a collision between PacMan and one of the monsters (i.e., being on the same location), PacMan explodes and the game is over.
### 3b 
When PacMan eats a pill (being on the pill’s location), the pill disappears and the current score on the title bar increases by 1 point.

### 3c 
When PacMan eats a gold piece (being on the gold piece’s location), the gold piece disappears and
the current score on the title bar increases by 5 points.
### 3d
When PacMan eats an ice cube (being on the ice cube’s location), the ice cube disappears but the
current score doesn’t increase.
### 3e
When PacMan eats all the pills and gold pieces, the player wins the game.
### 2b Feature 1
Three new kinds of monsters are created and placed into the map. Their initial locations are specified in the configuration file. A monster will move one cell at a time. Each type of monsters has
the following walking approach.
* Orion
Gold Surveillance: Instead of chasing PacMan, Orion wants to protect
gold pieces. Orion will walk through every gold location by selecting
one gold piece at random, walking to it, and so on. When all gold
pieces have been visited, Orion will start over again by randomly
selecting any gold locations. Even though the gold pieces are eaten by
PacMan, Orion still will visit those locations. However, Orion will
favour locations which still have gold pieces, that is, it will select from
locations with gold pieces before other locations where the gold
pieces have been eaten by PacMan. Once a gold location has been
selected, Orion will visit it, even if the gold piece in that location is
eaten while Orion is on the way there.
* Alien
Shortest Distance Finder: Unlike T-X5, Alien will calculate the distance
between PacMan and each of the neighbour locations (8 cells around
the Alien) that it can move (i.e., not the maze wall). Then, it will move
to the neighbour location that has the shortest distance to PacMan. If
more than one neighbour locations have the shortest distance, it will
randomly select one.
* Wizard
Wall-Through Walker: Wizard has a magic to walk through a 1-cell-
thick wall. Wizard randomly select one of its neighbour locations (8
cells around the Wizard).
• If the selected location is not a maze wall, Wizard will move to
that location.
• If the selected location is a maze wall, it will check if the adjacent
location in the same direction as the selected location is a wall or
not:
o If the adjacent location is not a wall, the Wizard walk
through the wall to that adjacent location.
o If the adjacent location is a wall, Wizard will randomly select
another neighbour location around itself.

### Feature 2: Additional Capabilities of Items
Extends Playing PacMan in the Multiverse
### 3c. 
When PacMan eats a gold piece, all the monsters get furious and move fasters. The monsters
determine the moving direction once based on their walking approach and move towards that
direction for 2 cells if they can. Otherwise, determining the new direction again using their own
walking approach until it can move by 2 cells. After 3 seconds, all the monsters will be back to move
normally using their own walking approach.
### 3d. When PacMan eats an ice cube,
#### 3d.1. 
Regardless of being normal or furious, all the monsters are frozen (i.e., stop moving) for 3
seconds. Then, they will be back to move normally using their own walking approach.
#### 3d.2. 
While the monsters are frozen, PacMan can eat a gold piece without making the monsters
furious.