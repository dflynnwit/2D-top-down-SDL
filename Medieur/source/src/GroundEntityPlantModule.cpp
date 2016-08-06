#include "GroundEntityPlantModule.h"

#include <iostream>

#include "GroundEntity.h"
#include "Tile.h"
#include "Job.h"
#include "World.h"

#include "Prototypes.h" // TODO: remove this

namespace {
	const int kMaxGrowth = 5;
	const int kMaxHealth = 10;
	const int kGrowthSteps = 100;
	const int kHealthDropSteps = 300;

	const int kHarvestAmount = 3;
}

GroundEntityPlantModule::GroundEntityPlantModule(
	GroundEntityPlantModule * pPrototype, GroundEntity * pThisEntity)
	:
	IGroundEntityModule(pThisEntity),
	mHealth(pPrototype->mHealth), mGrowth(pPrototype->mGrowth), mDropItemId(pPrototype->mDropItemId),
	mHealthCounter(kHealthDropSteps), mGrowthCounter(kGrowthSteps)
{
}

GroundEntityPlantModule::GroundEntityPlantModule(const int pDropItemId)
	:
	IGroundEntityModule(nullptr),
	mHealth(kMaxHealth), mGrowth(1), mDropItemId(pDropItemId),
	mHealthCounter(kHealthDropSteps), mGrowthCounter(kGrowthSteps)
{}

void GroundEntityPlantModule::update()
{
	mHealthCounter.step();
	mGrowthCounter.step();

	if (mHealthCounter.expired()) {
		mHealth--;
		mHealthCounter.pause(); // Remove
		if (mHealth == 5) {
			printf("Come interact!\n");
			Job* job = new Job(mThisEntity->getTile(), JobType::INTERACT);
			mThisEntity->getTile()->getWorld()->createJob(job);
		}
		else if (mHealth <= 0) {
			rot();
		}
		mHealthCounter.reset();
	}
	if (mGrowthCounter.expired()) {
		mGrowth++;
		if (mGrowth >= kMaxGrowth - 1) {
			Job* job = new Job(mThisEntity->getTile(), JobType::PICKUP);
			mThisEntity->getTile()->getWorld()->createJob(job);
			mGrowthCounter.pause(); // remove
		} else if (mGrowth > kMaxGrowth + 2) {
			rot();
		}
		mGrowthCounter.reset();
	}

}

void GroundEntityPlantModule::interact(Character* pCharacter)
{
	printf("Interact: ");
	printf("x: %i, y: %i\n", mThisEntity->getTile()->getX(), mThisEntity->getTile()->getY());
	mHealth = kMaxHealth;
	mHealthCounter.reset();
}

void GroundEntityPlantModule::pickup(Character* pCharacter)
{
	//if (mGrowth >= kMaxGrowth - 1) {
		printf("Pickup item!\n");
		PickableItem* item = mThisEntity->getTile()->getWorld()->createPickableItem(mDropItemId, kHarvestAmount);
		
		pCharacter->addItem(item);
		mThisEntity->erase();
	/*}
	else {
		rot();
	}*/
}

void GroundEntityPlantModule::rot()
{
	mThisEntity->erase();
}