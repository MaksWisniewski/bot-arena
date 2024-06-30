# How to use bots written in C++

## Install dependencies
Bots use [`json`](https://github.com/nlohmann/json) library for JSON parsing.
You can (probably) install it with your package manager:

- dnf (Fedora): `sudo dnf install json-devel`
- apt (Debian): `sudo apt install nlohmann-json3-dev`
- if you use other distro or package manager please find out how to install it and write it down here :)

## Compile bot
To compile bot `bot_name` use command: `make bot_name`. You can run `make all` to compile all bots.
If you add new bot, please add proper target to `Makefile`.

## Run bot
Compiled bots should by default be created in `bots` directory, if not you should move them there. Then the bots can be run in the same way as python bots.


# Project structure

## `common`

This directory contains the library providing common functionality that can be useful when creating C++ bots.
### `common/bot`

This module provides an abstract base class `Bot` which concrete bot classes can derive from.
It is very similar to [Python bot package](/bot_package/README.md), used to create bots in Python.
It implements communication with the arena and `run` method which controls bot's runtime. It provides 3 virtual methods that can be overriden to implement specific bot's logic:

```c++
class Bot
{
public:
    virtual void preprocess();
    virtual std::string make_move();
    virtual void post_move_action();

    void run();
    ...
};
```

- `preprocess`: here you can do some preprocessing, this method is called once before the game starts.
- `make_move`: this method should compute bot's next move and return it as `std::string`.
- `post_move_action`: here you can specify some additional actions that are performed after each move.

These methods by default do nothing (`make_move` returns `"W"` - an empty move), so you don't need to override them if your bot doesn't need them.

Example usage:

```c++
#include "common/bot/bot.hpp"

class MyBot : public Bot
{
public:
    std::string make_move() override
    {
        std::string move{"W"};

        // some computation

        return move;
    }
};

int main()
{
    for (;;)
    {
        MyBot bot;
        bot.run();
    }
}
```

## `common/engine`

