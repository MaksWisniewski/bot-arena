from .soldier_rt import SoldierRT
from ..game_logic.objects.soldiers import _Soldier

class SoldierTracker:
    '''Soldier objects tracker.

    Track Soldier objects from game logic and
    synchronize them with real time soldier
    objects in the graphics module. 
    '''

    def __init__(self, path: list[tuple[int, int]]):
        SoldierRT.path = path
        self.soldiers_rt = {'left': {}, 'right': {}}
    
    def update_tracker(self, soldiers: dict[str, list[_Soldier]]) -> None:
        '''Update number and state of real time soldiers,
        based on soldiers from game logic.

        Impact on graphics state:
            - adding new real time soldiers
            - removing soldiers that should be dead
            - updating path position of real time soldiers
            - updating state of real time soldiers
        '''
        def spawn_new_soldiers(site_soldiers: list[_Soldier], side: str):
            for soldier in site_soldiers:
                if soldier.id not in self.soldiers_rt[side]:
                    self.soldiers_rt[side][soldier.id] = SoldierRT(
                        soldier.id,
                        soldier.position,
                        'swordsman',
                        side
                    )

        spawn_new_soldiers(soldiers['left'], 'left')
        spawn_new_soldiers(soldiers['right'], 'right')

        def remove_dead_soldiers(site_soldiers: list[_Soldier], side: str):
            ids_to_remove = []

            for id in self.soldiers_rt[side]:
                if id not in [s.id for s in site_soldiers]:
                    ids_to_remove.append(id)

            for id in ids_to_remove:
                self.soldiers_rt[side].pop(id)
                    
        remove_dead_soldiers(soldiers['left'], 'left')
        remove_dead_soldiers(soldiers['right'], 'right')

        LEFT_EXISTS = len(soldiers['left']) > 0
        RIGHT_EXISTS = len(soldiers['right']) > 0

        is_fight = LEFT_EXISTS and RIGHT_EXISTS and \
                    abs(soldiers['left'][0].position - soldiers['right'][0].position) <= 1
        
        if is_fight:
            self.soldiers_rt['left'][soldiers['left'][0].id].set_state('fight')
            self.soldiers_rt['right'][soldiers['right'][0].id].set_state('fight')
        else:
            if LEFT_EXISTS: self.soldiers_rt['left'][soldiers['left'][0].id].set_state('walk')
            if RIGHT_EXISTS: self.soldiers_rt['right'][soldiers['right'][0].id].set_state('walk')

        def update_soldiers_state(site_soldiers: list[_Soldier], side: str):
            for i in range(1, len(site_soldiers)):
                current_soldier = self.soldiers_rt[side][site_soldiers[i].id]
                previous_soldier = self.soldiers_rt[side][site_soldiers[i-1].id]
                position_difference = abs(site_soldiers[i].position - site_soldiers[i-1].position)

                if position_difference < 1 or (position_difference == 1 and previous_soldier.state != 'walk'):
                    current_soldier.set_state('idle')
                else:
                    current_soldier.set_state('walk')

            for soldier in site_soldiers:
                self.soldiers_rt[side][soldier.id].set_path_position(soldier.position)

        update_soldiers_state(soldiers['left'], 'left')
        update_soldiers_state(soldiers['right'], 'right')

    def update_soldiers(self) -> None:
        '''Updates every real time soldier.
        '''

        for soldier in self.get_soldiers():
            soldier.update()

    def get_soldiers(self) -> list[SoldierRT]:
        '''Real time soldiers getter.
        '''

        return list(self.soldiers_rt['left'].values()) + \
              list(self.soldiers_rt['right'].values())
    
    def __str__(self):
        return f'[{", ".join(str(soldier) for soldier in self.get_soldiers())}]'
