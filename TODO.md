# Uwaga:
gra do konca nie jest turowa, a boty minmax, alpha_beta sztucznie robią z gry grę turową

# minmax bierze czas?

# dokumentacja xddd?

# MCTS - multithreading + optymalizacja pozwalająca zagrać jakkolwiek na mapie 15x15

# MINMAX - spamietywanie stanów + iterative deepening z estymacją czasu (bez sprawdzania czasu ID nie ma sensu)

# Inne warunki końca gry - na większych mapach gra między sensownymi botami się nie kończy

# timeouty są zepsute
- silnik w pytonie jest tak wolny, że podane timeouty nie są zgodne z prawdą (sam silnik zjada sporą ich część)
przez co bot timeoutuje, próbując maksymalnie wykorzystać dany mu czas
-> mcts nie jest w stanie grać na mapie 15x15, na 10x10 śmiga
- najbardziej zepsuty jest ready timeout
- coś sie psuje z tym jak sie puszcza więcej gier oraz jak sie da move timeout wiekszy niż 1 (xd)