This module provides `Engine` class that represents the game state and can be used to simulate the course of the game.
It is constructed with `Json` object representing game state in JSON format described [here](/bot_package/README.md#input-format-json-for-arena-properties).
To use this class please include `common/engine/engine.hpp` header.

```c++
class Engine
{
public:
    Engine(const Json&);

    void make_move(const std::string& left_move, const std::string& right_move);
    void undo_move();
    bool isWin() const;
    bool isWin(Side) const;

    std::vector<std::string> get_legal_moves(Side) const;

    std::vector<std::pair<int, int>> get_empty_cells() const;
    std::vector<std::pair<int, int>> get_path() const;
    std::pair<int, int> get_map_size() const;

    GameParameters get_game_parameters() const;

    std::vector<Building> get_farms(Side) const;
    std::vector<Building> get_turrets(Side) const;
    std::vector<Soldier> get_soldiers(Side) const;

    int get_gold(Side) const;
    int get_income(Side) const;
    ...
};
```

### Public methods
- `make_move`: updates game state and performs given player actions with respect to game rules.
- `undo_move`: currently not implemented.
- `isWin`: returns true if one of the players won, it has an overload checking if given player won - `isWin(Side)`.
- `get_legal_moves`: returns moves that can be performed by given player in current game state.
- `get_empty_cells`, `get_path`, `get_map_size`, `get_game_parameters`, `get_farms`, `get_turrets`, `get_soldiers`, `get_gold`, `get_income`: getters for game state with (hopefully) descriptive names.

## `common/engine/objects`
This directory contains structures representing game state which are used by `Engine` class.

### `Building`
Located in `building.hpp`, represents a building (farm or turret).
```c++
struct Building
{
    Building() = default;
    Building(const std::pair<int, int>&);

    operator std::pair<int, int>() const;

    std::pair<int, int> position;
};
```

### `GameParameters`
Located in `game_parameters.hpp`, represents parameters of the game. It is constructed from JSON in format of `arena.stats` [here](/bot_package/README.md#input-format-json-for-arena-properties).

#### Member types
- `SoldierParameters`: represents soldier parameters, constructed from JSON in format of `arena.stats.soldiers.{swordsman,archer}` [here](/bot_package/README.md#input-format-json-for-arena-properties).
    ```c++
    struct SoldierParameters
        {
            SoldierParameters() = default;
            SoldierParameters(const Json&);

            int max_hp;
            int damage;
            int range;
            int cost;
        };
    ```
- `FarmParameters`: represents farm parameters, constructed from JSON in format of `arena.stats.buildings.farm` [here](/bot_package/README.md#input-format-json-for-arena-properties).
    ```c++
    struct FarmParameters
    {
        FarmParameters(const Json&);

        int gold;
        int cost;
    };
    ```
- `TurretParameters`: represents turret parameters, constructed from JSON in format of `arena.stats.buildings.turret` [here](/bot_package/README.md#input-format-json-for-arena-properties).
    ```c++
    struct TurretParameters
    {
        TurretParameters(const Json&);

        int attack;
        int range;
        int cost;
    };
    ```

#### Fields
- `soldiers` of type `std::unordered_map<Soldier::Type, SoldierParameters>` - maps soldier type to its parameters.
- `farm` of type `GameParameters::FarmParameters` - farm parameters.
- `turret` of type `GameParameters::TurretParameters` - turret parameters.
- `passive_gold` of type `int` - basic amount of gold that is produced for each player each turn. The total player's income is `passive_gold` + number of farms * `farm.gold`.

### `Map`
Located in `map.hpp`, represents the map on which the game is played. It is constructed from JSON in format of `arena` [here](/bot_package/README.md#input-format-json-for-arena-properties) (only `path`, `obstacles` and `map_size` fields are needed).
```c++
struct Map
{
    Map(const Json&);

    std::pair<int, int> size;
    std::vector<std::pair<int, int>> path;
    std::vector<std::pair<int, int>> obstacles;
};
```
#### Fields
- `size`: the size of map, a pair (width, height).
- `path`: coordinates of cells lyihg on the path.
- `obstacles`: coordinates of cells containing obstacles.

### `Player`
Located in `player.hpp`, represents the player's state. It is constructed from JSON in format of `players.{left,right}` [here](/bot_package/README.md#input-format-json-for-arena-properties).
```c++
struct Player
{
    Player() = default;
    Player(const Json&);

    std::vector<Building> farms;
    std::vector<Building> turrets;
    std::vector<Soldier> soldiers;

    int gold;
    int income;
};
```
#### Fields
- `farms`: player's farms.
- `turrets`: player's turrets.
- `soldiers`: player's soldiers.
- `gold`: player's gold.
- `income`: player's income.

### `Soldier`
Located in `soldier.hpp`, represents the player's state.
It contains `Soldier::Type` enum representing type of a soldier.
```c++
enum class Type
    {
        swordsman,
        archer
    };
```
#### Constructors
`Soldier` can be constructed from JSON in format of single element of `players.{left,right}.units` [here](/bot_package/README.md#input-format-json-for-arena-properties).
There are two alternative constructors taking soldier's type (as `std::string` or `Soldier::Type`), hp and position.
```c++
Soldier() = default;
Soldier(const Json&);
Soldier(Type, int hp, int position);
Soldier(const std::string& type, int hp, int position);
```

#### Fields
- `type` of type `Soldier::Type` - soldier's type.
- `hp` of type `int` - soldiers hp.
- `position` of type `int` - soldiers position on a path, represented as distance (number of cells) from the corresponding base.
- `is_in_fight` of type `bool` - is set to `true` when the soldier participated in a fight in current turn.

The `soldier.hpp` header additionally provides two funtions for conversion between `std::string` and `Soldier::Type`.
```c++
Soldier::Type string_to_soldier_type(const std::string&);
std::string soldier_type_to_string(Soldier::Type);
```

## `common/eval_func`
This module contains functions that can be used to evaluate game state. In `eval.hpp` header file it provides `Eval` - an abstract base class that can be extended to implement a concrete evaluation function.
```c++
class Eval
{
public:
    using Type = int64_t;
    virtual Type operator() (const Engine&, Side) const = 0;
};
```
The `()` operator takes `Engine` representing current game state and `Side` representing the player for whom the game is evaluated.

This header also contains a few example implementations of evaluation function:
`Eval_1`, `Eval_2`, `BetterEval`.

## `common/optimizations`
This module contains functions that can be used to optimize bots' performance.
Currently only one optimization is implemented: `is_useless`.

### `common/optimizations/is_useless`
```c++
using Path = std::unordered_set<std::pair<int, int>, pair_hash>;

bool is_useless(const std::string&, const Path&);
```
A heuristic function that checks if given move is *uselses* i.e. if it is worth considering.
The move is considered useless if it is placing a turret in a cell not adjacent to the path.
If the move is placing a farm then it is classified as useless with some probability dependent on path length (it is a heuristic, feel free to improve it).

## `common/json.hpp`
Provides an abstraction for JSON
```c++
using Json = nlohmann::json;
```

## `common/read_json.hpp`
Provides function to read JSON file
```c++
std::optional<Json> read_json(const std::string& file_name);
```

## `common/side.hpp`
Provides an abstraction to represent player's side
```c++
enum class Side : bool
{
    left,
    right
};

Side other_side(Side);
Side string_to_side(const std::string&);
std::string side_to_string(Side);
```
