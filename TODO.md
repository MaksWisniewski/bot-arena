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

# Wizualizacja w czasie rzeczywistym podczas symulacji (?)

# Dokumentacja
- [x] linki do poprzedniego repo (przed forkiem) należy uaktualnić
- [x] bot_package/README.md – dodać opisy nowych pól struktury arena_properties
- [x] pełny opis parametrów wywołania RUN.sh
- [x] opis formatu pliku z definicją ekonomii
- [x] opis formatu pliku definiującego planszę
- [ ] opis formatu interfejsu bot-arena
- w głównym readme.md:
    - [ ] dodałbym jakiś wstęp, o co chodzi w tym projekcie
    - [x] „Script RUN.sh Usage” przesunąłbym do rozdziału „Activation” p.3 „use bash script RUN.sh”, tytuł rozdziału zmieniłbym na „How to launch”
    - [x] dodałbym link do \cpp_bots\README.md w rozdziale „Bot standard”
- [x] poprawiłbym linki względne do podrzędnych .md, nie działają w przeglądarce, ale nie wiem, czy .md ma taką możliwość
