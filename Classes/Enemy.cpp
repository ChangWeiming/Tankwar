#include "Enemy.h"
#include "EnemyTank.h"
#include "Constant.h"
#include <algorithm>
#include <vector>
#include "Tank.h"

using namespace cocos2d;

Enemy::Enemy()
{
}

Enemy::~Enemy() {

}
void Enemy::setEnemyNums(int temp) {
	enemyNums = temp;
}

void Enemy::setMissileUsed(Missile* missile) {
	for (size_t i = 0; i < enemyMissile.size();i++) {
		MissileState& it = enemyMissile[i];
		if (it.pMissile == missile) {
			it.used = false;
			return;
		}
	}
	assert(false);
	log("Debug:remove enemyMissile Error");
	return;
}

void Enemy::setMap(TileMap* mp) {
	map = mp;
}

bool Enemy::isOver() {
	return enemyNums == 0 && tank->lastObject() == nullptr;
}

using namespace std;

vector<Rect> Enemy::getTankAlivePos() {

	vector<Rect> ans;
	Ref* pRef;
	int number = tank->count();
	CCARRAY_FOREACH(tank,pRef){
		auto pObj = (EnemyTank*)pRef;
		ans.push_back(pObj->getRect());
	}
	return ans;

}


bool Enemy::born(double dt) {
	static double rebornGap = 2.1;
	rebornGap += dt;

	if (rebornGap < 2.0) {
		return false;
	}

	int now = tank->count();

	if (now >= 4) {
		return false;
	}

	if (now == 0 && enemyNums >= 3) {
		enemyNums -= 3;
		for (int i = 0; i < 3; i++) {
			auto pRet = EnemyTank::createWithType(TankState::Fast);
			auto pMissile = Missile::createWithType(TankState::EnemyTank);
			pRet->born(startPoint[i]);
			pRet->setPlayerTank(playerTank);
			pRet->setEnemyTanksArray(tank);
			pRet->setMissile(this->getUnusedMissile());
			pRet->setMap(map);
			tank->addObject(pRet);
			this->addChild(pRet);
		}
		rebornGap = 0;
		return true;
	}

	if (enemyNums > 0) {
		enemyNums--;
		int pos = rand() % 3;
		auto pRet = EnemyTank::createWithType(TankState::Normal);
		auto pMissile = Missile::createWithType(TankState::EnemyTank);
		pRet->setTankPosition(startPoint[pos]);
		pRet->setPlayerTank(playerTank);
		pRet->setMap(map);
		pRet->setMissile(this->getUnusedMissile());
		while (playerTank->getRect().intersectsRect(pRet->getRect())) {
			pos++;
			pos %= 3;
			pRet->setTankPosition(startPoint[pos]);
		}
		pRet->born(startPoint[pos]);
		pRet->setEnemyTanksArray(tank);
		tank->addObject(pRet);
		this->addChild(pRet);
		rebornGap = 0;
		return true;
	}
	else return false;

}

Missile* Enemy::getUnusedMissile() {
	for (size_t i = 0; i < enemyMissile.size(); i++) {
		MissileState& it = enemyMissile[i];
		if (it.used == false) {
			it.used = true;
			return it.pMissile;
		}
	}
	log("Debug:error occurs");
	assert(false);
	return nullptr;
}

bool Enemy::init() {
	if (!Layer::init()) {
		return false;
	}

	enemyDestoryed = 0;

	tank = Array::createWithCapacity(4);
	tank->retain();

	for (int i = 0; i < 4; i++) {
		auto pMissile = Missile::createWithType(TankState::EnemyTank);
		enemyMissile.push_back(MissileState(pMissile, false));
		this->addChild(pMissile, 10);
	}

	auto enemySize = Size(60,60);

	startPoint[0].y = startPoint[1].y = startPoint[2].y = gameSize.y - enemySize.height / 2;

	startPoint[0].x = enemySize.width / 2;
	startPoint[1].x = gameSize.x / 2;
	startPoint[2].x = gameSize.x - enemySize.width / 2;

	_scheduler->scheduleUpdate(this, 1, false);
	return true;
}

Enemy* Enemy::createWithEnemyNums(int temp) {
	Enemy* pRet = new(std::nothrow)Enemy();
	pRet->setEnemyNums(temp);
	if (pRet&&pRet->init()) {
		pRet->autorelease();
		return pRet;
	}
	else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}



void Enemy::setPlayerTankPointer(Tank* tank) {
	playerTank = tank;
	playerTank->retain();
}

void Enemy::update(float dt) {
	Layer::update(dt);
	born(dt);
	Ref* pRef;

	//move enemy and enemy's missile
	CCARRAY_FOREACH(tank,pRef){
		auto  pObj = (EnemyTank*)pRef;
		pObj->getNextState(dt);
	}

	for (size_t i = 0; i < enemyMissile.size(); i++) {
		if (enemyMissile[i].pMissile->isVisible()) {
			enemyMissile[i].pMissile->getNextState(dt);
		}
	}

	//player's missile with enemy's missile
	auto playerMissileRect = playerTank->getMissileRect();

	for (size_t i = 0; i < enemyMissile.size(); i++) {
		if (enemyMissile[i].used == false||
			enemyMissile[i].pMissile->isVisible() == false||
			enemyMissile[i].pMissile->getBoomingState() == true) continue;
		
		auto enemyMissileRect = enemyMissile[i].pMissile->getRect();
		if (playerMissileRect.intersectsRect(enemyMissileRect)) {
			enemyMissile[i].pMissile->setVisible(false);
			playerTank->setMissileInvisible();
		}
		
	}

	//Player's missile with enemy
	bool isBooming = playerTank->getMissileBoomingState();
	
	CCARRAY_FOREACH(tank, pRef) {
		if (isBooming) break;
		auto pObj = (EnemyTank*)pRef;
		if (!pObj->isVisible()) continue;
		auto enemyRect = pObj->getRect();
		if (enemyRect.intersectsRect(playerMissileRect)) {
			pObj->goBoom();
			playerTank->setMissileInvisible();
			break;
		}
	}

	//enemy's missile with player's tank
	auto playerRect = playerTank->getRect();
	for (size_t i = 0; i < enemyMissile.size(); i++) {
		if (enemyMissile[i].used == false||
			enemyMissile[i].pMissile->isVisible() == false) continue;
		auto enemyMissileRect = enemyMissile[i].pMissile->getRect();
		
		if (enemyMissileRect.intersectsRect(playerRect)) {
			if(playerTank->getBorningStatus() == false)
				playerTank->goBoom();
			enemyMissile[i].pMissile->setVisible(false);
		}

	}

	//remove invisible enemy
	CCARRAY_FOREACH(tank, pRef) {
		auto pObj = (EnemyTank*)pRef;
		if (pObj->isVisible() == false) {
			auto missile = pObj->getMissile();
			if (missile->isVisible()) {
				continue;
			}
			this->setMissileUsed(missile);
			tank->removeObject(pObj);
			enemyDestoryed++;
		}
	}
}

int Enemy::getDestoryedNum(){
	auto _ = enemyDestoryed;
	enemyDestoryed = 0;
	return _;
}

bool Enemy::collisionTest(const Rect&) {
	//Enemy with player
	return true;
}
