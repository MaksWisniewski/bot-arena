
# to dobrze dziala

```
python main.py run-simulation --b1 mcts.py --b2 alpha_beta.py --map map.json --games 10
```

# ale to juz nie dziala dobrze



```
python main.py run-simulation --b1 minmax.py --b2 alpha_beta.py --map map.json --games 10
```

- boty robia ten sam ruch, TODO: upewnic sie co robi silnik kiedy oba boty robia ten sam ruch
- hipoteza: nic sie nie dzieje wowczas, mozna dodac losowanie kto robi ruch albo cos innego
- Uwaga: gra do konca nie jest turowa, a boty minmax, alpha_beta sztucznie robią z gry grę turową

# timeouty sa skopane

# ficzery (bugi) w silniku

- kolejność akcji w `Game.update` inna niż opisana w README.md
- funkcja `Soldiers.fight` napisana po chinsku
- funkcja `Soldiers.move` nie sprawdza czy na polu stoi pionek przeciwnika - dwa pionki mogą stać na jednym polu
- powyższe raz występuje a raz nie :D


# engine::isWin()

# minmax bierze czas?

# dokumentacja xddd?

# 3 minmax,cpp dziala wolno?