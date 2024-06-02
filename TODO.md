# Uwaga:
gra do konca nie jest turowa, a boty minmax, alpha_beta sztucznie robią z gry grę turową

# Inne warunki końca gry - na większych mapach gra między sensownymi botami się nie kończy

# MCTS - czy filtrowanie ruchów ma sens? (jeśli tak to czy jest dobrze napisane?)
trzeba inaczej filtrować farmy

# MCTS - lepsze losowanie w symulacjach
może też z filtrowaniem ruchów jak wyżej?

# stats.py
jak mocna powinna być wieża?

# eval
- dobrać odpowiednie wagi (eksperymentalnie)
- wagi zależne od parametrów gry, np.:
    - wieży od jej kosztu/ataku lub nawet ilości wież
    - farmy od jej koszty/obecnego dochodu/ilości golda
    - żołnierza od odległości od bazy/hp/rodzaju/kosztu/...
