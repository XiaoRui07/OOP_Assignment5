---
config:
  layout: elk
---
classDiagram
    class GameObject {
        <<abstract>>
        +Vector2 position
        +float rotation
        +Color color
        +Draw() void
        +Update() void
    }
    
    class Ammo {
        -int damage
        -float ammospeed
        +Ammo(Vector2, float, int, float)
        +Update() void
        +Draw() void
        +TouchBound() bool
    }
    
    class Weapon {
        <<abstract>>
        #int maxammo
        #int damage
        #int currentammo
        #float ammospeed
        #float firerate
        #float curtime
        +Update() void
        +Fire(Ammomanager*, Vector2, float) void
        +reloadsuccess() void
    }
    
    class Pistol {
        +Pistol()
        +Draw() void
    }
    
    class Rifle {
        +Rifle()
        +Draw() void
    }
    
    class Shotgun {
        +Shotgun()
        +Draw() void
    }
    
    class Sniper {
        +Sniper()
        +Draw() void
    }
    
    class Ammomanager {
        -std::set~Ammo*~ ammos
        +newammo(Ammo*) void
        +Update() void
        +Draw() void
    }
    
    class GameObjectFactory {
        +CreateWeapon(int) Weapon*$
    }
    
    class Player {
        -float RELOAD_TIME
        -float speed
        -float reloadTimer
        -bool isReloading
        -bool canshoot
        -int currentWeapon
        -std::vector~Weapon*~ gun
        +Player()
        +Update() void
        +Update(Ammomanager*) void
        +Draw() void
    }
    
    GameObject <|-- Ammo
    GameObject <|-- Weapon
    GameObject <|-- Player
    Weapon <|-- Pistol
    Weapon <|-- Rifle
    Weapon <|-- Shotgun
    Weapon <|-- Sniper
    Ammomanager o-- Ammo:ammos
    Player *-- Weapon:gun
    Player ..> Ammomanager:uses
    GameObjectFactory ..> Weapon:create
