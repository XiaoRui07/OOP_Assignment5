//繪圖設計部分大部分參考Gemini
//__________________________________
#include "raylib.h"
#include "GameObject.h"
#include "raymath.h"
#include "rlgl.h"
#include <vector>
#include <set>
#include <cmath>

class Ammo:public GameObject{
public:
        Ammo(Vector2 startPos, float angleDegrees, int dmg, float amosped) {
        this->position = startPos;
        this->rotation = angleDegrees * DEG2RAD; 
        this->damage = dmg;
        this->ammospeed = amosped;
    }
	void Update(){
		float dt = GetFrameTime();
        	this->position.x += cosf(this->rotation) * ammospeed * dt;
        	this->position.y += sinf(this->rotation) * ammospeed * dt;
	}
	void Draw(){
		DrawCircleV(this->position, 4, YELLOW);
	}
	bool TouchBound(){
	    if (position.x < 0 || position.x > GetScreenWidth() || position.y < 0 || position.y > GetScreenHeight()) {
            return true;
        	}
            return false;
	}
        ~Ammo(){}
private:
        int damage;
        float ammospeed;
};
class Ammomanager{
	private:
		std::set<Ammo*>ammos;
	public:
		void newammo(Ammo* amo){
			ammos.insert(amo);
		}
		void Update(){
			for(auto it=ammos.begin();it!=ammos.end();){
				(*it)->Update();
				if((*it)->TouchBound()){
					delete *it;
					it=ammos.erase(it);

				}else{
				it++;
				}
			}
		}
		void Draw(){
			for(Ammo* it:ammos){
				it->Draw();
			}
		}
		~Ammomanager(){
			for(Ammo* it:ammos){
				delete it;
			}
			ammos.clear();
		}

};
class Weapon:public GameObject{
        protected:
                int maxammo;
                int damage;
                int currentammo;
                float ammospeed;
                float firerate;
                float curtime;
        public:
		void Update(){
			if (curtime > 0.0f) {
        	    curtime -= GetFrameTime();
        	}
		}
                void Fire(Ammomanager* manager, Vector2 spawnPos, float angleDegrees){
                        if(currentammo==0){return;}
                        if(curtime<=0.0f){
                                Ammo* amo=new Ammo(spawnPos, angleDegrees,damage,ammospeed);
				manager->newammo(amo);
				currentammo--;
				curtime=firerate;
                        }
                }
		void reloadsuccess(){
			currentammo=maxammo;
		}
                ~Weapon(){};
};
class Pistol:public Weapon{
public:
        Pistol(){
                maxammo=15;
                damage=20;
                currentammo=15;
                ammospeed=150.0;
                firerate=0.5;
                curtime=0.5;
        }
        void Draw(){}

};
class Riffle:public Weapon{
public:
        Riffle(){
                maxammo=40;
                damage=25;
                currentammo=40;
                ammospeed=200.0;
                firerate=0.2;
                curtime=0.2;
        }
        void Draw(){}
};
class Shotgun:public Weapon{
public:
        Shotgun(){
                maxammo=8;
		currentammo=8;
                damage=100;
                ammospeed=200.0;
                firerate=1.0;
                curtime=1.0;
        }
        void Draw(){}
};
class Sniper:public Weapon{
public:
        Sniper(){
                maxammo=5;
		currentammo=5;
                damage=50;
                ammospeed=400.0;
                firerate=1.75;
                curtime=1.75;
        }
        void Draw(){}
};
class GameObjectFactory {
public:
    static Weapon* CreateWeapon(int weaponType) {
        switch (weaponType) {
            case 0: return new Pistol();
            case 1: return new Riffle();
            case 2: return new Shotgun();
            case 3: return new Sniper();
        }
    }
    //還沒處理enemy，以後會加。
};
class Player:public GameObject{
private:
	const float RELOAD_TIME=2.0;
	float speed=100.1;
	float reloadTimer=0.0;
	bool isReloading=false;
	bool canshoot=true;
	int currentWeapon=0;
	// 定義火柴人的比例
    	float bodyLength = 40.0f;
    	float headRadius = 10.0f;
    	float armLength = 25.0f;
    	float legLength = 30.0f;
	std::vector<Weapon*>gun;
public:
	Player(){
		this->position = { (float)GetScreenWidth()/2, (float)GetScreenHeight()/2 };
        	this->color = WHITE; // 火柴人的顏色
        	this->rotation = 0;
		gun.push_back(GameObjectFactory::CreateWeapon(0));
		gun.push_back(GameObjectFactory::CreateWeapon(1));
		gun.push_back(GameObjectFactory::CreateWeapon(2));
		gun.push_back(GameObjectFactory::CreateWeapon(3));
	}
	void Update(){}
	void Update(Ammomanager* manager){
		float dt = GetFrameTime();
		gun[currentWeapon]->Update();
    		// 2. 高度鎖定 (目前先固定在中下方，例如 500 像素高。以後會改成 position.y = GetTerrainHeight(position.x);)
    		position.y = 500.0f; 

    		// 3. 計算瞄準角度
    		Vector2 mousePos = GetMousePosition();
    		Vector2 direction = Vector2Subtract(mousePos, this->position);
    		this->rotation = atan2f(direction.y, direction.x) * RAD2DEG; // 這是槍要轉的角度
		if(IsKeyPressed(KEY_R)&&!isReloading){
			isReloading=true;
			canshoot=false;
			reloadTimer=RELOAD_TIME;
			return;
		}
		if(isReloading){
			reloadTimer-=GetFrameTime();
			if(reloadTimer<=0.0){
				reloadTimer=0.0;
				isReloading=false;
				gun[currentWeapon]->reloadsuccess();
				canshoot=true;
			}
			return;
		}
		if(!isReloading){
			if(IsKeyDown(KEY_D))position.x+=speed*dt;
                	if(IsKeyDown(KEY_A))position.x-=speed*dt;
			if(IsKeyPressed(KEY_LEFT)){
				if(currentWeapon!=0){
					currentWeapon-=1;

				}
			}
			else if(IsKeyPressed(KEY_RIGHT)){
				if(currentWeapon!=3){
					currentWeapon+=1;
				}
			}
			if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
				gun[currentWeapon]->Fire(manager, this->position, this->rotation);
			}
		}
	}
	void Draw() {
		float thick = 3.0f;
    
    		// 判斷滑鼠在左邊還是右邊，決定翻轉係數 (flip = 1 或 -1)
    		float flip = (GetMousePosition().x >= position.x) ? 1.0f : -1.0f;

    		// 1. 畫不需要旋轉的身體與頭腳 (使用相對座標，以 position 為骨盆點)
    		// 所有的 X 座標都要乘以 flip，這樣滑鼠到左邊時，火柴人就會自動轉頭過去
    		Vector2 pelvis = position;
    		Vector2 neck = { pelvis.x, pelvis.y - bodyLength };
    		Vector2 head = { neck.x, neck.y - headRadius };
    
    		// 畫身體 (直立的)
    		DrawLineEx(neck, pelvis, thick, color);
    		// 畫頭
    		DrawCircleLinesV(head, headRadius, color);
    		// 畫腳 (往左右開叉，乘以 flip 確保方向正確)
    		DrawLineEx(pelvis, { pelvis.x - 10 * flip, pelvis.y + legLength }, thick, color);
    		DrawLineEx(pelvis, { pelvis.x + 10 * flip, pelvis.y + legLength }, thick, color);

    		// 2. 畫會旋轉的手臂與槍枝 (只讓這部分進入矩陣旋轉)
    		rlPushMatrix();
    		rlTranslatef(neck.x, neck.y, 0.0f); // 移到「肩膀/頸部」作為旋轉中心
    		rlRotatef(this->rotation, 0.0f, 0.0f, 1.0f); // 讓手和槍對準滑鼠角度

    		// 現在 (0,0) 是肩膀，且右邊 (X正向) 就是滑鼠的方向
    		// 畫手臂 (伸向滑鼠)
    		DrawLineEx({ 0, 0 }, { armLength, 0 }, thick, color);

    		// 畫武器 (貼在手上)
    		Color wColor;
    		Rectangle gunRec = { 10, -3, 30, 6 }; // 讓槍身在手的前端
		switch(currentWeapon){
			case 0:
				wColor=RED;
				DrawRectangleRec(gunRec, wColor);
				break;
			case 1:
				wColor=BLUE;
				 DrawRectangleRec(gunRec, wColor);
				break;
			case 2:
				wColor=GRAY;
				 DrawRectangleRec(gunRec, wColor);
				break;
			case 3:
				wColor=GREEN;
				 DrawRectangleRec(gunRec, wColor);
				break;
		}
		rlPopMatrix(); // 恢復畫布
    		if (isReloading) {
        		float progress = (RELOAD_TIME - reloadTimer) / RELOAD_TIME;
        		DrawRectangle(position.x - 20, position.y - 40, 40, 5, GRAY);      // 背景
        		DrawRectangle(position.x - 20, position.y - 40, 40 * progress, 5, RED); // 紅色進度
        		DrawText("RELOADING...", position.x - 30, position.y - 60, 10, RED);
 	   	}
	}
	~Player(){}

};

