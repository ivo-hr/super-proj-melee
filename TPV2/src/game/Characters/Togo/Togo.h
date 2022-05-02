#include "../../Character.h"

class DinoShield;

class Togo : public Character
{
public:
	Togo(FightManager* mngr, b2Vec2 pos, char input,int p);
	~Togo();

	virtual void BasicNeutral(int frameNumber) override;
	virtual void BasicForward(int frameNumber) override;
	virtual void BasicUpward(int frameNumber) override;
	virtual void BasicDownward(int frameNumber) override;

	virtual void SpecialNeutral(int frameNumber) override;
	virtual void SpecialForward(int frameNumber) override;
	virtual void SpecialUpward(int frameNumber) override;
	virtual void SpecialDownward(int frameNumber) override;

	virtual bool GetHit(attackData a, Entity* attacker);
	virtual void update() override;
	virtual void SetSpear(bool spear);
	void SpecialLHit(int frameNumber);
	virtual void drawHUD(int w, int h, int numOfPlayer, int screenadjust)override;
private:
	bool lanza = true;
	float spearTime;
	int attackBase;
	float attackMul;
	DinoShield* dShield = nullptr;
	SDL_Rect bite;
};