int main(void) {
	enum GameState {
    STATE_MENU,     
    STATE_GAMEPLAY, 
    STATE_GAMEOVER  
	};
	GameState currentState=STATE_MENU;
    const int baseWidth = 1280;
    const int baseHeight = 720;
    Rectangle startButton = { 300, 200, 200, 50 };
    InitWindow(baseWidth, baseHeight, "raylib [WSL] - 2D CS");
    SetWindowSize(GetMonitorWidth(0), GetMonitorHeight(0));
    ToggleFullscreen();
    SetTargetFPS(60);
    Player player;
    Ammomanager ammomanager;
    while (!WindowShouldClose()) {
        switch(currentState){
		case STATE_MENU:
			if(CheckCollisionPointRec(GetMousePosition(),startButton)){
					if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
								currentState=STATE_GAMEPLAY;
					}
			}
			break;
		case STATE_GAMEPLAY:
			player.Update(&ammomanager);
			ammomanager.Update();
			break;
		case STATE_GAMEOVER:
			break;
	}
	BeginDrawing();
	ClearBackground(BLACK);
	switch (currentState) {
        case STATE_MENU:{
            // 畫出開始畫面 UI
            DrawText("MY SHOOTING GAME", GetScreenWidth()/2 - 150, 200, 30, WHITE);

            // 畫按鈕 (滑鼠移上去變色)
            Color btnColor = CheckCollisionPointRec(GetMousePosition(), startButton) ? GRAY : DARKGRAY;
            DrawRectangleRec(startButton, btnColor);
            DrawText("START GAME", startButton.x + 40, startButton.y + 15, 20, WHITE);
            break;
			}
        case STATE_GAMEPLAY:
		player.Draw();
		ammomanager.Draw();
            // 畫出正式遊戲內容 (Player->Draw, BulletManager->Draw)
            //DrawGameContent();
            break;
	case STATE_GAMEOVER:
	    break;
    }
    EndDrawing();
    }

    CloseWindow();
    return 0;
}
